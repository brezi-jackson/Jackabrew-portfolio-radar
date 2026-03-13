import math
import os
import re
from datetime import datetime, timezone
from pathlib import Path

import pandas as pd
import plotly.express as px
import requests
import streamlit as st
import yfinance as yf

YF_HEADERS = {"User-Agent": "Mozilla/5.0"}

BASE_CURRENCY = "USD"
APP_PASSWORD = os.environ.get("APP_PASSWORD") or st.secrets.get("APP_PASSWORD", "")
HOLDINGS_PATH = Path(__file__).resolve().parent.parent / "holdings.csv"
WATCHLIST_PATH = Path(__file__).resolve().parent.parent / "watchlist.csv"

MARKET_SUFFIX = {
    "HK": ".HK",
    "US": "",
    "JP": "",
    "Crypto": "-USD",
}
MARKET_CURRENCY = {
    "HK": "HKD",
    "US": "USD",
    "JP": "JPY",
    "Crypto": "USD",
}
FX_SYMBOLS = {
    "HKD": "HKDUSD=X",
    "JPY": "JPYUSD=X",
    "CNH": "CNHUSD=X",
    "USD": None,
}


RANGE_CHOICES = {
    "1D": {"range": "5d", "interval": "30m"},
    "7D": {"range": "7d", "interval": "90m"},
    "1M": {"range": "1mo", "interval": "1d"},
    "1Y": {"range": "1y", "interval": "1wk"},
}


@st.cache_data(ttl=900)
def fetch_chart_series(symbol: str, span: str = "1mo", interval: str = "1d"):
    try:
        resp = requests.get(
            f"https://query1.finance.yahoo.com/v8/finance/chart/{symbol}",
            params={"range": span, "interval": interval},
            headers=YF_HEADERS,
            timeout=10,
        )
        resp.raise_for_status()
        payload = resp.json().get("chart", {}).get("result", [])
        if not payload:
            return None, None
        result = payload[0]
        timestamps = result.get("timestamp") or []
        closes = result.get("indicators", {}).get("quote", [{}])[0].get("close", [])
        if not timestamps or not closes:
            return None, None
        idx = pd.to_datetime(timestamps, unit="s", utc=True)
        series = pd.Series(closes, index=idx).dropna()
        currency = result.get("meta", {}).get("currency")
        return series, currency
    except Exception:
        return None, None


def enforce_password():
    if not APP_PASSWORD:
        return

    def password_entered():
        if st.session_state.get("password_input") == APP_PASSWORD:
            st.session_state["password_ok"] = True
            del st.session_state["password_input"]
        else:
            st.session_state["password_ok"] = False

    if st.session_state.get("password_ok"):
        return

    st.text_input(
        "Enter dashboard password",
        type="password",
        key="password_input",
        on_change=password_entered,
    )
    if st.session_state.get("password_ok"):
        return
    st.stop()


@st.cache_data(ttl=900)
def load_holdings(path: Path = HOLDINGS_PATH):
    if not path.exists():
        return [], 0.0

    df = pd.read_csv(path)
    holdings = []
    cash_usd = 0.0

    for _, row in df.iterrows():
        market = str(row.get("Market", "")).strip()
        ticker = str(row.get("Ticker", "")).strip()
        name = str(row.get("Asset Name", "")).strip() or ticker
        qty_raw = str(row.get("quantity(stock number)", "0"))

        if market.lower().startswith("remaining cash") or ticker.lower().startswith("remaining cash"):
            cash_usd = _parse_cash_value(row)
            continue

        if not ticker or ticker.lower() == "nan":
            continue

        shares = _parse_float(qty_raw)
        if shares <= 0:
            continue

        holdings.append(
            {
                "market": market,
                "ticker": ticker,
                "name": name,
                "shares": shares,
                "yf_symbol": to_yfinance_symbol(market, ticker),
                "currency": MARKET_CURRENCY.get(market, "USD"),
            }
        )

    return holdings, cash_usd


