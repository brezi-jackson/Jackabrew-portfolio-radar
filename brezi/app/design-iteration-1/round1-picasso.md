# Brezi iOS App — Wireframe & Design Spec v2.0
**Author:** Picasso (Product Design) | March 19, 2026 | For Jensen + Jackson

---

## Design Philosophy — What Changed and Why

v1 hid the most interesting thing about the product. You entered a number, got a brew name, and had to trust it. The data-to-brew chain was a black box. Jackson called it correctly: the correlation between your body and your cup is the entire product. If the user can't see it, it doesn't exist.

Three structural changes:

1. **The Chain is the UI.** Every recommendation screen now shows: Your Data → Your Insight → Your Brew. Three cards, stacked vertically, each one feeding the next. No mystery.
2. **Onboarding pairs the machine first.** The device is the product. The app is the companion. Onboarding now: Brand moment → Bluetooth pair → Wearable connect → Tutorial (the chain) → Dashboard landing.
3. **Extensible from day one.** The data layer doesn't say "coffee." It says "recommendation." When Brezi Blender ships, the same health card feeds a smoothie card instead of a brew card. The architecture is device-agnostic.

---

## 0. Design Tokens

### Colors
```swift
let black        = Color(hex: "#000000")  // primary text, nav, dark surfaces
let white        = Color(hex: "#FFFFFF")  // card surfaces, inputs
let offWhite     = Color(hex: "#F5F5F5")  // app background
let blue         = Color(hex: "#488BED")  // accent, CTAs, active states, data highlights
let midGrey      = Color(hex: "#888888")  // secondary text, labels
let lightGrey    = Color(hex: "#E0E0E0")  // dividers, borders
let darkCard     = Color(hex: "#0F0F0F")  // brew card background
let darkSurface  = Color(hex: "#1A1A1A")  // secondary dark surfaces, selection cards

// Status colors
let statusGreen  = Color(hex: "#34C759")  // peaked / high recovery
let statusYellow = Color(hex: "#FFD60A")  // functional / mid recovery
let statusRed    = Color(hex: "#FF453A")  // rebuilding / low recovery
let statusBlue   = Color(hex: "#488BED")  // data/insight accent (reuse brand blue)
```

### Typography
**System fonts for prototype. Swap to ABC Social before public launch.**

```swift
let t_display  = Font.system(size: 48, weight: .heavy)          // hero headlines
let t_h1       = Font.system(size: 34, weight: .bold)           // screen titles
let t_h2       = Font.system(size: 26, weight: .bold)           // card titles
let t_h3       = Font.system(size: 20, weight: .semibold)       // section headers
let t_body     = Font.system(size: 16, weight: .regular)        // body text
let t_small    = Font.system(size: 14, weight: .regular)        // secondary text
let t_mono     = Font.system(size: 13, weight: .regular, design: .monospaced) // data values
let t_eyebrow  = Font.system(size: 11, weight: .medium, design: .monospaced)  // labels, ALL CAPS
let t_data     = Font.system(size: 28, weight: .bold, design: .monospaced)    // NEW — large data readouts
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
Single push-stack with modal sheets for device management and manual override. No tab bar.

```
AppRoot
 └── OnboardingFlow (5 screens, first launch only)
      │  ├── 1. BrandIntroView
      │  ├── 2. DevicePairView         ← Bluetooth pair Brezi machine
      │  ├── 3. WearableConnectView    ← WHOOP / Oura / Apple Health / Skip
      │  ├── 4. DataChainTutorialView  ← shows data → insight → brew chain
      │  └── 5. FirstCheckInView       ← recovery input → lands on Dashboard
      │
      └── MainFlow
           ├── DashboardView                  ← default home (health + insight + brew)
           ├── BrewRecommendationView          ← full brew detail, pushed from dashboard
           ├── HistoryView                     ← brew log + correlation charts
           ├── DeviceHubView                   ← manage Brezi + wearables + future devices
           └── SettingsView                    ← preferences, units, about
