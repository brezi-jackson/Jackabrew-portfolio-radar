# Brezi iOS App — Visual Coffee Layer & Marketplace v5.0 (FINAL)
**Author:** Picasso (Product Design) | March 20, 2026 | Round 5 — Definitive Spec
**For:** Jackson (Product) · Jensen (Engineering) · Elon (Growth/Copy)
**Status:** FINAL. This is Jensen's build document. Every screen, every component, every copy string, every pixel.

---

## 0. What This Document Is

This is the single source of truth for the Brezi coffee-side experience. It incorporates five rounds of design, four rounds of critique, one brief alignment audit (14/14), and four approved v1 extensions. Jensen builds from this document. Nothing else.

### What Changed Since Round 3

| Change | Source | Why |
|--------|--------|-----|
| **General settings shortcut gated** | Elon R4, Picasso R4 discussion | New users must experience the palette transition 3x before the shortcut appears. |
| **State-bean bridge lines (Extension 1)** | Elon R5 approved for v1 | One sentence on the recommended bean card connecting body state to bean. Uses roaster emotional tag vocabulary per Elon's copy fix. |
| **Why Today maturation (Extension 3)** | Elon R5 approved for v1 | Copy evolves: new (Day 1-7) → familiar (Day 8-30) → veteran (Day 31+). |
| **Full-day emotional arc (Extension 6)** | Elon R5 approved for v1 | Card 3 copy changes across the day: pre-brew → recent → afternoon → evening. |
| **Return moments (Extension 7)** | Elon R5 approved for v1 | 3+ day absence triggers return-specific Why Today copy and suppression reset. |
| **Bridge lines use roaster vocabulary** | Elon R5 fix #1 | "Floral, bright origin for a low morning" not "Gentle origin for a low morning." |
| **Afternoon Tide Pool copy fix** | Elon iteration-2 signoff | "That's all it needed to be" replaces "done its job." |
| **Maturation increment on display only** | Elon iteration-3 signoff | Skipped Why Today does not count toward familiarity. |
| **Veteran template: render-time only** | Elon iteration-3 signoff | Never cache resolved veteran copy. Resolve `{lastPeakDay}` at display time. |
| **Return reset before suppression check** | Elon iteration-3 signoff | `checkReturnReset()` runs before `shouldShowWhyToday()`. |
| **Nav icon: cart → bag** | Round 3 | No in-app cart. "bag" signifies shop entry. |
| **Subscribed button state** | Round 3 | "SUBSCRIBED ✓" disabled state. UserDefaults flag for MVP. |
| **Price TBD banner** | Round 3 | "These beans are on deck" when all visible beans lack pricing. |
| **Toast includes bean name** | Round 3 | "We'll let you know when Dark Field drops." |
| **Subscribe banner qualifier** | Round 3 | "Available on select beans." |
| **WhyTodayConfig struct** | Round 3 | All animation timing tunable without code changes. |
| **Adaptation note at hinge position** | Round 3 | Between 3-box summary and detail table. Warm background chip. |
| **Gradient banding quality gate** | Round 3 | Test 5-stop gradients on LCD devices. Fall back to 3 stops if banding visible. |
| **Localizable.strings in Phase 1** | Round 3 | All ~95 coffee-side strings in localization infrastructure from day one. |

### What This Document Covers

Complete screen-by-screen specs for all 6 screens in the coffee-side flow, plus marketplace, plus all components, copy, data models, animations, and the four approved v1 extensions.

### What This Does NOT Cover

- Onboarding (Screens 1-5)
- Health data card (Card 1), Brezi's Read (Card 2)
- Device hub, settings (except new rows)
- All R3/R4/R5 health-side design
- **v1.1 deferred extensions:** Four-node "Why This Brew" chain (Extension 2), state-aware flavor framing (Extension 4), pattern vocabulary (Extension 5)

---

## 1. Dual Palette System — The Two Worlds

### The Principle

The app is two brands sharing one shell. The health side is a wearable dashboard. The coffee side is a specialty roaster's shelf. They meet at one seam — Why Today. Everything above the seam is cool. Everything below is warm.

### Health Side Palette (unchanged)

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

**Where it lives:** Dashboard Cards 1-2, onboarding, device hub, settings, all health metrics.

**Dashboard background:** `#F5F5F5` (cool neutral grey). Dark cards float on light grey. Intentional — clinical, precise feel. The transition to coffee world is a *temperature* change (cool grey → warm cream), not just a lightness change.

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
// Coffee side type scale — warm-toned colors, roaster-specific styles
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

---

## 3. Screen 1: Dashboard — `DashboardView.swift`

**Existing dashboard, with Card 3 modified.**

Card 3 shows the **profile suggestion**, a bridge line connecting body state to coffee direction, and a CTA to enter the coffee world.

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
│  │  Use general settings ›        │ │  SECONDARY CTA (GATED — see below)
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

### General Settings Shortcut Gate — ROUND 4 ADDITION

The "Use general settings >" secondary CTA on Card 3 is **hidden for new users**. It appears only after the user has completed the full brew flow 3+ times.

```swift
// Card 3 — Your Brew
var showGeneralShortcut: Bool {
    UserDefaults.standard.integer(forKey: "completedBrewFlows") >= 3
}

// Increment ONLY on full-flow completion:
// Why Today → Bean Selection → Brew Settings → Active Brew (Screen 5)
func recordFullFlowCompletion() {
    let current = UserDefaults.standard.integer(forKey: "completedBrewFlows")
    UserDefaults.standard.set(current + 1, forKey: "completedBrewFlows")
}

// "Use general settings" via shortcut (once unlocked) does NOT increment the counter.
// The counter tracks full-flow completions only.
```

**Before gate unlocks:** Card 3 shows only "CHOOSE YOUR BEAN →". No secondary link. The only forward path is into Why Today.

**After gate unlocks:** Card 3 shows both CTAs as designed.

**Why:** The palette transition is the hero brand moment. New users must experience it at least 3 times before being offered a bypass.

### Full-Day Emotional Arc — Card 3 Post-Brew States (Extension 6)

Card 3 changes copy across the day after a brew is logged:

**Pre-brew (default state):** As shown above — profile suggestion + CTAs.

**Recent (0-2 hours post-brew):**
```
│  ┌─ 3  YOUR BREW ─────────────────┐ │
│  │                                 │ │  bg: #0F0F0F
│  │  FIRST LIGHT                    │ │  CondensedBold, 32px, #FFFFFF
│  │  Regent Ethiopia Guji Hambela   │ │  Mono, 13px, #888888 [bean name]
│  │  7:14am · 12 min                │ │  Mono, 14px, #888888
│  │                                 │ │
│  │  Brewing. Steep in progress.    │ │  Regular, 14px, #CCCCCC
│  │  — or —                         │ │
│  │  Done. How did it land?         │ │  (post-brew, pre-rating)
│  │                                 │ │
│  │  ┌─────────────────────────┐   │ │  RATING CTA (visible)
│  │  │     RATE THIS BREW      │   │ │  bg: #FFFFFF, text: #000000
│  │  └─────────────────────────┘   │ │
│  └─────────────────────────────────┘ │
```

**Afternoon (2-6 hours post-brew):**
```swift
let card3_afternoon: [String: String] = [
    "tide_pool":   "Brewed this morning. Gentle start. That's all it needed to be.",  // FIXED per Elon signoff
    "first_light": "This morning's brew. Balanced, finished clean.",
    "deep_work":   "Dense cup earlier. Still carrying the momentum.",
    "voltage":     "Full send this morning. The signal's still warm.",
    "the_blank":   "Manual brew earlier. Your call, as always."
]
// Rating CTA still visible in afternoon phase.
```

**Evening (6+ hours post-brew):**
```swift
let card3_evening: [String: String] = [
    "tide_pool":   "Today's cup is behind you. Rest well.",
    "first_light": "Today's cup is behind you. Sleep well — tomorrow's signal starts now.",
    "deep_work":   "Session's done. Let the system rebuild overnight.",
    "voltage":     "Everything was up today. Let it come down. Tomorrow resets.",
    "the_blank":   "Today's brew is logged. Tomorrow is a clean read."
]
// Rating CTA hidden in evening phase. User has had enough time.
```

**Phase transition logic:**
```swift
func card3PostBrewPhase(brewTime: Date) -> PostBrewPhase {
    let hoursSinceBrew = Date().timeIntervalSince(brewTime) / 3600
    switch hoursSinceBrew {
    case ..<2:  return .recent
    case 2..<6: return .afternoon
    default:    return .evening
    }
}

enum PostBrewPhase {
    case preBrew, recent, afternoon, evening
}

// Phase transition on app foreground: crossfade 0.3s easeInOut
// Re-evaluate phase each time the app comes to foreground
```

### Bridge Copy Per Profile (exact strings)

```swift
let card3_bridgeCopy: [String: String] = [
    "tide_pool":   "Low and slow today.",
    "first_light": "Steady morning. Standard brew.",
    "deep_work":   "You've got the runway. Go deep.",
    "voltage":     "All systems. Full send.",
    "the_blank":   "Your call. Your brew."
]
```

### Copy (exact strings)

```swift
let card3_label        = "BREZI SUGGESTS"
let card3_cta_bean     = "CHOOSE YOUR BEAN"
let card3_cta_general  = "Use general settings"
let dashboard_browseBeans = "BROWSE BEANS"
```

### Navigation Behavior

- **"CHOOSE YOUR BEAN":** Pushes to Screen 2: Why Today.
- **"Use general settings" (gated):** Pushes directly to Screen 4: Brew Settings with `selectedBean = nil`, skipping Why Today and Bean Selection.
- **"BROWSE BEANS":** Pushes to Marketplace (Bean Shop).

---

## 4. Screen 2: Why Today — `WhyTodayView.swift`

### The Principle

Why Today is the emotional bridge. The screen where Brezi stops being a dashboard and starts being a companion. The language shifts from metrics to meaning. The palette shifts from dark to warm. The typography shifts from monospace data to human sentences.

This screen IS the palette transition. This screen IS the hero brand moment.

### The Transition — How It Works

The screen opens dark (`#0F0F0F`). As the message renders (typewriter-style, character by character), the background crossfades to warm cream (`#FAF6F0`). By the time the message is fully rendered, the world is warm. The transition happened *around a feeling*, not around a navigation push.

```swift
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
│  ┌─────────────────────────────────┐│
│  │       SEE YOUR BEANS  →        ││  CTA — appears after text completes
│  └─────────────────────────────────┘│  fadeIn + translateY(12→0)
│                                     │  delay: WhyTodayConfig.ctaDelay
│  Skip ›                             │  skip link — always visible
│                                     │  Mono, 13px, #B5A99A
│                                     │  44×44pt tap area
│                                     │  bottom padding: 48pt + safe area
└─────────────────────────────────────┘
```

### Layout Spec

```swift
struct WhyTodaySpec {
    let horizontalPadding: CGFloat = 32
    let verticalAlignment: Alignment = .center

    // Line 1 + 2 (the message)
    let messageFont = Font.system(size: 24, weight: .regular)
    let messageLineHeight: CGFloat = 1.5

    // Line 3 (the coffee direction)
    let directionFont = Font.system(size: 18, weight: .regular)
    let directionLineHeight: CGFloat = 1.4
    let directionTopPadding: CGFloat = 24

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

### Copy — Base Per Profile (exact strings)

Each profile generates a unique Why Today message with three parts:
1. **The data point** — what the health read says
2. **The interpretation** — what it means for the body
3. **The coffee direction** — what kind of brew this morning calls for

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
            line2: "Rebuilding. Your nervous system is doing the heavy lifting.",
            direction: "Clean start. Nothing to prove."
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

### Why Today Maturation — Extension 3 (v1)

Copy evolves as the user accumulates sessions. Three tiers:

```swift
enum WhyTodayMaturity {
    case new       // sessions 1-7: the app explains itself
    case familiar  // sessions 8-30: the app trusts you
    case veteran   // sessions 31+: the app knows your history
}

func maturityTier() -> WhyTodayMaturity {
    let count = UserDefaults.standard.integer(forKey: "whyTodaySessionCount")
    switch count {
    case 0..<8:   return .new
    case 8..<31:  return .familiar
    default:      return .veteran
    }
}

