# Brezi iOS App — Wireframe & Design Spec v1.0
**Status:** Ready for dev handoff | March 2026 | For Jensen

---

## 0. Design Tokens (lock these before touching Xcode)

### Colors
```swift
// Brand palette — use these exact values everywhere
let black       = Color(hex: "#000000")  // primary text, backgrounds
let white       = Color(hex: "#FFFFFF")  // card surfaces, inputs
let offWhite    = Color(hex: "#F5F5F5")  // app background
let blue        = Color(hex: "#488BED")  // accent, CTAs, active states
let midGrey     = Color(hex: "#888888")  // secondary text, labels
let lightGrey   = Color(hex: "#E0E0E0")  // dividers, borders
let darkCard    = Color(hex: "#0F0F0F")  // brew card background
let darkSurface = Color(hex: "#1A1A1A")  // secondary dark surfaces
```

### Typography
**Use whatever font feels right for now — brand font (ABC Social) will be swapped in before public launch.**

Suggested system defaults for prototype:
- Headlines / profile names: `SF Pro Display` — `.bold` or `.heavy`, condensed where available
- Body copy: `SF Pro Text` — `.regular`
- Numbers / labels / mono data: `SF Mono` — `.regular`

```swift
// Type scale — swap font names to ABC Social before launch
let t_display  = Font.system(size: 48, weight: .heavy, design: .default)   // hero headlines
let t_h1       = Font.system(size: 34, weight: .bold,  design: .default)   // screen titles
let t_h2       = Font.system(size: 26, weight: .bold,  design: .default)   // card titles
let t_h3       = Font.system(size: 20, weight: .semibold, design: .default) // section headers
let t_body     = Font.system(size: 16, weight: .regular, design: .default) // body text
let t_small    = Font.system(size: 14, weight: .regular, design: .default) // secondary text
let t_mono     = Font.system(size: 13, weight: .regular, design: .monospaced) // data values
let t_eyebrow  = Font.system(size: 11, weight: .medium,  design: .monospaced) // labels
```

### Spacing (8pt grid)
```
xs: 4    sm: 8    md: 16    lg: 24    xl: 32    xxl: 48    xxxl: 64
```

### Corner Radii
```
card: 16px    button: 12px    chip: 8px    input: 10px
```

### Shadows
```swift
// Card shadow
.shadow(color: .black.opacity(0.08), radius: 16, x: 0, y: 4)
// No shadow on dark cards
```

---

## 1. App Shell

### Navigation Pattern
Single push-stack. No tab bar in v0.1.

```
AppRoot
 └── OnboardingFlow (shown once on first launch)
      └── MainFlow
           ├── MorningCheckInView           ← default landing
           ├── BrewRecommendationView        ← pushed after check-in
           ├── BrewLogView                   ← pushed from nav
           └── SettingsView                  ← pushed from nav
```

### Navigation Bar (persistent, post-onboarding)
- Background: `#000000`
- Logo: "BREZI" in `ABCSocial-CondensedBold` / 18px / white — centered
- Right: Settings icon (SF Symbol: `gearshape`) — white, 20pt
- Left: Back arrow (only when pushed) — white
- No bottom tab bar

---

## 2. Onboarding Flow

**Trigger:** First launch only. Store `hasCompletedOnboarding: Bool` in UserDefaults. Skip if true.

### Shared Onboarding Shell
- Background: `#000000` full bleed
- Progress dots: 4 dots, bottom center, 8px diameter, `#488BED` active / `#333` inactive
- All text: white
- Back swipe: enabled (system gesture)
- Skip: text button top-right, `#888888`, "Skip" — jumps to Screen 4 (input)

---

### Onboarding Screen 1 — The Hook

```
┌─────────────────────────────┐
│                        Skip │  ← top-right, grey
│                             │
│  ●  ○  ○  ○                 │  ← progress dots, top-left
│                             │
│                             │
│                             │
│  Cold is                    │
│  a practice.                │  ← ABCSocial-CondensedBold, 52px, white
│                             │
│  Most people compromise     │
│  their coffee. Ice dilutes  │
│  it. Hot brew oxidizes it.  │
│  You've been drinking the   │  ← Regular, 17px, #CCCCCC, line-height 1.5
│  workaround this whole time.│
│                             │
│                             │
│  ┌─────────────────────┐    │
│  │ Start the practice → │    │  ← blue filled button, 54px tall
│  └─────────────────────┘    │
│                             │
└─────────────────────────────┘
```

