# Brezi iOS App — Final Design Spec v3.0
**Author:** Picasso (Product Design) | March 19, 2026 | Round 3 — Final
**For:** Jensen (Engineering) · Jackson (Product) · Elon (Growth/Copy)
**Status:** LOCKED. This is the build spec. No more design rounds.

---

## 0. Design Tokens

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

let statusGreen  = Color(hex: "#34C759")
let statusYellow = Color(hex: "#FFD60A")
let statusRed    = Color(hex: "#FF453A")
let statusBlue   = Color(hex: "#488BED")
let statusAmber  = Color(hex: "#FF9F0A")
```

### Typography
```swift
let t_display       = Font.system(size: 48, weight: .heavy)                          // "Brewed." overlay
let t_h1            = Font.system(size: 34, weight: .bold)                           // unused in current spec
let t_h2            = Font.system(size: 26, weight: .bold)                           // brew profile names (detail)
let t_h3            = Font.system(size: 20, weight: .semibold)                       // selection card labels
let t_body          = Font.system(size: 16, weight: .regular)                        // primary body text
let t_small         = Font.system(size: 14, weight: .regular)                        // secondary body text
let t_mono          = Font.system(size: 13, weight: .regular, design: .monospaced)   // metadata lines
let t_eyebrow       = Font.system(size: 11, weight: .medium, design: .monospaced)    // section headers, ALL CAPS
let t_data          = Font.system(size: 28, weight: .bold, design: .monospaced)      // health metric numbers
let t_condensedBold = Font.custom("ABCSocial-CondensedBold", size: 32)               // brew names, headlines
// Fallback: Font.system(size: N, weight: .heavy, design: .default) if custom font unavailable
```

### Spacing (8pt grid)
```
xs: 4    sm: 8    md: 16    lg: 24    xl: 32    xxl: 48    xxxl: 64
```

### Corner Radii
```
card: 16    button: 12    chip: 8    input: 10    brewCard: 24
```

### Shadows
```swift
let cardShadow = Shadow(color: .black.opacity(0.08), radius: 16, x: 0, y: 4)  // white cards on offWhite
// No shadow on dark cards or dark backgrounds
```

### Tap Targets
```
Minimum: 44 × 44 pt for all interactive elements.
Buttons: minimum height 48pt, full-width where specified.
Text links: pad to 44pt hit area even if text is smaller.
```

---

## 1. Information Architecture

### Navigation Pattern
Single push-stack with modal sheets. No tab bar.

### Full Nav Tree
```
AppRoot
 └─ OnboardingFlow (first launch only, gated by hasCompletedOnboarding: Bool)
     ├─ Screen 1: BrandIntroView.swift
     ├─ Screen 2: DevicePairView.swift
     │   ├─ State: idle
     │   ├─ State: searching
     │   ├─ State: found (happy path)
     │   ├─ State: bluetoothOff
     │   ├─ State: deviceNotFound (30s timeout)
     │   ├─ State: alreadyPaired (another phone)
     │   └─ State: firmwareUpdateRequired
     │       └─ State: firmwareUpdateFailed
     ├─ Screen 3: WearableConnectView.swift
     │   ├─ State: idle (selection)
     │   ├─ State: oauthCancelled (silent return)
     │   ├─ State: oauthFailed (network/server)
     │   └─ State: oauthSucceededDataPending
     ├─ Screen 4: DataChainTutorialView.swift
     └─ Screen 5: FirstCheckInView.swift
          └─ Transition: "Reading your signal..." → DashboardView

 └─ MainFlow (post-onboarding)
     ├─ DashboardView.swift
     │   ├─ State: preBrew (morning, not yet brewed)
     │   │   ├─ Variant: wearable user
     │   │   └─ Variant: manual user
     │   ├─ State: brewing (active brew in progress)
     │   │   ├─ State: brewCancelConfirm
     │   │   └─ State: bleDisconnected
     │   ├─ State: postBrew (brewed today)
     │   │   ├─ Variant: unrated
     │   │   └─ Variant: rated
     │   ├─ State: postCutoff + preBrew
     │   │   ├─ Variant: Tide Pool recommendation
     │   │   └─ Variant: Rest Day (low recovery + post-cutoff)
     │   └─ State: postCutoff + postBrew
     │
     ├─ BrewCompletionOverlay.swift (full-screen overlay on brew finish)
     │
     ├─ BrewRecommendationView.swift (pushed from "See full details")
     │   ├─ ManualOverrideSheet.swift (half-sheet modal)
     │   └─ ProfilePickerView.swift (full-screen modal)
     │
     ├─ HistoryView.swift (pushed from "VIEW BREW HISTORY")
     │
     ├─ DeviceHubView.swift (pushed from nav bar device icon)
     │
     └─ SettingsView.swift (pushed from nav bar gear icon)
```

### Navigation Bar — `NavBarView.swift` (persistent, post-onboarding)
```
┌─────────────────────────────────────────┐
│  [←]              bre:zi           📡  ⚙ │
│  44×44            18px            20pt   │
└─────────────────────────────────────────┘
```
- Background: `#000000`
- Height: 56pt
- Center: "bre:zi" — CondensedBold 18px white. Fallback: "BREZI" system heavy 18px white.
- Left: back arrow (SF Symbol: `chevron.left`) when pushed. Hidden at root. 44×44pt tap area.
- Right slot 1: SF Symbol `antenna.radiowaves.left.and.right` — white, 20pt. Tap → push `DeviceHubView`. 44×44pt tap area.
- Right slot 2: SF Symbol `gearshape` — white, 20pt. Tap → push `SettingsView`. 44×44pt tap area.
- Horizontal padding: 16pt (md).

---

## 2. Onboarding Flow (5 Screens)

**Trigger:** First launch only. Store `hasCompletedOnboarding: Bool` in UserDefaults.

### Shared Onboarding Shell — `OnboardingShellView.swift`
- Background: `#000000` full bleed, no safe-area insets on color
- Progress bar: `SegmentedProgressBar.swift`
  - 5 horizontal segments at top
  - Segment height: 2pt
  - Gap between segments: 4pt (xs)
  - Horizontal padding: 16pt (md)
  - Top padding: 8pt (sm) below safe area
  - Completed segment: `#488BED`
  - Current segment: `#488BED` with opacity pulse (1.0 ↔ 0.6, duration 1.5s, easeInOut, repeating)
  - Upcoming segment: `#333333`
- Back swipe: enabled (system interactivePopGestureRecognizer)
- Skip button: top-right, 44×44pt tap area, `#888888`, Regular 14px — **only on Screens 2 and 3**
- Content area padding: 24pt (lg) horizontal, 48pt (xxl) top below progress bar
- CTA button zone: pinned 32pt (xl) from bottom safe area

---

### Screen 1 — Brand Moment — `BrandIntroView.swift`

**Purpose:** Establish identity. No skip button. 8 seconds max.

```
┌─────────────────────────────────┐
│  ■■■■■  ─────  ─────  ─────  ───│  progress: segment 1 active
│                                 │
│                                 │
│                                 │
│                                 │
│                                 │
│                                 │
│  Cold is                        │  CondensedBold, 52px, #FFFFFF
│  a practice.                    │  line-height: 1.1
│                                 │  bottom padding: 16pt (md)
│  Brezi reads your body and      │  Regular, 17px, #CCCCCC
│  builds your brew. Cold.        │  line-height: 1.5
│  Precise. Personal.             │
│  Every morning.                 │
│                                 │
│                                 │
│                                 │
│  ┌─────────────────────────┐    │  blue filled button (#488BED)
│  │  START THE PRACTICE  →  │    │  CondensedBold, 16px, #FFFFFF, ALL CAPS
│  └─────────────────────────┘    │  height: 54pt, radius: 12pt
│                                 │  horizontal margin: 24pt (lg)
└─────────────────────────────────┘
```

**Animations:**
| Element | Type | Duration | Easing | Delay |
|---|---|---|---|---|
| Headline ("Cold is / a practice.") | fadeIn + translateY(12→0) | 0.4s | easeOut | 0.1s |
| Body ("Brezi reads your body...") | fadeIn (opacity 0→1) | 0.3s | easeOut | 0.5s |
| CTA button | fadeIn (opacity 0→1) | 0.2s | easeOut | 0.8s |

**Copy (exact strings):**
```swift
let screen1_headline = "Cold is\na practice."
let screen1_body = "Brezi reads your body and builds your brew. Cold. Precise. Personal. Every morning."
let screen1_cta = "START THE PRACTICE"
```

**Behavior:** Tap CTA → push Screen 2. No other actions.

---

### Screen 2 — Device Pairing — `DevicePairView.swift`

**Purpose:** Pair Brezi machine via Bluetooth. Skip button visible.

#### State: Idle (default)

```
┌─────────────────────────────────┐
│  ■■■■■  ■■■■■  ─────  ─────  ───│  progress: segment 2 active
│                            Skip │  #888888, Regular 14px, 44×44pt
│                                 │
│                                 │
│         ┌───────────┐           │
│         │           │           │
│         │   ╔═══╗   │           │  SF Symbol: cup.and.saucer.fill
│         │   ║ b ║   │           │  80pt, #FFFFFF, centered
│         │   ╚═══╝   │           │
│         │           │           │
│         └───────────┘           │
│                                 │
│  Pair your Brezi.               │  CondensedBold, 38px, #FFFFFF
│                                 │  bottom padding: 8pt (sm)
│  Power on your Brezi.           │  Regular, 17px, #CCCCCC
│  We'll find each other.         │  line-height: 1.5
│                                 │
│  ┌─────────────────────────┐    │  blue filled (#488BED)
│  │  SEARCH FOR BREZI       │    │  CondensedBold, 16px, #FFFFFF, ALL CAPS
│  └─────────────────────────┘    │  height: 48pt, radius: 12pt
│                                 │  horizontal margin: 24pt (lg)
│  I don't have one yet ›         │  Regular, 14px, #888888, centered
│                                 │  44×44pt tap area
└─────────────────────────────────┘
```

**Copy (exact strings):**
```swift
let screen2_headline = "Pair your Brezi."
let screen2_body = "Power on your Brezi. We'll find each other."
let screen2_cta = "SEARCH FOR BREZI"
let screen2_skip = "I don't have one yet"
let screen2_skipToast = "No problem. Pair anytime from Device Hub."
```

**"I don't have one yet" behavior:**
1. Store `breziPaired = false` in UserDefaults
2. Show toast: `screen2_skipToast` — Mono 13px, white on `#1A1A1A`, radius 8pt, bottom-center, auto-dismiss 2.5s
3. Push Screen 3

#### State: Searching

```
│  ┌─────────────────────────┐    │  blue filled (#488BED)
│  │  ◌  SEARCHING...        │    │  spinner (ProgressView) left of text
│  └─────────────────────────┘    │  CondensedBold, 16px, #FFFFFF, ALL CAPS
│                                 │  height: 48pt
│  Make sure Brezi is powered on  │  Mono, 12px, #888888, centered
│  and within range.              │
```

- Spinner: system `ProgressView()` tinted white, 16pt, 8pt right of spinner to text
- 30-second timeout. On timeout → State: deviceNotFound.

#### State: Found / Paired (happy path)

```
│  ┌─────────────────────────────┐│  background: #1A1A1A
│  │  ✓  BREZI-A4F2              ││  ✓: SF Symbol checkmark.circle.fill, #34C759, 20pt
│  │     Connected                ││  Mono, 12px, #34C759
│  │     Firmware 1.2.0           ││  Mono, 11px, #888888
│  └─────────────────────────────┘│  border: 1px #34C759, radius: 12pt, padding: 16pt
│                                 │
│  ┌─────────────────────────┐    │  blue filled (#488BED)
│  │  CONTINUE  →            │    │  CondensedBold, 16px, #FFFFFF, ALL CAPS
│  └─────────────────────────┘    │  height: 48pt
```

**Animation:** Device card fades in (0.3s easeOut). Checkmark scales from 0.5→1.0 (0.2s spring).

**Copy:**
```swift
let screen2_connected = "Connected"
let screen2_continue = "CONTINUE"
```

#### State: Bluetooth Off — `DevicePairFailureCard.swift`

```
│  ┌─────────────────────────────┐│  background: #1A1A1A
│  │  ⚠  Bluetooth is off.      ││  ⚠: SF Symbol exclamationmark.triangle.fill
│  │                             ││     #FF9F0A (statusAmber), 20pt
│  │  Brezi needs Bluetooth to   ││  Regular, 16px, #FFFFFF (first line)
│  │  connect to your machine.   ││  Regular, 14px, #CCCCCC (rest)
│  │                             ││
│  │  Open Settings              ││  Regular, 14px, #488BED, 44×44pt tap area
│  └─────────────────────────────┘│  left border: 3px #FF9F0A
│                                 │  radius: 12pt, padding: 16pt
│  Skip for now ›                 │  Regular, 14px, #888888, centered, 44×44pt
```

**Copy:**
```swift
let screen2_btOff_title = "Bluetooth is off."
let screen2_btOff_body = "Brezi needs Bluetooth to connect to your machine."
let screen2_btOff_action = "Open Settings"
let screen2_btOff_skip = "Skip for now"
```

**"Open Settings" behavior:** `UIApplication.shared.open(URL(string: UIApplication.openSettingsURLString)!)`

#### State: Device Not Found (30s timeout) — `DevicePairFailureCard.swift`

```
│  ┌─────────────────────────────┐│  background: #1A1A1A
│  │  Can't find your Brezi yet. ││  Regular, 16px, #FFFFFF
│  │                             ││
│  │  ○ Is it plugged in?        ││  Regular, 14px, #CCCCCC
│  │  ○ Is it within 10 feet?    ││  ○ = bullet, not interactive
│  │  ○ Try turning it off       ││
│  │    and back on.             ││
│  │                             ││
│  │  ┌─────────────────────┐   ││  outlined button: 1px #488BED border
│  │  │  TRY AGAIN          │   ││  CondensedBold, 14px, #488BED, ALL CAPS
│  │  └─────────────────────┘   ││  height: 44pt, radius: 12pt
│  └─────────────────────────────┘│  left border: 3px #FF9F0A
│                                 │  radius: 12pt, padding: 16pt
│  Skip for now ›                 │  Regular, 14px, #888888, centered, 44×44pt
```

**Copy:**
```swift
let screen2_notFound_title = "Can't find your Brezi yet."
let screen2_notFound_bullet1 = "Is it plugged in?"
let screen2_notFound_bullet2 = "Is it within 10 feet?"
let screen2_notFound_bullet3 = "Try turning it off and back on."
let screen2_notFound_retry = "TRY AGAIN"
```

