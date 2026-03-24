# Brezi iOS App — Wireframe & Design Spec v2.1
**Author:** Picasso (Product Design) | March 19, 2026 | Round 2 — Post-critique refinement
**For:** Jensen (Engineering) + Jackson (Product) + Elon (Growth/Copy)

---

## What Changed in This Round and Why

Elon's critique landed on three structural gaps: the dashboard dies after brewing, the insight copy reads like a fitness tracker, and device pairing only covers the happy path. All three are fixed. Nine other changes made it in. Two items I pushed back on (community signal on dashboard, "Done" vs "Brewed"). Full changelog at the bottom.

This spec supersedes v2.0 in all areas where they conflict.

---

## 0. Design Tokens

*Unchanged from v2.0 — included here for completeness.*

### Colors
```swift
let black        = Color(hex: "#000000")
let white        = Color(hex: "#FFFFFF")
let offWhite     = Color(hex: "#F5F5F5")
let blue         = Color(hex: "#488BED")
let midGrey      = Color(hex: "#888888")
let lightGrey    = Color(hex: "#E0E0E0")
let darkCard     = Color(hex: "#0F0F0F")
let darkSurface  = Color(hex: "#1A1A1A")

// Status
let statusGreen  = Color(hex: "#34C759")
let statusYellow = Color(hex: "#FFD60A")
let statusRed    = Color(hex: "#FF453A")
let statusBlue   = Color(hex: "#488BED")

// NEW — amber warning for manual override out-of-range
let statusAmber  = Color(hex: "#FF9F0A")
```

### Typography
```swift
let t_display  = Font.system(size: 48, weight: .heavy)
let t_h1       = Font.system(size: 34, weight: .bold)
let t_h2       = Font.system(size: 26, weight: .bold)
let t_h3       = Font.system(size: 20, weight: .semibold)
let t_body     = Font.system(size: 16, weight: .regular)
let t_small    = Font.system(size: 14, weight: .regular)
let t_mono     = Font.system(size: 13, weight: .regular, design: .monospaced)
let t_eyebrow  = Font.system(size: 11, weight: .medium, design: .monospaced)
let t_data     = Font.system(size: 28, weight: .bold, design: .monospaced)
```

### Spacing (8pt grid)
```
xs: 4    sm: 8    md: 16    lg: 24    xl: 32    xxl: 48    xxxl: 64
```

### Corner Radii
```
card: 16px    button: 12px    chip: 8px    input: 10px    brew-card: 24px
```

### Shadows
```swift
.shadow(color: .black.opacity(0.08), radius: 16, x: 0, y: 4)  // light cards
// No shadow on dark cards or dark backgrounds
```

---

## 1. Information Architecture + Nav Map

### Navigation Pattern
Single push-stack with modal sheets. No tab bar.

```
AppRoot
 └── OnboardingFlow (5 screens, first launch only)
      │  ├── 1. BrandIntroView
      │  ├── 2. DevicePairView         ← Bluetooth pair + failure states
      │  ├── 3. WearableConnectView    ← WHOOP / Oura / Apple Health / Skip
      │  ├── 4. DataChainTutorialView  ← shows data → insight → brew chain
      │  └── 5. FirstCheckInView       ← recovery input → lands on Dashboard
      │
      └── MainFlow
           ├── DashboardView                  ← THREE STATES: pre-brew / brewing / post-brew
           ├── BrewRecommendationView          ← full brew detail
           ├── HistoryView                     ← brew log + 7-day spark line + correlation
           ├── DeviceHubView                   ← manage Brezi + wearables + future devices
           └── SettingsView                    ← preferences, units, about
```

### Navigation Bar (persistent, post-onboarding)
- Background: `#000000`
- Center: "bre:zi" logo — `ABCSocial-CondensedBold` / 18px / white (use "BREZI" in system font for prototype)
- Left: Context-dependent — back arrow when pushed, nothing at root
- Right slot 1: Device hub icon (SF Symbol: `antenna.radiowaves.left.and.right`) — white, 20pt
- Right slot 2: Settings icon (SF Symbol: `gearshape`) — white, 20pt
- Height: 56px
- No bottom tab bar

---

## 2. Onboarding Flow (5 Screens)

**Trigger:** First launch only. Store `hasCompletedOnboarding: Bool` in UserDefaults.

### Shared Onboarding Shell
- Background: `#000000` full bleed
- Progress: 5 horizontal segments at top, 2px height, 4px gap between
  - Completed: `#488BED`
  - Current: `#488BED` with subtle pulse
  - Upcoming: `#333333`
- All text: white unless noted
- Back swipe: enabled (system gesture)
- Skip: text button top-right, `#888888` — only on screens 2 and 3

---

### Onboarding Screen 1 — Brand Moment (`BrandIntroView`)

**Purpose:** Establish identity. No skip button. This earns 8 seconds.

**[CHANGED] Body copy rewritten — cut the defensive pitch. User already bought the machine.**

```
┌─────────────────────────────────┐
│  ■■■■■  ─────  ─────  ─────  ───│  ← progress bar (segment 1 active)
│                                 │
│                                 │
│                                 │
│                                 │
│                                 │
│                                 │
│  Cold is                        │
│  a practice.                    │  ← CondensedBold, 52px, white
│                                 │
│  Brezi reads your body and      │
│  builds your brew. Cold.        │  ← Regular, 17px, #CCCCCC, line-height 1.5
│  Precise. Personal.             │
│  Every morning.                 │
│                                 │
│                                 │
│                                 │
│  ┌─────────────────────────┐    │
│  │  START THE PRACTICE  →  │    │  ← blue filled, 54px tall, ALL CAPS
│  └─────────────────────────┘    │
│                                 │
└─────────────────────────────────┘
```

**Motion:** Headline fades up from 12px below (0.4s ease-out, 0.1s delay). Body fades in 0.3s after headline. Button fades in last (0.2s, 0.3s delay).

**Why the copy changed:** The old body ("Most people compromise their coffee. Ice dilutes it...") was a sales pitch to someone who already bought. The new copy is forward-looking and specific — it tells the user what the product does, not what other products do wrong.

---

### Onboarding Screen 2 — Device Pairing (`DevicePairView`)

**Purpose:** Pair the Brezi machine via Bluetooth. This is the product.

**[CHANGED] Subhead rewritten. Four failure states added.**

```
┌─────────────────────────────────┐
│  ■■■■■  ■■■■■  ─────  ─────  ───│
│                            Skip │  ← #888888
│                                 │
│                                 │
│                                 │
│         ┌───────────┐           │
│         │           │           │
│         │   ╔═══╗   │           │  ← Brezi machine illustration
│         │   ║ b ║   │           │     (placeholder: SF Symbol
│         │   ╚═══╝   │           │      `cup.and.saucer.fill` at 80pt,
│         │           │           │      white, centered)
│         └───────────┘           │
│                                 │
│  Pair your Brezi.               │  ← CondensedBold, 38px, white
│                                 │
│  Power on your Brezi.           │
│  We'll find each other.         │  ← Regular, 17px, #CCCCCC [CHANGED]
│                                 │
│  ┌─────────────────────────┐    │
│  │  SEARCH FOR BREZI       │    │  ← blue filled button
│  └─────────────────────────┘    │
│                                 │
│  I don't have one yet ›         │  ← text link, #888888, centered
│                                 │
└─────────────────────────────────┘
```

**State: Searching**
```
│  ┌─────────────────────────┐    │
│  │  ◌  SEARCHING...        │    │  ← blue button, spinner left of text
│  └─────────────────────────┘    │
│                                 │
│  Make sure Brezi is powered on  │  ← Mono, 12px, #888888
│  and within range.              │
```

**State: Found / Paired (happy path)**
```
│  ┌─────────────────────────────┐│
│  │  ✓  BREZI-A4F2              ││  ← green border, #34C759
│  │     Connected                ││  ← Mono, 12px, #34C759
│  │     Firmware 1.2.0           ││  ← Mono, 11px, #888888
│  └─────────────────────────────┘│
│                                 │
│  ┌─────────────────────────┐    │
│  │  CONTINUE  →            │    │  ← blue filled
│  └─────────────────────────┘    │
```

**[NEW] State: Bluetooth Off (system-level)**
```
│                                 │
│  ┌─────────────────────────────┐│
│  │  ⚠  Bluetooth is off.      ││  ← Regular, 16px, white
│  │                             ││
│  │  Brezi needs Bluetooth to   ││  ← Regular, 14px, #CCCCCC
│  │  connect to your machine.   ││
│  │                             ││
│  │  [Open Settings]            ││  ← text link, #488BED
│  └─────────────────────────────┘│  ← inline card, #1A1A1A, amber left border 3px
│                                 │
│  Skip for now ›                 │  ← text link, #888888
│                                 │
```
No modal. Inline card keeps user in context. "Open Settings" deep-links to iOS Bluetooth settings via `UIApplication.openSettingsURLString`.

