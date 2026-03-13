#!/usr/bin/env python3
import os, requests, csv, xml.etree.ElementTree as ET
from pathlib import Path
from datetime import datetime, timezone

YH_HEADERS = {"User-Agent": "Mozilla/5.0"}
REPO_ROOT = Path(__file__).resolve().parent.parent.parent
HOLDINGS_CSV = REPO_ROOT / 'holdings.csv'
WATCHLIST_CSV = REPO_ROOT / 'watchlist.csv'
WEBHOOK = os.environ.get('DISCORD_WEBHOOK')

if not WEBHOOK:
    print('DISCORD_WEBHOOK not set; exiting')
    raise SystemExit(0)

def fetch_price(sym):
    try:
        r = requests.get(f'https://query1.finance.yahoo.com/v8/finance/chart/{sym}',
                         params={'range':'5d','interval':'1d'}, headers=YH_HEADERS, timeout=10)
        r.raise_for_status()
        closes = r.json()['chart']['result'][0]['indicators']['quote'][0]['close']
        closes = [c for c in closes if c is not None]
        prev = closes[-2] if len(closes) > 1 else closes[-1]
        return float(closes[-1]), float(prev)
    except Exception:
        return None, None

def fetch_rss(sym, n=2):
    try:
        r = requests.get(f'https://feeds.finance.yahoo.com/rss/2.0/headline?s={sym}&region=US&lang=en-US',
                         headers=YH_HEADERS, timeout=10)
        root = ET.fromstring(r.content)
        return [{'title': i.findtext('title','').strip(),
                 'link': i.findtext('link','').strip()}
                for i in root.findall('.//item')[:n]]
    except Exception:
        return []

FX = {}
for cur, sym in {'HKD':'HKDUSD=X','JPY':'JPYUSD=X'}.items():
    p, _ = fetch_price(sym)
    FX[cur] = p or 1.0
FX['USD'] = 1.0

holdings = []
with HOLDINGS_CSV.open() as f:
    for r in csv.DictReader(f):
        t = (r.get('Ticker') or '').strip()
        m = (r.get('Market') or '').strip()
        if not t: continue
        try: shares = float(r.get('quantity(stock number)','0').replace(',',''))
        except: continue
        if shares <= 0: continue
        sym = t
        if m == 'HK' and not sym.upper().endswith('.HK'):
            sym = sym.lstrip('0') + '.HK'
        elif m == 'Crypto':
            sym = t + '-USD'
        holdings.append({'name': r.get('Asset Name') or t, 'sym': sym,
                         'shares': shares, 'cur': {'HK':'HKD','JP':'JPY','Crypto':'USD'}.get(m,'USD')})

for h in holdings:
    price, prev = fetch_price(h['sym'])
    fx = FX.get(h['cur'], 1.0)
    h['price'] = price
    h['prev']  = prev
    h['usd']   = (price * fx * h['shares']) if price else 0
    h['chg']   = ((price/prev)-1)*100 if (price and prev) else None

top = sorted(holdings, key=lambda x: x['usd'], reverse=True)

lines = []
lines.append(f"## 📊 Daily Portfolio Digest — {datetime.now(tz=timezone.utc).strftime('%a %d %b %Y')}")
lines.append("")
lines.append("**Top Holdings**")
for h in top[:5]:
    p = f"${h['price']:,.2f}" if h['price'] else "—"
    chg = f"{h['chg']:+.2f}%" if h['chg'] is not None else "—"
    lines.append(f"- **{h['name']}** ({h['sym']}) · {p} ({chg}) · {h['shares']} shares")

total = sum(h['usd'] for h in holdings) 
lines.append(f"\n**Estimated Invested (USD):** ${total:,.0f}")

movers = sorted([h for h in holdings if h['chg'] is not None], key=lambda x: abs(x['chg']), reverse=True)
if movers:
    lines.append("\n**Biggest movers today**")
    for h in movers[:3]:
        emoji = "🟢" if h['chg'] >= 0 else "🔴"
        lines.append(f"- {emoji} **{h['name']}**: {h['chg']:+.2f}%")

lines.append("\n**Top Headlines**")
for h in top[:3]:
    news = fetch_rss(h['sym'])
    if news:
        lines.append(f"**{h['name']}**")
        for n in news:
            lines.append(f"  • [{n['title']}]({n['link']})")

lines.append("\n**Watchlist Alerts**")
alerts = []
with WATCHLIST_CSV.open() as f:
    for r in csv.DictReader(f):
        t = (r.get('Ticker') or '').strip()
        m = (r.get('Market') or 'US').strip()
        if not t: continue
        sym = t
        if m == 'HK' and not sym.upper().endswith('.HK'):
            sym = sym.lstrip('0') + '.HK'
        price, _ = fetch_price(sym)
        try: target = float(r.get('Desired Entry USD', 0))
        except: target = 0
        if price and target:
            delta = (price - target) / target * 100
            if delta <= 5.0:
                alerts.append(f"- 🔔 **{r.get('Name') or t}** at ${price:.2f} — {delta:+.1f}% vs target ${target:.2f}")
if alerts:
    lines.extend(alerts)
else:
    lines.append("- No tickers within entry band today.")

requests.post(WEBHOOK, json={"content": "\n".join(lines)})
print("Digest sent.")
