import math
import os
import re
from datetime import datetime, timezone
from pathlib import Path

import pandas as pd
import plotly.express as px
import streamlit as st
import yfinance as yf

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
    symbol_map = {}
    for cur in currencies:
        if cur and cur not in rates:
            sym = FX_SYMBOLS.get(cur)
            if sym:
                symbol_map[sym] = cur
    symbols = list(symbol_map.keys())
    if not symbols:
        return rates

    try:
        data = yf.download(symbols, period="5d", interval="1d", progress=False)
    except Exception:
        return rates

    if isinstance(data, pd.DataFrame):
        if data.empty:
            return rates
        subset = data
        if isinstance(data.columns, pd.MultiIndex):
            try:
                subset = data["Adj Close"]
            except KeyError:
                subset = data
        elif "Adj Close" in data.columns:
            subset = data["Adj Close"]
        try:
            last_row = subset.iloc[-1]
        except IndexError:
            return rates
        if isinstance(last_row, pd.Series):
            for sym, cur in symbol_map.items():
                if sym in last_row.index:
                    rates[cur] = float(last_row[sym])
        else:
            sym = symbols[0]
            cur = symbol_map.get(sym)
            if cur:
                rates[cur] = float(last_row)
    elif isinstance(data, pd.Series):
        if data.empty:
            return rates
        for sym, cur in symbol_map.items():
            if sym in data.index:
                rates[cur] = float(data[sym])

    return rates