**[NEW] State: Device Not Found (30s timeout)**
```
│                                 │
│  ┌─────────────────────────────┐│
│  │  Can't find your Brezi yet. ││  ← Regular, 16px, white
│  │                             ││
│  │  ○ Is it plugged in?        ││
│  │  ○ Is it within 10 feet?    ││  ← Regular, 14px, #CCCCCC
│  │  ○ Try turning it off       ││     checklist format
│  │    and back on.             ││
│  │                             ││
│  │  ┌─────────────────────┐   ││
│  │  │  TRY AGAIN          │   ││  ← blue outlined button
│  │  └─────────────────────┘   ││
│  └─────────────────────────────┘│  ← inline card, #1A1A1A, amber left border
│                                 │
│  Skip for now ›                 │  ← text link, #888888
│                                 │
```
"Try Again" resets the 30s Bluetooth scan. Troubleshooting checklist uses plain language, not error codes.

**[NEW] State: Device Paired to Another Phone**
```
│                                 │
│  ┌─────────────────────────────┐│
│  │  This Brezi is connected    ││  ← Regular, 16px, white
│  │  to another device.         ││
│  │                             ││
│  │  Disconnect it from the     ││  ← Regular, 14px, #CCCCCC
│  │  other phone first, then    ││
│  │  try again here.            ││
│  │                             ││
│  │  ┌─────────────────────┐   ││
│  │  │  TRY AGAIN          │   ││  ← blue outlined button
│  │  └─────────────────────┘   ││
│  └─────────────────────────────┘│  ← inline card, #1A1A1A, red left border 3px
│                                 │
│  Skip for now ›                 │  ← text link, #888888
│                                 │
```
This is a Bluetooth limitation — we can't force-unpair from another device. The copy is honest about what the user needs to do.

**[NEW] State: Firmware Update Required**
```
│                                 │
│  ┌─────────────────────────────┐│
│  │  ✓  BREZI-A4F2  ·  Found   ││  ← green border
│  │                             ││
│  │  Your Brezi needs a quick   ││  ← Regular, 14px, #CCCCCC
│  │  update before pairing.     ││
│  │  About 2 minutes.           ││
│  │                             ││
│  │  ████████░░░░░░░░  43%      ││  ← progress bar, #488BED fill
│  │                             ││
│  │  ┌─────────────────────┐   ││
│  │  │  UPDATE NOW         │   ││  ← blue filled button
│  │  └─────────────────────┘   ││
│  └─────────────────────────────┘│
│                                 │
```
Blocks pairing until firmware is current. Progress bar shows real OTA progress. "UPDATE NOW" changes to a progress bar once tapped. On completion, transitions to "Found / Paired" state automatically.

**"I don't have one yet" → Skip pairing:**
- Store `breziPaired: false`
- Show toast: `"No problem. Pair anytime from Device Hub."` — Mono, 13px, white on dark
- Advance to Screen 3

**Bluetooth permission:** Request `CBCentralManager` authorization. If denied:
```
Bluetooth access needed to find your Brezi.
[Open Settings]    ← text link, #488BED
```

---

### Onboarding Screen 3 — Wearable Connect (`WearableConnectView`)

**[CHANGED] Headline rewritten.**

```
┌─────────────────────────────────┐
│  ■■■■■  ■■■■■  ■■■■■  ─────  ───│
│                            Skip │
│                                 │
│  Your coffee should             │
│  know how you slept.            │  ← CondensedBold, 40px, white
│                                 │
│  Connect a wearable and Brezi   │
│  pulls your recovery data       │  ← Regular, 17px, #CCCCCC
│  automatically. No typing.      │
│                                 │
│  ┌─────────────────────────────┐│
│  │  ○  WHOOP                   ││  ← wearable row
│  │     HRV · Recovery · Sleep  ││  ← Mono, 11px, #888888
│  └─────────────────────────────┘│
│  ┌─────────────────────────────┐│
│  │  ○  Oura Ring               ││
│  │     HRV · Readiness · Sleep ││
│  └─────────────────────────────┘│
│  ┌─────────────────────────────┐│
│  │  ○  Apple Health            ││
│  │     HRV · Sleep · Activity  ││
│  └─────────────────────────────┘│
│                                 │
│  ┌─────────────────────────┐    │
│  │  CONNECT  →             │    │  ← blue, disabled until selection
│  └─────────────────────────┘    │
│                                 │
│  I'll log manually ›            │  ← text link, #888888
│                                 │
└─────────────────────────────────┘
```

**Wearable row spec:**
- Background: `#1A1A1A`, border `#333333` 1px, radius 12px
- Height: 64px, padding 16px horizontal
- Radio circle: 20px, `#333333` border, empty
- Selected: circle fill `#488BED`, white checkmark, row border `#488BED`
- Name: Regular, 17px, white
- Capabilities: Mono, 11px, `#888888`

**Connect flow:** OAuth sheet (WHOOP/Oura) or HealthKit permission dialog (Apple Health). On success:
```
│  ┌─────────────────────────────┐│
│  │  ✓  WHOOP  ·  Connected     ││  ← green accent, checkmark
│  │     Last sync: Just now      ││
│  └─────────────────────────────┘│
```

**"I'll log manually" → Skip:**
- Store `wearableSource: "manual"`
- Advance to Screen 4

---

### Onboarding Screen 4 — The Chain Tutorial (`DataChainTutorialView`)

**[CHANGED] Headline rewritten. CTA changed from "GOT IT" to "I SEE IT".**

```
┌─────────────────────────────────┐
│  ■■■■■  ■■■■■  ■■■■■  ■■■■■  ───│
│                                 │
│  Your data becomes              │
│  your brew.                     │  ← CondensedBold, 36px, white [CHANGED]
│                                 │
│  ┌─ YOUR DATA ────────────────┐ │
│  │  ↓                         │ │
│  │  HRV: 42ms                 │ │  ← Mono, 16px, white
│  │  Recovery: 61%             │ │
│  │  Sleep: 6h 48m             │ │
│  │  Source: WHOOP             │ │  ← Mono, 11px, #888888
│  └────────────────────────────┘ │  ← dark card, #1A1A1A
│           │                     │
│           ▼                     │  ← blue connecting line, animated
│  ┌─ BREZI'S READ ─────────────┐ │
│  │                             │ │
│  │  "Mid recovery. Your HRV   │ │
│  │   is 18% below baseline.   │ │  ← Regular, 15px, #CCCCCC
│  │   Not a crash — just a     │ │
│  │   dip. Go standard."       │ │
│  │                             │ │
│  └────────────────────────────┘ │  ← dark card with blue left border 3px
│           │                     │
│           ▼                     │
│  ┌─ YOUR BREW ────────────────┐ │
│  │                             │ │
│  │  FIRST LIGHT               │ │  ← CondensedBold, 26px, white
│  │  4–8°C · 12min · 60g/L    │ │  ← Mono, 14px, #888888
│  │  Medium roast · Medium grind│ │
│  │  1 serve · Best before 2pm │ │  ← Mono, 12px, #488BED
│  │                             │ │
│  └────────────────────────────┘ │  ← darkCard #0F0F0F, blue top border 3px
│                                 │
│  ┌─────────────────────────┐    │
│  │  I SEE IT  →            │    │  ← blue filled [CHANGED]
│  └─────────────────────────┘    │
│                                 │
└─────────────────────────────────┘
```

**Animation:** Cards appear sequentially top-to-bottom (0.3s stagger). Blue connecting lines draw downward between cards. Total sequence: ~1.5s.

**Content is example data** — not the user's real data yet.

**Why "I SEE IT" over "GOT IT":** "Got it" is a cookie banner dismissal. "I see it" mirrors the visual chain they just watched — it's a moment of comprehension, not dismissal. The user is acknowledging they understand how data becomes a brew.

---

### Onboarding Screen 5 — First Check-in (`FirstCheckInView`)

**[CHANGED] Subhead rewritten.**

