# Brezi iOS App — Emotional Layer Design
**Author:** Picasso (Product Design) | March 19, 2026 | Round 1 — Emotional UX
**For:** Jackson (Product) · Jensen (Engineering) · Elon (Growth/Copy)
**Status:** Design review. Additive to v3.0 spec — no existing screens replaced, new layers added.

---

## 0. Design Philosophy

The v3.0 spec builds a machine. This document gives it a nervous system.

The gap: health data produces a correct recommendation. But correct isn't felt. A user who sees "First Light — 4-8°C, 12min, 60g/L" knows what to brew. They don't know why it matters that they brew *this*, *today*, *in this body*. The emotional layer closes that gap.

Three principles:
1. **Coffee is a response, not a product.** The recommendation answers the body's question.
2. **Flavor is emotional language.** "Bergamot and jasmine" means ease. "Blackcurrant and citrus zest" means ignition. Write flavor like you're describing how a room feels, not what's in it.
3. **The ritual is the product.** Choosing, brewing, drinking, rating — each moment should feel like the user is being seen, not served.

---

## 1. Emotional Recommendation Card — `EmotionalBrewCardView.swift`

**Replaces:** The brew parameter block at the top of `DashboardBrewCardView.swift` (Card 3). Parameters move below the emotional layer — they're still there, just secondary.

**New Card 3 structure (pre-brew, wearable user example):**

```
┌─────────────────────────────────┐
│                                 │  bg: #0F0F0F, radius: 24pt (brewCard)
│  ┌─────────────────────────┐    │  EmotionalHeaderView.swift
│  │                         │    │  profile color gradient strip
│  │  ░░░░░░░░░░░░░░░░░░░░░  │    │  height: 3pt, radius: 1.5pt
│  └─────────────────────────┘    │  top padding: 24pt (lg)
│                                 │
│  FIRST LIGHT                    │  CondensedBold, 32px, #FFFFFF
│                                 │  bottom padding: 4pt (xs)
│  Not a recovery day. Not a      │  Regular, 16px, #CCCCCC
│  peak day. Just a morning       │  line-height: 1.5
│  that needs to begin            │
│  correctly.                     │  bottom padding: 20pt
│                                 │
│  ┌─────────────────────────┐    │  FlavorPreviewStrip.swift
│  │  This cup is:            │    │  Mono, 11px, #888888
│  │  Clean. Steady. Familiar.│    │  CondensedBold, 18px, #FFFFFF
│  └─────────────────────────┘    │  bottom padding: 20pt
│                                 │
│  ─────────────────────────────  │  divider: 1px #222222
│                                 │
│  ┌────┐  ┌────┐  ┌────┐        │  (parameters — same spec as v3.0)
│  │4–8°│  │ 12 │  │ 60 │        │  CondensedBold, 22px, #FFFFFF
│  │ C  │  │min │  │g/L │        │  Mono, 10px, #666666, ALL CAPS
│  │TEMP│  │TIME│  │DOSE│        │
│  └────┘  └────┘  └────┘        │  3 equal columns, 8pt gap
│                                 │
│  Medium roast · Medium grind    │  Mono, 13px, #888888
│  1 serve · Best before 2pm     │  Mono, 13px, #488BED
│                                 │  bottom padding: 24pt (lg)
│  ┌─────────────────────────┐    │
│  │  START BREW  →           │    │  white filled, #000000 text, 48pt
│  └─────────────────────────┘    │
│                                 │
│  See full details ›             │  Regular, 14px, #888888, 44×44pt
│                                 │
└─────────────────────────────────┘
```

### Profile Color Gradients — `ProfileGradient.swift`

Each profile has a signature gradient. Used in the 3pt strip at the top of the emotional card, in the flavor preview background tint, and in the "Why Today" transition screen.

```swift
let profileGradients: [String: [Color]] = [
    "tide_pool":   [Color(hex: "#2D6A4F"), Color(hex: "#52B788")],  // deep green → soft green
    "first_light": [Color(hex: "#488BED"), Color(hex: "#90B8F8")],  // blue → light blue
    "deep_work":   [Color(hex: "#B8860B"), Color(hex: "#DAA520")],  // dark gold → gold
    "voltage":     [Color(hex: "#FF453A"), Color(hex: "#FF6B6B")],  // red → warm red
    "the_blank":   [Color(hex: "#555555"), Color(hex: "#999999")]   // mid grey → light grey
]
```

- Gradient strip: 3pt height, full card width minus 48pt (24pt padding each side), radius 1.5pt
- Direction: leading to trailing (left → right)
- Subtle glow: `.shadow(color: gradient[0].opacity(0.3), radius: 8, x: 0, y: 2)` beneath strip

### Emotional Taglines — Complete Set

These replace the single-line taglines from v3.0 with multi-sentence felt descriptions. The tagline is the soul of the card. It answers: *why this, today, for you?*

```swift
let emotionalTaglines: [String: [String: String]] = [
    "tide_pool": [
        "high": "Everything's firing and you chose gentle. That's not holding back — that's knowing you don't need to prove anything today.",
        "mid":  "Your system's working, not surging. This is the cup that doesn't ask anything of you. Just warmth without volume.",
        "low":  "You're rebuilding. Your nervous system is doing the heavy lifting right now. Meet it where it is. Soft entry. No demands."
    ],
    "first_light": [
        "high": "Strong signal, and you're keeping it clean. First Light isn't conservative — it's the discipline of not needing more.",
        "mid":  "Not a recovery day. Not a peak day. Just a morning that needs to begin correctly.",
        "low":  "Low morning, standard brew. You're not pushing. You're just showing up. That's the practice."
    ],
    "deep_work": [
        "high": "Your body's ready for something sustained. Deep Work isn't about intensity — it's about the long middle. Three hours from now, you'll still feel this one.",
        "mid":  "Mid-range morning, and you're choosing focus over energy. Smart. This brew is built for the kind of thinking that doesn't want to be interrupted.",
        "low":  "Rough night, and you're going for it anyway. Deep Work on a low morning is a bet. Let's see if it pays."
    ],
    "voltage": [
        "high": "Everything's online. Voltage is the full send — concentrated, deliberate, built for a morning that can take it. Don't waste this one.",
        "mid":  "You're pushing past what the data says. Bold. This is the brew for people who trust their gut over their numbers.",
        "low":  "Low signal, full send. This is either exactly what you need or exactly what you don't. Rate it honest."
    ],
    "the_blank": [
        "high": "Full signal, custom brew. You already know what you want. Brezi's just the hands.",
        "mid":  "Mid recovery, your own recipe. You're running your own experiment today.",
        "low":  "Low recovery, custom call. Your body, your rules, your data."
    ]
]
```

**Selection logic:**
```swift
func emotionalTagline(profile: String, recoveryTier: String) -> String {
    return emotionalTaglines[profile]?[recoveryTier] ?? profileTaglines[profile] ?? ""
}
```

### "This cup is:" — Feeling Words

Three words that describe the sensory-emotional experience of the brew before the user drinks it. Not flavor notes. Feelings.