```

### Navigation Bar (persistent, post-onboarding)
- Background: `#000000`
- Center: "bre:zi" logo — `ABCSocial-CondensedBold` / 18px / white (use "BREZI" in system font for prototype)
- Left: Context-dependent — back arrow when pushed, hamburger/nothing at root
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
│  Most people compromise their   │
│  coffee. Ice dilutes it. Hot    │
│  brew oxidizes it. You've been  │  ← Regular, 17px, #CCCCCC, line-height 1.5
│  drinking the workaround this   │
│  whole time.                    │
│                                 │
│                                 │
│  ┌─────────────────────────┐    │
│  │  START THE PRACTICE  →  │    │  ← blue filled, 54px tall, ALL CAPS
│  └─────────────────────────┘    │
│                                 │
└─────────────────────────────────┘
```

**Motion:** Headline fades up from 12px below (0.4s ease-out, 0.1s delay). Body fades in 0.3s after headline. Button fades in last (0.2s, 0.3s delay).

---

### Onboarding Screen 2 — Device Pairing (`DevicePairView`)

**Purpose:** Pair the Brezi machine via Bluetooth. This is the product.

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
│  Turn on your machine and       │
│  bring your phone close.        │  ← Regular, 17px, #CCCCCC
│  We'll find it.                 │
│                                 │
│  ┌─────────────────────────┐    │
│  │  SEARCH FOR BREZI       │    │  ← blue filled button
│  └─────────────────────────┘    │
│                                 │
│  I don't have one yet ›         │  ← text link, #888888, centered
│                                 │
└─────────────────────────────────┘
```

**States:**

**Searching:**
```
│  ┌─────────────────────────┐    │
│  │  ◌  SEARCHING...        │    │  ← blue button, spinner left of text
│  └─────────────────────────┘    │
│                                 │
│  Make sure Brezi is powered on  │  ← Mono, 12px, #888888
│  and within range.              │
```

**Found / Paired:**
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

**"I don't have one yet" → Skip pairing:**
- Store `breziPaired: false`
- Show toast: `"No problem. Pair anytime from Device Hub."` — Mono, 13px, white on dark
- Advance to Screen 3

**Bluetooth permission:** Request `CBCentralManager` authorization. If denied, show inline helper:
```
Bluetooth access needed to find your Brezi.
[Open Settings]    ← text link, #488BED
```

---

### Onboarding Screen 3 — Wearable Connect (`WearableConnectView`)

**Purpose:** Connect health data source. This makes the recommendation engine real.

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

**Purpose:** This is the key fix. Show the user exactly how their data becomes a brew. Make the engine transparent.

```
┌─────────────────────────────────┐
│  ■■■■■  ■■■■■  ■■■■■  ■■■■■  ───│
│                                 │
│  This is how Brezi thinks.      │  ← CondensedBold, 36px, white
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
│  │  GOT IT  →              │    │  ← blue filled
│  └─────────────────────────┘    │
│                                 │
└─────────────────────────────────┘
```

**Animation:** Cards appear sequentially top-to-bottom (0.3s stagger). Blue connecting lines draw downward between cards. Total sequence: ~1.5s.

**Content is example data** — not the user's real data yet. Use placeholder values that tell a clear story.

---

### Onboarding Screen 5 — First Check-in (`FirstCheckInView`)

**Purpose:** Get the first recovery input. Lands on the real dashboard.

```
┌─────────────────────────────────┐
│  ■■■■■  ■■■■■  ■■■■■  ■■■■■  ■■■│
│                                 │
│  How are you waking             │
│  up today?                      │  ← CondensedBold, 40px, white
│                                 │
│  No wrong answers. Brezi works  │
│  with whatever you've got.      │  ← Regular, 15px, #888888
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

**This is the main event.** The dashboard is three stacked cards — the chain made visible. Health data at top, Brezi's insight in the middle, brew recommendation at bottom. The user sees exactly why they're getting what they're getting.

**Entry:** Default root screen post-onboarding. Refreshed each morning or on pull-to-refresh.

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
│  │  │  ms  │ │      │ │ hrs │ │ │  ← Mono, 10px, #888888
│  │  └──────┘ └──────┘ └─────┘ │ │
│  │                             │ │
│  │  ↓ 18% vs your baseline    │ │  ← Mono, 12px, statusYellow
│  │  Source: WHOOP · 6:42am    │ │  ← Mono, 11px, #888888
│  │                             │ │
│  └─────────────────────────────┘ │  ← white card, shadow, 16px radius
│                                 │
│  ┌─ 2  BREZI'S READ ──────────┐ │
│  │                             │ │
│  │  Mid recovery. HRV dipped  │ │
│  │  but sleep was adequate.   │ │  ← Regular, 15px, black
│  │  Standard output — don't   │ │
│  │  push it.                  │ │
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

### Card 1 — Health Data Card (`HealthDataCardView`)

**Layout:**
- White card, shadow, 16px radius, 20px padding
- Three metric columns, equal width, center-aligned
- Each column: large value (t_data, 28px bold mono) + label (Mono 10px #888 ALL CAPS) + unit (Mono 10px #888)
- Dividers: 1px vertical `#E0E0E0` between columns
- Below metrics: trend line — `"↓ 18% vs your baseline"` colored by status (green/yellow/red)
- Source + timestamp: Mono 11px, #888888

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

