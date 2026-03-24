# Brezi iOS App — Iteration 4, Round 2 Design Spec
**Author:** Picasso (Design) | March 20, 2026
**Status:** Complete spec for engineering handoff — incorporates all Round 1 feedback
**Architecture:** Locked Iteration 4 — wellness-first, simplified daily flow

---

## Round 2 Changelog (from Round 1)

| # | Change | Source |
|---|---|---|
| 1 | Sleep-safe calculation: `brew_end + 10h` → `brew_end + 15h` (~12.5% remaining) | Elon Fix 1 |
| 2 | Long-press dose acceleration on [+]/[-] (hold > 0.5s, 5g per 0.2s) | Elon Fix 2 |
| 3 | Boundary glow on dose at min (30g) and max (120g) | Picasso addition |
| 4 | History insight line at 3 check-ins: "3 brews logged. Patterns emerging." | Picasso counter to Elon Fix 3 |
| 5 | One-time toast after 3rd check-in: "Noted. A few more and I'll start seeing patterns." | Elon Flag 4 |
| 6 | "BREW ANYWAY" subtext shows projected sleep-safe time | Elon Flag 3 |
| 7 | Kill stale `brew_start + 25h` reference — sleep-safe is `brew_end + 15h` everywhere | Cleanup |

---

## 0. Design Tokens

### Colors

```swift
// === HEALTH / RECOVERY SIDE ===
let black          = Color(hex: "#000000")  // primary text
let nearBlack      = Color(hex: "#0F0F0F")  // card backgrounds, tab bar
let darkSurface    = Color(hex: "#1A1A1A")  // secondary dark surfaces
let blue           = Color(hex: "#488BED")  // accent, CTAs, tab pill
let white          = Color(hex: "#FFFFFF")  // primary text on dark
let offWhite       = Color(hex: "#F5F5F5")  // app background (light screens)
let midGrey        = Color(hex: "#888888")  // secondary text
let lightGrey      = Color(hex: "#E0E0E0")  // dividers
let dimGrey        = Color(hex: "#666666")  // tertiary text
let faintGrey      = Color(hex: "#333333")  // inactive dots, borders on dark

// === BREW CARD / CAFFEINE ===
let amber          = Color(hex: "#D4A96A")  // profile name accent, caffeine card
let amberLight     = Color(hex: "#E8C882")  // caffeine highlight
let amberDim       = Color(hex: "#8C7244")  // caffeine secondary text

// === BEANS TAB ===
let warmCream      = Color(hex: "#F5F0E8")  // beans tab background
let coffeeText     = Color(hex: "#1A1714")  // beans tab primary text
let coffeeMid      = Color(hex: "#8C8279")  // beans tab secondary text
let coffeeLight    = Color(hex: "#B5A99A")  // beans tab tertiary
let coffeeBorder   = Color(hex: "#E5DDD3")  // beans tab dividers
let greenAccent    = Color(hex: "#5B8C5A")  // freshness / in-stock

// === RECOVERY STATUS ===
let statusGreen    = Color(hex: "#34C759")  // peaked
let statusYellow   = Color(hex: "#FFD60A")  // functional
let statusRed      = Color(hex: "#FF453A")  // rebuilding

// === PER-ROASTER TINTS ===
let stitchBg       = Color(hex: "#2E2022")
let regentBg       = Color(hex: "#F5F0E8")
let corvusBg       = Color(hex: "#2A2418")
```

### Typography

```swift
// System fonts — swap to ABC Social before public launch
let t_display   = Font.system(size: 48, weight: .heavy)      // hero numbers (recovery %)
let t_h1        = Font.system(size: 34, weight: .bold)        // screen titles
let t_h2        = Font.system(size: 26, weight: .bold)        // brew profile name
let t_h3        = Font.system(size: 20, weight: .semibold)    // section headers
let t_body      = Font.system(size: 16, weight: .regular)     // body text
let t_bodyBold  = Font.system(size: 16, weight: .semibold)    // emphasized body
let t_small     = Font.system(size: 14, weight: .regular)     // secondary text
let t_caption   = Font.system(size: 13, weight: .regular)     // captions
let t_mono      = Font.system(size: 13, weight: .regular, design: .monospaced)  // data values
let t_eyebrow   = Font.system(size: 11, weight: .medium, design: .monospaced)   // labels, all-caps
let t_pill      = Font.system(size: 15, weight: .medium)      // pill buttons
let t_dose      = Font.system(size: 36, weight: .bold, design: .monospaced)     // dose number
let t_caffeine  = Font.system(size: 18, weight: .semibold, design: .monospaced) // caffeine readout
let t_timer     = Font.system(size: 56, weight: .heavy, design: .monospaced)    // brew timer
let t_tabLabel  = Font.system(size: 10, weight: .medium)      // tab bar labels
```

### Spacing (8pt grid)

```
xs: 4    sm: 8    md: 16    lg: 24    xl: 32    xxl: 48    xxxl: 64
```

### Corner Radii

```
card: 20px       button: 14px     pill: 24px (full-round)
chip: 8px        input: 10px      tab-pill: 16px
bean-card: 16px  brew-card: 24px  timer-card: 20px
```

### Shadows

```swift
// Light mode cards (beans tab)
.shadow(color: .black.opacity(0.06), radius: 12, x: 0, y: 4)
// No shadows on dark cards — they glow, they don't float
```

### Safe Areas

```
Top inset: respect system safe area (Dynamic Island / notch)
Bottom: 88pt reserved for tab bar (including home indicator)
Horizontal padding: 20pt (all screens)
```

---

## 1. Bottom Tab Bar — macOS Dock Style

The tab bar is the persistent anchor. It lives on every screen except onboarding, active brew timer, and modal sheets.

### Layout

```
┌──────────────────────────────────────────┐
│                                          │
│   🏠        ☕        📊        ⚙️       │
│  Home     Beans    History   Settings    │
│                                          │
│   ┌──────┐                               │  ← pill behind selected
│   │ pill │                               │
│   └──────┘                               │
└──────────────────────────────────────────┘
```

### Specs

| Property | Value |
|---|---|
| Background | `#0F0F0F` solid, no blur |
| Height | 88pt (including 34pt home indicator area) |
| Content height | 54pt (icon + label) |
| Top border | 1px `#1A1A1A` |
| Icon size | 24pt SF Symbols |
| Label font | `t_tabLabel` (10px medium) |
| Label spacing | 2pt below icon |
| Inactive color | `#666666` (icon + label) |
| Active color | `#FFFFFF` (icon + label) |
| Selection pill | `#488BED`, 64×36pt, corner radius 16px, centered behind active tab |
| Pill animation | `.spring(response: 0.35, dampingFraction: 0.8)` — slides horizontally |
| Tab icons | `house.fill` · `cup.and.saucer.fill` · `chart.bar.fill` · `gearshape.fill` |
| Haptic | `UIImpactFeedbackGenerator(.light)` on tab switch |

### Tab → Screen Mapping

| Tab | Screen | Background |
|---|---|---|
| 🏠 Home | All Home states (recovery → brew → timer → caffeine) | `#000000` |
| ☕ Beans | Bean marketplace | `#F5F0E8` (warm cream) |
| 📊 History | Brew log + patterns | `#000000` |
| ⚙️ Settings | Preferences | `#000000` |

---

## 2. Home Tab — All States

The Home tab is a single scrollable view that changes its content based on time of day and brew state. No navigation pushes — cards appear and disappear in place.

---

### 2A. Home — Pre-Brew (Morning, No Active Brew)

Default state when user opens app in the morning. Three sections stacked vertically.

```
┌──────────────────────────────────────────┐
│                                          │
│  BREZI                              7:14a│  ← nav area
│                                          │
│ ┌──────────────────────────────────────┐ │
│ │  RECOVERY                            │ │  ← eyebrow, #888, monospaced
│ │                                      │ │
│ │           68%                        │ │  ← t_display (48px), white, centered
│ │                                      │ │
│ │  HRV 62ms  ·  7h 20m sleep          │ │  ← t_mono (13px), #888
│ │                                      │ │
│ │  ● ● ● ● ● ● ○ ○ ○ ○               │ │  ← recovery dots (see spec)
│ │                                      │ │
│ └──────────────────────────────────────┘ │  ← card: #0F0F0F, 20px radius
│                                          │
│ ┌──────────────────────────────────────┐ │
│ │                                      │ │
│ │  Solid night. Not your peak, but     │ │  ← t_body (16px), #CCC
│ │  your body showed up. Meet it        │ │
│ │  where it is.                        │ │
│ │                                      │ │
│ └──────────────────────────────────────┘ │  ← card: #0F0F0F, 20px radius
│                                          │
│ ┌──────────────────────────────────────┐ │
│ │                                      │ │
│ │  TODAY'S BREW                        │ │  ← eyebrow, #488BED, monospaced
│ │                                      │ │
│ │  First Light                         │ │  ← t_h2 (26px), #D4A96A (amber)
│ │  Your everyday standard.             │ │  ← t_small (14px), #888
│ │                                      │ │
│ │  ──────────────────────────────────  │ │  ← divider, #333
│ │                                      │ │
│ │        [−]    60g    [+]             │ │  ← dose control (see spec)
│ │         (medium strength)            │ │  ← t_caption (13px), #888
│ │                                      │ │
│ │  Estimated caffeine   ~120mg         │ │  ← t_small + t_caffeine, #D4A96A
│ │                                      │ │
│ │  ┌────────────────────────────────┐  │ │
│ │  │ ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔  │  │ │  ← brew window bar (see spec)
│ │  │  6a          NOW          2p   │  │ │
│ │  │  Best before 2:00 PM           │  │ │
│ │  └────────────────────────────────┘  │ │
│ │                                      │ │
│ │  ┌────────────────────────────────┐  │ │
│ │  │         START BREW              │  │ │  ← primary CTA
│ │  └────────────────────────────────┘  │ │
│ │                                      │ │
│ │  ▸ Advanced                          │ │  ← collapsed toggle
│ │                                      │ │
│ └──────────────────────────────────────┘ │  ← card: #0F0F0F, 24px radius
│                                          │
│                88pt tab bar              │
└──────────────────────────────────────────┘
```

---

### Recovery Card Specs

| Property | Value |
|---|---|
| Background | `#0F0F0F` |
| Corner radius | 20px |
| Padding | 24pt horizontal, 20pt vertical |
| Eyebrow | "RECOVERY" — `t_eyebrow`, `#888888`, letter-spacing 2pt |
| Percentage | `t_display` (48px heavy), `#FFFFFF`, center-aligned |
| Data line | "HRV {n}ms · {h}h {m}m sleep" — `t_mono`, `#888888` |
| Recovery dots | 10 circles, 8pt diameter, 6pt gap. Filled = `#488BED`, empty = `#333333`. Count = `round(recovery_pct / 10)` |

**Recovery percentage color:**
- 75–100%: `#34C759` (green)
- 50–74%: `#FFD60A` (yellow)
- 0–49%: `#FF453A` (red)

