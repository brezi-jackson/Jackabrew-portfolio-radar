---
name: calendar-sync
description: "Sync and manage events across Google Calendar, Feishu Calendar, and Apple iCloud Calendar. Use when reading Jackson's schedule from any of the three calendars, creating 1on1s or meetings, checking availability, or syncing events between platforms. Triggers on: check my calendar, schedule a 1on1, what's on my calendar, create a meeting, sync calendars, what do I have this week."
---

# Calendar Sync

Reads and writes events across Google Calendar, Feishu Calendar, and Apple iCloud via a Python script.

## Script Location
`scripts/sync_calendars.py` — all calendar operations go through this script.

## Setup (first time)
See `references/setup.md` for credential setup. Config stored in `calendar-sync/config.json`.

## Common Operations

### Read upcoming events (all calendars)
```bash
cd /Users/jackson/.openclaw/workspace/calendar-sync
python scripts/sync_calendars.py read --days 14
```

### Create a meeting
```bash
python scripts/sync_calendars.py create "TITLE" START_ISO END_ISO --cal [google|feishu|apple]
# Example: 1on1 with 姜月 tomorrow 10am-10:30am
python scripts/sync_calendars.py create "1on1 - 姜月" 2026-03-25T10:00:00 2026-03-25T10:30:00 --cal feishu
```

### First-time Google auth
```bash
python scripts/sync_calendars.py auth-google
```
This opens a browser. After sign-in, token is saved to `google_token.pickle` — no re-auth needed.

## Credentials Status
- Google: `google_credentials.json` ✅ present — run `auth-google` once to complete OAuth
- Feishu: set `feishu_app_id` + `feishu_app_secret` in config.json
- Apple: set `apple_id` + `apple_app_password` in config.json (optional)

## Notes
- Feishu calendar API uses Unix timestamps; Google uses ISO 8601
- Apple CalDAV requires an app-specific password (not the main iCloud password)
- Default calendar for new events: Feishu (change with `--cal` flag)
