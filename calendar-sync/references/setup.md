# Calendar Sync — Setup Guide

## Required Credentials

### 1. Google Calendar
- `google_credentials.json` — already in skill directory (OAuth Desktop app)
- First run: `python scripts/sync_calendars.py auth-google` → browser opens → sign in → token saved

### 2. Feishu Calendar
Set in `config.json`:
```json
{
  "feishu_app_id": "cli_xxxxxxxx",
  "feishu_app_secret": "xxxxxxxx"
}
```
Or via env: `FEISHU_APP_ID` / `FEISHU_APP_SECRET`

### 3. Apple iCloud (optional)
1. Go to https://appleid.apple.com → Security → App-Specific Passwords → Generate
2. Set in config.json:
```json
{
  "apple_id": "you@icloud.com",
  "apple_app_password": "xxxx-xxxx-xxxx-xxxx"
}
```

## config.json helper
```bash
python scripts/sync_calendars.py config --set feishu_app_id=cli_xxx
python scripts/sync_calendars.py config --set feishu_app_secret=xxx
python scripts/sync_calendars.py config --set apple_id=you@icloud.com
python scripts/sync_calendars.py config --set apple_app_password=xxxx-xxxx-xxxx-xxxx
```

## Usage Examples
```bash
# Authenticate Google (first time only)
python scripts/sync_calendars.py auth-google

# Read next 14 days from all calendars
python scripts/sync_calendars.py read

# Read next 7 days
python scripts/sync_calendars.py read --days 7

# Create a 1on1 in Feishu
python scripts/sync_calendars.py create "1on1 - 姜月" 2026-03-25T10:00:00 2026-03-25T10:30:00 --cal feishu

# Create in Google Calendar
python scripts/sync_calendars.py create "Team Sync" 2026-03-25T14:00:00 2026-03-25T15:00:00 --cal google
```
