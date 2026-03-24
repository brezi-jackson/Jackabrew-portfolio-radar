# Brezi iOS App — Wireframe & Design Spec v1 (Iteration 1)
**Status:** Design review draft | March 2026 | For Jackson + Jensen
**Supersedes:** wireframe-spec.md v1.0
**Core changes:** Visible correlation chain, device-first onboarding, expanded brew recommendation, history/correlation view, extensible device hub

---

## 0. Design Tokens

### Colors
```swift
// Brand palette — unchanged from v1.0
let black         = Color(hex: "#000000")
let white         = Color(hex: "#FFFFFF")
let offWhite      = Color(hex: "#F5F5F5")  // app background
let blue          = Color(hex: "#488BED")  // accent, CTAs, active states
let midGrey       = Color(hex: "#888888")  // secondary text, labels
let lightGrey     = Color(hex: "#E0E0E0")  // dividers, borders
let darkCard      = Color(hex: "#0F0F0F")  // brew card background
let darkSurface   = Color(hex: "#1A1A1A")  // secondary dark surfaces

// NEW — correlation & status colors
let greenPositive = Color(hex: "#34C759")  // HRV up, positive trends
let amberWarning  = Color(hex: "#FFB833")  // moderate strain, caution
let redAlert      = Color(hex: "#E84545")  // high strain, recovery needed
let subtleBlue    = Color(hex: "rgba(72,139,237,0.08)")  // tinted card bg
let connectorLine = Color(hex: "#488BED").opacity(0.40)  // correlation connector
```

### Typography
System fonts for prototype. ABC Social swapped pre-launch.
```swift
let t_display   = Font.system(size: 48, weight: .heavy)      // hero headlines
let t_h1        = Font.system(size: 34, weight: .bold)        // screen titles
let t_h2        = Font.system(size: 26, weight: .bold)        // card titles
let t_h3        = Font.system(size: 20, weight: .semibold)    // section headers
let t_body      = Font.system(size: 16, weight: .regular)     // body text
let t_small     = Font.system(size: 14, weight: .regular)     // secondary text
let t_mono      = Font.system(size: 13, weight: .regular, design: .monospaced)
let t_eyebrow   = Font.system(size: 11, weight: .medium, design: .monospaced)
let t_data      = Font.system(size: 15, weight: .semibold, design: .monospaced) // NEW — live metric values
let t_metric_lg = Font.system(size: 28, weight: .bold, design: .monospaced)    // NEW — hero metric numbers
```

### Spacing (8pt grid)
```
xs: 4    sm: 8    md: 16    lg: 24    xl: 32    xxl: 48    xxxl: 64
```

### Corner Radii
```
card: 16px    button: 12px    chip: 8px    input: 10px    metric-pill: 20px
```

### Shadows
```swift
.shadow(color: .black.opacity(0.08), radius: 16, x: 0, y: 4)   // light cards
// No shadow on dark cards — they float via contrast
```

### Motion Tokens
```swift
let m_enter    = Animation.easeOut(duration: 0.35)      // elements entering screen
let m_micro    = Animation.easeInOut(duration: 0.2)     // micro-interactions (toggles, chips)
let m_spring   = Animation.spring(response: 0.45, dampingFraction: 0.75)  // bouncy reveals
let m_stagger  = 0.08  // seconds between staggered children
let m_pulse    = Animation.easeInOut(duration: 1.2).repeatForever(autoreverses: true) // loading pulse
```

---

## 1. App Shell

### Navigation Architecture (revised)
```
AppRoot
 └── OnboardingFlow (5 screens, shown once on first launch)
      └── MainFlow
           ├── HomeDashboardView           ← default landing (NEW — replaces MorningCheckInView)
           ├── BrewRecommendationView      ← pushed from dashboard
           ├── HistoryCorrelationView      ← pushed from dashboard (NEW)
           ├── DeviceHubView              ← pushed from dashboard (NEW)
           └── SettingsView               ← pushed from nav
```

### Navigation Bar (persistent, post-onboarding)
```
┌─────────────────────────────────────────┐
│  [≡]          B R E Z I          [⚙]   │
│  24pt          18px/white         20pt  │
│             #000 background             │
└─────────────────────────────────────────┘
```
- Logo: "BREZI" — `SF Pro Display` condensed bold / 18px / letter-spacing 3px / white / centered
- Left: hamburger `line.3.horizontal` SF Symbol — taps to DeviceHubView. 24pt, white
- Right: `gearshape` SF Symbol — taps to SettingsView. 20pt, white
- Back arrow replaces hamburger when pushed deeper
- Height: 56pt (includes status bar inset)
- No bottom tab bar

---

## 2. Onboarding Flow (5 screens — revised order)

**Trigger:** First launch only. Store `hasCompletedOnboarding: Bool` in UserDefaults.

### Shared Onboarding Shell
- Background: `#000000` full bleed
- Progress dots: 5 dots, bottom center, 32px from bottom safe area
  - Diameter: 8px
  - Active: `#488BED` with glow (`shadow(color: blue.opacity(0.5), radius: 4)`)
  - Inactive: `#333333`
  - Spacing: 12px between dots
- All text: white unless specified
- Gesture: horizontal swipe enabled (system `TabView` with `.tabViewStyle(.page)`)
- Skip: top-right, `#888888`, 14px, "Skip" — jumps to Screen 5 (ready state)

---

### Onboarding Screen 1 — Brand Hero
**Purpose:** Set the tone. Cold, dark, sharp. No UI chrome.

```
┌─────────────────────────────────────────┐
│                                    Skip │  14px, #888888, right-aligned, 16px inset
│                                         │
│                                         │
│                                         │
│                                         │
│                                         │
│                                         │
│        Cold is                          │
│        a practice.                      │  SF Pro Display Heavy, 52px, white
│                                         │  line-height: 56px
│                                         │  left-aligned, 32px horizontal padding
│                                         │
│        Most people compromise           │
│        their coffee. Ice dilutes        │
│        it. Hot brew oxidizes it.        │  SF Pro Text Regular, 17px, #CCCCCC
│        You've been drinking the         │  line-height: 26px
│        workaround this whole time.      │  32px horizontal padding
│                                         │
│                                         │
│                                         │
│  ┌───────────────────────────────────┐  │
│  │       Start the practice →        │  │  #488BED bg, white text, 16px semibold
│  └───────────────────────────────────┘  │  height: 54px, radius: 12px
│                                         │  margin: 0 24px 0 24px
│           ●  ○  ○  ○  ○                │  progress dots
│                                         │
└─────────────────────────────────────────┘
```

**Motion:**
1. Background: solid `#000000`, no animation
2. Headline: `opacity(0) → opacity(1)`, `offset(y: 16) → offset(y: 0)`, `m_enter`, delay `0.1s`
3. Body: same animation, delay `0.5s`
4. CTA button: `opacity(0) → opacity(1)`, delay `0.9s`, `m_enter`

---

### Onboarding Screen 2 — Device Pairing (NEW)
**Purpose:** Pair the Brezi cold brew machine via Bluetooth. This is the hardware handshake.