```swift
let cupFeelings: [String: [String: String]] = [
    "tide_pool": [
        "high": "Gentle. Sweet. Deliberate.",
        "mid":  "Soft. Grounding. Unhurried.",
        "low":  "Quiet. Warm. Asks nothing."
    ],
    "first_light": [
        "high": "Clean. Steady. Familiar.",
        "mid":  "Balanced. Clear. Reliable.",
        "low":  "Simple. Honest. Enough."
    ],
    "deep_work": [
        "high": "Structured. Rich. Sustaining.",
        "mid":  "Focused. Layered. Patient.",
        "low":  "Dense. Intentional. A gamble."
    ],
    "voltage": [
        "high": "Electric. Complex. Alive.",
        "mid":  "Bold. Bright. Unapologetic.",
        "low":  "Sharp. Aggressive. Your call."
    ],
    "the_blank": [
        "high": "Yours. Entirely. Yours.",
        "mid":  "Custom. Curious. Uncharted.",
        "low":  "Manual. Deliberate. Personal."
    ]
]
```

**Display spec:**
- Container: no background, inline within card
- Label: `"This cup is:"` — Mono, 11px, #888888
- Words: CondensedBold, 18px, #FFFFFF
- Bottom padding: 20pt before divider
- Animation on card appear: words fade in sequentially, 0.15s each, 0.1s delay between, easeOut

---

## 2. Bean Selection Layer — `BeanRecommendationView.swift`

### Architecture

Each brew profile maps to 2–3 recommended beans. Beans are not random — they're chosen because their sensory character matches the emotional state the profile addresses.

**New Core Data Entity:**

```swift
@objc(BeanProfile)
class BeanProfile: NSManagedObject {
    @NSManaged var id: UUID
    @NSManaged var name: String              // "Ethiopia Yirgacheffe"
    @NSManaged var origin: String            // "Ethiopia"
    @NSManaged var region: String            // "Yirgacheffe"
    @NSManaged var processing: String        // "Washed"
    @NSManaged var roastLevel: String        // "Light", "Medium", "Dark"
    @NSManaged var flavorFeeling: String     // "Soft, floral, asks nothing of you"
    @NSManaged var emotionalTag: String      // "For when you need ease"
    @NSManaged var pairingNote: String       // "Gentle extraction unlocks the florals..."
    @NSManaged var originStory: String       // 1-2 sentence origin story
    @NSManaged var sensoryCue: String        // "bergamot, jasmine, raw honey"
    @NSManaged var profileKeys: String       // comma-separated: "tide_pool,first_light"
    @NSManaged var sortOrder: Int16          // display order within profile
    @NSManaged var createdAt: Date
}
```

### Complete Bean Database — Seed Data

#### Tide Pool Beans (Recovery / Rest / Ease)

**Bean 1: Ethiopia Yirgacheffe — Washed**
```swift
let tp_bean1 = BeanSeed(
    name: "Ethiopia Yirgacheffe",
    origin: "Ethiopia",
    region: "Yirgacheffe",
    processing: "Washed",
    roastLevel: "Light",
    flavorFeeling: "Soft, floral, asks nothing of you. Like opening a window in a quiet room.",
    emotionalTag: "For when you need ease",
    pairingNote: "Gentle cold extraction at low dose unlocks the florals without any bite. This is what recovery tastes like.",
    originStory: "Yirgacheffe is where coffee began — not as a commodity, but as a cherry growing wild in Ethiopian highland forests. Every washed lot from here carries that original delicacy. It's the most unhurried coffee on earth.",
    sensoryCue: "bergamot, jasmine, raw honey",
    profileKeys: "tide_pool",
    sortOrder: 1
)
```

**Bean 2: Colombia Huila — Washed**
```swift
let tp_bean2 = BeanSeed(
    name: "Colombia Huila",
    origin: "Colombia",
    region: "Huila",
    processing: "Washed",
    roastLevel: "Medium",
    flavorFeeling: "Warm and round, like a hand on your shoulder. Nothing sharp. Nothing loud.",
    emotionalTag: "For mornings that need softness",
    pairingNote: "Huila's natural sweetness comes through cleanly at Tide Pool's low concentration. Comfort without heaviness.",
    originStory: "Huila sits in southern Colombia where volcanic soil and high altitude conspire to produce coffee that is impossibly balanced. Farmers here have been growing for generations. The patience shows.",
    sensoryCue: "panela, tangerine, milk chocolate",
    profileKeys: "tide_pool,first_light",
    sortOrder: 2
)
```

**Bean 3: Honduras Marcala — Washed**
```swift
let tp_bean3 = BeanSeed(
    name: "Honduras Marcala",
    origin: "Honduras",
    region: "Marcala",
    processing: "Washed",
    roastLevel: "Medium",
    flavorFeeling: "Brown sugar in warm water. Uncomplicated. The cup equivalent of a deep breath.",
    emotionalTag: "For rest days",
    pairingNote: "Marcala's simplicity is its strength at low extraction — naturally sweet, no rough edges, nothing to process.",
    originStory: "Marcala was Honduras's first designated origin — a small mountain town that proved Central American coffee could be gentle and exceptional at the same time. Quiet excellence.",
    sensoryCue: "brown sugar, mild stone fruit, velvet body",
    profileKeys: "tide_pool",
    sortOrder: 3
)
```

#### First Light Beans (Balance / Everyday / Steady)

**Bean 1: Guatemala Antigua — Washed**
```swift
let fl_bean1 = BeanSeed(
    name: "Guatemala Antigua",
    origin: "Guatemala",
    region: "Antigua",
    processing: "Washed",
    roastLevel: "Medium",
    flavorFeeling: "Structured and clean, like a well-made thing. Nothing missing. Nothing extra.",
    emotionalTag: "For the everyday standard",
    pairingNote: "Antigua's balance survives any extraction method. At First Light parameters, it's the definition of reliable — the cup you'd build a morning around.",
    originStory: "Antigua Valley sits between three volcanoes. The ash in the soil gives the coffee a mineral backbone that most origins can't touch. It's been Guatemala's benchmark for a century. Still is.",
    sensoryCue: "dark chocolate, walnut, clean finish",
    profileKeys: "first_light,deep_work",
    sortOrder: 1
)
```

**Bean 2: Brazil Cerrado — Natural**
```swift
let fl_bean2 = BeanSeed(
    name: "Brazil Cerrado",
    origin: "Brazil",
    region: "Cerrado Mineiro",
    processing: "Natural",
    roastLevel: "Medium",
    flavorFeeling: "Familiar and grounding. The taste of not needing to think about what you're drinking.",
    emotionalTag: "For mornings on autopilot",
    pairingNote: "Natural processing gives Cerrado a sweetness that makes First Light's moderate extraction feel indulgent without being heavy. Effortless quality.",
    originStory: "Cerrado Mineiro is Brazil's savanna coffee country — flat, hot, mechanized. What it lacks in romance it makes up in consistency. This is the backbone of the world's coffee supply, and the good lots are genuinely great.",
    sensoryCue: "toasted almond, caramel, cocoa nib",
    profileKeys: "first_light",
    sortOrder: 2
)
```

**Bean 3: Costa Rica Tarrazú — Honey**
```swift
let fl_bean3 = BeanSeed(
    name: "Costa Rica Tarrazú",
    origin: "Costa Rica",
    region: "Tarrazú",
    processing: "Honey",
    roastLevel: "Medium",
    flavorFeeling: "Bright enough to notice, smooth enough to forget. A small daily pleasure.",
    emotionalTag: "For when steady feels good",
    pairingNote: "Honey processing adds a layer of body that First Light's clean extraction makes transparent. You taste the craft without having to think about it.",
    originStory: "Tarrazú is Costa Rica's highest-altitude growing region, and the farmers there treat honey processing like fine art — partially removing the cherry mucilage to control sweetness at the molecular level. Obsessive. Worth it.",
    sensoryCue: "apricot, honey, toasted grain",
    profileKeys: "first_light",
    sortOrder: 3
)
```

