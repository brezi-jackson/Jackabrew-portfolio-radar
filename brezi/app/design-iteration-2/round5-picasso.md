# Brezi iOS App — Emotional Layer Design v5.0
**Author:** Picasso (Product Design) | March 19, 2026 | Round 5 — Extensions
**For:** Jackson (Product) · Jensen (Engineering) · Elon (Growth/Copy)
**Status:** EXTENSION. Round 4 is the build spec. This document adds seven layers that Round 4 doesn't cover.

---

## 0. What This Is and What It Isn't

Round 4 is locked. Thirty decisions. Every string paste-ready. Nothing in this document contradicts, replaces, or reopens anything from Rounds 1-4.

Round 5 covers **gaps between the locked pieces** — connections the locked architecture implies but never makes explicit. Seven extensions:

1. **State-Bean Bridge Lines** — The missing sentence that connects the user's body state to the specific bean they're about to drink
2. **Four-Node "Why This Brew" Chain** — The bean enters the reasoning the user sees
3. **Why Today Maturation** — The screen looks different on Day 1 than Day 100
4. **State-Aware Flavor Framing** — The sensory description acknowledges where you are today, not just what the coffee tastes like
5. **Pattern Vocabulary** — The app describes your brewing pattern back to you
6. **Full-Day Emotional Arc** — Emotional tone shifts from morning through afternoon through evening
7. **Return Moments** — What happens when you come back after days away

**What these share:** They're all temporal or relational. Round 4 designs what happens at each touchpoint. Round 5 designs how those touchpoints **change over time** and **connect to each other**.

**Division of labor preserved:** Five layers, five jobs. Why Today owns state. Card 3 owns response. Flavor Preview owns sensation. Bean Card owns character. Check-In owns feedback. Nothing here violates that.

---

## 1. State-Bean Bridge Lines — `BeanBridgeView.swift`

### The Gap

Card 3 tells you what the coffee does. The bean card tells you the coffee's personality. But nowhere does the app say: **"This specific bean, because of YOUR specific state today."**

The connection is implicit. The user has to do the math:
- Why Today said "Rebuilding"
- Card 3 said "gentle extraction"
- The bean card says "Ethiopia Yirgacheffe — soft, floral"

The bridge between state and bean is obvious to us. It is not obvious to a first-time user at 6:30am.

### Solution

One line. Appears on the bean card (Section 2 of R4), between the `characterTag` and the bottom of the card. Only on the **first** (top) bean card — the recommended one.

```
|  |  The quietest bean in       ||  characterTag (existing)
|  |  the lineup                 ||  Mono, 12px, #488BED
|  |                             ||
|  |  Gentle origin for a low    ||  STATE-BEAN BRIDGE [NEW]
|  |  morning. That's the match. ||  Mono, 12px, #888888
|  |                             ||
|  +-----------------------------+|
```

### Spec

- Position: below `characterTag`, 8pt top padding
- Font: Mono, 12px, #888888
- Only renders on the first bean card (sortOrder position 1 for the active profile)
- Only renders when recovery data is available (not on The Blank)

### Copy — Complete Set (15 Combinations: 5 Profiles × 3 Tiers)

```swift
let beanBridgeLines: [String: [String: String]] = [
    "tide_pool": [
        "high": "Gentle origin on a strong morning. Room to appreciate the subtlety.",
        "mid":  "Low intensity for a mid-range day. Stays out of your way.",
        "low":  "Gentle origin for a low morning. That's the match."
    ],
    "first_light": [
        "high": "Balanced bean on your best signal. Everyday standard, no waste.",
        "mid":  "Reliable origin for a steady day. Nothing to prove.",
        "low":  "Clean bean on a rough morning. Honest cup, honest day."
    ],
    "deep_work": [
        "high": "Complex origin backed by a full recovery. Built for the long session.",
        "mid":  "Layered bean on a mid signal. Extraction does the heavy lifting.",
        "low":  "Dense origin on a low day. The dose is the bet. The bean holds up."
    ],
    "voltage": [
        "high": "High-intensity origin on peak signal. Everything's aligned.",
        "mid":  "Sharp bean on a mid morning. You chose the intensity. The origin delivers it.",
        "low":  "Concentrated origin on low recovery. Not what the data suggests. Bean's ready if you are."
    ],
    "the_blank": [
        "high": "",  // The Blank: no bridge line
        "mid":  "",
        "low":  ""
    ]
]
```

**Selection logic:**
```swift
func beanBridgeLine(profileKey: String, recoveryTier: String) -> String? {
    let line = beanBridgeLines[profileKey]?[recoveryTier] ?? ""
    return line.isEmpty ? nil : line
}
```

**Why this matters:** This is the sentence that makes the user feel *known*. Not "here's a good coffee." Here's a good coffee **for you, this morning, because of what your body did last night.** It's the difference between a menu and a concierge.

---

## 2. Four-Node "Why This Brew" Chain — `WhyThisBrewView.swift`

### The Gap