**Motion:** Headline fades up from 12px below on appear (0.4s ease-out, 0.1s delay). Body fades in 0.3s after headline settles. Button fades in last (0.2s, 0.3s delay after body).

---

### Onboarding Screen 2 — The Why

```
┌─────────────────────────────┐
│                        Skip │
│                             │
│  ●  ●  ○  ○                 │
│                             │
│  12 minutes.                │
│  No ice. No shortcuts.      │  ← CondensedBold, 48px, white
│                             │
│  Brezi brews at cold        │
│  temperature — not          │
│  chilled-after-the-fact.    │
│  The difference isn't       │  ← Regular, 17px, #CCCCCC
│  subtle.                    │
│                             │
│  Your machine. Your data.   │
│  Your brew.                 │  ← Mono, 13px, #488BED
│                             │
│  ┌─────────────────────┐    │
│  │    Makes sense →     │    │  ← blue button
│  └─────────────────────┘    │
└─────────────────────────────┘
```

---

### Onboarding Screen 3 — The Data Ask

```
┌─────────────────────────────┐
│                        Skip │
│  ●  ●  ●  ○                 │
│                             │
│  Your coffee should         │
│  know how you slept.        │  ← CondensedBold, 44px, white
│                             │
│  Log your HRV and recovery  │
│  score each morning. Brezi  │
│  uses it to recommend the   │
│  right brew for the day.    │  ← Regular, 17px, #CCCCCC
│                             │
│  Connect Oura or WHOOP      │
│  later. For now, just tell  │
│  us how you feel.           │  ← Mono, 13px, #888888
│                             │
│  ┌─────────────────────┐    │
│  │ I'm in — calibrate → │   │  ← blue button
│  └─────────────────────┘    │
└─────────────────────────────┘
```

---

### Onboarding Screen 4 — First Input (leads into main app)

```
┌─────────────────────────────┐
│  ●  ●  ●  ●                 │
│                             │
│  How are you waking         │
│  up today?                  │  ← CondensedBold, 40px, white
│                             │
│  No wrong answers. Brezi    │
│  works with whatever        │
│  you've got.                │  ← Regular, 15px, #888888
│                             │
│  ┌────────────────────────┐ │
│  │  🟢  Peaked            │ │  ← selection card (see spec below)
│  │  HRV high, slept clean │ │
│  └────────────────────────┘ │
│  ┌────────────────────────┐ │
│  │  🟡  Functional        │ │
│  │  Decent night          │ │
│  └────────────────────────┘ │
│  ┌────────────────────────┐ │
│  │  🔴  Rebuilding        │ │
│  │  Paying for yesterday  │ │
│  └────────────────────────┘ │
│                             │
│  ┌─────────────────────┐    │
│  │ Build my first brew →│   │  ← blue, active after selection
│  └─────────────────────┘    │
└─────────────────────────────┘
```

**Selection Card spec:**
- Default: border `#333`, background `#1A1A1A`, 16px radius, 20px padding
- Selected: border `#488BED` 2px, background `rgba(72,139,237,0.12)`, slight scale(1.01) animation
- Label: `ABCSocial-CondensedBold` 20px white
- Sublabel: `ABCSocial-Regular` 14px `#888888`
- Emoji: 28px, left-aligned with 16px leading margin

**Onboarding → App Transition:** After "Build my first brew" tapped, show loading state:
```
"Reading your signal..."    ← Mono, 14px, centered, fade in
```
Then push to BrewRecommendationView with profile pre-calculated.
Mark `hasCompletedOnboarding = true`.

---

## 3. Screen 1 — Morning Check-in (`MorningCheckInView`)

**Entry point:** Default root screen post-onboarding. Presented fresh each morning.

