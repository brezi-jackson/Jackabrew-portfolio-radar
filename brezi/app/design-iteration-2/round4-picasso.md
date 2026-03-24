# Brezi iOS App — Emotional Layer Design v4.0 (Final)
**Author:** Picasso (Product Design) | March 19, 2026 | Round 4 — Final
**For:** Jackson (Product) · Jensen (Engineering) · Elon (Growth/Copy)
**Status:** LOCKED. This is the build spec. No more design rounds.

---

## 0. What Changed from Round 3

Round 3 was seventeen line rewrites and one small feature. Round 4 is six targeted changes from Elon's final review, plus one designer-initiated addition. No structural changes. No new screens. No new navigation. The five-job architecture doesn't move.

**Changes:**
- "Surprising" decoupled from star ratings — separate signal for variety weighting, no quality inflation
- Why Today gains smart suppression — three consecutive skips trigger 7-day cooldown with shortened reintroduction, not permanent kill
- Bean rotation logic defined and built — sort-order demotion on 2+ "Flat" ratings for same bean. "We'll rotate the origin" is now a kept promise
- Five feedback copy lines surgically rewritten per Elon's polish audit — mirrored user language, specific mechanics, honest methodology labels
- Origin story sheet gains "Your Yirgacheffe" relationship line at 5+ brews
- Sixth emotional check-in option: "Not yet" — deferred rating with 60-minute notification, converts skips to signals

