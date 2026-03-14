#!/usr/bin/env python3
"""Daily Portfolio Digest — analyst-style briefing with news summaries + insights."""
import os, requests, csv, xml.etree.ElementTree as ET, re
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
        if r.status_code >= 400:
            print('Discord error:', r.status_code, r.text)

def fetch_price(sym):
    try:
        r = requests.get(f'https://query1.finance.yahoo.com/v8/finance/chart/{sym}',
                         params={'range':'5d','interval':'1d'}, headers=YH_HEADERS, timeout=10)
        closes = [c for c in r.json()['chart']['result'][0]['indicators']['quote'][0]['close'] if c is not None]
        return float(closes[-1]), float(closes[-2]) if len(closes) > 1 else float(closes[-1])
    except Exception:
        return None, None

def clean(text):
    """Strip HTML tags and whitespace from RSS description."""
    text = re.sub(r'<[^>]+>', '', text or '')
    text = re.sub(r'\s+', ' ', text).strip()
    return text[:280] + '…' if len(text) > 280 else text

def fetch_news(sym, n=2):
    try:
        r = requests.get(
            f'https://feeds.finance.yahoo.com/rss/2.0/headline?s={sym}&region=US&lang=en-US',
            headers=YH_HEADERS, timeout=10)
        root = ET.fromstring(r.content)
        results = []
        for item in root.findall('.//item')[:n]:
            title = clean(item.findtext('title', ''))
            desc = clean(item.findtext('description', ''))
            summary = desc if desc and desc != title else title
            results.append(summary)
        return results
    except Exception:
        return []

# FX rates
FX = {'USD': 1.0}
for cur, sym in {'HKD': 'HKDUSD=X', 'JPY': 'JPYUSD=X'}.items():
    p, _ = fetch_price(sym)
    FX[cur] = p or 1.0

# Load holdings
holdings = []
with HOLDINGS_CSV.open() as f:
    for row in csv.DictReader(f):
        t = (row.get('Ticker') or '').strip()
        m = (row.get('Market') or '').strip()
        if not t or t.startswith('Remaining'): continue
        try:
            shares = float(str(row.get('quantity(stock number)', '0')).replace(',','').replace('USD','').strip())
        except: continue
        if shares <= 0: continue
        sym = t
        if m == 'HK' and not sym.upper().endswith('.HK'):
            sym = sym.lstrip('0') + '.HK'
        currency = {'HK': 'HKD', 'JP': 'JPY'}.get(m, 'USD')
        holdings.append({'name': row.get('Asset Name') or t, 'sym': sym,
                         'shares': shares, 'cur': currency, 'market': m})

for h in holdings:
    p, prev = fetch_price(h['sym'])
    fx = FX.get(h['cur'], 1.0)
    h['price'] = p
    h['usd'] = (p * fx * h['shares']) if p else 0
    h['chg'] = ((p / prev) - 1) * 100 if (p and prev and prev > 0) else None

top = sorted(holdings, key=lambda x: x['usd'], reverse=True)
total_usd = sum(h['usd'] for h in holdings)
now = datetime.now(tz=timezone.utc).astimezone()
date_str = now.strftime('%a %d %b %Y')

# Brief analyst context per ticker (static backdrop, updated by holdings logic)
BACKDROP = {
    'NVDA':  'AI capex cycle still dominant; watch for any data center demand signals.',
    'GOOGL': 'Cloud + AI monetization in focus; Gemini integration updates moving the needle.',
    'INTC':  'Deep value turnaround play — execution on 18A node is the sole catalyst.',
    'RKLB':  'Neutron development pace + contract wins are the key re-rating triggers.',
    'URNM':  'Uranium supply constraint thesis intact; utility contracting cycle accelerating.',
    '1211.HK': 'BYD EV export data + margin trend is the near-term price driver.',
    '9660.HK': 'Horizon Robotics: autonomous driving SoC adoption in China OEMs.',
    '2105.HK': 'Laekna: watch pipeline readouts.',
    '2692.HK': 'Zhaowei: precision gearbox demand tied to robotics/EV actuator cycle.',
    '6324.T':  'Harmonic Drive: robot joint demand surge from humanoid robotics wave.',
    'BTC-USD':  'Macro risk-on/off + ETF flow data drives short-term; long-term thesis intact.',
    'ETH-USD':  'ETH staking yield + L2 activity metrics are the health indicators.',
    'RENDER-USD': 'GPU compute demand for AI/3D rendering — directional bet on decentralised AI infra.',
    'DOGE-USD': 'Sentiment/meme driven; size accordingly.',
}

# --- Block 1: Header + Portfolio snapshot ---
lines = [f"## 📊 Morning Briefing — {date_str}", f"**Total Invested ≈ ${total_usd:,.0f} USD**\n"]
lines.append("**Portfolio Snapshot**")
for h in top[:6]:
    p_str = f"${h['price']:,.2f}" if h['price'] else "—"
    chg_str = (f"{'🟢' if h['chg'] >= 0 else '🔴'}{h['chg']:+.1f}%") if h['chg'] is not None else "—"
    lines.append(f"- **{h['name']}** ({h['sym']}) {p_str} {chg_str}")

movers = sorted([h for h in holdings if h['chg'] is not None], key=lambda x: abs(x['chg']), reverse=True)[:3]
if movers:
    lines.append("\n**Biggest Movers**")
    for h in movers:
        e = '🟢' if h['chg'] >= 0 else '🔴'
        lines.append(f"- {e} **{h['name']}** {h['chg']:+.1f}%")

send('\n'.join(lines))

# --- Block 2: News + insights per top 3 holdings ---
news_lines = ["**📰 What's Moving — News + Yongping's Take**\n"]
for h in top[:4]:
    items = fetch_news(h['sym'], n=2)
    backdrop = BACKDROP.get(h['sym'], '')
    news_lines.append(f"**{h['name']}** ({h['sym']})")
    if items:
        for it in items:
            news_lines.append(f"  • {it}")
    else:
        news_lines.append("  • No headlines today.")
    if backdrop:
        news_lines.append(f"  💡 *{backdrop}*")
    news_lines.append("")

send('\n'.join(news_lines))

# --- Block 3: Watchlist ---
wl_lines = ["**🎯 Watchlist — Entry Radar**\n"]
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
            if delta <= 0:
                status = "✅ AT/BELOW TARGET — consider adding"
            elif delta <= 5:
                status = f"🔔 {delta:.1f}% above target — getting close"
            else:
                status = f"  {delta:+.1f}% above target — wait"
            wl_lines.append(f"- **{name}** ${p:.2f} vs ${target:.2f} target — {status}")
        else:
            wl_lines.append(f"- **{name}**: price unavailable")

wl_lines.append(f"\n*Next scheduled digest: tomorrow 10:00 HKT*")
send('\n'.join(wl_lines))