```
┌─────────────────────────────────┐
│  ■■■■■  ■■■■■  ■■■■■  ■■■■■  ■■■│
│                                 │
│  How are you waking             │
│  up today?                      │  ← CondensedBold, 40px, white
│                                 │
│  Honest answer. Brezi           │
│  adjusts.                       │  ← Regular, 15px, #888888 [CHANGED]
│                                 │
│  ┌─────────────────────────────┐│
│  │  🟢  Peaked                 ││
│  │  HRV high, slept clean      ││  ← maps to score 9
│  └─────────────────────────────┘│
│  ┌─────────────────────────────┐│
│  │  🟡  Functional             ││
│  │  Decent night, a little     ││
│  │  behind                     ││  ← maps to score 6
│  └─────────────────────────────┘│
│  ┌─────────────────────────────┐│
│  │  🔴  Rebuilding             ││
│  │  Wrecked it yesterday       ││  ← maps to score 3
│  └─────────────────────────────┘│
│                                 │
│  ┌─────────────────────────┐    │
│  │  BUILD MY FIRST BREW  → │    │  ← blue, active after selection
│  └─────────────────────────┘    │
│                                 │
└─────────────────────────────────┘
```

**Selection card spec:**
- Default: border `#333`, background `#1A1A1A`, radius 16px, padding 20px
- Selected: border `#488BED` 2px, background `rgba(72,139,237,0.12)`, scale(1.01) spring animation
- Emoji: 28px, inline with label
- Label: CondensedBold, 20px, white
- Sublabel: Regular, 14px, `#888888`

**Transition:** After tap → loading state:
```
Black screen.
"Reading your signal..."     ← Mono, 16px, white, centered, pulse animation
```
Duration: 0.8s max. Then push to `DashboardView` with first brew recommendation pre-calculated. Mark `hasCompletedOnboarding = true`.

---

## 3. Dashboard — Home (`DashboardView`)

**[MAJOR CHANGE] The dashboard now has three temporal states. The v2.0 dashboard was the pre-brew state only. Now the dashboard lives through the entire day.**

The daily loop: **check in → brew → live → reflect → sleep → repeat.**

---

### Dashboard State 1: Pre-Brew (Morning)

**When:** User has checked in today but hasn't brewed yet. This is the default morning state.

```
┌─────────────────────────────────┐
│  ⟨devices⟩         bre:zi    ⚙  │  ← nav bar, black
│─────────────────────────────────│
│                                 │  ← offWhite background (#F5F5F5)
│  GOOD MORNING                   │  ← Mono, 11px, #888888, ALL CAPS
│  MARCH 19                       │  ← Mono, 11px, #888888
│                                 │
│  ┌─ 1  YOUR DATA ─────────────┐ │
│  │                             │ │
│  │  ┌──────┐ ┌──────┐ ┌─────┐ │ │
│  │  │  42  │ │  61% │ │ 6:48│ │ │  ← t_data (28px bold mono)
│  │  │  HRV │ │ RECV │ │ SLP │ │ │  ← Mono, 10px, #888888, ALL CAPS
│  │  │  ms  │ │      │ │ hrs │ │ │
│  │  └──────┘ └──────┘ └─────┘ │ │
│  │                             │ │
│  │  ↓ 18% vs your baseline    │ │  ← Mono, 12px, statusYellow
│  │  Source: WHOOP · 6:42am    │ │  ← Mono, 11px, #888888
│  │                             │ │
│  │  Feeling different?         │ │  [NEW] re-check-in action
│  │  Update your check-in ›    │ │  ← text link, Mono, 12px, #488BED
│  │                             │ │
│  └─────────────────────────────┘ │  ← white card, shadow, 16px radius
│                                 │
│  ┌─ 2  BREZI'S READ ──────────┐ │
│  │                             │ │
│  │  You're sharp today. Full   │ │
│  │  signal. Let's use it.      │ │  ← Regular, 15px, black [CHANGED — new voice]
│  │                             │ │
│  └─────────────────────────────┘ │  ← white card, blue left border 3px
│                                 │
│  ┌─ 3  YOUR BREW ─────────────┐ │
│  │                             │ │
│  │  FIRST LIGHT               │ │  ← CondensedBold, 32px, white
│  │  Your everyday standard.   │ │  ← Regular, 14px, #CCCCCC
│  │                             │ │
│  │  ┌────┐  ┌────┐  ┌────┐   │ │
│  │  │4–8°│  │ 12 │  │ 60 │   │ │  ← CondensedBold, 22px, white
│  │  │ C  │  │min │  │g/L │   │ │  ← Mono, 10px, #666, ALL CAPS
│  │  │TEMP│  │TIME│  │DOSE│   │ │
│  │  └────┘  └────┘  └────┘   │ │
│  │                             │ │
│  │  Medium roast · Medium grind│ │  ← Mono, 13px, #888888
│  │  1 serve · Best before 2pm │ │  ← Mono, 13px, #488BED
│  │                             │ │
│  │  ┌─────────────────────┐   │ │
│  │  │  START BREW  →      │   │ │  ← white filled, black text, 48px
│  │  └─────────────────────┘   │ │
│  │                             │ │
│  │  See full details ›        │ │  ← text link, #888888
│  │                             │ │
│  └─────────────────────────────┘ │  ← darkCard #0F0F0F, 24px radius
│                                 │
│  ┌─────────────────────────┐    │
│  │  VIEW BREW HISTORY      │    │  ← outlined button, black border
│  └─────────────────────────┘    │
│                                 │
└─────────────────────────────────┘
```

---

### Dashboard State 2: Brewing (Active)

**When:** User tapped "START BREW" and machine is running. This is a progress overlay.

```
┌─────────────────────────────────┐
│  ⟨devices⟩         bre:zi    ⚙  │
│─────────────────────────────────│
│                                 │
│                                 │
│                                 │
│         ┌───────────┐           │
│         │           │           │
│         │   ╔═══╗   │           │  ← Machine icon, subtle pulse animation
│         │   ║ b ║   │           │
│         │   ╚═══╝   │           │
│         │           │           │
│         └───────────┘           │
│                                 │
│  Brewing First Light...         │  ← Mono, 16px, white, centered
│                                 │
│  ████████████░░░░░░░░  67%      │  ← progress bar, #488BED fill
│                                 │
│  4–8°C · 8 of 12 minutes       │  ← Mono, 13px, #888888
│                                 │
│  [Cancel Brew]                  │  ← text link, #FF453A, centered
│                                 │
│                                 │
└─────────────────────────────────┘
```

Full-screen dark overlay on the dashboard. Progress bar maps to real brew duration from the Brezi machine via Bluetooth. On completion, auto-transitions to Post-Brew state.

---

### [NEW] Dashboard State 3: Post-Brew (Rest of Day)

**When:** User has brewed today. This state persists until the next morning check-in.

**The key insight Elon flagged: the day doesn't end at "Start Brew." It ends at the rating. The dashboard needs to breathe through the whole day.**

```
┌─────────────────────────────────┐
│  ⟨devices⟩         bre:zi    ⚙  │
│─────────────────────────────────│
│                                 │  ← offWhite background
│  GOOD AFTERNOON                 │  ← Mono, 11px, #888888, ALL CAPS
│  MARCH 19                       │  ← (time-of-day greeting)
│                                 │
│  ┌─ 1  YOUR DATA ─────────────┐ │
│  │                             │ │
│  │  ┌──────┐ ┌──────┐ ┌─────┐ │ │
│  │  │  42  │ │  61% │ │ 6:48│ │ │  ← same health data — your data
│  │  │  HRV │ │ RECV │ │ SLP │ │ │     is your data, it doesn't change
│  │  │  ms  │ │      │ │ hrs │ │ │
│  │  └──────┘ └──────┘ └─────┘ │ │
│  │                             │ │
│  │  ↓ 18% vs your baseline    │ │
│  │  Source: WHOOP · 6:42am    │ │
│  │                             │ │
│  └─────────────────────────────┘ │  ← white card, shadow
│                                 │
│  ┌─ 2  TODAY'S SIGNAL ────────┐ │  [CHANGED — Card 2 shifts to reflection]
│  │                             │ │
│  │  You went First Light on a  │ │
│  │  mid-recovery morning.      │ │  ← Regular, 15px, black
│  │  That tracks.               │ │
│  │                             │ │
│  └─────────────────────────────┘ │  ← white card, blue left border 3px
│                                 │
│  ┌─ 3  TODAY'S BREW ──────────┐ │  [CHANGED — Card 3 flips to brew record]
│  │                             │ │
│  │  FIRST LIGHT               │ │  ← CondensedBold, 32px, white
│  │  Brewed at 7:14am          │ │  ← Mono, 13px, #888888
│  │                             │ │
│  │  4–8°C · 12min · 60g/L    │ │  ← Mono, 13px, #666
│  │  Medium roast · Medium grind│ │
│  │                             │ │
│  │  ─────────────────────────  │ │  ← divider
│  │                             │ │
│  │  ┌─────────────────────┐   │ │  [IF NOT YET RATED]
│  │  │  RATE THIS BREW     │   │ │  ← white filled, black text, 48px
│  │  └─────────────────────┘   │ │
│  │                             │ │
│  │  ─── OR ───────────────     │ │  [IF ALREADY RATED]
│  │                             │ │
│  │  ★ ★ ★ ★ ☆                 │ │  ← RatingStarsView, 28px
│  │  Rated 4/5  ·  Edit ›      │ │  ← Mono, 13px, #888888
│  │                             │ │
│  │  See you tomorrow morning.  │ │  ← Mono, 13px, #888888, centered
│  │                             │ │
│  └─────────────────────────────┘ │  ← darkCard #0F0F0F, 24px radius
│                                 │
│  ┌─────────────────────────┐    │
│  │  VIEW BREW HISTORY      │    │  ← outlined button, black border
│  └─────────────────────────┘    │
│                                 │
└─────────────────────────────────┘
```

