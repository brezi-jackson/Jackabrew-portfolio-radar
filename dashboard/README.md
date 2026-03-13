# Portfolio Radar Dashboard

Lightweight Streamlit app that turns `holdings.csv` + `watchlist.csv` into a live browser dashboard with:

- Live prices (Yahoo Finance) in USD, including FX normalization for HKD/JPY names
- Allocation breakdowns, concentration score, and conviction tiers
- Watchlist with alert window logic (user-set threshold via sidebar)
- Opportunity reminders when tickers trade within your entry band
- Fresh headlines for the largest positions
- Optional password gate via `APP_PASSWORD`

## Run locally

```bash
cd dashboard
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
streamlit run app.py
```

The app loads data from `../holdings.csv` and `../watchlist.csv` by default, so keep those files updated at the workspace root (or adjust the paths near the top of `app.py`).

Set a password by either exporting `APP_PASSWORD` before `streamlit run ...` or placing it inside `.streamlit/secrets.toml`.

## Deploy to Streamlit Community Cloud

1. Push this folder to a GitHub repo (e.g., `portfolio-radar`).
2. Go to <https://share.streamlit.io/>, click **New app**, pick the repo/branch, and set `dashboard/app.py` as the entry point.
3. Under **Advanced settings → Secrets**, add:
   ```
   APP_PASSWORD = "your-phrase"
   ```
4. Hit **Deploy**. Share the generated URL + password with whoever should have access.

## Other hosting options

- **Fly.io / Render / Railway**: create a Dockerfile that installs requirements and runs `streamlit run app.py --server.port $PORT --server.address 0.0.0.0`.
- **Self-host**: run on a VPS + reverse proxy (Caddy/NGINX) with basic auth.

## Customization hooks

- Update `watchlist.csv` with columns `Ticker,Name,Desired Entry USD,Notes` to drive the reminder logic.
- Adjust conviction tiers in `conviction_label()` if you want different cutoffs.
- Expand `MARKET_SUFFIX` / `MARKET_CURRENCY` mappings if you add EU or other listings.