// CRITICAL: Increment whyTodaySessionCount on DISPLAY, not on app open or skip.
// A skipped Why Today does not count toward the familiarity that earns shorter copy.
func recordWhyTodayInteraction(wasSkip: Bool) {
    if !wasSkip {
        let count = UserDefaults.standard.integer(forKey: "whyTodaySessionCount")
        UserDefaults.standard.set(count + 1, forKey: "whyTodaySessionCount")
    }
}
```

#### New Tier Copy (Days 1-7) — Base Copy

Uses the base `whyTodayCopy()` function above. The app is explaining itself. Full sentences. Context-setting.

#### Familiar Tier Copy (Days 8-30)

Shorter. The app trusts the user understands the system.

```swift
func whyTodayCopyFamiliar(profile: String, recoveryPercent: Int) -> WhyTodayCopy {
    switch profile {
    case "tide_pool":
        return WhyTodayCopy(
            line1: "You're at \(recoveryPercent)%.",
            line2: "Low morning. Honor it.",
            direction: "Gentle cup. Let it coast."
        )
    case "first_light":
        return WhyTodayCopy(
            line1: "\(recoveryPercent)%.",
            line2: "Full signal. You know what to do with this.",
            direction: "Clean start."
        )
    case "deep_work":
        return WhyTodayCopy(
            line1: "\(recoveryPercent)%. Strong.",
            line2: "Full extraction. Use what you've built.",
            direction: "Fuel up."
        )
    case "voltage":
        return WhyTodayCopy(
            line1: "\(recoveryPercent)%. Peak.",
            line2: "Everything's green. Send it.",
            direction: "Pick your weapon."
        )
    case "the_blank":
        return WhyTodayCopy(
            line1: "\(recoveryPercent)%.",
            line2: "Manual mode. Your call.",
            direction: "Here's what we've got."
        )
    default:
        return whyTodayCopyFamiliar(profile: "first_light", recoveryPercent: recoveryPercent)
    }
}
```

#### Veteran Tier Copy (Day 31+)

The app knows your history. Uses dynamic references.

```swift
func whyTodayCopyVeteran(profile: String, recoveryPercent: Int) -> WhyTodayCopy {
    // Resolve dynamic references at DISPLAY TIME only. Never cache resolved strings.
    let lastPeakDay = resolveLastPeakDay()     // e.g. "Tuesday" or nil
    let midCount = resolveMidCount()           // e.g. "14" or nil
    let daysSinceLow = resolveDaysSinceLow()   // e.g. "8" or nil

    switch profile {
    case "tide_pool":
        let line2 = daysSinceLow != nil
            ? "Low signal. \(daysSinceLow!) days since the last one. It passes."
            : "Low morning. Honor it."  // fallback to familiar
        return WhyTodayCopy(
            line1: "\(recoveryPercent)%.",
            line2: line2,
            direction: "Gentle cup."
        )
    case "first_light":
        let line2 = lastPeakDay != nil
            ? "Best signal since \(lastPeakDay!). Act on it."
            : "Full signal. You know what to do with this."  // fallback to familiar
        return WhyTodayCopy(
            line1: "\(recoveryPercent)%.",
            line2: line2,
            direction: "Clean start."
        )
    case "deep_work":
        let line2 = midCount != nil
            ? "Steady. Your most common state — \(midCount!) sessions and counting."
            : "Strong recovery. Use it."  // fallback to familiar
        return WhyTodayCopy(
            line1: "\(recoveryPercent)%.",
            line2: line2,
            direction: "Full extraction."
        )
    case "voltage":
        let line2 = lastPeakDay != nil
            ? "Peak state. Last time: \(lastPeakDay!). Make it count."
            : "Everything's green. Send it."  // fallback to familiar
        return WhyTodayCopy(
            line1: "\(recoveryPercent)%. Peak.",
            line2: line2,
            direction: "All systems."
        )
    case "the_blank":
        return WhyTodayCopy(
            line1: "\(recoveryPercent)%.",
            line2: "Manual. Your call.",
            direction: "Here's what we've got."
        )
    default:
        return whyTodayCopyFamiliar(profile: "first_light", recoveryPercent: recoveryPercent)
    }
}

// TEMPLATE VARIABLE SAFETY RULE:
// NEVER render a template variable ({lastPeakDay}, {midCount}, etc.) to the user.
// If any dynamic reference resolves to nil, fall back to the familiar-tier copy.
// This is the single most trust-destroying bug possible. Unit test every nil combination.

func resolveLastPeakDay() -> String? {
    guard let dateStr = UserDefaults.standard.string(forKey: "lastSessionDate_high"),
          let date = DateFormatter.yyyyMMdd.date(from: dateStr) else { return nil }
    let formatter = DateFormatter()
    formatter.dateFormat = "EEEE"  // "Tuesday"
    return formatter.string(from: date)
}

func resolveMidCount() -> String? {
    let count = UserDefaults.standard.integer(forKey: "tierSessionCount_mid")
    return count > 0 ? "\(count)" : nil
}

func resolveDaysSinceLow() -> String? {
    guard let dateStr = UserDefaults.standard.string(forKey: "lastSessionDate_low"),
          let date = DateFormatter.yyyyMMdd.date(from: dateStr) else { return nil }
    let days = Calendar.current.dateComponents([.day], from: date, to: Date()).day ?? 0
    return days > 0 ? "\(days)" : nil
}
```

#### Session Tracking for Veteran Copy

```swift
func recordWhyTodaySession(tier: String) {
    // Record last session date for this tier
    let today = DateFormatter.yyyyMMdd.string(from: Date())
    UserDefaults.standard.set(today, forKey: "lastSessionDate_\(tier)")

    // Increment tier session count
    let key = "tierSessionCount_\(tier)"
    let count = UserDefaults.standard.integer(forKey: key)
    UserDefaults.standard.set(count + 1, forKey: key)
}
```

### Return Moments — Extension 7 (v1)

When the user hasn't opened the app in 3+ days, Why Today acknowledges the absence.

```swift
func checkReturnReset() {
    guard let lastDate = UserDefaults.standard.string(forKey: "lastWhyTodayDate"),
          let last = DateFormatter.yyyyMMdd.date(from: lastDate) else { return }

    let daysSince = Calendar.current.dateComponents([.day], from: last, to: Date()).day ?? 0

    if daysSince >= 3 {
        // Clear any active suppression — returning user always sees Why Today
        UserDefaults.standard.set("", forKey: "whyTodaySuppressedUntil")
        UserDefaults.standard.set(false, forKey: "whyTodayReintroduced")
        UserDefaults.standard.set(0, forKey: "consecutiveWhyTodaySkips")
    }
}

// CRITICAL: Call checkReturnReset() BEFORE shouldShowWhyToday() on app launch.
// Order matters. The reset clears suppression so the returning user sees Why Today fresh.

func isReturnSession() -> Bool {
    guard let lastDate = UserDefaults.standard.string(forKey: "lastWhyTodayDate"),
          let last = DateFormatter.yyyyMMdd.date(from: lastDate) else { return false }
    let daysSince = Calendar.current.dateComponents([.day], from: last, to: Date()).day ?? 0
    return daysSince >= 3
}

func daysSinceLastSession() -> Int {
    guard let lastDate = UserDefaults.standard.string(forKey: "lastWhyTodayDate"),
          let last = DateFormatter.yyyyMMdd.date(from: lastDate) else { return 0 }
    return Calendar.current.dateComponents([.day], from: last, to: Date()).day ?? 0
}
```

#### Return Copy

```swift
func whyTodayCopyReturn(profile: String, recoveryPercent: Int, daysSince: Int) -> WhyTodayCopy {
    let prefix = "\(daysSince) days since the last one. \(recoveryPercent)% recovery."

    switch profile {
    case "tide_pool":
        return WhyTodayCopy(
            line1: prefix,
            line2: "You're back. Low signal. Ease in. The routine remembers you.",
            direction: "Gentle cup. Welcome back."
        )
    case "first_light":
        return WhyTodayCopy(
            line1: prefix,
            line2: "You're back. Mid-range signal. Normal morning. Start there.",
            direction: "Same place. Same morning. Let's go."
        )
    case "deep_work":
        return WhyTodayCopy(
            line1: prefix,
            line2: "You're back. And you came back strong. Pick up where you left off.",
            direction: "Full extraction. Welcome back."
        )
    case "voltage":
        return WhyTodayCopy(
            line1: prefix,
            line2: "You're back. And you came back strong. Pick up where you left off.",
            direction: "All systems. Let's go."
        )
    case "the_blank":
        return WhyTodayCopy(
            line1: prefix,
            line2: "You're back. Manual mode. Your call.",
            direction: "Here's what we've got."
        )
    default:
        return whyTodayCopyReturn(profile: "first_light", recoveryPercent: recoveryPercent, daysSince: daysSince)
    }
}
```

### Master Copy Resolver

```swift
func resolveWhyTodayCopy(profile: String, recoveryPercent: Int) -> WhyTodayCopy {
    // 1. Return session check (highest priority)
    if isReturnSession() {
        let days = daysSinceLastSession()
        return whyTodayCopyReturn(profile: profile, recoveryPercent: recoveryPercent, daysSince: days)
    }

    // 2. Maturation tier
    switch maturityTier() {
    case .new:
        return whyTodayCopy(profile: profile, recoveryPercent: recoveryPercent)
    case .familiar:
        return whyTodayCopyFamiliar(profile: profile, recoveryPercent: recoveryPercent)
    case .veteran:
        return whyTodayCopyVeteran(profile: profile, recoveryPercent: recoveryPercent)
    }
}
```

### Interaction Details

- **First visit of the day:** Full typewriter render + palette transition. ~4-6 seconds total.
- **Return visits same day:** Message appears instantly, background starts at `#FAF6F0`.
- **State tracking:** `UserDefaults` key `lastWhyTodayDate`.

```swift
func shouldAnimateWhyToday() -> Bool {
    let lastDate = UserDefaults.standard.string(forKey: "lastWhyTodayDate")
    let today = DateFormatter.yyyyMMdd.string(from: Date())
    if lastDate == today { return false }
    UserDefaults.standard.set(today, forKey: "lastWhyTodayDate")
    return true
}
```

### CTA + Navigation

```swift
let whyToday_cta  = "SEE YOUR BEANS"
let whyToday_skip = "Skip"
```

- **"SEE YOUR BEANS":** Pushes to Screen 3: Bean Selection.
- **"Skip":** Same destination. Skip is for returning users who've seen Why Today many times.

---

## 5. Bean Card Visual Design — `BeanCardView.swift`

### The Card

Each bean card is a self-contained product card. It lives in three contexts: bean selection (Screen 3), the marketplace, and bean detail. One component, three rendering modes.

**Two visual variants** based on roast level:
- **Light/Medium roast:** white card (`#FFFFFF`) on cream background
- **Dark roast:** dark card (`#2A2520`) — inverts text colors

**Three context modes:**
- **`.selection`** — Screen 3. Brew intent. Shows "BREW WITH THIS" only.
- **`.marketplace`** — Bean Shop. Purchase intent. Shows price + "ADD TO CART" + subscribe.
- **`.detail`** — BeanDetailView. Both intents, full information.

### Layout — Light/Medium Roast, Selection Context (Regent, Corvus)