**Post-brew Card 2 reflection copy** — acknowledges the choice, contextualizes against the data. Examples:

| Recovery Tier | Brew Chosen | Reflection Copy |
|---|---|---|
| High (7–10) | Voltage | "Peak morning, peak brew. You matched the signal." |
| High (7–10) | First Light | "Strong signal, light brew. Playing it steady." |
| Mid (4–6) | First Light | "You went First Light on a mid-recovery morning. That tracks." |
| Mid (4–6) | Voltage | "You pushed past the signal. Let's see how it lands." |
| Low (1–3) | Tide Pool | "Low morning, gentle brew. You listened." |
| Low (1–3) | Deep Work | "Rough night but you went for it. Respect. Rate it honest." |

**Post-brew Card 3 states:**
- **Unrated:** "RATE THIS BREW" CTA button replaces "START BREW"
- **Rated:** Shows star rating with edit option + closing line "See you tomorrow morning."
- **Tapping "Edit"** opens inline star picker — saves immediately, no modal.

**Greeting logic:**
```swift
var greeting: String {
    let hour = Calendar.current.component(.hour, from: Date())
    switch hour {
    case 0..<12:  return "GOOD MORNING"
    case 12..<17: return "GOOD AFTERNOON"
    default:      return "GOOD EVENING"
    }
}
```

---

### [NEW] Dashboard State: Post-Cutoff (3pm+ Behavior)

**When:** Current time is past the user's caffeine cutoff setting. Applies to both pre-brew and post-brew states.

**Pre-brew + past cutoff:**
```
│  ┌─ 2  BREZI'S READ ──────────┐ │
│  │                             │ │
│  │  Past your caffeine window. │ │
│  │  If you're still brewing,   │ │  ← Regular, 15px, black
│  │  go Tide Pool or skip today.│ │
│  │                             │ │
│  └─────────────────────────────┘ │

│  ┌─ 3  YOUR BREW ─────────────┐ │
│  │                             │ │
│  │  TIDE POOL                 │ │  ← shifts to gentlest profile
│  │  Light caffeine. Easy exit. │ │
│  │                             │ │
│  │  ... (standard brew card)   │ │
│  │                             │ │
│  │  Choose a different brew ›  │ │  ← user can still override
│  │                             │ │
│  └─────────────────────────────┘ │
```

**Rest Day state** (alternative to Tide Pool when recovery is very low AND past cutoff):
```
│  ┌─ 3  REST DAY ──────────────┐ │
│  │                             │ │
│  │  No brew recommended.      │ │  ← CondensedBold, 24px, white
│  │                             │ │
│  │  Low recovery + late in    │ │
│  │  the day. Let your body    │ │  ← Regular, 14px, #CCCCCC
│  │  come back on its own.     │ │
│  │                             │ │
│  │  Choose a different brew › │ │  ← text link, #888888
│  │                             │ │
│  └─────────────────────────────┘ │  ← darkCard, 24px radius
```

The user can always override via "Choose a different brew" — we're not their parent.

---

### [NEW] Re-Check-In Action

Added to Card 1 in the pre-brew state. A text link below the source line:

```
│  Feeling different?             │
│  Update your check-in ›        │  ← text link, Mono, 12px, #488BED
```

**Behavior:** Tapping re-opens the check-in flow:
- If wearable connected: re-fetches latest wearable data, recalculates
- If manual: shows recovery slider overlay (half-sheet modal)
- After update: recalculates recommendation, updates Cards 2 and 3 with spring animation

**Use case:** User naps at 10am and recovery improves. Or user feels worse than their data suggests. The re-check-in lets them re-run without pull-to-refresh ambiguity.

---

### Card 1 — Health Data Card (`HealthDataCardView`)

*Layout unchanged from v2.0, with re-check-in link added.*

**Metric display rules:**
| Source | HRV | Recovery | Sleep |
|---|---|---|---|
| WHOOP | HRV (ms) | Recovery % | Sleep hours |
| Oura | HRV (ms) | Readiness score | Sleep hours |
| Apple Health | HRV (ms) | — (hide column) | Sleep hours |
| Manual | Recovery score /10 | — (hide column) | — (hide column) |

**Manual fallback:** If no wearable connected, Card 1 becomes the recovery slider:
```
│  ┌─ 1  YOUR DATA ─────────────┐ │
│  │                             │ │
│  │         [  5  ]             │ │  ← 72px number, CondensedBold
│  │    ○──────●──────○          │ │  ← recovery slider (1–10)
│  │    1               10      │ │
│  │                             │ │
│  │  Quick set:                 │ │
│  │  [1–3 Rest] [4–6 OK] [7–10]│ │  ← chip row
│  │                             │ │
│  │  Logging manually           │ │  ← Mono, 11px, #888888
│  │  Connect a wearable ›      │ │  ← text link, #488BED
│  │                             │ │
│  └─────────────────────────────┘ │
```

### Card 2 — Insight Strip (`InsightStripView`)

**[MAJOR CHANGE] Complete copy rewrite. The v2.0 copy read like a fitness dashboard. Brezi's voice is direct, a little warm, a little blunt. Like a coach who doesn't waste your time.**

**Layout:**
- White card, blue left border 3px, 16px radius, 16px padding
- 2–3 lines of natural language insight
- Font: Regular, 15px, black
- No CTA — read-only context

**[NEW] Insight Copy — 3 variants per tier, rotation system**

Each tier has 3 variants. The system tracks which variant was last shown (`lastInsightVariant` in UserDefaults per tier) and rotates so the user doesn't see the same line twice in a week.

**High recovery (7–10):**
```
Variant A: "You're sharp today. Full signal. Let's use it."
Variant B: "Clean night. Your body's ready for something real."
Variant C: "Everything's online. Don't waste this one."
```

**Mid recovery (4–6):**
```
Variant A: "Rough edges from last night. Functional, not sharp. Easy start."
Variant B: "Not your best, not your worst. Keep it clean today."
Variant C: "Your system's running but it's not asking for more. Listen to it."
```

**Low recovery (1–3):**
```
Variant A: "Yesterday hit you. Go light. Let your body come back on its own terms."
Variant B: "You're rebuilding. This isn't the morning to push. Gentle brew, gentle day."
Variant C: "Low signal. That's data, not a verdict. Rest and regroup."
```

**Rotation logic:**
```swift
func selectInsightVariant(tier: RecoveryTier) -> String {
    let variants = insightVariants[tier]!          // 3 per tier
    let lastUsedKey = "lastInsight_\(tier.rawValue)"
    let lastUsedIndex = UserDefaults.standard.integer(forKey: lastUsedKey)
    let nextIndex = (lastUsedIndex + 1) % variants.count
    UserDefaults.standard.set(nextIndex, forKey: lastUsedKey)
    return variants[nextIndex]
}
```

**Contextual suffix (appended when wearable data available):**
When HRV baseline data exists, append a data-aware line after the main insight:
```
"HRV is 18% below your 7-day average."    ← Mono, 13px, #888888
"Sleep was short at 5h 12m."               ← Mono, 13px, #888888
```
This keeps the voice human in the main line while providing the specific data below.

### Card 3 — Brew Card (`DashboardBrewCardView`)

*Layout unchanged from v2.0 for pre-brew state. Post-brew state specified above.*

**"START BREW" behavior:**
- If Brezi paired: sends brew parameters to machine via Bluetooth → transitions to Brewing state
- If not paired: behaves as "LOG THIS BREW" — records to BrewLog → transitions to Post-Brew state

### Dashboard — Pull to Refresh
- Standard iOS pull-to-refresh
- Re-fetches wearable data, recalculates recommendation
- Loading: "Pulling your signal..." — Mono, 13px, #888888

### Dashboard — Empty States

**[CHANGED] Cut "Log your first recovery score" — the UI is self-evident.**

| State | Copy |
|---|---|
| First use, no wearable | Card 1: recovery slider (manual mode). Card 2: "Your practice starts blank. Give it three mornings." Card 3: recommendation based on manual input. |
| Has history, not yet brewed today | "Nothing brewed yet today. Your data's ready. Your machine's ready. You're the last variable." |