#### Deep Work Beans (Focus / Sustained / Structured)

**Bean 1: Guatemala Antigua — Washed**
(Shared with First Light — see above. `profileKeys: "first_light,deep_work"`)

**Bean 2: Ethiopia Sidama — Natural**
```swift
let dw_bean2 = BeanSeed(
    name: "Ethiopia Sidama",
    origin: "Ethiopia",
    region: "Sidama",
    processing: "Natural",
    roastLevel: "Medium",
    flavorFeeling: "Layered and evolving. The cup that changes as it warms. Something new every sip.",
    emotionalTag: "For focus that doesn't flatten",
    pairingNote: "Deep Work's extended steep pulls complexity out of natural Sidama that shorter brews miss. The layers keep your palate engaged for hours — which is the point.",
    originStory: "Sidama is Ethiopia's most productive coffee zone but the natural lots — dried in the cherry under open sun — are anything but industrial. They're wild, fruity, polarizing. The good ones are transcendent.",
    sensoryCue: "blueberry, dark chocolate, wine finish",
    profileKeys: "deep_work",
    sortOrder: 2
)
```

**Bean 3: Sumatra Lintong — Wet-Hulled**
```swift
let dw_bean3 = BeanSeed(
    name: "Sumatra Lintong",
    origin: "Indonesia",
    region: "Lintong",
    processing: "Wet-Hulled",
    roastLevel: "Medium-Dark",
    flavorFeeling: "Heavy and anchoring, like a weighted blanket for your mouth. Stays with you.",
    emotionalTag: "For deep concentration",
    pairingNote: "Sumatra's earthy body at Deep Work's dose creates something dense and sustaining. Not a sipping coffee — a working coffee. It holds the room.",
    originStory: "Lintong sits on the edge of Lake Toba in northern Sumatra. Wet-hulling — the region's signature process — strips the parchment early and dries the green bean in humid air. It produces a cup that tastes like no other origin on earth. You either get it or you don't.",
    sensoryCue: "cedar, dark cocoa, tobacco, earthy sweetness",
    profileKeys: "deep_work",
    sortOrder: 3
)
```

#### Voltage Beans (Performance / Peak / Intensity)

**Bean 1: Kenya AA — Washed**
```swift
let v_bean1 = BeanSeed(
    name: "Kenya AA",
    origin: "Kenya",
    region: "Nyeri / Kirinyaga",
    processing: "Washed",
    roastLevel: "Medium",
    flavorFeeling: "Electric. Hits the front of your tongue and doesn't let go. The coffee equivalent of cold water on your face.",
    emotionalTag: "For when you mean business",
    pairingNote: "Kenya's famous phosphoric acidity at Voltage's high concentration is a controlled detonation. Bright, complex, demanding — the brew that matches your best morning.",
    originStory: "Kenya grades its coffee by bean size — AA is the largest, the densest, the most sought-after. Nyeri and Kirinyaga produce lots with an acidity so clean it borders on electric. This is specialty coffee's performance benchmark.",
    sensoryCue: "blackcurrant, grapefruit, sparkling finish",
    profileKeys: "voltage",
    sortOrder: 1
)
```

**Bean 2: Ethiopia Guji — Natural**
```swift
let v_bean2 = BeanSeed(
    name: "Ethiopia Guji",
    origin: "Ethiopia",
    region: "Guji",
    processing: "Natural",
    roastLevel: "Light-Medium",
    flavorFeeling: "Explosive and alive. Fruit-forward in a way that makes you pay attention. Not background noise.",
    emotionalTag: "For peak mornings",
    pairingNote: "Natural Guji at Voltage parameters is almost too much — in the best way. The fruit intensity matches the caffeine intensity. Full signal, full flavor.",
    originStory: "Guji split from Sidama as its own designated origin because the coffee was simply too different to share a name. Wild-grown, sun-dried, often processed by single families on raised beds. Every lot is a fingerprint.",
    sensoryCue: "strawberry, tropical fruit, dark honey, electric finish",
    profileKeys: "voltage",
    sortOrder: 2
)
```

**Bean 3: Rwanda Nyamasheke — Washed**
```swift
let v_bean3 = BeanSeed(
    name: "Rwanda Nyamasheke",
    origin: "Rwanda",
    region: "Nyamasheke",
    processing: "Washed",
    roastLevel: "Medium",
    flavorFeeling: "Bright and precise. Every note in its place. The kind of cup that makes you sit up straighter.",
    emotionalTag: "For mornings that deserve complexity",
    pairingNote: "Rwandan lots bring a juicy complexity that Voltage's parameters amplify rather than overwhelm. Concentration meets clarity.",
    originStory: "Rwanda's coffee industry was rebuilt from nothing after 1994. Twenty years later, Nyamasheke washing stations are producing some of East Africa's most refined lots. There's a precision to Rwandan coffee that feels earned.",
    sensoryCue: "red apple, lemon zest, black tea, crisp body",
    profileKeys: "voltage",
    sortOrder: 3
)
```

#### The Blank Beans (Manual / Experimental)

No default bean recommendations. Instead, show:

```swift
let theBlank_beanNote = "You're driving. Pick any bean you want — The Blank adapts to whatever you bring."
```

### Bean Card UI — `BeanCardView.swift`

Appears within the `BrewRecommendationView` (detail screen), below brew parameters, above "Why This Brew" chain.

```
│                                 │
│  RECOMMENDED BEANS              │  Mono, 11px, #888888, ALL CAPS
│                                 │  bottom padding: 12pt
│  ┌─────────────────────────────┐│  BeanCardView.swift
│  │                             ││
│  │  Ethiopia Yirgacheffe       ││  CondensedBold, 20px, #FFFFFF
│  │  Washed · Light             ││  Mono, 12px, #888888
│  │                             ││  bottom padding: 8pt (sm)
│  │  "Soft, floral, asks        ││  Regular, 15px, #CCCCCC
│  │   nothing of you. Like      ││  line-height: 1.5
│  │   opening a window in       ││  italic style
│  │   a quiet room."            ││
│  │                             ││  bottom padding: 8pt (sm)
│  │  bergamot · jasmine ·       ││  Mono, 11px, #666666
│  │  raw honey                  ││  (traditional flavor notes, subdued)
│  │                             ││  bottom padding: 12pt
│  │  For when you need ease     ││  Mono, 12px, #488BED
│  │                             ││
│  └─────────────────────────────┘│  bg: #1A1A1A, radius: 16pt
│  gap: 8pt (sm)                  │  padding: 20pt inner
│  ┌─────────────────────────────┐│
│  │  Colombia Huila             ││  BeanCardView.swift (card 2)
│  │  Washed · Medium            ││
│  │                             ││
│  │  "Warm and round, like a    ││
│  │   hand on your shoulder.    ││
│  │   Nothing sharp. Nothing    ││
│  │   loud."                    ││
│  │                             ││
│  │  panela · tangerine ·       ││
│  │  milk chocolate             ││
│  │                             ││
│  │  For mornings that          ││
│  │  need softness              ││
│  │                             ││
│  └─────────────────────────────┘│
│                                 │
│  Origin story ›                 │  Regular, 14px, #888888, 44×44pt
│                                 │  taps open OriginStorySheet
```

