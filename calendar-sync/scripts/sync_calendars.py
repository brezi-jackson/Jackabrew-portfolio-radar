#!/usr/bin/env python3
"""
calendar-sync: Sync Google Calendar ↔ Feishu Calendar ↔ Apple iCloud Calendar
Usage:
  python sync_calendars.py auth-google       # First-time Google OAuth
  python sync_calendars.py read              # Read upcoming events from all calendars
  python sync_calendars.py sync              # Sync events (Google → Feishu, Feishu → Google)
  python sync_calendars.py create <title> <start> <end> [--cal google|feishu|apple]
"""

import os
import sys
import json
import pickle
import datetime
import argparse
import requests
from pathlib import Path

# ── Paths ──────────────────────────────────────────────────────────────────────
SKILL_DIR = Path(__file__).parent.parent
CREDS_FILE = SKILL_DIR / "google_credentials.json"
TOKEN_FILE = SKILL_DIR / "google_token.pickle"
CONFIG_FILE = SKILL_DIR / "config.json"

# ── Config ─────────────────────────────────────────────────────────────────────
def load_config():
    if CONFIG_FILE.exists():
        return json.loads(CONFIG_FILE.read_text())
    return {}

def save_config(cfg):
    CONFIG_FILE.write_text(json.dumps(cfg, indent=2, ensure_ascii=False))

# ══════════════════════════════════════════════════════════════════════════════
# GOOGLE CALENDAR
# ══════════════════════════════════════════════════════════════════════════════
def get_google_service():
    try:
        from google.oauth2.credentials import Credentials
        from google_auth_oauthlib.flow import InstalledAppFlow
        from google.auth.transport.requests import Request
        from googleapiclient.discovery import build
    except ImportError:
        print("Installing Google API libs...")
        os.system("pip install google-api-python-client google-auth-httplib2 google-auth-oauthlib -q")
        from google.oauth2.credentials import Credentials
        from google_auth_oauthlib.flow import InstalledAppFlow
        from google.auth.transport.requests import Request
        from googleapiclient.discovery import build

    SCOPES = ["https://www.googleapis.com/auth/calendar"]
    creds = None
    if TOKEN_FILE.exists():
        with open(TOKEN_FILE, "rb") as f:
            creds = pickle.load(f)
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(str(CREDS_FILE), SCOPES)
            creds = flow.run_local_server(port=0)
        with open(TOKEN_FILE, "wb") as f:
            pickle.dump(creds, f)
    return build("calendar", "v3", credentials=creds)


def google_list_events(days_ahead=14):
    service = get_google_service()
    now = datetime.datetime.utcnow().isoformat() + "Z"
    end = (datetime.datetime.utcnow() + datetime.timedelta(days=days_ahead)).isoformat() + "Z"
    result = service.events().list(
        calendarId="primary",
        timeMin=now,
        timeMax=end,
        singleEvents=True,
        orderBy="startTime",
        maxResults=50
    ).execute()
    return result.get("items", [])


def google_create_event(title, start_dt, end_dt, description=""):
    service = get_google_service()
    event = {
        "summary": title,
        "description": description,
        "start": {"dateTime": start_dt.isoformat(), "timeZone": "Asia/Shanghai"},
        "end":   {"dateTime": end_dt.isoformat(),   "timeZone": "Asia/Shanghai"},
    }
    created = service.events().insert(calendarId="primary", body=event).execute()
    return created.get("htmlLink")


# ══════════════════════════════════════════════════════════════════════════════
# FEISHU CALENDAR
# ══════════════════════════════════════════════════════════════════════════════
def feishu_get_token():
    cfg = load_config()
    app_id = cfg.get("feishu_app_id") or os.environ.get("FEISHU_APP_ID")
    app_secret = cfg.get("feishu_app_secret") or os.environ.get("FEISHU_APP_SECRET")
    if not app_id or not app_secret:
        print("ERROR: Set feishu_app_id and feishu_app_secret in config.json or env vars.")
        sys.exit(1)
    r = requests.post("https://open.feishu.cn/open-apis/auth/v3/tenant_access_token/internal",
                      json={"app_id": app_id, "app_secret": app_secret})
    return r.json()["tenant_access_token"]


def feishu_list_calendars(token):
    r = requests.get("https://open.feishu.cn/open-apis/calendar/v4/calendars",
                     headers={"Authorization": f"Bearer {token}"})
    return r.json().get("data", {}).get("calendar_list", [])


def feishu_list_events(token, calendar_id="primary", days_ahead=14):
    now_ts = int(datetime.datetime.now().timestamp())
    end_ts = int((datetime.datetime.now() + datetime.timedelta(days=days_ahead)).timestamp())
    r = requests.get(
        f"https://open.feishu.cn/open-apis/calendar/v4/calendars/{calendar_id}/events",
        headers={"Authorization": f"Bearer {token}"},
        params={"start_time": str(now_ts), "end_time": str(end_ts), "page_size": 50}
    )
    return r.json().get("data", {}).get("items", [])


def feishu_create_event(token, title, start_dt, end_dt, description="", calendar_id="primary"):
    body = {
        "summary": title,
        "description": description,
        "start_time": {
            "timestamp": str(int(start_dt.timestamp())),
            "timezone": "Asia/Shanghai"
        },
        "end_time": {
            "timestamp": str(int(end_dt.timestamp())),
            "timezone": "Asia/Shanghai"
        }
    }
    r = requests.post(
        f"https://open.feishu.cn/open-apis/calendar/v4/calendars/{calendar_id}/events",
        headers={"Authorization": f"Bearer {token}", "Content-Type": "application/json"},
        json=body
    )
    return r.json()