```
┌─────────────────────────────────────────┐
│                                    Skip │
│                                         │
│  Pair your Brezi.                       │  Heavy, 44px, white
│                                         │  32px left padding
│                                         │
│  ┌───────────────────────────────────┐  │
│  │                                   │  │
│  │                                   │  │
│  │     ╭─────────────────────╮       │  │
│  │     │   ◉                 │       │  │  Brezi machine illustration
│  │     │   B R E Z I         │       │  │  (placeholder: SF Symbol `cube.fill`
│  │     │                     │       │  │   64pt, white, centered)
│  │     ╰─────────────────────╯       │  │
│  │                                   │  │
│  │         ┈ ┈ ┈ ┈ ┈ ┈              │  │  animated dashed arc
│  │                                   │  │  (Bluetooth scan indicator)
│  │     ╭──────────╮                  │  │
│  │     │ 📱 iPhone │                  │  │  SF Symbol `iphone`, 32pt
│  │     ╰──────────╯                  │  │
│  │                                   │  │
│  └───────────────────────────────────┘  │  #1A1A1A bg, radius 24px
│                                         │  padding: 32px, margin: 0 24px
│                                         │
│  Searching for Brezi nearby...          │  ← STATE A: scanning
│  ◌ animated spinner                     │  Mono 14px, #888888
│                                         │  spinner: 20px, #488BED, rotating
│                                         │
│  ─── OR ───                             │
│                                         │
│  Brezi Cold Brew Machine                │  ← STATE B: found
│  Serial: BRZ-00412                      │  Mono 13px, #888888
│  ┌───────────────────────────────────┐  │
│  │           Pair device             │  │  blue button, 54px
│  └───────────────────────────────────┘  │
│                                         │
│  ─── OR ───                             │
│                                         │
│  ✓ Paired                               │  ← STATE C: connected
│  Brezi Cold Brew Machine                │  greenPositive, Mono 14px
│  ┌───────────────────────────────────┐  │
│  │           Continue →              │  │  blue button, 54px
│  └───────────────────────────────────┘  │
│                                         │
│  I don't have a Brezi yet ›             │  text link, #888888, 14px, centered
│                                         │  → skips to Screen 3, sets
│                                         │    `devicePaired: false`
│                                         │
│           ○  ●  ○  ○  ○                │
└─────────────────────────────────────────┘
```

**Bluetooth scan animation spec:**
- Dashed arc between device icons: 3 arcs radiating outward from Brezi machine icon
- Each arc: `#488BED`, stroke 1.5px, dashed (4px dash, 4px gap)
- Animation: arcs scale from `scale(0.6)` to `scale(1.2)` with `opacity(1.0) → opacity(0.0)`, 1.5s loop, staggered 0.5s apart
- On pair success: arcs collapse into solid line connecting both devices, `#34C759`, 0.3s

**State machine:**
```
SCANNING → device found → FOUND → user taps "Pair" → PAIRING (2s timeout) → PAIRED
                                                     → FAILED → retry button
```

**Error state:**
```
Couldn't find your Brezi.                  Mono 14px, #E84545
Make sure it's plugged in and nearby.      Mono 13px, #888888
[Try again]                                outlined button, #E84545 border
```

**Tech notes for Jensen:**
- Use `CBCentralManager` for BLE scanning
- Filter for Brezi service UUID (to be defined by hardware team)
- Store paired device identifier in UserDefaults: `pairedDeviceUUID: String?`
- If no Brezi paired, app still functions — brew recommendations still generated, just no machine control

---

### Onboarding Screen 3 — Wearable Connection (NEW)
**Purpose:** Connect health data source. WHOOP / Oura Ring / Apple Health / Skip.

```
┌─────────────────────────────────────────┐
│                                    Skip │
│                                         │
│  Your coffee should                     │
│  know how you slept.                    │  Heavy, 40px, white
│                                         │  32px left padding
│  Connect a wearable so Brezi            │
│  can read your recovery data            │
│  automatically.                         │  Regular, 16px, #CCCCCC
│                                         │  32px horizontal padding
│                                         │
│  ┌───────────────────────────────────┐  │
│  │  ┌──────┐                         │  │
│  │  │WHOOP │  WHOOP                  │  │  logo placeholder: 40x40 rounded rect
│  │  │ logo │  Strain, HRV, Sleep     │  │  name: semibold 17px, white
│  │  └──────┘  OAuth connection       │  │  subtitle: regular 13px, #888888
│  │                            [ → ]  │  │  chevron: 16pt, #488BED
│  └───────────────────────────────────┘  │  #1A1A1A bg, radius 16px, padding 20px
│                                         │  margin: 0 24px, 8px gap between cards
│  ┌───────────────────────────────────┐  │
│  │  ┌──────┐                         │  │
│  │  │ Oura │  Oura Ring              │  │
│  │  │ logo │  HRV, Sleep, Readiness  │  │
│  │  └──────┘  OAuth connection       │  │
│  │                            [ → ]  │  │
│  └───────────────────────────────────┘  │
│                                         │
│  ┌───────────────────────────────────┐  │
│  │  ┌──────┐                         │  │
│  │  │  ♥︎   │  Apple Health           │  │  SF Symbol: `heart.fill`, #FF375F
│  │  │      │  HRV, Sleep Analysis    │  │
│  │  └──────┘  HealthKit permission   │  │
│  │                            [ → ]  │  │
│  └───────────────────────────────────┘  │
│                                         │
│                                         │
│  I'll log manually for now ›            │  text link, #888888, 14px, centered
│                                         │  sets `wearableSource: "manual"`
│                                         │
│           ○  ○  ●  ○  ○                │
└─────────────────────────────────────────┘
```

**Wearable card spec (`WearableOptionCard`):**
- Default: `#1A1A1A` bg, no border, radius 16px, padding 20px
- Hover/pressed: `scale(0.98)`, 0.1s
- Logo area: 40x40px rounded rect, `#333` bg placeholder
- Name: `SF Pro Display` semibold, 17px, white
- Subtitle: `SF Pro Text` regular, 13px, `#888888`
- Chevron: `chevron.right` SF Symbol, 16pt, `#488BED`, right-aligned, vertically centered
- Tap: initiates OAuth flow (WHOOP/Oura) or HealthKit permission (Apple Health)

**Connection success state (inline, replaces chevron):**
```
✓ Connected                                #34C759, Mono 13px
```
Card border becomes `1px #34C759`. Background becomes `rgba(52,199,89,0.06)`.

**After any wearable connected or "log manually" tapped → auto-advance to Screen 4.**

**Tech notes:**
- WHOOP: OAuth 2.0, scopes: `read:recovery`, `read:sleep`, `read:strain`
- Oura: OAuth 2.0, scopes: `daily_readiness`, `daily_sleep`, `heartrate`
- Apple Health: `HKHealthStore.requestAuthorization`, types: `HKQuantityType(.heartRateVariabilitySDNN)`, `HKCategoryType(.sleepAnalysis)`
- Store `wearableSource: String` in UserDefaults: `"whoop"` | `"oura"` | `"apple_health"` | `"manual"`

---

### Onboarding Screen 4 — Tutorial Animation (NEW)
**Purpose:** Show the correlation chain explicitly. Three-step animation: Your Data → Your Insight → Your Brew.

```
┌─────────────────────────────────────────┐
│                                         │
│  How Brezi reads you.                   │  Heavy, 38px, white
│                                         │  32px left padding
│                                         │
│  ┌───────────────────────────────────┐  │
│  │                                   │  │  #1A1A1A bg, radius 24px
│  │         STEP 1 of 3               │  │  Mono 11px, #488BED
│  │                                   │  │
│  │  ┌─────────┐                      │  │
│  │  │ HRV     │                      │  │
│  │  │ 68 ms   │  ← animated counter  │  │  metric pill (see spec)
│  │  └─────────┘                      │  │
│  │  ┌─────────┐                      │  │
│  │  │ STRAIN  │                      │  │
│  │  │ 12.3    │                      │  │
│  │  └─────────┘                      │  │
│  │  ┌─────────┐                      │  │
│  │  │ SLEEP   │                      │  │
│  │  │ 6h 40m  │                      │  │
│  │  └─────────┘                      │  │
│  │                                   │  │
│  │  YOUR DATA                        │  │  Eyebrow, 11px, #888888
│  │  Raw numbers from your wearable.  │  │  Regular, 14px, #CCCCCC
│  │                                   │  │
│  └───────────────────────────────────┘  │
│                                         │
│                                         │
│           ○  ○  ○  ●  ○                │
│                                         │
│  Tap to continue                        │  Mono 13px, #888888, centered
│                                         │  pulse animation (opacity 1↔0.4, 1.5s)
└─────────────────────────────────────────┘
```

**Three animation states (user taps to advance):**