**Layout:**
- White card, blue left border 3px, 16px radius, 16px padding
- 2–3 lines of natural language insight
- Font: Regular, 15px, black
- No CTA — this is read-only context

**Insight generation logic (rule-based v1):**
```swift
func insightText(hrv: Int?, baseline: Int?, recovery: Double?, sleep: Double?) -> String {
    // Recovery buckets with contextual language
    // High (7–10): "Strong recovery. Your system is ready for high output."
    // Mid (4–6): "Mid recovery. [specific dip reason]. Standard output — don't push it."
    // Low (1–3): "Your body's rebuilding. Low stimulus, high patience."
    // Include delta vs baseline when available: "HRV is X% [above/below] your 7-day average."
    // Include sleep context: "Sleep was [short/adequate/solid] at Xh Ym."
}
```

### Card 3 — Brew Card (`DashboardBrewCardView`)

**Layout:**
- Background: `#0F0F0F`, 24px radius
- Padding: 24px horizontal, 28px vertical
- Profile name: CondensedBold, 32px, white
- One-line description: Regular, 14px, #CCCCCC
- Settings row: 3 equal columns, center-aligned
  - Value: CondensedBold, 22px, white
  - Label: Mono, 10px, #666666, ALL CAPS
- **New expanded details:**
  - Bean recommendation: Mono, 13px, #888888 — `"Medium roast · Medium grind"`
  - Timing + serving: Mono, 13px, #488BED — `"1 serve · Best before 2pm"`
- Primary CTA: "START BREW" — white filled, black text, 48px tall
- "See full details" → pushes to `BrewRecommendationView`

**"START BREW" behavior:**
- If Brezi paired: sends brew parameters to machine via Bluetooth, shows brew progress
- If not paired: behaves as "LOG THIS BREW" — records to BrewLog

### Dashboard — Pull to Refresh
- Standard iOS pull-to-refresh
- Re-fetches wearable data, recalculates recommendation
- Loading: "Pulling your signal..." — Mono, 13px, #888888

### Dashboard — Empty State (first use, no wearable)
```
Card 1 shows recovery slider (manual mode).
Card 2: "Your practice starts blank. Give it three mornings."
Card 3: Shows recommendation based on manual input.
```

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
│  │  ─────────────────────────  ││  ← divider, #333333
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

### Expanded Brew Parameters (new in v2)

Each brew profile now includes:

| Parameter | Description | Display |
|---|---|---|
| Temp (°C) | Brew temperature | CondensedBold, 24px |
| Time | Brew duration (Brezi is 10–15min, not overnight) | CondensedBold, 24px |
| Dose (g/L) | Coffee-to-water ratio | CondensedBold, 24px |
| Roast | Recommended roast level | CondensedBold, 20px |
| Grind | Recommended grind size | CondensedBold, 20px |
| Serve | Number of servings | CondensedBold, 20px |
| Timing | Recommended consumption window | Mono, 13px, #488BED |

### "Why This Brew" Chain
The data→insight→brew chain from the tutorial, now populated with real user data. Three small stacked cards with blue connecting lines between them. This is the transparency Jackson asked for.

### "Adjust Settings" — Manual Override Sheet
- Modal half-sheet (`.presentationDetents([.medium])`)
- Sliders: Temp (°C), Time (min), Dose (g/L)
- Pickers: Roast (Light / Medium / Dark), Grind (Fine / Medium / Coarse)
- Live preview updates
- CTA: "APPLY & START BREW"
- Saving an override stores to `BrewLog.brewSettingsJSON`

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