**Data source priority:**
1. WHOOP API (if connected) → real HRV, sleep, recovery %
2. Oura API (if connected) → real HRV, sleep, readiness score mapped to recovery %
3. Manual input → recovery slider (1–10 mapped to 10–100%), no HRV/sleep shown

**Manual input fallback layout** (no wearable connected):

```
┌──────────────────────────────────────┐
│  RECOVERY                            │
│                                      │
│  How are you waking up?              │  ← t_h3 (20px), white
│                                      │
│  ┌──────┐  ┌────────────┐  ┌───────┐│
│  │🟢 High│  │🟡 Functional│  │🔴 Rest ││  ← 3 chips
│  └──────┘  └────────────┘  └───────┘│
│                                      │
│  Or slide:  ○──────●──────○   7     │  ← slider, 1–10
│                                      │
└──────────────────────────────────────┘
```

**Manual chip mapping:**
- 🟢 High → recovery 80%, sets slider to 8
- 🟡 Functional → recovery 60%, sets slider to 6
- 🔴 Rest → recovery 30%, sets slider to 3

---

### Why Today Card Specs

| Property | Value |
|---|---|
| Background | `#0F0F0F` |
| Corner radius | 20px |
| Padding | 24pt horizontal, 20pt vertical |
| Copy font | `t_body` (16px regular), `#CCCCCC` |
| Max lines | 3 (1–2 sentences) |
| Top margin | 12pt below recovery card |

**Why Today Copy — All Variants:**

Recovery 75–100% (Peaked):
1. "Your system is primed. Use this window. It won't last forever."
2. "Peak recovery. This is what discipline earns you."
3. "HRV is up, sleep was clean. Go hard today — you've got the runway."

Recovery 50–74% (Functional):
1. "Solid night. Not your peak, but your body showed up. Meet it where it is."
2. "Middle of the range. Consistent mornings compound — this one counts."
3. "Not your best, not your worst. The kind of day that benefits from a clean start."

Recovery 0–49% (Rebuilding):
1. "Rough night. Your body's rebuilding — don't fight it, work with it."
2. "Low signal today. Gentle start. You'll feel the difference by afternoon."
3. "Recovery's down. Pull back the dose and let your system catch up."

First-time user:
1. "Welcome. Your first brew is about to set the baseline. Pay attention to how it lands."

**Selection logic:** Random from tier, no repeat within 3 days. Store last 3 shown in `UserDefaults` array `recentWhyTodayCopy`.

---

### Brew Card Specs (THE MOST IMPORTANT COMPONENT)

This is the heart of the app. A recommendation card, not a settings screen.

| Property | Value |
|---|---|
| Background | `#0F0F0F` |
| Corner radius | 24px |
| Padding | 24pt horizontal, 28pt vertical |
| Top margin | 12pt below Why Today card |
| Eyebrow | "TODAY'S BREW" — `t_eyebrow`, `#488BED`, letter-spacing 2pt |
| Profile name | `t_h2` (26px bold), `#D4A96A` (amber) |
| Tagline | `t_small` (14px), `#888888` |
| Divider | 1px `#333333`, full width minus padding, 16pt vertical margin |

#### Profile Names + Taglines (locked)

| Recovery % | Profile | Tagline |
|---|---|---|
| 0–25% | Tide Pool | "Recovery mode. Smooth and low." |
| 26–49% | Tide Pool | "Your body's rebuilding. Go gentle." |
| 50–64% | First Light | "Your everyday standard." |
| 65–74% | First Light | "Solid ground. Clean start." |
| 75–84% | Deep Work | "Focused extraction. Sustained output." |
| 85–94% | Voltage | "System primed. Full send." |
| 95–100% | Voltage | "Peak day. Don't waste this." |

#### Dose Control

```
          [−]    60g    [+]
           (medium strength)
```

| Property | Value |
|---|---|
| Minus button | 36×36pt circle, 1px border `#666`, "−" in `t_h3` white. Tap: −5g |
| Plus button | 36×36pt circle, 1px border `#666`, "+" in `t_h3` white. Tap: +5g |
| Dose number | `t_dose` (36px bold mono), `#FFFFFF`, center-aligned between buttons |
| Unit label | "g" appended to number — same font, `#888888` |
| Strength label | `t_caption` (13px), `#888888`, centered below dose |
| Min dose | 30g |
| Max dose | 120g |
| Step | 5g |
| Haptic | `UIImpactFeedbackGenerator(.light)` on each tap |

**⟐ ROUND 2 — Long-press dose acceleration:**

| Property | Value |
|---|---|
| Trigger | Hold [+] or [−] for > 0.5s |
| Behavior | Auto-increment 5g every 0.2s in held direction |
| Haptic | `.light` per tick (same as single tap) |
| Scale animation | Dose number pulses at 1.08 (not 1.15) per tick — subtler for continuous gesture |
| Cancel | On touch-up, timer cancels immediately |
| Boundary | Stops at min (30g) or max (120g), does not wrap |

```swift
// Long-press acceleration
.onLongPressGesture(minimumDuration: 0.5) {
    startAccelerating(direction: .up) // or .down
}
// Timer.publish(every: 0.2) — each tick:
//   dose += 5 * direction
//   UIImpactFeedbackGenerator(style: .light).impactOccurred()
//   doseScale = 1.08 → 1.0 (spring, 0.15s)
// On touch-up: cancel timer
```

**⟐ ROUND 2 — Boundary glow at min/max:**

When dose reaches 30g (min) or 120g (max), the dose number flashes its color for 1 frame:
- At **30g (min)**: dose number flashes `#FF453A` (red) for 0.15s, then returns to `#FFFFFF`
- At **120g (max)**: dose number flashes `#488BED` (blue) for 0.15s, then returns to `#FFFFFF`
- Signals "you've hit the boundary" without a modal or toast

```swift
// Boundary glow
if dose == minDose || dose == maxDose {
    withAnimation(.easeOut(duration: 0.15)) {
        doseColor = dose == minDose ? statusRed : blue
    }
    DispatchQueue.main.asyncAfter(deadline: .now() + 0.15) {
        withAnimation(.easeIn(duration: 0.15)) {
            doseColor = white
        }
    }
}
```

**Dose → strength label mapping:**

| Dose | Strength Label | Caffeine |
|---|---|---|
| 30–40g | "(light)" | ~60–80mg |
| 45–55g | "(mild)" | ~90–110mg |
| 60–70g | "(medium strength)" | ~120–140mg |
| 75–85g | "(strong)" | ~150–170mg |
| 90–100g | "(very strong)" | ~180–200mg |
| 105–120g | "(maximum)" | ~210–240mg |

**Caffeine formula:** `dose_g × 2.0` — display as "~{value}mg"

**Default dose per profile:**

| Profile | Default Dose |
|---|---|
| Tide Pool | 40g |
| First Light | 60g |
| Deep Work | 75g |
| Voltage | 90g |

#### Dose Tap Animation

```swift
// On [−] or [+] single tap:
withAnimation(.spring(response: 0.2, dampingFraction: 0.6)) {
    doseScale = 1.15  // dose number briefly scales up
}
withAnimation(.spring(response: 0.2, dampingFraction: 0.6).delay(0.1)) {
    doseScale = 1.0   // returns to normal
}
// Caffeine number cross-fades (0.15s)
// Strength label cross-fades if threshold crossed (0.2s)
UIImpactFeedbackGenerator(style: .light).impactOccurred()
```

#### Estimated Caffeine Line

```
  Estimated caffeine   ~120mg
```

| Property | Value |
|---|---|
| Label | "Estimated caffeine" — `t_small` (14px), `#888888`, left-aligned |
| Value | "~{caffeine}mg" — `t_caffeine` (18px semibold mono), `#D4A96A`, right-aligned |
| Layout | Horizontal stack, space-between, 12pt below dose control |
| Update | Cross-fade on dose change, 0.15s |

#### Brew Window Bar

A horizontal progress bar showing the optimal brew window.

```
  ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔
  6a         NOW          2p
  Best before 2:00 PM
```

| Property | Value |
|---|---|
| Bar height | 6pt |
| Bar background | `#333333` |
| Bar fill | Linear gradient `#488BED` → `#34C759`, left-to-right, width = now_position / total_window |
| Bar radius | 3px (full round) |
| NOW marker | 4pt wide × 14pt tall white rectangle at current time position, radius 2px |
| Time labels | "6a" and "2p" — `t_eyebrow` (11px), `#666666`, positioned at bar start/end |
| "NOW" label | `t_eyebrow` (11px), `#FFFFFF`, centered above marker (only if current time is within window) |
| Subtext | "Best before {cutoff_time}" — `t_caption` (13px), `#888888`, centered below bar |
| Top margin | 16pt below caffeine line |
| Total width | Full card width minus padding |

**Window calculation:**
- Start: user wake time (default 6:00 AM, configurable in Settings)
- End: caffeine cutoff goal (default 2:00 PM, configurable in Settings)
- If current time is past cutoff: bar is full, subtext changes to "Past your brew window" in `#FF453A`

#### START BREW Button

| Property | Value |
|---|---|
| Background | `#488BED` |
| Corner radius | 14px |
| Height | 54pt |
| Width | Full card width minus padding |
| Text | "START BREW" — `Font.system(size: 17, weight: .bold)`, `#FFFFFF`, centered |
| Top margin | 20pt below brew window bar |
| Haptic | `UIImpactFeedbackGenerator(.medium)` on tap |
| Animation | Scale 0.97 on press, spring back on release |

**⟐ ROUND 2 — Past-window state with projected sleep-safe time:**

If current time > cutoff, button and subtext change:

| Property | Value |
|---|---|
| Button background | `#333333` |
| Button text | "BREW ANYWAY" |
| Subtext line 1 | "Past your window. Sleep-safe by {projected_time}." — `t_caption`, `#888888` |
| Subtext line 2 | "Still your call." — `t_caption`, `#666666` |
| Projected time calculation | `Date() + estimated_brew_duration + 15h` |
| `estimated_brew_duration` | Use profile's recommended steep time (default 12 min) |

Example at 3:30 PM: "Past your window. Sleep-safe by 6:42 AM." — The data does the discouraging. User keeps agency.

#### Advanced Toggle (collapsed by default)

```
  ▸ Advanced
```

Tapped:

```
  ▾ Advanced

  Grind       Medium-coarse
  Temp        Cold (4°C)
  Ratio       1:15

  These are defaults for First Light.
  Adjust if you know your setup.
```

| Property | Value |
|---|---|
| Toggle label | "Advanced" — `t_small` (14px), `#888888` |
| Chevron | `chevron.right` rotates 90° on expand, `#888888`, 12pt |
| Top margin | 12pt below START BREW |
| Expanded area padding | 16pt top |
| Setting labels | `t_caption` (13px), `#888888`, left-aligned |
| Setting values | `t_caption` (13px), `#FFFFFF`, right-aligned |
| Row height | 36pt |
| Row divider | 1px `#1A1A1A` |
| Footer text | `t_caption` (13px), `#666666`, 8pt top margin |
| Expand animation | `.easeInOut(duration: 0.25)` |