**Step 1 — "Your Data"**
- Three metric pills fade in staggered (0.08s apart), slide up from 12px below
- Counter animation: numbers count up from 0 to final value over 0.6s
- Label: "YOUR DATA" + "Raw numbers from your wearable."

**Step 2 — "Your Insight" (tap to advance)**
- Metric pills slide left and shrink to 60% scale, grouped
- A connector line draws from pills → new insight card (animated, 0.4s, `#488BED` 2px stroke)
- Insight card appears:
```
┌───────────────────────────────────┐
│  "Nervous system under load.     │  Regular 16px, white
│   Recovery mode."                │
│                                  │
│  YOUR INSIGHT                    │  Eyebrow, 11px, #888BED
│  What the numbers actually mean. │  Regular, 14px, #CCCCCC
└───────────────────────────────────┘
```

**Step 3 — "Your Brew" (tap to advance)**
- Insight card slides left, shrinks
- Second connector line draws to brew card
- Brew card appears:
```
┌───────────────────────────────────┐
│  TIDE POOL                       │  CondensedBold 28px, white
│  Low extraction · 6h · 40g/L    │  Mono 13px, #888888
│                                  │
│  YOUR BREW                       │  Eyebrow, 11px, #488BED
│  Matched to your recovery.       │  Regular, 14px, #CCCCCC
└───────────────────────────────────┘
```

**Final state: all three visible left-to-right with connector lines:**
```
[Data] ──→ [Insight] ──→ [Brew]
```
Then CTA button fades in at bottom:
```
┌───────────────────────────────────┐
│          I get it →               │  blue button, 54px
└───────────────────────────────────┘
```

**Metric pill spec (`MetricPillView`):**
- Background: `#333333`, radius: 20px
- Padding: 12px horizontal, 10px vertical
- Label: Eyebrow, 10px, `#888888`, all-caps
- Value: `t_metric_lg`, 28px bold mono, white
- Min width: 90px, height: auto
- Counter animation: `Text("\(value)").contentTransition(.numericText())`

---

### Onboarding Screen 5 — Ready State (NEW)
**Purpose:** First dashboard preview. Land the user with confidence.

```
┌─────────────────────────────────────────┐
│                                         │
│                                         │
│                                         │
│                                         │
│        You're dialed in.                │  Heavy, 44px, white
│                                         │  32px left padding
│                                         │
│        ┌─────────────────────────────┐  │
│        │  ◉ Brezi Machine            │  │  #1A1A1A bg, 16px radius
│        │    Connected · BRZ-00412    │  │  or "Not paired" in #888888
│        └─────────────────────────────┘  │  padding 16px
│        ┌─────────────────────────────┐  │
│        │  ♥︎ WHOOP                    │  │
│        │    Syncing recovery data    │  │  or "Oura" / "Apple Health" / "Manual"
│        └─────────────────────────────┘  │
│                                         │
│        Your data flows through          │
│        Brezi's engine. Every morning,   │
│        your brew adapts.                │  Regular 16px, #CCCCCC
│                                         │  32px horizontal padding
│                                         │
│  ┌───────────────────────────────────┐  │
│  │       See your first brew →       │  │  blue button, 54px
│  └───────────────────────────────────┘  │  margin: 0 24px
│                                         │
│           ○  ○  ○  ○  ●                │
│                                         │
└─────────────────────────────────────────┘
```

**Device summary cards:**
- Show actual connection state from previous screens
- If device not paired: show grayed card with "Not paired — you can add it later" in `#888888`
- If wearable skipped: show "Manual input" with slider icon

**Transition:** Tapping "See your first brew" → mark `hasCompletedOnboarding = true` → push to `HomeDashboardView`. Show loading state: `"Reading your signal..."` (Mono 16px, white, centered, pulse animation, black bg, 1.2s max).

---

## 3. Home Dashboard (`HomeDashboardView`)

**This replaces `MorningCheckInView` as the default root screen.**

The dashboard has three cards stacked vertically, scrollable. Each card makes one layer of the correlation chain visible: raw data → insight → brew recommendation.

```
┌─────────────────────────────────────────┐
│  [≡]          B R E Z I          [⚙]   │  nav bar, #000
│─────────────────────────────────────────│
│  bg: #F5F5F5                            │
│                                         │
│  GOOD MORNING                           │  Mono 11px, #888888, all-caps
│  Thursday, March 19                     │  Regular 14px, #888888
│                                         │
│ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓ │
│ ┃  CARD 1: HEALTH DATA                 ┃ │
│ ┃                                      ┃ │
│ ┃  YOUR SIGNAL                         ┃ │  Mono 11px, #888888
│ ┃                                      ┃ │
│ ┃  ┌──────────┐ ┌──────────┐ ┌───────┐ ┃ │
│ ┃  │ HRV      │ │ STRAIN   │ │ SLEEP │ ┃ │
│ ┃  │ 68 ms    │ │ 12.3     │ │ 6h40m │ ┃ │
│ ┃  │ ↓ 14%    │ │ ↑ HIGH   │ │ ↓ LOW │ ┃ │
│ ┃  └──────────┘ └──────────┘ └───────┘ ┃ │
│ ┃                                      ┃ │
│ ┃  Source: WHOOP · 6:42am              ┃ │  Mono 11px, #888888
│ ┃  ────────────────────────────────     ┃ │
│ ┃  ↻ Refresh    Edit manually ›        ┃ │  blue text, 13px
│ ┃                                      ┃ │
│ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛ │  white bg, radius 16px, shadow
│                                         │  margin: 0 16px, padding: 24px
│            │                            │
│            │  ← vertical connector      │  2px, #488BED @ 40% opacity
│            │    16px tall               │  centered horizontally
│            ▼                            │
│ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓ │
│ ┃  CARD 2: INSIGHT                     ┃ │
│ ┃                                      ┃ │
│ ┃  THE READ                            ┃ │  Mono 11px, #488BED
│ ┃                                      ┃ │
│ ┃  "Nervous system under load.         ┃ │  Regular 18px, #000000
│ ┃   Recovery mode."                    ┃ │  line-height: 26px
│ ┃                                      ┃ │
│ ┃  HRV is down 14% from your          ┃ │  Regular 14px, #888888
│ ┃  7-day average. High strain          ┃ │
│ ┃  yesterday + short sleep = your      ┃ │
│ ┃  body is rebuilding.                 ┃ │
│ ┃                                      ┃ │
│ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛ │  subtleBlue bg, radius 16px
│                                         │  margin: 0 16px, padding: 24px
│            │                            │  NO shadow (tinted bg = flat)
│            │  "So your brew today →"    │  Mono 12px, #488BED, centered
│            │  ← vertical connector      │  includes text label
│            ▼                            │
│ ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓ │
│ ┃  CARD 3: BREW RECOMMENDATION         ┃ │
│ ┃                                      ┃ │  #0F0F0F bg, radius 16px
│ ┃  TODAY'S BREW                        ┃ │  Mono 11px, #488BED
│ ┃                                      ┃ │
│ ┃  TIDE POOL                           ┃ │  CondensedBold 34px, white
│ ┃                                      ┃ │
│ ┃  Low extraction, easy on the         ┃ │  Regular 15px, #CCCCCC
│ ┃  nervous system. You're not          ┃ │
│ ┃  skipping coffee — you're            ┃ │
│ ┃  adjusting the dial.                 ┃ │
│ ┃                                      ┃ │
│ ┃  TEMP     STEEP    RATIO             ┃ │  Mono 10px, #666666
│ ┃  4°C      6–8h     40 g/L           ┃ │  CondensedBold 20px, white
│ ┃                                      ┃ │
│ ┃  ┌─────────────────────────────────┐ ┃ │
│ ┃  │        Start this brew →        │ ┃ │  white bg, black text, 16px semibold
│ ┃  └─────────────────────────────────┘ ┃ │  height: 52px, radius: 12px
│ ┃                                      ┃ │  margin: 0 inside card padding
│ ┃  See full recommendation ›           ┃ │  text link, #888888, 13px, centered
│ ┃                                      ┃ │
│ ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛ │  margin: 0 16px, padding: 24px
│                                         │
│  ─────────────────────────────────────  │  divider, #E0E0E0
│                                         │
│  ┌────────────┐  ┌────────────────────┐ │
│  │ 📊 History  │  │ 🔌 My Devices     │ │  shortcut row
│  └────────────┘  └────────────────────┘ │  #FFFFFF bg, radius 12px, shadow
│                                         │  height: 48px, mono 13px, #000
│                                         │  SF Symbol left, 8px gap
│                                         │  equal width, 8px gap between
│                                         │  margin: 0 16px
│                                         │
│  Streak: 14 days                        │  Mono 12px, #488BED, centered
│                                         │
└─────────────────────────────────────────┘
```

