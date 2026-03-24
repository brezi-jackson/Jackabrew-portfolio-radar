#!/usr/bin/env python3
"""
Feishu User OAuth — get user access token for reading personal calendar
Run once: python feishu_auth.py
"""
import json, http.server, webbrowser, urllib.parse, requests, threading
from pathlib import Path

SKILL_DIR = Path(__file__).parent.parent
CONFIG_FILE = SKILL_DIR / "config.json"

def load_config():
    return json.loads(CONFIG_FILE.read_text())

def save_config(cfg):
    CONFIG_FILE.write_text(json.dumps(cfg, indent=2, ensure_ascii=False))

cfg = load_config()
APP_ID = cfg["feishu_app_id"]
APP_SECRET = cfg["feishu_app_secret"]
REDIRECT_URI = "http://localhost:9988/callback"
SCOPE = "calendar:calendar"

auth_code = None

class Handler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        global auth_code
        parsed = urllib.parse.urlparse(self.path)
        params = urllib.parse.parse_qs(parsed.query)
        if "code" in params:
            auth_code = params["code"][0]
            self.send_response(200)
            self.end_headers()
            self.wfile.write(b"<h2>Auth complete! Return to terminal.</h2>")
        else:
            self.send_response(400)
            self.end_headers()
    def log_message(self, *args): pass

def run_server():
    server = http.server.HTTPServer(("localhost", 9988), Handler)
    server.handle_request()

# Start local callback server
t = threading.Thread(target=run_server)
t.start()

# Open browser for auth
auth_url = (
    f"https://open.feishu.cn/open-apis/authen/v1/authorize"
    f"?app_id={APP_ID}&redirect_uri={urllib.parse.quote(REDIRECT_URI)}"
    f"&scope={SCOPE}&response_type=code"
)
print(f"Opening browser for Feishu auth...")
print(f"If browser doesn't open, visit:\n{auth_url}\n")
webbrowser.open(auth_url)

t.join()

if not auth_code:
    print("No auth code received.")
    exit(1)

# Step 1: Get app_access_token
r0 = requests.post(
    "https://open.feishu.cn/open-apis/auth/v3/app_access_token/internal",
    json={"app_id": APP_ID, "app_secret": APP_SECRET}
)
app_token = r0.json().get("app_access_token")
print(f"app_access_token: {'ok' if app_token else 'FAILED'}")

# Step 2: Exchange code for user token
r = requests.post(
    "https://open.feishu.cn/open-apis/authen/v1/oidc/access_token",
    headers={"Authorization": f"Bearer {app_token}", "Content-Type": "application/json"},
    json={"grant_type": "authorization_code", "code": auth_code}
)
data = r.json()
print("Response:", json.dumps(data, indent=2))

user_token = data.get("data", {}).get("access_token")
refresh_token = data.get("data", {}).get("refresh_token")

if user_token:
    cfg["feishu_user_token"] = user_token
    cfg["feishu_refresh_token"] = refresh_token or ""
    save_config(cfg)
    print(f"\n✅ Feishu user auth complete. Token saved.")
else:
    print("\n❌ Failed to get user token. Check app permissions.")