---

## 4. Brew Recommendation — Full Detail (`BrewRecommendationView`)

**Entry:** Pushed from "See full details" on dashboard, or after selecting a different brew profile.

```
┌─────────────────────────────────┐
│  ‹ Back            bre:zi    ⚙  │
│─────────────────────────────────│
│                                 │
│  ┌─────────────────────────────┐│
│  │                             ││
│  │  TODAY'S BREW               ││  ← Mono, 11px, #488BED, ALL CAPS
│  │                             ││
│  │  FIRST LIGHT               ││  ← CondensedBold, 42px, white
│  │                             ││
│  │  Not a recovery day. Not   ││
│  │  a peak day. Just a morning││
│  │  that needs to begin       ││  ← Regular, 16px, #CCCCCC
│  │  correctly.                ││
│  │                             ││
│  │  ─────────────────────────  ││
│  │                             ││
│  │   TEMP      TIME     DOSE  ││  ← Mono, 10px, #666, ALL CAPS
│  │  4–8°C     12 min   60g/L ││  ← CondensedBold, 24px, white
│  │                             ││
│  │  ─────────────────────────  ││
│  │                             ││
│  │   ROAST     GRIND    SERVE ││  ← Mono, 10px, #666, ALL CAPS
│  │  Medium    Medium      1   ││  ← CondensedBold, 20px, white
│  │                             ││
│  │  ─────────────────────────  ││
│  │                             ││
│  │  ⏱ Best consumed before 2pm││  ← Mono, 13px, #488BED
│  │                             ││
│  │  ┌─────────────────────┐   ││
│  │  │  START BREW  →      │   ││  ← white filled, black text
│  │  └─────────────────────┘   ││
│  │                             ││
│  └─────────────────────────────┘│  ← darkCard, 24px radius, full-bleed
│                                 │
│  ─────────────────────────────  │
│                                 │
│  WHY THIS BREW                  │  ← Mono, 11px, #888888, ALL CAPS
│                                 │
│  ┌─ DATA ─────────────────────┐ │
│  │  HRV: 42ms (↓18% baseline) │ │
│  │  Recovery: 61%              │ │  ← Mono, 14px, black
│  │  Sleep: 6h 48m              │ │
│  └─────────────────────────────┘ │  ← white card, lightGrey border
│           │                     │
│           ▼                     │  ← blue connecting line
│  ┌─ INSIGHT ──────────────────┐ │
│  │  Mid recovery. HRV dipped  │ │
│  │  but sleep was adequate.   │ │  ← Regular, 14px, black
│  │  Moderate caffeine, clean  │ │
│  │  extraction, no overshoot. │ │
│  └─────────────────────────────┘ │  ← white card, blue left border
│           │                     │
│           ▼                     │
│  ┌─ BREW ─────────────────────┐ │
│  │  First Light selected.     │ │
│  │  Score range: 5–6.         │ │  ← Regular, 14px, black
│  │  Your score: 5.            │ │
│  │                             │ │
│  │  You went Voltage last time │ │  [NEW] past rating acknowledgment
│  │  at this recovery level and │ │  ← Regular, 13px, #888888
│  │  loved it.                  │ │  (only shows if prior rating data exists)
│  └─────────────────────────────┘ │  ← white card, blue top border
│                                 │
│  ─────────────────────────────  │
│                                 │
│  ┌─────────────────────────┐    │
│  │  ADJUST SETTINGS        │    │  ← outlined button, black border
│  └─────────────────────────┘    │
│                                 │
│  Choose a different brew ›      │  ← text link, #888888
│                                 │
└─────────────────────────────────┘
```

### "Why This Brew" Chain
The data→insight→brew chain from the tutorial, populated with real user data. Three small stacked cards with blue connecting lines.

**[NEW] Past Rating Acknowledgment:** If the user has previous brew logs for the same recovery tier, the BREW card can reference them: "You went Voltage last time at this recovery level and loved it." This is the lightest version of the rating feedback loop — it surfaces history without building a preference-weighted engine. The data is already in `BrewLog`. When the engine gets a preference layer in v2.x, this data is ready.

### "Adjust Settings" — Manual Override Sheet

**[CHANGED] Added bounded ranges per profile + amber warnings for The Blank.**

- Modal half-sheet (`.presentationDetents([.medium])`)
- Sliders: Temp (°C), Time (min), Dose (g/L)
- Pickers: Roast (Light / Medium / Dark), Grind (Fine / Medium / Coarse)
- Live preview updates
- CTA: "APPLY & START BREW"
- Saving an override stores to `BrewLog.brewSettingsJSON`

**[NEW] Bounded ranges per profile:**

Each profile defines min/max for temp, time, and dose. The sliders are constrained to the profile's range.

| Profile | Temp Range | Time Range | Dose Range |
|---|---|---|---|
| Tide Pool | 2–6°C | 8–12 min | 30–55 g/L |
| First Light | 3–10°C | 10–14 min | 50–70 g/L |
| Deep Work | 8–18°C | 12–16 min | 65–90 g/L |
| Voltage | 12–22°C | 13–18 min | 70–95 g/L |
| The Blank | 1–25°C | 5–20 min | 20–100 g/L |

**[NEW] Amber warning zone (The Blank only):**

When The Blank is selected and values go outside recommended ranges, show an amber visual signal:

```
│  TEMP                           │
│  ○────────────────●──○          │
│  1°C            22°C  25°C      │
│                   ⚠ Outside     │  ← statusAmber, Mono, 11px
│                     recommended │
│                     range       │
```

- Slider track turns from `#488BED` to `statusAmber` in the warning zone
- Text label appears below: "Outside recommended range" in `statusAmber`
- No hard block — The Blank is for people who know what they're doing
- Warning disappears when value returns to safe range

**[NEW] Roast/grind row — commerce-ready hook:**

The roast and grind recommendation row is built as a tappable component with an optional `onTap` closure that defaults to nil. When the commerce layer exists, this row becomes a link to "Order these beans." For now, it's display-only. Jensen should build:

```swift
struct BeanRecommendationRow: View {
    let roast: String
    let grind: String
    var onTap: (() -> Void)? = nil    // commerce hook, nil for v1
}
```

### "Choose a different brew" — Profile Picker
- Full-screen modal
- 5 profile cards in scrollable list
- Recommended profile: full color, blue "RECOMMENDED" badge
- Other profiles: slightly dimmed (opacity 0.7), no badge
- Each card: name, score range, one-line rationale, key settings
- Tap to select → replaces current recommendation, dismisses modal

### Updated Profile → Score Mapping
```swift
func profileKey(for score: Int) -> String {
    switch score {
    case 1...2:  return "tide_pool"
    case 3...4:  return "tide_pool"
    case 5...6:  return "first_light"
    case 7...8:  return "deep_work"
    case 9...10: return "voltage"
    default:     return "first_light"
    }
}
// "the_blank" is manual-only — accessed via "Choose a different brew"
```

### Updated Profile Specs
| Profile | Score | Temp | Time | Dose | Roast | Grind | Serve | Timing |
|---|---|---|---|---|---|---|---|---|
| Tide Pool | 1–4 | 4°C | 10 min | 40–50 g/L | Light–Med | Medium | 1 | Anytime |
| First Light | 5–6 | 4–8°C | 12 min | 60 g/L | Medium | Medium | 1 | Before 2pm |
| Deep Work | 7–8 | 10–15°C | 14 min | 80 g/L | Medium | Med-Coarse | 1 | Before 1pm |
| Voltage | 9–10 | 15–20°C | 15 min | 80 g/L | Med-Dark | Coarse | 1 | Before 12pm |
| The Blank | Manual | User | User | User | User | User | User | — |

---

## 5. History & Correlation (`HistoryView`)

**[CHANGED] Added 7-day recovery spark line at top. Correlation chart improved.**

**Entry:** "VIEW BREW HISTORY" from dashboard, or nav push.