### Health Data Card Spec (`HealthDataCardView`)

**Layout:** horizontal `HStack`, 3 equal columns, 8px gap

**Metric column spec (`MetricColumnView`):**
```
┌──────────┐
│  LABEL   │  Eyebrow, 10px, #888888, all-caps, centered
│  VALUE   │  t_metric_lg, 28px bold mono, #000, centered
│  DELTA   │  Mono 12px, colored, centered
└──────────┘
```
- Background: `#F5F5F5` (slightly recessed into card), radius 12px, padding 12px vertical
- Delta colors:
  - `↓ ##%` or `↓ LOW`: `#E84545` (redAlert) — if metric indicates poor recovery
  - `↑ ##%` or `↑ HIGH`: `#34C759` (greenPositive) — if metric indicates good recovery
  - Context-dependent: high strain = `#E84545` (bad for recovery), high HRV = `#34C759` (good)
  - Flat / no change: `#888888`
- Delta arrow: included in text string, not a separate icon

**Source row:**
- Mono 11px, `#888888`
- Format: `"Source: {WHOOP|Oura|Apple Health|Manual} · {time}"`
- Refresh icon: `arrow.clockwise` SF Symbol, 13pt, `#488BED`, tap to re-fetch
- "Edit manually": text link, `#488BED`, 13px — opens manual input sheet

**Manual input fallback (`ManualInputSheet`):**
- Presented as `.sheet` half-height (`.presentationDetents([.medium])`)
- Title: "LOG YOUR NUMBERS" — Mono 11px, #888888
- Three input fields:
  ```
  HRV (ms)        [  68  ]     ← numeric keypad, Mono 20px
  Strain (0–21)   [ 12.3 ]     ← numeric keypad, one decimal
  Sleep (hours)   [  6.7 ]     ← numeric keypad, one decimal
  ```
- OR: recovery slider (same as v1.0 `RecoverySliderView`) for simplified input
- Toggle at top: "Exact numbers" | "How I feel (1–10)"
- CTA: "Save" — blue, 48px
- Input field spec: `#F5F5F5` bg, `#E0E0E0` border 1px, radius 10px, height 48px, padding 16px horizontal

### Insight Strip Spec (`InsightStripView`)

- Background: `subtleBlue` — `rgba(72,139,237,0.08)`
- One-line headline: Regular 18px, `#000000` — max 2 lines
- Supporting detail: Regular 14px, `#888888` — max 4 lines
- Left border accent: 3px wide, `#488BED`, full height of card, radius 2px (inset)
- Padding: 24px all sides

**Insight copy generation rules (for recommendation engine):**

| HRV Trend | Strain | Sleep | Insight Headline | Detail |
|---|---|---|---|---|
| ↓ >10% | High (>14) | <7h | "Nervous system under load. Recovery mode." | "HRV is down {x}% from your 7-day average. High strain yesterday + short sleep = your body is rebuilding." |
| ↓ >10% | Low (<8) | >7h | "Something's off. Sleep was fine but your body disagrees." | "HRV dropped {x}% despite decent sleep. Could be stress, alcohol, or delayed onset from training 2 days ago." |
| Flat (±5%) | Moderate | 7–8h | "Steady state. Nothing heroic, nothing broken." | "Your numbers are holding. This is a maintenance day — don't waste it, don't force it." |
| ↑ >10% | Low (<8) | >8h | "Your system is primed. This is a peak window." | "HRV is up {x}% and you're well-rested. Your nervous system has capacity. Use it." |
| ↑ >5% | Any | >7h | "Recovering well. Trending in the right direction." | "HRV climbing back up. Whatever you did yesterday worked." |

### Vertical Connector Spec

- Width: 2px
- Color: `#488BED` @ 40% opacity
- Height: 16px (between cards)
- Centered horizontally in scroll view
- Between Card 2 and Card 3: includes text label "So your brew today →" — Mono 12px, `#488BED`, centered alongside the connector line
  - Text sits to the right of the connector, vertically centered on the line segment
  - Connector extends 8px above text and 8px below

### Brew Card (Dashboard version)
- Abbreviated version of full `BrewRecommendationView`
- "Start this brew" = sends command to paired Brezi machine (if connected) + logs brew
- "See full recommendation" = pushes to `BrewRecommendationView`
- If no Brezi machine paired: button text becomes "Log this brew" (manual confirmation)

### Shortcut Row
- Two equal-width pill buttons
- `#FFFFFF` bg, shadow, radius 12px, height 48px
- Icons: `chart.xyaxis.line` (History), `cable.connector` (Devices) — SF Symbols, 16pt, `#000`
- Text: Mono 13px, `#000`
- Tap: push to respective views

### Streak Counter
- Mono 12px, `#488BED`, centered below shortcut row
- Format: `"Streak: {n} days"` — counts consecutive days with a BrewLog entry
- If streak = 0: `"Start your streak. Brew today."` in `#888888`

### Dashboard Motion
1. Health card: slides up from 20px below, fades in, 0.35s, delay 0.0s
2. Connector 1: draws downward, 0.2s, delay 0.3s
3. Insight card: slides up from 20px below, fades in, 0.35s, delay 0.4s
4. Connector 2 + label: draws downward, text fades in, 0.2s, delay 0.7s
5. Brew card: slides up from 20px below, fades in, 0.35s, delay 0.8s
6. Bottom row: fades in, 0.3s, delay 1.1s

**Subsequent visits (same session):** No stagger animation. Instant render.
Store `hasAnimatedDashboardThisSession: Bool` in memory (not persisted).

---

## 4. Brew Recommendation Screen (`BrewRecommendationView`)

**Entry:** Pushed from dashboard "See full recommendation" or "Start this brew" (after logging).

This is the expanded, full-detail version. The correlation chain is shown vertically with all data visible and expandable.

