# Brezi iOS App — Visual Coffee Layer & Marketplace v3.0
**Author:** Picasso (Product Design) | March 20, 2026 | Iteration 3 — Round 3
**For:** Jackson (Product) · Jensen (Engineering) · Elon (Growth/Copy)
**Status:** FINAL. Complete screen-by-screen specs incorporating all Round 2 feedback and Elon's critique.

---

## 0. What Changed Since Round 2

Round 2 gave the app a soul. Round 3 sharpens the edges. Nothing structural moves.

| Change | Why |
|--------|-----|
| **Adaptation note → hinge position** | Elon's principle: meaning before metrics. Moved inside parameter card between 3-box summary and detail table. Gets warm background chip. |
| **Price TBD banner** | When all visible beans have no price, marketplace shows "These beans are on deck" frame instead of a wall of NOTIFY ME buttons. |
| **Toast includes bean name** | "We'll let you know when Dark Field drops." — specificity signals real notification. |
| **Subscribe banner qualifier** | "Available on select beans." — honest framing when only Regent supports subscriptions at launch. |
| **WhyTodayConfig struct** | Animation timing is tunable via config, not hardcoded. TestFlight can adjust without code changes. |
| **Nav icon: cart → bag** | No in-app cart exists. "bag" signifies "shop here" without implying cart state. Internal rename to marketplace. |
| **Subscribed state on Subscribe button** | "SUBSCRIBED ✓" disabled state when subscription is active. UserDefaults flag for MVP. |
| **Lead tasting note accent in wireframes** | Was in spec but not rendered in wireframes. Now explicitly shown. |
| **First Light copy tightened** | line2 shortened. "Nothing to prove." moves to direction line where it hits harder. |
| **Gradient banding quality gate** | Spec note for Jensen: test 5-stop gradients on LCD devices, fall back to 3 stops if banding visible. |
| **Localizable.strings in Phase 1** | All ~85 coffee-side strings go into localization infrastructure from day one. |

**What this document covers:** Complete screen-by-screen specs for all 6 screens in the coffee-side flow, plus marketplace, plus all components, copy, data models, and animations.

**What this does NOT touch:** Onboarding (Screens 1-5), health data card (Card 1), Brezi's Read (Card 2), device hub, settings (except new rows), all R3/R4/R5 health-side design.

---

## 1. Dual Palette System — The Two Worlds

### The Principle

The app is two brands sharing one shell. The health side is a wearable dashboard. The coffee side is a specialty roaster's shelf. They meet at one seam — Why Today. Everything above the seam is cool. Everything below is warm.

### Health Side Palette (unchanged from R3)

```swift
// HEALTH SIDE — cool, precise, WHOOP-like
let health_bg             = Color(hex: "#000000")
let health_surface        = Color(hex: "#0F0F0F")
let health_card           = Color(hex: "#1A1A1A")
let health_text_primary   = Color(hex: "#FFFFFF")
let health_text_secondary = Color(hex: "#888888")
let health_accent         = Color(hex: "#488BED")
let health_status_green   = Color(hex: "#34C759")
let health_status_yellow  = Color(hex: "#FFD60A")
let health_status_red     = Color(hex: "#FF453A")
```