```
┌─────────────────────────────────┐
│  ‹ Back            bre:zi    ⚙  │
│─────────────────────────────────│
│                                 │
│  YOUR PRACTICE                  │  ← Mono, 11px, #888888, ALL CAPS
│                                 │
│  ┌─ 7-DAY RECOVERY ──────────┐ │  [NEW]
│  │                             │ │
│  │  ╱╲    ╱╲                   │ │  ← spark line, no axes, no labels
│  │ ╱  ╲╱╱  ╲╱╲                │ │     just the shape of last 7 days
│  │                             │ │     color: #488BED, 2px stroke
│  │                             │ │
│  │  Avg recovery: 6.2          │ │  ← CondensedBold, 20px, black
│  │  ↑ from 5.4 last week      │ │  ← Mono, 13px, statusGreen
│  │                             │ │
│  └─────────────────────────────┘ │  ← white card
│                                 │
│  ┌─ STREAK ───────────────────┐ │
│  │                             │ │
│  │  14 days                    │ │  ← CondensedBold, 48px, black
│  │  Current brew streak        │ │  ← Mono, 12px, #888888
│  │                             │ │
│  │  M  T  W  T  F  S  S       │ │  ← week view, circles
│  │  ●  ●  ●  ●  ●  ○  ○       │ │  ← filled = brewed, empty = future
│  │                             │ │     blue = today
│  └─────────────────────────────┘ │  ← white card
│                                 │
│  ┌─ CORRELATION ──────────────┐ │
│  │                             │ │
│  │  RECOVERY vs BREW RATING   │ │  ← Mono, 11px, #888888, ALL CAPS
│  │                             │ │
│  │  5★ ·          ·    ●      │ │
│  │  4★ ·     ●  ●   ●        │ │  ← scatter plot
│  │  3★ ·  ●     ●            │ │     x: recovery score
│  │  2★ ·  ●                  │ │     y: brew rating (howFelt)
│  │  1★ ·                     │ │     dot color: profile color
│  │     └──┬──┬──┬──┬──┬──┬   │ │
│  │        2  3  4  5  6  7   │ │  ← recovery score axis
│  │                             │ │
│  │  Higher recovery = better   │ │
│  │  rated brews. Pattern holds.│ │  ← Regular, 13px, #888888
│  │                             │ │
│  └─────────────────────────────┘ │  ← white card
│                                 │
│  ┌─ PROFILE MIX ─────────────┐ │
│  │                             │ │
│  │  ████████████░░░░░░░░       │ │  ← horizontal stacked bar
│  │  First Light 42%            │ │     each segment = profile color
│  │  Deep Work 28%              │ │  ← Mono, 12px, #888888
│  │  Voltage 18%                │ │
│  │  Tide Pool 12%              │ │
│  │                             │ │
│  └─────────────────────────────┘ │  ← white card
│                                 │
│  ─ March 2026 ─────────────     │  ← Mono, 12px, section header
│                                 │
│  ┌─────────────────────────────┐│
│  │  FIRST LIGHT       Mar 19  ││
│  │  4–8°C · 12min · 60g/L    ││  ← Mono, 13px, #666
│  │  Recovery: 5  · Med roast  ││  ← Mono, 12px, #888
│  │  ★ ★ ★ ☆ ☆                 ││
│  └─────────────────────────────┘│
│  ┌─────────────────────────────┐│
│  │  VOLTAGE           Mar 18  ││
│  │  15–20°C · 15min · 80g/L  ││
│  │  Recovery: 9  · Dark roast ││
│  │  ★ ★ ★ ★ ★                 ││
│  └─────────────────────────────┘│
│                                 │
└─────────────────────────────────┘
```

### [NEW] 7-Day Recovery Spark Line (`RecoverySparkView`)

**Purpose:** One glance tells you trajectory — trending up, trending down, or flat. No axes, no labels, just the shape.

**Spec:**
- Simple line chart, 7 data points (one per day, last 7 days)
- Stroke: `#488BED`, 2px, rounded line cap
- Fill: gradient from `#488BED` at 0.15 opacity to transparent, below the line
- Height: 48px
- Width: full card width minus padding
- No x-axis labels, no y-axis labels, no grid lines — just the line
- Missing days: interpolated (dotted segment between known points)
- Below the spark line: "Avg recovery: X.X" in CondensedBold 20px + trend comparison "↑/↓ from X.X last week" in Mono 13px, colored green (up) / red (down) / grey (flat, <5% change)

**Why not a full 30-day chart:** A 30-day HRV trend is a full component with date range selection, axis scaling, and edge cases. The spark line gives the trajectory feeling without building a charting library. Deeper analytics is a v2.x feature.

