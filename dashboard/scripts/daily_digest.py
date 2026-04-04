#!/usr/bin/env python3
"""Daily Portfolio Digest — portfolio snapshot + market sector scan + watchlist radar."""
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
    text = re.sub(r'<[^>]+>', '', text or '')
    text = re.sub(r'\s+', ' ', text).strip()
    return text[:200] + '…' if len(text) > 200 else text

def fetch_top_news(sym, n=1):
    try:
        r = requests.get(f'https://feeds.finance.yahoo.com/rss/2.0/headline?s={sym}&region=US&lang=en-US',
                         headers=YH_HEADERS, timeout=10)
        root = ET.fromstring(r.content)
        results = []
        for item in root.findall('.//item')[:n]:
            title = clean(item.findtext('title', ''))
            desc = clean(item.findtext('description', ''))
            results.append(desc if desc and desc != title else title)
        return results
    except Exception:
        return []

# ── FX ──────────────────────────────────────────────────────────────
FX = {'USD': 1.0}
for cur, sym in {'HKD': 'HKDUSD=X', 'JPY': 'JPYUSD=X'}.items():
    p, _ = fetch_price(sym)
    FX[cur] = p or 1.0

# ── Holdings ────────────────────────────────────────────────────────
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
movers = sorted([h for h in holdings if h['chg'] is not None], key=lambda x: abs(x['chg']), reverse=True)
now = datetime.now(tz=timezone.utc).astimezone()
date_str = now.strftime('%a %d %b %Y')

# ── BLOCK 1: Portfolio Snapshot ──────────────────────────────────────
lines = [f"## 📊 Morning Briefing — {date_str}",
         f"**Portfolio ≈ ${total_usd:,.0f} USD invested**\n"]
lines.append("**Top Holdings**")
for h in top[:6]:
    p_str = f"${h['price']:,.2f}" if h['price'] else "—"
    chg_str = (f"{'🟢' if h['chg'] >= 0 else '🔴'}{h['chg']:+.1f}%") if h['chg'] is not None else "—"
    lines.append(f"- **{h['name']}** ({h['sym']}) {p_str} {chg_str}")

if movers:
    lines.append("\n**Biggest Movers**")
    for h in movers[:3]:
        e = '🟢' if h['chg'] >= 0 else '🔴'
        lines.append(f"- {e} **{h['name']}** {h['chg']:+.1f}%")

# Flag any holding with >5% single-day move as alert
alerts = [h for h in holdings if h['chg'] is not None and abs(h['chg']) >= 5]
if alerts:
    lines.append("\n**⚠️ Alerts — Large Moves (>5%)**")
    for h in alerts:
        e = '🟢' if h['chg'] >= 0 else '🔴'
        news = fetch_top_news(h['sym'], 1)
        headline = f" — {news[0]}" if news else ""
        lines.append(f"- {e} **{h['name']}** {h['chg']:+.1f}%{headline}")

send('\n'.join(lines))

# ── BLOCK 2: Portfolio News + Insights ──────────────────────────────
BACKDROP = {
    'NVDA':      'CUDA moat + Blackwell ramp. Watch hyperscaler capex guidance.',
    'GOOGL':     'Cloud + Gemini monetisation. Watch Search market share quarterly.',
    'INTC':      'CHIPS Act backstop + 18A node. Turnaround story — execution is everything.',
    'RKLB':      'Small launch dominance + Space Systems compounder. Neutron = re-rating event.',
    'URNM':      'Uranium supply squeeze + utility contracting cycle. Long-term structural hold.',
    '1211.HK':   'BYD: EV export growth (EU +3x) vs China domestic slowdown. Watch monthly sales.',
    '9660.HK':   'Horizon: ADAS SoC adoption in Chinese OEMs. Design wins = catalyst.',
    '2105.HK':   'Laekna: pre-revenue biotech. Pipeline readouts are binary events.',
    '2692.HK':   'Zhaowei: precision gearbox demand from robotics/EV actuator wave.',
    '6324.T':    'Harmonic Drive: strain wave gears in every serious robot joint. Humanoid wave = demand surge.',
    'BTC-USD':   'Digital gold. ETF inflows + macro risk-on drives price. Long-term hold.',
    'ETH-USD':   'Smart contract platform + staking yield. L2 activity = health metric.',
    'RENDER-USD':'Decentralised GPU compute. Speculative AI infrastructure bet.',
}

news_lines = ["**📰 Portfolio News + Yongping's Take**\n"]
for h in top[:5]:
    items = fetch_top_news(h['sym'], n=2)
    backdrop = BACKDROP.get(h['sym'], '')
    news_lines.append(f"**{h['name']}** ({h['sym']})")
    for it in (items or ["No headlines today."]):
        news_lines.append(f"  • {it}")
    if backdrop:
        news_lines.append(f"  💡 *{backdrop}*")
    news_lines.append("")

send('\n'.join(news_lines))

# ── BLOCK 3: Sector Market Scan ──────────────────────────────────────
SECTORS = {
    "🤖 AI & Semiconductors": ["NVDA","AMD","MRVL","AVGO","TSM","AMAT","MU","ARM","INTC"],
    "⚡ New Energy & Nuclear": ["URNM","NNE","OKLO","VST","CEG","FSLR"],
    "🚀 Aerospace & Space":   ["RKLB","LUNR","ASTS","JOBY","LMT","RTX","NOC"],
    "🧬 Biotech & AI Drug":   ["RXRX","CRSP","ISRG","MRNA","VRTX"],
    "🦾 Humanoid Robotics":   ["TSLA","ABB","6954.T","6506.T"],
    "💰 Crypto Infra":        ["COIN","MSTR","HOOD"],
    "⚛️ Quantum":             ["IONQ","RGTI","QUBT","IBM"],
    "🛡️ Defense":             ["LMT","RTX","NOC","GD","BA"],
}

scan_lines = ["**🔍 Market Scan — Key Sectors**\n"]
for sector, tickers in SECTORS.items():
    scan_lines.append(f"**{sector}**")
    results = []
    for sym in tickers:
        p, prev = fetch_price(sym)
        if p is None: continue
        chg = (p/prev - 1)*100 if prev else 0
        results.append((chg, sym, p))
    results.sort(reverse=True)
    for chg, sym, p in results:
        e = "🟢" if chg >= 0 else "🔴"
        news = fetch_top_news(sym, 1)
        headline = f" → {news[0][:90]}" if news else ""
        scan_lines.append(f"  {e} {sym} ${p:.2f} {chg:+.1f}%{headline}")
    scan_lines.append("")

send('\n'.join(scan_lines))

# ── BLOCK 4: Watchlist Radar ──────────────────────────────────────────
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
                status = "✅ AT/BELOW TARGET — act now"
            elif delta <= 5:
                status = f"🔔 {delta:.1f}% above target — close"
            elif delta <= 15:
                status = f"  {delta:+.1f}% above target — wait"
            else:
                status = f"  {delta:+.1f}% above target — far"
            wl_lines.append(f"- **{name}** ${p:.2f} vs ${target:.2f} — {status}")
        else:
            wl_lines.append(f"- **{name}**: price unavailable")

wl_lines.append(f"\n*Next digest: tomorrow 10:00 HKT*")
send('\n'.join(wl_lines))

print("Digest sent successfully.")