```
┌─────────────────────────────────────┐
│                                     │  bg: #FFFFFF
│  ┌─────────────────────────────┐    │  radius: 20pt
│  │                             │    │  shadow: coffee_card_shadow
│  │      [BEAN BAG IMAGE]       │    │  280pt tall
│  │      contentMode: .fit      │    │  edge-to-edge within card radius
│  │                             │    │  clipped by card clipShape
│  └─────────────────────────────┘    │  image container: per-bean imageBgTint
│                                     │
│  REGENT COFFEE                      │  t_roaster_name, #8C8279, ALL CAPS
│                                     │  tracking: 15%, top padding: 16pt
│  Ethiopia Guji                      │  t_bean_name, 22px bold, #1A1714
│  Hambela Washed                     │  line-height: 1.2, top padding: 4pt
│                                     │
│  from $27                           │  Mono, 12px, #B5A99A (selection context)
│                                     │
│  ┌──────┐ ┌──────┐ ┌──────┐ ┌────┐ │
│  │* 🍫 *│ │  🌸  │ │  🍓  │ │ 🍋 ││  TASTING NOTE ICONS
│  │Cocoa │ │Jasmi-│ │Straw-│ │Lime││  * = lead icon accent bg (#D4A96A @ 20%)
│  │      │ │ne    │ │berry │ │    ││  others: standard #F5F0E8
│  └──────┘ └──────┘ └──────┘ └────┘ │  top padding: 12pt
│                                     │
│  ROAST   ░░▓░░░░░░░░░░░░░░░░       │  FLAVOR SLIDERS (Section 10)
│           Light                     │
│  SWEET   ░░░░░░░░░░░░▓░░░░░░       │
│           High                      │
│  ACID    ░░░░░░░░░░░░▓░░░░░░       │
│           Medium-High               │
│                                     │
│  ┌──────────────────────────────┐   │
│  │  ★ TODAY'S PICK             │   │  RECOMMENDATION TAG (conditional)
│  └──────────────────────────────┘   │  top padding: 12pt
│                                     │
│  Floral, bright origin for a low    │  STATE-BEAN BRIDGE LINE (Extension 1)
│  morning. Effortless match.         │  Mono italic, 13px, #8C8279
│                                     │  ONLY on first (recommended) bean card
│                                     │  top padding: 8pt
│                                     │
│  ┌─────────────────────────────┐    │  SELECTION CONTEXT CTA
│  │    BREW WITH THIS  →        │    │  bg: #5B8C5A, text: #FFFFFF
│  └─────────────────────────────┘    │  CondensedBold, 14px, ALL CAPS
│                                     │  height: 44pt, radius: 10pt
│                                     │  top padding: 16pt, bottom: 20pt
└─────────────────────────────────────┘
```

### Layout — Dark Roast, Selection Context (Stitch)

```
┌─────────────────────────────────────┐
│                                     │  bg: #2A2520
│  ┌─────────────────────────────┐    │  radius: 20pt
│  │      [BEAN BAG IMAGE]       │    │  280pt tall
│  │      Stitch bear + Fiat     │    │  imageBgTint: #2E2022
│  └─────────────────────────────┘    │
│                                     │
│  STITCH COFFEE                      │  t_roaster_name, #B5A99A, ALL CAPS
│                                     │
│  Dark Field                         │  t_bean_name, 22px bold, #FFFFFF
│  Blend                              │
│                                     │
│  For when you mean business.        │  EMOTIONAL TAG (Stitch only)
│  Dense, structured, no apologies.   │  Regular italic, 13px, #B5A99A
│                                     │
│  ┌──────┐ ┌──────┐ ┌──────┐        │
│  │* 🍫 *│ │  🌰  │ │  🍮  │        │  icons on dark bg
│  │Dark  │ │Toast-│ │Cara- │        │  * = lead icon accent bg (#D4A96A @ 30%)
│  │Choc  │ │ed    │ │mel   │        │  labels: #B5A99A, icon color: #D4A96A
│  │      │ │Almond│ │      │        │
│  └──────┘ └──────┘ └──────┘        │
│                                     │
│  ROAST   ░░░░░░░░░░░░░░░░░▓░       │  slider track: #3A3530
│           Dark                      │
│  SWEET   ░░░░░░░░▓░░░░░░░░░░       │
│           Medium                    │
│  ACID    ░░▓░░░░░░░░░░░░░░░░       │
│           Low                       │
│                                     │
│  ┌──────────────────────────────┐   │  tag bg: rgba(212,169,106,0.2)
│  │  ★ TODAY'S PICK             │   │  text: #D4A96A
│  └──────────────────────────────┘   │
│                                     │
│  ┌─────────────────────────────┐    │
│  │    BREW WITH THIS  →        │    │  bg: #D4A96A, text: #2A2520
│  └─────────────────────────────┘    │  (dark card variant CTA)
└─────────────────────────────────────┘
```

### Layout — Marketplace Context (purchase intent)

Same card structure, CTA block changes:

```
│                                     │
│  ┌─────────┐                        │
│  │  $27.00 │          ┌──────────┐  │  PRICE + CTA
│  └─────────┘          │ ADD TO   │  │  t_price, 18px bold mono, #1A1714
│  or subscribe         │  CART →  │  │  button: bg #1A1714, text #FFFFFF
│  & save 15%           └──────────┘  │  height: 44pt, radius: 12pt
│                                     │
│  or brew with this bean ›           │  secondary: Mono, 13px, #8C8279
│                                     │  bottom padding: 20pt
└─────────────────────────────────────┘
```

**Dark card marketplace CTA:** Price: #FFFFFF. "ADD TO CART": bg #D4A96A, text #2A2520. Subscribe text: #B5A99A. Brew link: #B5A99A.

### State-Bean Bridge Lines — Extension 1 (v1)

One sentence below the recommendation tag on the **first (recommended) bean card only**. Connects body state to bean. Uses each roaster's emotional tag vocabulary — not generic origin language.

```swift
// Bridge lines: 3 beans × 4 recovery states + blank
// Uses roaster vocabulary per Elon R5 fix #1

let bridgeLines: [String: [String: String]] = [
    "regent_ethiopia_guji_hambela": [
        "high":  "Floral, bright — built for a morning like this. Effortless match.",
        "mid":   "Floral, bright origin for a steady morning. Clean cup, clean start.",
        "low":   "Floral, bright origin for a low morning. Effortless match.",
        "blank": "Floral, bright, effortless. Your call on the rest."
    ],
    "stitch_dark_field": [
        "high":  "Dense, structured — peak day deserves peak intensity. No apologies.",
        "mid":   "Dense, structured origin for a steady morning. No apologies needed.",
        "low":   "Dense, structured origin on a low day. The dose is the bet. Bean holds up.",
        "blank": "Dense, structured, no apologies. Your parameters."
    ],
    "corvus_sadayana_winey": [
        "high":  "Wine-dark, fruit-forward — peak state meets peak complexity. Linger.",
        "mid":   "Wine-dark origin for a deep work day. Linger over this one.",
        "low":   "Wine-dark origin on a low morning. Not what the data suggests. Bean's ready if you are.",
        "blank": "Wine-dark. Fruit-forward. Your call on the extraction."
    ]
]

// Display logic:
// - Only on FIRST bean card in BeanSelectionView (the recommended one)
// - NOT on second/third bean cards
// - NOT on General Profile card
// - NOT in marketplace context
// - NOT in compact cards
func showBridgeLine(bean: Bean, isFirstRecommended: Bool, context: BeanCardContext) -> Bool {
    return isFirstRecommended && context == .selection
}
```

**Spec:**
- Font: Mono italic, 13px, `#8C8279`
- Top padding: 8pt (below recommendation tag or below sliders if no tag)
- Line-height: 1.5
- Max 2 lines

### Component Spec — `BeanCardView.swift`

```swift
struct BeanCardSpec {
    // Container
    let cardRadius: CGFloat = 20
    let cardPaddingH: CGFloat = 16
    let cardPaddingBottom: CGFloat = 20

    // Image container
    let imageHeight: CGFloat = 280
    let imageTopRadius: CGFloat = 20
    let imageBottomRadius: CGFloat = 0

    // Roaster name
    let roasterFont = Font.system(size: 11, weight: .semibold)  // ALL CAPS
    let roasterTracking: CGFloat = 0.15
    let roasterTopPadding: CGFloat = 16
    let roasterColorLight = Color(hex: "#8C8279")
    let roasterColorDark  = Color(hex: "#B5A99A")

    // Bean name
    let beanNameFont = Font.system(size: 22, weight: .bold)
    let beanNameTopPadding: CGFloat = 4
    let beanNameLineHeight: CGFloat = 1.2
    let beanNameColorLight = Color(hex: "#1A1714")
    let beanNameColorDark  = Color(hex: "#FFFFFF")

    // Price metadata (selection context)
    let priceMetaFont = Font.system(size: 12, weight: .regular, design: .monospaced)
    let priceMetaColor = Color(hex: "#B5A99A")
    let priceMetaTopPadding: CGFloat = 4

    // Emotional tag (Stitch only — showEmotionalTagOnCard: true)
    let emotionalTagFont = Font.system(size: 13, weight: .regular).italic()
    let emotionalTagColor = Color(hex: "#B5A99A")
    let emotionalTagTopPadding: CGFloat = 4

    // Tasting notes row
    let tastingNoteTopPadding: CGFloat = 12
    let tastingNoteIconSize: CGFloat = 36
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
    let tagIcon       = "star.fill"  // SF Symbol, 10pt
    let tagCopy       = "TODAY'S PICK"

    // Bridge line (Extension 1 — first recommended card only)
    let bridgeLineFont = Font.system(size: 13, weight: .regular, design: .monospaced).italic()
    let bridgeLineColor = Color(hex: "#8C8279")
    let bridgeLineTopPadding: CGFloat = 8
    let bridgeLineLineHeight: CGFloat = 1.5

    // Selection CTA — BREW WITH THIS
    let brewCtaHeight: CGFloat = 44
    let brewCtaRadius: CGFloat = 10
    let brewCtaFont = Font.system(size: 14, weight: .bold)  // ALL CAPS
    let brewCtaBgLight    = Color(hex: "#5B8C5A")
    let brewCtaTextLight  = Color(hex: "#FFFFFF")
    let brewCtaBgDark     = Color(hex: "#D4A96A")
    let brewCtaTextDark   = Color(hex: "#2A2520")
    let brewCtaTopPadding: CGFloat = 16

    // Marketplace CTA — ADD TO CART
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
    case detail       // BeanDetailView — both intents
}
```

### Tag & Emotional Tag Display Logic

```swift
func showRecommendationTag(bean: Bean, suggestedProfile: BrewProfile) -> Bool {
    return bean.bestForProfiles.contains(suggestedProfile.key)
}

func showEmotionalTagOnCard(bean: Bean) -> Bool {
    return bean.showEmotionalTagOnCard  // true for Stitch, false for others
}
```

### The 3 Real Bean Cards — Complete Data

#### Bean 1: Stitch Coffee — Dark Field Blend

```swift
let stitch_darkField = Bean(
    id: "stitch_dark_field",
    roasterName: "STITCH COFFEE",
    roasterLocation: "Singapore · Roasted in Sydney",
    beanName: "Dark Field",
    beanSubtitle: "Blend",
    roastLevel: .dark,
    origin: "Blend — globally sourced",
    process: "Espresso-oriented",
    description: "The darkest roast in the Stitch Coffee collection. For the Stoics among us. Inspired by classic Italian roasts. Tailored for a strong start to the day.",
    emotionalTag: "For when you mean business. Dense, structured, no apologies.",
    showEmotionalTagOnCard: true,
    tastingNotes: [
        TastingNote(icon: "icon_dark_chocolate", label: "Dark Chocolate"),
        TastingNote(icon: "icon_toasted_almond", label: "Toasted Almond"),
        TastingNote(icon: "icon_caramel", label: "Caramel")
    ],
    flavorProfile: FlavorProfile(roast: 0.9, sweetness: 0.55, acidity: 0.2),
    bestForProfiles: ["deep_work", "voltage"],
    price: nil,
    shopifyURL: nil,
    shopifyProductID: nil,
    imageAsset: "stitch_dark_field_bag",
    imageBgTint: "#2E2022",
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
    roastLevel: .medium,     // Brief says "Medium-Light" — slider at 0.45 reads as medium-light visually
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
    price: nil,
    shopifyURL: nil,
    shopifyProductID: nil,
    imageAsset: "corvus_sadayana_bag",
    imageBgTint: "#2A2418",
    cardVariant: .light
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
    imageAsset: "regent_guji_hambela_bag",
    imageBgTint: "#F5F0E8",
    cardVariant: .light
)
```

### Bean Data Model

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

    enum RoastLevel: String, Codable { case light, medium, dark }
    enum CardVariant: String, Codable { case light, dark }
}

struct TastingNote: Codable {
    let icon: String
    let label: String
}