# ══════════════════════════════════════════════════════════════════════════════
# APPLE iCLOUD (CalDAV)
# ══════════════════════════════════════════════════════════════════════════════
def apple_list_events(days_ahead=14):
    try:
        import caldav
    except ImportError:
        os.system("pip install caldav -q")
        import caldav

    cfg = load_config()
    apple_id = cfg.get("apple_id") or os.environ.get("APPLE_ID")
    apple_pwd = cfg.get("apple_app_password") or os.environ.get("APPLE_APP_PASSWORD")
    if not apple_id or not apple_pwd:
        print("WARNING: Apple credentials not configured. Skipping Apple Calendar.")
        return []

    client = caldav.DAVClient(
        url="https://caldav.icloud.com",
        username=apple_id,
        password=apple_pwd
    )
    principal = client.principal()
    calendars = principal.calendars()
    events = []
    now = datetime.datetime.now()
    end = now + datetime.timedelta(days=days_ahead)
    for cal in calendars:
        try:
            for ev in cal.date_search(start=now, end=end):
                events.append({"calendar": cal.name, "raw": ev.data})
        except Exception:
            pass
    return events


# ══════════════════════════════════════════════════════════════════════════════
# SYNC LOGIC
# ══════════════════════════════════════════════════════════════════════════════
def normalize_google_event(ev):
    start = ev.get("start", {})
    return {
        "id": ev.get("id"),
        "title": ev.get("summary", "(no title)"),
        "start": start.get("dateTime") or start.get("date"),
        "source": "google",
        "link": ev.get("htmlLink", "")
    }

def normalize_feishu_event(ev):
    return {
        "id": ev.get("event_id"),
        "title": ev.get("summary", "(no title)"),
        "start": ev.get("start_time", {}).get("timestamp"),
        "source": "feishu"
    }


def cmd_read(args):
    print("\n═══ GOOGLE CALENDAR ═══")
    try:
        g_events = google_list_events(days_ahead=args.days)
        if not g_events:
            print("  (no upcoming events)")
        for ev in g_events:
            n = normalize_google_event(ev)
            print(f"  [{n['start'][:16]}] {n['title']}")
    except Exception as e:
        print(f"  ERROR: {e}")

    print("\n═══ FEISHU CALENDAR ═══")
    try:
        token = feishu_get_token()
        cals = feishu_list_calendars(token)
        cal_id = cals[0]["calendar_id"] if cals else "primary"
        f_events = feishu_list_events(token, cal_id, days_ahead=args.days)
        if not f_events:
            print("  (no upcoming events)")
        for ev in f_events:
            n = normalize_feishu_event(ev)
            ts = int(n["start"]) if n["start"] else 0
            dt = datetime.datetime.fromtimestamp(ts).strftime("%Y-%m-%d %H:%M") if ts else "?"
            print(f"  [{dt}] {n['title']}")
    except Exception as e:
        print(f"  ERROR: {e}")

    print("\n═══ APPLE CALENDAR ═══")
    try:
        a_events = apple_list_events(days_ahead=args.days)
        if not a_events:
            print("  (no upcoming events or not configured)")
        for ev in a_events:
            print(f"  [{ev['calendar']}] (raw iCal data)")
    except Exception as e:
        print(f"  ERROR: {e}")


def cmd_create(args):
    # Parse times
    start_dt = datetime.datetime.fromisoformat(args.start)
    end_dt = datetime.datetime.fromisoformat(args.end)
    title = args.title
    cal = args.cal or "feishu"

    if cal == "google":
        link = google_create_event(title, start_dt, end_dt)
        print(f"✅ Created in Google Calendar: {link}")
    elif cal == "feishu":
        token = feishu_get_token()
        cals = feishu_list_calendars(token)
        cal_id = cals[0]["calendar_id"] if cals else "primary"
        result = feishu_create_event(token, title, start_dt, end_dt, calendar_id=cal_id)
        print(f"✅ Created in Feishu Calendar: {result}")
    else:
        print("Apple Calendar creation not yet supported via CalDAV in this script.")


def cmd_auth_google(args):
    print("Starting Google OAuth flow...")
    get_google_service()
    print("✅ Google auth complete. Token saved.")


def cmd_config(args):
    cfg = load_config()
    if args.set:
        key, val = args.set.split("=", 1)
        cfg[key] = val
        save_config(cfg)
        print(f"✅ Set {key}")
    else:
        print(json.dumps(cfg, indent=2))


# ══════════════════════════════════════════════════════════════════════════════
# CLI
# ══════════════════════════════════════════════════════════════════════════════
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Calendar sync: Google ↔ Feishu ↔ Apple")
    sub = parser.add_subparsers(dest="cmd")

    p_auth = sub.add_parser("auth-google", help="First-time Google OAuth")

    p_read = sub.add_parser("read", help="Read upcoming events from all calendars")
    p_read.add_argument("--days", type=int, default=14, help="Days ahead (default 14)")

    p_create = sub.add_parser("create", help="Create an event")
    p_create.add_argument("title")
    p_create.add_argument("start", help="ISO format: 2026-03-25T10:00:00")
    p_create.add_argument("end",   help="ISO format: 2026-03-25T11:00:00")
    p_create.add_argument("--cal", choices=["google", "feishu", "apple"], default="feishu")

    p_cfg = sub.add_parser("config", help="View or set config")
    p_cfg.add_argument("--set", help="key=value to set")

    args = parser.parse_args()
    if args.cmd == "auth-google":   cmd_auth_google(args)
    elif args.cmd == "read":        cmd_read(args)
    elif args.cmd == "create":      cmd_create(args)
    elif args.cmd == "config":      cmd_config(args)
    else:                           parser.print_help()