**"TRY AGAIN" behavior:** Resets 30s Bluetooth scan from scratch.

#### State: Already Paired to Another Phone — `DevicePairFailureCard.swift`

```
│  ┌─────────────────────────────┐│  background: #1A1A1A
│  │  This Brezi is connected    ││  Regular, 16px, #FFFFFF
│  │  to another device.         ││
│  │                             ││
│  │  Disconnect it from the     ││  Regular, 14px, #CCCCCC
│  │  other phone first, then    ││
│  │  try again here.            ││
│  │                             ││
│  │  ┌─────────────────────┐   ││  outlined button: 1px #488BED border
│  │  │  TRY AGAIN          │   ││  CondensedBold, 14px, #488BED, ALL CAPS
│  │  └─────────────────────┘   ││  height: 44pt
│  └─────────────────────────────┘│  left border: 3px #FF453A (statusRed)
│                                 │  radius: 12pt, padding: 16pt
│  Skip for now ›                 │  Regular, 14px, #888888, centered, 44×44pt
```

**Copy:**
```swift
let screen2_alreadyPaired_title = "This Brezi is connected to another device."
let screen2_alreadyPaired_body = "Disconnect it from the other phone first, then try again here."
```

#### State: Firmware Update Required

```
│  ┌─────────────────────────────┐│  background: #1A1A1A
│  │  ✓  BREZI-A4F2  ·  Found   ││  ✓: #34C759, 20pt. border: 1px #34C759
│  │                             ││
│  │  Your Brezi needs a quick   ││  Regular, 14px, #CCCCCC
│  │  update before pairing.     ││
│  │  About 2 minutes.           ││
│  │                             ││
│  │  ┌─────────────────────┐   ││  blue filled (#488BED)
│  │  │  UPDATE NOW         │   ││  CondensedBold, 14px, #FFFFFF, ALL CAPS
│  │  └─────────────────────┘   ││  height: 44pt
│  └─────────────────────────────┘│  radius: 12pt, padding: 16pt
```

**Copy:**
```swift
let screen2_firmware_body = "Your Brezi needs a quick update before pairing. About 2 minutes."
let screen2_firmware_cta = "UPDATE NOW"
let screen2_firmware_progress = "Updating..."  // replaces button text during OTA
```

**After tap "UPDATE NOW":** Button transforms to progress bar:
```
│  │  ████████░░░░░░░░  43%      ││  bar: #488BED fill on #333333 track
│  │                             ││  bar height: 8pt, radius: 4pt
│  │  Updating...                ││  Mono, 12px, #888888
```

On OTA completion → auto-transition to Found/Paired state (0.5s crossfade).

#### State: Firmware Update Failed

```
│  ┌─────────────────────────────┐│  background: #1A1A1A
│  │  Update interrupted.        ││  Regular, 14px, #FFFFFF
│  │  Your Brezi is fine —       ││  Regular, 14px, #CCCCCC
│  │  nothing was changed.       ││
│  │                             ││
│  │  ┌─────────────────────┐   ││  outlined button: 1px #488BED
│  │  │  TRY AGAIN          │   ││  CondensedBold, 14px, #488BED, ALL CAPS
│  │  └─────────────────────┘   ││  height: 44pt
│  └─────────────────────────────┘│  left border: 3px #FF9F0A
│                                 │  radius: 12pt, padding: 16pt
```

**Copy:**
```swift
let screen2_otaFail_title = "Update interrupted."
let screen2_otaFail_body = "Your Brezi is fine — nothing was changed."
let screen2_otaFail_retry = "TRY AGAIN"
```

**Hardware note for Jensen:** Confirm the device rolls back to previous firmware on interrupted OTA. If it doesn't, this copy must change and a different recovery path is needed.

#### Bluetooth Permission (if denied by iOS)
```
│  Bluetooth access needed to     │  Regular, 14px, #CCCCCC, centered
│  find your Brezi.               │
│  Open Settings                  │  Regular, 14px, #488BED, 44×44pt
```
Request `CBCentralManager` authorization on "SEARCH FOR BREZI" tap.

---

### Screen 3 — Wearable Connect — `WearableConnectView.swift`

**Purpose:** Connect WHOOP / Oura / Apple Health, or skip to manual. Skip button visible.

#### State: Idle (selection)

```
┌─────────────────────────────────┐
│  ■■■■■  ■■■■■  ■■■■■  ─────  ───│  progress: segment 3 active
│                            Skip │  #888888, Regular 14px, 44×44pt
│                                 │
│  Your coffee should             │  CondensedBold, 40px, #FFFFFF
│  know how you slept.            │  line-height: 1.1
│                                 │  bottom padding: 8pt (sm)
│  Connect a wearable and Brezi   │  Regular, 17px, #CCCCCC
│  pulls your recovery data       │  line-height: 1.5
│  automatically. No typing.      │
│                                 │  bottom padding: 24pt (lg)
│  ┌─────────────────────────────┐│  WearableRowView.swift
│  │  ○  WHOOP                   ││  row 1
│  │     HRV · Recovery · Sleep  ││
│  └─────────────────────────────┘│  gap: 8pt (sm)
│  ┌─────────────────────────────┐│
│  │  ○  Oura Ring               ││  row 2
│  │     HRV · Readiness · Sleep ││
│  └─────────────────────────────┘│  gap: 8pt (sm)
│  ┌─────────────────────────────┐│
│  │  ○  Apple Health            ││  row 3
│  │     HRV · Sleep · Activity  ││
│  └─────────────────────────────┘│
│                                 │  padding: 24pt (lg)
│  ┌─────────────────────────┐    │  blue filled (#488BED), DISABLED until selection
│  │  CONNECT  →             │    │  CondensedBold, 16px, #FFFFFF, ALL CAPS
│  └─────────────────────────┘    │  height: 48pt, opacity: 0.4 when disabled
│                                 │  horizontal margin: 24pt (lg)
│  or                             │  Mono, 12px, #888888, centered
│  I'll log manually ›            │  Regular, 15px, #CCCCCC, centered, 44×44pt
│                                 │
└─────────────────────────────────┘
```

**Copy:**
```swift
let screen3_headline = "Your coffee should\nknow how you slept."
let screen3_body = "Connect a wearable and Brezi pulls your recovery data automatically. No typing."
let screen3_cta = "CONNECT"
let screen3_manual_prefix = "or"
let screen3_manual_link = "I'll log manually"

let screen3_whoop_name = "WHOOP"
let screen3_whoop_caps = "HRV · Recovery · Sleep"
let screen3_oura_name = "Oura Ring"
let screen3_oura_caps = "HRV · Readiness · Sleep"
let screen3_apple_name = "Apple Health"
let screen3_apple_caps = "HRV · Sleep · Activity"
```

**WearableRowView.swift spec:**
- Background: `#1A1A1A`
- Border: 1px `#333333`
- Radius: 12pt
- Height: 64pt
- Horizontal padding: 16pt (md)
- Radio circle: 20pt diameter, border 1.5px `#333333`, no fill
- Selected state: circle fill `#488BED`, white checkmark (SF Symbol `checkmark`, 12pt), border `#488BED` 2px on row
- Name: Regular, 17px, #FFFFFF
- Capabilities: Mono, 11px, #888888
- Tap target: full row (64pt height)
- Single selection: tapping a new row deselects the previous

**"I'll log manually" behavior:**
1. Store `wearableSource = "manual"` in UserDefaults
2. Push Screen 4

**"CONNECT" behavior (WHOOP / Oura):** Opens OAuth `ASWebAuthenticationSession`. Three possible outcomes:

#### State: OAuth Cancelled by User

No error state. Return to idle. The selected wearable row deselects. Silent — the user changed their mind.

#### State: OAuth Failed (network/server)

```
│  ┌─────────────────────────────┐│  background: #1A1A1A
│  │  Couldn't reach WHOOP       ││  Regular, 16px, #FFFFFF
│  │  right now.                  ││
│  │                              ││
│  │  Their servers might be      ││  Regular, 14px, #CCCCCC
│  │  busy. Try again or skip     ││
│  │  and connect later from      ││
│  │  Settings.                   ││
│  │                              ││
│  │  ┌─────────────────────┐    ││  outlined: 1px #488BED
│  │  │  TRY AGAIN           │    ││  CondensedBold, 14px, #488BED, ALL CAPS
│  │  └─────────────────────┘    ││  height: 44pt
│  └─────────────────────────────┘│  left border: 3px #FF9F0A
│                                 │  radius: 12pt, padding: 16pt
│  I'll log manually ›            │  Regular, 15px, #CCCCCC, centered, 44×44pt
```

**Copy (parameterized by wearable name):**
```swift
func screen3_oauthFail_title(_ name: String) -> String {
    "Couldn't reach \(name) right now."
}
let screen3_oauthFail_body = "Their servers might be busy. Try again or skip and connect later from Settings."
let screen3_oauthFail_retry = "TRY AGAIN"
```

#### State: OAuth Succeeded, Data Pull Pending

```
│  ┌─────────────────────────────┐│  background: #1A1A1A
│  │  ✓  WHOOP  ·  Connected     ││  ✓: #34C759. "Connected": Mono, 12px, #34C759
│  │     ◌ Syncing your data...  ││  ◌: ProgressView, 12pt, #FFD60A
│  │     We'll pull it when       ││  Mono, 11px, #888888
│  │     the app opens.           ││
│  └─────────────────────────────┘│  border: 1px #34C759, radius: 12pt, padding: 16pt
│                                 │
│  ┌─────────────────────────┐    │  blue filled (#488BED), ENABLED
│  │  CONTINUE  →            │    │  CondensedBold, 16px, #FFFFFF, ALL CAPS
│  └─────────────────────────┘    │  height: 48pt
```

**Copy:**
```swift
let screen3_syncing = "Syncing your data..."
let screen3_syncNote = "We'll pull it when the app opens."
```

Do NOT block onboarding. User advances to Screen 4. If wearable data hasn't synced by dashboard, Card 1 falls back to manual mode with note: `"WHOOP connected · Waiting for first sync. Check in manually for now."`

**"CONNECT" behavior (Apple Health):** Shows `HKHealthStore.requestAuthorization` dialog. On grant → same "Connected" state. On deny → return to idle, deselect Apple Health row.

---

### Screen 4 — The Chain Tutorial — `DataChainTutorialView.swift`

**Purpose:** Show data → insight → brew chain with example data. No skip button.

```
┌─────────────────────────────────┐
│  ■■■■■  ■■■■■  ■■■■■  ■■■■■  ───│  progress: segment 4 active
│                                 │
│  Your data becomes              │  CondensedBold, 36px, #FFFFFF
│  your brew.                     │  line-height: 1.1
│                                 │  bottom padding: 24pt (lg)
│  ┌─ YOUR DATA ────────────────┐ │  DataChainView.swift — card 1
│  │  ↓                         │ │  ↓: Mono, 14px, #488BED
│  │  HRV: 42ms                 │ │  Mono, 16px, #FFFFFF
│  │  Recovery: 61%             │ │  Mono, 16px, #FFFFFF
│  │  Sleep: 6h 48m             │ │  Mono, 16px, #FFFFFF
│  │  Source: WHOOP             │ │  Mono, 11px, #888888
│  └────────────────────────────┘ │  bg: #1A1A1A, radius: 16pt, padding: 16pt
│           │                     │
│           ▼                     │  blue connecting line: 2px wide, #488BED
│  ┌─ BREZI'S READ ─────────────┐ │  DataChainView.swift — card 2
│  │                             │ │
│  │  "Mid recovery. Your HRV   │ │  Regular, 15px, #CCCCCC
│  │   is 18% below baseline.   │ │
│  │   Not a crash — just a     │ │
│  │   dip. Go standard."       │ │
│  │                             │ │
│  └────────────────────────────┘ │  bg: #1A1A1A, blue left border 3px, radius: 16pt
│           │                     │  padding: 16pt
│           ▼                     │  blue connecting line
│  ┌─ YOUR BREW ────────────────┐ │  DataChainView.swift — card 3
│  │                             │ │
│  │  FIRST LIGHT               │ │  CondensedBold, 26px, #FFFFFF
│  │  4–8°C · 12min · 60g/L    │ │  Mono, 14px, #888888
│  │  Medium roast · Medium grind│ │  Mono, 12px, #888888
│  │  1 serve · Best before 2pm │ │  Mono, 12px, #488BED
│  │                             │ │
│  └────────────────────────────┘ │  bg: #0F0F0F, blue top border 3px, radius: 16pt
│                                 │  padding: 16pt
│  ┌─────────────────────────┐    │  blue filled (#488BED)
│  │  I SEE IT  →            │    │  CondensedBold, 16px, #FFFFFF, ALL CAPS
│  └─────────────────────────┘    │  height: 48pt, horizontal margin: 24pt (lg)
│                                 │
└─────────────────────────────────┘
```

**Copy:**
```swift
let screen4_headline = "Your data becomes\nyour brew."
let screen4_sectionLabel1 = "YOUR DATA"
let screen4_sectionLabel2 = "BREZI'S READ"
let screen4_sectionLabel3 = "YOUR BREW"
let screen4_exampleHRV = "HRV: 42ms"
let screen4_exampleRecovery = "Recovery: 61%"
let screen4_exampleSleep = "Sleep: 6h 48m"
let screen4_exampleSource = "Source: WHOOP"
let screen4_exampleInsight = "\"Mid recovery. Your HRV is 18% below baseline. Not a crash — just a dip. Go standard.\""
let screen4_exampleBrew = "FIRST LIGHT"
let screen4_exampleBrewParams = "4–8°C · 12min · 60g/L"
let screen4_exampleBrewDetail = "Medium roast · Medium grind"
let screen4_exampleBrewTiming = "1 serve · Best before 2pm"
let screen4_cta = "I SEE IT"
```

**Section label spec:** Mono, 11px, #888888, ALL CAPS. Positioned top-left inside card, 12pt padding from top.

**Connecting lines:** Vertical, 2px wide, #488BED, 24pt tall, centered between cards.