struct FlavorProfile: Codable {
    let roast: Double      // 0.0 (lightest) → 1.0 (darkest)
    let sweetness: Double  // 0.0 (none) → 1.0 (very sweet)
    let acidity: Double    // 0.0 (flat) → 1.0 (bright/sharp)
}
```

### Recommendation Matching

```swift
func recommendedBeans(for profileKey: String, allBeans: [Bean]) -> [Bean] {
    return allBeans
        .filter { $0.bestForProfiles.contains(profileKey) }
        .sorted { bean1, bean2 in
            if (bean1.price != nil) != (bean2.price != nil) {
                return bean1.price != nil
            }
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
- Selection context: `"Coming soon"` — Mono, 12px, #B5A99A
- Marketplace context:
  - `"PRICE COMING SOON"` — Mono, 14px, #B5A99A
  - "ADD TO CART" becomes `"NOTIFY ME"` — bg: #E8DFD2, text: #8C8279
  - Tap "NOTIFY ME" → stores preference in UserDefaults, shows toast with bean name

```swift
let bean_priceTBD_selection    = "Coming soon"
let bean_priceTBD_marketplace  = "PRICE COMING SOON"
let bean_notifyMe              = "NOTIFY ME"

func notifyMeToast(beanName: String) -> String {
    "We'll let you know when \(beanName) drops."
}
```

### "Add to Cart" Behavior

```swift
func addToCart(bean: Bean) {
    guard let shopifyURL = bean.shopifyURL,
          let productID = bean.shopifyProductID else { return }
    let url = URL(string: "\(shopifyURL)/cart/add?id=\(productID)")!
    present(SFSafariViewController(url: url))
}
```

---

## 6. Screen 3: Bean Selection — `BeanSelectionView.swift`

### Layout

```
┌─────────────────────────────────────┐
│  [←]              bre:zi        🛍  │  NavBarView — bg: #000000
│─────────────────────────────────────│  back arrow + bag icon
│                                     │  bg: #FAF6F0 (warm cream)
│                                     │
│  FIRST LIGHT                        │  CondensedBold, 14px, #8C8279
│  RECOMMENDED BEANS                  │  Mono, 11px, #B5A99A, ALL CAPS
│                                     │  top padding: 20pt
│  Clean start. Nothing to prove.     │  Regular, 15px, #8C8279
│                                     │  line-height: 1.5
│                                     │  bottom padding: 16pt
│                                     │
│  ┌─────────────────────────────────┐│
│  │  RECOMMENDED BEAN CARD 1       ││  BeanCardView(context: .selection)
│  │  (full card with bridge line)  ││  THIS card gets the bridge line
│  │                                 ││
│  └─────────────────────────────────┘│
│                                     │  gap: 16pt
│  ┌─────────────────────────────────┐│
│  │  RECOMMENDED BEAN CARD 2       ││  (if another bean matches profile)
│  │  (no bridge line)              ││  Second+ cards: no bridge line
│  └─────────────────────────────────┘│
│                                     │  gap: 16pt
│  ┌─────────────────────────────────┐│
│  │  GENERAL PROFILE CARD (equal)  ││  GeneralProfileCard.swift
│  │                                 ││  bg: #FFFFFF, radius: 20pt
│  │  ┌───────────────────────────┐  ││  280pt illustration area
│  │  │  [GENERAL BREW ILLUS.]    │  ││  bg: #F5F0E8
│  │  └───────────────────────────┘  ││
│  │  GENERAL BREW                   ││  t_bean_name, 22px bold, #1A1714
│  │  Optimized for First Light.     ││  Regular, 14px, #8C8279
│  │  Works with whatever's in       ││
│  │  your bag.                      ││
│  │  ┌───────────────────────┐     ││
│  │  │   BREW GENERAL  →    │     ││  bg: #5B8C5A, text: #FFFFFF
│  │  └───────────────────────┘     ││  same green button
│  └─────────────────────────────────┘│
│                                     │
│  ── browse ─────────────────────── │  divider: 1px #E5DDD3
│                                     │  "browse": Mono, 12px, #B5A99A
│                                     │
│  BROWSE ALL BEANS                   │  Mono, 11px, #B5A99A, ALL CAPS
│                                     │
│  ┌─────────────────────────────────┐│
│  │  [Stitch]  [Corvus]  [Regent]  ││  BeanCardCompactView (horiz scroll)
│  └─────────────────────────────────┘│
│                                     │
│  ┌─────────────────────┐            │
│  │  VISIT BEAN SHOP  → │            │  outlined, border: 1px #8C8279
│  └─────────────────────┘            │  text: #8C8279, Mono, 13px
│                                     │  bottom safe area: 32pt
└─────────────────────────────────────┘
```

### Header Copy Per Profile (exact strings)

```swift
let beanSelection_label = "RECOMMENDED BEANS"

let beanSelection_headerCopy: [String: String] = [
    "tide_pool":   "Something gentle. Let these do the talking.",
    "first_light": "Clean start. Nothing to prove.",
    "deep_work":   "You need fuel. These are built for it.",
    "voltage":     "All systems go. Pick your weapon.",
    "the_blank":   "Your call today. Here's what we've got."
]
```

### General Profile Card Spec

```swift
struct GeneralProfileCardSpec {
    let cardRadius: CGFloat = 20
    let cardBg = Color(hex: "#FFFFFF")
    let cardShadow = Shadow(color: .black.opacity(0.06), radius: 12, x: 0, y: 4)
    let imageHeight: CGFloat = 280
    let imageBg = Color(hex: "#F5F0E8")
    let imageAsset = "general_brew_illustration"
    let title = "GENERAL BREW"
    let titleFont = Font.system(size: 22, weight: .bold)
    let titleColor = Color(hex: "#1A1714")
    let titleTopPadding: CGFloat = 16
    func body(_ profileName: String) -> String {
        "Optimized for \(profileName). Works with whatever's in your bag."
    }
    let bodyFont = Font.system(size: 14, weight: .regular)
    let bodyColor = Color(hex: "#8C8279")
    let bodyTopPadding: CGFloat = 4
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
│  160 × 160pt      │  bg: #FFFFFF (or #2A2520)
│  imageBgTint      │  radius: 16pt
│                   │
│  STITCH COFFEE    │  t_roaster_name, 10px, ALL CAPS
│  Dark Field       │  Bold, 16px
│                   │
│  *🍫* 🌰 🍮       │  tasting note icons (28pt, mini)
│                   │  lead icon has accent bg
│  ROAST ░░░░░░░▓░  │  mini sliders (no labels)
│  SWEET ░░░░▓░░░░  │
│  ACID  ░▓░░░░░░░  │
│                   │
│  Coming soon      │  price or "Coming soon"
└───────────────────┘
```

- Width: 180pt. Height: intrinsic.
- Image: 160 × 160pt, `contentMode: .fit`, per-bean `imageBgTint`.
- Horizontal scroll: `.scrollTargetBehavior(.viewAligned)`, padding 16pt, gap 12pt.
- Tap anywhere → pushes `BeanDetailView`.

### Navigation Behavior

- **"BREW WITH THIS":** Stores `selectedBean`, pushes to Screen 4 (Brew Settings).
- **"BREW GENERAL":** Sets `selectedBean = nil`, pushes to Screen 4.
- **Tap on bean card (outside CTA):** Pushes to `BeanDetailView`.
- **"VISIT BEAN SHOP":** Pushes to `MarketplaceView`.
- **Bag icon (🛍):** Pushes to `MarketplaceView`.

### All Copy — Screen 3

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

---

## 7. Screen 4: Brew Settings — `BrewSettingsView.swift`

### Variant 1: Bean-Specific Settings

Adaptation note at **hinge position** — between 3-box summary and detail table, inside the parameter card. Warm background chip.

```
┌─────────────────────────────────────┐
│  [←]              bre:zi        🛍  │  NavBarView (bag icon)
│─────────────────────────────────────│
│                                     │  bg: #FAF6F0
│  YOUR BREW                          │  Mono, 11px, #B5A99A, ALL CAPS
│                                     │
│  ┌─────────────────────────────────┐│  OPTIMIZATION BADGE
│  │  ⚡ Optimized for:              ││  Mono, 12px, #5B8C5A
│  │  Regent Ethiopia Guji Hambela   ││  Bold, 16px, #1A1714
│  │  × First Light profile          ││  Mono, 12px, #8C8279
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 12pt
│                                     │  left border: 3px #5B8C5A
│  ┌─────────────────────────────────┐│  SERVING INFOGRAPHIC (Section 11)
│  │     [ServingInfoCard]           ││
│  └─────────────────────────────────┘│
│                                     │
│  ┌─ BREW PARAMETERS ──────────────┐ │
│  │  ┌──────┐  ┌──────┐  ┌──────┐ │ │  3-BOX SUMMARY
│  │  │ 4–8° │  │  12  │  │  60  │ │ │  CondensedBold, 28px, #1A1714
│  │  │  °C  │  │ min  │  │ g/L  │ │ │  Mono, 11px, #8C8279
│  │  │ TEMP │  │ TIME │  │ DOSE │ │ │  3 equal columns
│  │  └──────┘  └──────┘  └──────┘ │ │
│  │                                 │ │
│  │  ┌──────────────────────────┐  │ │  ADAPTATION NOTE — HINGE POSITION
│  │  │  Adapted for this bean's │  │ │  bg: #F5F0E8, radius: 8pt
│  │  │  light roast — lower     │  │ │  padding: 12pt inner
│  │  │  temp, longer steep to   │  │ │  Mono italic, 13px, #8C8279
│  │  │  pull floral notes.      │  │ │  margin: 12pt top/bottom
│  │  └──────────────────────────┘  │ │
│  │                                 │ │
│  │  ┌──────────────────────────┐  │ │  PARAMETER DETAIL TABLE
│  │  │  ROAST    Light          │  │ │  Mono, 13px
│  │  │  GRIND    Medium         │  │ │  label: #B5A99A, value: #1A1714
│  │  │  WATER    Filtered       │  │ │  bg: #F5F0E8, radius: 10pt
│  │  └──────────────────────────┘  │ │
│  └─────────────────────────────────┘ │  bg: #FFFFFF, radius: 16pt
│                                     │
│  ┌─────────────────────────────────┐│  FLAVOR PREVIEW
│  │  WHAT YOU'LL TASTE              ││  Mono, 11px, #B5A99A, ALL CAPS
│  │  ┌──────┐ ┌──────┐ ┌──────┐   ││  tasting note icons (48pt)
│  │  │* 🍫 *│ │  🌸  │ │  🍓  │   ││  lead icon has accent bg
│  │  │Cocoa │ │Jasmi-│ │Straw-│   ││
│  │  └──────┘ └──────┘ └──────┘   ││
│  │  ┌──────┐                      ││
│  │  │  🍋  │                      ││
│  │  │Lime  │                      ││
│  │  └──────┘                      ││
│  │  Cocoa, jasmine, strawberry,    ││  Regular, 15px, #1A1714
│  │  lime. Bright, floral, stone    ││  line-height: 1.6
│  │  fruit. Exceptional cold        ││
│  │  clarity.                       ││
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │
│  ┌─────────────────────────────────┐│  START BREW CTA
│  │  ┌─────────────────────────┐   ││
│  │  │    START BREW  →        │   ││  bg: #1A1714, text: #FFFFFF
│  │  └─────────────────────────┘   ││  CondensedBold, 16px, ALL CAPS
│  │  Change bean ›                  ││  height: 52pt, radius: 12pt
│  └─────────────────────────────────┘│
│                                     │  bottom safe area: 32pt
└─────────────────────────────────────┘
```

### Adaptation Note Spec

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
- Optimization badge: `"Using general\nFirst Light settings"` — no lightning icon
- Left border: 3px #B5A99A (muted)
- Body: `"No specific bean selected. These parameters work with most medium roasts."`
- Adaptation note: `"Standard extraction for this profile. Works with most beans in the medium roast range."`
- No flavor preview section
- "Change bean" becomes `"Select a specific bean ›"` — pops back to Screen 3

### Adaptation Notes — Per Profile × Roast Level

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

```swift
struct BeanAdjustment {
    let tempDelta: Int
    let timeDelta: Int
    let doseDelta: Int
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
          let adjustment = beanAdjustments[bean.id]?[profile.key] else { return params }
    params.tempLow += adjustment.tempDelta
    params.tempHigh += adjustment.tempDelta
    params.steepMinutes += adjustment.timeDelta
    params.doseGramsPerLiter += adjustment.doseDelta
    params.roastSuggestion = adjustment.roastSuggestion
    params.grindSuggestion = adjustment.grindSuggestion
    return params
}
```

### All Copy — Screen 4

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
func brewSettings_generalTitle(_ profileName: String) -> String {
    "Using general\n\(profileName) settings"
}
let brewSettings_generalBody     = "No specific bean selected. These parameters work with most medium roasts."
let brewSettings_generalNote     = "Standard extraction for this profile. Works with most beans in the medium roast range."
```

---

## 8. Bean Detail View — `BeanDetailView.swift`

### Layout

```
┌─────────────────────────────────────┐
│  [←]              bre:zi        🛍  │  NavBarView (bg: #000000)
│─────────────────────────────────────│
│                                     │  bg: #FAF6F0
│  ┌─────────────────────────────────┐│  HERO IMAGE
│  │        [BEAN BAG IMAGE]         ││  full-width, 300pt tall
│  │         imageBgTint per bean    ││  contentMode: .fit
│  └─────────────────────────────────┘│  radius: 0 (full bleed)
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
│  │  Process: 72-hr anaerobic       ││
│  │          fermentation           ││
│  │  Drying: Raised beds & patios   ││
│  └─────────────────────────────────┘│  bg: #F5F0E8, radius: 12pt
│                                     │
│  ┌─ WHAT YOU'LL TASTE ─────────────┐│
│  │  ┌──────┐ ┌──────┐ ┌──────┐   ││  tasting note icons (48pt)
│  │  │* 🫐 *│ │  🍒  │ │  🍫  │   ││  lead icon has accent bg
│  │  │Rasp- │ │Cherry│ │Dark  │   ││
│  │  │berry │ │Syrup │ │Choc  │   ││
│  │  │Yogurt│ │      │ │      │   ││
│  │  └──────┘ └──────┘ └──────┘   ││
│  │                                 ││
│  │  Raspberry yogurt, cherry       ││  Regular, 15px, #1A1714
│  │  syrup, dark chocolate.         ││  line-height: 1.6
│  │  Wine-like when cupped.         ││
│  │                                 ││
│  │  ROAST   ░░░░░░░░▓░░░░░░░░░░   ││  flavor sliders (large)
│  │          Medium                 ││  with value labels + endpoint labels
│  │  SWEET   ░░░░░░░░░░░░░░░▓░░░   ││
│  │          High                   ││
│  │  ACID    ░░░░░░░░░▓░░░░░░░░░   ││
│  │          Medium                 ││
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │
│  ┌─ BREW PAIRING ──────────────────┐│
│  │  Best for:                      ││  Mono, 11px, #B5A99A
│  │  ┌──────────┐ ┌──────────┐     ││  profile chips
│  │  │DEEP WORK │ │ VOLTAGE  │     ││  bg: #E8DFD2, text: #8C8279
│  │  └──────────┘ └──────────┘     ││
│  │  "Wine-dark. Fruit-forward.     ││  Regular italic, 15px, #1A1714
│  │   The kind of brew you linger   ││
│  │   over."                        ││
│  │                                 ││
│  │  ┌─ YOUR HISTORY ────────────┐  ││  (ONLY if brew count ≥ 3)
│  │  │  You've brewed this 5×.   │  ││  See Section 12
│  │  │  Best day: recovery 78%.  │  ││
│  │  └───────────────────────────┘  ││
│  └─────────────────────────────────┘│
│                                     │
│  ┌─ SERVING GUIDE ─────────────────┐│
│  │     [ServingInfoCard]           ││  See Section 11
│  └─────────────────────────────────┘│
│                                     │
│  ┌─────────────────────────────────┐│  BREW + PURCHASE
│  │  ┌─────────────────────────┐   ││  primary: BREW WITH THIS
│  │  │  BREW WITH THIS  →      │   ││  bg: #5B8C5A, text: #FFFFFF
│  │  └─────────────────────────┘   ││  height: 52pt, radius: 12pt
│  │                                 ││
│  │  $27.00                         ││  t_price, #1A1714
│  │  or subscribe & save 15%        ││  Mono, 12px, #5B8C5A
│  │  — $22.95/delivery              ││
│  │                                 ││
│  │  ┌─────────────────────────┐   ││  ADD TO CART
│  │  │   ADD TO CART  →        │   ││  bg: #1A1714, text: #FFFFFF
│  │  └─────────────────────────┘   ││  height: 48pt, radius: 12pt
│  │                                 ││
│  │  ┌─────────────────────────┐   ││  SUBSCRIBE & SAVE
│  │  │   SUBSCRIBE & SAVE     │   ││  bg: transparent, border: 1px #1A1714
│  │  └─────────────────────────┘   ││  text: #1A1714, height: 44pt
│  └─────────────────────────────────┘│
│                                     │  bottom safe area: 32pt
└─────────────────────────────────────┘
```

### CTA Priority Flip — After 3+ Brews

**Default (< 3 brews):**
1. "BREW WITH THIS" — primary (green)
2. "ADD TO CART" — secondary (dark)
3. "SUBSCRIBE & SAVE" — tertiary (outlined)

**After 3+ brews:**
1. "BREW WITH THIS" — stays primary (green)
2. "SUBSCRIBE & SAVE 15%" — promoted (dark fill)
3. "ADD TO CART" — demoted (outlined)

### Subscribed State

```swift
struct SubscribedButtonSpec {
    let text = "SUBSCRIBED"
    let icon = "checkmark"  // SF Symbol, 12pt, leading
    let bg = Color.clear
    let borderColor = Color(hex: "#5B8C5A")
    let borderWidth: CGFloat = 1
    let textColor = Color(hex: "#5B8C5A")
    let height: CGFloat = 44
    let radius: CGFloat = 12
    let isEnabled = false
}

let detail_subscribedManage = "Subscribed — manage in Settings"
// Mono, 12px, #5B8C5A

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

### Concrete Savings Display

```swift
func subscribeSavingsText(price: Double) -> String {
    let subscriptionPrice = price * 0.85
    return "or subscribe & save 15% — $\(String(format: "%.2f", subscriptionPrice))/delivery"
}
```

### Subscribe & Save Behavior

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

### Section Labels

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
let detail_subscribed     = "SUBSCRIBED"
let detail_subscribedMgmt = "Subscribed — manage in Settings"
```

---

## 9. Screens 5 & 6: Brewing + Post-Brew

### Screen 5: Brew Confirmation / Timer

Unchanged from R3 spec. The active brew card now shows bean name if selected:

```
│  FIRST LIGHT                        │  CondensedBold, 32px, #FFFFFF
│  Regent Ethiopia Guji Hambela       │  Mono, 13px, #888888 [bean name]
│  7:14am · 12 min                    │  Mono, 14px, #888888
```

If no bean selected: bean name line is omitted.

### Screen 6: Post-Brew Check-In

Unchanged from R4/R5 emotional layer spec. The check-in result is stored alongside `selectedBeanID` in BrewLog.

```swift
brewLog.selectedBeanID = selectedBean?.id
brewLog.selectedBeanName = selectedBean?.beanName
brewLog.postBrewRating = checkInResult
```

---

## 10. Flavor Visualization — Sliders + Tasting Note Icons

### Flavor Slider Bars — `FlavorSliderView.swift`

Three horizontal bars: Roast, Sweetness, Acidity. Continuous spectrum with positioned dot indicator.

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
    let trackWidth: CGFloat = .infinity

    // Track gradients — LIGHT CARD
    let roastGradient: [Color] = [
        Color(hex: "#F5E6C8"),
        Color(hex: "#D4A96A"),
        Color(hex: "#8B6914"),
        Color(hex: "#3D2B1F"),
        Color(hex: "#1A1208")
    ]
    let sweetnessGradient: [Color] = [
        Color(hex: "#E5DDD3"),
        Color(hex: "#F5E6C8"),
        Color(hex: "#F0D4A0"),
        Color(hex: "#E8B860"),
        Color(hex: "#D4A96A")
    ]
    let acidityGradient: [Color] = [
        Color(hex: "#E5DDD3"),
        Color(hex: "#D4D8A0"),
        Color(hex: "#C8D070"),
        Color(hex: "#B8C850"),
        Color(hex: "#88B830")
    ]

    // Track gradients — DARK CARD (brighter low-end for contrast)
    let sweetnessGradientDark: [Color] = [
        Color(hex: "#C8BFB0"),
        Color(hex: "#F5E6C8"),
        Color(hex: "#F0D4A0"),
        Color(hex: "#E8B860"),
        Color(hex: "#D4A96A")
    ]
    let acidityGradientDark: [Color] = [
        Color(hex: "#C8CCA0"),
        Color(hex: "#D4D8A0"),
        Color(hex: "#C8D070"),
        Color(hex: "#B8C850"),
        Color(hex: "#88B830")
    ]

    // QUALITY GATE: If banding visible on LCD (iPhone SE, 11), reduce to 3 stops.
    // Jensen tests Day 1. Binary decision.

    // Indicator dot
    let dotSize: CGFloat = 12
    let dotColor = Color(hex: "#1A1714")
    let dotBorderColor = Color(hex: "#FFFFFF")
    let dotBorderWidth: CGFloat = 2
    let dotShadow = Shadow(color: .black.opacity(0.15), radius: 3, x: 0, y: 1)
    let dotColorDark = Color(hex: "#FFFFFF")
    let dotBorderColorDark = Color(hex: "#2A2520")

    // Labels
    let labelFont = Font.system(size: 11, weight: .medium)
    let categoryLabelColor = Color(hex: "#8C8279")
    let categoryLabelColorDark = Color(hex: "#B5A99A")
    let valueLabelColor = Color(hex: "#B5A99A")
    let valueLabelColorDark = Color(hex: "#8C8279")
    let labelTracking: CGFloat = 0.05

    // Endpoint labels (detail view only)
    let endpointFont = Font.system(size: 9, weight: .regular, design: .monospaced)
    let endpointColor = Color(hex: "#B5A99A")
    let roastEndpoints = ("Light", "Dark")
    let sweetnessEndpoints = ("Low", "High")
    let acidityEndpoints = ("Low", "High")

    // Layout
    let rowHeight: CGFloat = 32
    let categoryLabelWidth: CGFloat = 48
    let gapBetweenRows: CGFloat = 8
}
```

### Dot Positioning + Value Labels

```swift
func dotPosition(value: Double, trackWidth: CGFloat) -> CGFloat {
    let padding: CGFloat = 6
    return padding + (value * (trackWidth - padding * 2))
}

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

| Context | Track width | Value label | Endpoints | Row height |
|---|---|---|---|---|
| Bean card (full) | card width - 32pt | Yes | No | 32pt |
| Bean detail | card width - 40pt | Yes | Yes | 36pt |
| Bean card compact | 120pt | No | No | 20pt |
| Brew settings | card width - 40pt | Yes | No | 32pt |

### Tasting Note Icons — `TastingNoteView.swift`

Flat, minimal vector art. NOT emoji, NOT SF Symbols. Custom illustrations.

```swift
struct TastingNoteIconSpec {
    let containerSizeDetail: CGFloat = 48
    let containerSizeCard: CGFloat = 36
    let containerSizeMini: CGFloat = 28
    let containerBg = Color(hex: "#F5F0E8")
    let containerBgDark = Color(hex: "#3A3530")
    let containerRadius: CGFloat = 12

    let iconPadding: CGFloat = 8
    let iconColor = Color(hex: "#8C6B3E")
    let iconColorDark = Color(hex: "#D4A96A")

    // Lead icon — first in array gets accent bg
    let leadIconBg = Color(hex: "#D4A96A").opacity(0.2)       // light card
    let leadIconBgDark = Color(hex: "#D4A96A").opacity(0.3)   // dark card

    let labelFont = Font.system(size: 10, weight: .medium)
    let labelColor = Color(hex: "#8C8279")
    let labelColorDark = Color(hex: "#B5A99A")
    let labelTopPadding: CGFloat = 4
    let labelMaxWidth: CGFloat = 56
    let iconGap: CGFloat = 12
    let maxIconsPerRow: Int = 4
}
```

### Complete Icon Asset List — 15 Icons

```swift
enum TastingNoteIcon: String, CaseIterable {
    case darkChocolate = "icon_dark_chocolate"
    case cocoa = "icon_cocoa"
    case milkChocolate = "icon_milk_chocolate"
    case toastedAlmond = "icon_toasted_almond"
    case caramel = "icon_caramel"
    case honey = "icon_honey"
    case raspberry = "icon_raspberry"
    case strawberry = "icon_strawberry"
    case cherry = "icon_cherry"
    case berry = "icon_berry"
    case tropicalFruit = "icon_tropical_fruit"
    case lime = "icon_lime"
    case jasmine = "icon_jasmine"
    case whiskey = "icon_whiskey"
    case pepper = "icon_pepper"
}
```

**Art direction:** Flat vector, 2-3 colors max, thick rounded strokes (2-3pt at 48×48), no gradients, no shadows. Primary: `#8C6B3E`. Highlight: `#D4A96A`. Dark variant: `#D4A96A` primary, `#F5E6C8` highlight.

**Deliverables:** 15 icons × 2 color variants = 30 assets. PDF vector, 1x/2x/3x.

---

## 11. Serving Infographic Card — `ServingInfoCard.swift`

### Layout

```
┌─────────────────────────────────────┐
│  SERVING GUIDE                      │  Mono, 11px, #B5A99A, ALL CAPS
│                                     │  bg: #FFFFFF, radius: 16pt
│  ┌───────────────┬─────────────────┐│  padding: 20pt inner
│  │   SERVING     │   BEST BEFORE   ││
│  │               │                 ││
│  │   ┌─────┐    │   2:00 PM       ││  CondensedBold, 28px, #1A1714
│  │   │ ☕  │    │                 ││
│  │   └─────┘    │   After this,   ││  Mono, 12px, #B5A99A
│  │              │   caffeine       ││
│  │   1 cup      │   competes with  ││
│  │   ~250ml     │   your sleep.    ││
│  └──────────────┴──────────────────┘│  divider: 1px #E5DDD3 vertical
│                                     │
│  ☀️ 6am ━━━━━━━━━━━━━━━░░░ 2pm     │  TIMING BAR
│       ████████████████░░░░░         │  green fill = brew-safe window
│       ↑ brew window                 │  grey track = past cutoff
└─────────────────────────────────────┘
```

### Serving Size — Cup Icons

```swift
struct ServingIconSpec {
    let iconName = "cup.and.saucer.fill"  // SF Symbol
    let iconSize: CGFloat = 32
    let iconColor = Color(hex: "#D4A96A")
    let labelFont = Font.system(size: 14, weight: .semibold)
    let labelColor = Color(hex: "#1A1714")
    let sublabelFont = Font.system(size: 12, weight: .regular, design: .monospaced)
    let sublabelColor = Color(hex: "#B5A99A")
}
```

| Servings | Icons | Label | Sublabel |
|---|---|---|---|
| 1 | 1 cup | "1 cup" | "~250ml" |
| 2 | 2 cups | "2 cups" | "~500ml" |
| 3 | 3 cups | "3 cups" | "~750ml" |

### Timing Bar

```swift
struct TimingBarSpec {
    let barHeight: CGFloat = 8
    let barRadius: CGFloat = 4
    let barTrackColor = Color(hex: "#E5DDD3")
    let barFillColor = Color(hex: "#5B8C5A")
    let startIcon = "sunrise.fill"  // SF Symbol, 14pt
    let endIcon = "moon.fill"
    let iconColor = Color(hex: "#D4A96A")
    let labelFont = Font.system(size: 11, weight: .medium, design: .monospaced)
    let labelColor = Color(hex: "#B5A99A")
    let markerLabel = "brew window"
    let markerFont = Font.system(size: 10, weight: .medium)
    let markerColor = Color(hex: "#8C8279")
}
```

### Cutoff Times + Serving Counts by Profile

```swift
let profileCutoffHours: [String: Int] = [
    "tide_pool": 16,   // 4:00 PM
    "first_light": 14, // 2:00 PM
    "deep_work": 14,   // 2:00 PM
    "voltage": 12,     // 12:00 PM
    "the_blank": 14    // 2:00 PM
]

let profileServings: [String: Int] = [
    "tide_pool": 1, "first_light": 1, "deep_work": 2, "voltage": 1, "the_blank": 1
]
```

---

## 12. Your History With This Bean — `BeanBrewHistoryCard.swift`

### When It Appears

- **Threshold:** `brewCount >= 3` for this bean
- **Location:** Inside BREW PAIRING section of `BeanDetailView`, after profile chips
- **Also:** On bean card in `.marketplace` context (compact version)

### Layout — Full Version (BeanDetailView)

```
│  ┌─ YOUR HISTORY ────────────────┐  │
│  │  You've brewed this 5×.       │  │  Regular, 15px, #1A1714
│  │  Best day: recovery 78%.      │  │
│  │                               │  │
│  │  ┌────────────────────────┐   │  │  MINI CHART
│  │  │  ·  ·     ·  ·  ·     │   │  │  5 dots = last 5 brews
│  │  │  ─────────────────     │   │  │  y-axis: recovery score
│  │  │  3/1  3/5  3/10 3/15  │   │  │  Mono, 9px, #B5A99A
│  │  └────────────────────────┘   │  │  height: 48pt
│  │                               │  │  stroke: #D4A96A, 2pt
│  │  Your best sessions with      │  │  Mono, 12px, #8C8279
│  │  this bean were on recovery   │  │
│  │  days above 70%.              │  │
│  └───────────────────────────────┘  │  radius: 12pt
│                                     │  left border: 3px #D4A96A
```

### Compact Version (Marketplace)

```
│  ┌─ BREWED 5× ──────────────────┐  │
│  │  Best day: recovery 78%.     │  │  Mono, 12px, #8C8279
│  └──────────────────────────────┘  │  bg: #F5F0E8, radius: 8pt
```

### Component Spec

```swift
struct BeanBrewHistorySpec {
    let containerBg = Color(hex: "#FFFFFF")
    let containerRadius: CGFloat = 12
    let containerBorderColor = Color(hex: "#D4A96A")
    let containerBorderWidth: CGFloat = 3
    let containerPadding: CGFloat = 16
    let sectionLabel = "YOUR HISTORY"
    let sectionFont = Font.system(size: 11, weight: .semibold, design: .monospaced)
    let sectionColor = Color(hex: "#B5A99A")
    let messageFont = Font.system(size: 15, weight: .regular)
    let messageColor = Color(hex: "#1A1714")
    let chartHeight: CGFloat = 48
    let chartStrokeColor = Color(hex: "#D4A96A")
    let chartStrokeWidth: CGFloat = 2
    let chartDotSize: CGFloat = 6
    let chartDotFill = Color(hex: "#D4A96A")
    let insightFont = Font.system(size: 12, weight: .regular, design: .monospaced)
    let insightColor = Color(hex: "#8C8279")
    let compactBg = Color(hex: "#F5F0E8")
    let compactRadius: CGFloat = 8
    let compactPadding: CGFloat = 10
    let compactFont = Font.system(size: 12, weight: .regular, design: .monospaced)
    let compactColor = Color(hex: "#8C8279")
}
```

### Data Query + Copy Generation

```swift
struct BeanBrewHistory {
    let beanID: String
    let brewCount: Int
    let bestRecoveryScore: Int
    let averageRecoveryScore: Int
    let lastBrewDate: Date
    let recentBrews: [BrewDataPoint]

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
    let scores = beanBrews.compactMap { $0.recoveryScore }
    return BeanBrewHistory(
        beanID: beanID,
        brewCount: beanBrews.count,
        bestRecoveryScore: scores.max() ?? 0,
        averageRecoveryScore: scores.isEmpty ? 0 : scores.reduce(0, +) / scores.count,
        lastBrewDate: beanBrews.first!.date,
        recentBrews: Array(beanBrews.prefix(5)).map {
            .init(date: $0.date, recoveryScore: $0.recoveryScore ?? 0)
        }
    )
}

func historyMessage(_ h: BeanBrewHistory) -> String {
    "You've brewed this \(h.brewCount)×. Best day: recovery \(h.bestRecoveryScore)%."
}

func historyInsight(_ h: BeanBrewHistory) -> String {
    if h.bestRecoveryScore >= 70 {
        return "Your best sessions with this bean were on recovery days above 70%."
    } else if h.averageRecoveryScore >= 50 {
        return "This bean works well for you across a range of recovery days."
    } else {
        return "You tend to reach for this one on harder days. It holds up."
    }
}

func historyCompactLabel(_ h: BeanBrewHistory) -> String { "BREWED \(h.brewCount)×" }
func historyCompactBody(_ h: BeanBrewHistory) -> String { "Best day: recovery \(h.bestRecoveryScore)%." }
```

### BrewLog Extension

```swift
@NSManaged var selectedBeanID: String?
@NSManaged var selectedBeanName: String?
@NSManaged var recoveryScore: Int?
```

---

## 13. Marketplace Screen — `MarketplaceView.swift`

### Access Points

1. Bean Selection Screen 3 — "VISIT BEAN SHOP"
2. Nav bar bag icon (🛍) — on coffee-side screens
3. Dashboard — "BROWSE BEANS"
4. Settings — "Bean Shop" row

### Layout

```
┌─────────────────────────────────────┐
│  [←]              bre:zi        🛍  │  NavBarView (bag icon)
│─────────────────────────────────────│
│                                     │  bg: #FAF6F0
│  ┌─────────────────────────────────┐│  HEADER
│  │  BEAN SHOP                      ││  CondensedBold, 28px, #1A1714
│  │  Selected for cold brew.        ││  Regular, 15px, #8C8279
│  │  Matched to your morning.       ││
│  └─────────────────────────────────┘│
│                                     │
│  ┌─ FILTER CHIPS ──────────────────┐│
│  │  [All] [Tide Pool] [First      ││  horizontal scroll
│  │   Light] [Deep Work] [Voltage] ││  FilterChipView.swift
│  └─────────────────────────────────┘│
│                                     │
│  ┌─────────────────────────────────┐│  ALL-TBD BANNER (conditional)
│  │  These beans are on deck.       ││  Only when ALL visible beans
│  │  Tap NOTIFY ME and we'll        ││  have price == nil
│  │  ping you when they drop.       ││  Regular, 15px, #1A1714
│  └─────────────────────────────────┘│  bg: #F5F0E8, radius: 12pt
│                                     │
│  ┌─ RECOMMENDED FOR YOU ───────────┐│  (when filter = "All")
│  │  Beans that match today's       ││
│  │  First Light profile.           ││
│  │  ┌───────────────────────────┐  ││
│  │  │   [BEAN CARD]             │  ││  BeanCardView(context: .marketplace)
│  │  │   with "TODAY'S PICK" tag  │  ││  + "or brew with this bean ›"
│  │  │   + history compact card   │  ││
│  │  └───────────────────────────┘  ││
│  └─────────────────────────────────┘│
│                                     │
│  ┌─ ALL BEANS ─────────────────────┐│
│  │  [Stitch] [Corvus] [Regent]    ││  all bean cards, 16pt gap
│  └─────────────────────────────────┘│
│                                     │
│  ┌─────────────────────────────────┐│  SUBSCRIBE BANNER
│  │  Never run out.                 ││  CondensedBold, 22px, #1A1714
│  │  Subscribe and save 15% on      ││  Regular, 14px, #8C8279
│  │  every delivery. Available on   ││
│  │  select beans. Cancel anytime.  ││
│  │  ┌─────────────────────────┐   ││
│  │  │  SUBSCRIBE ON ANY BEAN →│   ││  outlined, border: 1px #D4A96A
│  │  └─────────────────────────┘   ││  text: #D4A96A, Mono, 13px
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │  top accent border: 2px #D4A96A
│  ┌─────────────────────────────────┐│  FOOTER
│  │  More roasters coming soon.     ││  Mono, 13px, #B5A99A, centered
│  │  Want to partner?               ││
│  │  hello@brezicoffee.com          ││  Mono, 13px, #D4A96A (tappable)
│  └─────────────────────────────────┘│
│                                     │  bottom safe area: 32pt
└─────────────────────────────────────┘
```

### Filter Chips — `FilterChipView.swift`

```swift
struct FilterChipSpec {
    let chipHeight: CGFloat = 36
    let chipRadius: CGFloat = 18
    let chipPaddingH: CGFloat = 16
    let chipGap: CGFloat = 8
    let defaultBg = Color(hex: "#F5F0E8")
    let defaultText = Color(hex: "#8C8279")
    let defaultFont = Font.system(size: 13, weight: .medium)
    let selectedBg = Color(hex: "#1A1714")
    let selectedText = Color(hex: "#FFFFFF")
    let chipLabels = ["All", "Tide Pool", "First Light", "Deep Work", "Voltage"]
}
```

### Filter Behavior

- "All": all beans, "Recommended For You" section at top.
- Profile-specific: only beans matching that profile.
- Empty state:

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
```

### All-TBD Banner

```swift
func showAllPriceTBDBanner(visibleBeans: [Bean]) -> Bool {
    return visibleBeans.allSatisfy { $0.price == nil }
}

let marketplace_allTBD_banner = "These beans are on deck. Tap NOTIFY ME and we'll ping you when they drop."
```

### Nav Bar Icon

```swift
let navBar_marketplaceIcon = "bag"  // SF Symbol
// Tap → push MarketplaceView
// No badge count — Brezi is not a cart app
```

### "BREW WITH THIS" in Marketplace

```swift
func brewWithBean(_ bean: Bean, profile: BrewProfile) {
    selectedBean = bean
    navigationPath.removeLast(navigationPath.count)
    navigationPath.append(.brewSettings(bean: bean, profile: profile))
}
```

### Settings Additions

```
│  ☕ Bean Shop                      › │  tap → push MarketplaceView
│  🔄 Manage Subscriptions          › │  tap → SFSafariViewController
```

### All Copy — Marketplace

```swift
let marketplace_title           = "BEAN SHOP"
let marketplace_subtitle        = "Selected for cold brew. Matched to your morning."
let marketplace_recommended     = "RECOMMENDED FOR YOU"
func marketplace_recommendedSub(_ profileName: String) -> String {
    "Beans that match today's \(profileName) profile."
}
let marketplace_allBeans        = "ALL BEANS"
let marketplace_subscribe_title = "Never run out."
let marketplace_subscribe_body  = "Subscribe and save 15% on every delivery. Available on select beans. Cancel anytime."
let marketplace_subscribe_cta   = "SUBSCRIBE ON ANY BEAN"
let marketplace_footer_line1    = "More roasters coming soon."
let marketplace_footer_line2    = "Want to partner?"
let marketplace_footer_email    = "hello@brezicoffee.com"
let marketplace_brewLink        = "or brew with this bean"
let marketplace_allTBD_banner   = "These beans are on deck. Tap NOTIFY ME and we'll ping you when they drop."
```

---

## 14. Animations

| Location | Element | Type | Duration | Easing | Delay | Trigger |
|---|---|---|---|---|---|---|
| **Why Today** | Background color | crossfade #0F0F0F → #FAF6F0 | WhyTodayConfig.transitionDuration | easeInOut | WhyTodayConfig.transitionDelay | Screen appear (first/day) |
| **Why Today** | Text color | crossfade #FFFFFF → #1A1714 | synced with bg | easeInOut | synced | Synced |
| **Why Today** | Message text | typewriter (char by char) | WhyTodayConfig.typewriterSpeed/char | linear | 0.0s | Screen appear (first/day) |
| **Why Today** | Direction text | typewriter | WhyTodayConfig.typewriterSpeed/char | linear | lineDelay after msg | After message |
| **Why Today** | CTA button | fadeIn + translateY(12→0) | 0.3s | easeOut | ctaDelay | After direction |
| Screen 3 | Recommended cards | fadeIn + translateY(12→0) | 0.3s | easeOut | 0.1s stagger | Appear |
| Screen 3 | General card | fadeIn + translateY(12→0) | 0.3s | easeOut | after last bean | Appear |
| Screen 3 | Browse section | fadeIn | 0.2s | easeOut | 0.3s | Scroll into view |
| Bean card | "TODAY'S PICK" tag | pulse scale(1.0↔1.02) | 2.0s | easeInOut | — | Loop while visible |
| Bean detail | Image | fadeIn + scale(0.98→1.0) | 0.3s | easeOut | 0.0s | Load |
| Bean detail | Origin card | fadeIn + translateY(8→0) | 0.25s | easeOut | 0.15s | Load |
| Bean detail | Flavor section | fadeIn + translateY(8→0) | 0.25s | easeOut | 0.25s | Load |
| Bean detail | History card | fadeIn + translateY(8→0) | 0.25s | easeOut | 0.35s | Load |
| Flavor sliders | Dot position | spring(response: 0.4, dampingFraction: 0.8) | — | spring | 0.0s | Appear |
| Timing bar | Fill draw | width(0→target) | 0.4s | easeOut | 0.2s | Appear |
| Marketplace | Filter chip select | spring(response: 0.3, dampingFraction: 0.7) | — | spring | 0.0s | Tap |
| Marketplace | Bean card filter | matchedGeometryEffect | 0.3s | easeInOut | 0.0s | Filter change |
| Card 3 | Post-brew phase | crossfade | 0.3s | easeInOut | 0.0s | App foreground |

---

## 15. Navigation — Complete Tree

### Nav Bar Variants

```swift
enum NavBarVariant {
    case health  // right icon: antenna (📡)
    case coffee  // right icon: bag (🛍)
}
```

Nav bar bg: `#000000` everywhere. Brand shell is always dark.

### Full Nav Tree

```
AppRoot
 └─ MainFlow
     ├─ DashboardView.swift (bg: #F5F5F5)
     │   ├─ State: preBrew → Card 3 CTA
     │   │   ├─ "CHOOSE YOUR BEAN" → WhyTodayView [Screen 2]
     │   │   │   └─ "SEE YOUR BEANS" → BeanSelectionView [Screen 3]
     │   │   │       ├─ "BREW WITH THIS" → BrewSettingsView [Screen 4]
     │   │   │       │   └─ "START BREW" → Brewing state
     │   │   │       ├─ "BREW GENERAL" → BrewSettingsView (bean=nil)
     │   │   │       ├─ Bean card tap → BeanDetailView
     │   │   │       │   ├─ "BREW WITH THIS" → BrewSettingsView
     │   │   │       │   ├─ "ADD TO CART" → SFSafariViewController
     │   │   │       │   └─ "SUBSCRIBE & SAVE" → SFSafariViewController
     │   │   │       └─ "VISIT BEAN SHOP" → MarketplaceView
     │   │   └─ "Skip" → BeanSelectionView (same)
     │   │
     │   ├─ "Use general settings" (GATED: completedBrewFlows >= 3)
     │   │   └─ BrewSettingsView (bean=nil, skip Why Today)
     │   ├─ "BROWSE BEANS" → MarketplaceView
     │   ├─ State: brewing (unchanged)
     │   └─ State: postBrew (+ bean name line + temporal arc)
     │
     ├─ MarketplaceView [from nav bag icon]
     │   ├─ Bean card tap → BeanDetailView
     │   ├─ "ADD TO CART" → SFSafariViewController
     │   ├─ "SUBSCRIBE & SAVE" → SFSafariViewController
     │   ├─ "or brew with this bean" → BrewSettingsView (with bean)
     │   └─ "SUBSCRIBE ON ANY BEAN" → scroll to first subscribable
     │
     ├─ HistoryView (unchanged)
     ├─ DeviceHubView (unchanged)
     └─ SettingsView
         ├─ "Bean Shop" → MarketplaceView
         └─ "Manage Subscriptions" → SFSafariViewController
```

---

## 16. New Components — Summary

| Component | File | Purpose |
|---|---|---|
| `WhyTodayView.swift` | Screen 2 | Palette transition, emotional bridge, maturation, return moments |
| `BeanCardView.swift` | Bean product card | 3 contexts, 2 variants, bridge line on first recommended |
| `BeanCardCompactView.swift` | Compact card | Horizontal scroll — image + name + mini sliders |
| `BeanDetailView.swift` | Bean detail | Full info, origin, flavor, history, purchase |
| `BeanSelectionView.swift` | Screen 3 | Recommended beans + general card + browse |
| `BrewSettingsView.swift` | Screen 4 | Bean-adapted or general brew parameters |
| `MarketplaceView.swift` | Bean Shop | All beans, profile filter, subscribe banner |
| `FlavorSliderView.swift` | Flavor bars | Gradient slider with dot indicator |
| `TastingNoteView.swift` | Tasting note icon | Illustrated icon + label |
| `ServingInfoCard.swift` | Serving infographic | Cup icons + cutoff time + timing bar |
| `TimingBarView.swift` | Day timeline | Horizontal bar showing brew window |
| `FilterChipView.swift` | Profile filter | Pill-shaped chip |
| `BeanBrewHistoryCard.swift` | Personal history | Brew count, recovery, mini chart |
| `GeneralProfileCard.swift` | General brew | Equal-weight card for "no specific bean" |

---

## 17. New Data Requirements

### Bean Entity — Bundled JSON

```swift
let beans: [Bean] = Bundle.main.decode("beans.json")
```

### BrewLog Extension

```swift
@NSManaged var selectedBeanID: String?
@NSManaged var selectedBeanName: String?
@NSManaged var recoveryScore: Int?
```

### UserDefaults Keys — Complete Registry

```swift
// === WHY TODAY ===
let key_lastWhyTodayDate          = "lastWhyTodayDate"          // String (yyyy-MM-dd)
let key_whyTodaySessionCount      = "whyTodaySessionCount"      // Int — displays, NOT skips
let key_consecutiveSkips          = "consecutiveWhyTodaySkips"   // Int
let key_suppressedUntil           = "whyTodaySuppressedUntil"    // String (date)
let key_reintroduced              = "whyTodayReintroduced"       // Bool

// === GENERAL SETTINGS GATE ===
let key_completedBrewFlows        = "completedBrewFlows"         // Int — full-flow completions

// === MARKETPLACE ===
let key_notifyBeans               = "notifyBeans"                // [String] — bean IDs
let key_subscribedBeans           = "subscribedBeans"            // [String] — bean IDs

// === VETERAN COPY TRACKING ===
let key_lastSessionDate_high      = "lastSessionDate_high"       // String (date)
let key_lastSessionDate_mid       = "lastSessionDate_mid"
let key_lastSessionDate_low       = "lastSessionDate_low"
let key_tierSessionCount_high     = "tierSessionCount_high"      // Int
let key_tierSessionCount_mid      = "tierSessionCount_mid"
let key_tierSessionCount_low      = "tierSessionCount_low"

// === MATURATION ROTATION ===
// Per-tier, per-maturity rotation indices
// Pattern: "lastWhyToday_{tier}_{maturity}"
// e.g., "lastWhyToday_high_familiar" = Int (rotation index)
```

---

## 18. Decisions — All Rounds Consolidated

### Round 1-3 Decisions (20 total, all carrying forward)

All decisions from Rounds 1-3 carry forward unchanged. Key decisions:
- Dual palette system: health=cool, coffee=warm
- Why Today as palette transition screen (not a CSS crossfade)
- Bean card 3-context architecture (.selection/.marketplace/.detail)
- Dark card variant for dark roasts
- "TODAY'S PICK" tag on recommended beans
- General Profile as equal-weight card
- Adaptation note at hinge position (between summary and table)
- SFSafariViewController for Shopify checkout
- WhyTodayConfig for tunable animation timing
- Gradient banding quality gate
- Localizable.strings from Phase 1

### Round 4 Decisions (1)

21. **General settings shortcut gated behind `completedBrewFlows >= 3`.** New users must experience the full palette transition at least 3 times before the bypass appears. Counter increments on full-flow completion only.

### Round 5 Decisions (4 — v1 extensions)

22. **State-bean bridge lines (Extension 1).** One sentence on the first recommended bean card connecting body state to this specific bean. Uses each roaster's emotional tag vocabulary (Elon fix: "Floral, bright" not "Gentle origin"). Only on first recommended card in `.selection` context.

23. **Why Today maturation (Extension 3).** Three tiers: new (1-7), familiar (8-30), veteran (31+). Copy gets shorter and more personal over time. Veteran tier uses dynamic references ({lastPeakDay}, {midCount}) resolved at display time only. Nil fallback to familiar tier. Session count increments on display, not on skip.

24. **Full-day emotional arc (Extension 6).** Card 3 post-brew copy changes across the day: recent (0-2h), afternoon (2-6h), evening (6h+). Rating CTA hides in evening phase. Phase transition on app foreground via crossfade. Afternoon Tide Pool: "That's all it needed to be."

25. **Return moments (Extension 7).** 3+ day absence triggers return-specific Why Today copy. `checkReturnReset()` runs before `shouldShowWhyToday()` — clears suppression so returning user always sees Why Today fresh. Copy: "You're back. [state-specific]. The routine remembers you."

### Deferred to v1.1 (3)

- Extension 2: Four-node "Why This Brew" chain — new detail screen UI
- Extension 4: State-aware flavor framing lines — detail screen enhancement
- Extension 5: Pattern vocabulary / one-time summary — needs 15+ brews

---

## 19. Copy Audit — All Strings (Round 5 Final)

### Why Today — Base Messages (5 profiles × 3 parts = 15 strings)

```
"You're at {recovery}% today."
"Your body is asking for less. Honor it. Low dose, low temp — let your system coast."
"Something gentle. Let these do the talking."

"You're at {recovery}% today."
"Rebuilding. Your nervous system is doing the heavy lifting."
"Clean start. Nothing to prove."

"You're at {recovery}% today."
"Strong recovery. Your body has the runway. Full extraction — use what you've built."
"You need fuel. These are built for it."

"You're at {recovery}% today."
"Peak state. Everything is green. Maximum dose, maximum extraction — send it."
"All systems go. Pick your weapon."

"You're at {recovery}% today."
"Manual mode. No recommendation — you know what you want today."
"Your call today. Here's what we've got."
```

### Why Today — Familiar Tier (15 strings)

```
"You're at {recovery}%."
"Low morning. Honor it."
"Gentle cup. Let it coast."

"{recovery}%."
"Full signal. You know what to do with this."
"Clean start."

"{recovery}%. Strong."
"Full extraction. Use what you've built."
"Fuel up."

"{recovery}%. Peak."
"Everything's green. Send it."
"Pick your weapon."

"{recovery}%."
"Manual mode. Your call."
"Here's what we've got."
```

### Why Today — Veteran Tier (15 strings, with dynamic fallbacks)

```
"{recovery}%."
"Low signal. {daysSinceLow} days since the last one. It passes." / fallback: "Low morning. Honor it."
"Gentle cup."

"{recovery}%."
"Best signal since {lastPeakDay}. Act on it." / fallback: "Full signal. You know what to do with this."
"Clean start."

"{recovery}%."
"Steady. Your most common state — {midCount} sessions and counting." / fallback: "Strong recovery. Use it."
"Full extraction."

"{recovery}%. Peak."
"Peak state. Last time: {lastPeakDay}. Make it count." / fallback: "Everything's green. Send it."
"All systems."

"{recovery}%."
"Manual. Your call."
"Here's what we've got."
```

### Why Today — Return Moments (5 messages)

```
"{days} days since the last one. {recovery}% recovery."
"You're back. Low signal. Ease in. The routine remembers you."
"Gentle cup. Welcome back."

"{days} days since the last one. {recovery}% recovery."
"You're back. Mid-range signal. Normal morning. Start there."
"Same place. Same morning. Let's go."

"{days} days since the last one. {recovery}% recovery."
"You're back. And you came back strong. Pick up where you left off."
"Full extraction. Welcome back."

"{days} days since the last one. {recovery}% recovery."
"You're back. And you came back strong. Pick up where you left off."
"All systems. Let's go."

"{days} days since the last one. {recovery}% recovery."
"You're back. Manual mode. Your call."
"Here's what we've got."
```

### Why Today CTAs (2)

```
"SEE YOUR BEANS"
"Skip"
```

### Dashboard Card 3 (7 + 10 temporal arc strings)

```
"BREZI SUGGESTS"
"CHOOSE YOUR BEAN"
"Use general settings"
"BROWSE BEANS"
"Low and slow today."
"Steady morning. Standard brew."
"You've got the runway. Go deep."
"All systems. Full send."
"Your call. Your brew."

// Afternoon (5)
"Brewed this morning. Gentle start. That's all it needed to be."
"This morning's brew. Balanced, finished clean."
"Dense cup earlier. Still carrying the momentum."
"Full send this morning. The signal's still warm."
"Manual brew earlier. Your call, as always."

// Evening (5)
"Today's cup is behind you. Rest well."
"Today's cup is behind you. Sleep well — tomorrow's signal starts now."
"Session's done. Let the system rebuild overnight."
"Everything was up today. Let it come down. Tomorrow resets."
"Today's brew is logged. Tomorrow is a clean read."
```

### Bean Selection — Screen 3 (12 strings)

```
"RECOMMENDED BEANS"
"BREW WITH THIS"
"GENERAL BREW"
"BREW GENERAL"
"BROWSE ALL BEANS"
"VISIT BEAN SHOP"
"browse"
"TODAY'S PICK"
"Something gentle. Let these do the talking."
"Clean start. Nothing to prove."
"You need fuel. These are built for it."
"All systems go. Pick your weapon."
"Your call today. Here's what we've got."
"Optimized for {profileName}. Works with whatever's in your bag."
```

### State-Bean Bridge Lines (12 strings)

```
"Floral, bright — built for a morning like this. Effortless match."
"Floral, bright origin for a steady morning. Clean cup, clean start."
"Floral, bright origin for a low morning. Effortless match."
"Floral, bright, effortless. Your call on the rest."
"Dense, structured — peak day deserves peak intensity. No apologies."
"Dense, structured origin for a steady morning. No apologies needed."
"Dense, structured origin on a low day. The dose is the bet. Bean holds up."
"Dense, structured, no apologies. Your parameters."
"Wine-dark, fruit-forward — peak state meets peak complexity. Linger."
"Wine-dark origin for a deep work day. Linger over this one."
"Wine-dark origin on a low morning. Not what the data suggests. Bean's ready if you are."
"Wine-dark. Fruit-forward. Your call on the extraction."
```

### Brew Settings — Screen 4 (11 strings)

```
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

```
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

```
"ORIGIN"
"WHAT YOU'LL TASTE"
"BREW PAIRING"
"SERVING GUIDE"
"Best for:"
"BREW WITH THIS"
"YOUR HISTORY"
"SUBSCRIBED"
"Subscribed — manage in Settings"
```

### Purchase CTAs (10 strings)

```
"ADD TO CART"
"SUBSCRIBE & SAVE"
"SUBSCRIBE & SAVE 15%"
"or subscribe & save 15%"
"or subscribe & save 15% — ${price}/delivery"
"PRICE COMING SOON"
"NOTIFY ME"
"We'll let you know when {beanName} drops."
"Coming soon"
"or brew with this bean"
```

### Marketplace (13 strings)

```
"BEAN SHOP"
"Selected for cold brew. Matched to your morning."
"RECOMMENDED FOR YOU"
"Beans that match today's {profileName} profile."
"ALL BEANS"
"Never run out."
"Subscribe and save 15% on every delivery. Available on select beans. Cancel anytime."
"SUBSCRIBE ON ANY BEAN"
"More roasters coming soon."
"Want to partner?"
"hello@brezicoffee.com"
"Nothing matched {profile} yet. Try {fallback} — similar energy, more options."
"These beans are on deck. Tap NOTIFY ME and we'll ping you when they drop."
```

### Serving Guide (5 strings)

```
"SERVING GUIDE"
"SERVING"
"BEST BEFORE"
"After this, caffeine competes with your sleep."
"brew window"
```

### Bean History (6 template strings)

```
"You've brewed this {n}×. Best day: recovery {score}%."
"Your best sessions with this bean were on recovery days above 70%."
"This bean works well for you across a range of recovery days."
"You tend to reach for this one on harder days. It holds up."
"BREWED {n}×"
"Best day: recovery {score}%."
```

### Bean Emotional Tags (3 strings)

```
"For when you mean business. Dense, structured, no apologies."
"Wine-dark. Fruit-forward. The kind of brew you linger over."
"The cleanest thing you'll drink today. Floral, bright, effortless."
```

### Settings (2 strings)

```
"Bean Shop"
"Manage Subscriptions"
```

**Total new strings: ~150** (including maturation tiers, temporal arc, return moments, and bridge lines)

---

## 20. Build Order — For Jensen

### Phase 1 (Days 1-2): Foundation

- Dual palette tokens (coffee-side colors, per-roaster tints)
- `Localizable.strings` with all ~150 coffee-side strings
- `BeanCardView.swift` + `FlavorSliderView.swift` + `TastingNoteView.swift`
- Get a single bean card rendering with real data in all three context modes
- Core Data: add `selectedBeanID`, `selectedBeanName`, `recoveryScore` to BrewLog
- Bean data: bundle `beans.json` with 3 roaster beans
- **Gradient banding quality gate:** Test 5-stop LinearGradient on iPhone 11 LCD. Keep 5 or reduce to 3. Binary decision Day 1.

### Phase 2 (Days 3-4): Emotional Bridge + Bean Selection

- `WhyTodayView.swift` (Screen 2) — typewriter + palette crossfade
- `WhyTodayConfig` struct for tunable timing
- Why Today maturation: 3 tiers, session count tracking, familiar + veteran copy pools
- Veteran dynamic references with nil-fallback (unit test every nil combination)
- Return moments: `checkReturnReset()` before `shouldShowWhyToday()`, return copy
- `BeanSelectionView.swift` (Screen 3) + `GeneralProfileCard.swift`
- State-bean bridge lines on first recommended card
- Dashboard Card 3 CTA changes + general settings shortcut gate
- Updated First Light copy

### Phase 3 (Day 5): Brew Settings

- `BrewSettingsView.swift` (Screen 4) + `ServingInfoCard.swift` + `TimingBarView.swift`
- Bean-specific parameter adjustments (`BeanAdjustment` deltas)
- Adaptation note at hinge position with warm background chip
- General profile variant

### Phase 4 (Day 6): Bean Detail + Compact Cards

- `BeanDetailView.swift` — origin, flavor, brew pairing, serving guide
- `BeanCardCompactView.swift` — horizontal scroll variant
- Subscribed button state (UserDefaults, disabled "SUBSCRIBED ✓")
- CTA priority flip after 3+ brews

### Phase 5 (Day 7): Marketplace

- `MarketplaceView.swift` + `FilterChipView.swift`
- Profile filtering, subscribe banner, guided empty states
- Nav bag icon on coffee-side screens
- All-TBD banner, bean-name-specific toast
- Subscribe banner qualifier copy
- Settings rows: Bean Shop + Manage Subscriptions
- Wire all 4 access points

### Phase 6 (Day 8): History + Temporal Arc

- `BeanBrewHistoryCard.swift` + BrewLog query
- Mini chart component
- History card placement in detail and marketplace
- Full-day emotional arc: 3 post-brew phases, 10 copy strings
- Phase transition on app foreground

### Phase 7 (Day 9): Polish + TestFlight

- All animations (Why Today typewriter, card staggers, slider springs, timing bar fill)
- Dark/light card variant edge cases
- Navigation flow end-to-end testing
- Post-brew bean name line
- Subscribe & Save Shopify deep-links
- Lead tasting note accent bg on all contexts
- Clean build, no warnings, no template variables in any copy path

**Total: ~9 days.** TestFlight target: first week of April.

### Asset Dependencies (blocking)

- **15 tasting note illustrations** — need from designer before Phase 1 is visually complete. SF Symbol placeholders until then.
- **3 bean bag product photos** — need from roaster partners. Placeholder images OK.
- **1 general brew illustration** — coffee bag silhouette placeholder OK.

### What Can Slip Without Damage

Cut in this order (last = cut first):
1. Full-day emotional arc — Card 3 can stay static post-brew
2. Return moments — app works without noticing absence
3. Bridge lines — state-to-bean connection is implicit
4. Why Today maturation — base copy works at any tenure

**Do not cut:** Bean cards, marketplace, Subscribe & Save, palette transition, history card, bean-adapted settings. These are the product.

---

## 21. What Doesn't Change

- Onboarding flow (Screens 1-5)
- Health Data Card (Card 1)
- Brezi's Read (Card 2)
- Emotional layer (R4/R5) — check-in, surprise moments
- Brew profiles — names, parameters, decision logic
- Device Hub, Settings (except new rows)
- All health-side design tokens
- Navigation pattern (single push stack, no tab bar)
- SFSafariViewController for Shopify
- All existing health-side animations

---

*Round 1 built the machine. Round 2 gave it a soul. Round 3 sharpened the edges. Round 4 locked the door new users shouldn't walk through. Round 5 taught the app to notice time passing — to get quieter as it knows you, to acknowledge when you've been away, to change its tone across the day. The health side reads your body. The coffee side reads like a menu at your favorite roaster. Why Today is the moment they shake hands. The handshake gets firmer every morning you show up.*

*150 strings. 14 components. 9 days. Ship it.*

— Picasso