**Advanced defaults per profile:**

| Profile | Grind | Temp | Ratio |
|---|---|---|---|
| Tide Pool | Coarse | Cold (4°C) | 1:16 |
| First Light | Medium-coarse | Cold (4°C) | 1:15 |
| Deep Work | Medium | Cold (4°C) | 1:14 |
| Voltage | Medium-fine | Cold (4°C) | 1:12 |

---

### 2B. Home — Brewing (Active Brew Timer)

After START BREW is tapped, the entire Home screen transforms. Tab bar hides.

```
┌──────────────────────────────────────────┐
│                                          │
│                                          │
│                                          │
│                                          │
│          BREWING · First Light           │  ← eyebrow + profile, #888 + #D4A96A
│                                          │
│              12:00                       │  ← t_timer (56px), white, counting up
│                                          │
│         ┌────── ○ ──────┐                │  ← circular progress ring
│         │                │                │
│         │   60g · 4°C    │                │  ← t_mono, #888, centered in ring
│         │                │                │
│         └────────────────┘                │
│                                          │
│      ~120mg caffeine incoming            │  ← t_small, #D4A96A
│                                          │
│                                          │
│  ┌────────────────────────────────────┐  │
│  │            END BREW                 │  │  ← red-tinted button
│  └────────────────────────────────────┘  │
│                                          │
│  You can close the app. We'll keep       │  ← t_caption, #666
│  counting.                               │
│                                          │
└──────────────────────────────────────────┘
```

| Property | Value |
|---|---|
| Background | `#000000` full bleed |
| Tab bar | Hidden during brew (slide down, 0.3s) |
| Status bar | Light content |
| Eyebrow | "BREWING ·" `t_eyebrow` `#888` + profile name `t_eyebrow` `#D4A96A` |
| Timer | `t_timer` (56px heavy mono), `#FFFFFF`, counts UP from 00:00 |
| Timer format | "MM:SS" — switches to "H:MM:SS" after 60 min |
| Progress ring | 120pt diameter, 4pt stroke, `#333` track, `#488BED` fill |
| Ring progress | Fills over recommended steep time for profile (see table below) |
| Ring animation | `.linear` — smooth continuous fill |
| Inner text | "{dose}g · {temp}" — `t_mono`, `#888888` |
| Caffeine note | "~{caffeine}mg caffeine incoming" — `t_small`, `#D4A96A` |
| END BREW button | Background `#FF453A`, 54pt tall, 14px radius, "END BREW" bold white |
| Helper text | "You can close the app. We'll keep counting." — `t_caption`, `#666` |
| Live Activity | iOS Live Activity on lock screen showing elapsed time (if available) |

**Recommended steep times per profile:**

| Profile | Steep Time |
|---|---|
| Tide Pool | 10 min |
| First Light | 12 min |
| Deep Work | 14 min |
| Voltage | 16 min |

**Timer behavior:**
- Counts up continuously from 00:00
- At recommended steep time, ring completes + subtle pulse animation (3 pulses at 1.05 scale)
- Timer keeps counting past recommended time — no auto-stop
- Persists across app close/reopen via `brewStartTime` in `UserDefaults`
- END BREW tap stops timer and transitions to post-brew state

**END BREW transition:**
```swift
withAnimation(.easeInOut(duration: 0.4)) {
    // Timer fades out
    // Tab bar slides back in
    // Caffeine countdown card fades in from below
}
```

---

### 2C. Home — Post-Brew (Caffeine Countdown)

After END BREW, the Home screen shows the caffeine countdown card and optional check-in.

```
┌──────────────────────────────────────────┐
│                                          │
│  BREZI                            12:26p │
│                                          │
│ ┌──────────────────────────────────────┐ │
│ │                                    ✕ │ │  ← dismiss button
│ │  ⚡ CAFFEINE ACTIVE                  │ │  ← eyebrow, amber
│ │                                      │ │
│ │  Peak in 33 min                      │ │  ← primary line, live countdown
│ │                                      │ │
│ │  ½ life at 5:14 PM                   │ │  ← secondary, static
│ │  🌙 Sleep-safe by 3:14 AM            │ │  ← tertiary, static ⟐ R2: brew_end + 15h
│ │                                      │ │
│ │  ┌──────────────────────────────┐    │ │
│ │  │▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░░░░░ │    │ │  ← caffeine decay bar
│ │  │ ~120mg              → 0mg   │    │ │
│ │  └──────────────────────────────┘    │ │
│ │                                      │ │
│ │  First Light · 60g · brewed 12:14p   │ │  ← brew summary, #888
│ │                                      │ │
│ └──────────────────────────────────────┘ │  ← card: #0F0F0F, amber left border
│                                          │
│ ┌──────────────────────────────────────┐ │
│ │                                      │ │
│ │  How did today's brew land?          │ │  ← check-in card (see spec)
│ │                                      │ │
│ │  ┌──────┐ ┌────────────┐ ┌────────┐ │ │
│ │  │👍 Right│ │➕ Needed more│ │➖ Too much│ │ │
│ │  └──────┘ └────────────┘ └────────┘ │ │
│ │                                      │ │
│ │  Skip                               │ │
│ │                                      │ │
│ └──────────────────────────────────────┘ │
│                                          │
│                88pt tab bar              │
└──────────────────────────────────────────┘
```

---

### Caffeine Countdown Card Specs

| Property | Value |
|---|---|
| Background | `#0F0F0F` |
| Corner radius | 20px |
| Left border | 3px `#D4A96A` (amber), full height |
| Padding | 20pt horizontal, 20pt vertical |
| Dismiss button | "✕" 14pt `#666`, top-right, 32×32pt tap target |

#### Caffeine Card Copy — All States

**State 1: Counting to peak** (0–45 min after brew end)

| Line | Value | Style |
|---|---|---|
| Eyebrow | "⚡ CAFFEINE ACTIVE" | `t_eyebrow`, `#D4A96A`, letter-spacing 2pt |
| Primary | "Peak in {minutes} min" | `t_h3` (20px semibold), `#FFFFFF` |
| Secondary | "½ life at {time}" | `t_small` (14px), `#888888` |
| Tertiary | "🌙 Sleep-safe by {time}" | `t_small` (14px), `#888888` |

**State 2: At peak** (exactly at 45 min, holds for 5 min)

| Line | Value | Style |
|---|---|---|
| Eyebrow | "⚡ CAFFEINE PEAK" | `t_eyebrow`, `#E8C882` (bright amber) |
| Primary | "You're at peak right now" | `t_h3`, `#FFFFFF` |
| Secondary | "½ life at {time}" | `t_small`, `#888888` |
| Tertiary | "🌙 Sleep-safe by {time}" | `t_small`, `#888888` |

**State 3: Post-peak, counting to clear** (50 min – sleep-safe time)

| Line | Value | Style |
|---|---|---|
| Eyebrow | "⚡ CAFFEINE CLEARING" | `t_eyebrow`, `#8C7244` (dim amber) |
| Primary | "½ life in {time}" or "½ life passed" | `t_h3`, `#FFFFFF` |
| Secondary | "🌙 Sleep-safe by {time}" | `t_small`, `#888888` |

**State 4: Cleared** (past sleep-safe time)

| Line | Value | Style |
|---|---|---|
| Eyebrow | "✓ CAFFEINE CLEARED" | `t_eyebrow`, `#34C759` |
| Primary | "You're clear. Recovery window open." | `t_h3`, `#FFFFFF` |

**⟐ ROUND 2 — Corrected time calculations:**

| Metric | Formula | Example (brew end 7:12 AM) |
|---|---|---|
| Peak time | `brew_end + 45min` | 7:57 AM |
| Half-life | `brew_end + 5h` | 12:12 PM |
| Sleep-safe | `brew_end + 15h` | 10:12 PM |

At 120mg initial and 15h elapsed: `120 × 0.5^(15/5) = 120 × 0.125 = 15mg` — genuinely sleep-safe.

**Why 15h (not 10h):** A WHOOP user who brews at 7 AM, gets told "sleep-safe by 5:12 PM" (10h), then sees their HRV dip that night will never trust the timer again. At 15h, remaining caffeine is ~15mg — below the threshold that measurably affects HRV for most adults. The timer earns trust by being conservative enough that wearable data confirms it, not contradicts it.

**Countdown format:**
- Under 60 min: "{n} min" (whole minutes, updates every 60s)
- Over 60 min: "{h}h {m}m"

#### Caffeine Decay Bar

| Property | Value |
|---|---|
| Width | Full card width minus padding |
| Height | 8pt |
| Background | `#333333` |
| Fill | Gradient `#D4A96A` → `#8C7244`, width = `remaining_caffeine / initial_caffeine` |
| Radius | 4px (full round) |
| Left label | "~{current_mg}mg" — `t_eyebrow`, `#D4A96A` |
| Right label | "→ 0mg" — `t_eyebrow`, `#666666` |
| Update | Bar width animates smoothly (`.linear`, recalculates every 60s) |

**Decay formula:**
```
remaining_mg = initial_mg × (0.5 ^ (elapsed_hours / 5.0))
```

#### Brew Summary Line

"First Light · 60g · brewed {time}" — `t_caption` (13px), `#888888`

#### Auto-Fade Behavior

- When sleep-safe time passes: card transitions to "cleared" state
- If dismissed (X tapped): card slides up and out (0.3s), stored in `UserDefaults` as dismissed
- If not dismissed: auto-fades 30 min after sleep-safe time (opacity → 0 over 2s, then removed)

#### Caffeine Card Fade-In Animation

```swift
// On END BREW tap:
withAnimation(.easeOut(duration: 0.5).delay(0.3)) {
    caffeineCardOpacity = 1.0   // fades in from 0
    caffeineCardOffset = 0      // slides up from 20pt below
}
```

---

### Check-In Card Specs

| Property | Value |
|---|---|
| Background | `#0F0F0F` |
| Corner radius | 20px |
| Padding | 20pt horizontal, 20pt vertical |
| Top margin | 12pt below caffeine card |
| Appears | 30 min after brew ends (not immediately) |

#### Layout

| Element | Value |
|---|---|
| Question | "How did today's brew land?" — `t_bodyBold` (16px semibold), `#FFFFFF` |
| Pill row | Horizontal stack, 8pt gap, wraps if needed |
| Pill 1 | "👍 Right" |
| Pill 2 | "➕ Needed more" |
| Pill 3 | "➖ Too much" |
| Skip | "Skip" — `t_small` (14px), `#888888`, underline, centered, 12pt below pills |

#### Pill Button Specs

| Property | Value |
|---|---|
| Background (default) | `#1A1A1A` |
| Background (selected) | `#488BED` |
| Border | 1px `#333333` (default) / none (selected) |
| Corner radius | 24px (full pill) |
| Height | 40pt |
| Horizontal padding | 16pt |
| Text | `t_pill` (15px medium), `#FFFFFF` |
| Haptic | `UIImpactFeedbackGenerator(.light)` on selection |

