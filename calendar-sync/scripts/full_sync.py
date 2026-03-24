#!/usr/bin/env python3
"""
full_sync.py — Feishu ↔ Google ↔ Apple full bidirectional sync
Usage:
  python full_sync.py                    # Sync all calendars
  python full_sync.py --dry-run          # Preview what would be synced
  python full_sync.py --source feishu    # Only push Feishu events out
"""

import os, sys, json, pickle, datetime, hashlib, argparse
import requests
from pathlib import Path

SKILL_DIR   = Path(__file__).parent.parent
TOKEN_FILE  = SKILL_DIR / "google_token.pickle"
CREDS_FILE  = SKILL_DIR / "google_credentials.json"
CONFIG_FILE = SKILL_DIR / "config.json"
SYNC_LOG    = SKILL_DIR / "sync_state.json"   # tracks which events we've already synced

def load_config():
    return json.loads(CONFIG_FILE.read_text()) if CONFIG_FILE.exists() else {}

def load_sync_log():
    return json.loads(SYNC_LOG.read_text()) if SYNC_LOG.exists() else {}

def save_sync_log(log):
    SYNC_LOG.write_text(json.dumps(log, indent=2, ensure_ascii=False))

def event_fingerprint(title, start):
    """Stable ID based on title + start time to detect duplicates"""
    return hashlib.md5(f"{title}|{start}".encode()).hexdigest()[:12]


# ── Google ─────────────────────────────────────────────────────────────────────
def get_google_service():
    from google.oauth2.credentials import Credentials
    from google.auth.transport.requests import Request
    from googleapiclient.discovery import build
    with open(TOKEN_FILE, "rb") as f:
        creds = pickle.load(f)
    if creds.expired and creds.refresh_token:
        creds.refresh(Request())
        with open(TOKEN_FILE, "wb") as f:
            pickle.dump(creds, f)
    return build("calendar", "v3", credentials=creds)

def google_get_events(days=30):
    svc = get_google_service()
    now = datetime.datetime.utcnow().isoformat() + "Z"
    end = (datetime.datetime.utcnow() + datetime.timedelta(days=days)).isoformat() + "Z"
    items = svc.events().list(
        calendarId="primary", timeMin=now, timeMax=end,
        singleEvents=True, orderBy="startTime", maxResults=100
    ).execute().get("items", [])
    results = []
    for ev in items:
        start = ev.get("start", {})
        start_str = start.get("dateTime") or start.get("date", "")
        results.append({
            "id": ev.get("id"), "title": ev.get("summary", "(no title)"),
            "start": start_str, "source": "google",
            "description": ev.get("description", "")
        })
    return results

def google_create_event(title, start_dt, end_dt, description=""):
    svc = get_google_service()
    event = {
        "summary": title, "description": description,
        "start": {"dateTime": start_dt.isoformat(), "timeZone": "Asia/Shanghai"},
        "end":   {"dateTime": end_dt.isoformat(),   "timeZone": "Asia/Shanghai"},
    }
    return svc.events().insert(calendarId="primary", body=event).execute()


# ── Feishu ─────────────────────────────────────────────────────────────────────
def feishu_token():
    """Returns user token if available, falls back to tenant token."""
    cfg = load_config()
    user_token = cfg.get("feishu_user_token")
    if user_token:
        return user_token
    r = requests.post(
        "https://open.feishu.cn/open-apis/auth/v3/tenant_access_token/internal",
        json={"app_id": cfg["feishu_app_id"], "app_secret": cfg["feishu_app_secret"]}
    )
    return r.json()["tenant_access_token"]

def feishu_primary_calendar(token):
    r = requests.get("https://open.feishu.cn/open-apis/calendar/v4/calendars",
                     headers={"Authorization": f"Bearer {token}"})
    cals = r.json().get("data", {}).get("calendar_list", [])
    # Prefer primary calendar
    for c in cals:
        if c.get("type") == "primary":
            return c["calendar_id"]
    return cals[0]["calendar_id"] if cals else "primary"

def feishu_get_events(days=30):
    token = feishu_token()
    cal_id = feishu_primary_calendar(token)
    now_ts = int(datetime.datetime.now().timestamp())
    end_ts = now_ts + days * 86400
    r = requests.get(
        f"https://open.feishu.cn/open-apis/calendar/v4/calendars/{cal_id}/events",
        headers={"Authorization": f"Bearer {token}"},
        params={"start_time": str(now_ts), "end_time": str(end_ts), "page_size": 100}
    )
    items = r.json().get("data", {}).get("items", [])
    results = []
    for ev in items:
        ts = int(ev.get("start_time", {}).get("timestamp", 0))
        start_str = datetime.datetime.fromtimestamp(ts).isoformat() if ts else ""
        results.append({
            "id": ev.get("event_id"), "title": ev.get("summary", "(no title)"),
            "start": start_str, "source": "feishu",
            "description": ev.get("description", "")
        })
    return results

def feishu_create_event(title, start_dt, end_dt, description=""):
    token = feishu_token()
    cal_id = feishu_primary_calendar(token)
    body = {
        "summary": title, "description": description,
        "start_time": {"timestamp": str(int(start_dt.timestamp())), "timezone": "Asia/Shanghai"},
        "end_time":   {"timestamp": str(int(end_dt.timestamp())),   "timezone": "Asia/Shanghai"},
    }
    r = requests.post(
        f"https://open.feishu.cn/open-apis/calendar/v4/calendars/{cal_id}/events",
        headers={"Authorization": f"Bearer {token}", "Content-Type": "application/json"},
        json=body
    )
    return r.json()