Round 4's Card 3 tagline explains the brew parameters. The bean card explains the bean's character. But the "Why This Brew" detail section (referenced in R4's bean card UI) currently shows a three-node chain:

```
Your data → Brew profile → Parameters
```

The bean is absent. The user sees why they got First Light but not why they got Guatemala Antigua specifically.

### Solution

Extend the reasoning chain to four nodes. The bean gets its own node with a one-line rationale.

### Layout — `WhyThisBrewView.swift` (Detail Screen)

```
|                                 |
|  WHY THIS BREW                  |  Mono, 11px, #888888, ALL CAPS
|                                 |  bottom padding: 16pt
|  ○ Your signal                  |  Node 1: DATA
|  │ 61% recovery. HRV dipped.   |  Mono, 12px, #888888
|  │                              |
|  ○ Tide Pool                    |  Node 2: PROFILE
|  │ Low dose, gentle extraction. |  Mono, 12px, #888888
|  │                              |
|  ○ Ethiopia Yirgacheffe         |  Node 3: BEAN [NEW]
|  │ Florals unlock at low dose.  |  Mono, 12px, #888888
|  │ The gentlest origin matches  |
|  │ the gentlest parameters.     |
|  │                              |
|  ○ 4-8°C . 12 min . 60 g/L     |  Node 4: PARAMETERS
|    Dialed for your morning.     |  Mono, 12px, #888888
|                                 |
```

### Spec

- Section label ("WHY THIS BREW"): Mono, 11px, #888888, ALL CAPS
- Node circles: 8pt diameter, 1.5px stroke #333333, no fill
- Vertical line between nodes: 1px #222222, 16pt segment length
- Node title: Mono, 13px, #FFFFFF
- Node body: Mono, 12px, #888888, line-height 1.5
- Padding: 20pt horizontal, 16pt top below label
- Background: none (sits within the detail screen's existing container)

### Bean Node Copy — Complete Set (11 Beans × Profile Rationale)

Each bean gets one pairing rationale per profile it serves. These explain **why this bean matches this profile** — not the bean's character (that's the bean card's job) and not the brew parameters (that's the profile node's job).

```swift
let beanPairingRationale: [String: [String: String]] = [
    // Tide Pool beans
    "Ethiopia Yirgacheffe": [
        "tide_pool": "Florals unlock at low dose. The gentlest origin matches the gentlest parameters."
    ],
    "Colombia Huila": [
        "tide_pool": "Natural sweetness at low concentration. Comfort without heaviness.",
        "first_light": "Balanced sweetness through moderate extraction. The versatile origin."
    ],
    "Honduras Marcala": [
        "tide_pool": "Simplicity is the strength at low extraction. No rough edges to process."
    ],

    // First Light beans
    "Guatemala Antigua": [
        "first_light": "Mineral backbone survives any method. At balanced parameters, it's the definition of reliable.",
        "deep_work": "Volcanic structure holds up under extended extraction. Gets richer the longer you steep."
    ],
    "Brazil Cerrado": [
        "first_light": "Natural sweetness makes moderate extraction feel indulgent. Effortless quality."
    ],
    "Costa Rica Tarrazu": [
        "first_light": "Honey processing adds body that clean extraction makes transparent. Craft you taste without thinking."
    ],

    // Deep Work beans
    "Ethiopia Sidama": [
        "deep_work": "Extended steep pulls layers that keep your palate working for hours. Complexity is the point."
    ],
    "Sumatra Lintong": [
        "deep_work": "Earthy body at full dose creates something dense and sustaining. A working coffee."
    ],

    // Voltage beans
    "Kenya AA": [
        "voltage": "Phosphoric acidity at high concentration. Controlled detonation. Bright, complex, demanding."
    ],
    "Ethiopia Guji": [
        "voltage": "Fruit intensity matches caffeine intensity at these parameters. Full signal, full flavor."
    ],
    "Rwanda Nyamasheke": [
        "voltage": "Juicy complexity that concentration amplifies rather than overwhelms. Clarity at volume."
    ]
]
```

**Selection logic:**
```swift
func beanRationale(beanName: String, profileKey: String) -> String {
    return beanPairingRationale[beanName]?[profileKey] ?? ""
}
```

**The Blank:** No bean node. Chain stays at three nodes (signal → manual → parameters). The user chose everything — there's no "why" to explain.

### Animation

Nodes appear sequentially on detail screen load:
- Each node: fadeIn + translateY(6→0), 0.25s easeOut, 0.2s delay between nodes
- Vertical line segments draw downward: 0.15s each, synchronized with the next node's appearance

---

## 3. Why Today Maturation — `WhyTodayView.swift` Extension

### The Gap

Why Today looks identical on Day 1 and Day 100. The tier word is "Rebuilding." whether the user has never seen it or has seen it thirty times. The data line reads the same. The need text rotates through three variants forever.

A screen that claims to *know* you should show evidence of knowing you over time.

### Solution

Three maturation tiers. Same layout, same five-job architecture. The **copy registers** shift. Nothing structural changes. No new animations. No new screens.

### Maturation Tiers

```swift
enum WhyTodayMaturity {
    case new       // Days 1-7 (sessions 1-7)
    case familiar  // Days 8-30 (sessions 8-30)
    case veteran   // Day 31+ (session 31+)
}

func whyTodayMaturity() -> WhyTodayMaturity {
    let count = UserDefaults.standard.integer(forKey: "whyTodaySessionCount")
    if count <= 7 { return .new }
    if count <= 30 { return .familiar }
    return .veteran
}
```

### What Changes at Each Tier

**Tier Word (Phase 1a) — unchanged.** "Peaked." "Steady." "Rebuilding." These are the anchors. They don't mature. They're the same word on Day 1 and Day 300 because the body state is the same.

**Data Line (Phase 1b) — unchanged.** Recovery percentages don't get more personal. "61% recovery" is "61% recovery."

**Need Text (Phase 2) — new variants per maturity.** This is where the register shifts.

#### New Maturity Variants — Need Text

**Days 1-7 (new):** Current copy. These are the defaults from R4. No change.

```swift
let whyToday_needText_new: [String: [String]] = [
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

**Days 8-30 (familiar):** Shorter. More confident. Assumes the user already gets the framework.

```swift
let whyToday_needText_familiar: [String: [String]] = [
    "high": [
        "Full signal. You know what to do with this.",
        "Everything recovered. Use it.",
        "Best signal this week. Go."
    ],
    "mid": [
        "Mid-range. Same as most mornings. You know the drill.",
        "Not your best, not your worst. Standard input.",
        "Functional. Nothing more to read into."
    ],
    "low": [
        "Low signal. You've seen this before. It passes.",
        "Rough one. Gentle input. You know the move.",
        "Your body's working on it. Don't fight it."
    ]
]
```

**Day 31+ (veteran):** Even shorter. Occasionally references the user's own history. Treats the user like someone who's been here.

```swift
let whyToday_needText_veteran: [String: [String]] = [
    "high": [
        "Peaked. Rare for you lately. Don't waste it.",
        "Full recovery. Hasn't happened in a few days. Go.",
        "Best signal since {lastPeakDay}. Act on it."
    ],
    "mid": [
        "Steady. Your most common state. Brezi knows what works here.",
        "Mid-range. You've brewed from this signal {midCount} times. Dialed.",
        "Average morning. That's not a criticism — your average is good."
    ],
    "low": [
        "Low. Last time was {lastLowDay}. Same approach worked then.",
        "Rebuilding. {daysSinceLow} days since the last one. It passes.",
        "Low signal. Gentle parameters. You've done this."
    ]
]
```

#### Veteran Copy — Dynamic References

Veteran variants reference the user's own data. These require lookup functions:

```swift
func veteranCopyParams() -> [String: String] {
    var params: [String: String] = [:]

    // Last peaked day
    if let lastPeak = lastSessionDate(tier: "high") {
        params["lastPeakDay"] = shortDateString(lastPeak)  // "Tuesday" or "Mar 12"
    }

    // Mid count
    let midCount = tierSessionCount("mid")
    params["midCount"] = "\(midCount)"

    // Last low day
    if let lastLow = lastSessionDate(tier: "low") {
        params["lastLowDay"] = shortDateString(lastLow)  // "Thursday" or "Mar 10"
    }

    // Days since last low
    if let lastLow = lastSessionDate(tier: "low") {
        let days = Calendar.current.dateComponents([.day], from: lastLow, to: Date()).day ?? 0
        params["daysSinceLow"] = "\(days)"
    }

    return params
}
```

**Fallback:** If a veteran variant references data that doesn't exist (e.g., `{lastPeakDay}` but the user has never peaked), fall back to the familiar-tier variant for that slot. Never show a template variable to the user.

```swift
func resolveVeteranCopy(_ template: String, params: [String: String]) -> String? {
    var resolved = template
    let pattern = /\{(\w+)\}/
    for match in template.matches(of: pattern) {
        let key = String(match.1)
        guard let value = params[key] else { return nil }  // fallback signal
        resolved = resolved.replacing(match.0, with: value)
    }
    return resolved
}

func selectWhyTodayNeed(tier: String) -> String {
    let maturity = whyTodayMaturity()
    let variants: [String]

    switch maturity {
    case .new:
        variants = whyToday_needText_new[tier]!
    case .familiar:
        variants = whyToday_needText_familiar[tier]!
    case .veteran:
        variants = whyToday_needText_veteran[tier]!
    }

    let key = "lastWhyToday_\(tier)_\(maturity)"
    let lastIndex = UserDefaults.standard.integer(forKey: key)
    let nextIndex = (lastIndex + 1) % variants.count
    UserDefaults.standard.set(nextIndex, forKey: key)

    let candidate = variants[nextIndex]

    // Veteran copy may need dynamic resolution
    if maturity == .veteran {
        let params = veteranCopyParams()
        if let resolved = resolveVeteranCopy(candidate, params: params) {
            return resolved
        }
        // Fallback to familiar
        let familiarVariants = whyToday_needText_familiar[tier]!
        return familiarVariants[nextIndex % familiarVariants.count]
    }

    return candidate
}
```

### New UserDefaults Keys

```swift
// Maturity-aware rotation (extends existing rotation keys)
"lastWhyToday_high_new"         // Int (0, 1, or 2)
"lastWhyToday_high_familiar"    // Int
"lastWhyToday_high_veteran"     // Int
"lastWhyToday_mid_new"          // Int
"lastWhyToday_mid_familiar"     // Int
"lastWhyToday_mid_veteran"      // Int
"lastWhyToday_low_new"          // Int
"lastWhyToday_low_familiar"     // Int
"lastWhyToday_low_veteran"      // Int

// Veteran copy data (for dynamic references)
"lastSessionDate_high"          // String (date)
"lastSessionDate_mid"           // String (date)
"lastSessionDate_low"           // String (date)
"tierSessionCount_high"         // Int
"tierSessionCount_mid"          // Int
"tierSessionCount_low"          // Int
```

**Recording tier sessions:**
```swift
func recordWhyTodaySession(tier: String) {
    let key = "tierSessionCount_\(tier)"
    let count = UserDefaults.standard.integer(forKey: key) + 1
    UserDefaults.standard.set(count, forKey: key)
    UserDefaults.standard.set(todayDateString(), forKey: "lastSessionDate_\(tier)")
}
```

Call `recordWhyTodaySession()` after Why Today displays, inside `recordWhyTodayInteraction()`.

---

## 4. State-Aware Flavor Framing — `FlavorPreviewView.swift` Extension

### The Gap

The sensory descriptions in R4 are static per profile × tier. "Jasmine and bergamot up front" reads the same on a rebuilding Monday and a peaked Friday. The flavor doesn't change — but the *framing* of that flavor could acknowledge where you are.

### Solution

A single **intro line** prepended to the existing sensory description. One sentence. Appears only in the expanded `FlavorPreviewView` (detail screen), not in the dashboard strip.

### Layout Addition

```
|  |  WHAT YOU'LL TASTE          ||  (existing label)
|  |                             ||
|  |  Low morning. Gentle cup.   ||  FRAMING LINE [NEW]
|  |                             ||  Mono, 12px, #888888
|  |  Jasmine and bergamot up    ||  (existing sensory description)
|  |  front, then something      ||
|  |  sweet and round...         ||
```

### Spec

- Position: between "WHAT YOU'LL TASTE" label and sensory description body
- Font: Mono, 12px, #888888
- Bottom padding: 8pt before sensory description
- Only renders when recovery data is available (not on The Blank manual entries)

### Copy — Complete Set (15 Combinations)

```swift
let flavorFramingLines: [String: [String: String]] = [
    "tide_pool": [
        "high": "Strong morning. You chose gentle anyway. Here's what that tastes like.",
        "mid":  "Mid-range signal. Soft approach. Here's the cup.",
        "low":  "Low morning. Gentle cup. This is what matches."
    ],
    "first_light": [
        "high": "Peaked and balanced. The standard cup for the standard you've set.",
        "mid":  "Steady morning, steady cup. Nothing needs to be different.",
        "low":  "Rough morning, honest cup. Nothing heroic. Just present."
    ],
    "deep_work": [
        "high": "Full recovery. Full extraction. This is what three hours of focus tastes like.",
        "mid":  "Mid-signal, full commitment. The dose does the work.",
        "low":  "Low morning, heavy pour. The bet is the extraction carries you through."
    ],
    "voltage": [
        "high": "Peak signal, peak intensity. Everything's on. Here's what that tastes like.",
        "mid":  "Mid-range signal, maximum brew. Your choice outranks your data today.",
        "low":  "Low recovery, full send. Not recommended. But you'll know exactly what you're tasting."
    ],
    "the_blank": [
        "high": "",  // no framing on The Blank
        "mid":  "",
        "low":  ""
    ]
]
```

**Selection logic:**
```swift
func flavorFramingLine(profileKey: String, recoveryTier: String) -> String? {
    let line = flavorFramingLines[profileKey]?[recoveryTier] ?? ""
    return line.isEmpty ? nil : line
}
```

**Why one sentence:** Two sentences fight the sensory description for attention. One sentence sets the table. The sensory copy does the rest.

---

## 5. Pattern Vocabulary — `PatternView.swift`

### The Gap

Emotional feedback accumulates in UserDefaults. The InsightEngine uses it to adjust recommendations. But the **user never sees their own pattern described.** They rate "Hit right" ten times and the app says "Your baseline's dialed" — but it never says "You're a morning-peaked First Light brewer who almost never goes Voltage."

The app has an opinion about you. It should say it out loud — once.

### Solution

A pattern summary that renders once the app has enough data to say something real. Not a dashboard widget. Not a recurring notification. A **one-time moment** in the origin story sheet or the brew detail screen — the places where the user is already in a reflective headspace.

### Trigger

Pattern summary renders when **all three conditions** are met:
1. Total brew count ≥ 15
2. One profile has ≥ 60% of all brews
3. The user has not dismissed the pattern summary before

```swift
func shouldShowPatternSummary() -> Bool {
    let dismissed = UserDefaults.standard.bool(forKey: "patternSummaryDismissed")
    if dismissed { return false }

    let request: NSFetchRequest<BrewLog> = BrewLog.fetchRequest()
    guard let allBrews = try? context.fetch(request), allBrews.count >= 15 else { return false }

    let profileCounts = Dictionary(grouping: allBrews, by: { $0.profileKey })
        .mapValues { $0.count }

    let total = allBrews.count
    let dominant = profileCounts.max(by: { $0.value < $1.value })

    guard let dominant = dominant, Double(dominant.value) / Double(total) >= 0.6 else { return false }

    return true
}
```

### Layout — Inline in Brew Detail Screen

Appears below the "Why This Brew" chain, above deferred items. Dismissible.

```
|                                 |
|  YOUR PATTERN                   |  Mono, 11px, #888888, ALL CAPS
|                                 |  bottom padding: 12pt
|  +-----------------------------+|
|  |                             ||  bg: #1A1A1A, radius: 16pt
|  |  15 brews in. Here's what  ||  Regular, 15px, #CCCCCC
|  |  Brezi sees:               ||  line-height: 1.5
|  |                             ||
|  |  First Light brewer.       ||  CondensedBold, 17px, #FFFFFF
|  |  Mid-recovery mornings.    ||  CondensedBold, 17px, #FFFFFF
|  |  Guatemala Antigua, mostly.||  CondensedBold, 17px, #FFFFFF
|  |                             ||
|  |  Hit right 10 out of 12    ||  Mono, 12px, #888888
|  |  times on this profile.    ||
|  |  Your parameters are       ||
|  |  dialed.                   ||
|  |                             ||
|  |  Got it                    ||  Mono, 12px, #488BED, 44x44pt
|  |                             ||
|  +-----------------------------+|
```

### Spec

- Container: #1A1A1A, radius 16pt, padding 20pt
- Section label: Mono, 11px, #888888, ALL CAPS
- Intro: Regular, 15px, #CCCCCC
- Pattern lines: CondensedBold, 17px, #FFFFFF — always three lines
- Detail: Mono, 12px, #888888
- Dismiss: "Got it" — Mono, 12px, #488BED, 44x44pt tap target
- Tapping "Got it" sets `UserDefaults "patternSummaryDismissed" = true`
- Animation: fadeIn 0.3s easeOut on first render

### Pattern Copy — Dynamic Generation

```swift
struct PatternSummary {
    let profileName: String       // "First Light"
    let tierName: String          // "Mid-recovery"
    let topBeanName: String       // "Guatemala Antigua"
    let hitRightCount: Int        // 10
    let totalProfileBrews: Int    // 12
}

func generatePatternSummary() -> PatternSummary? {
    let request: NSFetchRequest<BrewLog> = BrewLog.fetchRequest()
    guard let allBrews = try? context.fetch(request), allBrews.count >= 15 else { return nil }

    // Dominant profile
    let profileCounts = Dictionary(grouping: allBrews, by: { $0.profileKey })
        .mapValues { $0.count }
    guard let dominant = profileCounts.max(by: { $0.value < $1.value }) else { return nil }
    let profileKey = dominant.key

    let profileBrews = allBrews.filter { $0.profileKey == profileKey }

    // Most common tier
    let tierCounts = Dictionary(grouping: profileBrews, by: { tierFromScore($0.recoveryScore) })
        .mapValues { $0.count }
    let dominantTier = tierCounts.max(by: { $0.value < $1.value })?.key ?? "mid"

    // Most common bean
    let beanCounts = Dictionary(grouping: profileBrews.compactMap { $0.beanName != nil ? $0 : nil },
                                by: { $0.beanName! })
        .mapValues { $0.count }
    let topBean = beanCounts.max(by: { $0.value < $1.value })?.key ?? ""

    // Hit right count
    let hitRight = profileBrews.filter { $0.emotionalRating == "hit_right" }.count

    return PatternSummary(
        profileName: profileDisplayName(profileKey),
        tierName: tierDisplayName(dominantTier),
        topBeanName: topBean,
        hitRightCount: hitRight,
        totalProfileBrews: profileBrews.count
    )
}
```

### Pattern Description Lines — Templates

Three lines. Always three. Mirrors the three-word constraint on "This cup is:" — three-line pattern description is a rhythm.

```swift
func patternLines(_ summary: PatternSummary) -> [String] {
    return [
        "\(summary.profileName) brewer.",
        "\(summary.tierName) mornings.",
        summary.topBeanName.isEmpty ? "No bean preference yet." : "\(summary.topBeanName), mostly."
    ]
}

func patternDetail(_ summary: PatternSummary) -> String {
    if summary.hitRightCount >= summary.totalProfileBrews / 2 {
        return "Hit right \(summary.hitRightCount) out of \(summary.totalProfileBrews) times on this profile. Your parameters are dialed."
    } else {
        return "\(summary.totalProfileBrews) brews on \(summary.profileName). Still finding the sweet spot — and that's normal at this stage."
    }
}
```

### Pattern Intro — Maturity-Aware

```swift
func patternIntro(brewCount: Int) -> String {
    if brewCount < 20 {
        return "\(brewCount) brews in. Here's what Brezi sees:"
    } else if brewCount < 50 {
        return "\(brewCount) brews. Here's your pattern:"
    } else {
        return "\(brewCount) brews. You're consistent. Here's the data:"
    }
}
```

---

## 6. Full-Day Emotional Arc — Temporal Tone

### The Gap

Brezi's emotional layer is designed for the morning. Why Today fires at first load. Card 3 shows the brew recommendation. Post-brew check-in happens after brewing. But what happens at 3pm when the user opens the app? What about 8pm?

The app currently shows the same post-brew card for the rest of the day. The tone is frozen at the brew moment. The day moves on but the app doesn't.

### Solution

Three temporal states for the post-brew dashboard. Not new screens. Not new navigation. The **Card 3 post-brew display** shifts its copy register based on elapsed time since brew.

### Temporal States

```swift
enum PostBrewPhase {
    case recent     // 0-2 hours after brew
    case afternoon  // 2-6 hours after brew
    case evening    // 6+ hours after brew, or after daily cutoff
}

func postBrewPhase(brewedAt: Date) -> PostBrewPhase {
    let elapsed = Date().timeIntervalSince(brewedAt)
    let hours = elapsed / 3600

    if hours < 2 { return .recent }
    if hours < 6 { return .afternoon }
    return .evening
}
```

### Card 3 — Post-Brew Copy by Phase

**Recent (0-2h):** The existing post-brew state from v3.0. No change.

```swift
// Existing: shows brew summary, "HOW DID THAT LAND?" CTA
```

**Afternoon (2-6h):** The brew is done. The card acknowledges the transition.

```swift
let postBrew_afternoon: [String: String] = [
    "tide_pool":   "Brewed this morning. Gentle start, done its job.",
    "first_light": "This morning's brew. Balanced, finished clean.",
    "deep_work":   "Deep Work from this morning. Should still be holding.",
    "voltage":     "Voltage from this morning. If you're still feeling it, that's by design.",
    "the_blank":   "This morning's brew. Your settings, your outcome."
]
```

**Evening (6h+):** The day is winding down. Card 3 shifts to forward-looking.

```swift
let postBrew_evening: [String: String] = [
    "tide_pool":   "Done for today. Tomorrow's brew depends on tonight.",
    "first_light": "Today's cup is behind you. Sleep well — tomorrow's signal starts now.",
    "deep_work":   "Session's over. Recovery starts when you shut down.",
    "voltage":     "Full send this morning. Give the system time to reset.",
    "the_blank":   "Manual brew, logged. Tomorrow's a new set of variables."
]
```

### Layout — Afternoon / Evening State

```
+-----------------------------------------+
|                                         |
|  +-----------------------------------+  |  gradient strip (same as pre-brew)
|  |  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  |  |
|  +-----------------------------------+  |
|                                         |
|  FIRST LIGHT                            |  CondensedBold, 32px, #FFFFFF
|  7:14am . 12 min                        |  Mono, 14px, #888888
|                                         |
|  This morning's brew. Balanced,         |  Regular, 16px, #888888
|  finished clean.                        |  (afternoon copy)
|                                         |
|  Hit right                              |  Mono, 13px, #488BED
|                                         |  (emotional rating if set)
|                                         |
+-----------------------------------------+
```

### Spec

- Afternoon/evening copy replaces the tagline position (below profile name)
- Font: Regular, 16px, #888888 (dimmer than pre-brew tagline's #CCCCCC)
- Emotional rating (if set) renders below in Mono, 13px, #488BED
- "HOW DID THAT LAND?" CTA still renders in recent phase; hides in afternoon/evening (the moment has passed)
- Pending rating "Rate now >" persists through afternoon, hides in evening

**No rating prompt in evening:** If the user hasn't rated by 6 hours post-brew, they're not going to. The "Rate now" link disappears. The pending state expires at 24h per R4 spec. No nagging.

### Transition

- Recent → Afternoon: next app foreground after 2h elapsed. Crossfade 0.3s easeOut.
- Afternoon → Evening: next app foreground after 6h elapsed. Crossfade 0.3s easeOut.
- No animation if the app is already in the later phase when foregrounded.

---

## 7. Return Moments — `ReturnView.swift`

### The Gap

The user brews every day for two weeks, then disappears for five days. When they come back, Why Today fires as if nothing happened. "Steady." — same word, same weight, no acknowledgment that they left.

This is a missed moment. The return is emotionally loaded. The app should notice.

### Solution

A single line inserted into Why Today — Phase 1b (data line) — when the user returns after 3+ days of inactivity.

### Trigger

```swift
func daysSinceLastSession() -> Int {
    let lastDate = UserDefaults.standard.string(forKey: "lastWhyTodayDate") ?? ""
    guard !lastDate.isEmpty,
          let last = dateFromString(lastDate) else { return 0 }
    return Calendar.current.dateComponents([.day], from: last, to: Date()).day ?? 0
}

func isReturnSession() -> Bool {
    return daysSinceLastSession() >= 3
}
```

### What Changes

**Phase 1b (data line):** Prepends a return acknowledgment before the recovery data.

```swift
func whyToday_dataLine_withReturn(isReturn: Bool, daysSince: Int, /* existing params */) -> String {
    var parts: [String] = []

    if isReturn {
        parts.append("\(daysSince) days since the last one")
    }

    // ... existing data line logic from R4 ...
    // recovery, HRV delta, sleep note

    return parts.joined(separator: ". ") + "."
}
```

**Examples:**
- `"5 days since the last one. 72% recovery."`
- `"3 days since the last one. Checked in at 6/10."`

### Return-Specific Need Text (Phase 2)

When `isReturnSession() == true`, the need text pool adds three return-specific variants (one per tier). These are selected **instead of** the standard need text, not in addition to it.

```swift
let whyToday_needText_return: [String: String] = [
    "high": "You're back. And you came back strong. Pick up where you left off.",
    "mid":  "You're back. Mid-range signal. Normal morning. Start there.",
    "low":  "You're back. Low signal. Ease in. The routine remembers you."
]
```

**Selection logic:**
```swift
func selectWhyTodayNeed(tier: String) -> String {
    if isReturnSession() {
        return whyToday_needText_return[tier]!
    }

    // ... existing maturity-aware selection from Section 3 ...
}
```

### Return-Specific Bridge (Phase 3)

```swift
let whyToday_bridge_return = "Same place. Same morning. Let's go."
```

When `isReturnSession()`, this replaces the standard `"This is what meets you there."` bridge. Surprise moments still override if triggered (surprise check runs first, return bridge is the fallback for return sessions).

### What Doesn't Change on Return

- Tier word: same as always. "Rebuilding." doesn't become "Welcome back." The body state is what it is.
- Haptic: same.
- Animation timing: uses current maturity tier's timing (standard or shortened).
- Smart suppression: if the user was suppressed before leaving, the return resets the suppression. Leaving for 3+ days is a natural cooldown — they don't need an artificial one too.

**Suppression reset on return:**
```swift
func checkReturnReset() {
    if isReturnSession() {
        // Clear any active suppression — the break was long enough
        UserDefaults.standard.set("", forKey: "whyTodaySuppressedUntil")
        UserDefaults.standard.set(0, forKey: "consecutiveWhyTodaySkips")
        UserDefaults.standard.set(false, forKey: "whyTodayReintroduced")
    }
}
```

Call `checkReturnReset()` before `shouldShowWhyToday()` on app launch.

---

## 8. Integration with Rounds 1-4

Every extension in this document is **additive**. Existing strings, components, and logic from R4 are untouched.

| Extension | Touches | How |
|---|---|---|
| State-Bean Bridge Lines | `BeanCardView.swift` | New line below characterTag on first card only |
| Four-Node Chain | `WhyThisBrewView.swift` | New bean node inserted between profile and parameters |
| Why Today Maturation | `WhyTodayView.swift` | Need text selection gains maturity-awareness. New copy pools. |
| State-Aware Flavor Framing | `FlavorPreviewView.swift` | New intro line above existing sensory description |
| Pattern Vocabulary | Brew detail screen | New section below Why This Brew chain. One-time, dismissible. |
| Full-Day Emotional Arc | `PostBrewCardView.swift` | Post-brew card copy shifts by elapsed time. Three phases. |
| Return Moments | `WhyTodayView.swift` | Data line, need text, and bridge gain return variants |

### What Doesn't Change

Everything from R4's "What Doesn't Change" list. Plus:
- All 30 locked decisions
- All 15 brew taglines
- All 15 cup feelings
- All 15 sensory descriptions (framing line is prepended, not replacing)
- All 11 bean records
- All 8 surprise lines
- All 6 check-in options
- Bean rotation logic
- Smart suppression (extended by return reset, not replaced)
- All animations (new ones are additive)
- Navigation architecture
- Design tokens

---

## 9. New UserDefaults Keys — Complete

```swift
// Why Today maturation — need text rotation per maturity tier
"lastWhyToday_high_new"             // Int
"lastWhyToday_high_familiar"        // Int
"lastWhyToday_high_veteran"         // Int
"lastWhyToday_mid_new"              // Int
"lastWhyToday_mid_familiar"        // Int
"lastWhyToday_mid_veteran"          // Int
"lastWhyToday_low_new"              // Int
"lastWhyToday_low_familiar"         // Int
"lastWhyToday_low_veteran"          // Int

// Tier session tracking (for veteran copy)
"lastSessionDate_high"              // String (date)
"lastSessionDate_mid"               // String (date)
"lastSessionDate_low"               // String (date)
"tierSessionCount_high"             // Int
"tierSessionCount_mid"              // Int
"tierSessionCount_low"              // Int

// Pattern summary
"patternSummaryDismissed"           // Bool
```

**Note:** R4's existing keys are unchanged. The maturity-aware rotation keys replace the simpler `"lastWhyToday_high"` etc. — but the migration is trivial (ignore old keys, new keys start at 0).

---

## 10. New Animations — Complete

| Location | Element | Type | Duration | Easing | Delay | Trigger |
|---|---|---|---|---|---|---|
| Bean card | Bridge line | fadeIn | 0.2s | easeOut | 0.0s | With card appear |
| Why This Brew | Node appear | fadeIn + translateY(6→0) | 0.25s each | easeOut | 0.2s between | Detail screen load |
| Why This Brew | Vertical line draw | height expand | 0.15s each | easeOut | Synced with next node | Detail screen load |
| Pattern summary | Container appear | fadeIn | 0.3s | easeOut | 0.0s | First render |
| Post-brew card | Phase transition | crossfade | 0.3s | easeOut | 0.0s | App foreground |

---

## 11. Copy Audit — All New Strings

### State-Bean Bridge Lines (12 strings — 15 minus 3 empty Blank slots)
```swift
"Gentle origin on a strong morning. Room to appreciate the subtlety."
"Low intensity for a mid-range day. Stays out of your way."
"Gentle origin for a low morning. That's the match."
"Balanced bean on your best signal. Everyday standard, no waste."
"Reliable origin for a steady day. Nothing to prove."
"Clean bean on a rough morning. Honest cup, honest day."
"Complex origin backed by a full recovery. Built for the long session."
"Layered bean on a mid signal. Extraction does the heavy lifting."
"Dense origin on a low day. The dose is the bet. The bean holds up."
"High-intensity origin on peak signal. Everything's aligned."
"Sharp bean on a mid morning. You chose the intensity. The origin delivers it."
"Concentrated origin on low recovery. Not what the data suggests. Bean's ready if you are."
```

### Bean Pairing Rationale (13 strings — 11 beans, 2 dual-profile)
```swift
"Florals unlock at low dose. The gentlest origin matches the gentlest parameters."
"Natural sweetness at low concentration. Comfort without heaviness."
"Balanced sweetness through moderate extraction. The versatile origin."
"Simplicity is the strength at low extraction. No rough edges to process."
"Mineral backbone survives any method. At balanced parameters, it's the definition of reliable."
"Volcanic structure holds up under extended extraction. Gets richer the longer you steep."
"Natural sweetness makes moderate extraction feel indulgent. Effortless quality."
"Honey processing adds body that clean extraction makes transparent. Craft you taste without thinking."
"Extended steep pulls layers that keep your palate working for hours. Complexity is the point."
"Earthy body at full dose creates something dense and sustaining. A working coffee."
"Phosphoric acidity at high concentration. Controlled detonation. Bright, complex, demanding."
"Fruit intensity matches caffeine intensity at these parameters. Full signal, full flavor."
"Juicy complexity that concentration amplifies rather than overwhelms. Clarity at volume."
```

### Why Today — Familiar Need Text (9 strings)
```swift
"Full signal. You know what to do with this."
"Everything recovered. Use it."
"Best signal this week. Go."
"Mid-range. Same as most mornings. You know the drill."
"Not your best, not your worst. Standard input."
"Functional. Nothing more to read into."
"Low signal. You've seen this before. It passes."
"Rough one. Gentle input. You know the move."
"Your body's working on it. Don't fight it."
```

### Why Today — Veteran Need Text (9 strings, some with dynamic params)
```swift
"Peaked. Rare for you lately. Don't waste it."
"Full recovery. Hasn't happened in a few days. Go."
"Best signal since {lastPeakDay}. Act on it."
"Steady. Your most common state. Brezi knows what works here."
"Mid-range. You've brewed from this signal {midCount} times. Dialed."
"Average morning. That's not a criticism — your average is good."
"Low. Last time was {lastLowDay}. Same approach worked then."
"Rebuilding. {daysSinceLow} days since the last one. It passes."
"Low signal. Gentle parameters. You've done this."
```

### State-Aware Flavor Framing (12 strings — 15 minus 3 empty Blank slots)
```swift
"Strong morning. You chose gentle anyway. Here's what that tastes like."
"Mid-range signal. Soft approach. Here's the cup."
"Low morning. Gentle cup. This is what matches."
"Peaked and balanced. The standard cup for the standard you've set."
"Steady morning, steady cup. Nothing needs to be different."
"Rough morning, honest cup. Nothing heroic. Just present."
"Full recovery. Full extraction. This is what three hours of focus tastes like."
"Mid-signal, full commitment. The dose does the work."
"Low morning, heavy pour. The bet is the extraction carries you through."
"Peak signal, peak intensity. Everything's on. Here's what that tastes like."
"Mid-range signal, maximum brew. Your choice outranks your data today."
"Low recovery, full send. Not recommended. But you'll know exactly what you're tasting."
```

### Post-Brew Temporal Copy — Afternoon (5 strings)
```swift
"Brewed this morning. Gentle start, done its job."
"This morning's brew. Balanced, finished clean."
"Deep Work from this morning. Should still be holding."
"Voltage from this morning. If you're still feeling it, that's by design."
"This morning's brew. Your settings, your outcome."
```

### Post-Brew Temporal Copy — Evening (5 strings)
```swift
"Done for today. Tomorrow's brew depends on tonight."
"Today's cup is behind you. Sleep well — tomorrow's signal starts now."
"Session's over. Recovery starts when you shut down."
"Full send this morning. Give the system time to reset."
"Manual brew, logged. Tomorrow's a new set of variables."
```

### Return Moments — Need Text (3 strings)
```swift
"You're back. And you came back strong. Pick up where you left off."
"You're back. Mid-range signal. Normal morning. Start there."
"You're back. Low signal. Ease in. The routine remembers you."
```

### Return Moments — Bridge (1 string)
```swift
"Same place. Same morning. Let's go."
```

### Pattern Vocabulary — Templates (3 strings + 2 detail variants + 3 intro variants)
```swift
// Pattern lines (dynamic)
"{profileName} brewer."
"{tierName} mornings."
"{topBeanName}, mostly."  // or "No bean preference yet."

// Detail
"Hit right {N} out of {M} times on this profile. Your parameters are dialed."
"{M} brews on {profileName}. Still finding the sweet spot — and that's normal at this stage."

// Intro
"{N} brews in. Here's what Brezi sees:"
"{N} brews. Here's your pattern:"
"{N} brews. You're consistent. Here's the data:"

// UI
"YOUR PATTERN"
"Got it"
```

### Section Labels (2 strings)
```swift
"WHY THIS BREW"                     // (may already exist — confirm with Jensen)
"YOUR PATTERN"
```

---

## 12. Build Order — For Jensen

All seven extensions can be built **after** the R4 build is complete. None block R4 work. Suggested order based on dependency:

**Day 1 (post-R4):** Why Today Maturation (Section 3) + Return Moments (Section 7). Both modify `WhyTodayView.swift`. Build together to avoid merge conflicts. Add new UserDefaults keys. Add `recordWhyTodaySession()` call. Add `checkReturnReset()` call.

**Day 2:** State-Bean Bridge Lines (Section 1) + State-Aware Flavor Framing (Section 4). Both are single-line additions to existing components. Small diffs, no new views.

**Day 3:** Four-Node Chain (Section 2). New `WhyThisBrewView.swift` component or extension of existing detail screen. Bean pairing rationale dictionary. Node animation.

**Day 4:** Pattern Vocabulary (Section 5) + Full-Day Emotional Arc (Section 6). Pattern summary is a new section in the detail screen. Temporal arc modifies `PostBrewCardView`. Both are self-contained.

**Total: ~4 days of additional work.** No structural changes. No new Core Data entities. No new navigation. All UserDefaults.

---

## 13. Decisions — Round 5

31. State-bean bridge line connects body state to specific bean in one sentence. First card only. The Blank excluded.
32. Why This Brew chain gains a fourth node — the bean enters the reasoning the user sees. The Blank stays at three.
33. Why Today need text matures across three tiers: new (1-7), familiar (8-30), veteran (31+). Copy gets shorter, more confident, eventually references user's own history.
34. Veteran copy uses dynamic references (`{lastPeakDay}`, `{midCount}`) with automatic fallback to familiar tier if data is missing. Never show a template variable.
35. Flavor framing line (one sentence, Mono 12px, #888888) prepends existing sensory description. Sets the table. Doesn't compete with the meal.
36. Pattern summary is one-time, dismissible, fires at 15+ brews with 60%+ dominance on one profile. Three lines + detail. "Got it" kills it forever.
37. Post-brew card shifts through three temporal phases: recent (0-2h), afternoon (2-6h), evening (6h+). Rating CTA disappears after afternoon. No nagging.
38. Return sessions (3+ days away) get custom data line prefix, return-specific need text, and return bridge. Suppression state resets on return.
39. All new copy passes the text-message test. No therapy voice. No welcome-back-we-missed-you. "You're back. Mid-range signal. Normal morning. Start there."
40. All seven extensions are additive. Zero changes to R4's locked spec. Build after R4 ships.

---

*Seven extensions. Sixty-nine new copy strings. Zero structural changes. Everything builds on what's locked.*

*The gap between Round 4 and a shipping product isn't more screens or more features. It's the moments between the moments — the sentence that connects your body state to your bean, the copy that shortens as it learns you, the card that knows it's 3pm, the return that doesn't pretend you never left.*

*Round 4 built the machine. Round 5 teaches it to notice time passing.*

— Picasso