```
┌─────────────────────────────┐
│ ≡                  BREZI  ⚙ │  ← nav bar (black bg)
│─────────────────────────────│
│                             │
│  MORNING CHECK-IN           │  ← Mono, 11px, #888888, all-caps, letter-spaced
│                             │
│  How recovered              │
│  do you feel?               │  ← CondensedBold, 38px, black
│                             │
│  ┌─────────────────────┐    │
│  │       [  5  ]        │    │  ← large numeric readout, 72px, CondensedBold
│  │   ○──────●──────○    │    │  ← custom slider (see spec)
│  │   1               10 │    │  ← Mono, 12px, #888888
│  └─────────────────────┘    │
│                             │
│  Quick set:                 │  ← Regular, 13px, #888888
│  ┌───────┐┌───────┐┌──────┐ │
│  │  1–3  ││  4–6  ││  7–10│ │  ← chip buttons (see spec)
│  │  Rest ││  OK   ││  High│ │
│  └───────┘└───────┘└──────┘ │
│                             │
│  ─────────────────────────  │  ← divider, #E0E0E0
│                             │
│  Last check-in: Yesterday   │  ← Mono, 12px, #888888
│  Score: 7 · First Light     │  ← Mono, 12px, #488BED
│                             │
│  ┌─────────────────────┐    │
│  │  Get Recommendation  │    │  ← blue filled, 54px tall, full width
│  └─────────────────────┘    │
│                             │
│  Skip today ›               │  ← small text link, #888888, centered
│                             │
└─────────────────────────────┘
```

**Slider spec (`RecoverySliderView`):**
- Track: `#E0E0E0` inactive, `#488BED` active (left of thumb)
- Track height: 6px, rounded
- Thumb: 28px circle, `#FFFFFF` fill, `#488BED` 2px border, shadow
- Haptic: `UIImpactFeedbackGenerator(.light)` on each integer tick
- Value display: large number above slider, updates live
- Range: 1–10, integer steps only

**Quick-set chip spec:**
- Background: `#F0F0F0`, border none, radius 8px
- Selected: background `#488BED`, text white
- Font: `ABCSocial-Mono` 13px
- Height: 36px, padding: 0 16px
- Tap: sets slider to midpoint of range (1–3 → 2, 4–6 → 5, 7–10 → 8)

**Last check-in row:** only shows if previous BrewLog exists. Hidden on first use; show empty state instead:
```
"Your practice starts blank. Give it three mornings."
← Regular, 14px, #888888, centered
```

---

## 4. Screen 2 — Brew Recommendation (`BrewRecommendationView`)

**Entry:** Pushed from MorningCheckInView after "Get Recommendation". Receives `recoveryScore: Int`.

```
┌─────────────────────────────┐
│ ‹ Back             BREZI  ⚙ │
│─────────────────────────────│
│                             │
│  ┌─────────────────────────┐│
│  │                         ││
│  │  TODAY'S BREW           ││  ← Mono, 11px, #488BED, all-caps
│  │                         ││
│  │  FIRST LIGHT            ││  ← CondensedBold, 42px, white
│  │                         ││
│  │  Not a recovery day.    ││
│  │  Not a peak day. Just   ││
│  │  a morning that needs   ││  ← Regular, 16px, #CCCCCC
│  │  to begin correctly.    ││
│  │                         ││
│  │  ─────────────────────  ││
│  │                         ││
│  │  TEMP    STEEP   RATIO  ││  ← Mono, 10px, #666666, all-caps
│  │  4–8°C   12–14h  60g/L  ││  ← CondensedBold, 22px, white
│  │                         ││
│  │  ┌─────────────────┐    ││
│  │  │  Log this brew ✓│    ││  ← white filled, black text, 48px tall
│  │  └─────────────────┘    ││
│  │                         ││
│  └─────────────────────────┘│  ← dark card (#0F0F0F), 24px radius, full-bleed top
│                             │
│  Recommended because your   │
│  recovery score was 5.      │  ← Regular, 14px, #888888
│                             │
│  ▼ Why this brew?           │  ← expandable section
│                             │
│  ─────────────────────────  │
│                             │
│  ┌─────────────────────┐    │
│  │   Adjust settings   │    │  ← outlined button, black border
│  └─────────────────────┘    │
│                             │
│  Choose a different brew ›  │  ← text link, #888888
│                             │
└─────────────────────────────┘
```