**BeanCardView.swift spec:**
- Background: `#1A1A1A`
- Radius: 16pt
- Padding: 20pt all sides
- Bean name: CondensedBold, 20px, #FFFFFF
- Processing + roast: Mono, 12px, #888888
- Flavor feeling: Regular, 15px, #CCCCCC, italic (`Font.system(size: 15, weight: .regular).italic()`)
- Sensory cue: Mono, 11px, #666666 — traditional tasting notes, deliberately subdued
- Emotional tag: Mono, 12px, #488BED
- First card in list has left border: 3px, profile gradient start color
- Subsequent cards: no border accent
- Max 3 cards shown. Horizontal swipe NOT used — vertical stack for scannability.

### "Origin story ›" — `OriginStorySheet.swift`

Half-sheet modal (`.presentationDetents([.medium])`). Shows the origin story for the currently top/selected bean.

```
┌─────────────────────────────────┐
│                                 │  bg: #0F0F0F
│  ─────                          │  sheet handle: #333333, centered
│                                 │
│  YIRGACHEFFE, ETHIOPIA          │  Mono, 11px, #888888, ALL CAPS
│                                 │  bottom padding: 12pt
│  Where coffee began.            │  CondensedBold, 28px, #FFFFFF
│                                 │  bottom padding: 16pt (md)
│  Yirgacheffe is where coffee    │  Regular, 16px, #CCCCCC
│  began — not as a commodity,    │  line-height: 1.6
│  but as a cherry growing wild   │
│  in Ethiopian highland forests. │
│  Every washed lot from here     │
│  carries that original          │
│  delicacy. It's the most        │
│  unhurried coffee on earth.     │
│                                 │  bottom padding: 24pt (lg)
│  ░░░░░░░░░░░░░░░░░░░░░░░░░░░   │  profile gradient strip (same as card)
│                                 │  3pt height, full width minus padding
│  Processing: Washed             │  Mono, 12px, #666666
│  Altitude: 1,700–2,200 masl    │  Mono, 12px, #666666
│  Harvest: Oct–Jan               │  Mono, 12px, #666666
│                                 │
└─────────────────────────────────┘
```

**Origin story headline — derived from origin story text.** First sentence fragment, or a crafted hook. Examples:

```swift
let originHeadlines: [String: String] = [
    "Ethiopia Yirgacheffe":  "Where coffee began.",
    "Colombia Huila":        "Volcanic patience.",
    "Honduras Marcala":      "Quiet excellence.",
    "Guatemala Antigua":     "Between three volcanoes.",
    "Brazil Cerrado":        "The backbone.",
    "Costa Rica Tarrazú":    "Obsessive craft.",
    "Ethiopia Sidama":       "Wild, fruity, polarizing.",
    "Sumatra Lintong":       "Like no other origin on earth.",
    "Kenya AA":              "Specialty's performance benchmark.",
    "Ethiopia Guji":         "Every lot is a fingerprint.",
    "Rwanda Nyamasheke":     "Precision that feels earned."
]
```

**Metadata block spec:**
- Mono, 12px, #666666
- 3 lines: Processing, Altitude, Harvest season
- Informational only — for the user who cares. Most won't scroll this far.

---

## 3. "Why Today" Transitional Screen — `WhyTodayView.swift`

### Purpose

Bridge between health data and brew recommendation. This is the moment where the app says: *I see you. Here's what that means. Here's what meets you there.*

Not a screen the user navigates to. It's a **transitional moment** — appears after check-in (or after wearable data refresh on first dashboard load), before the full dashboard renders. Only shows once per day, on the first dashboard load with fresh data.