```
┌─────────────────────────────────────────┐
│  ‹ Back          B R E Z I       [⚙]   │
│─────────────────────────────────────────│
│  bg: #000000 (full dark mode screen)    │
│                                         │
│  ┌───────────────────────────────────┐  │
│  │                                   │  │
│  │  TODAY'S BREW                     │  │  Mono 11px, #488BED
│  │                                   │  │
│  │  TIDE POOL                        │  │  CondensedBold 42px, white
│  │                                   │  │
│  │  ─────────────────────────────    │  │  divider, #333
│  │                                   │  │
│  │  SECTION 1: THE DATA              │  │  Mono 11px, #666, all-caps
│  │                                   │  │
│  │  HRV         STRAIN      SLEEP    │  │  Mono 10px, #666
│  │  68 ms       12.3        6h 40m   │  │  CondensedBold 22px, white
│  │  ↓14%        ↑ high      ↓ low    │  │  Mono 12px, colored (red/red/red)
│  │                                   │  │
│  │  ─────────────────────────────    │  │
│  │                                   │  │
│  │  SECTION 2: THE READ              │  │  Mono 11px, #488BED, all-caps
│  │                                   │  │
│  │  "Nervous system under load.      │  │  Regular 17px, #CCCCCC
│  │   Recovery mode."                 │  │  italic
│  │                                   │  │
│  │  HRV dropped 14% from your       │  │  Regular 14px, #888888
│  │  7-day average (79ms → 68ms).     │  │
│  │  Strain was 12.3 yesterday.       │  │
│  │  Sleep was 40 minutes below       │  │
│  │  your baseline.                   │  │
│  │                                   │  │
│  │  ─── So your brew today → ─────  │  │  Mono 12px, #488BED
│  │                                   │  │
│  │  SECTION 3: THE BREW              │  │  Mono 11px, #488BED, all-caps
│  │                                   │  │
│  │  TEMP        STEEP       RATIO    │  │  Mono 10px, #666
│  │  4°C         6–8h        40 g/L   │  │  CondensedBold 24px, white
│  │                                   │  │
│  │  Caffeine: Low                    │  │  Mono 13px, #888888
│  │                                   │  │
│  └───────────────────────────────────┘  │  #0F0F0F bg, radius 24px
│                                         │  padding: 32px h, 36px v
│                                         │  margin: 0 16px
│                                         │
│  ┌───────────────────────────────────┐  │
│  │                                   │  │
│  │  BEAN ORIGIN                      │  │  Mono 11px, #888888
│  │                                   │  │
│  │  Ethiopian Yirgacheffe            │  │  CondensedBold 20px, white
│  │  or similar light-roast single    │  │  Regular 14px, #CCCCCC
│  │  origin. Low-acid, naturally      │  │
│  │  sweet. Pairs with your low-      │  │
│  │  extraction Tide Pool profile.    │  │
│  │                                   │  │
│  │  ─────────────────────────────    │  │
│  │                                   │  │
│  │  GRIND          DOSE              │  │  Mono 10px, #666
│  │  Coarse         40g               │  │  CondensedBold 20px, white
│  │  (sea salt)     (for 1L water)    │  │  Regular 13px, #888888
│  │                                   │  │
│  │  ─────────────────────────────    │  │
│  │                                   │  │
│  │  BEST TIME TO DRINK              │  │  Mono 11px, #888888
│  │                                   │  │
│  │  Before 11am                      │  │  CondensedBold 20px, white
│  │  Your cortisol window is early.   │  │  Regular 14px, #CCCCCC
│  │  Don't wait until afternoon —     │  │
│  │  even with low caffeine.          │  │
│  │                                   │  │
│  └───────────────────────────────────┘  │  #1A1A1A bg, radius 16px
│                                         │  padding: 24px, margin: 0 16px, 12px gap
│                                         │
│  ┌───────────────────────────────────┐  │
│  │                                   │  │
│  │  ⚠  WHAT TO AVOID TODAY          │  │  Mono 11px, #FFB833 (amberWarning)
│  │                                   │  │
│  │  • Skip the double shot           │  │  Regular 15px, #CCCCCC
│  │    before 2pm.                    │  │  bullet: `•` in #FFB833
│  │                                   │  │
│  │  • No espresso chaser — your      │  │
│  │    nervous system doesn't         │  │
│  │    have the headroom.             │  │
│  │                                   │  │
│  │  • Avoid caffeine entirely        │  │
│  │    after 2pm today.               │  │
│  │                                   │  │
│  └───────────────────────────────────┘  │  #1A1A1A bg, radius 16px
│                                         │  left border: 3px, #FFB833
│                                         │  padding: 24px, margin: 0 16px
│                                         │
│                                         │
│  ┌───────────────────────────────────┐  │
│  │        Start this brew →          │  │  #488BED bg, white text, 16px semibold
│  └───────────────────────────────────┘  │  height: 54px, radius: 12px
│                                         │  margin: 0 24px
│                                         │
│  ┌───────────────────────────────────┐  │
│  │        Adjust settings            │  │  outlined, #333 border, white text
│  └───────────────────────────────────┘  │  height: 48px, radius: 12px
│                                         │  margin: 0 24px, 8px gap
│                                         │
│  Choose a different brew ›              │  text link, #888888, 14px, centered
│                                         │
│                                         │
└─────────────────────────────────────────┘
```

### Section detail specs

**Bean origin recommendation logic:**

| Profile | Bean Suggestion | Grind | Dose (per 1L) |
|---|---|---|---|
| Tide Pool | Ethiopian Yirgacheffe or light single origin | Coarse (sea salt) | 40g |
| First Light | Colombian or medium-roast blend | Medium-coarse (kosher salt) | 60g |
| Deep Work | Guatemalan or medium-dark single origin | Medium-coarse (kosher salt) | 70g |
| Voltage | Sumatra Mandheling or dark roast | Medium (table salt) | 80g |
| The Blank | "Your call." | User-set | User-set |

**Best time to drink logic:**

| Profile | Recommendation | Reasoning |
|---|---|---|
| Tide Pool | Before 11am | Low caffeine still hits; cortisol window is early on recovery days |
| First Light | 7–10am | Standard cortisol alignment |
| Deep Work | 9am–12pm | Delayed intake for sustained release during focus blocks |
| Voltage | 6–9am | Peak readiness = early start; get it in before cortisol drops |
| The Blank | "When you want." | Manual = no guardrails |

**What NOT to do logic:**

| Profile | Warnings |
|---|---|
| Tide Pool | No double shots. No espresso chasers. No caffeine after 2pm. |
| First Light | Don't stack another coffee before noon. If you need a second, make it Tide Pool. |
| Deep Work | Don't front-load — let the slow release work. No pre-workout caffeine on top. |
| Voltage | Don't add sugar — it'll spike-and-crash. No caffeine after 1pm even though you feel good. |
| The Blank | "You're on your own. Don't say we didn't warn you." |

### "Adjust settings" sheet
Same spec as v1.0:
- Modal half-sheet (`.presentationDetents([.medium])`)
- Background: `#1A1A1A`
- Three sliders:
  - Temp (°C): range 2–25, step 1, track `#333`, active `#488BED`, thumb white 28px
  - Steep time (hours): range 4–24, step 1
  - Ratio (g/L): range 20–100, step 5
- Live preview updates brew card values
- CTA: "Apply & Start" — blue, 48px

### "Choose a different brew" sheet
- Full-screen modal, `#000000` bg
- Title: "ALL PROFILES" — Mono 11px, #888888
- 5 profile cards stacked vertically:
  ```
  ┌───────────────────────────────────┐
  │  TIDE POOL              ← MATCH  │  recommended has "← MATCH" badge
  │  Recovery · Low caffeine          │  in #488BED
  │  Score range: 1–4                 │
  └───────────────────────────────────┘
  ```
  - Recommended: `#488BED` border 2px, `rgba(72,139,237,0.08)` bg
  - Others: `#1A1A1A` bg, `#333` border 1px
  - Card height: 80px, radius 16px, padding 20px
  - Name: CondensedBold 20px, white
  - Subtitle: Regular 14px, #888888
  - Tap: selects and dismisses, reloads recommendation screen with new profile

---

## 5. History & Correlation View (`HistoryCorrelationView`)

**Entry:** "History" shortcut on dashboard, or nav push.