def _parse_cash_value(row: pd.Series) -> float:
    for field in (row.get("Ticker", ""), row.get("Asset Name", ""), row.get("quantity(stock number)", "")):
        digits = re.findall(r"[0-9.]+", str(field))
        if digits:
            return float(digits[0])
    return 0.0


def _parse_float(value: str) -> float:
    value = str(value).replace(",", "").strip()
    if not value:
        return 0.0
    try:
        return float(value)
    except ValueError:
        return 0.0


def to_yfinance_symbol(market: str, ticker: str) -> str:
    if market == "JP" and ticker.endswith(".T"):
        return ticker
    suffix = MARKET_SUFFIX.get(market, "")
    return f"{ticker}{suffix}"


@st.cache_data(ttl=900)
def fetch_fx_rates(currencies):
    rates = {BASE_CURRENCY: 1.0, "USD": 1.0}
    for cur in currencies:
        if not cur or cur in rates:
            continue
        sym = FX_SYMBOLS.get(cur)
        if not sym:
            continue
        series, _ = fetch_chart_series(sym, span="1mo", interval="1d")
        if series is None or series.empty:
            continue
        rates[cur] = float(series.dropna().iloc[-1])
    return rates


@st.cache_data(ttl=300)
def fetch_quotes(symbols):
    quotes = {}
    if not symbols:
        return quotes

    for symbol in symbols:
        series, currency = fetch_chart_series(symbol, span="5d", interval="1d")
        if series is None or series.empty:
            continue
        series = series.dropna()
        price = float(series.iloc[-1])
        prev = float(series.iloc[-2]) if len(series) > 1 else price
        quotes[symbol] = {
            "price": price,
            "prev_close": prev,
            "currency": currency,
        }

    return quotes


@st.cache_data(ttl=600)
def load_watchlist(path: Path = WATCHLIST_PATH):
    if not path.exists():
        return pd.DataFrame(columns=["Ticker", "Name", "Desired Entry USD", "Notes"])
    df = pd.read_csv(path)
    df["Desired Entry USD"] = pd.to_numeric(df["Desired Entry USD"], errors="coerce")
    return df


@st.cache_data(ttl=900)
def build_nav_history(holdings_signature, cash_balance, range_key):
    settings = RANGE_CHOICES.get(range_key, RANGE_CHOICES["1M"])
    price_series = {}
    position_series = {}
    fx_cache = {}

    for signature in holdings_signature:
        ticker, shares, currency, yf_symbol = signature
        series, meta_currency = fetch_chart_series(yf_symbol, span=settings["range"], interval=settings["interval"])
        if series is None or series.empty:
            continue
        usd_series = series.dropna()
        local_currency = meta_currency or currency
        if local_currency != "USD":
            fx_symbol = FX_SYMBOLS.get(local_currency)
            if fx_symbol:
                fx_series = fx_cache.get((fx_symbol, range_key))
                if fx_series is None:
                    fx_series, _ = fetch_chart_series(fx_symbol, span=settings["range"], interval=settings["interval"])
                    if fx_series is not None:
                        fx_series = fx_series.dropna()
                        fx_cache[(fx_symbol, range_key)] = fx_series
                if fx_series is not None and not fx_series.empty:
                    fx_aligned = fx_series.reindex(usd_series.index, method="ffill")
                    usd_series = (usd_series * fx_aligned).dropna()
        price_series[ticker] = usd_series
        position_series[ticker] = (usd_series * shares).dropna()

    if not position_series:
        return pd.Series(dtype=float), pd.DataFrame()

    value_df = pd.concat(position_series, axis=1).fillna(method="ffill")
    nav_series = value_df.sum(axis=1) + cash_balance

    if range_key == "1D":
        nav_series = nav_series.last("1D")
        price_series = {k: v.last("1D") for k, v in price_series.items()}
    elif range_key == "7D":
        nav_series = nav_series.last("7D")
        price_series = {k: v.last("7D") for k, v in price_series.items()}
    elif range_key == "1M":
        nav_series = nav_series.last("31D")
        price_series = {k: v.last("31D") for k, v in price_series.items()}
    elif range_key == "1Y":
        nav_series = nav_series.last("366D")
        price_series = {k: v.last("366D") for k, v in price_series.items()}

    price_df = pd.concat(price_series, axis=1).fillna(method="ffill")
    return nav_series, price_df