**Brew card spec (`ProfileCardView`):**
- Background: `#0F0F0F`
- Top-left to top-right: full bleed (no horizontal margin from screen edge)
- Bottom: 24px radius only (flat top corners on the card = clips to screen edge)
- Padding inside card: 32px horizontal, 36px vertical
- Settings row: 3 columns equal-width, center-aligned, divider dots between

**"Why this brew?" expandable:**
- Collapsed: chevron down + label in `Mono 13px #888888`
- Expanded: shows full brew profile rationale text (`Regular 15px #333`)
- Animation: `.animation(.easeInOut(duration: 0.25))`

**"Adjust settings" → manual override sheet:**
- Modal half-sheet (`.presentationDetents([.medium])`)
- Three sliders: Temp (°C), Steep time (hours), Ratio (g/L)
- Shows live preview of changes
- CTA: "Apply & Log"

**"Choose a different brew" → profile picker:**
- Full-screen modal
- List of all 5 profiles as cards (dimmed if not recommended)
- Each card shows: name, score range, one-line rationale
- Tap to select → replaces current recommendation

**Profile → score mapping (for `profileKey(for:)`):**
```swift
case 1...2:  return "tide_pool"     // Tide Pool (deep rest)
case 3...4:  return "tide_pool"     // Tide Pool
case 5...6:  return "first_light"   // First Light
case 7...8:  return "deep_work"     // Deep Work
case 9...10: return "voltage"       // Voltage
// Manual: "the_blank"
```

---

## 5. Screen 3 — Brew Log (`BrewLogView`)

**Entry:** Nav button from top-right (icon: `list.bullet.clipboard`) or after logging a brew.

```
┌─────────────────────────────┐
│ ‹ Back             BREZI  ⚙ │
│─────────────────────────────│
│                             │
│  YOUR BREWS                 │  ← Mono, 11px, #888888, all-caps
│                             │
│  ─ March 2026 ──────────    │  ← Mono, 12px, grouped by month
│                             │
│  ┌─────────────────────────┐│
│  │  FIRST LIGHT   Mar 19   ││  ← CondensedBold 17px + Mono 12px #888888
│  │  4–8°C · 12h · 60g/L   ││  ← Mono, 13px, #666666
│  │  Recovery: 5            ││  ← Mono, 12px, #888888
│  │  ★ ★ ★ ☆ ☆              ││  ← rating (tap to edit)
│  └─────────────────────────┘│
│  ┌─────────────────────────┐│
│  │  VOLTAGE       Mar 18   ││
│  │  15–20°C · 14h · 80g/L ││
│  │  Recovery: 9            ││
│  │  ★ ★ ★ ★ ★              ││
│  └─────────────────────────┘│
│                             │
│  ─ Empty state (first use) ─│
│  Nothing brewed yet.        │
│  Your data's ready.         │  ← Regular, 16px, #333, centered
│  Your machine's ready.      │
│  You're the last variable.  │
│                             │
│  [See today's recommendation]│ ← blue text link
│                             │
└─────────────────────────────┘
```

**Brew log cell spec:**
- Background: `#FFFFFF`, border `1px #E0E0E0`, radius 12px
- Padding: 16px
- Profile name: `ABCSocial-CondensedBold` 17px black
- Date: `ABCSocial-Mono` 12px `#888888` — right-aligned
- Settings summary: `ABCSocial-Mono` 13px `#666666`
- Recovery score: `ABCSocial-Mono` 12px `#888888`
- Star rating: `RatingStarsView` — tap any star to rate / re-rate; saves to `BrewLog.howFelt`

**Grouping:** Section headers by month/year, `ABCSocial-Mono` 12px `#888888`.

**Sort:** Newest first.

---

## 6. Screen 4 — Settings (`SettingsView`)

**Entry:** Gear icon in nav bar. v0.1 is a placeholder — minimal.