@st.cache_data(ttl=300)
def fetch_quotes(symbols):
    quotes = {}
    if not symbols:
        return quotes

    for symbol in symbols:
        try:
            ticker = yf.Ticker(symbol)
        except Exception:
            continue

        price = None
        prev = None
        currency = None

        try:
            info = ticker.fast_info or {}
            price = info.get("last_price") or info.get("regular_market_price")
            prev = info.get("previous_close")
            currency = info.get("currency")
        except Exception:
            info = {}

        if price is None or prev is None:
            try:
                hist = ticker.history(period="5d", interval="1d")
                if not hist.empty:
                    price = price or hist["Close"].iloc[-1]
                    prev = prev or (hist["Close"].iloc[-2] if len(hist) > 1 else hist["Close"].iloc[-1])
            except Exception:
                pass

        if price is None:
            continue

        quotes[symbol] = {
            "price": float(price),
            "prev_close": float(prev) if prev else float(price),
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
def fetch_news(symbols, max_items=8):
    items = []
    seen_titles = set()
    for symbol in symbols:
        try:
            news_feed = yf.Ticker(symbol).news or []
        except Exception:
            news_feed = []
        for story in news_feed:
            title = story.get("title")
            link = story.get("link")
            if not title or not link or title in seen_titles:
                continue
            seen_titles.add(title)
            ts = story.get("providerPublishTime")
            if ts:
                published = datetime.fromtimestamp(ts, tz=timezone.utc)
                age_hours = (datetime.now(timezone.utc) - published).total_seconds() / 3600
                age_str = f"{age_hours:.0f}h ago" if age_hours < 48 else published.strftime("%b %d")
            else:
                age_str = ""
            items.append(
                {
                    "title": title,
                    "link": link,
                    "publisher": story.get("publisher", ""),
                    "age": age_str,
                }
            )
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
st.title("Portfolio Radar")
st.caption("Live view across holdings, watchlist, and opportunity reminders")

alert_threshold = st.sidebar.slider(
    "Watchlist alert window (% above target)",
    min_value=0.0,
    max_value=10.0,
    value=3.0,
    step=0.5,
)

holdings, cash_balance = load_holdings()
if not holdings:
    st.warning("No holdings found. Update holdings.csv to populate the dashboard.")

symbols = [h["yf_symbol"] for h in holdings]
quotes = fetch_quotes(symbols)
fx_rates = fetch_fx_rates({h["currency"] for h in holdings})

rows = []
for h in holdings:
    symbol = h["yf_symbol"]
    quote = quotes.get(symbol)
    if not quote:
        continue

    price_local = quote["price"]
    prev_local = quote["prev_close"]
    currency = h["currency"]
    fx = fx_rates.get(currency, 1.0)
    price_usd = price_local * fx
    value_usd = price_usd * h["shares"]
    daily_change = ((price_local / prev_local) - 1.0) * 100 if prev_local else 0.0

    rows.append(
        {
            "Name": h["name"],
            "Ticker": h["ticker"],
            "Market": h["market"],
            "Shares": h["shares"],
            "Price (local)": price_local,
            "Currency": currency,
            "USD Value": value_usd,
            "Daily %": daily_change,
            "Conviction": None,
            "Ticker Symbol": symbol,
        }
    )

positions_df = pd.DataFrame(rows)
if positions_df.empty:
    st.error("Live pricing feed failed (yfinance returned no data). Give it a minute and hit rerun.")
    st.stop()

total_equity_value = positions_df["USD Value"].sum()
total_nav = total_equity_value + cash_balance
positions_df["Weight %"] = positions_df["USD Value"] / total_nav * 100 if total_nav else 0
positions_df["Conviction"] = positions_df["Weight %"].apply(lambda w: conviction_label(w / 100))

col1, col2, col3 = st.columns(3)
col1.metric("Total NAV (USD)", f"${total_nav:,.0f}")
col2.metric("Invested Capital", f"${total_equity_value:,.0f}")
col3.metric("Cash Buffer", f"${cash_balance:,.0f}", help="Dry powder available for redeployment")

st.subheader("Positions")
st.dataframe(
    positions_df[
        [
            "Name",
            "Ticker",
            "Market",
            "Shares",
            "Price (local)",
            "Currency",
            "USD Value",
            "Weight %",
            "Daily %",
            "Conviction",
        ]
    ].sort_values("USD Value", ascending=False),
    use_container_width=True,
    hide_index=True,
)

if not positions_df.empty:
    class_map = {
        "US": "US Equities",
        "HK": "HK Equities",
        "JP": "Japan Equities",
        "Crypto": "Crypto",
    }
    positions_df["Asset Class"] = positions_df["Market"].map(class_map).fillna(positions_df["Market"])

    alloc_by_class = (
        positions_df.groupby("Asset Class")["USD Value"].sum().reset_index()
    )
    chart_col1, chart_col2 = st.columns(2)
    with chart_col1:
        st.markdown("**Allocation by asset class**")
        fig = px.pie(alloc_by_class, values="USD Value", names="Asset Class", hole=0.4)
        st.plotly_chart(fig, use_container_width=True)
    with chart_col2:
        st.markdown("**Top positions by USD weight**")
        top_df = positions_df.nlargest(8, "USD Value")
        fig = px.bar(
            top_df,
            x="Name",
            y="USD Value",
            color="Conviction",
            text=top_df["Weight %"].map(lambda w: f"{w:.1f}%"),
        )
        fig.update_traces(textposition="outside")
        st.plotly_chart(fig, use_container_width=True)

    weights = positions_df["Weight %"] / 100
    hhi = float((weights ** 2).sum())
    st.markdown(
        f"**Concentration score (HHI):** {hhi:.3f} — "
        + ("Highly concentrated" if hhi > 0.25 else "Balanced core/satellite" if hhi > 0.15 else "Well diversified")
    )

watchlist_df = load_watchlist()
if not watchlist_df.empty:
    st.subheader("Watchlist & Opportunity Reminders")
    watch_symbols = []
    for raw in watchlist_df["Ticker"].tolist():
        if "-" in raw or "." in raw:
            watch_symbols.append(raw)
        else:
            watch_symbols.append(to_yfinance_symbol("US", raw))

    watch_quotes = fetch_quotes(watch_symbols)
    alert_rows = []

    for _, row in watchlist_df.iterrows():
        ticker = row["Ticker"]
        name = row.get("Name", ticker)
        target = row.get("Desired Entry USD")
        if "-" in ticker or "." in ticker:
            yf_symbol = ticker
        else:
            yf_symbol = to_yfinance_symbol("US", ticker)
        quote = watch_quotes.get(yf_symbol)
        price = quote["price"] if quote else None

        delta = None
        within_band = False
        if price and target and target > 0:
            delta = (price - target) / target * 100
            within_band = delta <= alert_threshold

        alert_rows.append(
            {
                "Ticker": ticker,
                "Name": name,
                "Price": price,
                "Target": target,
                "Δ vs target %": delta,
                "Notes": row.get("Notes", ""),
                "Ready": within_band,
            }
        )

    alert_df = pd.DataFrame(alert_rows)
    st.dataframe(alert_df, use_container_width=True, hide_index=True)

    ready = alert_df[alert_df["Ready"] == True]
    if not ready.empty:
        st.success(
            "\n".join(
                f"{r.Ticker} is {r['Δ vs target %']:.2f}% above target (within alert window)." for r in ready.itertuples()
            )
        )

if not positions_df.empty:
    st.subheader("Fresh headlines (top holdings)")
    top_symbols = positions_df.sort_values("USD Value", ascending=False)["Ticker Symbol"].head(3).tolist()
    news_items = fetch_news(top_symbols, max_items=6)
    if news_items:
        for item in news_items:
            publisher = f" — {item['publisher']}" if item.get("publisher") else ""
            age = f" ({item['age']})" if item.get("age") else ""
            st.markdown(f"- **[{item['title']}]({item['link']})**{publisher}{age}")
    else:
        st.info("No recent headlines surfaced from Yahoo Finance.")

st.caption(
    "Data via Yahoo! Finance (via yfinance). Refresh interval ~5 minutes."
)