#### Check-In Behavior

- Tap a pill → pill highlights blue → card auto-dismisses after 0.8s with a fade-up
- Tap "Skip" → card dismisses immediately (fade-up, 0.3s)
- Selection saved to `BrewLog.checkInRating` (values: `"right"`, `"needed_more"`, `"too_much"`, `"skipped"`)
- Card does not reappear once dismissed or rated for this brew
- Gate: `UserDefaults.bool(forKey: "showCheckIn")` — defaults true, can be disabled in Settings

**⟐ ROUND 2 — One-time toast after 3rd check-in:**

After the user's 3rd check-in rating (not skip), show a toast:

| Property | Value |
|---|---|
| Copy | "Noted. A few more and I'll start seeing patterns." |
| Style | `t_small` (14px), `#FFFFFF`, on `#1A1A1A` background with `#488BED` left border |
| Position | Bottom of screen, 100pt above tab bar |
| Duration | 2.5s, then fade out (0.3s) |
| Corner radius | 12px |
| Padding | 12pt horizontal, 10pt vertical |
| Gate | `UserDefaults.bool(forKey: "hasShownPatternTeaser")` — set to `true` after showing. Never shown again. |
| Trigger | Check after each non-skip check-in: `if checkInCount == 3 && !hasShownPatternTeaser` |

#### Check-In Fade-In Animation

```swift
// 30 min after brew end:
withAnimation(.easeOut(duration: 0.4)) {
    checkInOpacity = 1.0
    checkInOffset = 0  // slides up from 16pt below
}
```

---

### 2D. Home — Evening / Cleared