```
┌─────────────────────────────┐
│ ‹ Back             BREZI  ⚙ │
│─────────────────────────────│
│                             │
│  SETTINGS                   │  ← Mono, 11px, #888888
│                             │
│  ─ Wearable ─────────────   │
│  Oura Ring         ○ [OFF]  │  ← disabled toggle, greyed
│  WHOOP             ○ [OFF]  │  ← disabled toggle, greyed
│  "Coming soon"              │  ← Mono, 11px, #488BED
│                             │
│  ─ Preferences ──────────   │
│  Units             Metric ›  │
│                             │
│  ─ About ────────────────   │
│  Version           0.1.0    │
│  Built by Brezi             │
│                             │
└─────────────────────────────┘
```

---

## 7. Loading / Transition States

### "Reading your signal..."
Used when calculating recommendation (post check-in).

```
Black background, full screen.
"Reading your signal..."     ← Mono, 16px, white, centered
Subtle pulse animation on text (opacity 1.0 ↔ 0.5, 1.2s loop)
Max duration: 0.8s then push to BrewRecommendationView
```

### Brew log save confirmation
After "Log this brew" tapped:
- Checkmark (SF Symbol: `checkmark.circle.fill`) in `#488BED`, 48pt
- One-line text below: `"Logged."` ← `ABCSocial-Mono` 14px `#888888`
- Auto-dismiss after 1.2s, pop back to MorningCheckInView

---

## 8. Empty States (copy from CMO brief)

| Screen | Empty state copy |
|---|---|
| Brew Log (no history) | "Nothing brewed yet. Your data's ready. Your machine's ready. You're the last variable." |
| Check-in (first use) | "Your practice starts blank. Log your first recovery score — give it three mornings. You'll see why this matters." |
| Settings / Wearables | "Flying manual right now. That's fine. Connect Oura or WHOOP when you're ready." |

---

## 9. Component Library Summary

| Component | File | Used in |
|---|---|---|
| `RecoverySliderView` | RecoverySliderView.swift | Screen 1 |
| `QuickSetChipRow` | QuickSetChipRow.swift | Screen 1 |
| `ProfileCardView` | ProfileCardView.swift | Screen 2, Onboarding 4 |
| `BrewSettingsRow` | BrewSettingsRow.swift | Screen 2 |
| `ExpandableSection` | ExpandableSection.swift | Screen 2 |
| `BrewLogCell` | BrewLogCell.swift | Screen 3 |
| `RatingStarsView` | RatingStarsView.swift | Screen 3 |
| `SaveButton` | SaveButton.swift | All screens |
| `OnboardingSelectionCard` | OnboardingSelectionCard.swift | Onboarding 4 |
| `EmptyStateView` | EmptyStateView.swift | Screens 2, 3 |

---

## 10. Build Order (recommended for Jensen)

1. **Day 1–2:** Project setup — embed fonts, define Color/Font extensions, set up Core Data stack, seed 5 brew profiles, write `profileKey(for:)` logic
2. **Day 3–4:** `RecoverySliderView` + `QuickSetChipRow` + `MorningCheckInView` (no data layer yet, mock)
3. **Day 5–6:** `ProfileCardView` + `BrewRecommendationView` + "Log this brew" → `BrewLog` Core Data write
4. **Day 7:** Wire up `MorningCheckIn` → `BrewRecommendation` with live data; test end-to-end flow
5. **Day 8–9:** `BrewLogView` (list + rating)
6. **Day 10:** Onboarding flow (4 screens, `hasCompletedOnboarding` flag)
7. **Day 11–12:** `SettingsView` placeholder, polish, TestFlight prep

**TestFlight target: 12 working days from start**

---

## 11. Decisions — locked by Jackson (March 19)

1. ~~Font files~~ — **use system fonts for now, brand fonts swapped in pre-launch**
2. **Profile icons: SF Symbols** — use throughout, custom icons post-MVP
3. **Morning push notification: YES** — trigger day after a BrewLog is written: `"Your signal's ready. Time to check in."` (Mono tone). Request `UNUserNotificationCenter` permission on first brew log.
4. **Core Data iCloud sync: v0.1** — use `NSPersistentCloudKitContainer` from day one

---

*Spec written March 2026. Lock design tokens before Day 1. Any changes to profile names or score thresholds must update both this spec and `jensen-tech-spec.md` simultaneously.*