**Animations:**
| Element | Type | Duration | Easing | Delay |
|---|---|---|---|---|
| Card 1 (YOUR DATA) | fadeIn + translateY(16→0) | 0.3s | easeOut | 0.0s |
| Connecting line 1 | drawFromTop (height 0→24pt) | 0.2s | easeOut | 0.3s |
| Card 2 (BREZI'S READ) | fadeIn + translateY(16→0) | 0.3s | easeOut | 0.5s |
| Connecting line 2 | drawFromTop | 0.2s | easeOut | 0.8s |
| Card 3 (YOUR BREW) | fadeIn + translateY(16→0) | 0.3s | easeOut | 1.0s |
| CTA button | fadeIn | 0.2s | easeOut | 1.3s |

**Total animation sequence:** ~1.5s.

**Interruptible animation:** Tapping "I SEE IT" at any point during the sequence completes all pending animations immediately (snap all opacity to 1, all translateY to 0, all lines to full height) and advances to Screen 5. Do not trap an impatient user.

**Content is example data** — not the user's real data.

---

### Screen 5 — First Check-In — `FirstCheckInView.swift`

**Purpose:** Quick emotional check-in to seed first recommendation. No skip button.

```
┌─────────────────────────────────┐
│  ■■■■■  ■■■■■  ■■■■■  ■■■■■  ■■■│  progress: segment 5 active
│                                 │
│  How are you waking             │  CondensedBold, 40px, #FFFFFF
│  up today?                      │  line-height: 1.1
│                                 │  bottom padding: 8pt (sm)
│  Honest answer. Brezi           │  Regular, 15px, #888888
│  adjusts.                       │  line-height: 1.4
│                                 │  bottom padding: 24pt (lg)
│  ┌─────────────────────────────┐│  OnboardingSelectionCard.swift
│  │  🟢  Peaked                 ││  card 1 — maps to recoveryScore: 9
│  │  HRV high, slept clean      ││
│  └─────────────────────────────┘│  gap: 8pt (sm)
│  ┌─────────────────────────────┐│
│  │  🟡  Functional             ││  card 2 — maps to recoveryScore: 6
│  │  Decent night, a little     ││
│  │  behind                     ││
│  └─────────────────────────────┘│  gap: 8pt (sm)
│  ┌─────────────────────────────┐│
│  │  🔴  Rebuilding             ││  card 3 — maps to recoveryScore: 3
│  │  Wrecked it yesterday       ││
│  └─────────────────────────────┘│
│                                 │  padding: 24pt (lg)
│  ┌─────────────────────────┐    │  blue filled (#488BED)
│  │  BUILD MY FIRST BREW  → │    │  DISABLED until selection
│  └─────────────────────────┘    │  CondensedBold, 16px, #FFFFFF, ALL CAPS
│                                 │  height: 48pt, opacity: 0.4 when disabled
└─────────────────────────────────┘
```

**Copy:**
```swift
let screen5_headline = "How are you waking\nup today?"
let screen5_subhead = "Honest answer. Brezi adjusts."
let screen5_cta = "BUILD MY FIRST BREW"

let screen5_option1_emoji = "🟢"
let screen5_option1_label = "Peaked"
let screen5_option1_sublabel = "HRV high, slept clean"
let screen5_option1_score = 9

let screen5_option2_emoji = "🟡"
let screen5_option2_label = "Functional"
let screen5_option2_sublabel = "Decent night, a little behind"
let screen5_option2_score = 6

let screen5_option3_emoji = "🔴"
let screen5_option3_label = "Rebuilding"
let screen5_option3_sublabel = "Wrecked it yesterday"
let screen5_option3_score = 3
```

**OnboardingSelectionCard.swift spec:**
- Default: border 1px `#333333`, background `#1A1A1A`, radius 16pt, padding 20pt
- Selected: border 2px `#488BED`, background `rgba(72, 139, 237, 0.12)`, scale(1.01) spring animation (response: 0.35, dampingFraction: 0.7)
- Emoji: 28pt, inline left of label, 8pt gap
- Label: CondensedBold, 20px, #FFFFFF
- Sublabel: Regular, 14px, #888888, top padding 4pt (xs)
- Min height: 64pt (ensures 44pt tap target)
- Single selection: tapping a new card deselects the previous

**Transition after CTA tap:**
1. Push to full-screen black view
2. Show: `"Reading your signal..."` — Mono, 16px, #FFFFFF, centered vertically and horizontally
3. Pulse animation: opacity 1.0 ↔ 0.5, duration 1.2s, easeInOut, repeating
4. Duration: 0.8s max
5. Push `DashboardView` with first brew recommendation pre-calculated
6. Store `hasCompletedOnboarding = true` in UserDefaults

**Copy:**
```swift
let transition_loading = "Reading your signal..."
```

---

## 3. Dashboard — `DashboardView.swift`

The dashboard has **three temporal states** that cover the entire day:
1. **Pre-Brew** — morning, checked in but not yet brewed
2. **Brewing** — active brew in progress
3. **Post-Brew** — brewed today, rest of day until next morning

Additional overlay states: **Post-Cutoff** (past caffeine window) modifies both pre-brew and post-brew.

### Greeting Logic
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
- Font: Mono, 11px, #888888, ALL CAPS
- Below greeting: date in format "MARCH 19" — Mono, 11px, #888888, ALL CAPS

### Dashboard Shell
- Background: `#F5F5F5` (offWhite)
- Content padding: 16pt (md) horizontal
- Card gap: 16pt (md) vertical
- Pull-to-refresh: standard iOS `refreshable {}`. Loading text: `"Pulling your signal..."` — Mono, 13px, #888888. Re-fetches wearable data, recalculates recommendation.

---

### Dashboard State 1: Pre-Brew — Wearable User

**When:** User has checked in today, hasn't brewed yet, has wearable connected.

```
┌─────────────────────────────────┐
│  ⟨📡⟩            bre:zi       ⚙  │  NavBarView.swift
│─────────────────────────────────│
│                                 │  #F5F5F5 background
│  GOOD MORNING                   │  Mono, 11px, #888888
│  MARCH 19                       │  Mono, 11px, #888888
│                                 │  bottom padding: 16pt (md)
│  ┌─ 1  YOUR DATA ─────────────┐ │  HealthDataCardView.swift
│  │                             │ │
│  │  ┌──────┐ ┌──────┐ ┌─────┐ │ │
│  │  │  42  │ │  61% │ │ 6:48│ │ │  t_data (28px bold mono), #000000
│  │  │  HRV │ │ RECV │ │ SLP │ │ │  Mono, 10px, #888888, ALL CAPS
│  │  │  ms  │ │      │ │ hrs │ │ │  Mono, 10px, #888888
│  │  └──────┘ └──────┘ └─────┘ │ │  3 equal columns, 8pt (sm) gap
│  │                             │ │
│  │  ↓ 18% vs your baseline    │ │  Mono, 12px, #FFD60A (statusYellow)
│  │  Source: WHOOP · 6:42am    │ │  Mono, 11px, #888888
│  │                             │ │
│  │  Feeling different?         │ │  Mono, 12px, #000000
│  │  Update your check-in ›    │ │  Mono, 12px, #488BED, 44×44pt tap area
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, shadow, radius: 16pt
│                                 │  padding: 16pt (md) inner
│  ┌─ 2  BREZI'S READ ──────────┐ │  InsightStripView.swift
│  │                             │ │
│  │  You're sharp today. Full   │ │  Regular, 15px, #000000
│  │  signal. Let's use it.      │ │
│  │                             │ │
│  │  HRV is 18% below your     │ │  Mono, 13px, #888888
│  │  7-day average.             │ │  (contextual suffix, wearable only)
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, blue left border 3px
│                                 │  radius: 16pt, padding: 16pt (md)
│  ┌─ 3  YOUR BREW ─────────────┐ │  DashboardBrewCardView.swift
│  │                             │ │
│  │  FIRST LIGHT               │ │  CondensedBold, 32px, #FFFFFF
│  │  Your everyday standard.   │ │  Regular, 14px, #CCCCCC
│  │                             │ │  bottom padding: 16pt (md)
│  │  ┌────┐  ┌────┐  ┌────┐   │ │
│  │  │4–8°│  │ 12 │  │ 60 │   │ │  CondensedBold, 22px, #FFFFFF
│  │  │ C  │  │min │  │g/L │   │ │  Mono, 10px, #666666, ALL CAPS
│  │  │TEMP│  │TIME│  │DOSE│   │ │  3 equal columns, 8pt gap
│  │  └────┘  └────┘  └────┘   │ │
│  │                             │ │  padding: 16pt (md) below
│  │  Medium roast · Medium grind│ │  Mono, 13px, #888888
│  │  1 serve · Best before 2pm │ │  Mono, 13px, #488BED
│  │                             │ │  padding: 24pt (lg) below
│  │  ┌─────────────────────┐   │ │  white filled button
│  │  │  START BREW  →      │   │ │  CondensedBold, 16px, #000000, ALL CAPS
│  │  └─────────────────────┘   │ │  bg: #FFFFFF, height: 48pt, radius: 12pt
│  │                             │ │
│  │  See full details ›        │ │  Regular, 14px, #888888, centered, 44×44pt
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #0F0F0F, radius: 24pt (brewCard)
│                                 │  padding: 24pt (lg) inner
│  ┌─────────────────────────┐    │  outlined button
│  │  VIEW BREW HISTORY      │    │  CondensedBold, 14px, #000000, ALL CAPS
│  └─────────────────────────┘    │  bg: transparent, border: 1px #000000
│                                 │  height: 48pt, radius: 12pt
│                                 │  horizontal margin: 16pt (md)
└─────────────────────────────────┘
```

**Card 1 section label spec:** `"1  YOUR DATA"` — the number and label are Mono, 11px, #888888, ALL CAPS. Positioned as card header with 16pt padding.

**Metric display rules by source:**

| Source | Col 1 | Col 2 | Col 3 |
|---|---|---|---|
| WHOOP | HRV (ms) | Recovery % | Sleep (hrs) |
| Oura | HRV (ms) | Readiness score | Sleep (hrs) |
| Apple Health | HRV (ms) | *(hide column)* | Sleep (hrs) |
| Manual | Recovery /10 | *(hide column)* | *(hide column)* |

**Baseline comparison line:**
- Arrow: `↓` or `↑` — Mono, 12px
- Color: `↓` = statusYellow. `↑` = statusGreen. Within ±5% = #888888 ("= baseline")
- Format: `"↓ 18% vs your baseline"` or `"↑ 12% vs your baseline"` or `"= your baseline"`

**Source timestamp:**
- Format: `"Source: WHOOP · 6:42am"` — Mono, 11px, #888888
- If data >8 hours stale: timestamp turns `#FFD60A` (statusYellow)

**Re-check-in link:**
```swift
let reCheckIn_label = "Feeling different?"
let reCheckIn_action = "Update your check-in"
```
- Tap behavior (wearable): re-fetches latest wearable data, recalculates recommendation. Updates Cards 2 and 3 with spring animation (response: 0.4, dampingFraction: 0.8).
- Tap behavior (manual): shows `RecoverySliderView` as half-sheet modal (`.presentationDetents([.medium])`). On save → recalculates.

---

### Dashboard State 1: Pre-Brew — Manual User

**When:** User has checked in today, hasn't brewed, wearableSource == "manual".

Card 1 replaces metric columns with recovery slider:

```
│  ┌─ 1  YOUR DATA ─────────────┐ │  HealthDataCardView.swift (manual variant)
│  │                             │ │
│  │         [  5  ]             │ │  CondensedBold, 72px, #000000, centered
│  │    ○──────●──────○          │ │  RecoverySliderView.swift
│  │    1               10      │ │  Mono, 12px, #888888
│  │                             │ │
│  │  Quick set:                 │ │  Mono, 12px, #888888
│  │  [1–3 Rest] [4–6 OK] [7–10]│ │  QuickSetChipRow.swift
│  │                             │ │
│  │  Manual check-in logged.    │ │  Mono, 11px, #888888
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, shadow, radius: 16pt
```

**Copy:**
```swift
let manual_confirmation = "Manual check-in logged."
```

**No "Connect a wearable" link on this card.** The wearable connection option lives in Device Hub and Settings only.

**RecoverySliderView.swift spec:**
- Track: 4pt height, `#E0E0E0`, radius 2pt
- Fill: left side of thumb, `#488BED`
- Thumb: 28pt circle, `#FFFFFF`, shadow (2pt blur, 0.15 opacity)
- Range: 1–10, integer steps
- Labels: "1" left, "10" right — Mono, 12px, #888888
- Min tap target: 44pt vertical (extend hit area above/below track)

**QuickSetChipRow.swift spec:**
- 3 chips: `"1–3 Rest"`, `"4–6 OK"`, `"7–10 Peak"`
- Chip: bg `#1A1A1A`, radius 8pt, padding 8pt horizontal / 6pt vertical
- Text: Mono, 12px, #FFFFFF
- Selected chip: bg `#488BED`, text #FFFFFF
- Tap: sets slider to midpoint of range (2, 5, 8)
- Min height: 44pt per chip
- Gap: 8pt (sm)

**Manual-user Card 2 contextual suffix (3+ days of check-in history):**

When user has 3+ consecutive days of manual check-ins, Card 2 (InsightStripView) appends a suffix line below the insight:

```
│  │  Not your best, not your    │ │  Regular, 15px, #000000 (main insight)
│  │  worst. Keep it clean today.│ │
│  │                             │ │
│  │  3-day trend: improving     │ │  Mono, 13px, #888888 (suffix)
```

**Suffix types (alternating):**
- **Trend suffix:** `"3-day trend: improving"` / `"3-day trend: steady"` / `"3-day trend: declining"` — based on direction of last 3 manual scores
- **Streak suffix:** `"Checked in N days straight"` — counts consecutive days with manual check-in

**Suffix logic:**
```swift
func manualSuffix(checkIns: [CheckIn]) -> String? {
    let consecutive = consecutiveDayCount(checkIns)
    guard consecutive >= 3 else { return nil }
    let lastSuffixType = UserDefaults.standard.string(forKey: "lastManualSuffixType") ?? "streak"
    if lastSuffixType == "streak" {
        UserDefaults.standard.set("trend", forKey: "lastManualSuffixType")
        let trend = trendDirection(last3: checkIns.prefix(3).map(\.recoveryScore))
        return "3-day trend: \(trend)"  // "improving" / "steady" / "declining"
    } else {
        UserDefaults.standard.set("streak", forKey: "lastManualSuffixType")
        return "Checked in \(consecutive) days straight"
    }
}
```

- Days 1–2: no suffix shown
- Days 3+: one suffix line, alternating type each day

---

### Dashboard State 2: Brewing — `BrewProgressView.swift`

**When:** User tapped "START BREW" and machine is running.

Full-screen dark overlay on the dashboard. Background: `#000000`.

```
┌─────────────────────────────────┐
│  ⟨📡⟩            bre:zi       ⚙  │  NavBarView.swift
│─────────────────────────────────│
│                                 │  #000000 background
│                                 │
│                                 │
│         ┌───────────┐           │
│         │           │           │
│         │   ╔═══╗   │           │  SF Symbol: cup.and.saucer.fill
│         │   ║ b ║   │           │  64pt, #FFFFFF
│         │   ╚═══╝   │           │  subtle pulse: scale 1.0↔1.03, 2s, easeInOut, repeat
│         │           │           │
│         └───────────┘           │
│                                 │
│  Brewing First Light...         │  Mono, 16px, #FFFFFF, centered
│                                 │  bottom padding: 24pt (lg)
│  ████████████░░░░░░░░  67%      │  progress bar
│                                 │  bar: #488BED fill on #333333 track
│  4–8°C · 8 of 12 minutes       │  bar height: 8pt, radius: 4pt
│                                 │  Mono, 13px, #888888, centered
│  Cancel Brew                    │  Regular, 14px, #FF453A, centered
│                                 │  44×44pt tap area
│                                 │
└─────────────────────────────────┘
```

**Copy:**
```swift
func brewing_status(_ profileName: String) -> String {
    "Brewing \(profileName)..."
}
func brewing_progress(_ temp: String, _ elapsed: Int, _ total: Int) -> String {
    "\(temp) · \(elapsed) of \(total) minutes"
}
let brewing_cancel = "Cancel Brew"
```

**Progress bar:** Maps to real brew duration from Brezi machine via Bluetooth. Updates in real-time.

**"START BREW" behavior:**
- If Brezi paired + BLE connected: sends brew parameters to machine via Bluetooth → transitions to Brewing state
- If Brezi not paired OR BLE disconnected: behaves as "LOG THIS BREW" → records to BrewLog → shows `BrewLoggedConfirmation` → transitions to Post-Brew state

---

### Brew Cancel Flow

**Trigger:** Tap "Cancel Brew" during brewing state.

#### Step 1: Inline Confirmation (not a modal)

Replaces the cancel link area with:

```
│                                 │
│  Cancel this brew?              │  Regular, 16px, #FFFFFF
│  Coffee in the machine will     │  Regular, 14px, #CCCCCC
│  be discarded.                  │
│                                 │
│  ┌──────────┐  ┌────────────┐  │
│  │  GO BACK  │  │  CANCEL IT  │  │
│  └──────────┘  └────────────┘  │
│                                 │
```

- "GO BACK": outlined button, 1px #FFFFFF border, Regular 14px #FFFFFF, ALL CAPS, height 44pt, radius 12pt
- "CANCEL IT": filled button, bg #FF453A, Regular 14px #FFFFFF, ALL CAPS, height 44pt, radius 12pt
- Both buttons: equal width, 8pt (sm) gap between them

**Animation:** Confirmation slides up from cancel link position (0.2s, easeOut). Progress bar + status text stay visible above.

**Copy:**
```swift
let brewCancel_title = "Cancel this brew?"
let brewCancel_body = "Coffee in the machine will be discarded."
let brewCancel_goBack = "GO BACK"
let brewCancel_confirm = "CANCEL IT"
```

#### Step 2: After Cancel

- App sends cancel command via Bluetooth, waits for ACK
- Dashboard returns to pre-brew state (crossfade, 0.3s)
- Brew is NOT logged. No "Cancelled" record.
- Recommendation stays the same. User can tap "START BREW" again or choose a different profile.

**Hardware edge case:** If the machine can't stop mid-dispense (last few seconds of pour):
- Confirmation body changes to: `"Stop after the current pour finishes."`
- Progress bar runs to the next safe stopping point, then transitions to pre-brew state

```swift
let brewCancel_body_midPour = "Stop after the current pour finishes."
```

---

### BLE Disconnect During Brew

**When:** App loses Bluetooth connection to Brezi during active brew.

Replaces brewing progress UI with:

```
│                                 │
│  Lost connection to Brezi.      │  Regular, 16px, #FFFFFF
│                                 │
│  Your brew may still be running │  Regular, 14px, #CCCCCC
│  on the machine.                │
│                                 │
│  ┌─────────────────────────┐    │  outlined: 1px #488BED
│  │  RECONNECT              │    │  CondensedBold, 14px, #488BED, ALL CAPS
│  └─────────────────────────┘    │  height: 48pt, radius: 12pt
│  Mark as brewed ›               │  Regular, 14px, #888888, centered, 44×44pt
│                                 │
```

**Copy:**
```swift
let bleLost_title = "Lost connection to Brezi."
let bleLost_body = "Your brew may still be running on the machine."
let bleLost_reconnect = "RECONNECT"
let bleLost_markBrewed = "Mark as brewed"
```

**"RECONNECT":** Re-initiates BLE scan. On reconnect → resumes brewing state if brew still running, or transitions to post-brew if brew completed.
**"Mark as brewed":** Transitions to post-brew state. Logs brew with `connectionLost: true` flag. User still gets rating prompt.

---

### Brew Completion Overlay — `BrewCompletionOverlay.swift`

**When:** Brew finishes (progress hits 100%). This is the PRIMARY rating moment.

Full-screen overlay. Background: `#000000`. No auto-dismiss — user controls timing.

```
┌─────────────────────────────────┐
│                                 │
│                                 │
│         Brewed.                 │  CondensedBold, 48px, #FFFFFF
│                                 │  bottom padding: 16pt (md)
│         FIRST LIGHT             │  Mono, 14px, #888888
│         7:14am · 12 min         │  Mono, 14px, #888888
│                                 │  bottom padding: 32pt (xl)
│  How was it?                    │  Regular, 16px, #CCCCCC
│                                 │  bottom padding: 16pt (md)
│  ☆ ☆ ☆ ☆ ☆                     │  RatingStarsView.swift
│                                 │  star size: 36pt, gap: 8pt
│                                 │  tappable, default: all unselected
│                                 │  bottom padding: 32pt (xl)
│  ┌─────────────────────────┐    │  blue filled (#488BED)
│  │  DONE  →                │    │  CondensedBold, 16px, #FFFFFF, ALL CAPS
│  └─────────────────────────┘    │  height: 48pt, radius: 12pt
│                                 │  horizontal margin: 24pt (lg)
│  Skip for now ›                 │  Regular, 14px, #888888, centered
│                                 │  44×44pt tap area
└─────────────────────────────────┘
```

**Copy:**
```swift
let completion_headline = "Brewed."
func completion_meta(_ profileName: String, _ time: String, _ duration: String) -> String {
    "\(profileName)\n\(time) · \(duration)"
}
let completion_prompt = "How was it?"
let completion_done = "DONE"
let completion_skip = "Skip for now"
```

**RatingStarsView.swift spec:**
- 5 stars in a row
- Unselected: SF Symbol `star`, #333333, size as specified per usage
- Selected: SF Symbol `star.fill`, #FFD60A, size as specified per usage
- Tap: selects that star and all to its left (standard 1–5 behavior)
- Min tap target: 44×44pt per star
- Completion overlay: 36pt stars, 8pt gap
- Post-brew card: 28pt stars, 6pt gap
- History cells: 16pt stars, 4pt gap

**"DONE" behavior:**
- If stars selected → writes rating to `BrewLog.rating`, dismisses overlay
- If no stars selected → same as "Skip for now" (dismisses without rating)
- After dismiss → dashboard is already in post-brew state underneath. Overlay removal reveals it. No additional transition animation.

**"Skip for now" behavior:** Dismisses overlay. `BrewLog.rating` remains nil. Dashboard post-brew Card 3 shows "RATE THIS BREW" CTA as fallback.

**State awareness:** Single `BrewLog.rating` field. Completion overlay writes to it. Dashboard reads from it. If non-nil → show rating. If nil → show CTA. One source of truth.

**Animation on appear:**
| Element | Type | Duration | Easing | Delay |
|---|---|---|---|---|
| "Brewed." | fadeIn + translateY(12→0) | 0.4s | easeOut | 0.0s |
| Profile name + meta | fadeIn | 0.3s | easeOut | 0.3s |
| "How was it?" + stars | fadeIn | 0.3s | easeOut | 0.5s |
| DONE button + skip | fadeIn | 0.2s | easeOut | 0.7s |

---

### Brew Logged Confirmation (no device paired)

**When:** User taps "START BREW" but Brezi is not paired / not connected.

```
│                                 │  #000000 full screen
│  ✓                              │  SF Symbol: checkmark.circle.fill
│                                 │  #488BED, 48pt, centered
│  Logged.                        │  Mono, 14px, #888888, centered
│                                 │
```

Auto-dismiss: 1.2s. Then dashboard transitions to post-brew state.

```swift
let brewLogged_label = "Logged."
```

---

### Dashboard State 3: Post-Brew — `PostBrewCardView.swift`

**When:** User has brewed today. Persists until next morning check-in.

```
┌─────────────────────────────────┐
│  ⟨📡⟩            bre:zi       ⚙  │  NavBarView.swift
│─────────────────────────────────│
│                                 │  #F5F5F5 background
│  GOOD AFTERNOON                 │  Mono, 11px, #888888
│  MARCH 19                       │  Mono, 11px, #888888
│                                 │
│  ┌─ 1  YOUR DATA ─────────────┐ │  HealthDataCardView.swift
│  │                             │ │  (same as pre-brew — data doesn't change)
│  │  ┌──────┐ ┌──────┐ ┌─────┐ │ │
│  │  │  42  │ │  61% │ │ 6:48│ │ │  t_data, #000000
│  │  │  HRV │ │ RECV │ │ SLP │ │ │  Mono, 10px, #888888
│  │  │  ms  │ │      │ │ hrs │ │ │
│  │  └──────┘ └──────┘ └─────┘ │ │
│  │                             │ │
│  │  ↓ 18% vs your baseline    │ │  Mono, 12px, #FFD60A
│  │  Source: WHOOP · 6:42am    │ │  Mono, 11px, #888888
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, shadow, radius: 16pt
│                                 │
│  ┌─ 2  TODAY'S SIGNAL ────────┐ │  InsightStripView.swift
│  │                             │ │  (shifts to post-brew reflection copy)
│  │  You went First Light on a  │ │  Regular, 15px, #000000
│  │  mid-recovery morning.      │ │
│  │  That tracks.               │ │
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, blue left border 3px
│                                 │  radius: 16pt, padding: 16pt
│  ┌─ 3  TODAY'S BREW ──────────┐ │  PostBrewCardView.swift
│  │                             │ │
│  │  FIRST LIGHT               │ │  CondensedBold, 32px, #FFFFFF
│  │  Brewed at 7:14am          │ │  Mono, 13px, #888888
│  │                             │ │
│  │  4–8°C · 12min · 60g/L    │ │  Mono, 13px, #666666
│  │  Medium roast · Medium grind│ │  Mono, 13px, #666666
│  │                             │ │
│  │  ─────────────────────────  │ │  divider: 1px #333333
│  │                             │ │
│  │  ┌─────────────────────┐   │ │  [IF BrewLog.rating == nil]
│  │  │  RATE THIS BREW     │   │ │  white filled, #000000 text
│  │  └─────────────────────┘   │ │  CondensedBold, 16px, ALL CAPS
│  │                             │ │  height: 48pt, radius: 12pt
│  │  ── OR (if rated) ─────    │ │
│  │                             │ │  [IF BrewLog.rating != nil]
│  │  ★ ★ ★ ★ ☆                 │ │  RatingStarsView.swift, 28pt stars
│  │  Rated 4/5  ·  Edit ›      │ │  Mono, 13px, #888888
│  │                             │ │  "Edit ›": 44×44pt tap area
│  │  See you tomorrow morning.  │ │  Mono, 13px, #888888, centered
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #0F0F0F, radius: 24pt (brewCard)
│                                 │  padding: 24pt (lg) inner
│  ┌─────────────────────────┐    │  outlined button
│  │  VIEW BREW HISTORY      │    │  CondensedBold, 14px, #000000, ALL CAPS
│  └─────────────────────────┘    │  border: 1px #000000, height: 48pt
│                                 │
└─────────────────────────────────┘
```

**Copy:**
```swift
func postBrew_timestamp(_ time: String) -> String {
    "Brewed at \(time)"
}
let postBrew_rateCTA = "RATE THIS BREW"
func postBrew_ratedLabel(_ score: Int) -> String {
    "Rated \(score)/5  ·  Edit"
}
let postBrew_closing = "See you tomorrow morning."
```

**"RATE THIS BREW" tap:** Opens inline star picker (same as completion overlay stars, 28pt, within the card). Tap a star → saves immediately to `BrewLog.rating`. Card updates to rated state with spring animation (0.3s).

**"Edit ›" tap:** Opens inline star picker replacing the current rating display. Re-tap saves immediately. No modal.

---

### Post-Brew Reflection Copy — Complete Table (Card 2)

All 15 `(recoveryTier, brewProfile) → reflectionCopy` combinations. Jensen maps directly. No fallback logic.

```swift
let reflectionCopy: [String: [String: String]] = [
    "high": [
        "voltage":    "Peak morning, peak brew. You matched the signal.",
        "deep_work":  "Strong signal, focused pour. You're building something today.",
        "first_light": "Strong signal, light brew. Playing it steady.",
        "tide_pool":  "Everything's firing and you went gentle. Deliberate. Interesting.",
        "the_blank":  "Full signal, custom brew. You know what you wanted."
    ],
    "mid": [
        "voltage":    "You pushed past the signal. Let's see how it lands.",
        "deep_work":  "Mid morning, focused brew. Solid read on yourself.",
        "first_light": "You went First Light on a mid-recovery morning. That tracks.",
        "tide_pool":  "Playing it safe on a mid day. Nothing wrong with that.",
        "the_blank":  "Mid recovery, custom build. You're running your own experiment."
    ],
    "low": [
        "voltage":    "Low signal, full send. Bold. Rate this one honest.",
        "deep_work":  "Rough night but you went for it. Respect. Rate it honest.",
        "first_light": "Low morning, standard brew. Keeping the practice without pushing.",
        "tide_pool":  "Low morning, gentle brew. You listened.",
        "the_blank":  "Low recovery, custom call. Your body, your rules."
    ]
]
```

**Recovery tier mapping:**
```swift
func recoveryTier(score: Int) -> String {
    switch score {
    case 1...3:  return "low"
    case 4...6:  return "mid"
    case 7...10: return "high"
    default:     return "mid"
    }
}
```

---

### Dashboard State: Post-Cutoff (Past Caffeine Window)

**When:** Current time is past the user's `caffeineCutoffHour` setting (default: 2:00 PM / hour 14).

Applies as modifier to both pre-brew and post-brew states.

#### Post-Cutoff + Pre-Brew: Tide Pool Recommendation

**When:** Past cutoff, recovery is mid or high, user hasn't brewed today.

Card 2 changes:
```
│  ┌─ 2  BREZI'S READ ──────────┐ │  InsightStripView.swift
│  │                             │ │
│  │  Past your caffeine window. │ │  Regular, 15px, #000000
│  │  If you're still brewing,   │ │
│  │  go Tide Pool or skip today.│ │
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, blue left border 3px
```

Card 3 shifts to gentlest available profile:
```
│  ┌─ 3  YOUR BREW ─────────────┐ │  DashboardBrewCardView.swift
│  │                             │ │
│  │  TIDE POOL                 │ │  CondensedBold, 32px, #FFFFFF
│  │  Light caffeine. Easy exit. │ │  Regular, 14px, #CCCCCC
│  │                             │ │
│  │  ┌────┐  ┌────┐  ┌────┐   │ │
│  │  │ 4° │  │ 10 │  │ 40 │   │ │  CondensedBold, 22px, #FFFFFF
│  │  │ C  │  │min │  │g/L │   │ │  Mono, 10px, #666666
│  │  │TEMP│  │TIME│  │DOSE│   │ │
│  │  └────┘  └────┘  └────┘   │ │
│  │                             │ │
│  │  Light–Med roast · Med grind│ │  Mono, 13px, #888888
│  │  1 serve · Anytime         │ │  Mono, 13px, #488BED
│  │                             │ │
│  │  ┌─────────────────────┐   │ │
│  │  │  START BREW  →      │   │ │  white filled, #000000 text, 48pt
│  │  └─────────────────────┘   │ │
│  │                             │ │
│  │  See full details ›        │ │  Regular, 14px, #888888, 44×44pt
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #0F0F0F, radius: 24pt
```

**Copy:**
```swift
let postCutoff_insight = "Past your caffeine window. If you're still brewing, go Tide Pool or skip today."
let tidePools_tagline = "Light caffeine. Easy exit."
```

**Caffeine cutoff shift logic:**
```swift
func shiftedProfile(original: String, isPastCutoff: Bool) -> String {
    guard isPastCutoff else { return original }
    switch original {
    case "voltage":    return "deep_work"
    case "deep_work":  return "first_light"
    case "first_light": return "tide_pool"
    case "tide_pool":  return "tide_pool"  // already gentlest
    case "the_blank":  return "the_blank"  // user override
    default:           return "tide_pool"
    }
}
```

#### Post-Cutoff + Pre-Brew: Rest Day (Low Recovery + Past Cutoff)

**When:** Past cutoff AND recovery score 1–3. No brew recommended.

```
│  ┌─ 2  BREZI'S READ ──────────┐ │  InsightStripView.swift
│  │                             │ │
│  │  Your body's telling you to │ │  Regular, 15px, #000000
│  │  sit this one out. Listen   │ │
│  │  to it.                     │ │
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, blue left border 3px

│  ┌─ 3  REST DAY ──────────────┐ │  DashboardBrewCardView.swift (rest variant)
│  │                             │ │
│  │  No brew recommended.      │ │  CondensedBold, 24px, #FFFFFF
│  │                             │ │
│  │  Not today. Recovery's low  │ │  Regular, 14px, #CCCCCC
│  │  and the window's closed.   │ │
│  │  Tomorrow.                  │ │
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #0F0F0F, radius: 24pt
│                                 │  padding: 24pt (lg) inner
│  ┌─────────────────────────┐    │  outlined button
│  │  BREW ANYWAY             │    │  CondensedBold, 14px, #FFFFFF, ALL CAPS
│  └─────────────────────────┘    │  border: 1px #FFFFFF, height: 48pt, radius: 12pt
│                                 │  full-width (minus 16pt horizontal margin)
│  ┌─────────────────────────┐    │  outlined button
│  │  VIEW BREW HISTORY      │    │  CondensedBold, 14px, #000000, ALL CAPS
│  └─────────────────────────┘    │  border: 1px #000000, height: 48pt, radius: 12pt
│                                 │  gap: 8pt (sm) between buttons
```

**Copy:**
```swift
let restDay_title = "No brew recommended."
let restDay_body = "Not today. Recovery's low and the window's closed. Tomorrow."
let restDay_insight = "Your body's telling you to sit this one out. Listen to it."
let restDay_override = "BREW ANYWAY"
```

**"BREW ANYWAY" behavior:** Opens `ProfilePickerView` (full-screen modal). User selects a profile → returns to pre-brew state with that profile loaded. "BREW ANYWAY" is outside the card, full-width, visible. The user who overrides is making a deliberate choice, not finding a hidden escape hatch.

#### Post-Cutoff + Post-Brew

Same as standard post-brew state. No changes — the brew already happened.

---

### Insight Copy System — `InsightStripView.swift`

**Layout:**
- White card, blue left border 3px (#488BED), radius 16pt, padding 16pt
- 2–3 lines of natural language insight
- Main text: Regular, 15px, #000000
- Contextual suffix: Mono, 13px, #888888 (below main text, 8pt gap)
- No CTA — read-only context

**Pre-Brew Insight Copy — 3 variants per tier, rotation system:**

```swift
let insightVariants: [String: [String]] = [
    "high": [
        "You're sharp today. Full signal. Let's use it.",
        "Clean night. Your body's ready for something real.",
        "Everything's online. Don't waste this one."
    ],
    "mid": [
        "Rough edges from last night. Functional, not sharp. Easy start.",
        "Not your best, not your worst. Keep it clean today.",
        "Your system's running but it's not asking for more. Listen to it."
    ],
    "low": [
        "Yesterday hit you. Go light. Let your body come back on its own terms.",
        "You're rebuilding. This isn't the morning to push. Gentle brew, gentle day.",
        "Low signal. That's data, not a verdict. Rest and regroup."
    ]
]
```

**Rotation logic:**
```swift
func selectInsightVariant(tier: String) -> String {
    let variants = insightVariants[tier]!
    let lastUsedKey = "lastInsight_\(tier)"
    let lastUsedIndex = UserDefaults.standard.integer(forKey: lastUsedKey)
    let nextIndex = (lastUsedIndex + 1) % variants.count
    UserDefaults.standard.set(nextIndex, forKey: lastUsedKey)
    return variants[nextIndex]
}
```

Tracks `lastInsight_high`, `lastInsight_mid`, `lastInsight_low` in UserDefaults. Rotates so user doesn't see the same line twice in a week.

**Contextual suffix (wearable users only, appended below main insight):**
```swift
// When HRV baseline data exists:
"HRV is 18% below your 7-day average."     // Mono, 13px, #888888
// When sleep is notably short:
"Sleep was short at 5h 12m."                // Mono, 13px, #888888
```

Suffix selection logic: show HRV comparison if `|currentHRV - baselineHRV| / baselineHRV > 0.10`. Show sleep note if `sleepHours < 6.0`. If both qualify, show HRV (priority). If neither qualifies, no suffix.

---

### Dashboard — Empty States

| State | Location | Copy |
|---|---|---|
| First use, no wearable | Card 1 | Recovery slider (manual mode) |
| First use, no wearable | Card 2 | `"Your practice starts blank. Give it three mornings."` |
| First use, no wearable | Card 3 | Recommendation based on manual input |
| Has history, not brewed today | Below greeting | `"Nothing brewed yet today. Your data's ready. Your machine's ready. You're the last variable."` — Regular, 14px, #888888 |
| Wearable connected, sync pending | Card 1 | `"WHOOP connected · Waiting for first sync. Check in manually for now."` — Mono, 12px, #888888 below manual slider |

```swift
let empty_firstUse_insight = "Your practice starts blank. Give it three mornings."
let empty_notBrewed = "Nothing brewed yet today. Your data's ready. Your machine's ready. You're the last variable."
let empty_syncPending = "WHOOP connected · Waiting for first sync. Check in manually for now."
```

---

## 4. Brew Recommendation Detail — `BrewRecommendationView.swift`

**Entry:** Pushed from "See full details ›" on dashboard, or after selecting a different profile from `ProfilePickerView`.

```
┌─────────────────────────────────┐
│  ‹ Back            bre:zi    ⚙  │  NavBarView.swift
│─────────────────────────────────│
│                                 │  #F5F5F5 background
│  ┌─────────────────────────────┐│
│  │                             ││
│  │  TODAY'S BREW               ││  Mono, 11px, #488BED, ALL CAPS
│  │                             ││  bottom padding: 8pt (sm)
│  │  FIRST LIGHT               ││  CondensedBold, 42px, #FFFFFF
│  │                             ││  bottom padding: 8pt (sm)
│  │  Not a recovery day. Not   ││  Regular, 16px, #CCCCCC
│  │  a peak day. Just a morning││  line-height: 1.5
│  │  that needs to begin       ││
│  │  correctly.                ││
│  │                             ││  bottom padding: 24pt (lg)
│  │  ─────────────────────────  ││  divider: 1px #333333
│  │                             ││
│  │   TEMP      TIME     DOSE  ││  Mono, 10px, #666666, ALL CAPS
│  │  4–8°C     12 min   60g/L ││  CondensedBold, 24px, #FFFFFF
│  │                             ││  3 equal columns, 8pt gap
│  │  ─────────────────────────  ││  bottom padding: 16pt (md)
│  │                             ││
│  │   ROAST     GRIND    SERVE ││  Mono, 10px, #666666, ALL CAPS
│  │  Medium    Medium      1   ││  CondensedBold, 20px, #FFFFFF
│  │                             ││  BeanRecommendationRow.swift
│  │  ─────────────────────────  ││
│  │                             ││
│  │  ⏱ Best consumed before 2pm││  Mono, 13px, #488BED
│  │                             ││  ⏱: SF Symbol clock, 13pt, #488BED
│  │                             ││  bottom padding: 24pt (lg)
│  │  ┌─────────────────────┐   ││  white filled button
│  │  │  START BREW  →      │   ││  CondensedBold, 16px, #000000, ALL CAPS
│  │  └─────────────────────┘   ││  height: 48pt, radius: 12pt
│  │                             ││
│  └─────────────────────────────┘│  bg: #0F0F0F, radius: 24pt (brewCard)
│                                 │  padding: 24pt (lg) inner, full-bleed horizontal
│  ─────────────────────────────  │
│                                 │  padding: 16pt (md)
│  WHY THIS BREW                  │  Mono, 11px, #888888, ALL CAPS
│                                 │  bottom padding: 16pt (md)
│  ┌─ DATA ─────────────────────┐ │  DataChainView.swift — card 1
│  │  HRV: 42ms (↓18% baseline) │ │  Mono, 14px, #000000
│  │  Recovery: 61%              │ │
│  │  Sleep: 6h 48m              │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, border 1px #E0E0E0, radius: 16pt
│           │                     │  padding: 16pt (md)
│           ▼                     │  blue connecting line: 2px, #488BED, 24pt tall
│  ┌─ INSIGHT ──────────────────┐ │  DataChainView.swift — card 2
│  │  Mid recovery. HRV dipped  │ │  Regular, 14px, #000000
│  │  but sleep was adequate.   │ │
│  │  Moderate caffeine, clean  │ │
│  │  extraction, no overshoot. │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, blue left border 3px, radius: 16pt
│           │                     │
│           ▼                     │  blue connecting line
│  ┌─ BREW ─────────────────────┐ │  DataChainView.swift — card 3
│  │  First Light selected.     │ │  Regular, 14px, #000000
│  │  Score range: 5–6.         │ │
│  │  Your score: 5.            │ │
│  │                             │ │
│  │  You went Voltage last time │ │  Regular, 13px, #888888
│  │  at this recovery level and │ │  (only if prior rating data exists
│  │  loved it.                  │ │   for same recovery tier)
│  └─────────────────────────────┘ │  bg: #FFFFFF, blue top border 3px, radius: 16pt
│                                 │
│  ─────────────────────────────  │
│                                 │
│  ┌─────────────────────────┐    │  outlined button
│  │  ADJUST SETTINGS        │    │  CondensedBold, 14px, #000000, ALL CAPS
│  └─────────────────────────┘    │  border: 1px #000000, height: 48pt, radius: 12pt
│                                 │  bottom padding: 8pt (sm)
│  Choose a different brew ›      │  Regular, 14px, #888888, centered, 44×44pt
│                                 │
└─────────────────────────────────┘
```

**Copy:**
```swift
let brewDetail_sectionLabel = "TODAY'S BREW"
let brewDetail_whyLabel = "WHY THIS BREW"
let brewDetail_adjustCTA = "ADJUST SETTINGS"
let brewDetail_changeBrew = "Choose a different brew"
```

**Past rating acknowledgment (BREW card in chain):** Only shows if `BrewLog` contains a rated entry for the same recovery tier. Format: `"You went [Profile] last time at this recovery level and [loved it / rated it 3/5]."` — Regular, 13px, #888888.

**BeanRecommendationRow.swift spec:**
```swift
struct BeanRecommendationRow: View {
    let roast: String
    let grind: String
    var onTap: (() -> Void)? = nil    // commerce hook, nil for v1
}
```
Display-only for v1. When commerce layer exists, `onTap` links to "Order these beans."

### "Adjust Settings" — Manual Override Sheet — `ManualOverrideSheet.swift`

- Presentation: `.presentationDetents([.medium])` half-sheet modal
- Background: `#FFFFFF`
- Padding: 24pt (lg)

**Sliders:** Temp (°C), Time (min), Dose (g/L)
- Track: 4pt height, `#E0E0E0`, radius 2pt
- Fill: `#488BED` (normal) or `#FF9F0A` (amber warning zone, The Blank only)
- Thumb: 28pt circle, #FFFFFF, shadow
- Labels: current value above thumb — CondensedBold, 16px, #000000
- Range labels: Mono, 12px, #888888 at each end
- Min tap target: 44pt vertical

**Pickers:** Roast (Light / Medium / Dark), Grind (Fine / Medium / Coarse)
- Segmented control style: chips in a row
- Selected: bg #488BED, text #FFFFFF
- Unselected: bg #1A1A1A, text #888888
- Chip: radius 8pt, min height 44pt, padding 12pt horizontal

**Bounded ranges per profile:**

| Profile | Temp Range | Time Range | Dose Range |
|---|---|---|---|
| Tide Pool | 2–6°C | 8–12 min | 30–55 g/L |
| First Light | 3–10°C | 10–14 min | 50–70 g/L |
| Deep Work | 8–18°C | 12–16 min | 65–90 g/L |
| Voltage | 12–22°C | 13–18 min | 70–95 g/L |
| The Blank | 1–25°C | 5–20 min | 20–100 g/L |

Sliders constrained to profile's range.

**Amber warning zone (The Blank only):**
When values go outside recommended ranges (Tide Pool's ranges as "recommended"):
- Slider track turns from `#488BED` to `#FF9F0A` (statusAmber)
- Below slider: `"Outside recommended range"` — Mono, 11px, #FF9F0A
- No hard block — The Blank is for people who know what they're doing
- Warning disappears when value returns to safe range

**CTA:**
```
│  ┌─────────────────────────┐    │  blue filled (#488BED)
│  │  APPLY & START BREW  →  │    │  CondensedBold, 16px, #FFFFFF, ALL CAPS
│  └─────────────────────────┘    │  height: 48pt, radius: 12pt
```

**Copy:**
```swift
let override_cta = "APPLY & START BREW"
let override_amberWarning = "Outside recommended range"
```

**Live preview:** Adjusting sliders updates the parameter display in real-time.
**Saving:** Override stores to `BrewLog.brewSettingsJSON`.

### "Choose a different brew" — Profile Picker — `ProfilePickerView.swift`

- Presentation: full-screen modal (`.fullScreenCover`)
- Background: `#F5F5F5`
- Nav bar: "Choose Your Brew" — CondensedBold, 20px, #000000. Close button (SF Symbol `xmark`) top-right, 44×44pt.

**5 profile cards in scrollable list:**

```
│  ┌─────────────────────────────┐│  ProfileCardView.swift
│  │  RECOMMENDED                ││  Mono, 10px, #488BED (badge, only on recommended)
│  │                             ││
│  │  FIRST LIGHT               ││  CondensedBold, 24px, #FFFFFF
│  │  Your everyday standard.   ││  Regular, 14px, #CCCCCC
│  │                             ││
│  │  4–8°C · 12min · 60g/L    ││  Mono, 12px, #888888
│  │  Score range: 5–6          ││  Mono, 12px, #488BED
│  │                             ││
│  └─────────────────────────────┘│  bg: #0F0F0F, radius: 16pt, padding: 16pt
```

- Recommended profile: full opacity, "RECOMMENDED" badge
- Other profiles: opacity 0.7, no badge
- Tap to select → replaces current recommendation, dismisses modal
- Card gap: 8pt (sm)

**Profile specs:**

| Profile | Score | Temp | Time | Dose | Roast | Grind | Serve | Timing |
|---|---|---|---|---|---|---|---|---|
| Tide Pool | 1–4 | 4°C | 10 min | 40–50 g/L | Light–Med | Medium | 1 | Anytime |
| First Light | 5–6 | 4–8°C | 12 min | 60 g/L | Medium | Medium | 1 | Before 2pm |
| Deep Work | 7–8 | 10–15°C | 14 min | 80 g/L | Medium | Med-Coarse | 1 | Before 1pm |
| Voltage | 9–10 | 15–20°C | 15 min | 80 g/L | Med-Dark | Coarse | 1 | Before 12pm |
| The Blank | Manual | User set | User set | User set | User set | User set | User set | — |

**Profile → Score mapping:**
```swift
func profileKey(for score: Int) -> String {
    switch score {
    case 1...4:  return "tide_pool"
    case 5...6:  return "first_light"
    case 7...8:  return "deep_work"
    case 9...10: return "voltage"
    default:     return "first_light"
    }
}
// "the_blank" is manual-only — accessed via profile picker
```

**Profile taglines:**
```swift
let profileTaglines: [String: String] = [
    "tide_pool":   "Light caffeine. Easy exit.",
    "first_light": "Your everyday standard.",
    "deep_work":   "Locked in. No distractions.",
    "voltage":     "Full send. Peak morning.",
    "the_blank":   "Your rules. Your brew."
]
```

---

## 5. History & Correlation — `HistoryView.swift`

**Entry:** "VIEW BREW HISTORY" from dashboard, or nav push.

```
┌─────────────────────────────────┐
│  ‹ Back            bre:zi    ⚙  │  NavBarView.swift
│─────────────────────────────────│
│                                 │  #F5F5F5 background
│  YOUR PRACTICE                  │  Mono, 11px, #888888, ALL CAPS
│                                 │  bottom padding: 16pt (md)
│  ┌─ 7-DAY RECOVERY ──────────┐ │  RecoverySparkView.swift
│  │                             │ │
│  │  ╱╲    ╱╲                   │ │  spark line: #488BED, 2px stroke
│  │ ╱  ╲╱╱  ╲╱╲                │ │  rounded line cap
│  │                             │ │  height: 48pt, full card width
│  │                             │ │  fill: gradient #488BED @ 0.15 → transparent
│  │  Avg recovery: 6.2          │ │  CondensedBold, 20px, #000000
│  │  ↑ from 5.4 last week      │ │  Mono, 13px, #34C759 (statusGreen)
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, shadow, radius: 16pt, padding: 16pt
│                                 │
│  ┌─ STREAK ───────────────────┐ │  StreakCardView.swift
│  │                             │ │
│  │  14 days                    │ │  CondensedBold, 48px, #000000
│  │  Current brew streak        │ │  Mono, 12px, #888888
│  │                             │ │
│  │  M  T  W  T  F  S  S       │ │  Mono, 12px, #888888, centered per circle
│  │  ●  ●  ●  ●  ●  ○  ○       │ │  circles: 24pt diameter
│  │                             │ │  ● filled: #488BED (past), #34C759 (today)
│  └─────────────────────────────┘ │  ○ empty: #E0E0E0 (future)
│                                 │  bg: #FFFFFF, shadow, radius: 16pt, padding: 16pt
│  ┌─ CORRELATION ──────────────┐ │  CorrelationCardView.swift
│  │                             │ │
│  │  RECOVERY vs BREW RATING   │ │  Mono, 11px, #888888, ALL CAPS
│  │                             │ │
│  │  5★ ·          ·    ●      │ │  scatter plot
│  │  4★ ·     ●  ●   ●        │ │  x: recovery score (1–10)
│  │  3★ ·  ●     ●            │ │  y: brew rating (1–5 stars)
│  │  2★ ·  ●                  │ │  dot size: 8pt, filled circle
│  │  1★ ·                     │ │  axis labels: Mono, 11px, #888888
│  │     └──┬──┬──┬──┬──┬──┬   │ │
│  │        2  3  4  5  6  7   │ │
│  │                             │ │
│  │  Higher recovery = better   │ │  Regular, 13px, #888888
│  │  rated brews. Pattern holds.│ │
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, shadow, radius: 16pt, padding: 16pt
│                                 │
│  ┌─ PROFILE MIX ─────────────┐ │  ProfileMixView.swift
│  │                             │ │
│  │  ████████████░░░░░░░░       │ │  horizontal stacked bar
│  │  First Light 42%            │ │  bar height: 16pt, radius: 8pt
│  │  Deep Work 28%              │ │  Mono, 12px, #888888
│  │  Voltage 18%                │ │  each line: profile name + percentage
│  │  Tide Pool 12%              │ │
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, shadow, radius: 16pt, padding: 16pt
│                                 │
│  ─ March 2026 ─────────────     │  Mono, 12px, #888888, section header
│                                 │  bottom padding: 8pt (sm)
│  ┌─────────────────────────────┐│  BrewLogCell.swift
│  │  FIRST LIGHT       Mar 19  ││  CondensedBold, 16px, #000000 | Mono, 12px, #888888
│  │  4–8°C · 12min · 60g/L    ││  Mono, 13px, #666666
│  │  Recovery: 5  · Med roast  ││  Mono, 12px, #888888
│  │  ★ ★ ★ ☆ ☆                 ││  RatingStarsView.swift, 16pt stars
│  └─────────────────────────────┘│  bg: #FFFFFF, radius: 12pt, padding: 16pt
│                                 │  gap: 8pt (sm) between cells
│  ┌─────────────────────────────┐│
│  │  VOLTAGE           Mar 18  ││
│  │  15–20°C · 15min · 80g/L  ││
│  │  Recovery: 9  · Dark roast ││
│  │  ★ ★ ★ ★ ★                 ││
│  └─────────────────────────────┘│
│                                 │
└─────────────────────────────────┘
```

### RecoverySparkView.swift

**Spec:**
- Simple line chart, 7 data points (one per day, last 7 days)
- Stroke: `#488BED`, 2px, rounded line cap (`CGLineCap.round`)
- Fill: linear gradient from `#488BED` at 0.15 opacity → transparent, below the line
- Chart height: 48pt
- Chart width: full card width minus 32pt padding
- No x-axis labels, no y-axis labels, no grid lines — just the line
- Missing days: interpolated (dotted segment, dash pattern: [4, 4])
- Y range: auto-scales to min/max of available data points, with 10% padding

**Below spark line:**
- `"Avg recovery: X.X"` — CondensedBold, 20px, #000000
- Trend line: `"↑ from X.X last week"` or `"↓ from X.X last week"` or `"= X.X last week"`
  - ↑: Mono, 13px, #34C759 (statusGreen)
  - ↓: Mono, 13px, #FF453A (statusRed)
  - = (within 5%): Mono, 13px, #888888

**Appears:** After 3+ days of check-ins. Before that, hide card entirely (no empty state placeholder).

### StreakCardView.swift

- Streak count: CondensedBold, 48px, #000000
- Subtitle: `"Current brew streak"` — Mono, 12px, #888888
- Week view: 7 circles, 24pt diameter, 8pt gap
  - Brewed today: `#34C759` (statusGreen)
  - Brewed past: `#488BED`
  - Not brewed / future: `#E0E0E0`
- Day labels: Mono, 12px, #888888, centered above each circle

### CorrelationCardView.swift

**Dot color mapping by profile:**
```swift
let profileDotColors: [String: Color] = [
    "tide_pool":   Color(hex: "#34C759"),  // green
    "first_light": Color(hex: "#488BED"),  // blue
    "deep_work":   Color(hex: "#FFD60A"),  // yellow
    "voltage":     Color(hex: "#FF453A"),  // red
    "the_blank":   Color(hex: "#888888")   // grey
]
```

**Insight line below chart (generated from correlation direction):**
```swift
let correlation_positive = "Higher recovery = better rated brews. Pattern holds."
let correlation_weak = "No clear pattern yet. More data will sharpen this."
let correlation_negative = "Interesting — you're rating lower-recovery brews higher. You might prefer gentler profiles."
```

**Appears:** After 5+ logged brews with ratings. Before that:
```swift
let correlation_locked = "Log 5 rated brews to unlock your correlation chart."
// Mono, 13px, #888888, centered in card
```

### ProfileMixView.swift

- Horizontal stacked bar, height 16pt, radius 8pt
- Each segment colored per `profileDotColors`
- Below bar: one line per profile — name + percentage — Mono, 12px, #888888
- Appears after 3+ brews

### BrewLogCell.swift

- Background: #FFFFFF, radius 12pt, padding 16pt
- Row 1: profile name (CondensedBold, 16px, #000000) left, date (Mono, 12px, #888888) right
- Row 2: params (Mono, 13px, #666666)
- Row 3: recovery + roast (Mono, 12px, #888888)
- Row 4: RatingStarsView, 16pt stars. If unrated: `"Not rated"` — Mono, 12px, #888888
- Grouped by month: section header = `"March 2026"` — Mono, 12px, #888888

### History — Empty State

```swift
let history_empty_line1 = "Nothing brewed yet."
let history_empty_line2 = "Your data's ready."
let history_empty_line3 = "Your machine's ready."
let history_empty_line4 = "You're the last variable."
let history_empty_cta = "See today's recommendation"
```
- Text: Regular, 16px, #333333, centered
- CTA: text link, Regular, 14px, #488BED, 44×44pt tap area. Tap → pop to dashboard.

---

## 6. Device Hub — `DeviceHubView.swift`

**Entry:** Nav bar device icon (antenna symbol).

```
┌─────────────────────────────────┐
│  ‹ Back            bre:zi    ⚙  │  NavBarView.swift
│─────────────────────────────────│
│                                 │  #F5F5F5 background
│  YOUR GEAR                      │  Mono, 11px, #888888, ALL CAPS
│                                 │  bottom padding: 16pt (md)
│  ┌─ BREZI MACHINE ────────────┐ │  DeviceCardView.swift
│  │                             │ │
│  │  ┌──┐  BREZI-A4F2          │ │  icon: SF Symbol cup.and.saucer.fill, 40pt
│  │  │☕│  Connected            │ │  Mono, 12px, #34C759
│  │  └──┘  Firmware 1.2.0      │ │  Mono, 11px, #888888
│  │                             │ │
│  │  Last brew: Today, 7:14am  │ │  Mono, 12px, #888888
│  │  Brews logged: 47          │ │  Mono, 12px, #888888
│  │                             │ │
│  │  Disconnect                │ │  Regular, 14px, #FF453A, 44×44pt
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, shadow, radius: 16pt, padding: 20pt
│                                 │
│  ┌─ WEARABLE ─────────────────┐ │  DeviceCardView.swift
│  │                             │ │
│  │  ┌──┐  WHOOP 4.0           │ │  icon: SF Symbol applewatch, 40pt
│  │  │⌚│  Connected            │ │  Mono, 12px, #34C759
│  │  └──┘  Last sync: 6:42am   │ │  Mono, 11px, #888888
│  │                             │ │
│  │  Pulling: HRV, Recovery,   │ │  Mono, 12px, #888888
│  │  Sleep duration             │ │
│  │                             │ │
│  │  Disconnect                │ │  Regular, 14px, #FF453A, 44×44pt
│  │                             │ │
│  └─────────────────────────────┘ │  bg: #FFFFFF, shadow, radius: 16pt, padding: 20pt
│                                 │
│  ─ ADD DEVICE ──────────────    │  Mono, 11px, #888888, ALL CAPS
│                                 │
│  ┌─────────────────────────────┐│  bg: #FFFFFF, radius: 12pt, padding: 16pt
│  │  + Pair another Brezi       ││  Regular, 16px, #000000
│  └─────────────────────────────┘│  44pt min height, 44×full tap area
│                                 │  gap: 8pt (sm)
│  ┌─────────────────────────────┐│
│  │  + Connect wearable         ││  Regular, 16px, #000000
│  └─────────────────────────────┘│  44pt min height
│                                 │
│  Add a Wearable ›               │  Regular, 14px, #488BED
│  WHOOP · Oura · Apple Health    │  Mono, 11px, #888888
│                                 │  44×44pt tap area
│  ─ COMING SOON ─────────────    │  Mono, 11px, #488BED, ALL CAPS
│                                 │
│  ┌─────────────────────────────┐│  bg: #FFFFFF, dashed border 1px #E0E0E0
│  │  ┌──┐  Brezi Blender       ││  radius: 12pt, padding: 16pt
│  │  │🫙│  Health-responsive    ││  icon: SF Symbol blender.fill (or cup), 40pt
│  │  └──┘  smoothies.          ││  Regular, 14px, #CCCCCC
│  │        Notify me ›         ││  Regular, 14px, #488BED, 44×44pt
│  └─────────────────────────────┘│
│                                 │
└─────────────────────────────────┘
```

**DeviceCardView.swift spec:**
- Background: #FFFFFF, radius 16pt, shadow, padding 20pt
- Icon: 40pt, left-aligned (SF Symbols for prototype)
- Device name: CondensedBold, 18px, #000000
- Status: Mono, 12px — connected `#34C759`, disconnected `#FF453A`, syncing `#488BED`
- Metadata: Mono, 11px, #888888
- "Disconnect": Regular, 14px, #FF453A, 44×44pt tap area

**"Notify me ›" behavior:** Sets `interestedInBlender = true` on user profile. No email capture, no push permission. A boolean synced when backend exists. After tap, text changes to `"We'll let you know."` — Regular, 14px, #888888.

```swift
let blender_notify = "Notify me"
let blender_notified = "We'll let you know."
```

**Device Hub — Empty States:**

| State | Copy |
|---|---|
| No Brezi paired | `"No Brezi paired. Your recommendations still work — you'll just log manually."` + `"Pair now ›"` (#488BED, 44×44pt) |
| No wearable | `"Flying manual right now. That's fine. Connect Oura or WHOOP when you're ready."` |

```swift
let deviceHub_noBrezi = "No Brezi paired. Your recommendations still work — you'll just log manually."
let deviceHub_noBrezi_cta = "Pair now"
let deviceHub_noWearable = "Flying manual right now. That's fine. Connect Oura or WHOOP when you're ready."
```

**Extensibility note for Jensen:**
```swift
enum RecommendationType {
    case brew(BrewProfile)
    case blend(BlendProfile)    // future — comment only, no stub entity
}
// BlendProfile: future entity for Brezi Blender. See RecommendationType.blend. Structure TBD.
```

---

## 7. Settings — `SettingsView.swift`

**Entry:** Nav bar gear icon.

```
┌─────────────────────────────────┐
│  ‹ Back            bre:zi    ⚙  │  NavBarView.swift
│─────────────────────────────────│
│                                 │  #F5F5F5 background
│  SETTINGS                       │  Mono, 11px, #888888, ALL CAPS
│                                 │
│  ─ Your Gear ────────────────   │  Mono, 12px, #888888, section header
│  ┌─────────────────────────────┐│
│  │  Brezi Machine    Connected ›││  Regular, 16px, #000000
│  │─────────────────────────────││  divider: 1px #F5F5F5
│  │  Wearable         WHOOP    ›││  Regular, 16px, #000000
│  └─────────────────────────────┘│  bg: #FFFFFF, radius: 12pt
│                                 │  value: Mono, 14px, #888888, right-aligned
│  ─ How You Brew ─────────────   │  chevron: SF Symbol chevron.right, 12pt, #E0E0E0
│  ┌─────────────────────────────┐│
│  │  Default roast     Medium  ›││  row height: 48pt (min 44pt tap)
│  │─────────────────────────────││  padding: 16pt horizontal
│  │  Default grind     Medium  ›││
│  │─────────────────────────────││
│  │  Caffeine cutoff   2:00 PM ›││
│  │─────────────────────────────││
│  │  Serving size      1       ›││
│  └─────────────────────────────┘│  bg: #FFFFFF, radius: 12pt
│                                 │
│  ─ Numbers ──────────────────   │
│  ┌─────────────────────────────┐│
│  │  Temperature       Celsius ›││
│  │─────────────────────────────││
│  │  Weight            Grams   ›││
│  └─────────────────────────────┘│  bg: #FFFFFF, radius: 12pt
│                                 │
│  ─ Notifications ────────────   │
│  ┌─────────────────────────────┐│
│  │  Morning check-in  [====]   ││  Toggle: system UISwitch style
│  │─────────────────────────────││  ON tint: #488BED
│  │  Check-in time     7:00 AM ›││
│  │─────────────────────────────││
│  │  Brew reminders    [====]   ││
│  └─────────────────────────────┘│  bg: #FFFFFF, radius: 12pt
│                                 │
│  ─ Data Sources ─────────────   │
│  ┌─────────────────────────────┐│
│  │  Wearable: None connected   ││  Regular, 16px, #000000
│  │  Connect ›                  ││  Regular, 14px, #488BED, 44×44pt
│  └─────────────────────────────┘│  bg: #FFFFFF, radius: 12pt
│                                 │  (only if wearableSource == "manual")
│  ─ Data ─────────────────────   │
│  ┌─────────────────────────────┐│
│  │  Export brew history       ›││
│  │─────────────────────────────││
│  │  Reset all data            ›││  Regular, 16px, #FF453A
│  └─────────────────────────────┘│  bg: #FFFFFF, radius: 12pt
│                                 │
│  ─ About ────────────────────   │
│  ┌─────────────────────────────┐│
│  │  Version            0.2.0   ││  Mono, 14px, #888888
│  │─────────────────────────────││
│  │  Built by Brezi             ││  Mono, 14px, #888888
│  └─────────────────────────────┘│  bg: #FFFFFF, radius: 12pt
│                                 │
└─────────────────────────────────┘
```

**Settings details:**
- **Caffeine cutoff:** Profiles recommended after this time shift one tier gentler per `shiftedProfile()`. Default: 2:00 PM.
- **Export:** Generates CSV of all BrewLog entries — date, profile, settings, recovery score, rating. Opens system share sheet.
- **Reset all data:** Destructive action. Confirmation alert:
  ```swift
  let reset_title = "Reset All Data"
  let reset_message = "This removes all your brew history, recovery data, and device connections. This can't be undone."
  let reset_cancel = "Cancel"
  let reset_confirm = "Reset Everything"  // #FF453A
  ```
  "Cancel" is default (bold). "Reset Everything" is destructive style.
- **Data Sources section:** Only visible when `wearableSource == "manual"`. This is where manual users discover wearable connection without being nudged on the daily screen.

```swift
let settings_dataSource_none = "Wearable: None connected"
let settings_dataSource_connect = "Connect"
```

---

## 8. Notification Tiers

Request `UNUserNotificationCenter` authorization after first BrewLog (not during onboarding). If denied, no persistent nag. Settings shows toggle as OFF.

### Tier 1: Day 1–7 (Everything is new)
**Trigger:** Morning, at user's configured check-in time.
**Copy (micro-variation):**
```swift
let notification_day1 = "Your first signal is ready."
let notification_day2 = "Day 2. Your signal's ready."
let notification_day3plus = "Your signal's ready. Time to check in."
```

**Logic:**
```swift
func tier1Notification() -> String {
    let dayCount = UserDefaults.standard.integer(forKey: "notificationDayCount")
    switch dayCount {
    case 1:  return notification_day1
    case 2:  return notification_day2
    default: return notification_day3plus
    }
}
```
Increment `notificationDayCount` in UserDefaults daily.

### Tier 2: Day 8–14 (Data starts to matter)
**Trigger:** Morning, at check-in time.
**Copy (recovery-trend-aware):**
```swift
func tier2Notification(trendPercent: Int, direction: String) -> String {
    switch direction {
    case "up":   return "Your recovery is up \(trendPercent)% this week. See what Brezi recommends."
    case "down": return "Recovery dipped overnight. Brezi's recalibrated — check in."
    default:     return "Steady recovery all week. Your signal's consistent."
    }
}
```

### Tier 3: Day 15+ (Patterns emerge)
**Trigger:** Morning, at check-in time.
**Three notification types, rotating:**

**Streak:**
```swift
func notification_streak(_ days: Int) -> String {
    "Day \(days). Keep it going."
}
// Example: "Day 14. Keep it going." / "Day 21. Three weeks of practice."
```

**Pattern:**
```swift
let notification_pattern_good = "You brew best on high-recovery mornings. Today's looking good."
func notification_pattern_profile(_ profile: String, _ stars: Int) -> String {
    "\(profile) got \(stars) stars last time you were at this recovery level."
}
```

**Nudge** (only if no check-in by 2 hours past configured time):
```swift
let notification_nudge = "Haven't checked in today. Your data's waiting."
```

**Rotation:** Alternate between streak and pattern. Nudge only fires as fallback, never as primary morning push.

---

## 9. Offline Behavior Rules

### No Internet
- App works normally. All data is local (Core Data).
- iCloud sync via `NSPersistentCloudKitContainer` happens when connectivity returns.
- User never knows. No banner, no warning, no degraded state.

### No Bluetooth (after pairing)
- Dashboard shows recommendation normally — all data is already synced locally.
- "START BREW" button changes to: `"LOG THIS BREW"`
- Below button: `"Can't reach your Brezi. Brew manually and log it here."` — Mono, 12px, #888888
- When Bluetooth reconnects, button reverts to "START BREW" automatically. No user action needed.

```swift
let noBLE_cta = "LOG THIS BREW"
let noBLE_note = "Can't reach your Brezi. Brew manually and log it here."
```

### No Bluetooth (during pairing)
Covered in §2 Screen 2 failure states.

### Wearable Sync Cadence (design recommendation — Jensen's call)
- Sync on app open (foreground fetch)
- Background fetch every 4 hours (`BGAppRefreshTaskRequest`)
- Pull-to-refresh triggers manual sync
- Card 1 timestamp (`"Source: WHOOP · 6:42am"`) tells user when data was last pulled
- If data >8 hours stale: timestamp color changes from #888888 to #FFD60A (statusYellow)

---

## 10. Complete Empty States Table

| Screen | State | Copy | Style |
|---|---|---|---|
| Dashboard — first use, no wearable | Card 1 | Recovery slider (manual mode) | Component |
| Dashboard — first use, no wearable | Card 2 | `"Your practice starts blank. Give it three mornings."` | Regular, 15px, #000000 |
| Dashboard — first use, no wearable | Card 3 | Recommendation based on manual input | Component |
| Dashboard — not brewed today | Below greeting | `"Nothing brewed yet today. Your data's ready. Your machine's ready. You're the last variable."` | Regular, 14px, #888888 |
| Dashboard — wearable sync pending | Card 1 note | `"WHOOP connected · Waiting for first sync. Check in manually for now."` | Mono, 12px, #888888 |
| Dashboard — post-brew, unrated | Card 3 | "RATE THIS BREW" button | White filled, 48pt |
| Dashboard — post-cutoff, low recovery | Card 3 | Rest Day card + "BREW ANYWAY" outside | See §3 Rest Day |
| History — no brews | Full screen | `"Nothing brewed yet."` / `"Your data's ready."` / `"Your machine's ready."` / `"You're the last variable."` | Regular, 16px, #333333, centered |
| History — no brews | CTA | `"See today's recommendation ›"` | Regular, 14px, #488BED, 44×44pt |
| History — no correlation | Card placeholder | `"Log 5 rated brews to unlock your correlation chart."` | Mono, 13px, #888888, centered |
| History — no spark line | Card | Hidden entirely (no placeholder) | — |
| History — no profile mix | Card | Hidden entirely (< 3 brews) | — |
| Device Hub — no Brezi | Card | `"No Brezi paired. Your recommendations still work — you'll just log manually."` | Regular, 14px, #888888 |
| Device Hub — no Brezi | CTA | `"Pair now ›"` | Regular, 14px, #488BED, 44×44pt |
| Device Hub — no wearable | Card | `"Flying manual right now. That's fine. Connect Oura or WHOOP when you're ready."` | Regular, 14px, #888888 |
| Settings — no wearable | Data Sources section | `"Wearable: None connected"` + `"Connect ›"` | Regular, 16px + 14px #488BED |

---

## 11. Component Library

| Component | File | Used In |
|---|---|---|
| `HealthDataCardView` | HealthDataCardView.swift | Dashboard (all states) |
| `InsightStripView` | InsightStripView.swift | Dashboard (pre-brew + post-brew), Brew Recommendation |
| `DashboardBrewCardView` | DashboardBrewCardView.swift | Dashboard (pre-brew, post-cutoff, rest day) |
| `PostBrewCardView` | PostBrewCardView.swift | Dashboard (post-brew) |
| `BrewProgressView` | BrewProgressView.swift | Dashboard (brewing state) |
| `BrewCompletionOverlay` | BrewCompletionOverlay.swift | Full-screen overlay after brew |
| `DataChainView` | DataChainView.swift | Onboarding Screen 4, Brew Recommendation "Why This Brew" |
| `RecoverySliderView` | RecoverySliderView.swift | Dashboard (manual mode), Re-check-in half-sheet |
| `QuickSetChipRow` | QuickSetChipRow.swift | Dashboard (manual mode) |
| `ProfileCardView` | ProfileCardView.swift | Profile Picker, Brew Recommendation |
| `BrewSettingsRow` | BrewSettingsRow.swift | Brew Recommendation |
| `BeanRecommendationRow` | BeanRecommendationRow.swift | Brew Recommendation (commerce-ready hook) |
| `ManualOverrideSheet` | ManualOverrideSheet.swift | Brew Recommendation (half-sheet, bounded sliders) |
| `ProfilePickerView` | ProfilePickerView.swift | Full-screen modal from "Choose a different brew" / "BREW ANYWAY" |
| `BrewLogCell` | BrewLogCell.swift | History |
| `RatingStarsView` | RatingStarsView.swift | Completion overlay (36pt), Post-brew card (28pt), History (16pt) |
| `StreakCardView` | StreakCardView.swift | History |
| `RecoverySparkView` | RecoverySparkView.swift | History |
| `CorrelationCardView` | CorrelationCardView.swift | History |
| `ProfileMixView` | ProfileMixView.swift | History |
| `DeviceCardView` | DeviceCardView.swift | Device Hub |
| `WearableRowView` | WearableRowView.swift | Onboarding Screen 3, Device Hub |
| `OnboardingSelectionCard` | OnboardingSelectionCard.swift | Onboarding Screen 5 |
| `DevicePairFailureCard` | DevicePairFailureCard.swift | Onboarding Screen 2 (all failure states) |
| `NavBarView` | NavBarView.swift | All post-onboarding screens |
| `SegmentedProgressBar` | SegmentedProgressBar.swift | Onboarding shell |
| `SaveButton` | SaveButton.swift | All screens (primary CTA pattern) |
| `EmptyStateView` | EmptyStateView.swift | All screens |

---

## 12. Data Model (for Jensen)

### Core Data Entities

**Device**
```swift
@objc(Device)
class Device: NSManagedObject {
    @NSManaged var id: UUID
    @NSManaged var type: String              // "brezi", "brezi_blender" (future)
    @NSManaged var name: String              // "BREZI-A4F2"
    @NSManaged var firmwareVersion: String?
    @NSManaged var isPaired: Bool
    @NSManaged var lastConnectedAt: Date?
    @NSManaged var createdAt: Date
}
```

**WearableConnection**
```swift
@objc(WearableConnection)
class WearableConnection: NSManagedObject {
    @NSManaged var id: UUID
    @NSManaged var source: String            // "whoop", "oura", "apple_health", "manual"
    @NSManaged var isConnected: Bool
    @NSManaged var lastSyncAt: Date?
    @NSManaged var accessToken: String?      // encrypted, for OAuth sources
    @NSManaged var createdAt: Date
}
```

**CheckIn**
```swift
@objc(CheckIn)
class CheckIn: NSManagedObject {
    @NSManaged var id: UUID
    @NSManaged var recoveryScore: Int16      // 1–10 (manual) or mapped from wearable
    @NSManaged var hrvValue: Double           // raw HRV in ms (from wearable), 0 if manual
    @NSManaged var recoveryPercent: Double    // recovery % (from wearable), 0 if manual
    @NSManaged var sleepHours: Double         // total sleep (from wearable), 0 if manual
    @NSManaged var baselineHrv: Double        // 7-day rolling average, 0 if insufficient data
    @NSManaged var source: String             // "manual", "whoop", "oura", "apple_health"
    @NSManaged var isRecheck: Bool            // true if replaced earlier check-in today
    @NSManaged var createdAt: Date
}
```

**BrewProfile**
```swift
@objc(BrewProfile)
class BrewProfile: NSManagedObject {
    @NSManaged var id: UUID
    @NSManaged var key: String               // "tide_pool", "first_light", etc.
    @NSManaged var name: String              // "Tide Pool", "First Light", etc.
    @NSManaged var tagline: String           // "Light caffeine. Easy exit."
    @NSManaged var scoreMin: Int16
    @NSManaged var scoreMax: Int16
    @NSManaged var defaultTemp: Double
    @NSManaged var defaultTime: Int16
    @NSManaged var defaultDose: Double
    @NSManaged var recommendedRoast: String
    @NSManaged var recommendedGrind: String
    @NSManaged var defaultServings: Int16
    @NSManaged var caffeineCutoffHour: Int16
    @NSManaged var brewTimeMinutes: Int16
    // Manual override bounds:
    @NSManaged var minTemp: Double
    @NSManaged var maxTemp: Double
    @NSManaged var minTime: Int16
    @NSManaged var maxTime: Int16
    @NSManaged var minDose: Double
    @NSManaged var maxDose: Double
}
```

**BrewLog**
```swift
@objc(BrewLog)
class BrewLog: NSManagedObject {
    @NSManaged var id: UUID
    @NSManaged var profileKey: String
    @NSManaged var recoveryScore: Int16
    @NSManaged var tempCelsius: Double
    @NSManaged var brewTimeMinutes: Int16
    @NSManaged var doseGramsPerLiter: Double
    @NSManaged var roastUsed: String?
    @NSManaged var grindUsed: String?
    @NSManaged var servings: Int16
    @NSManaged var rating: Int16             // 0 = unrated, 1–5 = star rating
    @NSManaged var brewSettingsJSON: String?  // serialized override settings
    @NSManaged var deviceId: UUID?
    @NSManaged var connectionLost: Bool       // true if BLE disconnected during brew
    @NSManaged var brewedAt: Date?           // exact brew timestamp
    @NSManaged var createdAt: Date
}
```

### Insight Engine — `InsightEngine.swift`

```swift
struct HealthSignal {
    let recoveryScore: Int       // 1–10
    let hrv: Double?             // ms, nil if manual
    let baselineHrv: Double?     // 7-day avg, nil if <7 days
    let recoveryPercent: Double? // wearable recovery %, nil if manual
    let sleepHours: Double?      // nil if manual
    let source: String           // "manual", "whoop", "oura", "apple_health"
}

struct BrewInsight {
    let profileKey: String       // "tide_pool", "first_light", etc.
    let insightText: String      // pre-brew insight copy
    let reflectionText: String   // post-brew reflection copy (after brew profile known)
    let dataPoints: [String]     // human-readable data summaries
    let confidence: Double       // 0–1, higher with more data
    let variant: Int             // which copy variant was selected (0–2)
}

func generateInsight(from signal: HealthSignal) -> BrewInsight
```

### UserDefaults Keys (complete list)

```swift
// Onboarding
"hasCompletedOnboarding"     // Bool
"breziPaired"                // Bool
"wearableSource"             // String: "manual", "whoop", "oura", "apple_health"

// Insight rotation
"lastInsight_high"           // Int (0, 1, or 2)
"lastInsight_mid"            // Int
"lastInsight_low"            // Int

// Manual user suffix
"lastManualSuffixType"       // String: "trend" or "streak"

// Notifications
"notificationDayCount"       // Int, incremented daily

// Blender interest
"interestedInBlender"        // Bool

// Settings
"caffeineCutoffHour"         // Int (0–23), default 14
"temperatureUnit"            // String: "celsius" or "fahrenheit"
"weightUnit"                 // String: "grams" or "ounces"
"defaultRoast"               // String: "light", "medium", "dark"
"defaultGrind"               // String: "fine", "medium", "coarse"
"defaultServings"            // Int, default 1
"morningCheckInEnabled"      // Bool, default true
"checkInTimeHour"            // Int (0–23), default 7
"checkInTimeMinute"          // Int (0–59), default 0
"brewRemindersEnabled"       // Bool, default true
```

---

## 13. Animation Reference (Complete)

| Location | Element | Type | Duration | Easing | Delay | Trigger |
|---|---|---|---|---|---|---|
| Onboarding 1 | Headline | fadeIn + translateY(12→0) | 0.4s | easeOut | 0.1s | Screen appear |
| Onboarding 1 | Body | fadeIn | 0.3s | easeOut | 0.5s | Screen appear |
| Onboarding 1 | CTA | fadeIn | 0.2s | easeOut | 0.8s | Screen appear |
| Onboarding 2 | Device found card | fadeIn | 0.3s | easeOut | 0.0s | BLE discovery |
| Onboarding 2 | Checkmark | scale(0.5→1.0) | 0.2s | spring | 0.0s | BLE paired |
| Onboarding 3 | Progress bar | segmented | N/A | easeOut | 0.0s | Screen appear |
| Onboarding 4 | Card 1 | fadeIn + translateY(16→0) | 0.3s | easeOut | 0.0s | Screen appear |
| Onboarding 4 | Line 1 | drawFromTop | 0.2s | easeOut | 0.3s | After Card 1 |
| Onboarding 4 | Card 2 | fadeIn + translateY(16→0) | 0.3s | easeOut | 0.5s | After Line 1 |
| Onboarding 4 | Line 2 | drawFromTop | 0.2s | easeOut | 0.8s | After Card 2 |
| Onboarding 4 | Card 3 | fadeIn + translateY(16→0) | 0.3s | easeOut | 1.0s | After Line 2 |
| Onboarding 4 | CTA | fadeIn | 0.2s | easeOut | 1.3s | After Card 3 |
| Onboarding 4 | ALL (interruptible) | snap to final | immediate | — | — | CTA tap during sequence |
| Onboarding 5 | Selection card | scale(1.0→1.01) | 0.35s | spring(0.7) | 0.0s | Card tap |
| Loading | "Reading your signal..." | opacity pulse (1.0↔0.5) | 1.2s | easeInOut | repeat | Screen appear |
| Progress bar | Segment pulse | opacity (1.0↔0.6) | 1.5s | easeInOut | repeat | Current segment |
| Brewing | Machine icon | scale(1.0↔1.03) | 2.0s | easeInOut | repeat | Brewing state |
| Brew cancel | Confirmation | slideUp | 0.2s | easeOut | 0.0s | Cancel tap |
| Brew cancel | Return to pre-brew | crossfade | 0.3s | easeOut | 0.0s | Cancel confirmed |
| Completion overlay | "Brewed." | fadeIn + translateY(12→0) | 0.4s | easeOut | 0.0s | Brew complete |
| Completion overlay | Meta | fadeIn | 0.3s | easeOut | 0.3s | After headline |
| Completion overlay | Stars + prompt | fadeIn | 0.3s | easeOut | 0.5s | After meta |
| Completion overlay | Buttons | fadeIn | 0.2s | easeOut | 0.7s | After stars |
| Completion overlay | Dismiss | none — reveals dashboard underneath | — | — | — | DONE / Skip tap |
| Dashboard | Re-check-in update | spring (Cards 2+3) | 0.4s | spring(0.8) | 0.0s | Re-check-in saved |
| Dashboard | Inline rating save | spring | 0.3s | spring(0.8) | 0.0s | Star tapped |
| Brew logged (no device) | Checkmark + "Logged." | fadeIn | 0.3s | easeOut | 0.0s | Brew logged |
| Brew logged (no device) | Auto-dismiss | fadeOut | 0.3s | easeOut | 1.2s | After appear |
| Firmware OTA | Progress bar | linear fill | real-time | linear | — | OTA running |
| Firmware OTA | Transition to paired | crossfade | 0.5s | easeOut | 0.0s | OTA complete |
| Toast (skip pairing) | Appear | fadeIn + slideUp | 0.3s | easeOut | 0.0s | Skip tap |
| Toast (skip pairing) | Dismiss | fadeOut | 0.3s | easeOut | 2.5s | Auto |

---

## 14. Build Order (recommended for Jensen)

1. **Day 1–2:** Core Data — Device, WearableConnection, updated CheckIn/BrewProfile/BrewLog with all fields (override bounds, `isRecheck`, `brewedAt`, `connectionLost`). Seed 5 profile records. Build `InsightEngine` with rule-based logic + copy variants + rotation.

2. **Day 3–4:** Dashboard — all three temporal states + post-cutoff + rest day. `HealthDataCardView` (wearable + manual variants) + `InsightStripView` (pre-brew insight + post-brew reflection + manual suffix) + `DashboardBrewCardView` + `PostBrewCardView` + `BrewProgressView`. Wire manual recovery input (`RecoverySliderView` + `QuickSetChipRow`). Wire re-check-in action. Wire post-cutoff behavior. Wire greeting logic.

3. **Day 5–6:** `BrewCompletionOverlay` (primary rating moment). `BrewRecommendationView` — brew card, "Why This Brew" chain with past rating acknowledgment, `ManualOverrideSheet` with bounded sliders + amber warnings, `ProfilePickerView`. `BeanRecommendationRow` (commerce hook). Brew cancel flow + BLE disconnect handling.

4. **Day 7–8:** `HistoryView` — `RecoverySparkView` (7-day spark line), `StreakCardView`, `BrewLogCell` list grouped by month, `CorrelationCardView` (colored dots + insight line + unlock gating), `ProfileMixView`.

5. **Day 9–10:** Onboarding flow — 5 screens with all copy. `BrandIntroView` with staggered animations. `DevicePairView` with all 6 states (idle, searching, found, bluetoothOff, notFound, alreadyPaired) + firmware update + firmware failure. `WearableConnectView` with OAuth states (cancelled, failed, data-pending) + manual skip. `DataChainTutorialView` with interruptible animation. `FirstCheckInView`.

6. **Day 11:** `DeviceHubView` — device cards, add/disconnect flows, Blender placeholder with `interestedInBlender` boolean. `NavBarView`.

7. **Day 12–13:** `SettingsView` with all sections including Data Sources for manual users. Notification scheduling (3 tiers + day count tracking). Data export (CSV). Reset all data. Offline behavior rules. Polish all transitions and loading states.

8. **Day 14:** End-to-end testing, TestFlight prep, bug sweep.

**TestFlight target: 14 working days from start.**

---

## 15. Decisions — Locked (March 19, 2026)

1. **No tab bar.** Single push-stack with modal sheets.
2. **Dark brew card on light dashboard.** The brew is the hero.
3. **Recovery score 1–10, not percentage.** Simple mental model.
4. **Manual input is a first-class path.** No wearable required. No daily nudge.
5. **5 brew profiles mapped to recovery score.** The Blank is manual-only.
6. **"Brewed." over "Done."** Specificity wins over brevity.
7. **No community signal on dashboard.** Personal chain is the product. History screen at scale.
8. **Post-brew dashboard is a first-class design.** The daily loop doesn't end at "Start Brew."
9. **Three notification tiers.** Day 1–7 static (with micro-variation), Day 8–14 data-aware, Day 15+ pattern-based. Day 90 strategy comes from TestFlight data.
10. **7-day spark line, not 30-day trend chart.** Trajectory without a charting library.
11. **Manual override bounded per profile.** The Blank gets full range with amber warnings, not hard blocks.
12. **Single rating moment.** Completion overlay primary, dashboard Card 3 fallback. Never both.
13. **"BREW ANYWAY" outside the Rest Day card.** Deliberate override, not hidden escape.
14. **Interruptible animations.** Never trap an impatient user.
15. **Dashboard single-chain layout for v1.** Blender will require structural dashboard redesign — horizontal selector or parallel cards — not a bolt-on.

---

## 16. Not in This Spec (Explicitly Deferred)

| Item | Reason | When |
|---|---|---|
| BlendProfile Core Data entity | Comment only. Empty entities create migration obligations. | When Blender ships |
| 7–10 insight variants per tier | Content debt. 3 per tier is enough for launch. | First month post-launch |
| 30-day trend charts | Spark line gives trajectory. Full charts need a charting library. | v2.x |
| Bean commerce flow | Architecture-ready (`BeanRecommendationRow.onTap`), not designed. | When commerce layer exists |
| Community/social signal | Parked. Personal chain is the product. | v2+ |
| Day 90 notification strategy | Need real TestFlight retention data first. | Post-TestFlight |
| Preference-weighted brew engine | Current engine is rule-based. Rating data accumulates for v2.x. | v2.x |

---

*The chain is the product. Data → insight → brew → rate → repeat. The dashboard breathes through the whole day. The voice sounds like a person. The first 5 minutes of ownership have real failure states. Manual users are seen. Every string is paste-ready. Every component is named. Every edge case is handled.*

*v3.0 spec locked. Jensen builds.*

— Picasso