When caffeine has cleared (or if no brew today and it's past cutoff time).

```
┌──────────────────────────────────────────┐
│                                          │
│  BREZI                            8:42p  │
│                                          │
│                                          │
│                                          │
│                                          │
│       Caffeine cleared.                  │  ← t_h3 (20px), #FFFFFF, centered
│       Recovery window open.              │  ← t_body (16px), #888, centered
│                                          │
│                                          │
│       Sleep well tonight.                │  ← t_small (14px), #666, centered
│                                          │
│                                          │
│                                          │
│                88pt tab bar              │
└──────────────────────────────────────────┘
```

| Property | Value |
|---|---|
| Background | `#000000` |
| Layout | Centered vertically (offset up by 44pt to account for tab bar) |
| Primary | "Caffeine cleared." — `t_h3`, `#FFFFFF` |
| Secondary | "Recovery window open." — `t_body`, `#888888` |
| Tertiary | "Sleep well tonight." — `t_small`, `#666666`, 24pt below secondary |

**If no brew today and it's past cutoff:**
- Primary: "No brew today."
- Secondary: "Your recovery window starts now."
- Tertiary: "Rest days count too."

---

## 3. Onboarding — 4 Screens, Under 2 Minutes

**Trigger:** First launch only. Store `hasCompletedOnboarding: Bool` in `UserDefaults`.

### Shared Onboarding Shell

| Property | Value |
|---|---|
| Background | `#000000` full bleed |
| Status bar | Light content |
| Progress dots | 4 dots, 8pt diameter, 6pt gap, bottom center above safe area |
| Active dot | `#488BED` |
| Inactive dot | `#333333` |
| Skip button | "Skip" — `t_small`, `#888888`, top-right, 16pt from edges |
| Swipe | System back swipe enabled between screens |
| Tab bar | Hidden during onboarding |

---

### Screen 1 — The Hook

```
┌──────────────────────────────────────────┐
│                                     Skip │
│                                          │
│                                          │
│                                          │
│  Cold is                                 │
│  a practice.                             │  ← 48px heavy, white
│                                          │
│  Most people compromise their            │
│  coffee. Ice dilutes it. Hot brew        │
│  oxidizes it. You've been drinking       │  ← 17px regular, #CCC, 1.5 line-height
│  the workaround this whole time.         │
│                                          │
│                                          │
│  ┌────────────────────────────────────┐  │
│  │       Start the practice →         │  │  ← blue button
│  └────────────────────────────────────┘  │
│                                          │
│          ●  ○  ○  ○                      │
└──────────────────────────────────────────┘
```

| Element | Copy (exact) |
|---|---|
| Headline | "Cold is a practice." |
| Body | "Most people compromise their coffee. Ice dilutes it. Hot brew oxidizes it. You've been drinking the workaround this whole time." |
| CTA | "Start the practice →" |

**Headline spec:** `Font.system(size: 48, weight: .heavy)`, `#FFFFFF`, leading-aligned, 20pt left padding.

**Body spec:** `Font.system(size: 17, weight: .regular)`, `#CCCCCC`, 1.5 line spacing, 20pt left padding, 12pt top margin.

**CTA spec:** `#488BED` background, 54pt height, 14px radius, `Font.system(size: 17, weight: .bold)`, `#FFFFFF`, centered, 20pt horizontal margin.

**Animation:**
- Headline: fade up from 12pt below, 0.4s ease-out, 0.1s delay
- Body: fade in 0.3s, 0.2s delay after headline settles
- Button: fade in 0.2s, 0.3s delay after body

---

### Screen 2 — The Why

```
┌──────────────────────────────────────────┐
│                                     Skip │
│                                          │
│                                          │
│  12 minutes.                             │
│  No ice. No shortcuts.                   │  ← 44px heavy, white
│                                          │
│  Brezi brews at cold temperature —       │
│  not chilled-after-the-fact. The         │  ← 17px regular, #CCC
│  difference isn't subtle.                │
│                                          │
│  Your machine. Your data.                │
│  Your brew.                              │  ← 13px mono, #488BED
│                                          │
│  ┌────────────────────────────────────┐  │
│  │         Makes sense →              │  │
│  └────────────────────────────────────┘  │
│                                          │
│          ○  ●  ○  ○                      │
└──────────────────────────────────────────┘
```

| Element | Copy (exact) |
|---|---|
| Headline | "12 minutes. No ice. No shortcuts." |
| Body | "Brezi brews at cold temperature — not chilled-after-the-fact. The difference isn't subtle." |
| Accent | "Your machine. Your data. Your brew." |
| CTA | "Makes sense →" |

**Headline spec:** `Font.system(size: 44, weight: .heavy)`, `#FFFFFF`.

**Accent spec:** `t_mono` (13px), `#488BED`, 16pt top margin.

---

### Screen 3 — The Data Ask

```
┌──────────────────────────────────────────┐
│                                     Skip │
│                                          │
│                                          │
│  Your coffee should                      │
│  know how you slept.                     │  ← 40px heavy, white
│                                          │
│  Connect WHOOP or Oura and Brezi         │
│  reads your recovery each morning.       │  ← 17px regular, #CCC
│  Or just tell us how you feel.           │
│                                          │
│  ┌────────────────────────────────────┐  │
│  │  Connect WHOOP                     │  │  ← blue button
│  └────────────────────────────────────┘  │
│  ┌────────────────────────────────────┐  │
│  │  Connect Oura                      │  │  ← outlined button, #488BED border
│  └────────────────────────────────────┘  │
│                                          │
│  I'll log manually →                     │  ← text link, #888
│                                          │
│          ○  ○  ●  ○                      │
└──────────────────────────────────────────┘
```

| Element | Copy (exact) |
|---|---|
| Headline | "Your coffee should know how you slept." |
| Body | "Connect WHOOP or Oura and Brezi reads your recovery each morning. Or just tell us how you feel." |
| CTA Primary | "Connect WHOOP" |
| CTA Secondary | "Connect Oura" |
| CTA Tertiary | "I'll log manually →" |

**Headline spec:** `Font.system(size: 40, weight: .heavy)`, `#FFFFFF`.

**CTA Primary:** `#488BED` background, 54pt height, 14px radius, bold white text.

**CTA Secondary:** Transparent background, 1.5px `#488BED` border, 54pt height, 14px radius, `#488BED` text.

**CTA Tertiary:** "I'll log manually →" — `t_small` (14px), `#888888`, centered, 16pt top margin. Text link, no button chrome.

**Connect buttons:** Initiate OAuth flow for respective wearable. On success → green checkmark replaces button text ("Connected ✓" in `#34C759`). On failure → "Connection failed. Try again." inline error in `#FF453A`.

**"I'll log manually":** Skips wearable, sets `inputMode: "manual"` in UserDefaults.

---

### Screen 4 — First Recovery Input

```
┌──────────────────────────────────────────┐
│                                          │
│                                          │
│  How are you waking                      │
│  up today?                               │  ← 40px heavy, white
│                                          │
│  No wrong answers. Brezi works           │
│  with whatever you've got.               │  ← 15px regular, #888
│                                          │
│  ┌────────────────────────────────────┐  │
│  │  🟢  Peaked                        │  │
│  │  HRV high, slept clean, ready      │  │  ← selection card
│  └────────────────────────────────────┘  │
│  ┌────────────────────────────────────┐  │
│  │  🟡  Functional                    │  │
│  │  Decent night, a little behind     │  │
│  └────────────────────────────────────┘  │
│  ┌────────────────────────────────────┐  │
│  │  🔴  Rebuilding                    │  │
│  │  Wrecked it yesterday, paying now  │  │
│  └────────────────────────────────────┘  │
│                                          │
│  ┌────────────────────────────────────┐  │
│  │      Build my first brew →         │  │  ← blue, active after selection
│  └────────────────────────────────────┘  │
│                                          │
│          ○  ○  ○  ●                      │
└──────────────────────────────────────────┘
```

| Element | Copy (exact) |
|---|---|
| Headline | "How are you waking up today?" |
| Subhead | "No wrong answers. Brezi works with whatever you've got." |
| Option 1 | "🟢 Peaked" / "HRV high, slept clean, ready" |
| Option 2 | "🟡 Functional" / "Decent night, a little behind" |
| Option 3 | "🔴 Rebuilding" / "Wrecked it yesterday, paying now" |
| CTA | "Build my first brew →" |

**Headline spec:** `Font.system(size: 40, weight: .heavy)`, `#FFFFFF`.

**Subhead spec:** `Font.system(size: 15, weight: .regular)`, `#888888`, 8pt top margin.

**Selection card spec:**

| Property | Value |
|---|---|
| Default bg | `#1A1A1A` |
| Default border | 1px `#333333` |
| Selected bg | `rgba(72, 139, 237, 0.12)` |
| Selected border | 2px `#488BED` |
| Corner radius | 16px |
| Padding | 20pt horizontal, 16pt vertical |
| Emoji | 28pt, left-aligned |
| Label | `Font.system(size: 20, weight: .bold)`, `#FFFFFF` |
| Sublabel | `Font.system(size: 14, weight: .regular)`, `#888888` |
| Selection animation | `.spring(response: 0.3, dampingFraction: 0.7)` with `scaleEffect(1.01)` |
| Card gap | 12pt vertical |

**CTA disabled state:** `#333333` bg, `#666666` text, no tap response until selection made.

**Onboarding → App transition:**
1. "Build my first brew →" tapped
2. Full-screen black, centered text: "Reading your signal..." — `t_mono` 16px white, pulse opacity 1.0↔0.5 (1.2s loop)
3. Max 0.8s pause, then transition to Home tab with pre-brew state
4. Set `hasCompletedOnboarding = true`
5. Recovery score mapped: Peaked → 85%, Functional → 60%, Rebuilding → 30%

---

## 4. Beans Tab

A completely different world. Warm cream background. Story-forward. Specialty coffee brand energy. This is NOT the health side of the app.

### Beans Tab — Main Screen

```
┌──────────────────────────────────────────┐
│                                          │
│  Beans                                   │  ← t_h1 (34px bold), #1A1714
│                                          │
│  FOR YOU TODAY                           │  ← eyebrow, #8C8279, monospaced
│  Based on your First Light profile       │  ← t_small, #B5A99A
│                                          │
│  ┌─────────────────┐                     │
│  │                 │                     │
│  │  [bean image]   │  ← horizontal scroll│
│  │                 │                     │
│  │  REGENT COFFEE  │                     │
│  │  Ethiopia Guji  │                     │
│  │  Hambela        │                     │
│  │                 │                     │
│  │  cocoa · jasmine│                     │
│  │  strawberry     │                     │
│  │                 │                     │
│  │  $27.00         │                     │
│  │                 │                     │
│  └─────────────────┘                     │
│                                          │
│  ──────────────────────────────────────  │
│                                          │
│  BROWSE ALL                              │  ← eyebrow, #8C8279
│                                          │
│  ┌────────────┐  ┌────────────┐          │
│  │            │  │            │          │
│  │ [image]    │  │ [image]    │          │
│  │            │  │            │          │  ← 2-column grid
│  │ STITCH     │  │ CORVUS     │          │
│  │ Dark Field │  │ Sadayana   │          │
│  │            │  │ Winey      │          │
│  │ Price TBD  │  │ Price TBD  │          │
│  └────────────┘  └────────────┘          │
│                                          │
│  ┌────────────┐                          │
│  │            │                          │
│  │ [image]    │                          │
│  │            │                          │
│  │ REGENT     │                          │
│  │ Ethiopia   │                          │
│  │ Guji       │                          │
│  │ $27.00     │                          │
│  └────────────┘                          │
│                                          │
│                88pt tab bar              │
└──────────────────────────────────────────┘
```

### Beans Tab Specs

| Property | Value |
|---|---|
| Background | `#F5F0E8` (warm cream) |
| Title | "Beans" — `t_h1` (34px bold), `#1A1714`, 20pt left, 16pt top from safe area |
| Section headers | `t_eyebrow` (11px medium mono), `#8C8279`, letter-spacing 2pt |
| Section subtext | `t_small` (14px), `#B5A99A` |
| Section gap | 32pt between For You and Browse All |

#### "For You Today" Section

- Horizontal scroll, `.horizontal, showsIndicators: false`
- Card width: 200pt
- Card gap: 12pt
- Leading padding: 20pt (aligns with screen padding)
- Shows beans matching current active profile's `bestForProfiles`
- If Tide Pool / First Light active → shows Regent
- If Deep Work / Voltage active → shows Stitch, Corvus

#### "For You" Copy Per Profile

| Active Profile | Header | Subtext |
|---|---|---|
| Tide Pool | "FOR YOU TODAY" | "Gentle beans for a recovery day" |
| First Light | "FOR YOU TODAY" | "Based on your First Light profile" |
| Deep Work | "FOR YOU TODAY" | "Dense beans for focused work" |
| Voltage | "FOR YOU TODAY" | "High-intensity beans for peak days" |

#### Bean Card — Marketplace Context

| Property | Value |
|---|---|
| Background | `#FFFFFF` |
| Corner radius | 16px |
| Shadow | `.shadow(color: .black.opacity(0.06), radius: 12, x: 0, y: 4)` |
| Image area | Full width, 140pt tall, background: `bean.imageBgTint` |
| Image | `bean.bagImageName` — aspect fill, centered |
| Content padding | 12pt horizontal, 12pt vertical |
| Roaster name | `t_eyebrow` (11px), `#8C8279`, all-caps |
| Bean name | `Font.system(size: 16, weight: .semibold)`, `#1A1714` |
| Bean subtitle | `t_caption` (13px), `#B5A99A` (if non-empty) |
| Tasting notes | `t_caption` (13px), `#8C8279`, joined with " · " |
| Price | `Font.system(size: 15, weight: .bold)`, `#1A1714` |
| Price TBD | "Price TBD" — `t_caption`, `#B5A99A` |

#### "Browse All" Section — 2-Column Grid

| Property | Value |
|---|---|
| Layout | `LazyVGrid(columns: [GridItem(.flexible()), GridItem(.flexible())], spacing: 12)` |
| Horizontal padding | 20pt |
| Shows all 3 beans regardless of profile |
| Same card component as "For You" but smaller: image area 100pt tall |

---

### Bean Detail Screen

Pushed when any bean card is tapped. Navigation push within Beans tab.

```
┌──────────────────────────────────────────┐
│ ‹ Beans                                  │
│                                          │
│  ┌──────────────────────────────────────┐│
│  │                                      ││
│  │         [bean bag image]             ││  ← full width, 280pt, bg tint
│  │                                      ││
│  └──────────────────────────────────────┘│
│                                          │
│  REGENT COFFEE                           │  ← eyebrow, #8C8279
│  Ethiopia Guji Hambela                   │  ← t_h2 (26px bold), #1A1714
│  Hambela Washed                          │  ← t_body (16px), #B5A99A
│                                          │
│  "Bright, floral, stone fruit.           │
│   Exceptional cold clarity."             │  ← t_body (16px italic), #1A1714
│                                          │
│  ──────────────────────────────────────  │
│                                          │
│  TASTING NOTES                           │  ← eyebrow
│  🫘 Cocoa  🌸 Jasmine  🍓 Strawberry     │
│  🍋 Lime                                 │  ← icon + label pills
│                                          │
│  ──────────────────────────────────────  │
│                                          │
│  FLAVOR PROFILE                          │  ← eyebrow
│                                          │
│  Roast     ○───────────────●── Light     │  ← slider at 0.15
│  Sweetness ──────────●──────── High      │  ← slider at 0.80
│  Acidity   ─────────●───────── High      │  ← slider at 0.75
│                                          │
│  ──────────────────────────────────────  │
│                                          │
│  ORIGIN                                  │  ← eyebrow
│  Ethiopia, Guji — Hambela                │
│  Altitude: 1,800-2,200 masl             │
│  Harvest: Oct-Jan                        │
│  Process: Washed                         │
│                                          │
│  "Varietal: Paru Biyu. Washed process    │
│   strips everything back to the seed,    │
│   leaving only what the altitude         │
│   put there."                            │  ← origin story
│                                          │
│  ──────────────────────────────────────  │
│                                          │
│  BEST FOR                                │  ← eyebrow
│  ┌──────────┐  ┌────────────┐            │
│  │ Tide Pool│  │ First Light│            │  ← profile chips
│  └──────────┘  └────────────┘            │
│                                          │
│  ──────────────────────────────────────  │
│                                          │
│  ┌────────────────────────────────────┐  │
│  │       Add to Cart — $27.00         │  │  ← primary CTA
│  └────────────────────────────────────┘  │
│  ┌────────────────────────────────────┐  │
│  │     Subscribe & Save               │  │  ← secondary CTA
│  └────────────────────────────────────┘  │
│                                          │
│  Set as my bean                          │  ← text link
│                                          │
│                88pt tab bar              │
└──────────────────────────────────────────┘
```

### Bean Detail Specs

| Property | Value |
|---|---|
| Background | `#F5F0E8` |
| Image area | Full width, 280pt height, background: `bean.imageBgTint` |
| Content padding | 20pt horizontal |
| Back button | "‹ Beans" — system back, `#1A1714` |

#### Tasting Note Pills

| Property | Value |
|---|---|
| Background | `#FFFFFF` |
| Border | 1px `#E5DDD3` |
| Corner radius | 8px |
| Height | 32pt |
| Padding | 8pt horizontal |
| Icon | 16pt emoji or SF Symbol |
| Label | `t_caption` (13px), `#1A1714` |
| Gap between pills | 8pt |
| Layout | Horizontal flow, wraps |

#### Flavor Profile Sliders (read-only)

| Property | Value |
|---|---|
| Track width | Full width minus padding |
| Track height | 4pt |
| Track bg | `#E5DDD3` |
| Track fill | Gradient from `#D4A96A` to `#5B8C5A` |
| Dot | 12pt circle, `#1A1714`, positioned at score value |
| Label left | Slider name — `t_small` (14px), `#8C8279` |
| Label right | Level word — `t_small` (14px), `#1A1714` |
| Row height | 40pt |
| Row gap | 8pt |

**Slider level words:**
- 0–0.2: "Low"
- 0.21–0.4: "Medium-Low"
- 0.41–0.6: "Medium"
- 0.61–0.8: "High"
- 0.81–1.0: "Very High"

#### Origin Section

| Property | Value |
|---|---|
| Key labels | `t_small` (14px), `#8C8279` |
| Key values | `t_small` (14px), `#1A1714` |
| Origin story | `t_body` (16px regular), `#1A1714`, italic, 12pt top margin |

#### "Best For" Profile Chips

| Property | Value |
|---|---|
| Background | `#1A1714` |
| Text | `t_caption` (13px), `#F5F0E8` |
| Corner radius | 8px |
| Height | 28pt |
| Padding | 10pt horizontal |
| Gap | 8pt |

#### Add to Cart / Subscribe & Save Buttons

**Add to Cart (price available):**

| Property | Value |
|---|---|
| Background | `#1A1714` |
| Text | "Add to Cart — ${price}" — `Font.system(size: 17, weight: .bold)`, `#F5F0E8` |
| Height | 54pt |
| Corner radius | 14px |
| Action | Opens `SFSafariViewController` with `bean.shopifyUrl` |

**Add to Cart (price TBD):**

| Property | Value |
|---|---|
| Background | `#E5DDD3` |
| Text | "NOTIFY ME" — `Font.system(size: 17, weight: .bold)`, `#8C8279` |
| Action | Saves `beanName` to `UserDefaults` array `notifyMeBeans`, shows toast |
| Toast | "We'll let you know when {beanName} drops." — 2s duration |

**Subscribe & Save:**

| Property | Value |
|---|---|
| Background | `transparent` |
| Border | 1.5px `#1A1714` |
| Text | "Subscribe & Save" — `Font.system(size: 17, weight: .semibold)`, `#1A1714` |
| Height | 54pt |
| Corner radius | 14px |
| Only shown when | `bean.price != nil` |
| Action | Opens Shopify subscribe URL in `SFSafariViewController` |

**Subscribed state:**

| Property | Value |
|---|---|
| Background | `#5B8C5A` |
| Text | "SUBSCRIBED ✓" — bold, `#FFFFFF` |
| State stored in | `UserDefaults.stringArray(forKey: "subscribedBeans")` |

**"Set as my bean":**

| Property | Value |
|---|---|
| Style | Text link — `t_small` (14px), `#8C8279`, underline |
| Top margin | 12pt below buttons |
| Action | Saves `bean.name` to `UserDefaults` `preferredBeanName`, shows toast |
| Toast | "{beanName} set as your bean." — 2s |
| After set | Link text changes to "✓ Your bean" — `#5B8C5A`, no underline |

---

## 5. History Tab

Dark background. Data-forward. Shows brew log and emerging patterns.

### History — Main Screen

```
┌──────────────────────────────────────────┐
│                                          │
│  History                                 │  ← t_h1, white
│                                          │
│  📊 3 brews logged. Patterns emerging.   │  ← ⟐ R2: insight line (3+ check-ins)
│                                          │
│ ┌──────────────────────────────────────┐ │
│ │  💡 PATTERN                          │ │  ← pattern card (5+ check-ins)
│ │                                      │ │
│ │  You rate First Light highest on     │ │
│ │  65–75% recovery days.               │ │
│ │                                      │ │
│ └──────────────────────────────────────┘ │
│                                          │
│  MARCH 2026                              │  ← month header
│                                          │
│ ┌──────────────────────────────────────┐ │
│ │  Today · 12:14 PM                    │ │
│ │  FIRST LIGHT          60g · ~120mg   │ │
│ │  Recovery: 68%         👍 Right       │ │
│ └──────────────────────────────────────┘ │
│                                          │
│ ┌──────────────────────────────────────┐ │
│ │  Yesterday · 7:45 AM                 │ │
│ │  VOLTAGE              90g · ~180mg   │ │
│ │  Recovery: 88%         👍 Right       │ │
│ └──────────────────────────────────────┘ │
│                                          │
│ ┌──────────────────────────────────────┐ │
│ │  Mar 18 · 8:02 AM                   │ │
│ │  DEEP WORK            75g · ~150mg   │ │
│ │  Recovery: 76%         ➖ Too much    │ │
│ └──────────────────────────────────────┘ │
│                                          │
│ ┌──────────────────────────────────────┐ │
│ │  Mar 17 · 9:30 AM                   │ │
│ │  TIDE POOL            40g · ~80mg    │ │
│ │  Recovery: 42%         ➕ Needed more │ │
│ └──────────────────────────────────────┘ │
│                                          │
│                88pt tab bar              │
└──────────────────────────────────────────┘
```

### History Specs

| Property | Value |
|---|---|
| Background | `#000000` |
| Title | "History" — `t_h1`, `#FFFFFF` |
| Empty state copy | "No brews yet. Your pattern shows up after the first few mornings." — `t_body`, `#888`, centered |

**⟐ ROUND 2 — Insight Line (replaces empty space before patterns are ready):**

| Property | Value |
|---|---|
| Appears at | 3+ check-in ratings (non-skip) and < 5 check-ins |
| Copy | "📊 {count} brews logged. Patterns emerging." |
| Style | `t_small` (14px), `#888888`, left-aligned, 20pt padding |
| Position | Below title, above month headers |
| Disappears when | Pattern card appears (at 5+ check-ins), replaced by pattern card |

**Progressive states:**
- 0 brews: empty state copy (centered)
- 1–2 brews with check-ins: no insight line (too early)
- 3–4 brews with check-ins: "📊 {count} brews logged. Patterns emerging."
- 5+ brews with check-ins: insight line replaced by full pattern card

#### Pattern Card

| Property | Value |
|---|---|
| Background | `#0F0F0F` |
| Left border | 3px `#488BED` |
| Corner radius | 16px |
| Padding | 16pt |
| Eyebrow | "💡 PATTERN" — `t_eyebrow`, `#488BED` |
| Copy | `t_body` (16px), `#FFFFFF` |
| Appears after | 5+ brews with check-in data (non-skip) |
| Max 1 pattern card shown at top |

**Pattern copy variants** (generated from check-in data):
1. "You rate {profile} highest on {recovery_range}% recovery days."
2. "Your best brews happen before {avg_brew_time}."
3. "{bean_name} gets your best check-ins. Worth stocking up."
4. "You tend to need more caffeine on {weekday}s."

#### Brew Log Entry

| Property | Value |
|---|---|
| Background | `#0F0F0F` |
| Corner radius | 12px |
| Padding | 16pt |
| Vertical gap | 8pt between entries |
| Date line | "Today · {time}" or "Mar {day} · {time}" — `t_caption` (13px), `#888888` |
| Profile name | `t_bodyBold` (16px semibold), `#FFFFFF`, all-caps |
| Brew params | "{dose}g · ~{caffeine}mg" — `t_mono` (13px), `#888888`, right-aligned on same line as profile |
| Recovery line | "Recovery: {pct}%" — `t_caption` (13px), recovery color |
| Check-in | Check-in pill text — `t_caption` (13px), `#888888`, right-aligned on same line as recovery |

**Check-in display in log:**
- "right" → "👍 Right"
- "needed_more" → "➕ Needed more"
- "too_much" → "➖ Too much"
- "skipped" or nil → no check-in shown

**Per-bean history** (shown after 3+ brews with same bean set):

| Property | Value |
|---|---|
| Section header | "{bean_name} · {count} brews" — `t_h3`, `#D4A96A` |
| Shows | Filtered log entries for that bean |
| Summary line | "Best day: recovery {pct}%. Average dose: {avg}g." — `t_small`, `#888` |

---

## 6. Settings Tab

Minimal. Functional. Dark.

```
┌──────────────────────────────────────────┐
│                                          │
│  Settings                                │  ← t_h1, white
│                                          │
│  WEARABLE                                │  ← section eyebrow
│ ┌──────────────────────────────────────┐ │
│ │  WHOOP            Connected ✓   ›    │ │  ← or "Connect >"
│ │  Oura Ring        Not connected ›    │ │
│ └──────────────────────────────────────┘ │
│                                          │
│  BREW PREFERENCES                        │
│ ┌──────────────────────────────────────┐ │
│ │  Wake time              6:00 AM  ›   │ │
│ │  Caffeine cutoff        2:00 PM  ›   │ │
│ │  Show check-in card       [ON]       │ │
│ └──────────────────────────────────────┘ │
│                                          │
│  ABOUT                                   │
│ ┌──────────────────────────────────────┐ │
│ │  Version                     0.1.0   │ │
│ │  Built by Brezi                      │ │
│ └──────────────────────────────────────┘ │
│                                          │
│  Flying manual right now. That's         │  ← shown if no wearable
│  fine. Connect when you're ready.        │  ← t_small, #888, centered
│                                          │
│                88pt tab bar              │
└──────────────────────────────────────────┘
```

### Settings Specs

| Property | Value |
|---|---|
| Background | `#000000` |
| Title | "Settings" — `t_h1`, `#FFFFFF` |
| Section eyebrow | `t_eyebrow`, `#888888`, letter-spacing 2pt, 24pt top margin |
| Section card | Background `#0F0F0F`, corner radius 16px |
| Row height | 48pt |
| Row padding | 16pt horizontal |
| Row label | `t_body` (16px), `#FFFFFF` |
| Row value | `t_body` (16px), `#888888` |
| Row chevron | `chevron.right`, 12pt, `#666666` |
| Row divider | 1px `#1A1A1A`, inset 16pt leading |

#### Wearable Section

| Row | Default State | Connected State |
|---|---|---|
| WHOOP | "Connect >" `#488BED` | "Connected ✓" `#34C759` |
| Oura Ring | "Connect >" `#488BED` | "Connected ✓" `#34C759` |

Tap → Initiates OAuth flow. Success → stores token, refreshes recovery data on next Home visit.

#### Brew Preferences

| Setting | Default | Picker |
|---|---|---|
| Wake time | 6:00 AM | Time picker (wheel), 15-min increments, 4:00–10:00 AM |
| Caffeine cutoff | 2:00 PM | Time picker (wheel), 15-min increments, 12:00–6:00 PM |
| Show check-in card | ON | Toggle switch, `#488BED` tint |

**Wake time** affects brew window bar start. **Caffeine cutoff** affects brew window bar end and "past your window" state.

#### Empty Wearable State Copy

"Flying manual right now. That's fine. Connect when you're ready." — `t_small` (14px), `#888888`, centered, 16pt below wearable section.

---

## 7. Complete Copy Strings Index

Every user-facing string in the app, organized by screen.

### Navigation / Chrome

| Key | Copy |
|---|---|
| `nav_title` | "BREZI" |
| `tab_home` | "Home" |
| `tab_beans` | "Beans" |
| `tab_history` | "History" |
| `tab_settings` | "Settings" |

### Onboarding

| Key | Copy |
|---|---|
| `onboard_skip` | "Skip" |
| `onboard_1_headline` | "Cold is a practice." |
| `onboard_1_body` | "Most people compromise their coffee. Ice dilutes it. Hot brew oxidizes it. You've been drinking the workaround this whole time." |
| `onboard_1_cta` | "Start the practice →" |
| `onboard_2_headline` | "12 minutes. No ice. No shortcuts." |
| `onboard_2_body` | "Brezi brews at cold temperature — not chilled-after-the-fact. The difference isn't subtle." |
| `onboard_2_accent` | "Your machine. Your data. Your brew." |
| `onboard_2_cta` | "Makes sense →" |
| `onboard_3_headline` | "Your coffee should know how you slept." |
| `onboard_3_body` | "Connect WHOOP or Oura and Brezi reads your recovery each morning. Or just tell us how you feel." |
| `onboard_3_cta_whoop` | "Connect WHOOP" |
| `onboard_3_cta_oura` | "Connect Oura" |
| `onboard_3_cta_manual` | "I'll log manually →" |
| `onboard_3_connected` | "Connected ✓" |
| `onboard_3_error` | "Connection failed. Try again." |
| `onboard_4_headline` | "How are you waking up today?" |
| `onboard_4_subhead` | "No wrong answers. Brezi works with whatever you've got." |
| `onboard_4_peaked` | "Peaked" |
| `onboard_4_peaked_sub` | "HRV high, slept clean, ready" |
| `onboard_4_functional` | "Functional" |
| `onboard_4_functional_sub` | "Decent night, a little behind" |
| `onboard_4_rebuilding` | "Rebuilding" |
| `onboard_4_rebuilding_sub` | "Wrecked it yesterday, paying now" |
| `onboard_4_cta` | "Build my first brew →" |
| `onboard_loading` | "Reading your signal..." |

### Recovery Card

| Key | Copy |
|---|---|
| `recovery_eyebrow` | "RECOVERY" |
| `recovery_data` | "HRV {hrv}ms · {hours}h {min}m sleep" |
| `recovery_manual_headline` | "How are you waking up?" |
| `recovery_manual_slide` | "Or slide:" |
| `recovery_chip_high` | "🟢 High" |
| `recovery_chip_functional` | "🟡 Functional" |
| `recovery_chip_rest` | "🔴 Rest" |

### Why Today

| Key | Copy |
|---|---|
| `why_peaked_1` | "Your system is primed. Use this window. It won't last forever." |
| `why_peaked_2` | "Peak recovery. This is what discipline earns you." |
| `why_peaked_3` | "HRV is up, sleep was clean. Go hard today — you've got the runway." |
| `why_functional_1` | "Solid night. Not your peak, but your body showed up. Meet it where it is." |
| `why_functional_2` | "Middle of the range. Consistent mornings compound — this one counts." |
| `why_functional_3` | "Not your best, not your worst. The kind of day that benefits from a clean start." |
| `why_rebuilding_1` | "Rough night. Your body's rebuilding — don't fight it, work with it." |
| `why_rebuilding_2` | "Low signal today. Gentle start. You'll feel the difference by afternoon." |
| `why_rebuilding_3` | "Recovery's down. Pull back the dose and let your system catch up." |
| `why_first_time` | "Welcome. Your first brew is about to set the baseline. Pay attention to how it lands." |

### Brew Card

| Key | Copy |
|---|---|
| `brew_eyebrow` | "TODAY'S BREW" |
| `brew_caffeine_label` | "Estimated caffeine" |
| `brew_caffeine_value` | "~{caffeine}mg" |
| `brew_window_label` | "Best before {time}" |
| `brew_window_past` | "Past your brew window" |
| `brew_cta` | "START BREW" |
| `brew_cta_past` | "BREW ANYWAY" |
| `brew_cta_past_sub_1` | "Past your window. Sleep-safe by {time}." |
| `brew_cta_past_sub_2` | "Still your call." |
| `brew_advanced` | "Advanced" |
| `brew_advanced_footer` | "These are defaults for {profile}. Adjust if you know your setup." |
| `brew_grind` | "Grind" |
| `brew_temp` | "Temp" |
| `brew_ratio` | "Ratio" |

### Profile Names + Taglines

| Key | Copy |
|---|---|
| `profile_tide_pool` | "Tide Pool" |
| `profile_tide_pool_tag_low` | "Recovery mode. Smooth and low." |
| `profile_tide_pool_tag_mid` | "Your body's rebuilding. Go gentle." |
| `profile_first_light` | "First Light" |
| `profile_first_light_tag_low` | "Your everyday standard." |
| `profile_first_light_tag_high` | "Solid ground. Clean start." |
| `profile_deep_work` | "Deep Work" |
| `profile_deep_work_tag` | "Focused extraction. Sustained output." |
| `profile_voltage` | "Voltage" |
| `profile_voltage_tag_low` | "System primed. Full send." |
| `profile_voltage_tag_high` | "Peak day. Don't waste this." |

### Strength Labels

| Key | Copy |
|---|---|
| `strength_light` | "(light)" |
| `strength_mild` | "(mild)" |
| `strength_medium` | "(medium strength)" |
| `strength_strong` | "(strong)" |
| `strength_very_strong` | "(very strong)" |
| `strength_maximum` | "(maximum)" |

### Brew Timer

| Key | Copy |
|---|---|
| `timer_eyebrow` | "BREWING ·" |
| `timer_caffeine` | "~{caffeine}mg caffeine incoming" |
| `timer_end` | "END BREW" |
| `timer_helper` | "You can close the app. We'll keep counting." |

### Caffeine Countdown

| Key | Copy |
|---|---|
| `caffeine_active` | "⚡ CAFFEINE ACTIVE" |
| `caffeine_peak_label` | "⚡ CAFFEINE PEAK" |
| `caffeine_clearing` | "⚡ CAFFEINE CLEARING" |
| `caffeine_cleared` | "✓ CAFFEINE CLEARED" |
| `caffeine_peak_in` | "Peak in {min} min" |
| `caffeine_at_peak` | "You're at peak right now" |
| `caffeine_half_life` | "½ life at {time}" |
| `caffeine_half_passed` | "½ life passed" |
| `caffeine_sleep_safe` | "🌙 Sleep-safe by {time}" |
| `caffeine_cleared_msg` | "You're clear. Recovery window open." |
| `caffeine_brew_summary` | "{profile} · {dose}g · brewed {time}" |

### Check-In

| Key | Copy |
|---|---|
| `checkin_question` | "How did today's brew land?" |
| `checkin_right` | "👍 Right" |
| `checkin_needed_more` | "➕ Needed more" |
| `checkin_too_much` | "➖ Too much" |
| `checkin_skip` | "Skip" |
| `checkin_pattern_teaser` | "Noted. A few more and I'll start seeing patterns." |

### Evening / Cleared

| Key | Copy |
|---|---|
| `evening_cleared` | "Caffeine cleared." |
| `evening_recovery` | "Recovery window open." |
| `evening_sleep` | "Sleep well tonight." |
| `evening_no_brew` | "No brew today." |
| `evening_no_brew_sub` | "Your recovery window starts now." |
| `evening_no_brew_rest` | "Rest days count too." |

### Beans Tab

| Key | Copy |
|---|---|
| `beans_title` | "Beans" |
| `beans_for_you` | "FOR YOU TODAY" |
| `beans_browse` | "BROWSE ALL" |
| `beans_price_tbd` | "Price TBD" |
| `beans_notify_me` | "NOTIFY ME" |
| `beans_notify_toast` | "We'll let you know when {bean} drops." |
| `beans_add_to_cart` | "Add to Cart — ${price}" |
| `beans_subscribe` | "Subscribe & Save" |
| `beans_subscribed` | "SUBSCRIBED ✓" |
| `beans_set_as_mine` | "Set as my bean" |
| `beans_set_confirm` | "{bean} set as your bean." |
| `beans_set_done` | "✓ Your bean" |
| `beans_tasting_notes` | "TASTING NOTES" |
| `beans_flavor_profile` | "FLAVOR PROFILE" |
| `beans_origin` | "ORIGIN" |
| `beans_best_for` | "BEST FOR" |

### History

| Key | Copy |
|---|---|
| `history_title` | "History" |
| `history_pattern` | "💡 PATTERN" |
| `history_insight_line` | "📊 {count} brews logged. Patterns emerging." |
| `history_empty` | "No brews yet. Your pattern shows up after the first few mornings." |

### Settings

| Key | Copy |
|---|---|
| `settings_title` | "Settings" |
| `settings_wearable` | "WEARABLE" |
| `settings_whoop` | "WHOOP" |
| `settings_oura` | "Oura Ring" |
| `settings_connected` | "Connected ✓" |
| `settings_not_connected` | "Not connected" |
| `settings_connect` | "Connect >" |
| `settings_prefs` | "BREW PREFERENCES" |
| `settings_wake` | "Wake time" |
| `settings_cutoff` | "Caffeine cutoff" |
| `settings_checkin_toggle` | "Show check-in card" |
| `settings_about` | "ABOUT" |
| `settings_version` | "Version" |
| `settings_built_by` | "Built by Brezi" |
| `settings_manual_msg` | "Flying manual right now. That's fine. Connect when you're ready." |

### Error / Edge States

| Key | Copy |
|---|---|
| `error_wearable_fail` | "Connection failed. Try again." |
| `error_generic` | "Something broke. Try again." |
| `empty_first_checkin` | "Your practice starts blank. Give it three mornings." |

### "For You" Section Copy Per Profile

| Key | Copy |
|---|---|
| `beans_for_you_tide_pool` | "Gentle beans for a recovery day" |
| `beans_for_you_first_light` | "Based on your First Light profile" |
| `beans_for_you_deep_work` | "Dense beans for focused work" |
| `beans_for_you_voltage` | "High-intensity beans for peak days" |

---

## 8. Animation Spec — Complete

### Dose Tap Feedback (Single Tap)

```swift
// Trigger: [−] or [+] tapped once
// 1. Dose number scales
withAnimation(.spring(response: 0.2, dampingFraction: 0.6)) {
    doseScale = 1.15
}
DispatchQueue.main.asyncAfter(deadline: .now() + 0.1) {
    withAnimation(.spring(response: 0.2, dampingFraction: 0.6)) {
        doseScale = 1.0
    }
}
// 2. Caffeine value cross-fades
withAnimation(.easeInOut(duration: 0.15)) {
    caffeineValue = newValue
}
// 3. Strength label cross-fades if threshold crossed
if strengthLabelChanged {
    withAnimation(.easeInOut(duration: 0.2)) {
        strengthLabel = newLabel
    }
}
// 4. Haptic
UIImpactFeedbackGenerator(style: .light).impactOccurred()
```

### ⟐ R2 — Dose Long-Press Acceleration

```swift
// Trigger: hold [+] or [−] for > 0.5s
// 1. Start repeating timer
let accelerationTimer = Timer.publish(every: 0.2, on: .main, in: .common).autoconnect()
accelerationTimer.sink { _ in
    dose = max(minDose, min(maxDose, dose + (5 * direction)))

    // 2. Subtler continuous pulse (1.08 not 1.15)
    withAnimation(.spring(response: 0.15, dampingFraction: 0.7)) {
        doseScale = 1.08
    }
    DispatchQueue.main.asyncAfter(deadline: .now() + 0.08) {
        withAnimation(.spring(response: 0.15, dampingFraction: 0.7)) {
            doseScale = 1.0
        }
    }

    // 3. Haptic per tick
    UIImpactFeedbackGenerator(style: .light).impactOccurred()

    // 4. Caffeine + strength label update same as single tap
    caffeineValue = dose * 2.0
    strengthLabel = strengthLabel(for: dose)
}

// On touch-up:
accelerationTimer.cancel()
```

### ⟐ R2 — Dose Boundary Glow

```swift
// Trigger: dose reaches 30g (min) or 120g (max)
if dose == minDose || dose == maxDose {
    let glowColor = dose == minDose ? statusRed : blue
    withAnimation(.easeOut(duration: 0.15)) {
        doseColor = glowColor
    }
    DispatchQueue.main.asyncAfter(deadline: .now() + 0.15) {
        withAnimation(.easeIn(duration: 0.15)) {
            doseColor = white
        }
    }
}
```

### Timer Counting

```swift
// Brew timer counts up every second
// Display updates: MM:SS format
// Progress ring: continuous linear animation over recommended steep time
withAnimation(.linear(duration: recommendedSteepSeconds)) {
    ringProgress = 1.0
}
// At completion: ring pulses 3 times
withAnimation(.easeInOut(duration: 0.6).repeatCount(3)) {
    ringScale = 1.05
}
```

### Caffeine Card Fade-In

```swift
// Trigger: END BREW tapped
// 1. Timer screen fades out
withAnimation(.easeInOut(duration: 0.3)) {
    timerOpacity = 0
}
// 2. Tab bar slides back in
withAnimation(.easeOut(duration: 0.3).delay(0.2)) {
    tabBarOffset = 0
}
// 3. Caffeine card fades in from below
withAnimation(.easeOut(duration: 0.5).delay(0.3)) {
    caffeineCardOpacity = 1.0
    caffeineCardOffset = 0  // from 20pt below
}
// 4. Check-in card fades in 30 min later (scheduled)
DispatchQueue.main.asyncAfter(deadline: .now() + 1800) {
    withAnimation(.easeOut(duration: 0.4)) {
        checkInOpacity = 1.0
        checkInOffset = 0  // from 16pt below
    }
}
```

### Tab Bar Pill Slide

```swift
// Trigger: tab switch
withAnimation(.spring(response: 0.35, dampingFraction: 0.8)) {
    pillOffset = targetTabX
}
UIImpactFeedbackGenerator(style: .light).impactOccurred()
```

### Onboarding Screen Transitions

```swift
// Each screen entrance:
// Headline: fade + slide up
withAnimation(.easeOut(duration: 0.4).delay(0.1)) {
    headlineOpacity = 1.0
    headlineOffset = 0  // from 12pt below
}
// Body: fade in after headline
withAnimation(.easeOut(duration: 0.3).delay(0.5)) {
    bodyOpacity = 1.0
}
// Button: fade in last
withAnimation(.easeOut(duration: 0.2).delay(0.8)) {
    buttonOpacity = 1.0
}
```

### Card Dismiss (Check-In / Caffeine)

```swift
// On dismiss (X tap, skip, or rating selected):
withAnimation(.easeIn(duration: 0.3)) {
    cardOpacity = 0
    cardOffset = -20  // slides up
}
```

### Recovery Dot Fill

```swift
// On recovery data load:
ForEach(0..<10) { i in
    Circle()
        .fill(i < filledCount ? blue : faintGrey)
        .animation(.easeOut(duration: 0.15).delay(Double(i) * 0.05), value: filledCount)
}
// Dots fill left-to-right with staggered delay
```

### Brew Window Bar NOW Marker

```swift
// NOW marker position updates every minute
withAnimation(.easeInOut(duration: 0.5)) {
    nowPosition = calculatePosition(for: Date())
}
```

### Advanced Toggle Expand/Collapse

```swift
withAnimation(.easeInOut(duration: 0.25)) {
    isAdvancedExpanded.toggle()
}
// Chevron rotates 90° in sync
```

### ⟐ R2 — Pattern Teaser Toast

```swift
// Trigger: 3rd non-skip check-in, hasShownPatternTeaser == false
withAnimation(.easeOut(duration: 0.3)) {
    toastOpacity = 1.0
    toastOffset = 0  // from 12pt below
}
DispatchQueue.main.asyncAfter(deadline: .now() + 2.5) {
    withAnimation(.easeIn(duration: 0.3)) {
        toastOpacity = 0
        toastOffset = -8  // slides up slightly
    }
}
UserDefaults.standard.set(true, forKey: "hasShownPatternTeaser")
```

---

## 9. Data Model Summary

### UserDefaults Keys

| Key | Type | Default | Purpose |
|---|---|---|---|
| `hasCompletedOnboarding` | Bool | false | Gate onboarding flow |
| `inputMode` | String | "manual" | "manual" / "whoop" / "oura" |
| `wakeTime` | Date | 6:00 AM | Brew window start |
| `caffeineCutoff` | Date | 2:00 PM | Brew window end |
| `showCheckIn` | Bool | true | Check-in card toggle |
| `recentWhyTodayCopy` | [String] | [] | Last 3 Why Today keys (no-repeat) |
| `preferredBeanName` | String? | nil | "Set as my bean" selection |
| `notifyMeBeans` | [String] | [] | Beans user wants price alerts for |
| `subscribedBeans` | [String] | [] | Beans with active subscriptions |
| `brewStartTime` | Date? | nil | Active brew start (persists across app close) |
| `lastBrewEndTime` | Date? | nil | For caffeine countdown |
| `lastBrewDose` | Double? | nil | For caffeine countdown |
| `lastBrewProfile` | String? | nil | For caffeine countdown |
| `lastBrewCaffeine` | Double? | nil | For caffeine countdown |
| `hasShownPatternTeaser` | Bool | false | ⟐ R2: One-time toast after 3rd check-in |
| `nonSkipCheckInCount` | Int | 0 | ⟐ R2: Track check-ins for insight line + toast |

### Core Data Entities

**BrewLog:**

| Field | Type | Notes |
|---|---|---|
| `id` | UUID | Primary key |
| `date` | Date | Brew date/time |
| `profileKey` | String | "tide_pool" / "first_light" / "deep_work" / "voltage" |
| `recoveryScore` | Int16 | 0–100 |
| `dose` | Double | Grams |
| `caffeine` | Double | Milligrams |
| `grind` | String | "coarse" / "medium-coarse" / "medium" / "medium-fine" |
| `temp` | String | "Cold (4°C)" |
| `ratio` | String | "1:12" through "1:16" |
| `brewDuration` | Double | Seconds |
| `checkInRating` | String? | "right" / "needed_more" / "too_much" / "skipped" / nil |
| `selectedBeanName` | String? | Bean used (if set) |

### Profile → Recovery Mapping

```swift
func profileKey(for recoveryPercent: Int) -> String {
    switch recoveryPercent {
    case 0...49:   return "tide_pool"
    case 50...74:  return "first_light"
    case 75...84:  return "deep_work"
    case 85...100: return "voltage"
    default:       return "first_light"
    }
}
```

### ⟐ R2 — Caffeine Time Calculations (Corrected)

```swift
func caffeineTimestamps(brewEnd: Date) -> (peak: Date, halfLife: Date, sleepSafe: Date) {
    let peak      = brewEnd.addingTimeInterval(45 * 60)      // +45 min
    let halfLife   = brewEnd.addingTimeInterval(5 * 3600)     // +5h
    let sleepSafe  = brewEnd.addingTimeInterval(15 * 3600)    // +15h ← ROUND 2 FIX
    return (peak, halfLife, sleepSafe)
}

// At 15h: remaining = initial × 0.5^(15/5) = initial × 0.125 = ~12.5% remaining
// For 120mg initial → ~15mg at sleep-safe. Genuinely below HRV-impact threshold.
```

---

## 10. Screen Navigation Map

```
AppRoot
 ├── OnboardingFlow (4 screens, sequential push, shown once)
 │    ├── Screen 1: The Hook
 │    ├── Screen 2: The Why
 │    ├── Screen 3: The Data Ask (wearable connect)
 │    └── Screen 4: First Recovery Input → "Reading your signal..." → Home
 │
 └── TabView (4 tabs, persistent post-onboarding)
      │
      ├── 🏠 Home (single view, state-driven content)
      │    ├── State: Pre-brew (Recovery + Why Today + Brew Card)
      │    ├── State: Brewing (Full-screen timer, tab bar hidden)
      │    ├── State: Post-brew (Caffeine countdown + Check-in)
      │    └── State: Evening (Cleared message)
      │
      ├── ☕ Beans (NavigationStack)
      │    ├── Bean list (For You + Browse All)
      │    └── Bean detail (push)
      │         └── SFSafariViewController (Add to Cart / Subscribe)
      │
      ├── 📊 History (single view, scrollable)
      │    ├── Insight line (3-4 check-ins) ← ROUND 2
      │    ├── Pattern card (5+ check-ins)
      │    └── Brew log entries
      │
      └── ⚙️ Settings (single view)
           ├── Wearable connect (OAuth flow)
           ├── Wake time picker (inline)
           └── Caffeine cutoff picker (inline)
```

**Hard rule validation:** Open → Home (1 tap) → START BREW (1 tap) = **2 taps from open to START BREW**. Under the 3-tap maximum.

---

## 11. Responsive Layout Notes

### iPhone SE (375pt width)

- Dose control: [−] and [+] buttons may overlap dose number at large values → enforce 120g max display width test
- Bean cards in 2-col grid: 165pt per card → reduce image area to 90pt
- Tab bar pill: shrink to 56×32pt
- Brew timer: reduce to 48px font

### iPhone 15 Pro Max (430pt width)

- Use extra width for breathing room, not larger elements
- Horizontal padding stays 20pt — content area expands
- Bean detail image area: full 280pt

### Dynamic Type

- Support up to `.accessibility3`
- At large sizes: stack dose control vertically (dose number above, [−][+] below)
- Tab labels hide at `.accessibility1` and above — icons only
- Brew card tagline wraps to 2 lines max

---

## 12. Hard Rules Checklist (from Locked Architecture)

| Rule | Implementation | Verified |
|---|---|---|
| Max 3 taps from open to START BREW | Open → Home (auto) → START BREW = 2 taps | ✓ |
| No temp on main brew card | Temp only in Advanced (collapsed by default) | ✓ |
| No bean selection as required step | Brew card works without touching Beans tab | ✓ |
| No g/L jargon without translation | Dose shows "60g" + "(medium strength)" always | ✓ |
| No clutter — wellness users will abandon | 3 cards pre-brew, 2 cards post-brew, 1 message evening | ✓ |
| Sleep-safe earns trust with wearable users | `brew_end + 15h` → ~12.5% caffeine remaining | ✓ R2 |
| Dose control is effortless at any range | Long-press acceleration + boundary glow | ✓ R2 |
| App signals it's learning before patterns appear | Insight line at 3 check-ins + toast | ✓ R2 |
| Past-window brew shows consequence | Sleep-safe time projected inline | ✓ R2 |

---

## 13. Round 2 Changes — Inline Reference

For engineering handoff clarity, every Round 2 change is marked inline with **⟐ R2** or **⟐ ROUND 2** wherever it appears in this spec. Search for these markers to find all changes from Round 1.

**Changes summary (7 total):**

1. **Section 2C / Caffeine Card** — Sleep-safe = `brew_end + 15h` (was `brew_end + 10h`)
2. **Section 2A / Dose Control** — Long-press acceleration (hold > 0.5s, 5g per 0.2s)
3. **Section 2A / Dose Control** — Boundary glow at min/max (red flash at 30g, blue flash at 120g)
4. **Section 5 / History** — Insight line at 3–4 check-ins: "📊 {count} brews logged. Patterns emerging."
5. **Section 2C / Check-In** — One-time toast after 3rd check-in: "Noted. A few more and I'll start seeing patterns."
6. **Section 2A / START BREW past-window** — Subtext shows projected sleep-safe time
7. **Section 9 / Data Model** — Sleep-safe formula corrected everywhere, stale `brew_start + 25h` killed

**What didn't change (and why):**
- Pattern card stays at 5 check-ins (3 data points isn't a pattern for WHOOP-trained users)
- Onboarding doesn't mention hardware (App Store listing handles this)
- Recovery dots unchanged (giant % number above eliminates ambiguity)
- Evening state stays minimal (restraint is the brand; no reliable real-time wearable data to show)

---

*Round 2 spec written March 20, 2026. All Round 1 feedback resolved. Every ⟐ R2 change integrated inline. Architecture locked. Copy locked. Ship it.*