**Appears:** After 3+ days of check-ins. Before that, hide the card entirely (don't show empty state for this).

### Streak Card (`StreakCardView`)
*Unchanged from v2.0.*

### Correlation Card (`CorrelationCardView`)

**Dot color mapping:**
| Profile | Dot Color |
|---|---|
| Tide Pool | `#34C759` (green) |
| First Light | `#488BED` (blue) |
| Deep Work | `#FFD60A` (yellow) |
| Voltage | `#FF453A` (red) |
| The Blank | `#888888` (grey) |

**Insight line below chart** — generated from correlation direction:
- Positive correlation: "Higher recovery = better rated brews. Pattern holds."
- Weak/no correlation: "No clear pattern yet. More data will sharpen this."
- Negative correlation (rare): "Interesting — you're rating lower-recovery brews higher. You might prefer gentler profiles."

**Appears:** After 5+ logged brews with ratings. Before that:
```
"Log 5 rated brews to unlock your correlation chart."
← Mono, 13px, #888888
```

### Profile Mix Card (`ProfileMixView`)
*Unchanged from v2.0. Appears after 3+ brews.*

### Brew Log Cells
*Unchanged from v2.0.*

### History — Empty State
```
Nothing brewed yet.
Your data's ready.               ← Regular, 16px, #333, centered
Your machine's ready.
You're the last variable.

[See today's recommendation ›]   ← blue text link
```

---

## 6. Device Hub (`DeviceHubView`)

*Unchanged from v2.0.*

```
┌─────────────────────────────────┐
│  ‹ Back            bre:zi    ⚙  │
│─────────────────────────────────│
│                                 │
│  YOUR GEAR                      │  ← Mono, 11px, #888888, ALL CAPS [CHANGED]
│                                 │
│  ┌─ BREZI MACHINE ────────────┐ │
│  │                             │ │
│  │  ┌──┐  BREZI-A4F2          │ │
│  │  │☕│  Connected            │ │  ← Mono, 12px, #34C759
│  │  └──┘  Firmware 1.2.0      │ │  ← Mono, 11px, #888888
│  │                             │ │
│  │  Last brew: Today, 7:14am  │ │
│  │  Brews logged: 47          │ │
│  │                             │ │
│  │  [Disconnect]              │ │  ← text button, #FF453A
│  │                             │ │
│  └─────────────────────────────┘ │
│                                 │
│  ┌─ WEARABLE ─────────────────┐ │
│  │                             │ │
│  │  ┌──┐  WHOOP 4.0           │ │
│  │  │⌚│  Connected            │ │  ← Mono, 12px, #34C759
│  │  └──┘  Last sync: 6:42am   │ │
│  │                             │ │
│  │  Pulling: HRV, Recovery,   │ │
│  │  Sleep duration             │ │
│  │                             │ │
│  │  [Disconnect]              │ │
│  │                             │ │
│  └─────────────────────────────┘ │
│                                 │
│  ─ ADD DEVICE ──────────────    │
│                                 │
│  ┌─────────────────────────────┐│
│  │  + Pair another Brezi       ││
│  └─────────────────────────────┘│
│  ┌─────────────────────────────┐│
│  │  + Connect wearable         ││
│  └─────────────────────────────┘│
│                                 │
│  ─ COMING SOON ─────────────    │  ← Mono, 11px, #488BED
│                                 │
│  ┌─────────────────────────────┐│
│  │  ┌──┐  Brezi Blender       ││
│  │  │🫙│  Health-responsive    ││
│  │  └──┘  smoothies.          ││
│  │        Notify me ›         ││  ← text link, #488BED
│  └─────────────────────────────┘│  ← white card, dashed border #E0E0E0
│                                 │
└─────────────────────────────────┘
```

### Device Card Spec (`DeviceCardView`)
- White card, 16px radius, shadow, 20px padding
- Icon: 40px, left-aligned (SF Symbols for prototype)
- Device name: CondensedBold, 18px, black
- Status: Mono, 12px — connected (#34C759), disconnected (#FF453A), syncing (#488BED)
- Metadata: Mono, 11px, #888888

### Extensibility note for Jensen
`RecommendationType` enum supports future Blender product:
```swift
enum RecommendationType {
    case brew(BrewProfile)
    case blend(BlendProfile)    // future
}
```

---

## 7. Settings (`SettingsView`)

**[CHANGED] Section headers updated per Elon's suggestion — "Your Gear," "How You Brew," "Numbers."**

```
┌─────────────────────────────────┐
│  ‹ Back            bre:zi    ⚙  │
│─────────────────────────────────│
│                                 │
│  SETTINGS                       │  ← Mono, 11px, #888888
│                                 │
│  ─ Your Gear ────────────────   │  [CHANGED]
│  Brezi Machine    Connected  ›  │
│  Wearable         WHOOP     ›  │
│                                 │
│  ─ How You Brew ─────────────   │  [CHANGED]
│  Default roast     Medium   ›  │
│  Default grind     Medium   ›  │
│  Caffeine cutoff   2:00 PM  ›  │
│  Serving size      1        ›  │
│                                 │
│  ─ Numbers ──────────────────   │  [CHANGED]
│  Temperature       Celsius  ›  │
│  Weight            Grams    ›  │
│                                 │
│  ─ Notifications ────────────   │
│  Morning check-in  [ON]        │
│  Check-in time     7:00 AM ›  │
│  Brew reminders    [ON]        │
│                                 │
│  ─ Data ─────────────────────   │
│  Export brew history        ›  │
│  Reset all data             ›  │
│                                 │
│  ─ About ────────────────────   │
│  Version            0.2.0      │
│  Built by Brezi                │
│                                 │
└─────────────────────────────────┘
```

### Settings details:
- **Caffeine cutoff:** Profiles recommended after this time shift one tier gentler (Deep Work → First Light, Voltage → Deep Work). Default: 2:00 PM.
- **Export:** Generates CSV of all BrewLog entries — date, profile, settings, recovery score, rating.
- **Reset all data:** Destructive action. Confirmation alert: "This removes all your brew history, recovery data, and device connections. This can't be undone." Two buttons: "Cancel" (default) and "Reset Everything" (#FF453A).

---

## 8. Loading / Transition States

### "Reading your signal..."
```
Black background, full screen.
"Reading your signal..."     ← Mono, 16px, white, centered
Subtle pulse (opacity 1.0 ↔ 0.5, 1.2s loop)
Duration: 0.8s max → push to DashboardView
```

### Brew Completion
After brew finishes (from Dashboard State 2):
```
Checkmark (SF: checkmark.circle.fill) #488BED, 48pt
"Brewed."                    ← Mono, 14px, #888888 [KEEPING — specificity wins]
"Rate this brew?"            ← Regular, 15px, white
[★ ★ ★ ★ ★]                 ← RatingStarsView, large (32px)
[Skip rating]                ← text link, #888888
```

On rating or skip → dismiss overlay → Dashboard transitions to Post-Brew state (State 3).

### Brew Logged Confirmation (no device)
```
Checkmark #488BED, 48pt
"Logged."                    ← Mono, 14px, #888888
Auto-dismiss 1.2s → Dashboard transitions to Post-Brew state
```

---

## 9. [NEW] Notification Tiers

**Three tiers based on user tenure. Covers the first month. We'll iterate on notification strategy based on retention data from TestFlight — not speccing Day 90 without Day 30 data.**

### Tier 1: Day 1–7 (Everything is new)
**Trigger:** Morning, at user's configured check-in time.
**Copy:** Static, single message.
```
"Your signal's ready. Time to check in."
```
**Why static:** The user is still learning the product. Consistency builds the habit. Novelty in notifications this early adds noise, not value.

### Tier 2: Day 8–14 (Data starts to matter)
**Trigger:** Morning, at check-in time.
**Copy:** Adds recovery trend data.
```
"Your recovery is up 12% this week. See what Brezi recommends."
"Recovery dipped overnight. Brezi's recalibrated — check in."
"Steady recovery all week. Your signal's consistent."
```
**Logic:** Pull the 7-day recovery trend from CheckIn history. Select copy variant based on trend direction (up/down/flat).

### Tier 3: Day 15+ (Patterns emerge)
**Trigger:** Morning, at check-in time.
**Copy:** Rotates between three notification types:

**Streak:**
```
"Day 14. Keep it going."
"Day 21. Three weeks of practice."
```

**Pattern:**
```
"You brew best on high-recovery mornings. Today's looking good."
"Voltage got 5 stars last time you were at this recovery level."
```

**Nudge** (only if no check-in by 2 hours past configured time):
```
"Haven't checked in today. Your data's waiting."
```

**Rotation logic:** Alternate between streak and pattern messages. Nudge only fires as a fallback, never as the primary morning push.

### Notification Permissions
- Request `UNUserNotificationCenter` authorization after first BrewLog (not during onboarding — earn it first).
- If denied, no persistent nag. Settings shows "Notifications: Off" with an "Enable" link.

---

## 10. [NEW] Offline Behavior Rules

**Not wireframes — just rules. Implementation is Jensen's domain.**

### No Internet
- App works normally. All data is local (Core Data).
- iCloud sync via `NSPersistentCloudKitContainer` happens when connectivity returns.
- User never knows. No banner, no warning, no degraded state.

### No Bluetooth (after pairing)
- Dashboard shows recommendation normally — all data is already synced locally.
- "START BREW" button changes to: "LOG THIS BREW"
- Below button: "Can't reach your Brezi. Brew manually and log it here." — Mono, 12px, #888888
- When Bluetooth reconnects, button reverts to "START BREW" automatically.

### No Bluetooth (during pairing)
- Covered in Onboarding Screen 2 failure states above.

### Wearable Sync Cadence
- **Design recommendation (Jensen's call on implementation):**
  - Sync on app open (foreground fetch)
  - Background fetch every 4 hours
  - Pull-to-refresh triggers manual sync
- Card 1 timestamp ("Source: WHOOP · 6:42am") tells the user when data was last pulled.
- If data is stale (>8 hours), timestamp turns `statusYellow` as a subtle visual cue.

---

## 11. Empty States (Complete Table)

| Screen | State | Copy |
|---|---|---|
| Dashboard — first use | No wearable, no history | Card 2: "Your practice starts blank. Give it three mornings." |
| Dashboard — no brew today | Has history, not yet brewed | "Nothing brewed yet today. Your data's ready. Your machine's ready. You're the last variable." |
| Dashboard — post-brew, unrated | Brewed but no rating | Card 3 shows "RATE THIS BREW" button |
| Dashboard — post-cutoff | Past caffeine window | Card 2: "Past your caffeine window." Card 3: Tide Pool or Rest Day |
| History — no brews | First visit | "Nothing brewed yet. Your data's ready. Your machine's ready. You're the last variable." |
| History — no correlation | < 5 rated brews | "Log 5 rated brews to unlock your correlation chart." |
| History — no spark line | < 3 days of check-ins | Hide spark line card entirely (no placeholder) |
| Device Hub — no Brezi | Skipped pairing | "No Brezi paired. Your recommendations still work — you'll just log manually. [Pair now ›]" |
| Device Hub — no wearable | Skipped wearable | "Flying manual right now. That's fine. Connect Oura or WHOOP when you're ready." |
| Settings — wearable | Not connected | "Connect a wearable and Brezi does the logging for you. [Connect ›]" |

---

## 12. Component Library

| Component | File | Used in |
|---|---|---|
| `HealthDataCardView` | HealthDataCardView.swift | Dashboard (all states) |
| `InsightStripView` | InsightStripView.swift | Dashboard, Brew Recommendation |
| `DashboardBrewCardView` | DashboardBrewCardView.swift | Dashboard (pre-brew) |
| `PostBrewCardView` | PostBrewCardView.swift | Dashboard (post-brew) **[NEW]** |
| `DataChainView` | DataChainView.swift | Onboarding 4, Brew Recommendation |
| `RecoverySliderView` | RecoverySliderView.swift | Dashboard (manual mode), Re-check-in |
| `QuickSetChipRow` | QuickSetChipRow.swift | Dashboard (manual mode) |
| `ProfileCardView` | ProfileCardView.swift | Brew Recommendation, Profile Picker |
| `BrewSettingsRow` | BrewSettingsRow.swift | Brew Recommendation |
| `BeanRecommendationRow` | BeanRecommendationRow.swift | Brew Recommendation **[NEW]** |
| `ManualOverrideSheet` | ManualOverrideSheet.swift | Brew Recommendation **[NEW — bounded sliders]** |
| `BrewLogCell` | BrewLogCell.swift | History |
| `RatingStarsView` | RatingStarsView.swift | History, Brew Confirmation, Post-brew card |
| `StreakCardView` | StreakCardView.swift | History |
| `RecoverySparkView` | RecoverySparkView.swift | History **[NEW]** |
| `CorrelationCardView` | CorrelationCardView.swift | History |
| `ProfileMixView` | ProfileMixView.swift | History |
| `DeviceCardView` | DeviceCardView.swift | Device Hub |
| `WearableRowView` | WearableRowView.swift | Onboarding 3, Device Hub |
| `OnboardingSelectionCard` | OnboardingSelectionCard.swift | Onboarding 5 |
| `DevicePairFailureCard` | DevicePairFailureCard.swift | Onboarding 2 **[NEW]** |
| `BrewProgressView` | BrewProgressView.swift | Dashboard (brewing state) **[NEW]** |
| `SaveButton` | SaveButton.swift | All screens |
| `EmptyStateView` | EmptyStateView.swift | All screens |
| `SegmentedProgressBar` | SegmentedProgressBar.swift | Onboarding shell |

---

## 13. Data Model Updates (for Jensen)

### New/Modified Core Data Entities

**Device** (new)
```swift
- id: UUID
- type: String           // "brezi", "brezi_blender" (future)
- name: String           // "BREZI-A4F2"
- firmwareVersion: String?
- isPaired: Bool
- lastConnectedAt: Date?
- createdAt: Date
```

**WearableConnection** (new)
```swift
- id: UUID
- source: String         // "whoop", "oura", "apple_health", "manual"
- isConnected: Bool
- lastSyncAt: Date?
- accessToken: String?   // encrypted, for OAuth sources
- createdAt: Date
```

**CheckIn** (updated)
```swift
- id: UUID
- recoveryScore: Int16   // 1–10 (manual) or mapped from wearable
- hrvValue: Double?       // raw HRV in ms (from wearable)
- recoveryPercent: Double? // recovery % (from wearable)
- sleepHours: Double?     // total sleep (from wearable)
- baselineHrv: Double?    // 7-day rolling average
- source: String          // "manual", "whoop", "oura", "apple_health"
- isRecheck: Bool         // [NEW] true if this replaced an earlier check-in today
- createdAt: Date
```

**BrewProfile** (updated)
```swift
// Existing fields unchanged, add:
- recommendedRoast: String
- recommendedGrind: String
- defaultServings: Int16
- caffeineCutoffHour: Int16
- brewTimeMinutes: Int16
// [NEW] Manual override bounds:
- minTemp: Double
- maxTemp: Double
- minTime: Int16
- maxTime: Int16
- minDose: Double
- maxDose: Double
```

**BrewLog** (updated)
```swift
// Existing fields unchanged, add:
- roastUsed: String?
- grindUsed: String?
- servings: Int16
- deviceId: UUID?
- brewedAt: Date?        // [NEW] exact brew timestamp (for post-brew card)
```

### Insight Engine (`InsightEngine.swift`)
```swift
struct HealthSignal {
    let recoveryScore: Int
    let hrv: Double?
    let baselineHrv: Double?
    let recoveryPercent: Double?
    let sleepHours: Double?
    let source: String
}

struct BrewInsight {
    let profileKey: String
    let insightText: String
    let dataPoints: [String]
    let confidence: Double       // 0–1, higher with more data
    let variant: Int             // [NEW] which copy variant was selected (0–2)
}

func generateInsight(from signal: HealthSignal) -> BrewInsight
```

### [NEW] Insight Rotation Tracking
```swift
// UserDefaults keys for insight variant rotation
// "lastInsight_high" -> Int (0, 1, or 2)
// "lastInsight_mid"  -> Int
// "lastInsight_low"  -> Int
```

---

## 14. Build Order (recommended for Jensen)

1. **Day 1–2:** Core Data updates — Device, WearableConnection, updated CheckIn/BrewProfile/BrewLog with new fields (including override bounds, `isRecheck`, `brewedAt`). Seed profile data. Build `InsightEngine` with rule-based logic + copy variants + rotation.

2. **Day 3–4:** Dashboard — all three states. `HealthDataCardView` + `InsightStripView` (with new copy) + `DashboardBrewCardView` (pre-brew) + `PostBrewCardView` (post-brew) + `BrewProgressView` (brewing). Wire manual recovery input. Wire re-check-in action. Wire post-cutoff behavior.

3. **Day 5–6:** `BrewRecommendationView` — brew card, "Why This Brew" chain with past rating acknowledgment, `ManualOverrideSheet` with bounded sliders + amber warnings, profile picker modal, `BeanRecommendationRow` (commerce-ready hook).

4. **Day 7–8:** `HistoryView` — `RecoverySparkView` (7-day spark line), `StreakCardView`, `BrewLogCell` list, `CorrelationCardView` (colored dots + insight line), `ProfileMixView`.

5. **Day 9–10:** Onboarding flow — 5 screens. Updated copy throughout. `DevicePairView` with all 4 failure states (`DevicePairFailureCard`). `WearableConnectView`. `DataChainTutorialView` ("I SEE IT" CTA). `FirstCheckInView` (updated subhead).

6. **Day 11:** `DeviceHubView` — device cards, add/disconnect flows. Blender placeholder.

7. **Day 12–13:** `SettingsView` with updated headers. Notification scheduling (3 tiers). Data export. Polish transitions and loading states. Offline behavior rules.

8. **Day 14:** End-to-end testing, TestFlight prep, bug sweep.

**TestFlight target: 14 working days from start.**

---

## 15. Decisions — Locked by Jackson (March 19)

*Original 10 decisions from v2.0 remain locked. Adding:*

11. **"Brewed." over "Done."** — Specificity wins. "Done" could be anything. "Brewed" could only be this.
12. **No community signal on dashboard.** The personal chain is the product. Community signal saved for History screen at scale. (Jackson's call to override — Elon argues for dashboard.)
13. **Post-brew dashboard state is a first-class design.** The daily loop doesn't end at "Start Brew."
14. **Three notification tiers.** Day 1–7 static, Day 8–14 data-aware, Day 15+ pattern-based. Day 90 strategy comes from TestFlight retention data.
15. **7-day spark line, not 30-day trend chart.** Trajectory without building a charting library.
16. **Manual override bounded per profile.** The Blank gets full range with amber warnings, not hard blocks.

---

## 16. v2.0 → v2.1 Changelog

### Added
| # | Change | Section |
|---|---|---|
| 1 | Post-brew dashboard state (three-card stack flips to reflection + rating) | §3, Dashboard State 3 |
| 2 | Insight strip copy rewrite — 3 variants per tier, rotation logic, Brezi voice | §3, Card 2 |
| 3 | Device pairing failure states — Bluetooth off, not found, already paired, firmware update | §2, Screen 2 |
| 4 | Manual override guardrails — bounded ranges per profile, amber warning on The Blank | §4, Adjust Settings |
| 5 | Notification tiers — Day 1–7, Day 8–14, Day 15+ | §9 |
| 6 | 7-day recovery spark line in History | §5 |
| 7 | Re-check-in action on dashboard | §3, Card 1 |
| 8 | Post-cutoff dashboard behavior (3pm+ state) | §3 |
| 9 | Offline behavior rules | §10 |
| 10 | Commerce-ready hook on roast/grind row | §4 |
| 11 | Past rating acknowledgment in "Why This Brew" | §4 |
| 12 | Brewing progress state (Dashboard State 2) | §3 |

### Changed
| # | Change | Reason |
|---|---|---|
| 1 | Onboarding 1 body copy → "Brezi reads your body..." | Cut the defensive pitch. User already bought it. |
| 2 | Onboarding 2 subhead → "Power on your Brezi. We'll find each other." | Warmer. |
| 3 | Onboarding 4 headline → "Your data becomes your brew." | Better centers the user. |
| 4 | Onboarding 4 CTA → "I SEE IT" (was "GOT IT") | Comprehension, not dismissal. |
| 5 | Onboarding 5 subhead → "Honest answer. Brezi adjusts." | Tighter. |
| 6 | Dashboard empty → cut "Log your first recovery score" | UI is self-evident. |
| 7 | Settings headers → "Your Gear," "How You Brew," "Numbers" | Low-effort, high-personality. |
| 8 | Correlation dots colored by profile | Better visual encoding. |
| 9 | Added `statusAmber` to design tokens | Needed for override warnings. |

### Kept As-Is (Pushed Back)
| # | Item | Reason |
|---|---|---|
| 1 | "Brewed." over "Done." | Specificity wins over brevity. |
| 2 | No community signal on dashboard | Personal chain is the product. Don't dilute it. History at scale. |
| 3 | No Day 90 notification strategy | Need real TestFlight data first. |
| 4 | No 30-day trend charts | Spark line gives trajectory. Full charts = v2.x. |
| 5 | No bean commerce flow | Architecture-ready, not designed. |

### For Jackson's Final Call
- **Community signal:** Dashboard vs. History vs. not at all. Picasso argues History. Elon wants dashboard.
- **"Brewed" vs. "Done":** Picasso argues "Brewed." Elon argues "Done."

---

*The chain is the product. Now it breathes through the whole day — morning check-in, active brew, afternoon reflection, evening close. The dashboard doesn't die after "Start Brew" anymore. The voice sounds like a person, not a fitness tracker. The first 5 minutes of ownership have real failure states instead of optimism. Everything else is tighter, bounded, and ready for Jensen to build.*

*v2.1 spec locked. 14 working days to TestFlight.*

— Picasso