### Updated Profile Specs (expanded for v2)
| Profile | Score | Temp | Time | Dose | Roast | Grind | Serve | Timing |
|---|---|---|---|---|---|---|---|---|
| Tide Pool | 1–4 | 4°C | 10 min | 40–50 g/L | Light–Med | Medium | 1 | Anytime |
| First Light | 5–6 | 4–8°C | 12 min | 60 g/L | Medium | Medium | 1 | Before 2pm |
| Deep Work | 7–8 | 10–15°C | 14 min | 80 g/L | Medium | Med-Coarse | 1 | Before 1pm |
| Voltage | 9–10 | 15–20°C | 15 min | 80 g/L | Med-Dark | Coarse | 1 | Before 12pm |
| The Blank | Manual | User | User | User | User | User | User | — |

---

## 5. History & Correlation (`HistoryView`)

**Entry:** "VIEW BREW HISTORY" from dashboard, or nav push.

```
┌─────────────────────────────────┐
│  ‹ Back            bre:zi    ⚙  │
│─────────────────────────────────│
│                                 │
│  YOUR PRACTICE                  │  ← Mono, 11px, #888888, ALL CAPS
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
│  ─ Empty State ─────────────    │
│  Nothing brewed yet.            │
│  Your data's ready.             │  ← Regular, 16px, #333, centered
│  Your machine's ready.          │
│  You're the last variable.      │
│                                 │
│  [See today's recommendation ›] │  ← blue text link
│                                 │
└─────────────────────────────────┘
```