@st.cache_data(ttl=900)
def fetch_news(symbols, max_items=8):
    import xml.etree.ElementTree as ET
    from email.utils import parsedate_to_datetime

    items = []
    seen_titles = set()
    for symbol in symbols:
        try:
            resp = requests.get(
                f"https://feeds.finance.yahoo.com/rss/2.0/headline?s={symbol}&region=US&lang=en-US",
                headers=YF_HEADERS,
                timeout=10,
            )
            resp.raise_for_status()
            root = ET.fromstring(resp.content)
            entries = root.findall(".//item")
        except Exception:
            entries = []
        for entry in entries:
            title = (entry.findtext("title") or "").strip()
            link = (entry.findtext("link") or "").strip()
            if not title or not link or title in seen_titles:
                continue
            seen_titles.add(title)
            pub_text = entry.findtext("pubDate") or ""
            age_str = ""
            if pub_text:
                try:
                    published = parsedate_to_datetime(pub_text)
                    age_hours = (datetime.now(timezone.utc) - published).total_seconds() / 3600
                    age_str = f"{age_hours:.0f}h ago" if age_hours < 48 else published.strftime("%b %d")
                except Exception:
                    pass
            items.append({"title": title, "link": link, "publisher": "Yahoo Finance", "age": age_str})
            if len(items) >= max_items:
                return items
    return items


def conviction_label(weight: float) -> str:
    if weight >= 0.2:
        return "High Conviction"
    if weight >= 0.08:
        return "Core"
    return "Satellite"


st.set_page_config(page_title="Portfolio Radar", page_icon="📈", layout="wide")
enforce_password()

# ── Sidebar ───────────────────────────────────────────────────────────────────
with st.sidebar:
    st.title("📈 Portfolio Radar")
    alert_threshold = st.slider("Watchlist alert window (%)", 0.0, 10.0, 3.0, 0.5)
    history_range = st.radio("Chart range", list(RANGE_CHOICES.keys()), index=2, horizontal=True)
    st.divider()

# ── Load data ─────────────────────────────────────────────────────────────────
holdings, cash_balance = load_holdings()
if not holdings:
    st.warning("No holdings found. Update holdings.csv to populate the dashboard.")
    st.stop()

symbols = [h["yf_symbol"] for h in holdings]
quotes   = fetch_quotes(symbols)
fx_rates = fetch_fx_rates({h["currency"] for h in holdings})

with st.sidebar:
    st.caption(f"HKD→USD {fx_rates.get('HKD', 1):.4f}  ·  JPY→USD {fx_rates.get('JPY', 1):.5f}")

# ── Build positions ───────────────────────────────────────────────────────────
rows = []
for h in holdings:
    quote = quotes.get(h["yf_symbol"])
    if not quote:
        continue
    price_local = quote["price"]
    prev_local  = quote["prev_close"]
    fx          = fx_rates.get(h["currency"], 1.0)
    value_usd   = price_local * fx * h["shares"]
    daily_pct   = ((price_local / prev_local) - 1.0) * 100 if prev_local else 0.0
    rows.append({
        "Name": h["name"], "Ticker": h["ticker"], "Market": h["market"],
        "Shares": h["shares"], "Price (local)": price_local,
        "Currency": h["currency"], "USD Value": value_usd,
        "Daily %": daily_pct, "Ticker Symbol": h["yf_symbol"],
    })

positions_df = pd.DataFrame(rows)
if positions_df.empty:
    st.error("Live pricing unavailable — please hit Rerun in a moment.")
    st.stop()