```
┌─────────────────────────────────────────┐
│  ‹ Back          B R E Z I       [⚙]   │
│─────────────────────────────────────────│
│  bg: #F5F5F5                            │
│                                         │
│  YOUR PRACTICE                          │  Mono 11px, #888888
│                                         │
│  ┌───────────────────────────────────┐  │
│  │                                   │  │
│  │  CORRELATION                      │  │  Mono 11px, #488BED
│  │                                   │  │
│  │  HRV (7-day)                      │  │  Eyebrow 10px, #888888
│  │  ┌───────────────────────────┐    │  │
│  │  │  82                       │    │  │
│  │  │   ╲       ╱╲              │    │  │  line chart, #488BED stroke 2px
│  │  │    ╲     ╱  ╲    ╱╲      │    │  │  area fill: #488BED @ 8%
│  │  │     ╲   ╱    ╲  ╱  ╲     │    │  │  height: 100px
│  │  │      ╲╱       ╲╱    ╲    │    │  │
│  │  │  62                  ╲   │    │  │  y-axis: Mono 10px, #888888
│  │  │  M  T  W  T  F  S  S    │    │  │  x-axis: Mono 10px, #888888
│  │  └───────────────────────────┘    │  │
│  │                                   │  │
│  │  Brew History                     │  │  Eyebrow 10px, #888888
│  │  ┌───────────────────────────┐    │  │
│  │  │  ■ ■ ■ ■ □ ■ ■            │    │  │  dot/bar chart aligned to same x-axis
│  │  │  V DW FL FL - TP FL       │    │  │  each bar = brew profile that day
│  │  └───────────────────────────┘    │  │  filled: brewed, empty: skipped
│  │                                   │  │  bar colors: profile-specific
│  │  ─────────────────────────────    │  │
│  │                                   │  │
│  │  💡 PATTERN                       │  │  Mono 11px, #488BED
│  │                                   │  │
│  │  "On your Voltage days, your     │  │  Regular 16px, #000
│  │   next-day HRV averages          │  │
│  │   8% higher."                    │  │  line-height: 24px
│  │                                   │  │
│  └───────────────────────────────────┘  │  white bg, radius 16px, shadow
│                                         │  padding: 24px, margin: 0 16px
│                                         │
│  ┌───────────────────────────────────┐  │
│  │  STREAK                           │  │  Mono 11px, #888888
│  │                                   │  │
│  │  14 days                          │  │  CondensedBold 48px, #488BED
│  │  Your longest: 22 days            │  │  Mono 13px, #888888
│  │                                   │  │
│  │  ○ ○ ○ ● ● ● ● ● ● ● ● ● ● ●  │  │  dot row: last 14 days
│  │  M T W T F S S M T W T F S S    │  │  filled=#488BED, empty=#E0E0E0
│  │                                   │  │  dot size: 10px, gap: 6px
│  └───────────────────────────────────┘  │  white bg, radius 16px, shadow
│                                         │  padding: 24px, margin: 0 16px, 12px gap
│                                         │
│  ─────────────────────────────────────  │
│                                         │
│  BREW LOG                               │  Mono 11px, #888888
│                                         │
│  ─ March 2026 ───────────────────────  │  Mono 12px, #888888
│                                         │
│  ┌───────────────────────────────────┐  │
│  │  TIDE POOL              Mar 19    │  │  CondensedBold 17px + Mono 12px #888
│  │  4°C · 6h · 40g/L                │  │  Mono 13px, #666
│  │  HRV: 68ms · Strain: 12.3        │  │  Mono 12px, #888  ← NEW: shows source data
│  │  ★ ★ ★ ☆ ☆                       │  │  tap to rate
│  └───────────────────────────────────┘  │  white bg, border 1px #E0E0E0, radius 12px
│                                         │  padding 16px, margin 0 16px
│  ┌───────────────────────────────────┐  │
│  │  FIRST LIGHT            Mar 18    │  │
│  │  4–8°C · 12h · 60g/L             │  │
│  │  HRV: 74ms · Strain: 8.1         │  │
│  │  ★ ★ ★ ★ ☆                       │  │
│  └───────────────────────────────────┘  │
│                                         │
│  ┌───────────────────────────────────┐  │
│  │  VOLTAGE                Mar 17    │  │
│  │  15–20°C · 14h · 80g/L           │  │
│  │  HRV: 82ms · Strain: 6.2         │  │
│  │  ★ ★ ★ ★ ★                       │  │
│  └───────────────────────────────────┘  │
│                                         │
│  Show earlier ›                         │  text link, #888888, centered
│                                         │
└─────────────────────────────────────────┘
```

### Correlation Chart Spec (`CorrelationChartView`)

**HRV line chart:**
- Framework: Swift Charts (`import Charts`)
- Line mark: `#488BED`, stroke 2px, interpolation `.catmullRom`
- Area mark: `#488BED` @ 8% opacity fill below line
- Y-axis: auto-scaled with 10% padding, labels Mono 10px `#888888`, right-aligned
- X-axis: day abbreviations, Mono 10px `#888888`, centered under each data point
- Grid lines: horizontal only, `#E0E0E0` @ 50% opacity, 0.5px
- Height: 100px fixed
- Interaction: tap data point → tooltip shows exact value + date

**Brew history bar chart:**
- Aligned to same x-axis as HRV chart (critical — same time scale)
- Each day: colored square 16x16px, centered under date
  - Voltage: `#488BED`
  - Deep Work: `#6B5CE7` (purple)
  - First Light: `#888888` (grey)
  - Tide Pool: `#34C759` (green)
  - The Blank: `#FFB833` (amber)
  - No brew: empty square, `#E0E0E0` border 1px
- Profile abbreviation below: Mono 8px, same color as square
- Spacing matches HRV chart x-axis exactly

**Pattern callout spec:**
- Icon: `lightbulb.fill` SF Symbol, 14pt, `#488BED`
- Label: "PATTERN" — Mono 11px, `#488BED`
- Text: Regular 16px, `#000000`
- Generated after ≥7 data points. Below 7: show `"Keep logging. Patterns emerge after a week."` in `#888888`

**Pattern generation rules (for recommendation engine):**
- Compare next-day HRV after each profile type
- Surface the strongest positive correlation
- Template: `"On your {Profile} days, your next-day HRV averages {x}% {higher|lower}."`
- If no significant pattern: `"No clear pattern yet. Your data needs more range — try varying your brew."`

### Streak Spec (`BrewStreakView`)
- Current streak: CondensedBold 48px, `#488BED`
- Record: Mono 13px, `#888888`
- Dot row: last 14 days, each dot 10px diameter, 6px gap
  - Brewed: `#488BED` fill
  - Skipped: `#E0E0E0` fill
- Day labels: Mono 8px, `#888888`, centered under each dot

### Brew Log Cell (updated from v1.0)
- Now includes source health data: `"HRV: {x}ms · Strain: {y}"` — Mono 12px, `#888888`
- Rest of spec unchanged from v1.0

### Empty state (no history)
```
No history yet.
This is where your pattern shows up —
recovery trends, brew streaks, what your
body actually responds to.

Come back in a week.
You'll want to see this.

[Log today's brew →]
```
- Regular 16px, `#333`, centered, 32px padding
- CTA: blue text link

---

## 6. Device Hub (`DeviceHubView`)

**Entry:** Hamburger menu or "My Devices" shortcut on dashboard.
**Purpose:** Manage connected devices. Extensible for future Brezi Blend (smoothie blender).