### Streak Card (`StreakCardView`)
- Current consecutive days with a BrewLog entry
- Week view: 7 circles, M–S labels (Mono 10px #888)
- Brewed: filled circle `#000000`
- Today (not yet brewed): ring `#488BED`
- Future: ring `#E0E0E0`
- Today (brewed): filled `#488BED`

### Correlation Card (`CorrelationCardView`)
- Only appears after 5+ logged brews with ratings
- Simple scatter plot: recovery score (x) vs brew rating (y)
- Dots colored by profile (use status colors or profile-specific hue)
- One-line insight below chart — generated from correlation direction
- Before 5 brews: show placeholder:
```
"Log 5 rated brews to unlock your correlation chart."
← Mono, 13px, #888888
```

### Profile Mix Card (`ProfileMixView`)
- Horizontal stacked bar showing % breakdown of brew profiles used
- Legend below with profile name + percentage
- Only shows after 3+ brews

### Brew Log Cells
- Same as v1 but with expanded info:
  - Profile name + date
  - Brew settings (temp · time · dose)
  - Recovery score + roast used
  - Star rating (tap to edit)
- Background: white, border `1px #E0E0E0`, radius 12px, padding 16px
- Grouped by month, sorted newest first

---

## 6. Device Hub (`DeviceHubView`)

**Entry:** Antenna icon in nav bar. Manages all connected hardware. Designed to be extensible.

```
┌─────────────────────────────────┐
│  ‹ Back            bre:zi    ⚙  │
│─────────────────────────────────│
│                                 │
│  YOUR DEVICES                   │  ← Mono, 11px, #888888, ALL CAPS
│                                 │
│  ┌─ BREZI MACHINE ────────────┐ │
│  │                             │ │
│  │  ┌──┐  BREZI-A4F2          │ │  ← device icon + name
│  │  │☕│  Connected            │ │  ← Mono, 12px, #34C759
│  │  └──┘  Firmware 1.2.0      │ │  ← Mono, 11px, #888888
│  │                             │ │
│  │  Last brew: Today, 7:14am  │ │  ← Mono, 12px, #888888
│  │  Brews logged: 47          │ │
│  │                             │ │
│  │  [Disconnect]              │ │  ← text button, #FF453A
│  │                             │ │
│  └─────────────────────────────┘ │  ← white card
│                                 │
│  ┌─ WEARABLE ─────────────────┐ │
│  │                             │ │
│  │  ┌──┐  WHOOP 4.0           │ │
│  │  │⌚│  Connected            │ │  ← Mono, 12px, #34C759
│  │  └──┘  Last sync: 6:42am   │ │  ← Mono, 11px, #888888
│  │                             │ │
│  │  Pulling: HRV, Recovery,   │ │
│  │  Sleep duration             │ │  ← Mono, 12px, #888888
│  │                             │ │
│  │  [Disconnect]              │ │
│  │                             │ │
│  └─────────────────────────────┘ │  ← white card
│                                 │
│  ─ ADD DEVICE ──────────────    │
│                                 │
│  ┌─────────────────────────────┐│
│  │  + Pair another Brezi       ││  ← add row
│  └─────────────────────────────┘│
│  ┌─────────────────────────────┐│
│  │  + Connect wearable         ││  ← opens wearable picker (same as
│  └─────────────────────────────┘│     onboarding screen 3)
│                                 │
│  ─ COMING SOON ─────────────    │  ← Mono, 11px, #488BED
│                                 │
│  ┌─────────────────────────────┐│
│  │  ┌──┐  Brezi Blender       ││
│  │  │🫙│  Health-responsive    ││  ← Regular, 14px, #888888
│  │  └──┘  smoothies.          ││
│  │        Notify me ›         ││  ← text link, #488BED
│  └─────────────────────────────┘│  ← white card, dashed border #E0E0E0
│                                 │
└─────────────────────────────────┘
```

### Device Card Spec (`DeviceCardView`)
- White card, 16px radius, shadow, 20px padding
- Icon: 40px, left-aligned (SF Symbols for prototype)
  - Brezi: `cup.and.saucer.fill`
  - WHOOP: `applewatch` (placeholder)
  - Oura: `circle.circle` (placeholder)
  - Apple Health: `heart.fill`
  - Brezi Blender: `blender.fill` (or fallback)
- Device name: CondensedBold, 18px, black
- Status: Mono, 12px — connected (#34C759), disconnected (#FF453A), syncing (#488BED)
- Metadata: Mono, 11px, #888888

### Coming Soon — Brezi Blender
- Dashed border card (`#E0E0E0`, 1px, dash pattern 6-3)
- Opacity 0.8
- "Notify me" stores email/flag for future marketing push
- **Extensibility note for Jensen:** The recommendation engine's output type should be an enum:
```swift
enum RecommendationType {
    case brew(BrewProfile)
    case blend(BlendProfile)    // future
}
```
The dashboard's Card 3 renders based on this type. Same health data, same insight engine, different output card.

---

## 7. Settings (`SettingsView`)

```
┌─────────────────────────────────┐
│  ‹ Back            bre:zi    ⚙  │
│─────────────────────────────────│
│                                 │
│  SETTINGS                       │  ← Mono, 11px, #888888
│                                 │
│  ─ Devices ──────────────────   │
│  Brezi Machine    Connected  ›  │  ← pushes to DeviceHubView
│  Wearable         WHOOP     ›  │
│                                 │
│  ─ Brew Preferences ─────────   │
│  Default roast     Medium   ›  │  ← picker
│  Default grind     Medium   ›  │  ← picker
│  Caffeine cutoff   2:00 PM  ›  │  ← time picker
│  Serving size      1        ›  │  ← stepper (1–3)
│                                 │
│  ─ Units ────────────────────   │
│  Temperature       Celsius  ›  │  ← toggle C/F
│  Weight            Grams    ›  │  ← toggle g/oz
│                                 │
│  ─ Notifications ────────────   │
│  Morning check-in  [ON]        │  ← toggle
│  Check-in time     7:00 AM ›  │  ← time picker
│  Brew reminders    [ON]        │  ← toggle
│                                 │
│  ─ Data ─────────────────────   │
│  Export brew history        ›  │  ← exports CSV
│  Reset all data             ›  │  ← destructive, confirmation alert
│                                 │
│  ─ About ────────────────────   │
│  Version            0.2.0      │
│  Built by Brezi                │
│                                 │
└─────────────────────────────────┘
```

### New in v2 Settings:
- **Brew Preferences:** Default roast, grind, caffeine cutoff time, serving size — these feed into the recommendation engine and brew card display
- **Caffeine cutoff:** Profiles recommended after this time will shift one tier gentler (Deep Work → First Light, Voltage → Deep Work)
- **Export:** Generates CSV of all BrewLog entries — date, profile, settings, recovery score, rating

---

## 8. Loading / Transition States

### "Reading your signal..."
Post check-in, calculating recommendation.
```
Black background, full screen.
"Reading your signal..."     ← Mono, 16px, white, centered
Subtle pulse (opacity 1.0 ↔ 0.5, 1.2s loop)
Duration: 0.8s max → push to DashboardView
```

### Brew Started Confirmation
After "START BREW" tapped (paired device):
```
Full-screen overlay, black, 0.3s fade-in.
Machine icon animating (subtle pulse).
"Brewing First Light..."     ← Mono, 16px, white
Progress bar: blue fill, 0→100% over brew duration
"4–8°C · 12 minutes"        ← Mono, 13px, #888888
[Cancel Brew]                ← text link, #FF453A
```

On completion:
```
Checkmark (SF: checkmark.circle.fill) #488BED, 48pt
"Brewed."                    ← Mono, 14px, #888888
"Rate this brew?"            ← Regular, 15px, white
[★ ★ ★ ★ ★]                 ← RatingStarsView, large (32px)
[Skip rating]                ← text link, #888888
```

### Brew Logged Confirmation (no device)
After "LOG THIS BREW":
```
Checkmark #488BED, 48pt
"Logged."                    ← Mono, 14px, #888888
Auto-dismiss 1.2s → back to Dashboard
```

---

## 9. Empty States

| Screen | State | Copy |
|---|---|---|
| Dashboard — first use | No wearable, no history | "Your practice starts blank. Log your first recovery score — give it three mornings." |
| Dashboard — no brew today | Has history, not yet brewed | "Nothing brewed yet today. Your data's ready. Your machine's ready. You're the last variable." |
| History — no brews | First visit | "Nothing brewed yet. Your data's ready. Your machine's ready. You're the last variable." |
| History — no correlation | < 5 rated brews | "Log 5 rated brews to unlock your correlation chart." |
| Device Hub — no Brezi | Skipped pairing | "No Brezi paired. Your recommendations still work — you'll just log manually. [Pair now ›]" |
| Device Hub — no wearable | Skipped wearable | "Flying manual right now. That's fine. Connect Oura or WHOOP when you're ready." |
| Settings — wearable | Not connected | "Connect a wearable and Brezi does the logging for you. [Connect ›]" |

---

## 10. Component Library

| Component | File | Used in |
|---|---|---|
| `HealthDataCardView` | HealthDataCardView.swift | Dashboard |
| `InsightStripView` | InsightStripView.swift | Dashboard, Brew Recommendation |
| `DashboardBrewCardView` | DashboardBrewCardView.swift | Dashboard |
| `DataChainView` | DataChainView.swift | Onboarding 4, Brew Recommendation |
| `RecoverySliderView` | RecoverySliderView.swift | Dashboard (manual mode), Onboarding 5 |
| `QuickSetChipRow` | QuickSetChipRow.swift | Dashboard (manual mode) |
| `ProfileCardView` | ProfileCardView.swift | Brew Recommendation, Profile Picker |
| `BrewSettingsRow` | BrewSettingsRow.swift | Brew Recommendation |
| `ExpandableSection` | ExpandableSection.swift | Brew Recommendation |
| `BrewLogCell` | BrewLogCell.swift | History |
| `RatingStarsView` | RatingStarsView.swift | History, Brew Confirmation |
| `StreakCardView` | StreakCardView.swift | History |
| `CorrelationCardView` | CorrelationCardView.swift | History |
| `ProfileMixView` | ProfileMixView.swift | History |
| `DeviceCardView` | DeviceCardView.swift | Device Hub |
| `WearableRowView` | WearableRowView.swift | Onboarding 3, Device Hub |
| `OnboardingSelectionCard` | OnboardingSelectionCard.swift | Onboarding 5 |
| `SaveButton` | SaveButton.swift | All screens |
| `EmptyStateView` | EmptyStateView.swift | All screens |
| `SegmentedProgressBar` | SegmentedProgressBar.swift | Onboarding shell |

---

## 11. Data Model Updates (for Jensen)

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
- createdAt: Date
```

**BrewProfile** (updated — add new fields)
```swift
// Existing fields unchanged, add:
- recommendedRoast: String    // "light", "medium", "medium-dark", "dark"
- recommendedGrind: String    // "fine", "medium", "medium-coarse", "coarse"
- defaultServings: Int16      // 1
- caffeineCutoffHour: Int16   // 14 = 2pm, 13 = 1pm, 0 = no cutoff
- brewTimeMinutes: Int16      // actual Brezi machine time (not overnight steep)
```

**BrewLog** (updated)
```swift
// Existing fields unchanged, add:
- roastUsed: String?         // actual roast used
- grindUsed: String?         // actual grind used
- servings: Int16            // number of servings
- deviceId: UUID?            // which Brezi machine brewed it
```

### Insight Engine (new: `InsightEngine.swift`)
```swift
struct HealthSignal {
    let recoveryScore: Int       // 1–10 (normalized)
    let hrv: Double?             // ms
    let baselineHrv: Double?     // 7-day avg
    let recoveryPercent: Double? // 0–100
    let sleepHours: Double?
    let source: String
}

struct BrewInsight {
    let profileKey: String
    let insightText: String      // natural language explanation
    let dataPoints: [String]     // formatted data for display
    let confidence: Double       // 0–1, higher with more data
}

func generateInsight(from signal: HealthSignal) -> BrewInsight
```

---

## 12. Build Order (recommended for Jensen)

1. **Day 1–2:** Project updates — add new Core Data entities (Device, WearableConnection), update CheckIn + BrewProfile + BrewLog with new fields. Seed updated profiles with roast/grind/timing data. Build `InsightEngine` with rule-based logic.

2. **Day 3–4:** `HealthDataCardView` + `InsightStripView` + `DashboardBrewCardView` + `DashboardView`. Wire up manual recovery input path first. This is the core loop.

3. **Day 5–6:** `BrewRecommendationView` with full detail — brew card, "Why This Brew" data chain, adjust settings sheet, profile picker modal.

4. **Day 7–8:** `HistoryView` — `StreakCardView`, `BrewLogCell` list, `CorrelationCardView` (scatter plot), `ProfileMixView` (stacked bar). Rating interaction.

5. **Day 9–10:** Onboarding flow — 5 screens. `BrandIntroView`, `DevicePairView` (Bluetooth stub — mock pairing for TestFlight), `WearableConnectView` (Apple Health real, WHOOP/Oura stubs), `DataChainTutorialView`, `FirstCheckInView`.

6. **Day 11:** `DeviceHubView` — device cards, add/disconnect flows, "Coming Soon" Blender placeholder.

7. **Day 12–13:** `SettingsView` with brew preferences, notification scheduling, data export. Polish transitions and loading states.

8. **Day 14:** End-to-end testing, TestFlight prep, bug sweep.

**TestFlight target: 14 working days from start.**

---

## 13. Decisions — Locked by Jackson (March 19)

1. **System fonts for prototype.** ABC Social swapped in pre-launch.
2. **SF Symbols for all icons.** Custom icons post-MVP.
3. **Device pairing in onboarding.** Bluetooth permission requested at Screen 2. Skippable.
4. **Wearable connect in onboarding.** Apple Health is real integration for v0.2. WHOOP/Oura are stubs (UI present, OAuth not wired). Mark "Coming soon" where needed.
5. **Data chain visible everywhere.** Dashboard shows 3-card stack. Brew detail shows full chain. This is the product.
6. **Expanded brew recommendation.** Every profile now includes: roast, grind, servings, timing window.
7. **Caffeine cutoff logic.** After user's set cutoff time, recommendation shifts one tier gentler.
8. **Core Data iCloud sync from day one.** `NSPersistentCloudKitContainer`.
9. **Morning push notification.** Trigger day after first BrewLog: `"Your signal's ready. Time to check in."` Request `UNUserNotificationCenter` on first brew log.
10. **Extensible architecture.** `RecommendationType` enum supports future Blender product. Device Hub shows "Coming Soon" slot.

---

## 14. What Changed from v1 → v2

| Area | v1 | v2 | Why |
|---|---|---|---|
| Home screen | Morning check-in form | 3-card dashboard (data + insight + brew) | The chain IS the product — show it |
| Recommendation | Brew name + settings | Full detail: roast, grind, servings, timing, data chain | Jackson: "expand recommendation" |
| Data transparency | "Recommended because score was 5" | Explicit 3-step chain: data → insight → brew | Jackson: "make data→insight→brew EXPLICIT" |
| Onboarding | Brand copy × 3 → input | Device pair → Wearable → Tutorial → Input | Jackson: "wrong onboarding order" |
| Device management | Settings toggle (disabled) | Dedicated Device Hub screen | Needed for Bluetooth pairing + extensibility |
| History | Flat brew log list | Streak + correlation chart + profile mix + log | Pattern recognition = retention |
| Extensibility | Coffee only | `RecommendationType` enum, Blender placeholder | Jackson: "extensible for Brezi Blender" |
| Brew profiles | Name + temp/steep/ratio | + roast, grind, servings, caffeine timing | Jackson: "bean origin, grind, quantity, timing" |

---

*Spec written March 19, 2026. Lock design tokens before Day 1. Profile data changes must update both this spec and `jensen-tech-spec.md` simultaneously. The chain is the product. If the user can't see why, we failed.*