**Where it lives:** Dashboard Cards 1-2 (Your Data, Brezi's Read), onboarding, device hub, settings, all health metrics.

**Dashboard background:** `#F5F5F5` (cool neutral grey). Cards float dark on this light background. This is intentional — dark cards on cool grey gives the health side its clinical, precise feel. The transition to the coffee world is a *temperature* change (cool grey → warm cream), not just a lightness change.

### Coffee Side Palette

```swift
// COFFEE SIDE — warm, tactile, specialty-roaster feel
let coffee_bg             = Color(hex: "#FAF6F0")  // warm cream — the "shelf"
let coffee_surface        = Color(hex: "#FFFFFF")  // clean white card
let coffee_card_bg        = Color(hex: "#F5F0E8")  // warm off-white for nested elements
let coffee_card_dark      = Color(hex: "#2A2520")  // dark roast card variant
let coffee_text_primary   = Color(hex: "#1A1714")  // almost-black, warm undertone
let coffee_text_secondary = Color(hex: "#8C8279")  // warm grey
let coffee_text_tertiary  = Color(hex: "#B5A99A")  // lighter warm grey for metadata
let coffee_accent         = Color(hex: "#D4A96A")  // warm amber — specialty coffee gold
let coffee_accent_hover   = Color(hex: "#C49A5A")  // amber darkened for pressed states
let coffee_green          = Color(hex: "#5B8C5A")  // muted sage — not neon, not clinical
let coffee_tag_bg         = Color(hex: "#E8DFD2")  // tag/chip background
let coffee_divider        = Color(hex: "#E5DDD3")  // warm divider line
```

### The Seam — Where Worlds Meet

The transition happens at **Why Today** (Screen 2). This is the hero brand moment — not a CSS crossfade, but an emotional beat. The palette transition is embedded in the narrative of the screen, not in a scroll position.

```
┌─ HEALTH WORLD ──────────────────────┐
│  Card 1: Your Data        (#F5F5F5 bg, #1A1A1A card)  │  dark cards, mono, metrics
│  Card 2: Brezi's Read     (#1A1A1A card, blue accent)  │  dark, clinical
│  Card 3: Your Brew        (#0F0F0F card)               │  profile suggestion + CTA
│         "CHOOSE YOUR BEAN →"                            │  pushes to Why Today
├─ THE SEAM ──────────────────────────┤
│  WHY TODAY                 (#0F0F0F → #FAF6F0)         │  HERO TRANSITION
│  Emotional bridge. Palette shifts from dark to warm     │  The app exhales.
│  as the message renders. Body state → coffee direction. │
├─ COFFEE WORLD ──────────────────────┤
│  Bean Selection            (#FAF6F0)                    │  warm cream background
│  Bean Cards                (#FFFFFF)                    │  warm white cards
│  Brew Settings             (#FAF6F0)                    │  warm cream
│  Marketplace               (#FAF6F0)                    │  warm cream
└──────────────────────────────────────┘
```

### Typography on Coffee Side

```swift
// Coffee side uses the same type scale but with warm-toned colors
// and adds roaster/bean-specific styles
let t_roaster_name    = Font.system(size: 11, weight: .semibold, design: .default)  // ALL CAPS, tracked 15%
let t_bean_name       = Font.system(size: 22, weight: .bold)                        // bean display name
let t_bean_body       = Font.system(size: 14, weight: .regular)                     // bean descriptions
let t_price           = Font.system(size: 18, weight: .bold, design: .monospaced)   // price display
let t_flavor_label    = Font.system(size: 11, weight: .medium)                      // slider labels
let t_tasting_note    = Font.system(size: 10, weight: .medium)                      // under tasting note icons

// When ABC Social ships, these map to:
// t_roaster_name   → ABC Social Mono Regular, ALL CAPS
// t_bean_name      → ABC Social Condensed Bold
// t_bean_body      → ABC Social Regular
// t_price          → ABC Social Mono Regular
// t_flavor_label   → ABC Social Mono Regular
// t_tasting_note   → ABC Social Mono Regular
```

---

## 2. The Full Flow — 6 Screens

Jackson's locked flow:

```
Screen 1: Health data (dashboard — Cards 1-3)
Screen 2: Why Today (EMOTIONAL BRIDGE — palette transition)
Screen 3: Bean Selection (WARM — 3 bean cards + general profile card)
Screen 4: Brew Settings (adapts to selected bean)
Screen 5: Brew confirmation / timer
Screen 6: Post-brew check-in
```

### Screen 1: Dashboard — `DashboardView.swift`

**Existing dashboard, with Card 3 modified.**

Card 3 no longer shows full brew parameters. It shows the **profile suggestion**, a bridge line connecting body state to coffee direction, and a CTA to enter the coffee world.

```
┌─────────────────────────────────────┐
│  ⟨📡⟩            bre:zi           ⚙  │  NavBarView.swift (dark)
│─────────────────────────────────────│
│                                     │  bg: #F5F5F5 (cool neutral)
│  GOOD MORNING                       │  Mono, 11px, #888888
│  MARCH 20                           │  Mono, 11px, #888888
│                                     │
│  ┌─ 1  YOUR DATA ─────────────────┐ │  HealthDataCardView (unchanged)
│  │  ┌──────┐ ┌──────┐ ┌─────┐    │ │
│  │  │  42  │ │  61% │ │ 6:48│    │ │  health metrics
│  │  │  HRV │ │ RECV │ │ SLP │    │ │
│  │  └──────┘ └──────┘ └─────┘    │ │
│  │  ↓ 18% vs your baseline       │ │
│  │  Source: WHOOP · 6:42am        │ │
│  └─────────────────────────────────┘ │
│                                     │
│  ┌─ 2  BREZI'S READ ──────────────┐ │  InsightStripView (unchanged)
│  │  Mid recovery. Your HRV is     │ │
│  │  18% below baseline. Go        │ │
│  │  standard.                      │ │
│  └─────────────────────────────────┘ │
│                                     │
│  ┌─ 3  YOUR BREW ─────────────────┐ │  DashboardBrewCardView.swift
│  │                                 │ │  bg: #0F0F0F, radius: 24pt
│  │  BREZI SUGGESTS                 │ │  Mono, 11px, #888888, ALL CAPS
│  │                                 │ │
│  │  FIRST LIGHT                    │ │  CondensedBold, 32px, #FFFFFF
│  │  Steady morning. Standard brew. │ │  Regular, 14px, #CCCCCC [BRIDGE]
│  │                                 │ │
│  │  4–8°C · 12 min · 60 g/L       │ │  Mono, 14px, #888888
│  │                                 │ │
│  │  ┌─────────────────────────┐   │ │  PRIMARY CTA
│  │  │  CHOOSE YOUR BEAN  →   │   │ │  bg: #FFFFFF, text: #000000
│  │  └─────────────────────────┘   │ │  CondensedBold, 16px, ALL CAPS
│  │                                 │ │  height: 48pt, radius: 12pt
│  │  Use general settings ›        │ │  SECONDARY CTA
│  │                                 │ │  Regular, 14px, #888888
│  └─────────────────────────────────┘ │  44×44pt tap area
│                                     │
│  ┌─────────────────────────┐        │  NEW — moved above history
│  │  BROWSE BEANS           │        │  outlined, border: 1px #888888
│  └─────────────────────────┘        │  text: #888888, Mono, 13px
│                                     │  height: 44pt, radius: 12pt
│  ┌─────────────────────────┐        │
│  │  VIEW BREW HISTORY      │        │  outlined button (unchanged)
│  └─────────────────────────┘        │
│                                     │
└─────────────────────────────────────┘
```

**Bridge copy per profile (exact strings):**
```swift
let card3_bridgeCopy: [String: String] = [
    "tide_pool":   "Low and slow today.",
    "first_light": "Steady morning. Standard brew.",
    "deep_work":   "You've got the runway. Go deep.",
    "voltage":     "All systems. Full send.",
    "the_blank":   "Your call. Your brew."
]
```

**Copy (exact strings):**
```swift
let card3_label        = "BREZI SUGGESTS"
let card3_cta_bean     = "CHOOSE YOUR BEAN"
let card3_cta_general  = "Use general settings"
let dashboard_browseBeans = "BROWSE BEANS"
```

**"CHOOSE YOUR BEAN" behavior:** Pushes to **Screen 2: Why Today**. This is where the visual world transitions.

**"Use general settings" behavior:** Pushes directly to **Screen 4: Brew Settings** with `selectedBean = nil`, skipping Why Today and Bean Selection.

**"BROWSE BEANS" behavior:** Pushes to **Marketplace** (Bean Shop).

---

## 3. Screen 2: Why Today — `WhyTodayView.swift`

### The Principle

Why Today is the emotional bridge. It's the screen where Brezi stops being a dashboard and starts being a companion. The language shifts from metrics to meaning. The palette shifts from dark to warm. The typography shifts from monospace data to human sentences.

This screen IS the palette transition. This screen IS the hero brand moment. It's the breath between "here's your data" and "here's what to brew."

### The Transition — How It Works

The screen opens dark (`#0F0F0F`). As the message renders (typewriter-style, character by character), the background begins its crossfade to warm cream (`#FAF6F0`). By the time the message is fully rendered, the world is warm. The user reads the message in the new world. The transition happened *around a feeling*, not around a navigation push.

```swift
// ROUND 3 CHANGE: All timing is now configurable via WhyTodayConfig
struct WhyTodayConfig {
    static var typewriterSpeed: TimeInterval = 0.03     // seconds per character
    static var lineDelay: TimeInterval = 0.3            // pause between lines
    static var transitionDuration: TimeInterval = 1.8   // total bg transition
    static var transitionDelay: TimeInterval = 0.4      // wait for first line to start rendering
    static var ctaDelay: TimeInterval = 0.3             // delay after text completes before CTA appears
    static var transitionCurve: Animation = .easeInOut(duration: 1.8)
}

// TUNING NOTE: If TestFlight feedback shows >30% of users tapping "Skip" by day 3,
// first lever to pull: typewriterSpeed = 0.02, transitionDuration = 1.2
// This cuts total time from ~5.8s to ~4.0s without changing the moment's character.

struct WhyTodayTransition {
    let bgStart       = Color(hex: "#0F0F0F")   // dark — where we came from
    let bgEnd         = Color(hex: "#FAF6F0")   // warm cream — where we're going
    let textStart     = Color(hex: "#FFFFFF")    // white text on dark
    let textEnd       = Color(hex: "#1A1714")    // warm black on cream
}
```

### Layout

```
┌─────────────────────────────────────┐
│  [←]              bre:zi           │  NavBarView — bg: #000000 (always dark)
│─────────────────────────────────────│
│                                     │  bg: animated #0F0F0F → #FAF6F0
│                                     │
│                                     │
│                                     │
│                                     │  vertical center
│                                     │
│  You're at 61% today.              │  LINE 1: the data point
│                                     │  Regular, 24px, animated #FFFFFF → #1A1714
│  Rebuilding. Your nervous          │  LINE 2: the interpretation
│  system is doing the               │  Regular, 24px, animated #FFFFFF → #1A1714
│  heavy lifting.                    │  line-height: 1.5
│                                     │
│                                     │
│  Clean start. Nothing to prove.     │  LINE 3: the coffee direction
│                                     │  Regular, 18px, animated #888888 → #8C8279
│                                     │  top padding: 24pt from line 2
│                                     │  line-height: 1.4
│                                     │
│                                     │
│                                     │
│                                     │
│                                     │
│                                     │
│  ┌─────────────────────────────────┐│
│  │       SEE YOUR BEANS  →        ││  CTA — appears after text completes
│  └─────────────────────────────────┘│  fadeIn + translateY(12→0)
│                                     │  delay: WhyTodayConfig.ctaDelay
│                                     │  after last character renders
│  Skip ›                             │  skip link — always visible
│                                     │  Mono, 13px, #B5A99A
│                                     │  44×44pt tap area
│                                     │  bottom padding: 48pt + safe area
└─────────────────────────────────────┘
```

### Layout Spec

```swift
struct WhyTodaySpec {
    // Container
    let horizontalPadding: CGFloat = 32   // generous — this is a reading screen
    let verticalAlignment: Alignment = .center

    // Line 1 + 2 (the message)
    let messageFont = Font.system(size: 24, weight: .regular)
    let messageLineHeight: CGFloat = 1.5
    // Color animates with background

    // Line 3 (the coffee direction)
    let directionFont = Font.system(size: 18, weight: .regular)
    let directionLineHeight: CGFloat = 1.4
    let directionTopPadding: CGFloat = 24
    // Color animates: #888888 → #8C8279

    // CTA
    let ctaFont = Font.system(size: 16, weight: .bold)  // CondensedBold when ABC Social ships
    let ctaHeight: CGFloat = 52
    let ctaRadius: CGFloat = 12
    let ctaBg = Color(hex: "#1A1714")
    let ctaText = Color(hex: "#FFFFFF")
    let ctaBottomPadding: CGFloat = 12

    // Skip
    let skipFont = Font.system(size: 13, weight: .medium, design: .monospaced)
    let skipColor = Color(hex: "#B5A99A")
    let skipBottomPadding: CGFloat = 48  // + safe area inset
}
```

### Copy — Per Profile (exact strings) — ROUND 3 UPDATED

Each profile generates a unique Why Today message. The message has three parts:
1. **The data point** — what the health read says (one sentence)
2. **The interpretation** — what it means for the body (one-two sentences max)
3. **The coffee direction** — what kind of brew this morning calls for (one sentence)

```swift
struct WhyTodayCopy {
    let line1: String    // data point
    let line2: String    // interpretation
    let direction: String // coffee direction
}

func whyTodayCopy(profile: String, recoveryPercent: Int) -> WhyTodayCopy {
    switch profile {
    case "tide_pool":
        return WhyTodayCopy(
            line1: "You're at \(recoveryPercent)% today.",
            line2: "Your body is asking for less. Honor it. Low dose, low temp — let your system coast.",
            direction: "Something gentle. Let these do the talking."
        )
    case "first_light":
        return WhyTodayCopy(
            line1: "You're at \(recoveryPercent)% today.",
            line2: "Rebuilding. Your nervous system is doing the heavy lifting.",  // ROUND 3: shortened
            direction: "Clean start. Nothing to prove."                           // ROUND 3: "Nothing to prove" moved here
        )
    case "deep_work":
        return WhyTodayCopy(
            line1: "You're at \(recoveryPercent)% today.",
            line2: "Strong recovery. Your body has the runway. Full extraction — use what you've built.",
            direction: "You need fuel. These are built for it."
        )
    case "voltage":
        return WhyTodayCopy(
            line1: "You're at \(recoveryPercent)% today.",
            line2: "Peak state. Everything is green. Maximum dose, maximum extraction — send it.",
            direction: "All systems go. Pick your weapon."
        )
    case "the_blank":
        return WhyTodayCopy(
            line1: "You're at \(recoveryPercent)% today.",
            line2: "Manual mode. No recommendation — you know what you want today.",
            direction: "Your call today. Here's what we've got."
        )
    default:
        return whyTodayCopy(profile: "first_light", recoveryPercent: recoveryPercent)
    }
}
```

**CTA copy:**
```swift
let whyToday_cta  = "SEE YOUR BEANS"
let whyToday_skip = "Skip"
```

**"SEE YOUR BEANS" behavior:** Pushes to Screen 3: Bean Selection. The coffee world is now fully warm.

**"Skip" behavior:** Same as "SEE YOUR BEANS" — pushes to Bean Selection. The skip is for returning users who've seen Why Today many times. It doesn't skip the bean selection, just the pause.

### Interaction Details

- **First visit of the day:** Full typewriter render + palette transition. ~4-6 seconds total (tunable via WhyTodayConfig).
- **Return visits same day:** Message appears instantly (no typewriter), background starts at `#FAF6F0` (already warm). The transition is a one-time-per-day moment.
- **State tracking:** `UserDefaults` key `lastWhyTodayDate`. If today's date matches, skip animation.

```swift
func shouldAnimateWhyToday() -> Bool {
    let lastDate = UserDefaults.standard.string(forKey: "lastWhyTodayDate")
    let today = DateFormatter.yyyyMMdd.string(from: Date())
    if lastDate == today { return false }
    UserDefaults.standard.set(today, forKey: "lastWhyTodayDate")
    return true
}
```

### Why This Is the Brand Moment

The transition from health to coffee happens *inside a feeling*. The user reads about their body. The background warms around them as they read. By the time they see the coffee direction, the world has changed. They didn't navigate to a new section — the section came to meet them.

This is the moment Jackson described: *"This is your body. This is what it deserves."* It's not a tagline on the screen. It's the experience of the screen itself.

---

## 4. Bean Card Visual Design — `BeanCardView.swift`

### The Card

Each bean card is a self-contained product card. It lives in three contexts: bean selection (Screen 3), the marketplace, and bean detail. One component, three rendering modes.

The card has **two visual variants** based on roast level:
- **Light/Medium roast:** white card on cream background
- **Dark roast:** dark card (`#2A2520`) — inverts text colors

And **three context modes** that control which CTA renders:
- **`.selection`** — Screen 3. Brew intent. Shows "BREW WITH THIS" only.
- **`.marketplace`** — Bean Shop. Purchase intent. Shows price + "ADD TO CART" + subscribe.
- **`.detail`** — BeanDetailView. Both intents, full information.

### Layout — Light/Medium Roast, Selection Context (Regent, Corvus)

```
┌─────────────────────────────────────┐
│                                     │  bg: #FFFFFF
│  ┌─────────────────────────────┐    │  radius: 20pt
│  │                             │    │  shadow: coffee_card_shadow
│  │                             │    │
│  │      [BEAN BAG IMAGE]       │    │  280pt tall
│  │      contentMode: .fit      │    │  edge-to-edge within card radius
│  │                             │    │  clipped by card clipShape
│  │                             │    │
│  └─────────────────────────────┘    │  image container: per-bean imageBgTint
│                                     │  radius: top corners match card, bottom 0
│  ┌──────────────────────┐           │  (image bleeds to card edges at top)
│  │  REGENT COFFEE       │           │  t_roaster_name, #8C8279, ALL CAPS
│  └──────────────────────┘           │  tracking: 15%, top padding: 16pt
│                                     │  horizontal padding: 16pt
│  Ethiopia Guji                      │  t_bean_name, 22px bold, #1A1714
│  Hambela Washed                     │  line-height: 1.2
│                                     │  top padding: 4pt
│  from $27                           │  Mono, 12px, #B5A99A
│                                     │  top padding: 4pt (selection context only)
│                                     │
│  ┌──────┐ ┌──────┐ ┌──────┐ ┌────┐ │
│  │* 🍫 *│ │  🌸  │ │  🍓  │ │ 🍋 ││  TASTING NOTE ICONS
│  │Cocoa │ │Jasmi-│ │Straw-│ │Lime││  * = lead icon accent bg (#D4A96A @ 20%)
│  │      │ │ne    │ │berry │ │    ││  others: standard #F5F0E8
│  └──────┘ └──────┘ └──────┘ └────┘ │  top padding: 12pt
│                                     │
│  ROAST   ░░▓░░░░░░░░░░░░░░░░       │  FLAVOR SLIDERS
│           Light                     │  See Section 8 for full spec
│  SWEET   ░░░░░░░░░░░░▓░░░░░░       │  top padding: 12pt
│           High                      │  value labels shown on full card
│  ACID    ░░░░░░░░░░░░▓░░░░░░       │
│           Medium-High               │
│                                     │
│  ┌──────────────────────────────┐   │
│  │  ★ TODAY'S PICK             │   │  RECOMMENDATION TAG
│  └──────────────────────────────┘   │  (conditional — only if bean matches profile)
│                                     │  top padding: 12pt
│                                     │
│  ┌─────────────────────────────┐    │  SELECTION CONTEXT CTA
│  │    BREW WITH THIS  →        │    │  bg: #5B8C5A, text: #FFFFFF
│  └─────────────────────────────┘    │  CondensedBold, 14px, ALL CAPS
│                                     │  height: 44pt, radius: 10pt
│                                     │  full width within card
│                                     │  top padding: 16pt
│                                     │  bottom padding: 20pt
└─────────────────────────────────────┘
```

### Layout — Dark Roast, Selection Context (Stitch)

Same structure. Inverted palette + emotional tag on card:

```
┌─────────────────────────────────────┐
│                                     │  bg: #2A2520
│  ┌─────────────────────────────┐    │  radius: 20pt
│  │                             │    │
│  │      [BEAN BAG IMAGE]       │    │  280pt tall
│  │      Stitch bear + Fiat     │    │  imageBgTint: #2E2022
│  │                             │    │
│  └─────────────────────────────┘    │
│                                     │
│  STITCH COFFEE                      │  t_roaster_name, #B5A99A, ALL CAPS
│                                     │
│  Dark Field                         │  t_bean_name, 22px bold, #FFFFFF
│  Blend                              │
│                                     │
│  For when you mean business.        │  EMOTIONAL TAG (Stitch only)
│  Dense, structured, no apologies.   │  Regular italic, 13px, #B5A99A
│                                     │  top padding: 4pt
│                                     │
│  ┌──────┐ ┌──────┐ ┌──────┐        │
│  │* 🍫 *│ │  🌰  │ │  🍮  │        │  icons on dark bg
│  │Dark  │ │Toast-│ │Cara- │        │  * = lead icon accent bg (#D4A96A @ 30%)
│  │Choc  │ │ed    │ │mel   │        │  labels: #B5A99A
│  │      │ │Almond│ │      │        │  icon color: #D4A96A
│  └──────┘ └──────┘ └──────┘        │
│                                     │
│  ROAST   ░░░░░░░░░░░░░░░░░▓░       │  slider track: #3A3530
│           Dark                      │  fill gradient: dark variant
│  SWEET   ░░░░░░░░▓░░░░░░░░░░       │
│           Medium                    │
│  ACID    ░░▓░░░░░░░░░░░░░░░░       │
│           Low                       │
│                                     │
│  ┌──────────────────────────────┐   │
│  │  ★ TODAY'S PICK             │   │  tag bg: rgba(212,169,106,0.2)
│  └──────────────────────────────┘   │  text: #D4A96A
│                                     │
│  ┌─────────────────────────────┐    │
│  │    BREW WITH THIS  →        │    │  bg: #D4A96A, text: #2A2520
│  └─────────────────────────────┘    │  (dark card variant CTA)
│                                     │
└─────────────────────────────────────┘
```

### Layout — Marketplace Context (purchase intent)

Same card structure, but CTA block changes:

```
│                                     │
│  ┌─────────┐                        │
│  │  $27.00 │          ┌──────────┐  │  PRICE + CTA
│  └─────────┘          │ ADD TO   │  │  t_price, 18px bold mono, #1A1714
│  or subscribe         │  CART →  │  │  button: bg #1A1714, text #FFFFFF
│  & save 15%           └──────────┘  │  height: 44pt, radius: 12pt
│                                     │
│  or brew with this bean ›           │  secondary: Mono, 13px, #8C8279
│                                     │  44×44pt tap area
│                                     │  bottom padding: 20pt
└─────────────────────────────────────┘
```

**Dark card marketplace CTA:**
- Price: #FFFFFF
- "ADD TO CART" button: bg #D4A96A, text #2A2520
- Subscribe text: #B5A99A
- Brew link: #B5A99A

### Component Spec — `BeanCardView.swift`

```swift
struct BeanCardSpec {
    // Container
    let cardRadius: CGFloat = 20
    let cardPaddingH: CGFloat = 16         // inner horizontal (below image)
    let cardPaddingBottom: CGFloat = 20

    // Image container
    let imageHeight: CGFloat = 280         // roaster art breathes
    let imageTopRadius: CGFloat = 20       // matches card top corners
    let imageBottomRadius: CGFloat = 0     // bleed into content area
    // Image bg color: per-bean imageBgTint (see bean data model)

    // Roaster name
    let roasterFont = Font.system(size: 11, weight: .semibold)  // ALL CAPS
    let roasterTracking: CGFloat = 0.15    // 15% tracking
    let roasterTopPadding: CGFloat = 16
    let roasterColorLight = Color(hex: "#8C8279")
    let roasterColorDark  = Color(hex: "#B5A99A")

    // Bean name
    let beanNameFont = Font.system(size: 22, weight: .bold)
    let beanNameTopPadding: CGFloat = 4
    let beanNameLineHeight: CGFloat = 1.2
    let beanNameColorLight = Color(hex: "#1A1714")
    let beanNameColorDark  = Color(hex: "#FFFFFF")

    // Price metadata (selection context only)
    let priceMetaFont = Font.system(size: 12, weight: .regular, design: .monospaced)
    let priceMetaColor = Color(hex: "#B5A99A")
    let priceMetaTopPadding: CGFloat = 4

    // Emotional tag (Stitch only — showEmotionalTagOnCard: true)
    let emotionalTagFont = Font.system(size: 13, weight: .regular).italic()
    let emotionalTagColor = Color(hex: "#B5A99A")
    let emotionalTagTopPadding: CGFloat = 4

    // Tasting notes row
    let tastingNoteTopPadding: CGFloat = 12
    let tastingNoteIconSize: CGFloat = 36      // on card
    let tastingNoteGap: CGFloat = 12

    // Flavor sliders
    let sliderTopPadding: CGFloat = 12

    // Recommendation tag — "TODAY'S PICK"
    let tagTopPadding: CGFloat = 12
    let tagHeight: CGFloat = 28
    let tagRadius: CGFloat = 8
    let tagFont = Font.system(size: 11, weight: .semibold)  // ALL CAPS
    let tagBgLight    = Color(hex: "#E8DFD2")
    let tagTextLight  = Color(hex: "#8C8279")
    let tagBgDark     = Color(hex: "#D4A96A").opacity(0.2)
    let tagTextDark   = Color(hex: "#D4A96A")
    let tagIcon       = "star.fill"  // SF Symbol, 10pt, same color as text
    let tagCopy       = "TODAY'S PICK"

    // Selection context CTA — BREW WITH THIS
    let brewCtaHeight: CGFloat = 44
    let brewCtaRadius: CGFloat = 10
    let brewCtaFont = Font.system(size: 14, weight: .bold)  // ALL CAPS
    let brewCtaBgLight    = Color(hex: "#5B8C5A")
    let brewCtaTextLight  = Color(hex: "#FFFFFF")
    let brewCtaBgDark     = Color(hex: "#D4A96A")
    let brewCtaTextDark   = Color(hex: "#2A2520")
    let brewCtaTopPadding: CGFloat = 16

    // Marketplace context CTA — ADD TO CART
    let cartButtonHeight: CGFloat = 44
    let cartButtonRadius: CGFloat = 12
    let cartButtonFont = Font.system(size: 14, weight: .bold)  // ALL CAPS
    let cartButtonBgLight    = Color(hex: "#1A1714")
    let cartButtonTextLight  = Color(hex: "#FFFFFF")
    let cartButtonBgDark     = Color(hex: "#D4A96A")
    let cartButtonTextDark   = Color(hex: "#2A2520")

    // Price (marketplace context)
    let priceFont = Font.system(size: 18, weight: .bold, design: .monospaced)
    let priceTopPadding: CGFloat = 16
    let priceColorLight = Color(hex: "#1A1714")
    let priceColorDark  = Color(hex: "#FFFFFF")

    // Shadow
    let cardShadowLight = Shadow(color: .black.opacity(0.06), radius: 12, x: 0, y: 4)
    let cardShadowDark  = Shadow(color: .black.opacity(0.3), radius: 16, x: 0, y: 6)
}
```

### Context Enum

```swift
enum BeanCardContext {
    case selection    // Screen 3 — brew intent only
    case marketplace  // Bean Shop — purchase intent
    case detail       // BeanDetailView — both intents, full info
}
```

### Tag Display Logic

```swift
func showRecommendationTag(bean: Bean, suggestedProfile: BrewProfile) -> Bool {
    return bean.bestForProfiles.contains(suggestedProfile.key)
}
```

### Emotional Tag Display Logic

```swift
func showEmotionalTagOnCard(bean: Bean) -> Bool {
    return bean.showEmotionalTagOnCard  // true for Stitch, false for others
}
```

### The 3 Real Bean Cards — Complete Corrected Data

#### Bean 1: Stitch Coffee — Dark Field Blend

```swift
let stitch_darkField = Bean(
    id: "stitch_dark_field",
    roasterName: "STITCH COFFEE",
    roasterLocation: "Singapore · Roasted in Sydney",
    beanName: "Dark Field",
    beanSubtitle: "Blend",
    roastLevel: .dark,            // triggers dark card variant
    origin: "Blend — globally sourced",
    process: "Espresso-oriented",
    description: "The darkest roast in the Stitch Coffee collection. For the Stoics among us. Inspired by classic Italian roasts. Tailored for a strong start to the day.",
    emotionalTag: "For when you mean business. Dense, structured, no apologies.",
    showEmotionalTagOnCard: true,  // Stitch's brand is voice-forward
    tastingNotes: [
        TastingNote(icon: "icon_dark_chocolate", label: "Dark Chocolate"),
        TastingNote(icon: "icon_toasted_almond", label: "Toasted Almond"),
        TastingNote(icon: "icon_caramel", label: "Caramel")
    ],
    flavorProfile: FlavorProfile(roast: 0.9, sweetness: 0.55, acidity: 0.2),
    bestForProfiles: ["deep_work", "voltage"],
    price: nil,                    // TBD — display "COMING SOON"
    shopifyURL: nil,               // TBD
    shopifyProductID: nil,
    imageAsset: "stitch_dark_field_bag",  // bear in red Fiat 500
    imageBgTint: "#2E2022",              // warm charcoal with subtle red warmth
    cardVariant: .dark
)
```

#### Bean 2: Corvus Coffee Roasters — Sadayana Winey

```swift
let corvus_sadayana = Bean(
    id: "corvus_sadayana_winey",
    roasterName: "CORVUS COFFEE ROASTERS",
    roasterLocation: "Denver, Colorado",
    beanName: "Sadayana Winey",
    beanSubtitle: "Winey Anaerobic Natural Ateng",
    roastLevel: .medium,
    origin: "West Java, Indonesia — Sadayana Washing Station, Indragiri, near Papandayan volcano",
    process: "72-hour natural anaerobic fermentation in sealed barrels, dried on raised beds and patios",
    description: "A striking example of innovation from Indonesia. Wine-like when cupped — shared with pride after dinner.",
    emotionalTag: "Wine-dark. Fruit-forward. The kind of brew you linger over.",
    showEmotionalTagOnCard: false,
    tastingNotes: [
        TastingNote(icon: "icon_raspberry", label: "Raspberry Yogurt"),
        TastingNote(icon: "icon_cherry", label: "Cherry Syrup"),
        TastingNote(icon: "icon_dark_chocolate", label: "Dark Chocolate")
    ],
    flavorProfile: FlavorProfile(roast: 0.45, sweetness: 0.85, acidity: 0.5),
    bestForProfiles: ["deep_work", "voltage"],
    price: nil,                    // TBD
    shopifyURL: nil,               // TBD
    shopifyProductID: nil,
    imageAsset: "corvus_sadayana_bag",     // gold temple/mandala on black with red accent
    imageBgTint: "#2A2418",               // deep gold, matches mandala packaging
    cardVariant: .light                    // light variant despite dark packaging — roast is medium
)
```

#### Bean 3: Regent Coffee — Ethiopia Guji Hambela Washed

```swift
let regent_guji = Bean(
    id: "regent_ethiopia_guji_hambela",
    roasterName: "REGENT COFFEE",
    roasterLocation: "Los Angeles, California",
    beanName: "Ethiopia Guji",
    beanSubtitle: "Hambela Washed",
    roastLevel: .light,
    origin: "Ethiopia, Guji — Hambela",
    process: "Washed",
    varietal: "Paru Biyu",
    description: "Bright, floral, stone fruit. Exceptional cold clarity.",
    emotionalTag: "The cleanest thing you'll drink today. Floral, bright, effortless.",
    showEmotionalTagOnCard: false,
    tastingNotes: [
        TastingNote(icon: "icon_cocoa", label: "Cocoa"),
        TastingNote(icon: "icon_jasmine", label: "Jasmine"),
        TastingNote(icon: "icon_strawberry", label: "Strawberry"),
        TastingNote(icon: "icon_lime", label: "Lime")
    ],
    flavorProfile: FlavorProfile(roast: 0.15, sweetness: 0.8, acidity: 0.75),
    bestForProfiles: ["tide_pool", "first_light"],
    price: 27.00,
    shopifyURL: "https://brezicoffee.com",
    shopifyProductID: "10151173947683",
    imageAsset: "regent_guji_hambela_bag",  // art deco line illustration
    imageBgTint: "#F5F0E8",                // neutral warm cream — lets line work breathe
    cardVariant: .light
)
```

### Updated Bean Data Model

```swift
struct Bean: Identifiable, Codable {
    let id: String
    let roasterName: String
    let roasterLocation: String
    let beanName: String
    let beanSubtitle: String
    let roastLevel: RoastLevel
    let origin: String
    let process: String
    var altitude: String? = nil
    var varietal: String? = nil
    var grade: String? = nil
    var score: String? = nil
    let description: String
    let emotionalTag: String
    let showEmotionalTagOnCard: Bool
    let tastingNotes: [TastingNote]
    let flavorProfile: FlavorProfile
    let bestForProfiles: [String]
    var price: Double? = nil
    var shopifyURL: String? = nil
    var shopifyProductID: String? = nil
    let imageAsset: String
    let imageBgTint: String
    let cardVariant: CardVariant

    enum RoastLevel: String, Codable {
        case light, medium, dark
    }

    enum CardVariant: String, Codable {
        case light, dark
    }
}

struct TastingNote: Codable {
    let icon: String       // asset catalog name
    let label: String
}

struct FlavorProfile: Codable {
    let roast: Double      // 0.0 (lightest) → 1.0 (darkest)
    let sweetness: Double  // 0.0 (none) → 1.0 (very sweet)
    let acidity: Double    // 0.0 (flat) → 1.0 (bright/sharp)
}
```

### Updated Recommendation Matching

```swift
func recommendedBeans(for profileKey: String, allBeans: [Bean]) -> [Bean] {
    return allBeans
        .filter { $0.bestForProfiles.contains(profileKey) }
        .sorted { bean1, bean2 in
            // Primary sort: beans with price available first
            if (bean1.price != nil) != (bean2.price != nil) {
                return bean1.price != nil
            }
            // Secondary sort: alphabetical by roaster
            return bean1.roasterName < bean2.roasterName
        }
}

// Profile → Bean mapping (current catalog)
// tide_pool   → Regent Ethiopia Guji Hambela
// first_light → Regent Ethiopia Guji Hambela
// deep_work   → Corvus Sadayana Winey, Stitch Dark Field
// voltage     → Corvus Sadayana Winey, Stitch Dark Field
// the_blank   → all beans shown, no "TODAY'S PICK" tags
```

### Price TBD State

When `bean.price == nil`:
- Selection context: price metadata line shows `"Coming soon"` — Mono, 12px, #B5A99A
- Marketplace context:
  - Price area shows: `"PRICE COMING SOON"` — Mono, 14px, #B5A99A
  - "ADD TO CART" button becomes: `"NOTIFY ME"` — same size but bg: #E8DFD2, text: #8C8279
  - Tap "NOTIFY ME" → stores notification preference in UserDefaults, shows toast with bean name

```swift
let bean_priceTBD_selection    = "Coming soon"
let bean_priceTBD_marketplace  = "PRICE COMING SOON"
let bean_notifyMe              = "NOTIFY ME"

// ROUND 3 CHANGE: Toast includes bean name for specificity
func notifyMeToast(beanName: String) -> String {
    "We'll let you know when \(beanName) drops."
}
```

### "Add to Cart" Behavior

```swift
func addToCart(bean: Bean) {
    guard let shopifyURL = bean.shopifyURL,
          let productID = bean.shopifyProductID else {
        // Price TBD state — handled by NOTIFY ME
        return
    }
    let url = URL(string: "\(shopifyURL)/cart/add?id=\(productID)")!
    present(SFSafariViewController(url: url))
}
```

---

## 5. Screen 3: Bean Selection — `BeanSelectionView.swift`

### The Principle

This is the pivot point. The moment the app becomes a coffee brand. The user has read Why Today, felt the palette shift, and now arrives in the warm world to choose their bean. The background is `#FAF6F0`. The nav bar stays `#000000` (brand shell is always dark).

This screen shows recommended beans (matched to profile), a general profile card (equal visual weight), and a browse section for all beans.

### Layout

```
┌─────────────────────────────────────┐
│  [←]              bre:zi        🛍  │  NavBarView — bg: #000000
│─────────────────────────────────────│  back arrow + bag icon (ROUND 3: was cart)
│                                     │  bg: #FAF6F0 (warm cream)
│                                     │
│  FIRST LIGHT                        │  CondensedBold, 14px, #8C8279
│  RECOMMENDED BEANS                  │  Mono, 11px, #B5A99A, ALL CAPS
│                                     │  top padding: 20pt
│  Clean start. Nothing to prove.     │  Regular, 15px, #8C8279
│                                     │  line-height: 1.5
│                                     │  (echoes Why Today direction copy)
│                                     │  bottom padding: 16pt
│  ┌─────────────────────────────────┐│
│  │                                 ││
│  │  ┌───────────────────────────┐  ││  RECOMMENDED BEAN CARD 1
│  │  │      [REGENT BAG]         │  ││  BeanCardView(context: .selection)
│  │  │      280pt tall            │  ││
│  │  │      imageBgTint: #F5F0E8 │  ││
│  │  │                           │  ││
│  │  │  REGENT COFFEE            │  ││
│  │  │  Ethiopia Guji            │  ││
│  │  │  Hambela Washed           │  ││
│  │  │  from $27                 │  ││  price metadata only
│  │  │                           │  ││
│  │  │  *🍫* Cocoa  🌸 Jasmine   │  ││  lead icon has accent bg
│  │  │  🍓 Strawberry  🍋 Lime   │  ││  4 tasting notes
│  │  │                           │  ││
│  │  │  ROAST ░░▓░░░░░░░░░░░░    │  ││
│  │  │         Light             │  ││
│  │  │  SWEET ░░░░░░░░░░░░▓░░    │  ││
│  │  │         High              │  ││
│  │  │  ACID  ░░░░░░░░░░░░▓░░    │  ││
│  │  │         Medium-High       │  ││
│  │  │                           │  ││
│  │  │  ★ TODAY'S PICK           │  ││
│  │  │                           │  ││
│  │  │  ┌───────────────────┐    │  ││  sole CTA: BREW WITH THIS
│  │  │  │ BREW WITH THIS  → │    │  ││  bg: #5B8C5A, text: #FFFFFF
│  │  │  └───────────────────┘    │  ││
│  │  │                           │  ││
│  │  └───────────────────────────┘  ││
│  │                                 ││
│  └─────────────────────────────────┘│
│                                     │  gap: 16pt between cards
│  ┌─────────────────────────────────┐│
│  │                                 ││  RECOMMENDED BEAN CARD 2
│  │  (next matching bean card)      ││  (if another bean matches profile)
│  │  BeanCardView(context: .sel)    ││
│  │                                 ││
│  └─────────────────────────────────┘│
│                                     │  gap: 16pt
│  ┌─────────────────────────────────┐│
│  │                                 ││  GENERAL PROFILE CARD (equal)
│  │  ┌───────────────────────────┐  ││  bg: #FFFFFF, radius: 20pt
│  │  │                           │  ││
│  │  │      [GENERAL BREW        │  ││  280pt tall illustration area
│  │  │       ILLUSTRATION]       │  ││  generic coffee bag silhouette
│  │  │      280pt tall            │  ││  or profile icon as hero image
│  │  │      imageBgTint: #F5F0E8 │  ││  bg: #F5F0E8
│  │  │                           │  ││
│  │  └───────────────────────────┘  ││
│  │                                 ││
│  │  GENERAL BREW                   ││  t_bean_name, 22px bold, #1A1714
│  │                                 ││
│  │  Optimized for First Light.     ││  Regular, 14px, #8C8279
│  │  Works with whatever's in       ││  line-height: 1.5
│  │  your bag.                      ││
│  │                                 ││
│  │  ┌───────────────────────┐     ││
│  │  │   BREW GENERAL  →    │     ││  bg: #5B8C5A, text: #FFFFFF
│  │  └───────────────────────┘     ││  same green button as bean cards
│  │                                 ││  height: 44pt, radius: 10pt
│  │                                 ││  full width within card
│  └─────────────────────────────────┘│  shadow: same as bean cards
│                                     │
│  ── browse ─────────────────────── │  divider
│                                     │  line: 1px #E5DDD3
│                                     │  "browse": Mono, 12px, #B5A99A
│                                     │  padding: 24pt vertical
│                                     │
│  BROWSE ALL BEANS                   │  Mono, 11px, #B5A99A, ALL CAPS
│                                     │  bottom padding: 12pt
│  ┌─────────────────────────────────┐│
│  │                                 ││  ALL BEANS — horizontal scroll
│  │  [Stitch]  [Corvus]  [Regent]  ││  BeanCardCompactView.swift
│  │                                 ││  (compact variant — see below)
│  └─────────────────────────────────┘│
│                                     │
│  ┌─────────────────────┐            │
│  │  VISIT BEAN SHOP  → │            │  outlined button
│  └─────────────────────┘            │  border: 1px #8C8279
│                                     │  text: #8C8279, Mono, 13px
│                                     │  height: 44pt, radius: 12pt
│                                     │  bottom safe area: 32pt
└─────────────────────────────────────┘
```

### Header Copy — Per Profile (exact strings) — ROUND 3 UPDATED

The header on Screen 3 echoes the coffee direction from Why Today. It's the same emotional register, carried forward.

```swift
let beanSelection_label = "RECOMMENDED BEANS"

let beanSelection_headerCopy: [String: String] = [
    "tide_pool":   "Something gentle. Let these do the talking.",
    "first_light": "Clean start. Nothing to prove.",                // ROUND 3: matches updated direction
    "deep_work":   "You need fuel. These are built for it.",
    "voltage":     "All systems go. Pick your weapon.",
    "the_blank":   "Your call today. Here's what we've got."
]
```

### General Profile Card — Equal Visual Weight

The general profile option is an **equal card** in the same scroll as the bean cards. Same component structure (`#FFFFFF` bg, 20pt radius, same shadow), same height feel. It is the last card before the "browse" divider.

```swift
struct GeneralProfileCardSpec {
    // Same container as BeanCardView
    let cardRadius: CGFloat = 20
    let cardBg = Color(hex: "#FFFFFF")
    let cardShadow = Shadow(color: .black.opacity(0.06), radius: 12, x: 0, y: 4)

    // Image area — profile illustration
    let imageHeight: CGFloat = 280
    let imageBg = Color(hex: "#F5F0E8")
    // Content: a simple illustration of a generic coffee bag silhouette,
    // or the brew profile icon (e.g., First Light's sunrise) as hero image
    // Asset: "general_brew_illustration" — clean, minimal, warm
    let imageAsset = "general_brew_illustration"

    // Title
    let title = "GENERAL BREW"
    let titleFont = Font.system(size: 22, weight: .bold)
    let titleColor = Color(hex: "#1A1714")
    let titleTopPadding: CGFloat = 16

    // Body
    func body(_ profileName: String) -> String {
        "Optimized for \(profileName). Works with whatever's in your bag."
    }
    let bodyFont = Font.system(size: 14, weight: .regular)
    let bodyColor = Color(hex: "#8C8279")
    let bodyTopPadding: CGFloat = 4
    let bodyLineHeight: CGFloat = 1.5

    // CTA — same style as bean card brew CTA
    let ctaCopy = "BREW GENERAL"
    let ctaHeight: CGFloat = 44
    let ctaRadius: CGFloat = 10
    let ctaBg = Color(hex: "#5B8C5A")
    let ctaText = Color(hex: "#FFFFFF")
    let ctaFont = Font.system(size: 14, weight: .bold)  // ALL CAPS
    let ctaTopPadding: CGFloat = 16
    let ctaBottomPadding: CGFloat = 20
}
```

### Bean Card Compact — `BeanCardCompactView.swift`

For horizontal scroll in "Browse All Beans". No slider value labels. No CTA — tap goes to BeanDetailView.

```
┌───────────────────┐
│  [BEAN BAG]       │  width: 180pt
│  160 × 160pt      │  bg: #FFFFFF (or #2A2520 for dark)
│  imageBgTint      │  radius: 16pt
│                   │
│  STITCH COFFEE    │  t_roaster_name, 10px, ALL CAPS
│  Dark Field       │  Bold, 16px, #1A1714
│                   │
│  *🍫* 🌰 🍮       │  tasting note icons (28pt, mini)
│                   │  lead icon has accent bg
│  ROAST ░░░░░░░▓░  │  mini sliders (no value labels)
│  SWEET ░░░░▓░░░░  │
│  ACID  ░▓░░░░░░░  │
│                   │
│  Coming soon      │  price or "Coming soon"
│                   │  Mono, 12px, #B5A99A
└───────────────────┘
```

**Spec:**
- Width: 180pt
- Height: intrinsic (content-driven)
- Image: 160 × 160pt, `contentMode: .fit`, per-bean `imageBgTint`
- Horizontal scroll: `.scrollTargetBehavior(.viewAligned)`, padding 16pt, gap 12pt
- Tap anywhere on compact card → pushes `BeanDetailView`
- Mini sliders: same logic as full sliders but 120pt wide, no value labels, no endpoint labels

### All Copy — Screen 3 (exact strings)

```swift
let beanSelection_label         = "RECOMMENDED BEANS"
let beanSelection_brewCTA       = "BREW WITH THIS"
let beanSelection_generalTitle  = "GENERAL BREW"
func beanSelection_generalBody(_ profileName: String) -> String {
    "Optimized for \(profileName). Works with whatever's in your bag."
}
let beanSelection_generalCTA    = "BREW GENERAL"
let beanSelection_browseLabel   = "BROWSE ALL BEANS"
let beanSelection_shopCTA       = "VISIT BEAN SHOP"
let beanSelection_dividerBrowse = "browse"
let beanSelection_tagCopy       = "TODAY'S PICK"
```

### Navigation Behavior

**"BREW WITH THIS" on a bean card:** Stores `selectedBean`, pushes to Screen 4 (Brew Settings) with bean context.

**"BREW GENERAL" on general profile card:** Sets `selectedBean = nil`, pushes to Screen 4 with general context.

**Tap on bean card (outside CTA):** Pushes to `BeanDetailView` for full bean info.

**"VISIT BEAN SHOP":** Pushes to `MarketplaceView`.

**Bag icon (🛍) in nav bar:** Pushes to `MarketplaceView`.

---

## 6. Screen 4: Brew Settings — `BrewSettingsView.swift`

### The Principle

Brew parameters, adapted to the selected bean. Two variants: bean-specific and general. Background stays `#FAF6F0` (coffee world).

### Variant 1: Bean-Specific Settings — ROUND 3 LAYOUT UPDATED

**Key change from Round 2:** The adaptation note moves from below the parameter detail table to the **hinge position** — between the 3-box summary and the detail table, inside the parameter card. This follows Elon's principle: meaning before metrics. The user sees the big numbers (hook), reads *why* those numbers were chosen (context), then sees the full detail table (data). The adaptation note gets a warm background chip (`#F5F0E8`, 8pt radius) to visually separate it from the parameter data while keeping it inside the parameter context.

```
┌─────────────────────────────────────┐
│  [←]              bre:zi        🛍  │  NavBarView (ROUND 3: bag icon)
│─────────────────────────────────────│
│                                     │  bg: #FAF6F0
│  YOUR BREW                          │  Mono, 11px, #B5A99A, ALL CAPS
│                                     │  top padding: 20pt
│  ┌─────────────────────────────────┐│
│  │                                 ││  OPTIMIZATION BADGE
│  │  ⚡ Optimized for:              ││  Mono, 12px, #5B8C5A
│  │  Regent Ethiopia Guji Hambela   ││  Bold, 16px, #1A1714
│  │  × First Light profile          ││  Mono, 12px, #8C8279
│  │                                 ││
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 12pt
│                                     │  left border: 3px #5B8C5A
│                                     │  padding: 16pt inner
│  ┌─────────────────────────────────┐│
│  │                                 ││  SERVING INFOGRAPHIC
│  │     [See Section 9 — full       ││  ServingInfoCard.swift
│  │      infographic card]          ││
│  │                                 ││
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │
│  ┌─ BREW PARAMETERS ──────────────┐ │
│  │                                 │ │  bg: #FFFFFF, radius: 16pt
│  │  ┌──────┐  ┌──────┐  ┌──────┐ │ │
│  │  │ 4–8° │  │  12  │  │  60  │ │ │  CondensedBold, 28px, #1A1714
│  │  │  °C  │  │ min  │  │ g/L  │ │ │  Mono, 11px, #8C8279, ALL CAPS
│  │  │ TEMP │  │ TIME │  │ DOSE │ │ │  3 equal columns, 8pt gap
│  │  └──────┘  └──────┘  └──────┘ │ │
│  │                                 │ │
│  │  ┌──────────────────────────┐  │ │  ROUND 3: ADAPTATION NOTE — HINGE POSITION
│  │  │                          │  │ │  bg: #F5F0E8, radius: 8pt
│  │  │  Adapted for this bean's │  │ │  padding: 12pt inner
│  │  │  light roast — lower     │  │ │  Mono italic, 13px, #8C8279
│  │  │  temp, longer steep to   │  │ │  line-height: 1.5
│  │  │  pull floral notes.      │  │ │  top margin: 12pt, bottom margin: 12pt
│  │  │                          │  │ │
│  │  └──────────────────────────┘  │ │
│  │                                 │ │
│  │  ┌──────────────────────────┐  │ │
│  │  │  ROAST    Light          │  │ │  parameter rows
│  │  │  GRIND    Medium         │  │ │  Mono, 13px
│  │  │  WATER    Filtered       │  │ │  label: #B5A99A, value: #1A1714
│  │  └──────────────────────────┘  │ │  bg: #F5F0E8, radius: 10pt
│  │                                 │ │  row height: 36pt, padding: 12pt
│  └─────────────────────────────────┘ │
│                                     │
│  ┌─────────────────────────────────┐│  FLAVOR PREVIEW
│  │                                 ││
│  │  WHAT YOU'LL TASTE              ││  Mono, 11px, #B5A99A, ALL CAPS
│  │                                 ││
│  │  ┌──────┐ ┌──────┐ ┌──────┐   ││  tasting note icons (48pt, detail size)
│  │  │* 🍫 *│ │  🌸  │ │  🍓  │   ││  lead icon has accent bg
│  │  │Cocoa │ │Jasmi-│ │Straw-│   ││
│  │  │      │ │ne    │ │berry │   ││
│  │  └──────┘ └──────┘ └──────┘   ││
│  │                                 ││
│  │  ┌──────┐                      ││
│  │  │  🍋  │                      ││  4th note (Regent)
│  │  │Lime  │                      ││
│  │  └──────┘                      ││
│  │                                 ││
│  │  Cocoa, jasmine, strawberry,    ││  Regular, 15px, #1A1714
│  │  lime. Bright, floral, stone    ││  line-height: 1.6
│  │  fruit. Exceptional cold        ││
│  │  clarity.                       ││
│  │                                 ││
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │  padding: 20pt inner
│  ┌─────────────────────────────────┐│
│  │                                 ││  START BREW CTA
│  │  ┌─────────────────────────┐   ││
│  │  │    START BREW  →        │   ││  bg: #1A1714, text: #FFFFFF
│  │  └─────────────────────────┘   ││  CondensedBold, 16px, ALL CAPS
│  │                                 ││  height: 52pt, radius: 12pt
│  │  Change bean ›                  ││  Mono, 13px, #8C8279
│  │                                 ││  44×44pt tap area
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │  padding: 20pt inner
│                                     │  bottom safe area: 32pt
└─────────────────────────────────────┘
```

### Adaptation Note Spec — ROUND 3

```swift
struct AdaptationNoteSpec {
    let bg = Color(hex: "#F5F0E8")
    let radius: CGFloat = 8
    let paddingInner: CGFloat = 12
    let topMargin: CGFloat = 12
    let bottomMargin: CGFloat = 12
    let font = Font.system(size: 13, weight: .regular, design: .monospaced).italic()
    let color = Color(hex: "#8C8279")
    let lineHeight: CGFloat = 1.5
}
```

### Variant 2: General Profile Settings

When `selectedBean == nil`:

```
│  ┌─────────────────────────────────┐│
│  │                                 ││  OPTIMIZATION BADGE (general)
│  │  Using general                  ││  Mono, 12px, #8C8279
│  │  First Light settings           ││  Bold, 16px, #1A1714
│  │                                 ││
│  │  No specific bean selected.     ││  Mono, 12px, #B5A99A
│  │  These parameters work with     ││
│  │  most medium roasts.            ││
│  │                                 ││
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 12pt
│                                     │  left border: 3px #B5A99A (muted)
```

- No flavor preview section (no bean to describe)
- No tasting note icons
- Adaptation note at hinge position reads: `"Standard extraction for this profile. Works with most beans in the medium roast range."`
- "Change bean" link becomes `"Select a specific bean ›"` — pops back to Screen 3

### All Copy — Screen 4 (exact strings)

```swift
let brewSettings_label           = "YOUR BREW"
let brewSettings_optimizedFor    = "Optimized for:"
func brewSettings_beanLine(_ beanName: String) -> String { beanName }
func brewSettings_profileLine(_ profileName: String) -> String { "× \(profileName) profile" }
let brewSettings_startBrew       = "START BREW"
let brewSettings_changeBean      = "Change bean"
let brewSettings_selectBean      = "Select a specific bean"
let brewSettings_whatYoullTaste   = "WHAT YOU'LL TASTE"
let brewSettings_brewParams       = "BREW PARAMETERS"

// General variant
func brewSettings_generalTitle(_ profileName: String) -> String {
    "Using general\n\(profileName) settings"
}
let brewSettings_generalBody     = "No specific bean selected. These parameters work with most medium roasts."
let brewSettings_generalNote     = "Standard extraction for this profile. Works with most beans in the medium roast range."
```

### Adaptation Notes — Per Profile × Roast Level (exact strings)

```swift
let adaptationNotes: [String: [String: String]] = [
    "tide_pool": [
        "light":  "Adapted for this bean's light roast — lower temp, longer steep to pull floral notes.",
        "medium": "Adapted for medium roast — standard extraction, balanced sweetness.",
        "dark":   "Adapted for dark roast — shorter steep to avoid over-extraction. Gentle approach."
    ],
    "first_light": [
        "light":  "Adapted for this bean's light roast — lower temp, longer steep to pull floral notes.",
        "medium": "Standard parameters for medium roast. Clean extraction, no adjustments needed.",
        "dark":   "Slightly shorter steep for dark roast. Prevents bitterness, keeps it balanced."
    ],
    "deep_work": [
        "light":  "Extended steep pulls complexity from light roast. Full dose, full extraction.",
        "medium": "Medium roast at full extraction. Dense, sustaining, built for the session.",
        "dark":   "Dark roast at full dose. Maximum density. The parameters match the intensity."
    ],
    "voltage": [
        "light":  "High concentration from light roast. Fruit intensity amplified. Controlled chaos.",
        "medium": "Maximum extraction from medium roast. Sharp, concentrated, efficient.",
        "dark":   "Dark roast at peak parameters. Everything's turned up. No subtlety, no apology."
    ]
]
```

### Bean-Specific Parameter Adjustments

When a bean is selected, brew parameters shift slightly from generic profile values.

```swift
struct BeanAdjustment {
    let tempDelta: Int       // degrees offset (-2 to +2)
    let timeDelta: Int       // minutes offset (-2 to +3)
    let doseDelta: Int       // g/L offset (-5 to +5)
    let roastSuggestion: String
    let grindSuggestion: String
}

let beanAdjustments: [String: [String: BeanAdjustment]] = [
    "regent_ethiopia_guji_hambela": [
        "tide_pool":   BeanAdjustment(tempDelta: -1, timeDelta: +1, doseDelta: 0,  roastSuggestion: "Light",  grindSuggestion: "Medium-Fine"),
        "first_light": BeanAdjustment(tempDelta: -1, timeDelta: +1, doseDelta: 0,  roastSuggestion: "Light",  grindSuggestion: "Medium"),
    ],
    "corvus_sadayana_winey": [
        "deep_work":   BeanAdjustment(tempDelta: 0,  timeDelta: +1, doseDelta: 0,  roastSuggestion: "Medium", grindSuggestion: "Medium"),
        "voltage":     BeanAdjustment(tempDelta: 0,  timeDelta: +2, doseDelta: +5, roastSuggestion: "Medium", grindSuggestion: "Medium"),
    ],
    "stitch_dark_field": [
        "deep_work":   BeanAdjustment(tempDelta: +1, timeDelta: -1, doseDelta: 0,  roastSuggestion: "Dark",   grindSuggestion: "Medium-Coarse"),
        "voltage":     BeanAdjustment(tempDelta: +1, timeDelta: -2, doseDelta: +5, roastSuggestion: "Dark",   grindSuggestion: "Coarse"),
    ]
]

func adjustedBrewParams(profile: BrewProfile, bean: Bean?) -> BrewParams {
    var params = profile.defaultParams

    guard let bean = bean,
          let adjustment = beanAdjustments[bean.id]?[profile.key] else {
        return params  // no adjustment — use profile defaults
    }

    params.tempLow += adjustment.tempDelta
    params.tempHigh += adjustment.tempDelta
    params.steepMinutes += adjustment.timeDelta
    params.doseGramsPerLiter += adjustment.doseDelta
    params.roastSuggestion = adjustment.roastSuggestion
    params.grindSuggestion = adjustment.grindSuggestion

    return params
}
```

---

## 7. Bean Detail View — `BeanDetailView.swift`

### The Principle

Full-screen bean information. Tapping a bean card (outside the CTA) pushes here. This is where the user goes deep — origin details, full tasting notes, brew pairing, serving guide, purchase options, and (after 3+ brews) their personal history with this bean.

### Layout

```
┌─────────────────────────────────────┐
│  [←]              bre:zi        🛍  │  NavBarView (bg: #000000)
│─────────────────────────────────────│  ROUND 3: bag icon
│                                     │  bg: #FAF6F0
│  ┌─────────────────────────────────┐│
│  │                                 ││  HERO IMAGE
│  │        [BEAN BAG IMAGE]         ││  full-width, 300pt tall
│  │         300 × 300pt             ││  contentMode: .fit
│  │         imageBgTint per bean    ││  per-bean background tint
│  │                                 ││
│  └─────────────────────────────────┘│  radius: 0 (full bleed to screen edges)
│                                     │
│  CORVUS COFFEE ROASTERS             │  t_roaster_name, #8C8279, ALL CAPS
│  Denver, Colorado                   │  Mono, 12px, #B5A99A
│                                     │  horizontal padding: 20pt
│  Sadayana Winey                     │  CondensedBold, 28px, #1A1714
│  Winey Anaerobic Natural Ateng      │  Regular, 16px, #8C8279
│                                     │
│  ┌─ ORIGIN ────────────────────────┐│
│  │  Region: West Java, Indonesia   ││  Mono, 13px
│  │  Station: Sadayana Washing Stn  ││  label: #B5A99A, value: #1A1714
│  │  Altitude: Near Papandayan      ││
│  │          volcano                ││
│  │  Process: 72-hr anaerobic       ││
│  │          fermentation in        ││
│  │          sealed barrels         ││
│  │  Drying: Raised beds & patios   ││
│  └─────────────────────────────────┘│  bg: #F5F0E8, radius: 12pt
│                                     │  padding: 16pt inner, 20pt horizontal
│  ┌─ WHAT YOU'LL TASTE ─────────────┐│
│  │                                 ││
│  │  ┌──────┐ ┌──────┐ ┌──────┐   ││  tasting note icons (48pt, detail size)
│  │  │* 🫐 *│ │  🍒  │ │  🍫  │   ││  lead icon has accent bg
│  │  │Rasp- │ │Cherry│ │Dark  │   ││
│  │  │berry │ │Syrup │ │Choc  │   ││
│  │  │Yogurt│ │      │ │      │   ││
│  │  └──────┘ └──────┘ └──────┘   ││
│  │                                 ││
│  │  Raspberry yogurt, cherry       ││  Regular, 15px, #1A1714
│  │  syrup, dark chocolate.         ││  line-height: 1.6
│  │  Wine-like when cupped —        ││
│  │  shared with pride after        ││
│  │  dinner.                        ││
│  │                                 ││
│  │  ROAST   ░░░░░░░░▓░░░░░░░░░░   ││  flavor sliders (large variant)
│  │          Medium                 ││  with value labels + endpoint labels
│  │  SWEET   ░░░░░░░░░░░░░░░▓░░░   ││
│  │          High                   ││
│  │  ACID    ░░░░░░░░░▓░░░░░░░░░   ││
│  │          Medium                 ││
│  │                                 ││
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │  padding: 20pt inner
│  ┌─ BREW PAIRING ──────────────────┐│
│  │                                 ││
│  │  Best for:                      ││  Mono, 11px, #B5A99A
│  │  ┌──────────┐ ┌──────────┐     ││
│  │  │DEEP WORK │ │ VOLTAGE  │     ││  profile chips
│  │  └──────────┘ └──────────┘     ││  bg: #E8DFD2, text: #8C8279
│  │                                 ││  radius: 8pt, height: 28pt
│  │  "Wine-dark. Fruit-forward.     ││  Regular, 15px, #1A1714
│  │   The kind of brew you linger   ││  line-height: 1.5
│  │   over."                        ││  italic
│  │                                 ││
│  │  ┌─ YOUR HISTORY ────────────┐  ││  BeanBrewHistoryCard
│  │  │                           │  ││  (ONLY if brew count ≥ 3)
│  │  │  You've brewed this 5×.   │  ││  See Section 10 for full spec
│  │  │  Best day: recovery 78%.  │  ││
│  │  │                           │  ││
│  │  └───────────────────────────┘  ││
│  │                                 ││
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │
│  ┌─ SERVING GUIDE ─────────────────┐│
│  │                                 ││  INFOGRAPHIC CARD
│  │     [See Section 9]             ││  ServingInfoCard.swift
│  │                                 ││
│  └─────────────────────────────────┘│
│                                     │
│  ┌─────────────────────────────────┐│  BREW + PURCHASE
│  │                                 ││
│  │  ┌─────────────────────────┐   ││  primary CTA: BREW
│  │  │  BREW WITH THIS  →      │   ││  bg: #5B8C5A, text: #FFFFFF
│  │  └─────────────────────────┘   ││  CondensedBold, 16px, ALL CAPS
│  │                                 ││  height: 52pt, radius: 12pt
│  │                                 ││
│  │  $27.00                         ││  t_price, #1A1714
│  │  or subscribe & save 15%        ││  Mono, 12px, #5B8C5A
│  │  — $22.95/delivery              ││  concrete savings display
│  │                                 ││
│  │  ┌─────────────────────────┐   ││  ADD TO CART
│  │  │   ADD TO CART  →        │   ││  bg: #1A1714, text: #FFFFFF
│  │  └─────────────────────────┘   ││  height: 48pt, radius: 12pt
│  │                                 ││  full width
│  │  ┌─────────────────────────┐   ││  SUBSCRIBE & SAVE
│  │  │   SUBSCRIBE & SAVE     │   ││  bg: transparent, border: 1px #1A1714
│  │  └─────────────────────────┘   ││  text: #1A1714, height: 44pt
│  │                                 ││  radius: 12pt
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │  padding: 20pt inner
│                                     │  bottom safe area: 32pt
└─────────────────────────────────────┘
```

### CTA Priority Flip — After 3+ Brews

When the user has brewed this bean 3+ times, the CTA priority flips to encourage subscription:

**Default (< 3 brews):**
1. `"BREW WITH THIS"` — primary (green)
2. `"ADD TO CART"` — secondary (dark)
3. `"SUBSCRIBE & SAVE"` — tertiary (outlined)

**After 3+ brews:**
1. `"BREW WITH THIS"` — primary (green) — stays on top
2. `"SUBSCRIBE & SAVE 15%"` — promoted to secondary (dark fill: bg #1A1714, text #FFFFFF)
3. `"ADD TO CART"` — demoted to tertiary (outlined: border 1px #1A1714, text #1A1714)

```swift
func ctaPriority(brewCount: Int) -> CTALayout {
    if brewCount >= 3 {
        return .subscriptionPromoted
    }
    return .default
}
```

### Subscribed State — ROUND 3 NEW

When the user has an active subscription for this bean:

```swift
// Subscribed button state
let detail_subscribed = "SUBSCRIBED"  // with checkmark icon

struct SubscribedButtonSpec {
    let text = "SUBSCRIBED"
    let icon = "checkmark"  // SF Symbol, 12pt, leading
    let bg = Color.clear
    let borderColor = Color(hex: "#5B8C5A")
    let borderWidth: CGFloat = 1
    let textColor = Color(hex: "#5B8C5A")
    let height: CGFloat = 44
    let radius: CGFloat = 12
    let isEnabled = false  // non-tappable
}

// Below price, replace subscribe savings line with:
let detail_subscribedManage = "Subscribed — manage in Settings"
// Mono, 12px, #5B8C5A

// Subscription detection (MVP — UserDefaults)
func isSubscribed(beanID: String) -> Bool {
    UserDefaults.standard.stringArray(forKey: "subscribedBeans")?.contains(beanID) ?? false
}

func markSubscribed(beanID: String) {
    var beans = UserDefaults.standard.stringArray(forKey: "subscribedBeans") ?? []
    if !beans.contains(beanID) {
        beans.append(beanID)
        UserDefaults.standard.set(beans, forKey: "subscribedBeans")
    }
}
```

### Section Labels (exact strings)

```swift
let detail_origin        = "ORIGIN"
let detail_whatYoullTaste = "WHAT YOU'LL TASTE"
let detail_brewPairing   = "BREW PAIRING"
let detail_servingGuide  = "SERVING GUIDE"
let detail_bestFor       = "Best for:"
let detail_brewCTA       = "BREW WITH THIS"
let detail_addToCart      = "ADD TO CART"
let detail_subscribe      = "SUBSCRIBE & SAVE"
let detail_subscribeSave  = "or subscribe & save 15%"
let detail_subscribed     = "SUBSCRIBED"                      // ROUND 3
let detail_subscribedMgmt = "Subscribed — manage in Settings" // ROUND 3
```

### "Subscribe & Save" Behavior

Opens Shopify subscription page in SFSafariViewController. Button hidden when `bean.price == nil` or `bean.shopifyURL == nil`. After tap, marks bean as subscribed in UserDefaults.

```swift
func showSubscribeButton(bean: Bean) -> Bool {
    return bean.price != nil && bean.shopifyURL != nil && !isSubscribed(beanID: bean.id)
}

func openSubscription(bean: Bean) {
    guard let shopifyURL = bean.shopifyURL else { return }
    let url = URL(string: "\(shopifyURL)/subscribe")!
    markSubscribed(beanID: bean.id)
    present(SFSafariViewController(url: url))
}
```

### Concrete Savings Display

When price is available, show actual dollar amount:
```swift
func subscribeSavingsText(price: Double) -> String {
    let subscriptionPrice = price * 0.85
    return "or subscribe & save 15% — $\(String(format: "%.2f", subscriptionPrice))/delivery"
}
// e.g., "or subscribe & save 15% — $22.95/delivery"
```

### "Add to Cart" Behavior

```swift
func addToCart(bean: Bean) {
    guard let shopifyURL = bean.shopifyURL,
          let productID = bean.shopifyProductID else { return }
    let url = URL(string: "\(shopifyURL)/cart/add?id=\(productID)")!
    present(SFSafariViewController(url: url))
}

---

## 8. Flavor Visualization — Sliders + Tasting Note Icons

### Flavor Slider Bars — `FlavorSliderView.swift`

Three horizontal bars: Roast, Sweetness, Acidity. Each is a continuous spectrum with a positioned dot indicator.

```
ROAST    ░░░░░░░░░░░░░░░░░░▓░░
         Light              Dark

SWEET    ░░░░░░░░░▓░░░░░░░░░░░
         Low               High

ACID     ░░░░░░░░░░░░▓░░░░░░░░
         Low               High
```

### Slider Spec

```swift
struct FlavorSliderSpec {
    // Track
    let trackHeight: CGFloat = 6
    let trackRadius: CGFloat = 3
    let trackWidth: CGFloat = .infinity  // fills available width

    // Track gradients — LIGHT CARD VARIANT
    let roastGradient: [Color] = [
        Color(hex: "#F5E6C8"),   // lightest roast — pale gold
        Color(hex: "#D4A96A"),   // medium — warm amber
        Color(hex: "#8B6914"),   // medium-dark — brown gold
        Color(hex: "#3D2B1F"),   // dark — espresso brown
        Color(hex: "#1A1208")    // darkest — almost black
    ]
    let sweetnessGradient: [Color] = [
        Color(hex: "#E5DDD3"),   // low — neutral warm grey
        Color(hex: "#F5E6C8"),   // light sweetness
        Color(hex: "#F0D4A0"),   // medium
        Color(hex: "#E8B860"),   // high
        Color(hex: "#D4A96A")    // very sweet — full amber
    ]
    let acidityGradient: [Color] = [
        Color(hex: "#E5DDD3"),   // low/flat — neutral
        Color(hex: "#D4D8A0"),   // mild — warm green
        Color(hex: "#C8D070"),   // medium — yellow-green
        Color(hex: "#B8C850"),   // medium-high — bright green
        Color(hex: "#88B830")    // high/bright — vivid green
    ]

    // Track gradients — DARK CARD VARIANT (brighter low-end for contrast)
    let sweetnessGradientDark: [Color] = [
        Color(hex: "#C8BFB0"),   // brighter than #E5DDD3 for ~3.2:1 contrast
        Color(hex: "#F5E6C8"),
        Color(hex: "#F0D4A0"),
        Color(hex: "#E8B860"),
        Color(hex: "#D4A96A")
    ]
    let acidityGradientDark: [Color] = [
        Color(hex: "#C8CCA0"),   // brighter for dark card contrast
        Color(hex: "#D4D8A0"),
        Color(hex: "#C8D070"),
        Color(hex: "#B8C850"),
        Color(hex: "#88B830")
    ]
    // Roast gradient works on both variants (dark end is dark on any bg)

    // QUALITY GATE: If banding is visible on LCD test devices (iPhone SE, iPhone 11),
    // reduce to 3 color stops per gradient: start, midpoint, end.
    // The visual language holds with fewer stops. Jensen should test on an iPhone 11
    // in Phase 1 and flag before proceeding.

    // Indicator dot
    let dotSize: CGFloat = 12
    let dotColor = Color(hex: "#1A1714")
    let dotBorderColor = Color(hex: "#FFFFFF")
    let dotBorderWidth: CGFloat = 2
    let dotShadow = Shadow(color: .black.opacity(0.15), radius: 3, x: 0, y: 1)

    // Dark card dot
    let dotColorDark = Color(hex: "#FFFFFF")
    let dotBorderColorDark = Color(hex: "#2A2520")

    // Labels
    let labelFont = Font.system(size: 11, weight: .medium)
    let categoryLabelColor = Color(hex: "#8C8279")       // "ROAST", "SWEET", "ACID"
    let categoryLabelColorDark = Color(hex: "#B5A99A")
    let valueLabelColor = Color(hex: "#B5A99A")           // "Light", "Medium", etc.
    let valueLabelColorDark = Color(hex: "#8C8279")
    let labelTracking: CGFloat = 0.05  // 5%

    // Endpoint labels (detail view only)
    let endpointFont = Font.system(size: 9, weight: .regular, design: .monospaced)
    let endpointColor = Color(hex: "#B5A99A")
    let roastEndpoints = ("Light", "Dark")
    let sweetnessEndpoints = ("Low", "High")
    let acidityEndpoints = ("Low", "High")

    // Layout
    let rowHeight: CGFloat = 32        // per slider row
    let categoryLabelWidth: CGFloat = 48  // fixed width for alignment
    let gapBetweenRows: CGFloat = 8
}
```

### Slider Rendering

```swift
func dotPosition(value: Double, trackWidth: CGFloat) -> CGFloat {
    let padding: CGFloat = 6  // half dot size — don't clip at edges
    return padding + (value * (trackWidth - padding * 2))
}
```

### Value Label Logic

```swift
func roastLabel(_ value: Double) -> String {
    switch value {
    case 0.0..<0.25: return "Light"
    case 0.25..<0.45: return "Light-Medium"
    case 0.45..<0.65: return "Medium"
    case 0.65..<0.85: return "Medium-Dark"
    default: return "Dark"
    }
}

func intensityLabel(_ value: Double) -> String {
    switch value {
    case 0.0..<0.2: return "Low"
    case 0.2..<0.4: return "Low-Medium"
    case 0.4..<0.6: return "Medium"
    case 0.6..<0.8: return "Medium-High"
    default: return "High"
    }
}
```

### Slider Variants by Context

| Context | Track width | Show value label | Show endpoints | Row height |
|---|---|---|---|---|
| Bean card (full) | card width - 32pt | Yes (below) | No | 32pt |
| Bean detail | card width - 40pt | Yes (below) | Yes | 36pt |
| Bean card compact | 120pt | No | No | 20pt |
| Brew settings | card width - 40pt | Yes (below) | No | 32pt |

### Tasting Note Icons — `TastingNoteView.swift`

Flat, minimal illustrations. Custom vector art — NOT emoji, NOT SF Symbols. Think specialty coffee bag tasting note icons.

### Icon Container Spec

```swift
struct TastingNoteIconSpec {
    // Container sizes by context
    let containerSizeDetail: CGFloat = 48     // bean detail view
    let containerSizeCard: CGFloat = 36       // bean card
    let containerSizeMini: CGFloat = 28       // compact card
    let containerBg = Color(hex: "#F5F0E8")   // warm off-white
    let containerBgDark = Color(hex: "#3A3530")  // dark card variant
    let containerRadius: CGFloat = 12         // rounded square

    // Icon
    let iconPadding: CGFloat = 8
    let iconColor = Color(hex: "#8C6B3E")     // warm brown — primary icon color
    let iconColorDark = Color(hex: "#D4A96A") // amber — dark card variant

    // Lead icon treatment — ROUND 3: explicitly rendered in wireframes
    // First icon in the array gets a subtle accent background
    let leadIconBg = Color(hex: "#D4A96A").opacity(0.2)       // light card
    let leadIconBgDark = Color(hex: "#D4A96A").opacity(0.3)   // dark card
    // All other icons use standard containerBg / containerBgDark

    // Label
    let labelFont = Font.system(size: 10, weight: .medium)
    let labelColor = Color(hex: "#8C8279")
    let labelColorDark = Color(hex: "#B5A99A")
    let labelTopPadding: CGFloat = 4
    let labelMaxWidth: CGFloat = 56

    // Row layout
    let iconGap: CGFloat = 12
    let maxIconsPerRow: Int = 4
}
```

### Complete Icon Asset List

15 icons total for current catalog + future extensibility:

```swift
enum TastingNoteIcon: String, CaseIterable {
    // Chocolate family
    case darkChocolate = "icon_dark_chocolate"     // broken chocolate bar piece
    case cocoa = "icon_cocoa"                       // cocoa bean or powder pile
    case milkChocolate = "icon_milk_chocolate"      // chocolate drop/square

    // Nut family
    case toastedAlmond = "icon_toasted_almond"      // almond shape with toast marks

    // Sweet family
    case caramel = "icon_caramel"                   // caramel drizzle/swirl
    case honey = "icon_honey"                       // honey dipper

    // Fruit family
    case raspberry = "icon_raspberry"               // raspberry with drupelets
    case strawberry = "icon_strawberry"             // strawberry with leaf
    case cherry = "icon_cherry"                     // two cherries on stem
    case berry = "icon_berry"                       // cluster of berries (blueberry style)
    case tropicalFruit = "icon_tropical_fruit"      // mango or papaya slice
    case lime = "icon_lime"                         // lime wedge

    // Floral family
    case jasmine = "icon_jasmine"                   // jasmine flower (5 petals, open)

    // Barrel / Aged
    case whiskey = "icon_whiskey"                   // oak barrel or whiskey glass

    // Spice
    case pepper = "icon_pepper"                     // peppercorn cluster
}
```

### Tasting Note Mapping — 3 Real Beans

```swift
let beanTastingNotes: [String: [TastingNoteDisplay]] = [
    "stitch_dark_field": [
        TastingNoteDisplay(icon: .darkChocolate, label: "Dark Chocolate"),   // LEAD — accent bg
        TastingNoteDisplay(icon: .toastedAlmond, label: "Toasted Almond"),
        TastingNoteDisplay(icon: .caramel, label: "Caramel")
    ],
    "corvus_sadayana_winey": [
        TastingNoteDisplay(icon: .raspberry, label: "Raspberry Yogurt"),     // LEAD — accent bg
        TastingNoteDisplay(icon: .cherry, label: "Cherry Syrup"),
        TastingNoteDisplay(icon: .darkChocolate, label: "Dark Chocolate")
    ],
    "regent_ethiopia_guji_hambela": [
        TastingNoteDisplay(icon: .cocoa, label: "Cocoa"),                    // LEAD — accent bg
        TastingNoteDisplay(icon: .jasmine, label: "Jasmine"),
        TastingNoteDisplay(icon: .strawberry, label: "Strawberry"),
        TastingNoteDisplay(icon: .lime, label: "Lime")
    ]
]
```

### Illustration Commissioning Brief (for designer/illustrator)

**Style:** Flat vector, 2-3 colors max per icon (primary + highlight), thick rounded strokes (2-3pt at 48×48), no gradients, no shadows. Inspired by modern specialty coffee packaging — Stitch Coffee's tasting note icons are the benchmark.

**Color palette for icons:**
- Primary: `#8C6B3E` (warm brown)
- Highlight: `#D4A96A` (amber) — used sparingly for accent details
- Container bg: `#F5F0E8` (warm off-white)
- Dark variant primary: `#D4A96A` (amber)
- Dark variant highlight: `#F5E6C8` (pale gold)

**Lead icon treatment:** First icon in each bean's tasting note array gets a subtle tinted background (`#D4A96A` at 20% opacity on light cards, 30% on dark cards) to establish visual hierarchy. One glance should say: *"This is a raspberry-forward coffee."*

**Deliverables:** 15 icons × 2 color variants (light + dark) = 30 assets. Format: PDF vector in Xcode asset catalog, 1x/2x/3x.

---

## 9. Serving Infographic Card — `ServingInfoCard.swift`

### The Principle

Serving size and cutoff time as visual elements, not settings text. Cup icons for servings, bold time for cutoff, horizontal timing bar for brew window.

### Layout

```
┌─────────────────────────────────────┐
│                                     │  bg: #FFFFFF, radius: 16pt
│  SERVING GUIDE                      │  Mono, 11px, #B5A99A, ALL CAPS
│                                     │  padding: 20pt inner
│  ┌───────────────┬─────────────────┐│
│  │               │                 ││  TWO COLUMNS
│  │   SERVING     │   BEST BEFORE   ││  Mono, 11px, #B5A99A, ALL CAPS
│  │               │                 ││
│  │   ┌─────┐    │                 ││
│  │   │ ☕  │    │   2:00 PM       ││  CondensedBold, 28px, #1A1714
│  │   │     │    │                 ││
│  │   └─────┘    │   After this,   ││  Mono, 12px, #B5A99A
│  │              │   caffeine       ││  line-height: 1.5
│  │   1 cup      │   competes with  ││
│  │   ~250ml     │   your sleep.    ││
│  │              │                 ││
│  └──────────────┴──────────────────┘│  divider: 1px #E5DDD3 vertical
│                                     │
│  ┌─────────────────────────────────┐│
│  │                                 ││  TIMING BAR
│  │  ☀️ 6am ━━━━━━━━━━━━━━━░░░ 2pm ││
│  │       ████████████████░░░░░     ││  green fill = brew-safe window
│  │       ↑ brew window             ││  grey track = past cutoff
│  │                                 ││
│  └─────────────────────────────────┘│
│                                     │
└─────────────────────────────────────┘
```

### Serving Size — Cup Icons

```swift
struct ServingIconSpec {
    let iconName = "cup.and.saucer.fill"  // SF Symbol
    let iconSize: CGFloat = 32
    let iconColor = Color(hex: "#D4A96A")  // coffee amber
    let labelFont = Font.system(size: 14, weight: .semibold)
    let labelColor = Color(hex: "#1A1714")
    let sublabelFont = Font.system(size: 12, weight: .regular, design: .monospaced)
    let sublabelColor = Color(hex: "#B5A99A")
}
```

**Serving size display rules:**

| Servings | Icon Display | Label | Sublabel |
|---|---|---|---|
| 1 | 1 cup icon | "1 cup" | "~250ml" |
| 2 | 2 cup icons side by side | "2 cups" | "~500ml" |
| 3 | 3 cup icons | "3 cups" | "~750ml" |

- Cup icons: SF Symbol `cup.and.saucer.fill`, 32pt, `#D4A96A`
- Multiple cups: 4pt gap between icons
- Label below icons: 4pt top padding

### Best Before — Bold Cutoff Time

```swift
struct BestBeforeSpec {
    let timeFont = Font.system(size: 28, weight: .bold)  // CondensedBold when ABC Social ships
    let timeColor = Color(hex: "#1A1714")
    let reasonFont = Font.system(size: 12, weight: .regular, design: .monospaced)
    let reasonColor = Color(hex: "#B5A99A")
    let reasonLineHeight: CGFloat = 1.5
    let reasonText = "After this, caffeine competes with your sleep."
}
```

### Timing Bar — Day Timeline

```swift
struct TimingBarSpec {
    let barHeight: CGFloat = 8
    let barRadius: CGFloat = 4
    let barTrackColor = Color(hex: "#E5DDD3")
    let barFillColor = Color(hex: "#5B8C5A")

    // Labels
    let startLabel = "6am"
    func endLabel(_ cutoffHour: Int) -> String {
        let formatter = DateFormatter()
        formatter.dateFormat = "ha"
        var components = DateComponents()
        components.hour = cutoffHour
        let date = Calendar.current.date(from: components)!
        return formatter.string(from: date).lowercased()
    }
    let startIcon = "sunrise.fill"  // SF Symbol, 14pt
    let endIcon = "moon.fill"       // SF Symbol, 14pt
    let iconColor = Color(hex: "#D4A96A")
    let labelFont = Font.system(size: 11, weight: .medium, design: .monospaced)
    let labelColor = Color(hex: "#B5A99A")

    // Marker
    let markerLabel = "brew window"
    let markerFont = Font.system(size: 10, weight: .medium)
    let markerColor = Color(hex: "#8C8279")
}
```

**Timing bar rendering:**
- Green fill: from 6am to cutoff time, proportional width
- Track: full width, `#E5DDD3`
- Arrow marker: small upward triangle + "brew window" label below the fill section
- SF Symbol sun icon at start, moon icon at end

### Cutoff Times by Profile

```swift
let profileCutoffHours: [String: Int] = [
    "tide_pool":   16,  // 4:00 PM — gentle, can go later
    "first_light": 14,  // 2:00 PM — standard
    "deep_work":   14,  // 2:00 PM — standard
    "voltage":     12,  // 12:00 PM — high caffeine, earlier cutoff
    "the_blank":   14   // 2:00 PM — default
]
```

### Serving Counts by Profile

```swift
let profileServings: [String: Int] = [
    "tide_pool":   1,
    "first_light": 1,
    "deep_work":   2,
    "voltage":     1,
    "the_blank":   1
]
```

---

## 10. Your History With This Bean — `BeanBrewHistoryCard.swift`

### The Principle

After 3+ brews with a specific bean, show a personal history card. This is the retention feature — the moment the app says *"I remember you."* It turns the marketplace from a catalog into a relationship. It drives repeat purchase naturally by reflecting the user's own data back to them.

### When It Appears

- **Threshold:** `brewCount >= 3` for this `selectedBeanID`
- **Location:** Inside the BREW PAIRING section of `BeanDetailView`, after the profile chips and emotional tag
- **Also appears:** On the bean card in `.marketplace` context, below the flavor sliders (compact version)

### Layout — BeanDetailView (full version)

```
│  ┌─ YOUR HISTORY ────────────────┐  │
│  │                               │  │  bg: #FFFFFF
│  │  You've brewed this 5×.       │  │  Regular, 15px, #1A1714
│  │  Best day: recovery 78%.      │  │  line-height: 1.5
│  │                               │  │
│  │  ┌────────────────────────┐   │  │  MINI CHART (optional)
│  │  │  ·  ·     ·  ·  ·     │   │  │  5 dots representing last 5 brews
│  │  │  ─────────────────     │   │  │  y-axis: recovery score
│  │  │  3/1  3/5  3/10 3/15  │   │  │  Mono, 9px, #B5A99A
│  │  └────────────────────────┘   │  │  height: 48pt
│  │                               │  │  chart stroke: #D4A96A, 2pt
│  │                               │  │  dot fill: #D4A96A
│  │  Your best sessions with      │  │  Mono, 12px, #8C8279
│  │  this bean were on recovery   │  │
│  │  days above 70%.              │  │
│  │                               │  │
│  └───────────────────────────────┘  │  radius: 12pt
│                                     │  left border: 3px #D4A96A (amber)
│                                     │  padding: 16pt inner
```

### Layout — Marketplace card (compact version)

```
│  ┌─ BREWED 5× ──────────────────┐  │
│  │  Best day: recovery 78%.     │  │  Mono, 12px, #8C8279
│  └──────────────────────────────┘  │  bg: #F5F0E8, radius: 8pt
│                                     │  padding: 10pt inner, top margin: 8pt
```

### Component Spec

```swift
struct BeanBrewHistorySpec {
    // Container (full version)
    let containerBg = Color(hex: "#FFFFFF")
    let containerRadius: CGFloat = 12
    let containerBorderColor = Color(hex: "#D4A96A")  // amber left border
    let containerBorderWidth: CGFloat = 3
    let containerPadding: CGFloat = 16

    // Section label
    let sectionLabel = "YOUR HISTORY"
    let sectionFont = Font.system(size: 11, weight: .semibold, design: .monospaced)
    let sectionColor = Color(hex: "#B5A99A")

    // Message
    let messageFont = Font.system(size: 15, weight: .regular)
    let messageColor = Color(hex: "#1A1714")
    let messageLineHeight: CGFloat = 1.5

    // Mini chart
    let chartHeight: CGFloat = 48
    let chartStrokeColor = Color(hex: "#D4A96A")
    let chartStrokeWidth: CGFloat = 2
    let chartDotSize: CGFloat = 6
    let chartDotFill = Color(hex: "#D4A96A")
    let chartDateFont = Font.system(size: 9, weight: .regular, design: .monospaced)
    let chartDateColor = Color(hex: "#B5A99A")

    // Insight line
    let insightFont = Font.system(size: 12, weight: .regular, design: .monospaced)
    let insightColor = Color(hex: "#8C8279")

    // Compact version (marketplace card)
    let compactBg = Color(hex: "#F5F0E8")
    let compactRadius: CGFloat = 8
    let compactPadding: CGFloat = 10
    let compactFont = Font.system(size: 12, weight: .regular, design: .monospaced)
    let compactColor = Color(hex: "#8C8279")
}
```

### Data Query

```swift
struct BeanBrewHistory {
    let beanID: String
    let brewCount: Int
    let bestRecoveryScore: Int
    let averageRecoveryScore: Int
    let lastBrewDate: Date
    let recentBrews: [BrewDataPoint]  // last 5 brews for mini chart

    struct BrewDataPoint {
        let date: Date
        let recoveryScore: Int
    }
}

func fetchBeanHistory(beanID: String, brewLogs: [BrewLog]) -> BeanBrewHistory? {
    let beanBrews = brewLogs
        .filter { $0.selectedBeanID == beanID }
        .sorted { $0.date > $1.date }

    guard beanBrews.count >= 3 else { return nil }

    let recoveryScores = beanBrews.compactMap { $0.recoveryScore }
    let bestRecovery = recoveryScores.max() ?? 0
    let avgRecovery = recoveryScores.isEmpty ? 0 : recoveryScores.reduce(0, +) / recoveryScores.count

    let recentBrews = Array(beanBrews.prefix(5)).map {
        BeanBrewHistory.BrewDataPoint(date: $0.date, recoveryScore: $0.recoveryScore ?? 0)
    }

    return BeanBrewHistory(
        beanID: beanID,
        brewCount: beanBrews.count,
        bestRecoveryScore: bestRecovery,
        averageRecoveryScore: avgRecovery,
        lastBrewDate: beanBrews.first!.date,
        recentBrews: recentBrews
    )
}
```

### Copy Generation

```swift
func historyMessage(history: BeanBrewHistory) -> String {
    "You've brewed this \(history.brewCount)×. Best day: recovery \(history.bestRecoveryScore)%."
}

func historyInsight(history: BeanBrewHistory) -> String {
    if history.bestRecoveryScore >= 70 {
        return "Your best sessions with this bean were on recovery days above 70%."
    } else if history.averageRecoveryScore >= 50 {
        return "This bean works well for you across a range of recovery days."
    } else {
        return "You tend to reach for this one on harder days. It holds up."
    }
}

// Compact version (marketplace card)
func historyCompactLabel(history: BeanBrewHistory) -> String {
    "BREWED \(history.brewCount)×"
}

func historyCompactBody(history: BeanBrewHistory) -> String {
    "Best day: recovery \(history.bestRecoveryScore)%."
}
```

### BrewLog Extension

Add optional bean reference to brew logs:

```swift
// Add to existing BrewLog entity
@NSManaged var selectedBeanID: String?      // nil = general profile
@NSManaged var selectedBeanName: String?    // denormalized for display
@NSManaged var recoveryScore: Int?          // recovery % on the day of the brew
```

---

## 11. Marketplace Screen — `MarketplaceView.swift`

### Access Points

1. **Bean Selection Screen 3** — "VISIT BEAN SHOP" button at bottom
2. **Navigation bar bag icon (🛍)** — on coffee-side screens (ROUND 3: was cart)
3. **Dashboard** — "BROWSE BEANS" button (above "VIEW BREW HISTORY")
4. **Settings** — "Bean Shop" row

### Layout

```
┌─────────────────────────────────────┐
│  [←]              bre:zi        🛍  │  NavBarView (ROUND 3: bag icon)
│─────────────────────────────────────│
│                                     │  bg: #FAF6F0
│  ┌─────────────────────────────────┐│
│  │                                 ││  HEADER
│  │  BEAN SHOP                      ││  CondensedBold, 28px, #1A1714
│  │  Selected for cold brew.        ││  Regular, 15px, #8C8279
│  │  Matched to your morning.       ││  line-height: 1.5
│  │                                 ││
│  └─────────────────────────────────┘│  padding: 20pt
│                                     │
│  ┌─ FILTER CHIPS ──────────────────┐│
│  │                                 ││
│  │  [All] [Tide Pool] [First      ││  horizontal scroll
│  │   Light] [Deep Work] [Voltage] ││  FilterChipView.swift
│  │                                 ││
│  └─────────────────────────────────┘│
│                                     │
│  ┌─────────────────────────────────┐│  ROUND 3: ALL-TBD BANNER (conditional)
│  │                                 ││  Only shows when ALL visible beans
│  │  These beans are on deck.       ││  have price == nil
│  │  Tap NOTIFY ME and we'll        ││  Regular, 15px, #1A1714
│  │  ping you when they drop.       ││  line-height: 1.5
│  │                                 ││
│  └─────────────────────────────────┘│  bg: #F5F0E8, radius: 12pt
│                                     │  padding: 16pt, top margin: 8pt
│                                     │
│  ┌─ RECOMMENDED FOR YOU ───────────┐│  (only when filter = "All")
│  │                                 ││
│  │  Beans that match today's       ││  Regular, 13px, #8C8279
│  │  First Light profile.           ││
│  │                                 ││
│  │  ┌───────────────────────────┐  ││
│  │  │   [REGENT BEAN CARD]      │  ││  BeanCardView(context: .marketplace)
│  │  │   Full card with price +   │  ││  with "TODAY'S PICK" tag
│  │  │   ADD TO CART + subscribe  │  ││  + "or brew with this bean ›"
│  │  │                           │  ││
│  │  │   ┌─ BREWED 3× ────────┐ │  ││  BeanBrewHistoryCard (compact)
│  │  │   │  Best: recovery 72% │ │  ││  (only if brewCount ≥ 3)
│  │  │   └─────────────────────┘ │  ││
│  │  │                           │  ││
│  │  └───────────────────────────┘  ││
│  │                                 ││
│  └─────────────────────────────────┘│  gap: 24pt
│                                     │
│  ┌─ ALL BEANS ─────────────────────┐│
│  │                                 ││
│  │  ┌───────────────────────────┐  ││
│  │  │   [STITCH BEAN CARD]      │  ││  BeanCardView(context: .marketplace)
│  │  └───────────────────────────┘  ││  gap: 16pt between cards
│  │  ┌───────────────────────────┐  ││
│  │  │   [CORVUS BEAN CARD]      │  ││
│  │  └───────────────────────────┘  ││
│  │  ┌───────────────────────────┐  ││
│  │  │   [REGENT BEAN CARD]      │  ││
│  │  └───────────────────────────┘  ││
│  │                                 ││
│  └─────────────────────────────────┘│
│                                     │
│  ┌─────────────────────────────────┐│  SUBSCRIBE BANNER
│  │                                 ││
│  │  Never run out.                 ││  CondensedBold, 22px, #1A1714
│  │                                 ││
│  │  Subscribe and save 15% on      ││  Regular, 14px, #8C8279
│  │  every delivery. Available on   ││  line-height: 1.5
│  │  select beans. Cancel anytime.  ││  ROUND 3: "Available on select beans."
│  │                                 ││
│  │  ┌─────────────────────────┐   ││
│  │  │  SUBSCRIBE ON ANY BEAN →│   ││  outlined button
│  │  └─────────────────────────┘   ││  border: 1px #D4A96A
│  │                                 ││  text: #D4A96A, Mono, 13px
│  │                                 ││  height: 44pt, radius: 12pt
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │  padding: 20pt inner
│                                     │  top accent border: 2px #D4A96A
│                                     │
│  ┌─────────────────────────────────┐│  FOOTER
│  │                                 ││
│  │  More roasters coming soon.     ││  Mono, 13px, #B5A99A, centered
│  │  Want to partner?               ││
│  │  hello@brezicoffee.com          ││  Mono, 13px, #D4A96A
│  │                                 ││  tappable → mailto:
│  └─────────────────────────────────┘│
│                                     │  bottom safe area: 32pt
└─────────────────────────────────────┘
```

### Filter Chips — `FilterChipView.swift`

```swift
struct FilterChipSpec {
    let chipHeight: CGFloat = 36
    let chipRadius: CGFloat = 18       // pill shape
    let chipPaddingH: CGFloat = 16
    let chipGap: CGFloat = 8

    // Default state
    let defaultBg = Color(hex: "#F5F0E8")
    let defaultText = Color(hex: "#8C8279")
    let defaultFont = Font.system(size: 13, weight: .medium)

    // Selected state
    let selectedBg = Color(hex: "#1A1714")
    let selectedText = Color(hex: "#FFFFFF")

    let chipLabels = ["All", "Tide Pool", "First Light", "Deep Work", "Voltage"]
}
```

**Filter behavior:**
- "All" shows all beans, with "Recommended For You" section at top
- Profile-specific filter shows only beans matching that profile
- If no beans match: show guided empty state

**Empty state (filtered, no matches):**
```swift
func marketplace_emptyFilter(profile: String) -> String {
    let fallback = suggestNearbyProfile(profile)
    return "Nothing matched \(profile) yet. Try \(fallback) — similar energy, more options."
}

func suggestNearbyProfile(_ profile: String) -> String {
    switch profile {
    case "tide_pool": return "First Light"
    case "first_light": return "Tide Pool"
    case "deep_work": return "Voltage"
    case "voltage": return "Deep Work"
    default: return "First Light"
    }
}
// Mono, 14px, #B5A99A, centered, padding: 48pt vertical
```

### All-TBD Banner — ROUND 3 NEW

When all visible beans in the current filter have `price == nil`, show a contextual banner reframing TBD as exclusivity:

```swift
func showAllPriceTBDBanner(visibleBeans: [Bean]) -> Bool {
    return visibleBeans.allSatisfy { $0.price == nil }
}

let marketplace_allTBD_banner = "These beans are on deck. Tap NOTIFY ME and we'll ping you when they drop."
```

**Spec:**
- bg: `#F5F0E8`, radius: 12pt
- padding: 16pt inner
- font: Regular, 15px, `#1A1714`
- line-height: 1.5
- position: below filter chips, above bean cards
- top margin: 8pt

### Subscribe Banner CTA Behavior

`"SUBSCRIBE ON ANY BEAN"` scrolls the user to the first bean card that has subscription available (i.e., `bean.price != nil && bean.shopifyURL != nil`).

### All Copy — Marketplace (exact strings) — ROUND 3 UPDATED

```swift
let marketplace_title           = "BEAN SHOP"
let marketplace_subtitle        = "Selected for cold brew. Matched to your morning."
let marketplace_recommended     = "RECOMMENDED FOR YOU"
func marketplace_recommendedSub(_ profileName: String) -> String {
    "Beans that match today's \(profileName) profile."
}
let marketplace_allBeans        = "ALL BEANS"
let marketplace_subscribe_title = "Never run out."
let marketplace_subscribe_body  = "Subscribe and save 15% on every delivery. Available on select beans. Cancel anytime."  // ROUND 3: qualifier added
let marketplace_subscribe_cta   = "SUBSCRIBE ON ANY BEAN"
let marketplace_footer_line1    = "More roasters coming soon."
let marketplace_footer_line2    = "Want to partner?"
let marketplace_footer_email    = "hello@brezicoffee.com"
let marketplace_brewLink        = "or brew with this bean"
let marketplace_allTBD_banner   = "These beans are on deck. Tap NOTIFY ME and we'll ping you when they drop."  // ROUND 3
```

### Nav Bar Icon — ROUND 3 UPDATED

On coffee-side screens, nav bar right slot shows bag icon:

```swift
let navBar_marketplaceIcon = "bag"  // SF Symbol — ROUND 3: was "cart"
// Tap → push MarketplaceView
// No badge count — Brezi is not a cart app
// Same position, size (22pt), and tap target as antenna icon
```

### "BREW WITH THIS" in Marketplace Context

When tapped from marketplace:
1. Store `selectedBean`
2. Pop to dashboard
3. Push through to `BrewSettingsView` with the bean pre-selected

```swift
func brewWithBean(_ bean: Bean, profile: BrewProfile) {
    selectedBean = bean
    navigationPath.removeLast(navigationPath.count)
    navigationPath.append(.brewSettings(bean: bean, profile: profile))
}
```

### Settings Row Addition

```
│  ┌─────────────────────────────────┐│
│  │  ☕ Bean Shop                  › ││  icon: SF Symbol "cup.and.saucer"
│  └─────────────────────────────────┘│  tap → push MarketplaceView
│  ┌─────────────────────────────────┐│
│  │  🔄 Manage Subscriptions      › ││  icon: SF Symbol "arrow.triangle.2.circlepath"
│  └─────────────────────────────────┘│  tap → SFSafariViewController to Shopify
│                                     │  subscription management page
```

---

## 12. Screens 5 & 6: Brewing + Post-Brew — Minor Updates

### Screen 5: Brew Confirmation / Timer

**Unchanged from R3 spec.** The brewing progress screen, brew completion overlay, and timer remain the same. The only addition: the active brew card now shows the bean name if one was selected.

```
│  FIRST LIGHT                        │  CondensedBold, 32px, #FFFFFF
│  Regent Ethiopia Guji Hambela       │  Mono, 13px, #888888 [bean name]
│  7:14am · 12 min                    │  Mono, 14px, #888888
```

If no bean selected: bean name line is omitted.

```swift
func brewCard_beanLine(_ beanName: String?) -> String? {
    return beanName  // nil = omit the line
}
```

### Screen 6: Post-Brew Check-In

**Unchanged from R4/R5 emotional layer spec.** The post-brew check-in (Hit right / Needed more / Too much today) stays as designed. One addition: the check-in result is stored alongside the `selectedBeanID` in the BrewLog, enabling the "Your History With This Bean" feature.

```swift
// When saving post-brew check-in, include bean context
brewLog.selectedBeanID = selectedBean?.id
brewLog.selectedBeanName = selectedBean?.beanName
brewLog.postBrewRating = checkInResult  // .hitRight, .neededMore, .tooMuch
```

---

## 13. Navigation Updates — Complete Tree

### Nav Bar Variants

```swift
enum NavBarVariant {
    case health       // right icon: antenna (📡)
    case coffee       // right icon: bag (🛍) — ROUND 3: was cart
}
```

Nav bar background stays `#000000` everywhere. The brand shell is always dark. Only the content area transitions.

### Full Nav Tree (all additions in **bold**)

```
AppRoot
 └─ MainFlow
     ├─ DashboardView.swift (bg: #F5F5F5)
     │   ├─ State: preBrew → Card 3 CTA
     │   │   ├─ "CHOOSE YOUR BEAN" → **WhyTodayView.swift** [Screen 2]
     │   │   │   └─ "SEE YOUR BEANS" → **BeanSelectionView.swift** [Screen 3]
     │   │   │       ├─ "BREW WITH THIS" → **BrewSettingsView.swift** [Screen 4]
     │   │   │       │   └─ "START BREW" → Brewing state (existing)
     │   │   │       ├─ "BREW GENERAL" → **BrewSettingsView.swift** (bean=nil)
     │   │   │       ├─ Bean card tap → **BeanDetailView.swift**
     │   │   │       │   ├─ "BREW WITH THIS" → BrewSettingsView
     │   │   │       │   ├─ "ADD TO CART" → SFSafariViewController
     │   │   │       │   └─ "SUBSCRIBE & SAVE" → SFSafariViewController
     │   │   │       └─ "VISIT BEAN SHOP" → **MarketplaceView.swift**
     │   │   │
     │   │   └─ "Skip" → **BeanSelectionView.swift** (same destination)
     │   │
     │   ├─ "Use general settings" → **BrewSettingsView.swift** (bean=nil, skip Why Today)
     │   ├─ **"BROWSE BEANS"** → **MarketplaceView.swift**
     │   ├─ State: brewing (unchanged)
     │   └─ State: postBrew (unchanged + bean name line)
     │
     ├─ **MarketplaceView.swift** [accessible from nav bag icon]
     │   ├─ Bean card tap → **BeanDetailView.swift**
     │   ├─ "ADD TO CART" → SFSafariViewController
     │   ├─ "SUBSCRIBE & SAVE" → SFSafariViewController
     │   ├─ "or brew with this bean" → BrewSettingsView (with bean)
     │   └─ "SUBSCRIBE ON ANY BEAN" → scroll to first subscribable bean
     │
     ├─ HistoryView.swift (unchanged)
     ├─ DeviceHubView.swift (unchanged)
     └─ SettingsView.swift
         ├─ **"Bean Shop"** → MarketplaceView
         └─ **"Manage Subscriptions"** → SFSafariViewController
```

---

## 14. Animations

| Location | Element | Type | Duration | Easing | Delay | Trigger |
|---|---|---|---|---|---|---|
| **Why Today** | Background color | crossfade (#0F0F0F → #FAF6F0) | WhyTodayConfig.transitionDuration | easeInOut | WhyTodayConfig.transitionDelay | Screen appear (first per day) |
| **Why Today** | Text color | crossfade (#FFFFFF → #1A1714) | synced with bg | easeInOut | synced with bg | Synced with bg |
| **Why Today** | Message text | typewriter (char by char) | WhyTodayConfig.typewriterSpeed/char | linear | 0.0s | Screen appear (first per day) |
| **Why Today** | Direction text | typewriter | WhyTodayConfig.typewriterSpeed/char | linear | WhyTodayConfig.lineDelay after msg | After message completes |
| **Why Today** | CTA button | fadeIn + translateY(12→0) | 0.3s | easeOut | WhyTodayConfig.ctaDelay | After direction completes |
| Screen 3 | Recommended cards | fadeIn + translateY(12→0) | 0.3s | easeOut | 0.1s stagger | Screen appear |
| Screen 3 | General profile card | fadeIn + translateY(12→0) | 0.3s | easeOut | after last bean | Screen appear |
| Screen 3 | Browse section | fadeIn | 0.2s | easeOut | 0.3s | Scroll into view |
| Bean card | "TODAY'S PICK" tag | subtle pulse scale(1.0↔1.02) | 2.0s | easeInOut | — | Loop while visible |
| Bean detail | Image | fadeIn + scale(0.98→1.0) | 0.3s | easeOut | 0.0s | Screen load |
| Bean detail | Origin card | fadeIn + translateY(8→0) | 0.25s | easeOut | 0.15s | Screen load |
| Bean detail | Flavor section | fadeIn + translateY(8→0) | 0.25s | easeOut | 0.25s | Screen load |
| Bean detail | History card | fadeIn + translateY(8→0) | 0.25s | easeOut | 0.35s | Screen load |
| Flavor sliders | Dot position | spring(response: 0.4, dampingFraction: 0.8) | — | spring | 0.0s | Appear |
| Timing bar | Fill draw | width(0→target) | 0.4s | easeOut | 0.2s | Appear |
| Marketplace | Filter chip select | spring(response: 0.3, dampingFraction: 0.7) | — | spring | 0.0s | Tap |
| Marketplace | Bean card filter | matchedGeometryEffect | 0.3s | easeInOut | 0.0s | Filter change |

---

## 15. New Components — Summary

| Component | File | Purpose |
|---|---|---|
| `WhyTodayView.swift` | Screen 2 — emotional bridge | Palette transition, body-state message, coffee direction |
| `BeanCardView.swift` | Bean product card | 3 contexts (selection/marketplace/detail), 2 variants (light/dark) |
| `BeanCardCompactView.swift` | Bean card (compact) | Horizontal scroll variant — image + name + mini sliders |
| `BeanDetailView.swift` | Bean detail screen | Full-screen bean info, origin, flavor, history, purchase |
| `BeanSelectionView.swift` | Screen 3 — bean picker | Recommended beans + general profile card + browse |
| `BrewSettingsView.swift` | Screen 4 — brew params | Bean-optimized or general brew parameters |
| `MarketplaceView.swift` | Bean Shop | All beans, filtered by profile, subscribe banner |
| `FlavorSliderView.swift` | Flavor bar chart | Horizontal gradient slider with dot indicator |
| `TastingNoteView.swift` | Tasting note icon | Illustrated icon in rounded container + label |
| `ServingInfoCard.swift` | Serving infographic | Cup icons + bold cutoff time + timing bar |
| `TimingBarView.swift` | Day timeline bar | Horizontal bar showing brew-safe window |
| `FilterChipView.swift` | Profile filter pill | Pill-shaped filter chip for marketplace |
| `BeanBrewHistoryCard.swift` | Personal history | Brew count, best recovery, mini chart, insight |
| `GeneralProfileCard.swift` | General brew option | Equal-weight card for "no specific bean" |

---

## 16. New Data Requirements

### Bean Entity — Bundled JSON

For MVP with 3 beans, skip Core Data. Bundle as JSON decoded at launch:

```swift
// beans.json in app bundle
let beans: [Bean] = Bundle.main.decode("beans.json")
```

### BrewLog Extension

```swift
// Add to existing BrewLog entity
@NSManaged var selectedBeanID: String?      // nil = general profile
@NSManaged var selectedBeanName: String?    // denormalized for display
@NSManaged var recoveryScore: Int?          // recovery % on brew day (for history feature)
```

### UserDefaults Keys

```swift
let key_lastWhyTodayDate = "lastWhyTodayDate"       // String (yyyy-MM-dd)
let key_notifyBeans: [String] = []                    // Array of bean IDs user wants notifications for
let key_subscribedBeans: [String] = []                // ROUND 3: Array of bean IDs with active subscriptions
```

---

## 17. Decisions — Iteration 3, Round 3

All Round 2 decisions carry forward. Round 3 adds/modifies:

1. **Adaptation note at hinge position.** Moved from below the detail table to between the 3-box summary and the detail table, inside the parameter card. Gets warm background chip (#F5F0E8, 8pt radius). Principle: hook (big numbers) → context (adaptation note) → data (detail table). Elon's principle honored; execution modified for better reading flow.
2. **Price TBD banner when all visible beans are TBD.** "These beans are on deck. Tap NOTIFY ME and we'll ping you when they drop." Reframes absence as exclusivity. Prevents the marketplace from looking like a coming-soon page.
3. **Toast includes bean name.** "We'll let you know when Dark Field drops." Specificity signals real notification.
4. **Subscribe banner qualifier.** "Available on select beans." Honest framing when only Regent supports subscriptions at launch.
5. **WhyTodayConfig struct for tunable timing.** All animation constants in one configurable struct. TestFlight can tune without code changes. First lever if Skip rate is high: typewriterSpeed 0.02, transitionDuration 1.2.
6. **Nav icon: cart → bag.** No in-app cart exists. "bag" (SF Symbol) signifies shop entry without implying cart state. Internal reference renamed to marketplace.
7. **Subscribed state on Subscribe button.** "SUBSCRIBED ✓" with sage green border, disabled. Below price: "Subscribed — manage in Settings." MVP: UserDefaults flag. Post-MVP: Shopify webhook.
8. **Lead tasting note accent in wireframes.** First icon in array explicitly marked with `*` in all wireframes. Accent bg: #D4A96A at 20% (light) / 30% (dark). Was in spec but missing from wireframes in Round 2.
9. **First Light copy tightened.** line2: "Rebuilding. Your nervous system is doing the heavy lifting." (removed "Standard brew — nothing to prove."). direction: "Clean start. Nothing to prove." (was "These match the morning."). "Nothing to prove" hits harder as closer.
10. **Gradient banding quality gate.** If 5-stop LinearGradient shows visible banding on LCD (iPhone SE/11), reduce to 3 stops. Jensen tests in Phase 1.
11. **Localizable.strings in Phase 1.** All ~85 coffee-side strings go into localization infrastructure from day one. NSLocalizedString / LocalizedStringKey everywhere.

All 20 Round 2 decisions remain unchanged. See Round 2 spec Section 17 for the complete list.

---

## 18. Copy Audit — All Strings (Round 3 Final)

### Why Today (8 strings + 5 template messages)
```swift
"SEE YOUR BEANS"
"Skip"

// Per-profile messages (3 parts each — line1, line2, direction)
// tide_pool
"You're at {recovery}% today."
"Your body is asking for less. Honor it. Low dose, low temp — let your system coast."
"Something gentle. Let these do the talking."

// first_light — ROUND 3 UPDATED
"You're at {recovery}% today."
"Rebuilding. Your nervous system is doing the heavy lifting."
"Clean start. Nothing to prove."

// deep_work
"You're at {recovery}% today."
"Strong recovery. Your body has the runway. Full extraction — use what you've built."
"You need fuel. These are built for it."

// voltage
"You're at {recovery}% today."
"Peak state. Everything is green. Maximum dose, maximum extraction — send it."
"All systems go. Pick your weapon."

// the_blank
"You're at {recovery}% today."
"Manual mode. No recommendation — you know what you want today."
"Your call today. Here's what we've got."
```

### Dashboard Card 3 (7 strings)
```swift
"BREZI SUGGESTS"
"CHOOSE YOUR BEAN"
"Use general settings"
"BROWSE BEANS"

// Bridge copy per profile
"Low and slow today."
"Steady morning. Standard brew."
"You've got the runway. Go deep."
"All systems. Full send."
"Your call. Your brew."
```

### Bean Selection — Screen 3 (12 strings)
```swift
"RECOMMENDED BEANS"
"BREW WITH THIS"
"GENERAL BREW"
"BREW GENERAL"
"BROWSE ALL BEANS"
"VISIT BEAN SHOP"
"browse"
"TODAY'S PICK"

// Header copy per profile — ROUND 3: first_light updated
"Something gentle. Let these do the talking."
"Clean start. Nothing to prove."
"You need fuel. These are built for it."
"All systems go. Pick your weapon."
"Your call today. Here's what we've got."

// General profile card
"Optimized for {profileName}. Works with whatever's in your bag."
```

### Brew Settings — Screen 4 (9 strings)
```swift
"YOUR BREW"
"Optimized for:"
"× {profileName} profile"
"START BREW"
"Change bean"
"Select a specific bean"
"WHAT YOU'LL TASTE"
"BREW PARAMETERS"
"Using general\n{profileName} settings"
"No specific bean selected. These parameters work with most medium roasts."
"Standard extraction for this profile. Works with most beans in the medium roast range."
```

### Adaptation Notes (12 strings)
```swift
"Adapted for this bean's light roast — lower temp, longer steep to pull floral notes."
"Adapted for medium roast — standard extraction, balanced sweetness."
"Adapted for dark roast — shorter steep to avoid over-extraction. Gentle approach."
"Standard parameters for medium roast. Clean extraction, no adjustments needed."
"Slightly shorter steep for dark roast. Prevents bitterness, keeps it balanced."
"Extended steep pulls complexity from light roast. Full dose, full extraction."
"Medium roast at full extraction. Dense, sustaining, built for the session."
"Dark roast at full dose. Maximum density. The parameters match the intensity."
"High concentration from light roast. Fruit intensity amplified. Controlled chaos."
"Maximum extraction from medium roast. Sharp, concentrated, efficient."
"Dark roast at peak parameters. Everything's turned up. No subtlety, no apology."
"Standard extraction for this profile. Works with most beans in the medium roast range."
```

### Bean Detail Labels (9 strings)
```swift
"ORIGIN"
"WHAT YOU'LL TASTE"
"BREW PAIRING"
"SERVING GUIDE"
"Best for:"
"BREW WITH THIS"
"YOUR HISTORY"
"SUBSCRIBED"                        // ROUND 3
"Subscribed — manage in Settings"   // ROUND 3
```

### Purchase CTAs (10 strings)
```swift
"ADD TO CART"
"SUBSCRIBE & SAVE"
"SUBSCRIBE & SAVE 15%"
"or subscribe & save 15%"
"or subscribe & save 15% — ${price}/delivery"
"PRICE COMING SOON"
"NOTIFY ME"
"We'll let you know when {beanName} drops."  // ROUND 3: includes bean name
"Coming soon"
"or brew with this bean"
```

### Marketplace (12 strings)
```swift
"BEAN SHOP"
"Selected for cold brew. Matched to your morning."
"RECOMMENDED FOR YOU"
"Beans that match today's {profileName} profile."
"ALL BEANS"
"Never run out."
"Subscribe and save 15% on every delivery. Available on select beans. Cancel anytime."  // ROUND 3: qualifier
"SUBSCRIBE ON ANY BEAN"
"More roasters coming soon."
"Want to partner?"
"hello@brezicoffee.com"
"Nothing matched {profile} yet. Try {fallback} — similar energy, more options."
"These beans are on deck. Tap NOTIFY ME and we'll ping you when they drop."  // ROUND 3
```

### Serving Guide (5 strings)
```swift
"SERVING GUIDE"
"SERVING"
"BEST BEFORE"
"After this, caffeine competes with your sleep."
"brew window"
```

### Bean History (6 template strings)
```swift
"You've brewed this {n}×. Best day: recovery {score}%."
"Your best sessions with this bean were on recovery days above 70%."
"This bean works well for you across a range of recovery days."
"You tend to reach for this one on harder days. It holds up."
"BREWED {n}×"
"Best day: recovery {score}%."
```

### Bean Emotional Tags (3 strings — from roaster data)
```swift
"For when you mean business. Dense, structured, no apologies."
"Wine-dark. Fruit-forward. The kind of brew you linger over."
"The cleanest thing you'll drink today. Floral, bright, effortless."
```

### Settings (2 strings)
```swift
"Bean Shop"
"Manage Subscriptions"
```

**Total new strings: ~90** (including template variants and Round 3 additions)

---

## 19. What Doesn't Change

- Onboarding flow (Screens 1-5) — stays dark, stays locked
- Health Data Card (Card 1) — stays dark, stays WHOOP-like
- Brezi's Read (Card 2) — stays dark, blue left border
- Emotional layer (R4/R5) — check-in, surprise moments, return moments
- Brew profiles (Voltage, First Light, etc.) — names, parameters, decision logic
- Device Hub — stays dark
- Settings — stays dark (except new rows)
- All R3 design tokens for the health side
- All R4/R5 copy and emotional layer strings
- Navigation pattern (single push stack, no tab bar)
- All animations from R3/R4/R5
- Why Today palette transition (Elon: "the best screen in the app")
- Typewriter animation at 0.03s/char default (now tunable, but default unchanged)
- Bean card 3-context architecture (.selection/.marketplace/.detail)
- Per-roaster imageBgTint + 280pt image containers
- Dark card variant for Stitch (amber CTA)
- Corvus corrected data (Sadayana Winey, West Java)
- Regent 4th tasting note (Lime)
- "TODAY'S PICK" tag language
- General profile as equal card
- Card 3 bridge copy per profile
- History card (3+ brew threshold)
- CTA priority flip after 3+ brews
- Marketplace filter chips as profile names
- Empty filter state with guided fallback
- Serving infographic (cups + bold time + timing bar)
- SFSafariViewController for Shopify checkout
- Bean adjustment dictionary (6 recommended pairings, 9 graceful fallbacks)
- Skip button on Why Today
- Why Today once-per-day animation
- Flavor sliders (gradient + dot + value labels on full cards)

---

## 20. Build Order — For Jensen — ROUND 3 UPDATED

**Phase 1 (Days 1-2):** Dual palette tokens + `BeanCardView.swift` + `FlavorSliderView.swift` + `TastingNoteView.swift`. Get a single bean card rendering with real data in all three context modes. This is the visual proof-of-concept. **ROUND 3 addition:** Set up `Localizable.strings` with all ~90 coffee-side strings. All string references use `NSLocalizedString` / `LocalizedStringKey`. Test gradient rendering on iPhone 11 LCD — if banding visible, reduce to 3 color stops.

**Phase 2 (Days 3-4):** `WhyTodayView.swift` (Screen 2) + `BeanSelectionView.swift` (Screen 3) + `GeneralProfileCard.swift`. Wire the emotional bridge and bean-first flow. Dashboard Card 3 CTA changes. Typewriter animation + palette crossfade. **ROUND 3 addition:** `WhyTodayConfig` struct for tunable animation timing. Updated First Light copy.

**Phase 3 (Day 5):** `BrewSettingsView.swift` (Screen 4) + `ServingInfoCard.swift` + `TimingBarView.swift`. Brew parameters with bean-specific adjustments. Serving infographic with cup icons and timing bar. **ROUND 3 change:** Adaptation note at hinge position (between 3-box summary and detail table) with warm background chip.

**Phase 4 (Day 6):** `BeanDetailView.swift` + `BeanCardCompactView.swift`. Full detail screen with origin, flavor, brew pairing. Compact horizontal scroll variant. **ROUND 3 addition:** Subscribed button state (UserDefaults flag, disabled "SUBSCRIBED ✓" variant).

**Phase 5 (Day 7):** `MarketplaceView.swift` + `FilterChipView.swift`. Bean Shop with profile filtering, subscribe banner, guided empty states. Wire all access points (dashboard, nav bag icon, settings). **ROUND 3 changes:** Nav icon `bag` not `cart`. All-TBD banner. Bean-name-specific NOTIFY ME toast. Subscribe banner qualifier copy. Internal naming: marketplace not cart.

**Phase 6 (Day 8):** `BeanBrewHistoryCard.swift` + BrewLog extension + CTA priority flip logic. The retention feature. Mini chart component. History card placement in detail and marketplace.

**Phase 7 (Day 9):** Polish. Animations (Why Today typewriter, card staggers, slider springs). Dark/light card variant edge cases. Navigation flow testing. Post-brew bean name line. Subscribe & Save Shopify deep-links. Lead tasting note icon accent bg on all card contexts.

**Total: ~9 days.** Round 3 changes add zero build days — all changes fold into existing phases. Can be parallelized — Phase 1 components are independent of Phase 2 flow work.

### Asset Dependencies (blocking)

- **15 tasting note illustrations** — need from designer before Phase 1 is visually complete. Can use SF Symbol placeholders until then.
- **3 bean bag product photos** — need from roaster partners. Can use placeholder images.
- **1 general brew illustration** — for the general profile card. Can use a simple coffee bag silhouette placeholder.
- **Roaster logos** — optional. The roaster name in ALL CAPS serves as the identity.

---

*Round 1 built the machine. Round 2 gave it a soul. Round 3 sharpened the edges. The adaptation note found its home at the hinge — between hook and data, where context lives. The marketplace handles its empty shelves with grace. The hero animation became tunable. The nav bar stopped lying about a cart that doesn't exist. First Light's direction line lands harder with "Nothing to prove."*

*Nothing structural moved because nothing structural needed to. Every Round 3 change is a layout tweak, a copy change, a config struct, or a conditional banner. The 9-day estimate holds. Jensen can start Phase 1.*

*The health side reads your body. The coffee side reads like a menu at your favorite roaster. Why Today is the moment where they shake hands. Round 3 made that handshake crisper.*

— Picasso
```