```
┌─────────────────────────────────────────┐
│  ‹ Back          B R E Z I       [⚙]   │
│─────────────────────────────────────────│
│  bg: #F5F5F5                            │
│                                         │
│  MY DEVICES                             │  Mono 11px, #888888
│                                         │
│  ┌───────────────────────────────────┐  │
│  │                                   │  │
│  │  ┌────────┐                       │  │
│  │  │ ◉ CUBE │  Brezi Cold Brew      │  │  SF Symbol: `cube.fill`, 32pt, white
│  │  │  icon  │  Machine              │  │  inside 56x56 circle, #0F0F0F bg
│  │  └────────┘                       │  │
│  │                                   │  │
│  │  ● Connected                      │  │  greenPositive, Mono 13px
│  │  Serial: BRZ-00412               │  │  Mono 12px, #888888
│  │  Firmware: 1.2.0                  │  │  Mono 12px, #888888
│  │                                   │  │
│  │  ─────────────────────────────    │  │
│  │                                   │  │
│  │  TODAY'S BREW FOR THIS DEVICE     │  │  Mono 10px, #888888
│  │                                   │  │
│  │  TIDE POOL                        │  │  CondensedBold 22px, #000
│  │  4°C · 6–8h · 40g/L              │  │  Mono 13px, #888888
│  │                                   │  │
│  │  ┌─────────────────────────────┐  │  │
│  │  │       Start brew →          │  │  │  blue, 48px
│  │  └─────────────────────────────┘  │  │
│  │                                   │  │
│  └───────────────────────────────────┘  │  white bg, radius 16px, shadow
│                                         │  padding: 24px, margin: 0 16px
│                                         │
│  ┌───────────────────────────────────┐  │
│  │                                   │  │
│  │  ┌────────┐                       │  │
│  │  │  BLEND │  Brezi Blend          │  │  SF Symbol: `blender.fill` (custom
│  │  │  icon  │                       │  │  fallback: `cup.and.saucer.fill`), 32pt
│  │  └────────┘                       │  │  inside 56x56 circle, #333 bg (dimmed)
│  │                                   │  │
│  │  COMING SOON                      │  │  Mono 11px, #488BED
│  │                                   │  │
│  │  Same health data. Same engine.   │  │  Regular 14px, #888888
│  │  Smoothie ingredients matched     │  │
│  │  to your recovery.               │  │
│  │                                   │  │
│  │  [Notify me when it launches →]   │  │  text link, #488BED, 14px
│  │                                   │  │
│  └───────────────────────────────────┘  │  white bg, radius 16px, shadow
│                                         │  padding: 24px, margin: 0 16px
│                                         │  opacity: 0.7 (dimmed)
│                                         │
│  ─────────────────────────────────────  │
│                                         │
│  WEARABLE SOURCE                        │  Mono 11px, #888888
│                                         │
│  ┌───────────────────────────────────┐  │
│  │  WHOOP              ● Connected   │  │  or Oura / Apple Health / Manual
│  │  Last sync: 6:42am               │  │  Mono 12px, #888888
│  │                         Change ›  │  │  text link, #488BED, right-aligned
│  └───────────────────────────────────┘  │  white bg, radius 12px, border 1px #E0E0E0
│                                         │  padding: 16px, margin: 0 16px
│                                         │
│  + Add a device                         │  text link, #488BED, 14px, centered
│                                         │  → opens BLE scan flow (same as onboarding)
│                                         │
└─────────────────────────────────────────┘
```

### Device Card Spec (`DeviceCardView`)

**Connected device:**
- White background, radius 16px, shadow
- Device icon: 32pt SF Symbol inside 56x56 circle, `#0F0F0F` bg, radius 28px (circle)
- Name: CondensedBold 20px, `#000`
- Status: `● Connected` — `#34C759`, Mono 13px (green dot is 8px circle inline)
- Serial/firmware: Mono 12px, `#888888`
- Brew recommendation: abbreviated, same profile as dashboard
- CTA: "Start brew" — sends BLE command to machine

**Coming soon device:**
- Same structure but dimmed: `opacity(0.7)` on entire card
- Icon circle: `#333` bg instead of `#0F0F0F`
- "COMING SOON" badge: Mono 11px, `#488BED`
- No CTA button, replaced with notification signup link

**Wearable source card:**
- Compact, single row with status
- "Change" link → opens wearable connection sheet (reuses onboarding Screen 3 as a sheet)

**Future extensibility note for Jensen:**
The device hub uses a `DeviceType` enum:
```swift
enum DeviceType: String, CaseIterable {
    case coldBrew = "cold_brew"
    case blend = "blend"    // future
    // case pour_over = "pour_over"  // far future
}
```
Each device type has its own recommendation engine entry point but shares the same health data pipeline. The `BrewProfile` model should be extended with a `deviceType: String` field so profiles can be filtered per device.

---

## 7. Settings (`SettingsView`)

**Entry:** Gear icon in nav bar.

```
┌─────────────────────────────────────────┐
│  ‹ Back          B R E Z I       [⚙]   │
│─────────────────────────────────────────│
│  bg: #F5F5F5                            │
│                                         │
│  SETTINGS                               │  Mono 11px, #888888
│                                         │
│  ─ Devices ──────────────────────────  │  Mono 12px, #888888
│                                         │
│  Brezi Machine     ● Connected    [ › ] │  tap → DeviceHubView
│  Wearable          WHOOP          [ › ] │  tap → wearable connection sheet
│                                         │
│  ─ Preferences ──────────────────────  │
│                                         │
│  Units             Metric          [ › ] │  tap → picker
│  Morning reminder  8:00 AM      [toggle] │  toggle: standard iOS switch
│  Caffeine cutoff   2:00 PM         [ › ] │  tap → time picker
│                                         │
│  ─ Data ─────────────────────────────  │
│                                         │
│  Export brew history                [ › ] │  exports CSV
│  Clear all data                     [ › ] │  destructive, confirmation alert
│                                         │
│  ─ About ────────────────────────────  │
│                                         │
│  Version           0.1.0                │  Mono 13px, #888888
│  Built by Brezi                         │  Mono 13px, #888888
│                                         │
└─────────────────────────────────────────┘
```

**Settings row spec:**
- Height: 52px
- Label: Regular 16px, `#000`, left-aligned
- Value: Regular 16px, `#888888`, right-aligned (before chevron)
- Chevron: `chevron.right` SF Symbol, 12pt, `#CCCCCC`
- Separator: `#E0E0E0`, 0.5px, full width
- Toggle: standard `Toggle()` SwiftUI, tint `#488BED`

**Morning reminder:**
- When enabled: schedules `UNUserNotificationCenter` local notification
- Copy: `"Your signal's ready. Time to check in."` — Mono tone
- Request permission on first enable

**Caffeine cutoff:**
- Stored in UserDefaults
- Used by recommendation engine: if current time > cutoff, downgrade brew profile by 1 tier
- Display: time picker wheel, `.datePickerStyle(.wheel)`

---

## 8. Loading & Transition States

### "Reading your signal..."
Used during: onboarding → dashboard, manual input save, wearable data refresh.
```
Full screen: #000000 bg
"Reading your signal..."     Mono 16px, white, centered vertically + horizontally
Pulse: opacity 1.0 ↔ 0.4, 1.2s loop (m_pulse)
Max display: 1.5s → transition to destination
```

### Brew started confirmation
After "Start this brew" or "Log this brew":
```
Full screen overlay: #000000 @ 90% opacity
Checkmark: SF Symbol `checkmark.circle.fill`, 64pt, #488BED
"Logged."                    Mono 14px, #888888, 8px below checkmark
Scale animation: checkmark 0.0 → 1.0, spring (response: 0.35, damping: 0.6)
Auto-dismiss: 1.2s → pop back to dashboard
```

### BLE command sent confirmation
After "Start brew" sends command to machine:
```
Same overlay as above, but:
"Brewing."                   Mono 14px, #888888
Subtitle: "Tide Pool · 6h"  Mono 12px, #888888
```

### Pull-to-refresh (dashboard)
- Standard `RefreshControl`
- Triggers wearable data re-fetch
- Loading text: `"Pulling your latest numbers..."` — Mono 13px, #888888

---

## 9. Empty States