total_invested = positions_df["USD Value"].sum()
total_nav      = total_invested + cash_balance
positions_df["Weight %"]  = positions_df["USD Value"] / total_nav * 100
positions_df["Conviction"] = positions_df["Weight %"].apply(lambda w: conviction_label(w / 100))

# ── Header metrics ────────────────────────────────────────────────────────────
m1, m2, m3, m4 = st.columns(4)
m1.metric("💰 Total NAV", f"${total_nav:,.0f}")
m2.metric("📊 Invested",  f"${total_invested:,.0f}")
m3.metric("🏦 Cash",      f"${cash_balance:,.0f}")
# portfolio daily change
port_daily = sum(
    r["Daily %"] * r["USD Value"] for _, r in positions_df.iterrows()
) / total_invested if total_invested else 0
m4.metric("📅 Today", f"{port_daily:+.2f}%",
          delta_color="normal" if port_daily >= 0 else "inverse")

st.divider()

# ── Performance chart ─────────────────────────────────────────────────────────
st.subheader("Total portfolio value")
holdings_sig = tuple((h["ticker"], h["shares"], h["currency"], h["yf_symbol"]) for h in holdings)
nav_series, _ = build_nav_history(holdings_sig, cash_balance, history_range)
if not nav_series.empty:
    try:
        nav_display = nav_series.tz_convert("Asia/Shanghai").tz_localize(None)
    except Exception:
        nav_display = nav_series
    # Plotly chart for better styling
    import plotly.graph_objects as go
    fig = go.Figure()
    fig.add_trace(go.Scatter(
        x=nav_display.index, y=nav_display.values,
        mode="lines", name="NAV",
        line=dict(color="#00d4a4", width=2),
        fill="tozeroy", fillcolor="rgba(0,212,164,0.08)"
    ))
    start_val = nav_display.iloc[0]
    end_val   = nav_display.iloc[-1]
    chg_pct   = (end_val / start_val - 1) * 100 if start_val else 0
    colour    = "#00d4a4" if chg_pct >= 0 else "#ff4b4b"
    fig.update_layout(
        height=260, margin=dict(l=0, r=0, t=8, b=0),
        xaxis=dict(showgrid=False), yaxis=dict(showgrid=True, gridcolor="#222"),
        plot_bgcolor="rgba(0,0,0,0)", paper_bgcolor="rgba(0,0,0,0)",
        showlegend=False,
        annotations=[dict(
            x=0.01, y=0.95, xref="paper", yref="paper",
            text=f"<b style='color:{colour}'>{chg_pct:+.2f}%</b> over {history_range}",
            showarrow=False, font=dict(size=13)
        )]
    )
    st.plotly_chart(fig, use_container_width=True)
else:
    st.info("Chart data unavailable for this range — try a longer window.")

st.divider()

# ── Positions table ───────────────────────────────────────────────────────────
st.subheader("Positions")
display_df = (
    positions_df[["Name","Ticker","Market","Shares","Price (local)","Currency",
                  "USD Value","Weight %","Daily %","Conviction"]]
    .sort_values("USD Value", ascending=False)
    .copy()
)
display_df["Daily %"]  = display_df["Daily %"].map(lambda x: f"{x:+.2f}%")
display_df["Weight %"] = display_df["Weight %"].map(lambda x: f"{x:.1f}%")
display_df["USD Value"] = display_df["USD Value"].map(lambda x: f"${x:,.0f}")
st.dataframe(display_df, use_container_width=True, hide_index=True)

# ── Allocation charts ─────────────────────────────────────────────────────────
class_map = {"US":"US Equities","HK":"HK Equities","JP":"Japan Equities","Crypto":"Crypto"}
positions_df["Asset Class"] = positions_df["Market"].map(class_map).fillna(positions_df["Market"])
alloc_df = positions_df.groupby("Asset Class")["USD Value"].sum().reset_index()

c1, c2 = st.columns(2)
with c1:
    fig = px.pie(alloc_df, values="USD Value", names="Asset Class", hole=0.45,
                 title="Allocation by asset class")
    fig.update_layout(margin=dict(l=0,r=0,t=40,b=0), showlegend=True,
                      plot_bgcolor="rgba(0,0,0,0)", paper_bgcolor="rgba(0,0,0,0)")
    st.plotly_chart(fig, use_container_width=True)