**What doesn't change from Round 3:**
- The five-job structure (Why Today -> Card 3 -> Flavor Preview -> Bean Card -> Check-In)
- All 11 bean seed records (origin stories, headlines, one-liners, all unchanged)
- All 15 brew-forward taglines (including Round 3's Blank rewrites)
- All 15 cup feelings (including Round 3's Blank rewrites)
- All 15 sensory descriptions (including Round 3's six rewrites)
- Why Today architecture (tier word -> data line -> need text -> bridge)
- Haptic on tier word
- Surprise moment system (all 8 variants, including Round 3's two rewrites)
- All animations
- All component specs (except EmotionalRatingOption gains one sibling)
- Profile color gradients
- Bean card UI, origin story sheet layout
- All integration points with v3.0 base spec

**Division of labor (locked, unchanged since Round 2):**

| Screen | Job | Speaks about |
|---|---|---|
| Why Today | Here's where you are | The body's state |
| Card 3 tagline | Here's what meets you there | The coffee's response |
| Flavor preview | What you'll taste | The sensory experience |
| Bean card | This specific coffee's personality | The bean's character |
| Emotional check-in | How did that land? | The user's felt response |

Five layers. Five jobs. No overlap.

---

## 1. Emotional Recommendation Card — `EmotionalBrewCardView.swift`

**Replaces:** Top section of `DashboardBrewCardView.swift` (Card 3). Parameters stay — they move below the emotional layer. Same component structure as Rounds 2-3.

### Card 3 Structure (Pre-Brew)

```
+-----------------------------------------+
|                                         |  bg: #0F0F0F, radius: 24pt (brewCard)
|  +-----------------------------------+  |  EmotionalHeaderView.swift
|  |                                   |  |  profile color gradient strip
|  |  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  |  |  height: 3pt, radius: 1.5pt
|  +-----------------------------------+  |  top padding: 24pt (lg)
|                                         |
|  FIRST LIGHT                            |  CondensedBold, 32px, #FFFFFF
|                                         |  bottom padding: 4pt (xs)
|  Balanced extraction, clean             |  Regular, 16px, #CCCCCC
|  finish. Your everyday                  |  line-height: 1.5
|  standard, set high.                    |
|                                         |  bottom padding: 20pt
|  +-----------------------------------+  |  FlavorPreviewStrip.swift
|  |  This cup is:                     |  |  Mono, 11px, #888888
|  |  Clean. Steady. Familiar.         |  |  CondensedBold, 18px, #FFFFFF
|  +-----------------------------------+  |  bottom padding: 20pt
|                                         |
|  -------------------------------------  |  divider: 1px #222222
|                                         |
|  +------+  +------+  +------+          |  (parameters — same spec as v3.0)
|  | 4-8  |  |  12  |  |  60  |          |  CondensedBold, 22px, #FFFFFF
|  |  C   |  | min  |  | g/L  |          |  Mono, 10px, #666666, ALL CAPS
|  | TEMP |  | TIME |  | DOSE |          |
|  +------+  +------+  +------+          |  3 equal columns, 8pt gap
|                                         |
|  Medium roast . Medium grind            |  Mono, 13px, #888888
|  1 serve . Best before 2pm             |  Mono, 13px, #488BED
|                                         |  bottom padding: 24pt (lg)
|  +-----------------------------------+  |
|  |  START BREW  ->                   |  |  white filled, #000000 text, 48pt
|  +-----------------------------------+  |
|                                         |
|  See full details >                     |  Regular, 14px, #888888, 44x44pt
|                                         |
+-----------------------------------------+
```

### Profile Color Gradients — `ProfileGradient.swift`

Each profile has a signature gradient used in the 3pt strip, flavor preview tint, and Why Today background.

```swift
let profileGradients: [String: [Color]] = [
    "tide_pool":   [Color(hex: "#2D6A4F"), Color(hex: "#52B788")],  // deep green -> soft green
    "first_light": [Color(hex: "#488BED"), Color(hex: "#90B8F8")],  // blue -> light blue
    "deep_work":   [Color(hex: "#B8860B"), Color(hex: "#DAA520")],  // dark gold -> gold
    "voltage":     [Color(hex: "#FF453A"), Color(hex: "#FF6B6B")],  // red -> warm red
    "the_blank":   [Color(hex: "#555555"), Color(hex: "#999999")]   // mid grey -> light grey
]
```

- Gradient strip: 3pt height, full card width minus 48pt (24pt padding each side), radius 1.5pt
- Direction: leading to trailing (left -> right)
- Subtle glow: `.shadow(color: gradient[0].opacity(0.3), radius: 8, x: 0, y: 2)` beneath strip

### Card 3 Taglines — Brew-Forward (All 15 Combinations)

These are about the **coffee's response**, not the body's state. Why Today already told the user their state. Card 3 tells them what the coffee does about it.

```swift
let brewTaglines: [String: [String: String]] = [
    "tide_pool": [
        "high": "Low dose, gentle extraction. You don't need intensity today and this doesn't pretend otherwise.",
        "mid":  "Low concentration, naturally sweet. Stays out of your way.",
        "low":  "The gentlest extraction Brezi makes. Low dose, no bite, no agenda."
    ],
    "first_light": [
        "high": "Balanced extraction, clean finish. Your everyday standard, set high.",
        "mid":  "Moderate dose, medium steep. The brew that shows up the same every time.",
        "low":  "Standard parameters, honest cup. Just enough to start the day."
    ],
    "deep_work": [
        "high": "Extended steep, full dose. Built to hold for three hours.",
        "mid":  "Longer extraction, heavier body. Focus-grade caffeine, slow release.",
        "low":  "Dense extraction on a low morning. This is a bet. Rate it honest."
    ],
    "voltage": [
        "high": "Maximum concentration, full extraction. The hardest brew Brezi pours.",
        "mid":  "Data says mid. This brew says full send. Your call.",
        "low":  "Concentrated, aggressive, not what the data recommends. You asked for it."
    ],
    "the_blank": [
        "high": "Your parameters, your bean, your morning. Brezi just holds the temperature.",
        "mid":  "Mid-range signal, manual brew. You're testing something. Brezi doesn't need to know what.",
        "low":  "Low signal, manual override. You know what you're doing — or you're finding out."
    ]
]
```

**Selection logic:**
```swift
func brewTagline(profile: String, recoveryTier: String) -> String {
    return brewTaglines[profile]?[recoveryTier] ?? profileTaglines[profile] ?? ""
}
```

### "This cup is:" — Feeling Words (All 15 Combinations)

Three words. Always three. Not four. Not a sentence. Three words is a feeling.

```swift
let cupFeelings: [String: [String: String]] = [
    "tide_pool": [
        "high": "Gentle. Sweet. Deliberate.",
        "mid":  "Soft. Grounding. Unhurried.",
        "low":  "Quiet. Warm. Simple."
    ],
    "first_light": [
        "high": "Clean. Steady. Familiar.",
        "mid":  "Balanced. Clear. Reliable.",
        "low":  "Simple. Honest. Enough."
    ],
    "deep_work": [
        "high": "Structured. Rich. Sustaining.",
        "mid":  "Focused. Layered. Patient.",
        "low":  "Dense. Heavy. Committed."
    ],
    "voltage": [
        "high": "Electric. Complex. Alive.",
        "mid":  "Bold. Bright. Unapologetic.",
        "low":  "Sharp. Concentrated. Yours."
    ],
    "the_blank": [
        "high": "Intentional. Specific. Yours.",
        "mid":  "Manual. Curious. Uncharted.",
        "low":  "Manual. Exact. Yours."
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

Each brew profile maps to 2-3 recommended beans. Beans are chosen because their sensory character matches the brew profile's purpose. Bean cards describe the **coffee's personality**, not the user's need.

**Core Data Entity:**

```swift
@objc(BeanProfile)
class BeanProfile: NSManagedObject {
    @NSManaged var id: UUID
    @NSManaged var name: String              // "Ethiopia Yirgacheffe"
    @NSManaged var origin: String            // "Ethiopia"
    @NSManaged var region: String            // "Yirgacheffe"
    @NSManaged var processing: String        // "Washed"
    @NSManaged var roastLevel: String        // "Light", "Medium", "Dark"
    @NSManaged var flavorFeeling: String     // "Soft, floral, disappears clean."
    @NSManaged var characterTag: String      // "The quietest bean in the lineup"
    @NSManaged var pairingNote: String       // "Gentle extraction unlocks the florals..."
    @NSManaged var originStory: String       // 1-2 sentence origin story
    @NSManaged var originHeadline: String    // "Where coffee began."
    @NSManaged var originOneLiner: String    // "The birthplace. The original delicacy."
    @NSManaged var sensoryCue: String        // "bergamot, jasmine, raw honey"
    @NSManaged var profileKeys: String       // comma-separated: "tide_pool,first_light"
    @NSManaged var altitude: String          // "1,700-2,200 masl"
    @NSManaged var harvest: String           // "Oct-Jan"
    @NSManaged var sortOrder: Int16          // display order within profile
    @NSManaged var createdAt: Date
}
```

### Complete Bean Database — Seed Data (11 Beans)

#### Tide Pool Beans (Gentle / Low-Intensity / Recovery)

**Bean 1: Ethiopia Yirgacheffe — Washed**
```swift
BeanSeed(
    name: "Ethiopia Yirgacheffe",
    origin: "Ethiopia",
    region: "Yirgacheffe",
    processing: "Washed",
    roastLevel: "Light",
    flavorFeeling: "Soft, floral, disappears clean. Like opening a window in a quiet room.",
    characterTag: "The quietest bean in the lineup",
    pairingNote: "Gentle cold extraction at low dose unlocks the florals without any bite. This is what recovery tastes like.",
    originStory: "Yirgacheffe is where coffee began — not as a commodity, but as a cherry growing wild in Ethiopian highland forests. Every washed lot from here carries that original delicacy. The most unhurried coffee on earth.",
    originHeadline: "Where coffee began.",
    originOneLiner: "The birthplace. The original delicacy.",
    sensoryCue: "bergamot, jasmine, raw honey",
    altitude: "1,700-2,200 masl",
    harvest: "Oct-Jan",
    profileKeys: "tide_pool",
    sortOrder: 1
)
```

**Bean 2: Colombia Huila — Washed**
```swift
BeanSeed(
    name: "Colombia Huila",
    origin: "Colombia",
    region: "Huila",
    processing: "Washed",
    roastLevel: "Medium",
    flavorFeeling: "Warm and round. Nothing sharp. Nothing loud. Just there.",
    characterTag: "Warm and round. No sharp edges.",
    pairingNote: "Huila's natural sweetness comes through cleanly at Tide Pool's low concentration. Comfort without heaviness.",
    originStory: "Huila sits in southern Colombia where volcanic soil and high altitude conspire to produce coffee that is impossibly balanced. Farmers here have been growing for generations. The patience shows.",
    originHeadline: "Volcanic patience.",
    originOneLiner: "Volcanic soil. Generational patience.",
    sensoryCue: "panela, tangerine, milk chocolate",
    altitude: "1,250-2,000 masl",
    harvest: "Apr-Jul, Oct-Jan",
    profileKeys: "tide_pool,first_light",
    sortOrder: 2
)
```

**Bean 3: Honduras Marcala — Washed**
```swift
BeanSeed(
    name: "Honduras Marcala",
    origin: "Honduras",
    region: "Marcala",
    processing: "Washed",
    roastLevel: "Medium",
    flavorFeeling: "Brown sugar in cool water. Uncomplicated. The coffee equivalent of a deep breath.",
    characterTag: "Uncomplicated on purpose",
    pairingNote: "Marcala's simplicity is its strength at low extraction — naturally sweet, no rough edges, nothing to process.",
    originStory: "Marcala was Honduras's first designated origin — a small mountain town that proved Central American coffee could be gentle and exceptional at the same time. Quiet excellence.",
    originHeadline: "Quiet excellence.",
    originOneLiner: "Central America's gentle benchmark.",
    sensoryCue: "brown sugar, mild stone fruit, velvet body",
    altitude: "1,300-1,700 masl",
    harvest: "Nov-Mar",
    profileKeys: "tide_pool",
    sortOrder: 3
)
```

#### First Light Beans (Balance / Everyday / Steady)

**Bean 1: Guatemala Antigua — Washed**
```swift
BeanSeed(
    name: "Guatemala Antigua",
    origin: "Guatemala",
    region: "Antigua",
    processing: "Washed",
    roastLevel: "Medium",
    flavorFeeling: "Structured and clean. Nothing missing. Nothing extra. A well-made thing.",
    characterTag: "The benchmark. Century-tested.",
    pairingNote: "Antigua's balance survives any extraction method. At First Light parameters, it's the definition of reliable — the cup you'd build a morning around.",
    originStory: "Antigua Valley sits between three volcanoes. The ash in the soil gives the coffee a mineral backbone that most origins can't touch. Guatemala's benchmark for a century. Still is.",
    originHeadline: "Between three volcanoes.",
    originOneLiner: "Three volcanoes. One mineral backbone.",
    sensoryCue: "dark chocolate, walnut, clean finish",
    altitude: "1,500-1,700 masl",
    harvest: "Dec-Mar",
    profileKeys: "first_light,deep_work",
    sortOrder: 1
)
```

**Bean 2: Brazil Cerrado — Natural**
```swift
BeanSeed(
    name: "Brazil Cerrado",
    origin: "Brazil",
    region: "Cerrado Mineiro",
    processing: "Natural",
    roastLevel: "Medium",
    flavorFeeling: "Familiar and grounding. The taste of not needing to think about what you're drinking.",
    characterTag: "Reliable. Every single time.",
    pairingNote: "Natural processing gives Cerrado a sweetness that makes First Light's moderate extraction feel indulgent without being heavy. Effortless quality.",
    originStory: "Cerrado Mineiro is Brazil's savanna coffee country — flat, hot, mechanized. What it lacks in romance it makes up in consistency. The backbone of the world's coffee supply, and the good lots are genuinely great.",
    originHeadline: "The backbone.",
    originOneLiner: "The world's backbone, done right.",
    sensoryCue: "toasted almond, caramel, cocoa nib",
    altitude: "800-1,300 masl",
    harvest: "May-Sep",
    profileKeys: "first_light",
    sortOrder: 2
)
```

**Bean 3: Costa Rica Tarrazu — Honey**
```swift
BeanSeed(
    name: "Costa Rica Tarrazu",
    origin: "Costa Rica",
    region: "Tarrazu",
    processing: "Honey",
    roastLevel: "Medium",
    flavorFeeling: "Bright enough to notice, smooth enough to forget. A small daily pleasure.",
    characterTag: "Craft you don't have to think about",
    pairingNote: "Honey processing adds a layer of body that First Light's clean extraction makes transparent. You taste the craft without having to think about it.",
    originStory: "Tarrazu is Costa Rica's highest-altitude growing region. Farmers there treat honey processing like fine art — partially removing the cherry mucilage to control sweetness at the molecular level. Obsessive. Worth it.",
    originHeadline: "Obsessive craft.",
    originOneLiner: "Honey-processed at molecular precision.",
    sensoryCue: "apricot, honey, toasted grain",
    altitude: "1,200-1,900 masl",
    harvest: "Nov-Mar",
    profileKeys: "first_light",
    sortOrder: 3
)
```

#### Deep Work Beans (Focus / Sustained / Structured)

**Bean 1: Guatemala Antigua — Washed**
(Shared with First Light — `profileKeys: "first_light,deep_work"`. See above.)

**Bean 2: Ethiopia Sidama — Natural**
```swift
BeanSeed(
    name: "Ethiopia Sidama",
    origin: "Ethiopia",
    region: "Sidama",
    processing: "Natural",
    roastLevel: "Medium",
    flavorFeeling: "Layered and evolving. Changes every sip. The cup that keeps your palate working.",
    characterTag: "Changes every sip. Keeps you there.",
    pairingNote: "Deep Work's extended steep pulls layers out of Sidama that keep your palate working for hours. The complexity is the point.",
    originStory: "Sidama is Ethiopia's most productive coffee zone but the natural lots — dried in the cherry under open sun — are anything but industrial. Wild, fruity, polarizing. The good ones are transcendent.",
    originHeadline: "Wild, fruity, polarizing.",
    originOneLiner: "Sun-dried. Wild. Transcendent when it works.",
    sensoryCue: "blueberry, dark chocolate, wine finish",
    altitude: "1,500-2,200 masl",
    harvest: "Oct-Jan",
    profileKeys: "deep_work",
    sortOrder: 2
)
```

**Bean 3: Sumatra Lintong — Wet-Hulled**
```swift
BeanSeed(
    name: "Sumatra Lintong",
    origin: "Indonesia",
    region: "Lintong",
    processing: "Wet-Hulled",
    roastLevel: "Medium-Dark",
    flavorFeeling: "Heavy and anchoring. Stays with you for hours. A working coffee, not a sipping one.",
    characterTag: "Stays in the room for hours",
    pairingNote: "Sumatra's earthy body at Deep Work's dose creates something dense and sustaining. Not a sipping coffee — a working coffee. Stays with you through the session.",
    originStory: "Lintong sits on the edge of Lake Toba in northern Sumatra. Wet-hulling strips the parchment early and dries the green bean in humid air. It produces a cup that tastes like no other origin on earth. You either get it or you don't.",
    originHeadline: "Like no other origin on earth.",
    originOneLiner: "Wet-hulled on the edge of Lake Toba.",
    sensoryCue: "cedar, dark cocoa, tobacco, earthy sweetness",
    altitude: "1,100-1,600 masl",
    harvest: "Year-round (primary: Jun-Dec)",
    profileKeys: "deep_work",
    sortOrder: 3
)
```

#### Voltage Beans (Performance / Peak / Intensity)

**Bean 1: Kenya AA — Washed**
```swift
BeanSeed(
    name: "Kenya AA",
    origin: "Kenya",
    region: "Nyeri / Kirinyaga",
    processing: "Washed",
    roastLevel: "Medium",
    flavorFeeling: "Electric. Hits the front of your tongue and stays. Cold water on your face, in a cup.",
    characterTag: "The sharpest thing Brezi pours",
    pairingNote: "Kenya's phosphoric acidity at Voltage's high concentration is a controlled detonation. Bright, complex, demanding — the brew that matches your best morning.",
    originStory: "Kenya grades its coffee by bean size — AA is the largest, the densest, the most sought-after. Nyeri and Kirinyaga produce lots with an acidity so clean it borders on electric. Specialty coffee's performance benchmark.",
    originHeadline: "Specialty's performance benchmark.",
    originOneLiner: "The densest bean. The cleanest acidity.",
    sensoryCue: "blackcurrant, grapefruit, sparkling finish",
    altitude: "1,400-2,000 masl",
    harvest: "Oct-Dec (main), Jun-Aug (fly)",
    profileKeys: "voltage",
    sortOrder: 1
)
```

**Bean 2: Ethiopia Guji — Natural**
```swift
BeanSeed(
    name: "Ethiopia Guji",
    origin: "Ethiopia",
    region: "Guji",
    processing: "Natural",
    roastLevel: "Light-Medium",
    flavorFeeling: "Explosive fruit. Not background noise. The cup that makes you stop what you're doing.",
    characterTag: "Fruit-forward. Full volume.",
    pairingNote: "Natural Guji at Voltage parameters is almost too much — in the best way. The fruit intensity matches the caffeine intensity. Full signal, full flavor.",
    originStory: "Guji split from Sidama as its own designated origin because the coffee was simply too different to share a name. Wild-grown, sun-dried, often processed by single families on raised beds. Every lot is a fingerprint.",
    originHeadline: "Every lot is a fingerprint.",
    originOneLiner: "Sun-dried by single families. Every lot unique.",
    sensoryCue: "strawberry, tropical fruit, dark honey, electric finish",
    altitude: "1,800-2,300 masl",
    harvest: "Oct-Jan",
    profileKeys: "voltage",
    sortOrder: 2
)
```

**Bean 3: Rwanda Nyamasheke — Washed**
```swift
BeanSeed(
    name: "Rwanda Nyamasheke",
    origin: "Rwanda",
    region: "Nyamasheke",
    processing: "Washed",
    roastLevel: "Medium",
    flavorFeeling: "Bright and precise. Every note in its place. Makes you sit up straighter.",
    characterTag: "Juicy, precise, earned",
    pairingNote: "Rwandan lots bring a juicy complexity that Voltage's parameters amplify rather than overwhelm. Concentration meets clarity.",
    originStory: "Rwanda's coffee industry was rebuilt from nothing after 1994. Twenty years later, Nyamasheke washing stations are producing some of East Africa's most refined lots. A precision that feels earned.",
    originHeadline: "Precision that feels earned.",
    originOneLiner: "Rebuilt from nothing. Now, East Africa's most refined.",
    sensoryCue: "red apple, lemon zest, black tea, crisp body",
    altitude: "1,500-2,000 masl",
    harvest: "Mar-Jul",
    profileKeys: "voltage",
    sortOrder: 3
)
```

#### The Blank — No Default Beans

```swift
let theBlank_beanNote = "Your brew, your bean. The Blank runs whatever you load."
```

### Bean Rotation Logic — `InsightEngine.swift` [NEW in R4]

**Purpose:** When a user rates "Flat" twice on the same bean, demote that bean in display order and promote the next one. Keeps the promise: "We'll rotate the origin next time."

```swift
// InsightEngine.swift extension
func rotateBeanForProfile(profileKey: String, flatBeanName: String) {
    let request: NSFetchRequest<BeanProfile> = BeanProfile.fetchRequest()
    request.predicate = NSPredicate(format: "profileKeys CONTAINS %@", profileKey)
    request.sortDescriptors = [NSSortDescriptor(keyPath: \BeanProfile.sortOrder, ascending: true)]

    guard let beans = try? context.fetch(request), beans.count > 1 else { return }

    // Find the offending bean
    guard let flatBean = beans.first(where: { $0.name == flatBeanName }) else { return }

    // Move it to last position, promote others
    let maxOrder = beans.map { $0.sortOrder }.max() ?? Int16(beans.count)
    flatBean.sortOrder = maxOrder + 1

    try? context.save()
}
```

**Trigger:** When `emotionalCount(profileKey, "flat") >= 2` AND the last two "Flat" ratings share the same `beanName`, call `rotateBeanForProfile`. The bean drops to last in display order. The next bean in the list gets promoted to position 1.

**Edge case:** If all three beans for a profile get demoted (all rated "Flat" twice), the sort order wraps — first demoted bean returns to top. At that point, the user has a profile mismatch, not a bean problem. That's a v2 insight.

```swift
func checkBeanRotation(profileKey: String) {
    let request: NSFetchRequest<BrewLog> = BrewLog.fetchRequest()
    request.predicate = NSPredicate(format: "profileKey == %@ AND emotionalRating == %@", profileKey, "flat")
    request.sortDescriptors = [NSSortDescriptor(keyPath: \BrewLog.brewedAt, ascending: false)]
    request.fetchLimit = 2

    guard let results = try? context.fetch(request),
          results.count >= 2,
          let bean1 = results[0].beanName,
          let bean2 = results[1].beanName,
          bean1 == bean2 else { return }

    rotateBeanForProfile(profileKey: profileKey, flatBeanName: bean1)
}
```

### Bean Card UI — `BeanCardView.swift`

Appears within `BrewRecommendationView` (detail screen), below brew parameters, above "Why This Brew" chain.

```
|                                 |
|  RECOMMENDED BEANS              |  Mono, 11px, #888888, ALL CAPS
|                                 |  bottom padding: 12pt
|  +-----------------------------+|  BeanCardView.swift
|  |                             ||
|  |  Ethiopia Yirgacheffe       ||  CondensedBold, 20px, #FFFFFF
|  |  Washed . Light             ||  Mono, 12px, #888888
|  |                             ||  bottom padding: 8pt (sm)
|  |  "Soft, floral, disappears  ||  Regular, 15px, #CCCCCC
|  |   clean. Like opening a     ||  line-height: 1.5
|  |   window in a quiet room."  ||  italic style
|  |                             ||  bottom padding: 8pt (sm)
|  |  bergamot . jasmine .       ||  Mono, 11px, #666666
|  |  raw honey                  ||  (traditional flavor notes, subdued)
|  |                             ||  bottom padding: 12pt
|  |  The quietest bean in       ||  Mono, 12px, #488BED
|  |  the lineup                 ||
|  |                             ||
|  +-----------------------------+|  bg: #1A1A1A, radius: 16pt
|  gap: 8pt (sm)                  |  padding: 20pt inner
|  +-----------------------------+|
|  |  Colombia Huila             ||  BeanCardView.swift (card 2)
|  |  Washed . Medium            ||
|  |                             ||
|  |  "Warm and round. Nothing   ||
|  |   sharp. Nothing loud.      ||
|  |   Just there."              ||
|  |                             ||
|  |  panela . tangerine .       ||
|  |  milk chocolate             ||
|  |                             ||
|  |  Warm and round. No sharp   ||
|  |  edges.                     ||
|  |                             ||
|  +-----------------------------+|
|                                 |
|  Origin story >                 |  Regular, 14px, #888888, 44x44pt
|                                 |  taps open OriginStorySheet
```

**BeanCardView.swift spec:**
- Background: `#1A1A1A`
- Radius: 16pt
- Padding: 20pt all sides
- Bean name: CondensedBold, 20px, #FFFFFF
- Processing + roast: Mono, 12px, #888888
- Flavor feeling: Regular, 15px, #CCCCCC, italic (`Font.system(size: 15, weight: .regular).italic()`)
- Sensory cue: Mono, 11px, #666666 — traditional tasting notes, deliberately subdued
- Character tag: Mono, 12px, #488BED — describes the coffee's personality
- First card in list has left border: 3px, profile gradient start color
- Subsequent cards: no border accent
- Max 3 cards shown. Vertical stack, not horizontal swipe.

**Animation on scroll into view:**
- Cards appear sequentially: fadeIn + translateY(12->0), 0.3s each, easeOut, 0.15s delay between

### "Origin story >" — `OriginStorySheet.swift`

Half-sheet modal (`.presentationDetents([.medium])`). Shows the origin story for the top bean.

```
+-----------------------------------------+
|                                         |  bg: #0F0F0F
|  -----                                  |  sheet handle: #333333, centered
|                                         |
|  YIRGACHEFFE, ETHIOPIA                  |  Mono, 11px, #888888, ALL CAPS
|                                         |  bottom padding: 12pt
|  Where coffee began.                    |  CondensedBold, 28px, #FFFFFF
|                                         |  bottom padding: 16pt (md)
|  Yirgacheffe is where coffee            |  Regular, 16px, #CCCCCC
|  began — not as a commodity,            |  line-height: 1.6
|  but as a cherry growing wild           |
|  in Ethiopian highland forests.         |
|  Every washed lot from here             |
|  carries that original                  |
|  delicacy. The most unhurried           |
|  coffee on earth.                       |
|                                         |  bottom padding: 24pt (lg)
|  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░         |  profile gradient strip (same as card)
|                                         |  3pt height, full width minus padding
|  Processing: Washed                     |  Mono, 12px, #666666
|  Altitude: 1,700-2,200 masl            |  Mono, 12px, #666666
|  Harvest: Oct-Jan                       |  Mono, 12px, #666666
|                                         |
|  -----------------------------------    |  divider: 1px #222222
|                                         |  (only renders if BrewLog contains
|  Brewed 3 times . Last: Mar 14          |  matching beanName entries)
|  Usually: Hit right                     |  Mono, 12px, #488BED + #666666
|                                         |
|  Your Yirgacheffe. At this point,       |  (only renders if count >= 5)
|  basically.                             |  Mono, 12px, #888888, italic
|                                         |
+-----------------------------------------+
```

**Sheet spec:**
- Sheet handle: 36pt wide, 5pt tall, #333333, centered, 8pt from top
- Region + origin: Mono, 11px, #888888, ALL CAPS
- Headline: CondensedBold, 28px, #FFFFFF
- Body: Regular, 16px, #CCCCCC, line-height 1.6
- Gradient strip: 3pt height, full width minus 40pt padding, radius 1.5pt
- Metadata: Mono, 12px, #666666 — 3 lines (Processing, Altitude, Harvest)
- Background: #0F0F0F
- Padding: 20pt horizontal, 24pt top below handle

### Brew-History Badge — `OriginStorySheet.swift`

**Conditional section below metadata:**

```
|  -----------------------------------    |  divider: 1px #222222
|                                         |
|  Brewed 3 times . Last: Mar 14          |  Mono, 12px, #488BED
|  Usually: Hit right                     |  Mono, 12px, #666666
|                                         |
```

**Spec:**
- Divider: 1px #222222, full width minus padding
- Top padding above divider: 16pt (md)
- "Brewed X times . Last: [date]" — Mono, 12px, #488BED
- "Usually: [most common emotional rating]" — Mono, 12px, #666666
- Date format: "Mar 14" (abbreviated month + day)
- Only renders if `BrewLog` contains 1+ entries where `beanName` matches the displayed bean
- If no matching brews: this entire section doesn't render. Sheet looks like a first-time introduction.

**Data query:**
```swift
func brewHistoryForBean(name: String) -> (count: Int, lastDate: Date, usualRating: String)? {
    let request: NSFetchRequest<BrewLog> = BrewLog.fetchRequest()
    request.predicate = NSPredicate(format: "beanName == %@", name)
    request.sortDescriptors = [NSSortDescriptor(keyPath: \BrewLog.brewedAt, ascending: false)]

    guard let results = try? context.fetch(request), !results.isEmpty else { return nil }

    let count = results.count
    let lastDate = results.first!.brewedAt ?? results.first!.createdAt

    // Most common emotional rating
    let emotionalRatings = results.compactMap(\.emotionalRating).filter { $0 != "pending" }
    let usualRating: String
    if let mostCommon = emotionalRatings.mostFrequent() {
        usualRating = emotionalRatingLabel(mostCommon)  // "Hit right", "Needed more", etc.
    } else {
        // Fall back to star average if no emotional ratings
        let rated = results.filter { $0.rating > 0 }
        if !rated.isEmpty {
            let avg = rated.map { Int($0.rating) }.reduce(0, +) / rated.count
            usualRating = "\(avg)/5 stars"
        } else {
            usualRating = "Not yet rated"
        }
    }

    return (count, lastDate, usualRating)
}
```

### "Your Yirgacheffe" Relationship Line [NEW in R4]

**Purpose:** After 5+ brews with the same bean, the origin story sheet acknowledges the relationship has graduated from recommendation to preference. The bean is yours now.

**Conditional section below brew-history badge:**

```swift
// OriginStorySheet.swift — below brew-history badge, conditional
if let history = brewHistoryForBean(name: bean.name), history.count >= 5 {
    // "Your {region}. At this point, basically."
    // Mono, 12px, #888888, italic
    // top padding: 4pt, no additional container
    let shortName = bean.region  // "Yirgacheffe", "Huila", "Antigua", etc.
    Text("Your \(shortName). At this point, basically.")
        .font(.system(size: 12, design: .monospaced))
        .italic()
        .foregroundColor(Color(hex: "#888888"))
        .padding(.top, 4)
}
```

Uses `bean.region` as the short name because that's how coffee people talk. Nobody says "my Ethiopia Yirgacheffe." They say "my Yirgacheffe." The region name is the relationship name.

---

## 3. "Why Today" Transitional Screen — `WhyTodayView.swift`

### Purpose

The moment where a number becomes a feeling. This is where the app says: *I see your state. Here's what that means.* Then advances to the dashboard where the coffee responds.

Not a navigable screen. A **transitional moment** — appears once per day, on the first dashboard load with fresh data. No back button. Advances forward.

### Trigger
- After onboarding Screen 5 -> transition (replaces "Reading your signal..." screen)
- After morning check-in refresh (pull-to-refresh with new day's data, first time only)
- Store: `lastWhyTodayDate: String` in UserDefaults (format: "2026-03-19"). If today's date matches, skip.
- **Suppression logic:** See Section 3a below.

### Layout

Full-screen overlay. Background: #000000 + profile gradient at 0.08 opacity. Auto-advances after all text appears, OR tap anywhere to advance immediately.

```
+-----------------------------------------+
|                                         |  bg: #000000 + gradient tint
|                                         |
|                                         |
|                                         |
|                                         |
|  Rebuilding.                            |  Phase 1: TIER WORD
|                                         |  CondensedBold, 36px, #FFFFFF
|                                         |  + haptic: UIImpactFeedbackGenerator(.soft)
|                                         |
|  61% recovery. HRV dipped               |  Phase 1b: DATA LINE
|  overnight.                             |  Mono, 14px, #888888
|                                         |
|                                         |  [pause 1.2s]
|                                         |
|  Nervous system's working hard.         |  Phase 2: NEED
|  Don't add to its job.                  |  Regular, 17px, #CCCCCC
|                                         |  line-height: 1.5
|                                         |
|                                         |  [pause 1.0s]
|                                         |
|  This is what meets                     |  Phase 3: BRIDGE
|  you there.                             |  Regular, 15px, #888888
|                                         |
|                  ->                     |  SF Symbol: arrow.right
|                                         |  #488BED, 16pt, pulse opacity 1.0<->0.5
|                                         |
|                                         |
|                                         |
+-----------------------------------------+
```

### Phase Animation Sequence

| Phase | Content | Animation | Duration | Delay |
|---|---|---|---|---|
| 1a | Tier word (e.g. "Rebuilding.") | fadeIn + translateY(8->0) | 0.3s easeOut | 0.2s |
| -- | Haptic | `UIImpactFeedbackGenerator(style: .soft).impactOccurred()` | instant | 0.2s |
| 1b | Data line (e.g. "61% recovery...") | fadeIn | 0.2s easeOut | 0.6s |
| -- | pause | -- | -- | 1.2s |
| 2 | Need text | fadeIn | 0.4s easeOut | 2.0s |
| -- | pause | -- | -- | 1.0s |
| 3 | Bridge text + arrow | fadeIn | 0.3s easeOut | 3.4s |
| -- | arrow pulse begins | opacity 1.0<->0.5, 1.2s, repeat | -- | 3.7s |
| -- | auto-advance to dashboard | crossfade | 0.5s easeOut | 5.5s |

**Total sequence: ~6 seconds.** Tap anywhere at any phase -> snap all text to visible, advance to dashboard immediately (0.3s crossfade). Never trap an impatient user.

**Shortened reintroduction sequence (after suppression — see 3a):**

| Phase | Content | Animation | Duration | Delay |
|---|---|---|---|---|
| 1a | Tier word | fadeIn + translateY(8->0) | 0.3s easeOut | 0.1s |
| -- | Haptic | same | instant | 0.1s |
| 1b | Data line | fadeIn | 0.2s easeOut | 0.4s |
| -- | pause | -- | -- | 0.6s |
| 2 | Need text | fadeIn | 0.3s easeOut | 1.2s |
| -- | pause | -- | -- | 0.5s |
| 3 | Bridge text + arrow | fadeIn | 0.2s easeOut | 2.0s |
| -- | auto-advance | crossfade | 0.5s easeOut | 3.5s |

**Shortened total: ~3.5 seconds.** Same content, tighter timing. The user already knows the format.

### Haptic Spec

- Trigger: simultaneous with tier word animation (Phase 1a)
- Generator: `UIImpactFeedbackGenerator(style: .soft)` — the lightest available
- Call `prepare()` on view appear, `impactOccurred()` at Phase 1a delay
- Fires once. Never repeats during the sequence.
- Respects system haptic settings (users who've disabled haptics don't feel it)

### Copy — Complete Set

#### Tier Words (Phase 1a)

```swift
let whyToday_tierWord: [String: String] = [
    "high": "Peaked.",
    "mid":  "Steady.",
    "low":  "Rebuilding."
]
```

#### Data Lines (Phase 1b)

```swift
func whyToday_dataLine(source: String, recoveryPercent: Double?, recoveryScore: Int, hrv: Double?, baselineHrv: Double?, sleepHours: Double?) -> String {
    var parts: [String] = []

    // Recovery
    if let pct = recoveryPercent, source != "manual" {
        parts.append("\(Int(pct))% recovery")
    } else {
        parts.append("Checked in at \(recoveryScore)/10")
    }

    // HRV delta (wearable only)
    if let hrv = hrv, let baseline = baselineHrv, baseline > 0 {
        let delta = ((hrv - baseline) / baseline) * 100
        if abs(delta) > 10 {
            let dir = delta > 0 ? "up" : "down"
            parts.append("HRV \(abs(Int(delta)))% \(dir) from baseline")
        }
    }

    // Sleep note (wearable only)
    if let sleep = sleepHours, sleep > 0 {
        if sleep < 5.5 {
            parts.append("Short night — \(String(format: "%.0f", sleep))h")
        } else if sleep > 8.5 {
            parts.append("\(String(format: "%.0f", sleep))h sleep")
        }
    }

    return parts.joined(separator: ". ") + "."
}
```

**Examples:**
- Wearable, notable data: `"61% recovery. HRV 18% down from baseline."`
- Wearable, normal: `"78% recovery."`
- Wearable, short sleep: `"61% recovery. Short night — 5h."`
- Manual: `"Checked in at 5/10."`

#### Need Text (Phase 2) — 3 Variants Per Tier, Rotating

```swift
let whyToday_needText: [String: [String]] = [
    "high": [
        "Clean night. Full signal. Go.",
        "System's fully online. This is the morning to use what you've got.",
        "Everything recovered. The question isn't whether — it's how far."
    ],
    "mid": [
        "Not sharp, not broken. Just a normal morning doing normal morning things.",
        "Rough edges from last night. Functional, not firing. Steady input.",
        "System's running. Not surging. Steady input, steady output."
    ],
    "low": [
        "Nervous system's working hard. Don't add to its job.",
        "Yesterday cost you. Low signal today. That's the data.",
        "Low signal. Your body's recovering. It does this."
    ]
]
```

**Rotation logic:**
```swift
func selectWhyTodayNeed(tier: String) -> String {
    let variants = whyToday_needText[tier]!
    let key = "lastWhyToday_\(tier)"
    let lastIndex = UserDefaults.standard.integer(forKey: key)
    let nextIndex = (lastIndex + 1) % variants.count
    UserDefaults.standard.set(nextIndex, forKey: key)
    return variants[nextIndex]
}
```

#### Bridge Text (Phase 3)

```swift
let whyToday_bridge = "This is what meets you there."
```

### Surprise Moment — Non-Coffee Variant

Once every 10-15 sessions, the bridge text is replaced with a line that acknowledges the human without selling them anything. No brew reference. No transition language. Just noticing.

**Trigger logic:**
```swift
func shouldShowSurpriseMoment() -> Bool {
    let count = UserDefaults.standard.integer(forKey: "whyTodaySessionCount")
    let newCount = count + 1
    UserDefaults.standard.set(newCount, forKey: "whyTodaySessionCount")

    // First surprise at session 12, then every 10-15 (randomized)
    let lastSurprise = UserDefaults.standard.integer(forKey: "lastSurpriseSession")
    let threshold = lastSurprise + Int.random(in: 10...15)
    if newCount >= threshold && newCount >= 12 {
        UserDefaults.standard.set(newCount, forKey: "lastSurpriseSession")
        return true
    }
    return false
}
```

**When triggered:** Phase 3 replaces `whyToday_bridge` with a surprise line. Everything else stays the same — tier word, data line, need text still play. The surprise replaces only the bridge.

**Surprise copy pool (8 variants, selected randomly):**

```swift
let surpriseLines: [String] = [
    "You showed up. That's the whole thing.",
    "Whatever you did last night — noted.",
    "\(streakDays) days straight. We see that.",
    "Same time tomorrow.",
    "Brew. Then whatever today is.",
    "Your morning. Your data. Your call. Always.",
    "Not every day needs to be a peak day. This one counts too.",
    "The routine is the thing. You already know that."
]
```

**Note on `\(streakDays)` variant:** Only selectable if streak >= 7. Otherwise, random selection from the remaining 7.

**After surprise:** Dashboard advances normally. No special state. The surprise is ephemeral — it exists for five seconds and then it's gone. That's why it works.

### Wearable vs. Manual Variants

**Wearable user:** Data line uses recovery percentage and can include HRV delta or sleep note.
**Manual user:** Data line uses recovery score ("Checked in at 5/10."). No HRV or sleep append.

Both see the same tier word, need text, and bridge. The data line is the only difference.

### 3a. Why Today — Smart Suppression [NEW in R4]

**Problem:** Why Today is the emotional centerpiece, but it's also a six-second gate between the user and their brew button. A user who taps to skip three days in a row is telling you they don't want this screen right now. Continuing to show it is the app prioritizing its own emotional arc over the user's morning.

**But:** Week 1 skippers are impatient, not uninterested. They're learning the app. By Week 3, when the data is personal, "Rebuilding." on a bad morning hits different. Permanent suppression after Week 1 kills the discovery story before it starts.

**UserDefaults keys:**
```swift
"consecutiveWhyTodaySkips": Int      // resets to 0 on any non-skip
"whyTodaySuppressedUntil": String    // date string, "" = not suppressed
"whyTodayReintroduced": Bool         // true after first reintroduction
```

**Skip definition:** The user taps to advance before Phase 2 (need text) appears. If the user watches through Phase 2 (even if they tap before auto-advance), that's a watch, not a skip. Auto-advance (full 5.5s elapsed) is always a watch.

**Suppression logic:**
```swift
func shouldShowWhyToday() -> Bool {
    let today = todayDateString()

    // Already shown today
    if UserDefaults.standard.string(forKey: "lastWhyTodayDate") == today { return false }

    // Check suppression
    let suppressedUntil = UserDefaults.standard.string(forKey: "whyTodaySuppressedUntil") ?? ""
    if !suppressedUntil.isEmpty && suppressedUntil > today { return false }

    // Suppression expired — clear it
    if !suppressedUntil.isEmpty {
        UserDefaults.standard.set("", forKey: "whyTodaySuppressedUntil")
    }

    return true
}

func recordWhyTodayInteraction(wasSkip: Bool) {
    if wasSkip {
        let consecutive = UserDefaults.standard.integer(forKey: "consecutiveWhyTodaySkips") + 1
        UserDefaults.standard.set(consecutive, forKey: "consecutiveWhyTodaySkips")

        let reintroduced = UserDefaults.standard.bool(forKey: "whyTodayReintroduced")

        if consecutive >= 3 && !reintroduced {
            // First suppression: 7 days
            let suppressDate = Calendar.current.date(byAdding: .day, value: 7, to: Date())!
            UserDefaults.standard.set(dateString(suppressDate), forKey: "whyTodaySuppressedUntil")
            UserDefaults.standard.set(true, forKey: "whyTodayReintroduced")
            UserDefaults.standard.set(0, forKey: "consecutiveWhyTodaySkips")
        } else if consecutive >= 1 && reintroduced {
            // Post-reintroduction skip: 30 days
            let suppressDate = Calendar.current.date(byAdding: .day, value: 30, to: Date())!
            UserDefaults.standard.set(dateString(suppressDate), forKey: "whyTodaySuppressedUntil")
            UserDefaults.standard.set(0, forKey: "consecutiveWhyTodaySkips")
            // After 30-day reintroduction, if they skip again: permanent
            // (30-day suppress + another skip = set suppression to "9999-12-31")
        }
    } else {
        // User watched — reset everything
        UserDefaults.standard.set(0, forKey: "consecutiveWhyTodaySkips")
        UserDefaults.standard.set(false, forKey: "whyTodayReintroduced")
    }
}
```

**Full sequence:**
1. Days 1-N: User skips three times in a row -> suppress 7 days
2. Day N+7: Show Why Today once with shortened sequence (3.5s instead of 5.5s, tighter delays)
3. If user watches the reintroduction (auto-advance or waits past Phase 2 before tapping) -> resume normal daily cadence, reset all counters
4. If user skips the reintroduction -> suppress for 30 days
5. Day N+37: Show Why Today once more with shortened sequence
6. If user watches -> resume. If user skips -> suppress permanently (`"9999-12-31"`)

**Reintroduction uses shortened animation timing** — the user already knows the format. They don't need the full 5.5 seconds. Shorter sequence = less friction = higher chance they watch = higher chance they discover the value they missed.

**Why not Elon's three-strikes-and-out:** The users who skip early and discover value later are the ones who tell friends about the app. "I didn't get it at first, but now it's my favorite part of the morning" is the retention story that compounds. One extra chance with better pacing. If they still skip, they genuinely don't want it.

---

## 4. Flavor Preview — `FlavorPreviewView.swift`

### Purpose

Before brewing, let the user feel what they're about to drink. First sentence = the taste. Everything after = the texture for people who want it.

### Two Placements

1. **Dashboard Card 3** — compact "This cup is:" strip (defined in Section 1)
2. **Brew Recommendation Detail** — expanded flavor preview below brew parameters, above bean cards

### Expanded Flavor Preview (Detail Screen)

```
|                                 |
|  +-----------------------------+|  FlavorPreviewView.swift
|  |                             ||
|  |  ░░░░░░░░░░░░░░░░░░░░░░░░  ||  gradient background tint
|  |                             ||  profile gradient at 0.06 opacity
|  |  WHAT YOU'LL TASTE          ||  Mono, 11px, #888888, ALL CAPS
|  |                             ||  bottom padding: 12pt
|  |  Jasmine and bergamot up    ||  Regular, 16px, #FFFFFF
|  |  front, then something      ||  line-height: 1.6
|  |  sweet and round underneath.||
|  |  Low acidity. No edge. Clean||
|  |  finish that clears fast.   ||
|  |  The second cup happens     ||
|  |  without deciding.          ||
|  |                             ||  bottom padding: 16pt (md)
|  |  This cup is:               ||  Mono, 11px, #888888
|  |  Soft. Grounding. Unhurried.||  CondensedBold, 18px, #FFFFFF
|  |                             ||
|  +-----------------------------+|  bg: #1A1A1A, radius: 16pt
|                                 |  padding: 20pt inner
```

**FlavorPreviewView.swift spec:**
- Background: `#1A1A1A`
- Gradient tint: first color from `profileGradients[profileKey]` at 0.06 opacity, top-leading to bottom-trailing
- Radius: 16pt
- Padding: 20pt all sides
- Section label ("WHAT YOU'LL TASTE"): Mono, 11px, #888888, ALL CAPS
- Description: Regular, 16px, #FFFFFF, line-height 1.6
- "This cup is:" label: Mono, 11px, #888888
- Feeling words: CondensedBold, 18px, #FFFFFF
- Words animation: sequential fadeIn, 0.15s each, 0.1s delay between, easeOut

### Sensory Descriptions — Complete Set (All 15 Combinations)

Every description front-loads the sensation. First sentence = what hits your tongue. Second sentence = the observation.

```swift
let sensoryCopy: [String: [String: String]] = [
    "tide_pool": [
        "high": "Naturally sweet, almost floral — honey dissolved in cool water. Low acidity, soft body, a finish that stops cleanly. You don't need intensity today and this cup doesn't pretend otherwise.",
        "mid":  "Jasmine and bergamot up front, then something sweet and round underneath. Low acidity. No edge. Clean finish that clears fast. The second cup happens without deciding.",
        "low":  "Thin and transparent — closer to tea than coffee. Floral notes you'd miss if you weren't still. Warmth without weight."
    ],
    "first_light": [
        "high": "Mild chocolate, a little nut, a finish that clears in seconds. Reliably good. The cup that doesn't need you to think about it to be worth drinking.",
        "mid":  "Mild sweetness up front, clean through the middle, a finish that's dry without being sharp. Balanced in the way that only matters when you're paying attention.",
        "low":  "Toasted grain, a little cocoa, a finish that doesn't linger. Nothing challenging. Just enough."
    ],
    "deep_work": [
        "high": "Chocolate and structure on the first sip. Walnut, maybe tobacco, on the second. Unfolds over hours. This cup isn't a moment — it's a session.",
        "mid":  "Dark chocolate dominance with a low, sustained sweetness underneath. Sits on your desk getting better as it warms. Doesn't distract. Just holds.",
        "low":  "Heavy body, dark flavors, no bright notes. Cedar and dark cocoa that sit on your tongue and stay. You'll taste this one an hour from now."
    ],
    "voltage": [
        "high": "Bright acidity hits first — grapefruit, blackcurrant — then a wave of concentrated fruit sweetness. Complex, demanding. Not background coffee.",
        "mid":  "Stone fruit and citrus up front, backed by a body heavier than you'd expect. The finish sparks. Not subtle. Not trying to be.",
        "low":  "Sharp acid hits first and doesn't back off. Heavy extraction, concentrated body, a finish that stays in your mouth for minutes. Not gentle."
    ],
    "the_blank": [
        "high": "You chose this. Every variable. The cup is exactly the one you designed — no algorithm smoothing the edges. If it's great, you did that.",
        "mid":  "Whatever your beans produce at these settings, unfiltered. No sweetness curve, no acidity adjustment, no algorithm between the grind and the cup. Raw signal.",
        "low":  "Full manual on a low day. The result is raw — whatever your beans and your settings produce, unmediated. That takes a specific kind of confidence."
    ]
]
```

---

## 5. Post-Brew Emotional Check-In — `EmotionalCheckInView.swift`

### Purpose

Replace clinical star rating with felt vocabulary. Stars remain as secondary — the primary check-in uses feeling words that feed back into the recommendation engine.

### Architecture

**New fields on BrewLog:**

```swift
@NSManaged var emotionalRating: String?  // "hit_right", "needed_more", "too_much", "surprising", "flat", "pending"
@NSManaged var beanName: String?         // e.g. "Ethiopia Yirgacheffe" — user's selected/logged bean
// BrewLog.rating (1-5 Int16) still exists — derived from emotionalRating, or set directly via stars
```

**Emotional -> Star Mapping:**

```swift
let emotionalToStars: [String: Int16] = [
    "hit_right":   4,
    "needed_more": 3,
    "too_much":    2,
    "surprising":  0,  // no star conversion — tracked as separate signal
    "flat":        2,
    "pending":     0   // deferred — not yet rated
]
```

**"Surprising" decoupled from stars [CHANGED in R4]:** "Surprising" gets no star mapping. The word has a wider emotional range than any other option — a user might mean "that was weird but interesting" (3-4 stars) or "that was transcendent" (5 stars). Flattening that to any single star rating pollutes the correlation data.

- `emotionalRating = "surprising"`, `rating = 0` (null equivalent)
- HistoryView shows the emotional label ("Surprising") instead of stars for these entries
- "Surprising" ratings increase a bean variety weighting counter (`surprising_{beanName}` in UserDefaults)
- When the user gets "Surprising" 2+ times on the same bean, that bean gets promoted in sort order — repeat surprise is genuine interest, not noise
- The star average in correlation charts is unaffected

**HistoryView rendering logic:**
```swift
if let emotional = brewLog.emotionalRating {
    if emotional == "surprising" {
        // Show label only, no stars
        showEmotionalLabel("Surprising")
    } else if emotional == "pending" {
        // Show pending indicator
        showEmotionalLabel("Rating pending")
    } else {
        // Show stars derived from emotional rating
        showStars(emotionalToStars[emotional] ?? 0)
        showEmotionalLabel(emotionalRatingLabel(emotional))
    }
} else if brewLog.rating > 0 {
    showStars(brewLog.rating)
}
```

Stars stored for correlation charts and history. Emotional rating is the primary input. "Surprising" and "pending" are separate signals that don't contaminate the star data.

### Updated Brew Completion Overlay

Replaces star picker as PRIMARY rating moment. Stars accessible via "Or rate with stars >" link. Six options including "Not yet."

```
+-----------------------------------------+
|                                         |
|                                         |
|         Brewed.                         |  CondensedBold, 48px, #FFFFFF
|                                         |  bottom padding: 16pt (md)
|         FIRST LIGHT                     |  Mono, 14px, #888888
|         7:14am . 12 min                 |  Mono, 14px, #888888
|                                         |  bottom padding: 32pt (xl)
|  How did that land?                     |  Regular, 17px, #CCCCCC
|                                         |  bottom padding: 16pt (md)
|  +-------------------------------------+|  EmotionalRatingOption.swift
|  |  * Hit right                        ||  option 1
|  |    Matched the morning.             ||
|  +-------------------------------------+|  gap: 8pt (sm)
|  +-------------------------------------+|
|  |  o Needed more                      ||  option 2
|  |    Wanted something stronger.       ||
|  +-------------------------------------+|  gap: 8pt (sm)
|  +-------------------------------------+|
|  |  o Too much today                   ||  option 3
|  |    Should've gone lighter.          ||
|  +-------------------------------------+|  gap: 8pt (sm)
|  +-------------------------------------+|
|  |  o Surprising                       ||  option 4
|  |    Didn't expect that.              ||
|  |    Good different.                  ||
|  +-------------------------------------+|  gap: 8pt (sm)
|  +-------------------------------------+|
|  |  o Flat                             ||  option 5
|  |    Nothing landed.                  ||
|  +-------------------------------------+|  gap: 8pt (sm)
|  +-------------------------------------+|
|  |  o Not yet                          ||  option 6 [NEW in R4]
|  |    Ask me in an hour.               ||
|  +-------------------------------------+|
|                                         |  bottom padding: 24pt (lg)
|  +-------------------------------------+|  blue filled (#488BED)
|  |  DONE  ->                           ||  DISABLED until selection
|  +-------------------------------------+|  CondensedBold, 16px, #FFFFFF, ALL CAPS
|                                         |  height: 48pt, opacity: 0.4 when disabled
|  Or rate with stars >                   |  Regular, 14px, #666666, centered, 44x44pt
|  Skip for now >                         |  Regular, 14px, #888888, centered, 44x44pt
|                                         |
+-----------------------------------------+
```

### EmotionalRatingOption.swift Spec

- Background: `#1A1A1A`
- Border: 1px `#333333`
- Radius: 12pt
- Padding: 16pt horizontal, 14pt vertical
- Min height: 52pt (ensures 44pt tap target)
- Full-width tap target

**States:**
- Default: border `#333333`, radio circle (o) border `#333333` 1.5px, no fill
- Selected: border 2px `#488BED`, background `rgba(72, 139, 237, 0.08)`, radio circle (*) fill `#488BED` + white center dot 6pt
- Selection animation: spring (response: 0.3, dampingFraction: 0.7)

**Text:**
- Label: CondensedBold, 17px, #FFFFFF
- Sublabel: Regular, 13px, #888888
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

let emotional_notYet_label = "Not yet"
let emotional_notYet_sub = "Ask me in an hour."

let emotional_starsLink = "Or rate with stars"
let emotional_feelingsLink = "Or tell us how it felt"
let emotional_skip = "Skip for now"
```

### "Not yet" — Deferred Rating System [NEW in R4]

**Problem:** Cold brew doesn't hit immediately. Caffeine onset is 20-45 minutes. The full sensory experience — especially for Deep Work and Voltage — takes time to evaluate. A user finishing a 12-minute Voltage brew doesn't know yet if it "Hit right" or was "Too much." They'll know in an hour.

Current options: guess, skip, or forget to come back. All three are bad data.

**When "Not yet" + DONE tapped:**
1. `emotionalRating` is set to `"pending"`
2. `rating` stays at 0
3. BrewCompletionOverlay dismisses — dashboard shows post-brew state
4. A local notification fires 60 minutes later

**Notification spec:**
```swift
let pendingNotification_title_fn: (String) -> String = { profileName in
    "That \(profileName) brew"
}
let pendingNotification_body = "How did it land?"
// Example: title "That First Light brew" / body "How did it land?"
```

- Fires via `UNUserNotificationCenter` — 60-minute delay from brew completion
- `UNMutableNotificationContent` with `sound: .default`
- Tapping the notification opens the app to the emotional check-in inline (same options, same behavior, pre-populated with brew context: profile name, time, bean)
- If user doesn't respond within 24 hours, `emotionalRating` stays `"pending"` — treated as unrated in all calculations
- Never auto-converted to a star rating
- Never counted in feedback loops
- HistoryView shows: "Rating pending" — Mono, 12px, #888888

**Scheduling:**
```swift
func scheduleDeferredRating(brewLog: BrewLog) {
    let content = UNMutableNotificationContent()
    content.title = pendingNotification_title_fn(brewLog.profileDisplayName)
    content.body = pendingNotification_body
    content.sound = .default
    content.userInfo = ["brewLogId": brewLog.id.uuidString]

    let trigger = UNTimeIntervalNotificationTrigger(timeInterval: 3600, repeats: false)
    let request = UNNotificationRequest(
        identifier: "deferred_rating_\(brewLog.id.uuidString)",
        content: content,
        trigger: trigger
    )

    UNUserNotificationCenter.current().add(request)
}
```

**Post-brew dashboard card with pending rating:**
```
|  |  Rating pending              | |  Mono, 13px, #888888
|  |  We'll ask again in a bit.   | |  Mono, 12px, #666666
|  |                              | |
|  |  Rate now >                  | |  Mono, 12px, #488BED, 44x44pt
```

"Rate now >" opens the emotional check-in inline, same as the notification path.

**Expiration:** If 24 hours pass without a response, the pending state resolves silently:
```swift
func expirePendingRatings() {
    let request: NSFetchRequest<BrewLog> = BrewLog.fetchRequest()
    let oneDayAgo = Calendar.current.date(byAdding: .day, value: -1, to: Date())!
    request.predicate = NSPredicate(format: "emotionalRating == %@ AND createdAt < %@", "pending", oneDayAgo as NSDate)

    if let expired = try? context.fetch(request) {
        for brew in expired {
            brew.emotionalRating = nil  // treat as unrated, not pending
        }
        try? context.save()
    }
}
```

Call `expirePendingRatings()` on app launch.

### "Or rate with stars >" Behavior

Tapping replaces emotional options with standard `RatingStarsView` (36pt stars, per v3.0 spec). A link appears: `"Or tell us how it felt >"` to toggle back. Crossfade transition, 0.2s easeOut. Whichever method the user uses last is what gets saved.

### Dashboard Post-Brew Update

`PostBrewCardView` CTA changes:

```swift
let postBrew_rateCTA = "HOW DID THAT LAND?"  // was "RATE THIS BREW"
```

Tap opens emotional check-in inline within Card 3. Same options, same behavior. Stars accessible via toggle.

### Feedback Loop — How Emotional Ratings Shape Recommendations

**Stored in UserDefaults (accumulating):**

```swift
// Key: "emotional_{profileKey}_{emotionalRating}"
// Value: count (Int)
// Example: "emotional_first_light_hit_right" = 8

// "Surprising" variety weighting (separate from profile feedback):
// Key: "surprising_{beanName}"
// Value: count (Int)
// Example: "surprising_Kenya AA" = 2
```

**Engine rules — `InsightEngine.swift` extension:**

```swift
func shouldAdjustRecommendation(profileKey: String) -> String? {
    let tooMuch = emotionalCount(profileKey, "too_much")
    let neededMore = emotionalCount(profileKey, "needed_more")
    let flat = emotionalCount(profileKey, "flat")
    let hitRight = emotionalCount(profileKey, "hit_right")

    // Consistent "too much" -> shift gentler
    if tooMuch >= 3 && tooMuch > neededMore {
        return "gentler"
    }

    // Consistent "needed more" -> shift stronger
    if neededMore >= 3 && neededMore > tooMuch {
        return "stronger"
    }

    // "Flat" dominates -> bean rotation (trigger separately via checkBeanRotation)
    if flat >= 3 {
        return "bean_change"
    }

    // Happy path — consistently dialed in
    // "locked_in" acknowledgment only renders when hitRight >= 5
    // "1 for 1" is a data point, not a pattern — don't show it
    if hitRight >= 5 {
        return "locked_in"
    }

    return nil
}
```

**"Surprising" variety weighting:**
```swift
func shouldPromoteSurprisingBean(beanName: String, profileKey: String) {
    let count = UserDefaults.standard.integer(forKey: "surprising_\(beanName)")
    if count >= 2 {
        // Promote this bean in sort order for its profile
        let request: NSFetchRequest<BeanProfile> = BeanProfile.fetchRequest()
        request.predicate = NSPredicate(format: "name == %@ AND profileKeys CONTAINS %@", beanName, profileKey)
        if let bean = try? context.fetch(request).first {
            bean.sortOrder = 0  // move to top
            try? context.save()
        }
    }
}
```

### Feedback Acknowledgment Copy — InsightStripView Post-Brew

When the user has 3+ emotional ratings for the same profile, Card 2's post-brew reflection can reference the pattern.

```swift
let emotionalFeedback: [String: String] = [
    "gentler":     "You keep rating {profile} too much at this level. Next time, Brezi pulls back one step.",
    "stronger":    "You keep wanting more from {profile}. Tomorrow, if your signal's the same, we'll bump the dose.",
    "locked_in":   "{profile} on {tier}-recovery mornings. {count} for {count}. Your baseline's dialed.",
    "surprising":  "Surprising — noted. That bean gets weighted higher next time your signal matches.",
    "bean_change": "Flat twice in a row on the same bean. We'll rotate the origin next time."
]
```

**Examples rendered:**
- `"First Light on mid-recovery mornings. 10 for 10. Your baseline's dialed."`
- `"You keep rating Voltage too much at this level. Next time, Brezi pulls back one step."`
- `"Flat twice in a row on the same bean. We'll rotate the origin next time."`

---

## 6. Origin Story Snippets — Complete Copy

Three tiers of origin content per bean:

1. **One-liner** — visible on `BeanCardView` (always shown)
2. **Headline** — top of `OriginStorySheet`
3. **Full story** — body of `OriginStorySheet`

All one-liners, headlines, and full stories are in the bean seed data (Section 2). Complete reference table:

| Bean | One-Liner | Headline |
|---|---|---|
| Ethiopia Yirgacheffe | The birthplace. The original delicacy. | Where coffee began. |
| Colombia Huila | Volcanic soil. Generational patience. | Volcanic patience. |
| Honduras Marcala | Central America's gentle benchmark. | Quiet excellence. |
| Guatemala Antigua | Three volcanoes. One mineral backbone. | Between three volcanoes. |
| Brazil Cerrado | The world's backbone, done right. | The backbone. |
| Costa Rica Tarrazu | Honey-processed at molecular precision. | Obsessive craft. |
| Ethiopia Sidama | Sun-dried. Wild. Transcendent when it works. | Wild, fruity, polarizing. |
| Sumatra Lintong | Wet-hulled on the edge of Lake Toba. | Like no other origin on earth. |
| Kenya AA | The densest bean. The cleanest acidity. | Specialty's performance benchmark. |
| Ethiopia Guji | Sun-dried by single families. Every lot unique. | Every lot is a fingerprint. |
| Rwanda Nyamasheke | Rebuilt from nothing. Now, East Africa's most refined. | Precision that feels earned. |

---

## 7. Integration with v3.0 Base Spec

This document is **additive**. Every touchpoint:

| v3.0 Element | Change | Notes |
|---|---|---|
| DashboardBrewCardView (Card 3) | Top section replaced by EmotionalBrewCardView | Parameters move below emotional layer. "START BREW" and "See full details" unchanged. |
| BrewCompletionOverlay | Star picker replaced by EmotionalCheckInView as primary | 6 options including "Not yet." Stars accessible via toggle. Both write to BrewLog. |
| BrewRecommendationView (detail) | FlavorPreviewView + BeanCardView sections added | Inserted between brew parameter card and "Why This Brew" chain. |
| "Reading your signal..." transition | Replaced by WhyTodayView | ~6s but skippable. Only fires once per day. Smart suppression after 3 consecutive skips. |
| InsightStripView (post-brew) | Emotional feedback acknowledgment lines added | Only after 3+ emotional ratings for same profile. |
| ProfilePickerView | Brew-forward tagline replaces single-line tagline | Cards describe what the coffee does, not just the name. |
| PostBrewCardView | "RATE THIS BREW" becomes "HOW DID THAT LAND?" | Opens emotional check-in inline. Stars still accessible. |
| Core Data | BeanProfile entity added. BrewLog gains emotionalRating + beanName | Migration needed. |
| HistoryView BrewLogCell | Emotional rating badge if present. "Surprising" shows label, no stars. | "Pending" shows "Rating pending" label. |
| OriginStorySheet | Brew-history badge (conditional) + relationship line at 5+ brews | Two conditional sections. One Core Data fetch. |
| Notifications | Deferred rating notification at 60 min for "Not yet" selections | One `UNNotificationRequest` per deferred brew. |

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
- Notification tiers (1-3) — untouched (deferred rating is additive)

---

## 8. Updated Component Library (Additions to v3.0)

| Component | File | Used In |
|---|---|---|
| `EmotionalBrewCardView` | EmotionalBrewCardView.swift | Dashboard Card 3 (replaces top section of DashboardBrewCardView) |
| `EmotionalHeaderView` | EmotionalHeaderView.swift | Card 3 — gradient strip + brew-forward tagline + feeling words |
| `FlavorPreviewStrip` | FlavorPreviewStrip.swift | Dashboard Card 3 — "This cup is:" compact |
| `FlavorPreviewView` | FlavorPreviewView.swift | Brew Recommendation Detail — expanded sensory block |
| `BeanCardView` | BeanCardView.swift | Brew Recommendation Detail — bean recommendations |
| `OriginStorySheet` | OriginStorySheet.swift | Half-sheet from bean card (+ brew-history badge + relationship line) |
| `WhyTodayView` | WhyTodayView.swift | Full-screen transitional moment (with smart suppression) |
| `EmotionalCheckInView` | EmotionalCheckInView.swift | Brew Completion Overlay — primary rating (6 options) |
| `EmotionalRatingOption` | EmotionalRatingOption.swift | Individual option in emotional check-in |
| `ProfileGradient` | ProfileGradient.swift | Gradient definitions per profile |

---

## 9. Updated Data Model (Additions to v3.0)

### New Entity: BeanProfile
See Section 2 for complete schema. 11 seed records.

### BrewLog — New Fields
```swift
@NSManaged var emotionalRating: String?   // "hit_right", "needed_more", "too_much", "surprising", "flat", "pending"
@NSManaged var beanName: String?          // e.g. "Ethiopia Yirgacheffe" — user's selected/logged bean
```

### New UserDefaults Keys (Complete)
```swift
// Why Today
"lastWhyTodayDate"                          // String: "2026-03-19"
"whyTodaySessionCount"                      // Int — total Why Today views
"lastSurpriseSession"                       // Int — session count of last surprise moment

// Why Today copy rotation
"lastWhyToday_high"                         // Int (0, 1, or 2)
"lastWhyToday_mid"                          // Int
"lastWhyToday_low"                          // Int

// Why Today smart suppression [NEW in R4]
"consecutiveWhyTodaySkips"                  // Int — resets to 0 on watch
"whyTodaySuppressedUntil"                   // String — date, "" = not suppressed
"whyTodayReintroduced"                      // Bool — true after first reintroduction

// Emotional feedback accumulation
"emotional_{profileKey}_{emotionalRating}"  // Int (count)
// e.g. "emotional_first_light_hit_right" = 8

// "Surprising" variety weighting [NEW in R4]
"surprising_{beanName}"                     // Int (count)
// e.g. "surprising_Kenya AA" = 2
```

---

## 10. Animation Reference (Additions to v3.0)

| Location | Element | Type | Duration | Easing | Delay | Trigger |
|---|---|---|---|---|---|---|
| Dashboard Card 3 | Gradient strip | fadeIn | 0.3s | easeOut | 0.0s | Card appear |
| Dashboard Card 3 | "This cup is:" words | sequential fadeIn | 0.15s each | easeOut | 0.1s between | Card appear |
| Why Today | Tier word | fadeIn + translateY(8->0) | 0.3s | easeOut | 0.2s | Screen appear |
| Why Today | Haptic | UIImpactFeedback(.soft) | instant | -- | 0.2s | With tier word |
| Why Today | Data line | fadeIn | 0.2s | easeOut | 0.6s | After tier word |
| Why Today | Need text | fadeIn | 0.4s | easeOut | 2.0s | After pause |
| Why Today | Bridge + arrow | fadeIn | 0.3s | easeOut | 3.4s | After pause |
| Why Today | Arrow pulse | opacity 1.0<->0.5 | 1.2s | easeInOut | repeat | After bridge |
| Why Today | Auto-advance | crossfade | 0.5s | easeOut | 5.5s | After all |
| Why Today | Tap-to-skip | snap + crossfade | 0.3s | easeOut | immediate | Tap anywhere |
| Why Today (reintro) | Tier word | fadeIn + translateY(8->0) | 0.3s | easeOut | 0.1s | Screen appear |
| Why Today (reintro) | Auto-advance | crossfade | 0.5s | easeOut | 3.5s | After all |
| Completion | Emotional option select | spring | 0.3s | spring(0.7) | 0.0s | Option tap |
| Completion | Stars <-> emotional toggle | crossfade | 0.2s | easeOut | 0.0s | Link tap |
| Bean cards | Sequential appear | fadeIn + translateY(12->0) | 0.3s each | easeOut | 0.15s between | Section scroll |
| Origin sheet | Sheet appear | system sheet | system | system | 0.0s | "Origin story >" tap |

---

## 11. Build Order Amendment (for Jensen)

Insert between existing v3.0 Day 5-6 and Day 7-8:

**Day 5 (amended):** Add `BeanProfile` Core Data entity + seed data (11 beans). Add `emotionalRating` + `beanName` to `BrewLog`. Add emotional feedback accumulation to `InsightEngine`. Add bean rotation logic (`rotateBeanForProfile`, `checkBeanRotation`). Add "Surprising" variety weighting.

**Day 6 (amended):** Build `WhyTodayView` (transitional screen + haptic + surprise moment + smart suppression with reintroduction). Build `EmotionalBrewCardView` (dashboard card 3 emotional layer). Build `FlavorPreviewStrip` + `FlavorPreviewView`. Build `BeanCardView` + `OriginStorySheet` (including brew-history badge + "Your Yirgacheffe" relationship line).

**Day 7 (amended):** Build `EmotionalCheckInView` + `EmotionalRatingOption` (6 options including "Not yet"). Wire emotional -> star mapping (with "Surprising" excluded). Wire "Or rate with stars" toggle. Wire "Not yet" deferred rating with 60-minute notification + pending state + 24-hour expiration. Wire emotional feedback accumulation + acknowledgment in post-brew InsightStripView. Wire happy-path feedback ("locked_in" acknowledgment, gated at count >= 5).

Everything else in v3.0 build order shifts by ~1 day. TestFlight target becomes 15 working days.

---

## 12. Copy Audit — All Strings (Complete)

Every user-facing string in this document, organized by location. Round 4 changes marked with `[R4]`.

### Why Today
```swift
// Tier words
"Peaked."
"Steady."
"Rebuilding."

// Data lines (parameterized — see Section 3)
"{pct}% recovery."
"Checked in at {score}/10."
"HRV {delta}% {up/down} from baseline."
"Short night — {hours}h."

// Need text (9 variants — 3 per tier)
"Clean night. Full signal. Go."
"System's fully online. This is the morning to use what you've got."
"Everything recovered. The question isn't whether — it's how far."
"Not sharp, not broken. Just a normal morning doing normal morning things."
"Rough edges from last night. Functional, not firing. Steady input."
"System's running. Not surging. Steady input, steady output."
"Nervous system's working hard. Don't add to its job."
"Yesterday cost you. Low signal today. That's the data."
"Low signal. Your body's recovering. It does this."

// Bridge
"This is what meets you there."

// Surprise variants (8)
"You showed up. That's the whole thing."
"Whatever you did last night — noted."
"{N} days straight. We see that."
"Same time tomorrow."
"Brew. Then whatever today is."
"Your morning. Your data. Your call. Always."
"Not every day needs to be a peak day. This one counts too."
"The routine is the thing. You already know that."
```

### Card 3 — Brew-Forward Taglines (15 strings)
```swift
// Tide Pool
"Low dose, gentle extraction. You don't need intensity today and this doesn't pretend otherwise."
"Low concentration, naturally sweet. Stays out of your way."
"The gentlest extraction Brezi makes. Low dose, no bite, no agenda."
// First Light
"Balanced extraction, clean finish. Your everyday standard, set high."
"Moderate dose, medium steep. The brew that shows up the same every time."
"Standard parameters, honest cup. Just enough to start the day."
// Deep Work
"Extended steep, full dose. Built to hold for three hours."
"Longer extraction, heavier body. Focus-grade caffeine, slow release."
"Dense extraction on a low morning. This is a bet. Rate it honest."
// Voltage
"Maximum concentration, full extraction. The hardest brew Brezi pours."
"Data says mid. This brew says full send. Your call."
"Concentrated, aggressive, not what the data recommends. You asked for it."
// The Blank
"Your parameters, your bean, your morning. Brezi just holds the temperature."
"Mid-range signal, manual brew. You're testing something. Brezi doesn't need to know what."
"Low signal, manual override. You know what you're doing — or you're finding out."
```

### Card 3 — Cup Feelings (15 strings)
```swift
"Gentle. Sweet. Deliberate."
"Soft. Grounding. Unhurried."
"Quiet. Warm. Simple."
"Clean. Steady. Familiar."
"Balanced. Clear. Reliable."
"Simple. Honest. Enough."
"Structured. Rich. Sustaining."
"Focused. Layered. Patient."
"Dense. Heavy. Committed."
"Electric. Complex. Alive."
"Bold. Bright. Unapologetic."
"Sharp. Concentrated. Yours."
"Intentional. Specific. Yours."
"Manual. Curious. Uncharted."
"Manual. Exact. Yours."
```

### Flavor Preview
```swift
"WHAT YOU'LL TASTE"
"This cup is:"
// 15 sensory descriptions — see Section 4
```

### Bean Cards
```swift
"RECOMMENDED BEANS"
"Origin story"
"Your brew, your bean. The Blank runs whatever you load."  // [R4: rewrite]

// Per bean (11 beans — all in Section 2):
// flavorFeeling, characterTag, pairingNote, originStory, originHeadline, originOneLiner, sensoryCue
```

### Origin Story Sheet
```swift
// Brew-history badge
"Brewed {count} times . Last: {date}"        // Mono, 12px, #488BED
"Usually: {rating}"                           // [R4: "Usually" not "Average"] Mono, 12px, #666666
"Not yet rated"                               // fallback when no ratings exist

// Relationship line [R4: new]
"Your {region}. At this point, basically."    // Mono, 12px, #888888, italic (count >= 5)
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
"Not yet"                                     // [R4: new]
"Ask me in an hour."                          // [R4: new]
"Or rate with stars"
"Or tell us how it felt"
"Skip for now"
"HOW DID THAT LAND?"                         // PostBrewCardView CTA
```

### Deferred Rating [R4: new]
```swift
"That {profile} brew"                         // notification title
"How did it land?"                            // notification body
"Rating pending"                              // HistoryView label
"We'll ask again in a bit."                   // post-brew card pending state
"Rate now"                                    // post-brew card CTA for pending
```

### Emotional Feedback Acknowledgment
```swift
"You keep rating {profile} too much at this level. Next time, Brezi pulls back one step."   // [R4: rewrite]
"You keep wanting more from {profile}. Tomorrow, if your signal's the same, we'll bump the dose."  // [R4: rewrite]
"{profile} on {tier}-recovery mornings. {count} for {count}. Your baseline's dialed."
"Surprising — noted. That bean gets weighted higher next time your signal matches."
"Flat twice in a row on the same bean. We'll rotate the origin next time."
```

---

## 13. Decisions — All Four Rounds (Final, Locked)

### Round 1 (Architecture)
1. Five-layer emotional system: Why Today -> Card 3 -> Flavor Preview -> Bean Card -> Check-In
2. Each layer has one job. No overlap.
3. Bean cards describe the coffee, not the user.
4. Emotional check-in as primary, stars as secondary.
5. Feedback loop shapes future recommendations.
6. Surprise moment every 10-15 sessions.
7. Happy-path acknowledgment ("locked_in").
8. Profile color gradients throughout emotional layer.
9. Three-word constraint on "This cup is:" — always.
10. Bean recommendations fixed per profile for v1.

### Round 2 (Voice)
11. Therapy voice killed. Every line passes the text-message test.
12. Why Today owns state. Card 3 owns response. No redundancy.
13. Tier word leads on Why Today. "Rebuilding." first, number second.
14. "Steady." for mid tier. Not "Working." Not "Functional."
15. Haptic on tier word. Single soft tap.
16. Front-loaded sensation in all sensory copy. First sentence = the taste.

### Round 3 (Polish)
17. The Blank has a personality. Ownership and respect, not absence. "Brezi just holds the temperature."
18. High-tier need text defended: "The question isn't whether — it's how far." Peaked mornings lean forward.
19. Sensory extremes sharpened: positive sensation leads, negatives don't chain, punchlines don't repeat across layers.
20. Origin story sheet gains brew-history badge. One Core Data fetch, two lines. Closes the loop.
21. Feedback acknowledgment speaks action, not education. "We'll rotate the origin" not "try switching terroir."
22. Surprise lines sound like a friend at 6am, not a founder on stage. "The routine is the thing" not "the practice is the product."

### Round 4 (Trust Mechanics)
23. "Surprising" decoupled from star ratings. Novelty and preference are different signals. No quality inflation.
24. Why Today gains smart suppression with reintroduction. Three skips -> 7-day cooldown -> shortened reintro -> watch resumes, skip extends. One extra chance with better pacing. Never fight the user, but don't abandon the screen after Week 1 either.
25. Bean rotation logic built. Sort-order demotion on 2+ "Flat" for same bean. "We'll rotate the origin" is a kept promise.
26. Feedback copy mirrors user language, not escalates it. "Too much" -> "you keep rating too much," not "hits too hard." Specific mechanics: "bump the dose" not "push it."
27. "Usually" not "Average" on brew-history badge. Honest about methodology.
28. "Your Yirgacheffe. At this point, basically." — at 5+ brews, the origin sheet acknowledges the relationship graduated. The bean is yours now.
29. Sixth check-in option: "Not yet / Ask me in an hour." Deferred rating with 60-minute notification. Respects the caffeine timeline. Converts skips to signals.
30. "The Blank runs whatever you load." Machine language for manual-mode users.

---

## 14. What This Doesn't Cover (Explicitly Deferred)

| Item | Reason | When |
|---|---|---|
| Dynamic bean matching from emotional preferences | Need rating data first | v2 |
| Bean inventory / availability | Requires commerce layer | When commerce ships |
| Seasonal bean rotation | Need supplier partnerships | Post-launch |
| "Flavor profile" user preference screen | Let implicit data accumulate | v2 |
| Bean rating (separate from brew rating) | One rating moment is enough for v1 | v2 |
| Sensory description variants (multiple per profile x tier) | 15 is enough for launch | Month 2 |
| AI-generated personalized sensory copy | Static copy is better than bad AI copy | v3+ |
| Day 90 notification tie-in with emotional data | Need retention data first | Post-TestFlight |
| Observational surprise lines ("You brew 4 min earlier on Mondays") | Requires data accumulation + pattern detection + privacy review | v2 (own design review) |
| Bean-level feedback acknowledgments ("Kenya AA keeps hitting too hard") | Need bean-specific rating data | v2 (data model ready — beanName on BrewLog) |
| Bean story share mechanic | Bean card + origin story is the share unit, not brew params | v2 (growth roadmap) |
| Emotional feedback migration from UserDefaults to Core Data | UserDefaults fine for v1 counts, won't scale to bean-level feedback | v2 |
| Profile mismatch detection ("You rated every Tide Pool bean Flat") | Needs all three beans demoted as trigger | v2 |

---

## 15. Complete Round 4 Diff — Every Changed Line

For Jensen's reference: exact before/after of every line that changed from Round 3 to Round 4. Six targeted changes plus one addition.

### Star Mapping (1 changed)
| Location | Round 3 | Round 4 |
|---|---|---|
| emotionalToStars["surprising"] | `"surprising": 5` | `"surprising": 0` — no star conversion. Separate variety weighting signal. |

### Feedback Copy (2 changed)
| Location | Round 3 | Round 4 |
|---|---|---|
| emotionalFeedback["gentler"] | "You keep saying {profile} hits too hard at this level. Next time, Brezi goes one step lighter." | "You keep rating {profile} too much at this level. Next time, Brezi pulls back one step." |
| emotionalFeedback["stronger"] | "You keep wanting more from {profile}. Tomorrow, if your signal's the same, we'll push it." | "You keep wanting more from {profile}. Tomorrow, if your signal's the same, we'll bump the dose." |

### Bean Note (1 changed)
| Location | Round 3 | Round 4 |
|---|---|---|
| theBlank_beanNote | "You're driving. Pick any bean — The Blank adapts to whatever you bring." | "Your brew, your bean. The Blank runs whatever you load." |

### Brew-History Badge (1 changed)
| Location | Round 3 | Round 4 |
|---|---|---|
| Badge methodology label | "Average: Hit right" | "Usually: Hit right" |

### New Content (4 additions)
| Item | Content |
|---|---|
| Why Today smart suppression | 3-skip -> 7-day cooldown -> shortened reintroduction -> escalating suppress. Three UserDefaults keys. |
| Bean rotation logic | `rotateBeanForProfile()` + `checkBeanRotation()`. Sort-order demotion on 2+ "Flat" same bean. |
| "Not yet" check-in option | Sixth emotional option. 60-minute deferred notification. "pending" state with 24-hour expiration. |
| "Your Yirgacheffe" relationship line | Conditional text at 5+ brews in origin story sheet. Uses `bean.region` as short name. |

**Total: 4 line changes + 4 feature additions. Zero structural changes. Five-job architecture untouched.**

---

*Five layers. Five jobs. No overlap. The data tells you your state. Why Today makes you feel it. The tagline tells you what the coffee does about it. The flavor preview lets you taste it before you brew. The bean card gives it a name and a place on earth. The emotional check-in closes the loop — and now it respects the timeline.*

*"Surprising" is its own signal. The Blank runs whatever you load. Your Yirgacheffe is yours at this point, basically. "Not yet — ask me in an hour" is the most honest answer the app allows. The bean rotation promise is kept. The Why Today screen earns its screen time or gets out of the way.*

*Four rounds. Thirty locked decisions. Every string paste-ready. Every promise buildable.*

*Round 4 locked. Jensen builds.*

— Picasso