# ── Apple (CalDAV) ─────────────────────────────────────────────────────────────
def apple_get_events(days=30):
    import caldav
    cfg = load_config()
    client = caldav.DAVClient(
        url="https://caldav.icloud.com",
        username=cfg["apple_id"], password=cfg["apple_app_password"]
    )
    principal = client.principal()
    results = []
    now = datetime.datetime.now()
    end = now + datetime.timedelta(days=days)
    for cal in principal.calendars():
        if "Reminder" in (cal.name or ""):
            continue
        try:
            for ev in cal.search(start=now, end=end, event=True):
                try:
                    comp = ev.icalendar_component
                    title = str(comp.get("SUMMARY", "(no title)"))
                    dtstart = comp.get("DTSTART")
                    start_str = dtstart.dt.isoformat() if dtstart else ""
                    results.append({
                        "id": str(comp.get("UID", "")), "title": title,
                        "start": start_str, "source": f"apple:{cal.name}",
                        "description": str(comp.get("DESCRIPTION", ""))
                    })
                except Exception:
                    pass
        except Exception:
            pass
    return results

def apple_create_event(title, start_dt, end_dt, description="", cal_name="Work"):
    import caldav
    from icalendar import Calendar as iCal, Event as iEvent
    import uuid
    cfg = load_config()
    client = caldav.DAVClient(
        url="https://caldav.icloud.com",
        username=cfg["apple_id"], password=cfg["apple_app_password"]
    )
    principal = client.principal()
    target = None
    for cal in principal.calendars():
        if cal.name == cal_name:
            target = cal
            break
    if not target:
        target = principal.calendars()[0]

    cal_obj = iCal()
    cal_obj.add("prodid", "-//calendar-sync//EN")
    cal_obj.add("version", "2.0")
    ev = iEvent()
    ev.add("summary", title)
    ev.add("dtstart", start_dt)
    ev.add("dtend", end_dt)
    ev.add("description", description)
    ev.add("uid", str(uuid.uuid4()))
    cal_obj.add_component(ev)
    target.save_event(cal_obj.to_ical())


# ── Sync Engine ────────────────────────────────────────────────────────────────
def sync_all(dry_run=False, source=None):
    print("Loading sync state...")
    log = load_sync_log()
    synced = log.get("synced_fingerprints", [])

    print("Fetching events from all calendars...")
    all_events = []
    
    if source in (None, "feishu"):
        try:
            all_events += feishu_get_events()
            print(f"  Feishu: {sum(1 for e in all_events if e['source']=='feishu')} events")
        except Exception as e:
            print(f"  Feishu ERROR: {e}")

    if source in (None, "google"):
        try:
            g = google_get_events()
            all_events += g
            print(f"  Google: {len(g)} events")
        except Exception as e:
            print(f"  Google ERROR: {e}")

    if source in (None, "apple"):
        try:
            a = apple_get_events()
            all_events += a
            print(f"  Apple: {len(a)} events")
        except Exception as e:
            print(f"  Apple ERROR: {e}")

    # Deduplicate by fingerprint
    new_syncs = 0
    for ev in all_events:
        fp = event_fingerprint(ev["title"], ev["start"])
        if fp in synced:
            continue  # Already synced this event

        # Parse start/end time
        try:
            start_dt = datetime.datetime.fromisoformat(ev["start"].replace("Z", "+00:00"))
            end_dt = start_dt + datetime.timedelta(hours=1)
        except Exception:
            continue

        src = ev["source"]
        title = ev["title"]
        desc = f"[Synced from {src}]\n{ev.get('description', '')}"

        actions = []
        if src == "feishu":
            actions = ["google", "apple"]  # Feishu → Google + Apple directly
        elif src == "google":
            actions = ["feishu"]  # Google → Feishu (Apple gets it via native sync)
        elif src.startswith("apple"):
            actions = ["feishu", "google"]  # Apple → both

        for target in actions:
            print(f"  {'[DRY RUN] ' if dry_run else ''}Copying [{src}] → [{target}]: {title} @ {ev['start'][:16]}")
            if not dry_run:
                try:
                    if target == "google":
                        google_create_event(title, start_dt, end_dt, desc)
                    elif target == "feishu":
                        feishu_create_event(title, start_dt, end_dt, desc)
                    elif target == "apple":
                        apple_create_event(title, start_dt, end_dt, desc)
                    new_syncs += 1
                except Exception as e:
                    print(f"    ERROR creating in {target}: {e}")

        synced.append(fp)

    log["synced_fingerprints"] = synced[-500:]  # Keep last 500
    log["last_sync"] = datetime.datetime.now().isoformat()
    if not dry_run:
        save_sync_log(log)

    print(f"\n✅ Sync complete. {new_syncs} new events copied.")
    if dry_run:
        print("   (Dry run — nothing was written)")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--source", choices=["feishu", "google", "apple"])
    args = parser.parse_args()
    sync_all(dry_run=args.dry_run, source=args.source)