### Trigger
- After onboarding Screen 5 → transition (replaces "Reading your signal..." screen)
- After morning check-in refresh (pull-to-refresh with new day's data, first time only)
- Store: `lastWhyTodayDate: String` in UserDefaults (format: "2026-03-19"). If today's date matches, skip.

### Layout

Full-screen overlay. Background: profile gradient (very dark, 0.08 opacity over #000000). Auto-advances after all text appears, OR tap anywhere to advance immediately.

```
┌─────────────────────────────────┐
│                                 │  bg: #000000 + gradient tint
│                                 │
│                                 │
│                                 │
│                                 │
│  You're at 61% today.           │  Phase 1: STATE
│  Rebuilding.                    │  Mono, 14px, #888888 (line 1)
│                                 │  CondensedBold, 28px, #FFFFFF (line 2)
│                                 │
│                                 │  [pause 1.2s]
│                                 │
│  Your nervous system is         │  Phase 2: NEED
│  doing the heavy lifting.       │  Regular, 17px, #CCCCCC
│  Give it ease.                  │  line-height: 1.5
│                                 │
│                                 │  [pause 1.0s]
│                                 │
│  This is what meets             │  Phase 3: BRIDGE
│  you there.                     │  Regular, 15px, #888888
│                                 │
│                  →              │  SF Symbol: arrow.right
│                                 │  #488BED, 16pt, pulse opacity 1.0↔0.5
│                                 │
│                                 │
│                                 │
└─────────────────────────────────┘
```

### Phase Animation Sequence

| Phase | Content | Animation | Duration | Delay |
|---|---|---|---|---|
| 1a | State line (e.g. "You're at 61% today.") | fadeIn | 0.3s easeOut | 0.2s |
| 1b | Tier word (e.g. "Rebuilding.") | fadeIn + translateY(8→0) | 0.3s easeOut | 0.6s |
| — | pause | — | — | 1.2s |
| 2 | Need text (e.g. "Your nervous system...") | fadeIn | 0.4s easeOut | 2.1s |
| — | pause | — | — | 1.0s |
| 3 | Bridge text + arrow | fadeIn | 0.3s easeOut | 3.5s |
| — | arrow pulse begins | opacity 1.0↔0.5, 1.2s, repeat | — | 3.8s |
| — | auto-advance to dashboard | crossfade | 0.5s easeOut | 5.5s |

**Total sequence: ~6 seconds.** Tap anywhere at any phase → snap all text to visible, advance to dashboard immediately (0.3s crossfade). Never trap an impatient user.

### Copy — Complete Set

#### State Lines (Phase 1a)

```swift
func whyToday_stateLine(source: String, recoveryPercent: Double?, recoveryScore: Int) -> String {
    if let pct = recoveryPercent, source != "manual" {
        return "You're at \(Int(pct))% today."
    } else {
        return "You checked in at \(recoveryScore) today."
    }
}
```

#### Tier Words (Phase 1b)

```swift
let whyToday_tierWord: [String: String] = [
    "high": "Peaked.",
    "mid":  "Rebuilding.",     // deliberately not "Functional" — rebuilding sounds more human
    "low":  "Resting."
]
```

Wait — Jackson's feedback uses "Rebuilding" for low tier. Let me realign:

```swift
let whyToday_tierWord: [String: String] = [
    "high": "Peaked.",
    "mid":  "Working.",
    "low":  "Rebuilding."
]
```

#### Need Text (Phase 2) — 3 variants per tier, rotating

```swift
let whyToday_needText: [String: [String]] = [
    "high": [
        "Your system's fully online. This is the morning to use what you've got.",
        "Clean night, full signal. Your body's not asking you to hold back today.",
        "Everything recovered. The question isn't whether to go — it's how far."
    ],
    "mid": [
        "Your nervous system is running but it's not asking for more. Listen to that.",
        "Not sharp, not broken. Just a body doing its job. Meet it where it is.",
        "Rough edges from last night, but nothing that needs protecting. Steady input."
    ],
    "low": [
        "Your nervous system is doing the heavy lifting right now. Give it ease.",
        "Yesterday took something from you. Today's job is to give a little back.",
        "Low signal. That's not a failure — it's your body telling you what it needs."
    ]
]
```

**Rotation:** Same mechanism as insight variants — `lastWhyToday_high/mid/low` in UserDefaults, index mod 3.

#### Bridge Text (Phase 3)

Single string, same for all tiers:

```swift
let whyToday_bridge = "This is what meets you there."
```

Alternatives considered and rejected:
- "Here's your brew." — too transactional
- "We built this for today." — too corporate
- "This is what meets you there." — perfect. Coffee as response.

### Wearable vs. Manual Variants

**Wearable user:** Phase 1a uses recovery percentage. Phase 2 can reference HRV or sleep.

Extended Phase 2 variants for wearable users (appended to need text when data is notable):

```swift
func whyToday_wearableAppend(hrv: Double?, baselineHrv: Double?, sleepHours: Double?) -> String? {
    if let hrv = hrv, let baseline = baselineHrv, baseline > 0 {
        let delta = ((hrv - baseline) / baseline) * 100
        if abs(delta) > 15 {
            let dir = delta > 0 ? "up" : "down"
            return "HRV is \(abs(Int(delta)))% \(dir) from your baseline."
        }
    }
    if let sleep = sleepHours, sleep < 5.5 {
        return "Short night — \(String(format: "%.0f", sleep)) hours."
    }
    return nil
}
```

If present, appended as a new line below need text — Mono, 13px, #666666. Factual counterpoint to the emotional text above.

**Manual user:** Phase 1a uses recovery score ("You checked in at 5 today."). No wearable append.

---

## 4. Flavor Preview — `FlavorPreviewView.swift`

### Purpose

Before brewing, let the user *feel* what they're about to drink. Not a tasting card. A sensory premonition.

### Location

Two placements:
1. **Dashboard Card 3** — the "This cup is:" strip (defined in §1 above)
2. **Brew Recommendation Detail** — expanded flavor preview section below brew parameters, above bean cards

### Expanded Flavor Preview (Detail Screen)

```
│                                 │
│  ┌─────────────────────────────┐│  FlavorPreviewView.swift
│  │                             ││
│  │  ░░░░░░░░░░░░░░░░░░░░░░░░  ││  gradient background tint
│  │                             ││  profile gradient at 0.06 opacity
│  │  WHAT YOU'LL TASTE          ││  Mono, 11px, #888888, ALL CAPS
│  │                             ││  bottom padding: 12pt
│  │  A cup that stays quiet.    ││  Regular, 16px, #FFFFFF
│  │  Floral up front — jasmine, ││  line-height: 1.6
│  │  a trace of bergamot — then ││
│  │  it settles into something  ││
│  │  sweet and round. Low       ││
│  │  acidity. No edge. The kind ││
│  │  of brew you finish without ││
│  │  realizing you've finished  ││
│  │  it.                        ││
│  │                             ││  bottom padding: 16pt (md)
│  │  This cup is:               ││  Mono, 11px, #888888
│  │  Soft. Grounding. Unhurried.││  CondensedBold, 18px, #FFFFFF
│  │                             ││
│  └─────────────────────────────┘│  bg: #1A1A1A, radius: 16pt
│                                 │  padding: 20pt inner
```

### Sensory Descriptions — Complete Set

One description per profile × tier. This is the paragraph that makes the user *taste* the cup before they brew it.

```swift
let sensoryCopy: [String: [String: String]] = [
    "tide_pool": [
        "high": "A cup that doesn't compete with your morning. Naturally sweet, almost floral — think honey dissolved in cool water. Low acidity, soft body, the kind of finish that just... stops cleanly. You won't notice it's gone.",
        "mid":  "A cup that stays quiet. Floral up front — jasmine, a trace of bergamot — then it settles into something sweet and round. Low acidity. No edge. The kind of brew you finish without realizing you've finished it.",
        "low":  "The gentlest thing Brezi makes. Almost tea-like in body — delicate, transparent, naturally sweet. No bitterness, no acid spike, nothing that demands your attention. Just warmth with a flavor."
    ],
    "first_light": [
        "high": "Clean and direct. A little chocolate, a little nut, a finish that clears in seconds. Not trying to impress you — just reliably good. The cup that never disappoints.",
        "mid":  "Balanced in the way that only matters when you're paying attention. Mild sweetness up front, clean through the middle, a finish that's dry without being sharp. Everyday coffee, done correctly.",
        "low":  "Familiar and honest. Toasted grain, a little cocoa, a finish that doesn't linger. Nothing challenging. Just enough to start the day without asking too much of it."
    ],
    "deep_work": [
        "high": "Layered. The first sip is chocolate and structure. The second reveals something darker — walnut, maybe tobacco — that unfolds over the next three hours. This cup isn't a moment. It's a session.",
        "mid":  "Dense and focused. Dark chocolate dominance with a low, sustained sweetness underneath. The kind of cup that sits on your desk getting better as it warms. Doesn't distract. Just holds.",
        "low":  "Rich and serious. Heavy body, dark flavors, no bright notes to compete for attention. Cedar, dark cocoa, a finish that stays. This is the cup that fills the room."
    ],
    "voltage": [
        "high": "Explosive. Bright acidity hits first — grapefruit, maybe blackcurrant — then a wave of concentrated fruit sweetness. Complex, demanding, the kind of cup that makes you close your eyes for a second. This is not background coffee.",
        "mid":  "Bold and unapologetic. Concentrated fruit notes up front — stone fruit, citrus — backed by a body that's heavier than you expect. The finish sparks. Not subtle. Not trying to be.",
        "low":  "Sharp. Concentrated. Aggressive in the way that either wakes you up or overwhelms you. Bright acid, heavy extraction, a finish that doesn't fade. You asked for this one. Honor it."
    ],
    "the_blank": [
        "high": "You're brewing your own recipe. Flavor depends on your beans, your settings, your instincts. Brezi just executes.",
        "mid":  "Custom settings, custom result. The flavor's whatever you're chasing today.",
        "low":  "Your parameters, your outcome. No predictions — just what you built."
    ]
]
```

### Gradient Background Tint Spec

The expanded flavor preview card has a very subtle gradient wash:
- Base: `#1A1A1A`
- Tint: first color from `profileGradients[profileKey]` at 0.06 opacity
- Applied as overlay gradient: top-leading to bottom-trailing
- Barely visible — subliminal color association

---

## 5. Post-Brew Emotional Check-In — `EmotionalCheckInView.swift`

### Purpose

Replace clinical star rating with an emotional vocabulary that feeds back into the recommendation engine. Stars remain as secondary/fallback (the v3.0 spec's rating system), but the primary check-in uses feeling words.

### Architecture Change

**New field on BrewLog:**

```swift
@NSManaged var emotionalRating: String?  // "hit_right", "needed_more", "too_much", "surprising", "flat"
// BrewLog.rating (1-5 Int16) still exists — derived from emotionalRating, or set directly via stars
```

**Emotional → Star Mapping:**

```swift
let emotionalToStars: [String: Int16] = [
    "hit_right":   4,
    "needed_more": 3,
    "too_much":    2,
    "surprising":  5,
    "flat":        2
]
```

Stars are still stored for correlation charts and history. But the primary input is emotional.

### Brew Completion Overlay — Updated

Replaces the star picker as the PRIMARY rating moment. Stars remain accessible via "Or rate with stars ›" link.

```
┌─────────────────────────────────┐
│                                 │
│                                 │
│         Brewed.                 │  CondensedBold, 48px, #FFFFFF
│                                 │  bottom padding: 16pt (md)
│         FIRST LIGHT             │  Mono, 14px, #888888
│         7:14am · 12 min         │  Mono, 14px, #888888
│                                 │  bottom padding: 32pt (xl)
│  How did that land?             │  Regular, 17px, #CCCCCC
│                                 │  bottom padding: 16pt (md)
│  ┌─────────────────────────────┐│  EmotionalRatingOption.swift
│  │  ● Hit right                ││  option 1
│  │    Matched the morning.     ││
│  └─────────────────────────────┘│  gap: 8pt (sm)
│  ┌─────────────────────────────┐│
│  │  ○ Needed more              ││  option 2
│  │    Wanted something stronger.│
│  └─────────────────────────────┘│  gap: 8pt (sm)
│  ┌─────────────────────────────┐│
│  │  ○ Too much today           ││  option 3
│  │    Should've gone lighter.  ││
│  └─────────────────────────────┘│  gap: 8pt (sm)
│  ┌─────────────────────────────┐│
│  │  ○ Surprising               ││  option 4
│  │    Didn't expect that.      ││
│  │    Good different.          ││
│  └─────────────────────────────┘│  gap: 8pt (sm)
│  ┌─────────────────────────────┐│
│  │  ○ Flat                     ││  option 5
│  │    Nothing landed.          ││
│  └─────────────────────────────┘│
│                                 │  bottom padding: 24pt (lg)
│  ┌─────────────────────────┐    │  blue filled (#488BED)
│  │  DONE  →                │    │  DISABLED until selection
│  └─────────────────────────┘    │  CondensedBold, 16px, #FFFFFF, ALL CAPS
│                                 │  height: 48pt, opacity: 0.4 when disabled
│  Or rate with stars ›           │  Regular, 14px, #666666, centered
│  Skip for now ›                 │  Regular, 14px, #888888, centered
│                                 │
└─────────────────────────────────┘
```

### EmotionalRatingOption.swift Spec

- Background: `#1A1A1A`
- Border: 1px `#333333`
- Radius: 12pt
- Padding: 16pt horizontal, 14pt vertical
- Min height: 52pt (ensures 44pt tap target)
- Full-width tap target

**States:**
- Default: border `#333333`, radio circle (○) border `#333333` 1.5px, no fill
- Selected: border 2px `#488BED`, background `rgba(72, 139, 237, 0.08)`, radio circle (●) fill `#488BED` + white center dot
- Selection animation: spring (response: 0.3, dampingFraction: 0.7)

**Text:**
- Label: CondensedBold, 17px, #FFFFFF (e.g. "Hit right")
- Sublabel: Regular, 13px, #888888 (e.g. "Matched the morning.")
- Radio circle: 18pt diameter, 12pt from leading edge, vertically centered

**Single selection:** Tapping a new option deselects the previous.

### Copy — Complete Set

```swift
let emotionalCheckIn_prompt = "How did that land?"

let emotional_hitRight_label = "Hit right"
let emotional_hitRight_sub = "Matched the morning."

let emotional_neededMore_label = "Needed more"
let emotional_neededMore_sub = "Wanted something stronger."

let emotional_tooMuch_label = "Too much today"
let emotional_tooMuch_sub = "Should've gone lighter."

let emotional_surprising_label = "Surprising"
let emotional_surprising_sub = "Didn't expect that. Good different."

let emotional_flat_label = "Flat"
let emotional_flat_sub = "Nothing landed."

let emotional_starsLink = "Or rate with stars"
let emotional_skip = "Skip for now"
```

### "Or rate with stars ›" Behavior

Tapping replaces the emotional options with the standard `RatingStarsView` (36pt stars, as per v3.0 spec). A link appears below: `"Or tell us how it felt ›"` to toggle back. Whichever method the user uses last is what gets saved.

### Feedback Loop — How Emotional Ratings Shape Future Recommendations

**Stored in UserDefaults (accumulating):**

```swift
// Key format: "emotional_{profileKey}_{emotionalRating}"
// Value: count (Int)
// Example: "emotional_first_light_hit_right" = 8, "emotional_voltage_too_much" = 3
```

**Engine rules (for Jensen — `InsightEngine.swift` extension):**

```swift
func shouldAdjustRecommendation(profileKey: String) -> String? {
    let tooMuch = emotionalCount(profileKey, "too_much")
    let neededMore = emotionalCount(profileKey, "needed_more")
    let flat = emotionalCount(profileKey, "flat")

    // If user consistently says "too much" for a profile at a given tier,
    // shift recommendation one tier gentler
    if tooMuch >= 3 && tooMuch > neededMore {
        return "gentler"  // shift down one profile
    }

    // If user consistently says "needed more", shift one tier stronger
    if neededMore >= 3 && neededMore > tooMuch {
        return "stronger"  // shift up one profile
    }

    // If "flat" dominates, suggest trying a different bean
    if flat >= 3 {
        return "bean_change"  // surface different bean recommendation
    }

    return nil  // no adjustment
}
```

**Post-brew Card 2 (InsightStripView) — emotional feedback acknowledgment:**

When the user has 3+ emotional ratings for the same profile, Card 2's post-brew reflection can reference the pattern:

```swift
let emotionalFeedback_tooMuch = "You've been saying Voltage hits too hard at this recovery level. Next time, Brezi will suggest one step lighter."
let emotionalFeedback_neededMore = "You keep wanting more from First Light. Tomorrow, if your signal's the same, we'll push it."
let emotionalFeedback_hitRight = "First Light keeps hitting right on mid-recovery mornings. Pattern locked."
let emotionalFeedback_surprising = "You rated this one surprising — in a good way. We'll remember that."
let emotionalFeedback_beanChange = "Flat twice in a row on the same bean. Try switching origins — different terroir might land better."
```

---

## 6. Origin Story Snippets — Complete Copy

These appear in three places:
1. `OriginStorySheet.swift` — full story (half-sheet, detail screen)
2. `BeanCardView.swift` — one-line hook visible on the card when expanded
3. `WhyTodayView.swift` — occasionally appended to the bridge phase as a whisper

### Full Origin Stories

```swift
let originStories: [String: OriginStory] = [
    "Ethiopia Yirgacheffe": OriginStory(
        headline: "Where coffee began.",
        body: "Yirgacheffe is where coffee began — not as a commodity, but as a cherry growing wild in Ethiopian highland forests. Every washed lot from here carries that original delicacy. It's the most unhurried coffee on earth.",
        altitude: "1,700–2,200 masl",
        harvest: "Oct–Jan"
    ),
    "Colombia Huila": OriginStory(
        headline: "Volcanic patience.",
        body: "Huila sits in southern Colombia where volcanic soil and high altitude conspire to produce coffee that is impossibly balanced. Farmers here have been growing for generations. The patience shows.",
        altitude: "1,250–2,000 masl",
        harvest: "Apr–Jul, Oct–Jan"
    ),
    "Honduras Marcala": OriginStory(
        headline: "Quiet excellence.",
        body: "Marcala was Honduras's first designated origin — a small mountain town that proved Central American coffee could be gentle and exceptional at the same time. Quiet excellence.",
        altitude: "1,300–1,700 masl",
        harvest: "Nov–Mar"
    ),
    "Guatemala Antigua": OriginStory(
        headline: "Between three volcanoes.",
        body: "Antigua Valley sits between three volcanoes. The ash in the soil gives the coffee a mineral backbone that most origins can't touch. It's been Guatemala's benchmark for a century. Still is.",
        altitude: "1,500–1,700 masl",
        harvest: "Dec–Mar"
    ),
    "Brazil Cerrado": OriginStory(
        headline: "The backbone.",
        body: "Cerrado Mineiro is Brazil's savanna coffee country — flat, hot, mechanized. What it lacks in romance it makes up in consistency. This is the backbone of the world's coffee supply, and the good lots are genuinely great.",
        altitude: "800–1,300 masl",
        harvest: "May–Sep"
    ),
    "Costa Rica Tarrazú": OriginStory(
        headline: "Obsessive craft.",
        body: "Tarrazú is Costa Rica's highest-altitude growing region, and the farmers there treat honey processing like fine art — partially removing the cherry mucilage to control sweetness at the molecular level. Obsessive. Worth it.",
        altitude: "1,200–1,900 masl",
        harvest: "Nov–Mar"
    ),
    "Ethiopia Sidama": OriginStory(
        headline: "Wild, fruity, polarizing.",
        body: "Sidama is Ethiopia's most productive coffee zone but the natural lots — dried in the cherry under open sun — are anything but industrial. They're wild, fruity, polarizing. The good ones are transcendent.",
        altitude: "1,500–2,200 masl",
        harvest: "Oct–Jan"
    ),
    "Sumatra Lintong": OriginStory(
        headline: "Like no other origin on earth.",
        body: "Lintong sits on the edge of Lake Toba in northern Sumatra. Wet-hulling — the region's signature process — strips the parchment early and dries the green bean in humid air. It produces a cup that tastes like no other origin on earth. You either get it or you don't.",
        altitude: "1,100–1,600 masl",
        harvest: "Year-round (primary: Jun–Dec)"
    ),
    "Kenya AA": OriginStory(
        headline: "Specialty's performance benchmark.",
        body: "Kenya grades its coffee by bean size — AA is the largest, the densest, the most sought-after. Nyeri and Kirinyaga produce lots with an acidity so clean it borders on electric. This is specialty coffee's performance benchmark.",
        altitude: "1,400–2,000 masl",
        harvest: "Oct–Dec (main), Jun–Aug (fly)"
    ),
    "Ethiopia Guji": OriginStory(
        headline: "Every lot is a fingerprint.",
        body: "Guji split from Sidama as its own designated origin because the coffee was simply too different to share a name. Wild-grown, sun-dried, often processed by single families on raised beds. Every lot is a fingerprint.",
        altitude: "1,800–2,300 masl",
        harvest: "Oct–Jan"
    ),
    "Rwanda Nyamasheke": OriginStory(
        headline: "Precision that feels earned.",
        body: "Rwanda's coffee industry was rebuilt from nothing after 1994. Twenty years later, Nyamasheke washing stations are producing some of East Africa's most refined lots. There's a precision to Rwandan coffee that feels earned.",
        altitude: "1,500–2,000 masl",
        harvest: "Mar–Jul"
    )
]
```

### One-Line Hooks (for BeanCardView inline)

When the bean card is visible but not expanded into the full origin sheet, a single line of origin character appears:

```swift
let originOneLiner: [String: String] = [
    "Ethiopia Yirgacheffe":  "The birthplace. The original delicacy.",
    "Colombia Huila":        "Volcanic soil. Generational patience.",
    "Honduras Marcala":      "Central America's gentle benchmark.",
    "Guatemala Antigua":     "Three volcanoes. One mineral backbone.",
    "Brazil Cerrado":        "The world's backbone, done right.",
    "Costa Rica Tarrazú":    "Honey-processed at molecular precision.",
    "Ethiopia Sidama":       "Sun-dried. Wild. Transcendent when it works.",
    "Sumatra Lintong":       "Wet-hulled on the edge of Lake Toba.",
    "Kenya AA":              "The densest bean. The cleanest acidity.",
    "Ethiopia Guji":         "Sun-dried by single families. Every lot unique.",
    "Rwanda Nyamasheke":     "Rebuilt from nothing. Now, East Africa's most refined."
]
```

---

## 7. Updated Component Library (Additions)

| Component | File | Used In |
|---|---|---|
| `EmotionalBrewCardView` | EmotionalBrewCardView.swift | Dashboard Card 3 (replaces top section of DashboardBrewCardView) |
| `EmotionalHeaderView` | EmotionalHeaderView.swift | Card 3 — gradient strip + tagline + feeling words |
| `FlavorPreviewStrip` | FlavorPreviewStrip.swift | Dashboard Card 3 — "This cup is:" compact |
| `FlavorPreviewView` | FlavorPreviewView.swift | Brew Recommendation Detail — expanded sensory block |
| `BeanCardView` | BeanCardView.swift | Brew Recommendation Detail — bean recommendations |
| `OriginStorySheet` | OriginStorySheet.swift | Half-sheet from bean card |
| `WhyTodayView` | WhyTodayView.swift | Full-screen transitional moment |
| `EmotionalCheckInView` | EmotionalCheckInView.swift | Brew Completion Overlay — primary rating |
| `EmotionalRatingOption` | EmotionalRatingOption.swift | Individual option in emotional check-in |
| `ProfileGradient` | ProfileGradient.swift | Gradient definitions per profile |

---

## 8. Updated Data Model (Additions)

### New Entity: BeanProfile (see §2 above)

### BrewLog — New Field
```swift
@NSManaged var emotionalRating: String?   // "hit_right", "needed_more", "too_much", "surprising", "flat"
@NSManaged var beanName: String?          // e.g. "Ethiopia Yirgacheffe" — user's selected/logged bean
```

### New UserDefaults Keys
```swift
// Why Today
"lastWhyTodayDate"                          // String: "2026-03-19"

// Why Today copy rotation
"lastWhyToday_high"                         // Int (0, 1, or 2)
"lastWhyToday_mid"                          // Int
"lastWhyToday_low"                          // Int

// Emotional feedback accumulation
"emotional_{profileKey}_{emotionalRating}"  // Int (count)
// e.g. "emotional_first_light_hit_right" = 8
```

---

## 9. Integration with v3.0 Spec

This document is **additive**. Here's how each new element integrates:

| v3.0 Element | Change | Notes |
|---|---|---|
| DashboardBrewCardView (Card 3) | Top section replaced by EmotionalBrewCardView | Parameters move below emotional layer. "START BREW" and "See full details" unchanged. |
| BrewCompletionOverlay | Star picker replaced by EmotionalCheckInView as primary | Stars accessible via toggle. Both write to BrewLog. |
| BrewRecommendationView (detail) | FlavorPreviewView + BeanCardView sections added | Inserted between brew parameter card and "Why This Brew" chain. |
| "Reading your signal..." transition | Replaced by WhyTodayView | Longer but skippable. Only fires once per day. |
| InsightStripView (post-brew) | Emotional feedback acknowledgment lines added | Only after 3+ emotional ratings for same profile. |
| ProfilePickerView | Emotional tagline replaces single-line tagline | Cards get richer. |
| PostBrewCardView | "RATE THIS BREW" button becomes "HOW DID THAT LAND?" | Opens emotional check-in inline. Stars still accessible. |
| Core Data | BeanProfile entity added. BrewLog gains emotionalRating + beanName | Migration needed. |
| HistoryView BrewLogCell | Adds emotional rating badge if present | e.g. "Hit right" in blue, below stars or replacing them. |

### What Doesn't Change
- Navigation architecture (push stack, no tab bar)
- Design tokens (colors, typography, spacing)
- Onboarding flow (screens 1-5 unchanged except transition after screen 5)
- Health data card (Card 1) — untouched
- Insight copy system (Card 2 pre-brew) — untouched, new post-brew lines added
- Brewing state — untouched
- Brew cancel flow — untouched
- BLE disconnect handling — untouched
- Device Hub — untouched
- Settings — untouched (bean preference settings deferred to v2)
- Offline behavior — untouched
- Notification tiers — untouched

---

## 10. Animation Reference (Additions)

| Location | Element | Type | Duration | Easing | Delay | Trigger |
|---|---|---|---|---|---|---|
| Dashboard Card 3 | "This cup is:" words | sequential fadeIn | 0.15s each | easeOut | 0.1s between | Card appear |
| Dashboard Card 3 | Gradient strip | fadeIn | 0.3s | easeOut | 0.0s | Card appear |
| Why Today | State line | fadeIn | 0.3s | easeOut | 0.2s | Screen appear |
| Why Today | Tier word | fadeIn + translateY(8→0) | 0.3s | easeOut | 0.6s | After state |
| Why Today | Need text | fadeIn | 0.4s | easeOut | 2.1s | After pause |
| Why Today | Bridge + arrow | fadeIn | 0.3s | easeOut | 3.5s | After pause |
| Why Today | Arrow pulse | opacity 1.0↔0.5 | 1.2s | easeInOut | repeat | After bridge |
| Why Today | Auto-advance | crossfade | 0.5s | easeOut | 5.5s | After all |
| Why Today | Tap-to-skip | snap + crossfade | 0.3s | easeOut | immediate | Tap anywhere |
| Completion | Emotional option select | spring | 0.3s | spring(0.7) | 0.0s | Option tap |
| Completion | Stars ↔ emotional toggle | crossfade | 0.2s | easeOut | 0.0s | Link tap |
| Bean cards | Sequential appear | fadeIn + translateY(12→0) | 0.3s each | easeOut | 0.15s between | Section scroll into view |
| Origin sheet | Sheet appear | system sheet | system | system | 0.0s | "Origin story ›" tap |

---

## 11. Build Order Amendment (for Jensen)

Insert between existing Day 5–6 and Day 7–8:

**Day 5 (amended):** Add `BeanProfile` Core Data entity + seed data (11 beans). Add `emotionalRating` + `beanName` to `BrewLog`. Add emotional feedback accumulation to `InsightEngine`.

**Day 6 (amended):** Build `WhyTodayView` (transitional screen). Build `EmotionalBrewCardView` (dashboard card 3 emotional layer). Build `FlavorPreviewStrip` + `FlavorPreviewView`. Build `BeanCardView` + `OriginStorySheet`.

**Day 7 (amended):** Build `EmotionalCheckInView` + `EmotionalRatingOption` (replaces star picker as primary). Wire emotional → star mapping. Wire "Or rate with stars" toggle. Wire emotional feedback accumulation + acknowledgment in post-brew InsightStripView.

Everything else in the build order shifts by ~1 day. TestFlight target becomes 15 working days.

---

## 12. Copy Audit — All New Strings

Every new user-facing string in this document, organized by screen:

### Why Today
```swift
// State lines
"You're at {pct}% today."
"You checked in at {score} today."

// Tier words
"Peaked."
"Working."
"Rebuilding."

// Need text (9 variants — 3 per tier, see §3)

// Bridge
"This is what meets you there."

// Wearable appends
"HRV is {delta}% {up/down} from your baseline."
"Short night — {hours} hours."
```

### Emotional Brew Card
```swift
// Emotional taglines (15 strings — see §1)
// Cup feelings (15 strings — see §1)
```

### Flavor Preview
```swift
"WHAT YOU'LL TASTE"
"This cup is:"
// Sensory descriptions (15 strings — see §4)
```

### Bean Cards
```swift
"RECOMMENDED BEANS"
"Origin story"
"You're driving. Pick any bean you want — The Blank adapts to whatever you bring."

// Per bean (11 beans):
// - flavorFeeling (11 strings)
// - emotionalTag (11 strings)
// - pairingNote (11 strings)
// - originStory body (11 strings)
// - originHeadline (11 strings)
// - originOneLiner (11 strings)
```

### Emotional Check-In
```swift
"How did that land?"
"Hit right"
"Matched the morning."
"Needed more"
"Wanted something stronger."
"Too much today"
"Should've gone lighter."
"Surprising"
"Didn't expect that. Good different."
"Flat"
"Nothing landed."
"Or rate with stars"
"Or tell us how it felt"
"Skip for now"
```

### Emotional Feedback Acknowledgment
```swift
"You've been saying {profile} hits too hard at this recovery level. Next time, Brezi will suggest one step lighter."
"You keep wanting more from {profile}. Tomorrow, if your signal's the same, we'll push it."
"{profile} keeps hitting right on {tier}-recovery mornings. Pattern locked."
"You rated this one surprising — in a good way. We'll remember that."
"Flat twice in a row on the same bean. Try switching origins — different terroir might land better."
```

### Post-Brew Card Update
```swift
"HOW DID THAT LAND?"  // replaces "RATE THIS BREW"
```

---

## 13. Decisions — This Round

1. **Emotional taglines are recovery-tier-aware.** Same profile, different tier = different emotional framing. 15 combinations, all written.
2. **Bean recommendations are fixed per profile, not dynamically matched.** v1 ships with curated pairings. Dynamic bean matching (based on flavor preference from emotional ratings) is v2.
3. **"Why Today" is a once-daily transitional moment, not a screen in the nav stack.** It doesn't have a back button. It advances forward. Skip by tapping.
4. **Emotional check-in replaces stars as primary, not as only.** Both input methods coexist. Emotional maps to stars internally. Correlation charts still work.
5. **Sensory copy is written per profile × tier, not per bean.** The flavor preview describes the *brew experience*, not the bean in isolation. Bean-specific flavor is on the bean card.
6. **Origin stories are not gated.** Available from day 1. Coffee people will read them. Non-coffee people will ignore them. Both are fine.
7. **"This cup is:" uses three words, always.** Brevity forces precision. Four words is a list. Three words is a feeling.
8. **Feedback loop is simple accumulation, not ML.** Count emotional ratings per profile. After 3+ of the same response, adjust. No weighting, no decay, no complexity. v2 can get smarter.

---

## 14. What This Doesn't Cover (Explicitly Deferred)

| Item | Reason | When |
|---|---|---|
| Dynamic bean matching from flavor preferences | Need emotional rating data first | v2 |
| Bean inventory / availability | Requires commerce layer | When commerce ships |
| Seasonal bean rotation | Need supplier partnerships | Post-launch |
| "Flavor profile" user preference screen | Let implicit data accumulate first | v2 |
| Bean rating (separate from brew rating) | One rating moment is enough for v1 | v2 |
| Sensory description variants (multiple per profile×tier) | 15 is enough for launch | Month 2 |
| AI-generated personalized sensory copy | Static copy is better than bad AI copy | v3+ |

---

*The data tells you your state. The coffee meets you there. Not functionally — emotionally, sensorially, specifically. The gap between "correct recommendation" and "felt recommendation" is closed by three things: language that sounds like a person who knows your body, flavor described as feeling instead of chemistry, and a transitional moment that says "I see you" before it says "here's your brew."*

*This is the soul layer.*

— Picasso