with c2:
    top8 = positions_df.nlargest(8, "USD Value")
    fig = px.bar(top8, x="Name", y="USD Value", color="Conviction",
                 title="Top positions",
                 text=top8["Weight %"].map(lambda w: f"{w:.1f}%"),
                 color_discrete_map={"High Conviction":"#00d4a4","Core":"#4da6ff","Satellite":"#ffbb44"})
    fig.update_traces(textposition="outside")
    fig.update_layout(margin=dict(l=0,r=0,t=40,b=0),
                      plot_bgcolor="rgba(0,0,0,0)", paper_bgcolor="rgba(0,0,0,0)")
    st.plotly_chart(fig, use_container_width=True)

weights = positions_df["Weight %"] / 100
hhi = float((weights ** 2).sum())
st.caption(
    f"Concentration (HHI): **{hhi:.3f}** — "
    + ("🔴 Highly concentrated" if hhi > 0.25 else "🟡 Balanced" if hhi > 0.15 else "🟢 Well diversified")
)

st.divider()

# ── Watchlist ─────────────────────────────────────────────────────────────────
watchlist_df = load_watchlist()
if not watchlist_df.empty:
    st.subheader("🎯 Watchlist & Opportunity Reminders")
    watch_symbols = [
        raw if ("-" in raw or "." in raw) else to_yfinance_symbol("US", raw)
        for raw in watchlist_df["Ticker"].tolist()
    ]
    watch_quotes = fetch_quotes(watch_symbols)
    alert_rows = []
    for _, row in watchlist_df.iterrows():
        t = row["Ticker"]
        sym = t if ("-" in t or "." in t) else to_yfinance_symbol("US", t)
        q = watch_quotes.get(sym)
        price = q["price"] if q else None
        target = row.get("Desired Entry USD")
        delta = (price - target) / target * 100 if (price and target and target > 0) else None
        within = delta is not None and delta <= alert_threshold
        alert_rows.append({"Ticker": t, "Name": row.get("Name", t),
                           "Price": f"${price:,.2f}" if price else "—",
                           "Target": f"${target:,.2f}" if target else "—",
                           "Δ vs target": f"{delta:+.1f}%" if delta is not None else "—",
                           "Notes": row.get("Notes", ""), "🔔": "✅" if within else ""})
    alert_df = pd.DataFrame(alert_rows)
    st.dataframe(alert_df, use_container_width=True, hide_index=True)
    ready = [r for r in alert_rows if r["🔔"] == "✅"]
    if ready:
        st.success("  |  ".join(f"{r['Ticker']} within entry band" for r in ready))

st.divider()

# ── Market news ───────────────────────────────────────────────────────────────
st.subheader("📰 Market News")
news_tab1, news_tab2 = st.tabs(["Top Holdings", "All Holdings"])

with news_tab1:
    top_news_symbols = positions_df.sort_values("USD Value", ascending=False)["Ticker Symbol"].head(3).tolist()
    items = fetch_news(top_news_symbols, max_items=8)
    if items:
        for item in items:
            age = f"  `{item['age']}`" if item.get("age") else ""
            st.markdown(f"**[{item['title']}]({item['link']})**{age}")
            st.divider()
    else:
        st.info("No headlines found — try refreshing.")

with news_tab2:
    all_news_symbols = positions_df.sort_values("USD Value", ascending=False)["Ticker Symbol"].tolist()
    items = fetch_news(all_news_symbols, max_items=12)
    if items:
        for item in items:
            age = f"  `{item['age']}`" if item.get("age") else ""
            st.markdown(f"**[{item['title']}]({item['link']})**{age}")
            st.divider()
    else:
        st.info("No headlines found — try refreshing.")

st.caption("Data: Yahoo Finance. Prices ~5 min delay. FX updated every 15 min.")