| Screen | Condition | Copy | CTA |
|---|---|---|---|
| Dashboard — no wearable | `wearableSource == "manual"` and no manual input today | "Flying manual right now. That's fine. Log HRV by hand or connect a wearable when you're ready." | [Log manually →] / [Connect a device →] |
| Dashboard — wearable connected, no data yet | First morning after connecting | "Your wearable is synced but hasn't sent today's numbers yet. Check back after your first sleep cycle." | [Enter manually →] |
| History — no brews | Zero BrewLog entries | "No history yet. This is where your pattern shows up — recovery trends, brew streaks, what your body actually responds to. Come back in a week. You'll want to see this." | [See today's recommendation →] |
| History — <7 days | 1–6 BrewLog entries | "Keep logging. Patterns emerge after a week." (replaces correlation chart) | — |
| Device Hub — no Brezi | `pairedDeviceUUID == nil` | "No Brezi machine paired. You can still get recommendations — just log your brews manually." | [Pair a device →] |
| Settings — no wearable | `wearableSource == "manual"` | "Flying manual right now. Connect Oura, WHOOP, or Apple Health when you're ready." | [Connect →] |

---

## 10. Component Library (updated)

| Component | File | Used in |
|---|---|---|
| `HealthDataCardView` | HealthDataCardView.swift | Dashboard |
| `MetricColumnView` | MetricColumnView.swift | Dashboard, Brew Rec |
| `InsightStripView` | InsightStripView.swift | Dashboard |
| `VerticalConnectorView` | VerticalConnectorView.swift | Dashboard |
| `BrewCardView` | BrewCardView.swift | Dashboard, Brew Rec |
| `MetricPillView` | MetricPillView.swift | Onboarding tutorial |
| `WearableOptionCard` | WearableOptionCard.swift | Onboarding, Settings |
| `DeviceCardView` | DeviceCardView.swift | Device Hub |
| `CorrelationChartView` | CorrelationChartView.swift | History |
| `BrewStreakView` | BrewStreakView.swift | History, Dashboard |
| `BrewLogCell` | BrewLogCell.swift | History |
| `RatingStarsView` | RatingStarsView.swift | History |
| `ManualInputSheet` | ManualInputSheet.swift | Dashboard |
| `RecoverySliderView` | RecoverySliderView.swift | Manual input |
| `ProfileCardView` | ProfileCardView.swift | Brew Rec, Profile picker |
| `SaveButton` | SaveButton.swift | All screens |
| `OnboardingPageView` | OnboardingPageView.swift | Onboarding (shared shell) |
| `BLEScanView` | BLEScanView.swift | Onboarding, Device Hub |
| `EmptyStateView` | EmptyStateView.swift | All screens |

---

## 11. Data Model Updates (for Jensen)

### New/modified Core Data entities

**CheckIn (updated)**
```
- id: UUID
- userId: UUID
- recoveryScore: Int16 (1–10)       // kept for manual/simplified input
- hrvMs: Double?                     // NEW — raw HRV in milliseconds
- strain: Double?                    // NEW — 0–21 scale (WHOOP)
- sleepHours: Double?                // NEW — total sleep in hours
- sleepQuality: String?              // NEW — "poor" / "fair" / "good" / "excellent"
- source: String                     // "whoop" / "oura" / "apple_health" / "manual"
- insightText: String?               // NEW — generated insight headline
- insightDetail: String?             // NEW — generated insight body
- createdAt: Date
- timezoneOffsetSeconds: Int32
- note: String?
```

**BrewLog (updated)**
```
- id: UUID
- profileKey: String
- brewedAt: Date
- brewSettingsJSON: String?
- beanOrigin: String?                // NEW
- grindSetting: String?              // NEW
- doseGrams: Double?                 // NEW
- recommendedDrinkBy: String?        // NEW — e.g. "Before 11am"
- sourceHrvMs: Double?               // NEW — snapshot of HRV at time of brew
- sourceStrain: Double?              // NEW — snapshot of strain
- sourceSleepHours: Double?          // NEW — snapshot of sleep
- howFelt: Int16? (1–5)
- note: String?
- createdAt: Date
```

**Device (NEW)**
```
- id: UUID
- deviceType: String                 // "cold_brew" / "blend"
- serialNumber: String?
- firmwareVersion: String?
- bleIdentifier: String?             // CBPeripheral identifier
- isPaired: Bool
- lastConnectedAt: Date?
- createdAt: Date
```

### Profile key mapping (updated — uses CMO brew names)
```swift
func profileKey(for score: Int) -> String {
    switch score {
    case ...2:   return "tide_pool"
    case 3...4:  return "tide_pool"
    case 5...6:  return "first_light"
    case 7...8:  return "deep_work"
    case 9...10: return "voltage"
    default:     return "first_light"
    }
}

// When HRV/strain/sleep data available, use weighted scoring:
func profileKey(hrv: Double, strain: Double, sleepHours: Double, baseline: UserBaseline) -> String {
    let hrvDelta = (hrv - baseline.avgHrv) / baseline.avgHrv  // % change
    let sleepDelta = sleepHours - baseline.avgSleep

    // Composite recovery score (0–10)
    var score = 5.0
    score += hrvDelta * 15       // HRV has highest weight
    score -= (strain / 21) * 3   // high strain penalizes
    score += sleepDelta * 0.8    // sleep hours contribute

    return profileKey(for: Int(score.clamped(to: 1...10)))
}
```

---

## 12. Navigation Flow Summary

```
FIRST LAUNCH:
  Onboarding 1 (Brand) → 2 (Device Pair) → 3 (Wearable) → 4 (Tutorial) → 5 (Ready)
  → Loading → HomeDashboard

DAILY USE:
  HomeDashboard
    ├── "Start this brew" → brew confirmation overlay → stays on dashboard
    ├── "See full recommendation" → BrewRecommendationView
    │     ├── "Start this brew" → confirmation → pop to dashboard
    │     ├── "Adjust settings" → half-sheet → apply
    │     └── "Choose different brew" → full-screen picker → reloads
    ├── "History" shortcut → HistoryCorrelationView
    │     └── brew cells (read-only, rate via stars)
    ├── "My Devices" shortcut → DeviceHubView
    │     ├── device detail (start brew, see serial)
    │     └── "+ Add a device" → BLE scan flow
    ├── [≡] hamburger → DeviceHubView
    └── [⚙] gear → SettingsView
          ├── Brezi Machine → DeviceHubView
          ├── Wearable → connection sheet
          └── preferences, data export, about
```

---

## 13. Build Order (revised for Jensen)

1. **Day 1–2:** Project setup — design tokens (Color/Font extensions), Core Data stack with updated entities, seed 5 brew profiles, `profileKey(for:)` logic
2. **Day 3–4:** `HealthDataCardView` + `MetricColumnView` + `InsightStripView` + `VerticalConnectorView` + `ManualInputSheet` — build the correlation chain components
3. **Day 5–6:** `HomeDashboardView` — wire cards together, mock data, stagger animation
4. **Day 7–8:** `BrewRecommendationView` — full detail screen with bean origin, grind, timing, warnings
5. **Day 9–10:** `HistoryCorrelationView` — correlation chart (Swift Charts), streak, brew log list
6. **Day 11–12:** `DeviceHubView` — device cards, BLE scan stub (actual BLE integration deferred to hardware availability)
7. **Day 13–14:** Onboarding flow — 5 screens, tutorial animation, device pairing flow
8. **Day 15–16:** `SettingsView`, wearable connection sheets, empty states
9. **Day 17–18:** Polish — motion, transitions, loading states, haptics, pull-to-refresh
10. **Day 19–20:** TestFlight prep — test all flows, stub wearable data, mock BLE

**TestFlight target: 20 working days from design lock**

---

## 14. Decisions — locked by Jackson (March 2026)

1. **Onboarding order:** Device pair → Wearable → Tutorial → Ready. Non-negotiable.
2. **Correlation chain visible on dashboard.** Raw data → insight → brew. Three cards, vertical connectors, explicit "because" language.
3. **Brew recommendation expanded.** Bean origin, grind, dose, best time, what to avoid. All visible without expanding.
4. **Profile names use CMO copy:** Tide Pool, First Light, Deep Work, Voltage, The Blank.
5. **Profile key mapping synced** between this spec and `jensen-tech-spec.md`.
6. **Device Hub extensible.** `DeviceType` enum. Brezi Blend is "coming soon" placeholder from day one.
7. **System fonts for prototype.** ABC Social swapped pre-launch.
8. **Morning push notification:** YES — same spec as v1.0.
9. **Core Data iCloud sync:** YES — `NSPersistentCloudKitContainer` from day one.
10. **BLE integration:** Stubbed in v0.1. Full integration when hardware team provides service UUIDs.

---

*Spec written March 2026. This is iteration 1 — expect review feedback from Jackson before dev handoff. Any changes to profile names, score thresholds, or data models must update both this spec and `jensen-tech-spec.md` simultaneously.*
