#!/usr/bin/env python3
"""Daily Portfolio Digest — posts to Discord via webhook, chunked to stay under 2000 chars."""
import os, requests, csv, xml.etree.ElementTree as ET
from pathlib import Path
from datetime import datetime, timezone

YH_HEADERS = {"User-Agent": "Mozilla/5.0"}
REPO_ROOT = Path(__file__).resolve().parent.parent.parent
HOLDINGS_CSV = REPO_ROOT / 'holdings.csv'
WATCHLIST_CSV = REPO_ROOT / 'watchlist.csv'
WEBHOOK = os.environ.get('DISCORD_WEBHOOK', '').strip()

if not WEBHOOK or not WEBHOOK.startswith('http'):
    print('ERROR: DISCORD_WEBHOOK not set or invalid.')
    raise SystemExit(1)

def send(text):
    """Post to Discord, splitting into ≤1900-char chunks."""
    chunks, cur = [], ''
    for line in text.split('\n'):
        if len(cur) + len(line) + 1 > 1900:
            chunks.append(cur)
            cur = ''
        cur += line + '\n'
    if cur.strip():
        chunks.append(cur)
    for chunk in chunks:
        r = requests.post(WEBHOOK, json={"content": chunk.strip()}, timeout=10)
        print('discord:', r.status_code)
        if r.status_code >= 400:
            print(r.text)

def fetch_price(sym):
    try:
        r = requests.get(f'https://query1.finance.yahoo.com/v8/finance/chart/{sym}',
                         params={'range':'5d','interval':'1d'}, headers=YH_HEADERS, timeout=10)
        data = r.json()['chart']['result'][0]['indicators']['quote'][0]['close']
        closes = [c for c in data if c is not None]
        return float(closes[-1]), float(closes[-2]) if len(closes) > 1 else float(closes[-1])
    except Exception:
        return None, None

def fetch_rss(sym, n=2):
    try:
        r = requests.get(
            f'https://feeds.finance.yahoo.com/rss/2.0/headline?s={sym}&region=US&lang=en-US',
            headers=YH_HEADERS, timeout=10)
        root = ET.fromstring(r.content)
        return [{'title': i.findtext('title','').strip(), 'link': i.findtext('link','').strip()}
                for i in root.findall('.//item')[:n]]
    except Exception:
        return []

# --- FX ---
FX = {'USD': 1.0}
for cur, sym in {'HKD': 'HKDUSD=X', 'JPY': 'JPYUSD=X'}.items():
    p, _ = fetch_price(sym)
    FX[cur] = p or 1.0

# --- Holdings ---
holdings = []
with HOLDINGS_CSV.open() as f:
    for row in csv.DictReader(f):
        t = (row.get('Ticker') or '').strip()
        m = (row.get('Market') or '').strip()
        if not t or t.startswith('Remaining'): continue
        try: shares = float(str(row.get('quantity(stock number)','0')).replace(',','').replace('USD','').strip())
        except: continue
        if shares <= 0: continue
        sym = t
        if m == 'HK' and not sym.upper().endswith('.HK'):
            sym = sym.lstrip('0') + '.HK'
        currency = {'HK':'HKD','JP':'JPY'}.get(m,'USD')
        holdings.append({'name': row.get('Asset Name') or t, 'sym': sym,
                         'shares': shares, 'cur': currency, 'market': m})

for h in holdings:
    p, prev = fetch_price(h['sym'])
    fx = FX.get(h['cur'], 1.0)
    h['price'] = p
    h['usd'] = (p * fx * h['shares']) if p else 0
    h['chg'] = ((p/prev)-1)*100 if (p and prev and prev > 0) else None

top = sorted(holdings, key=lambda x: x['usd'], reverse=True)
total_usd = sum(h['usd'] for h in holdings)
now = datetime.now(tz=timezone.utc).astimezone()

# --- Message 1: Header + Holdings ---
lines = [f"## 📊 Jackabrew Portfolio Digest — {now.strftime('%a %d %b %Y, %H:%M %Z')}"]
lines.append(f"**Portfolio Invested Value ≈ ${total_usd:,.0f} USD**\n")
lines.append("**Top Holdings**")
for h in top[:6]:
    p_str = f"${h['price']:,.2f}" if h['price'] else "—"
    chg_str = (f"{'🟢' if h['chg']>=0 else '🔴'} {h['chg']:+.2f}%") if h['chg'] is not None else "—"
    lines.append(f"- **{h['name']}** ({h['sym']}) · {p_str} {chg_str}")

# --- Movers ---
movers = sorted([h for h in holdings if h['chg'] is not None], key=lambda x: abs(x['chg']), reverse=True)[:3]
if movers:
    lines.append("\n**Biggest Movers**")
    for h in movers:
        e = '🟢' if h['chg'] >= 0 else '🔴'
        lines.append(f"- {e} {h['name']} {h['chg']:+.2f}%")

send('\n'.join(lines))

# --- Message 2: Headlines ---
news_lines = ["**📰 Top Headlines**"]
for h in top[:3]:
    items = fetch_rss(h['sym'])
    if items:
        news_lines.append(f"**{h['name']}**")
        for it in items:
            news_lines.append(f"  • [{it['title']}](<{it['link']}>)")
send('\n'.join(news_lines))

# --- Message 3: Watchlist alerts ---
alerts = ["**🎯 Watchlist Alerts**"]
with WATCHLIST_CSV.open() as f:
    for row in csv.DictReader(f):
        t = (row.get('Ticker') or '').strip()
        if not t: continue
        m = (row.get('Market') or 'US').strip()
        sym = t
        if m == 'HK' and not sym.upper().endswith('.HK'):
            sym = sym.lstrip('0') + '.HK'
        p, _ = fetch_price(sym)
        try: target = float(row.get('Desired Entry USD', 0))
        except: target = 0
        name = row.get('Name') or row.get('Asset Name') or t
        if p and target:
            delta = (p - target) / target * 100
            flag = "🔔 **NEAR ENTRY**" if delta <= 5.0 else ("✅ **AT/BELOW TARGET**" if delta <= 0 else f"  {delta:+.1f}% above target")
            alerts.append(f"- {name} ({sym}): ${p:.2f} — {flag} (target ${target:.2f})")
        else:
            alerts.append(f"- {name} ({sym}): price unavailable")

if len(alerts) == 1:
    alerts.append("- No tickers near entry today.")
send('\n'.join(alerts))
