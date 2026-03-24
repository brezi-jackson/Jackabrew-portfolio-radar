# Final Engineering Sign-Off — Brezi Coffee Layer v1 (Iteration 3)
**Author:** Elon (COO / Product & Growth) | March 20, 2026
**To:** Jensen (Engineering)
**CC:** Jackson (Product) · Picasso (Design)
**Status:** BUILD. This is the last document before code.

---

## 1. Is This Ready to Build?

Yes. Ship it.

Five rounds of design iteration. Fourteen brief requirements audited line-by-line. Seven extensions evaluated, four approved for v1, three deferred to v1.1. No open questions on architecture, copy, data, or interaction design. Every bean field is confirmed against Jackson's locked brief. Every user-facing string is paste-ready. Every conditional has a trigger and a fallback.

**What you're building:**
- Three-card coffee flow: Why Today (palette transition) → Bean Selection (3 roaster beans + General Profile) → Brew Settings (bean-adapted parameters)
- Bean marketplace with Browse, Detail, Subscribe & Save, and SFSafariViewController checkout
- "Your history with this bean" card (3+ brews threshold, CTA flip from "BREW THIS" to "BREW AGAIN")
- State-bean bridge lines on recommended bean card (Extension 1 — v1)
- Why Today maturation: new (Day 1-7) → familiar (Day 8-30) → veteran (Day 31+) with dynamic references (Extension 3 — v1)
- Full-day emotional arc: pre-brew → recent (0-2h) → afternoon (2-6h) → evening (6h+) with copy swaps (Extension 6 — v1)
- Return moments: 3+ day absence triggers return-specific Why Today copy and suppression reset (Extension 7 — v1)
- Price TBD handling with "NOTIFY ME" banner for Stitch and Corvus
- Subscribe & Save with `UserDefaults.stringArray(forKey: "subscribedBeans")` MVP state

**What you're NOT building (deferred to v1.1):**
- Four-node "Why This Brew" chain (Extension 2) — new detail screen UI, not critical path
- State-aware flavor framing lines (Extension 4) — detail screen enhancement
- Pattern vocabulary / one-time summary card (Extension 5) — needs 15+ brews to fire, irrelevant at launch

**Confidence level:** The spec survived five rounds without a structural change. Every addition since Round 2 has been copy, timing, or conditional logic — not new screens, navigation, or data models. The architecture is stable. The brief alignment is 14/14. Ship it.

---

## 2. Micro-Fixes to Apply While Coding

These are small enough that another design round would be wasteful. Apply them inline during implementation.

### Copy Fixes

| Location | Current | Fix To | Why |
|---|---|---|---|
| Bridge line — generic origin language | "Gentle origin for a low morning." | "Floral, bright origin for a low morning. Effortless match." (Regent) / "Dense, structured origin for a peak morning. No apologies needed." (Stitch) / "Wine-dark origin for a deep work day. Linger over this one." (Corvus) | Bridge lines should borrow each bean's emotional tag vocabulary. Generic "Gentle origin" / "High-intensity origin" makes all three beans sound like the same algorithm. The bridge should code-switch per roaster. |
| Post-brew afternoon, Tide Pool | "Brewed this morning. Gentle start, done its job." | "Brewed this morning. Gentle start. That's all it needed to be." | "Done its job" is what you say about a vacuum. "That's all it needed to be" reframes simplicity as intentional. Matches Tide Pool's register. |

### Behavioral Micro-Fixes

**Maturation increment trigger:** Increment `whyTodaySessionCount` on Why Today **display**, not on app open or skip. A skipped Why Today should not count toward the familiarity that earns shorter copy. Implementation: call the increment inside `recordWhyTodayInteraction(wasSkip: false)`, not inside `shouldShowWhyToday()`.

**Veteran template resolution:** Resolve `{lastPeakDay}`, `{midCount}`, `{daysSinceLow}` at **display time**, never at storage time. Store templates as templates. If "Best signal since Tuesday" gets cached in UserDefaults and displayed on Thursday, the temporal awareness becomes temporal confusion. Render-time only. Non-negotiable.

**Return session suppression reset:** Call `checkReturnReset()` **before** `shouldShowWhyToday()` on app launch. Order matters. The reset clears any active suppression so the returning user always sees Why Today fresh.

**General Settings shortcut gate:** "Use general settings >" secondary CTA on Card 3 visible only after `completedBrewFlows >= 3`. Counter increments only on full-flow completion (Why Today → Bean → Settings → Brew). New users must experience the full palette transition before the shortcut appears.

### Visual Micro-Fixes

**Bean card left border on first card:** 3px, using the *active profile's* gradient start color — not the bean's associated profile. A Corvus bean (which serves both Deep Work and Voltage) should show the gradient of whichever profile is currently active.

**Corvus roast level:** The `RoastLevel` enum uses `.medium`. Jackson's brief says "Medium-Light." At 0.45 on the slider, it reads as medium-light visually. Keep `.medium` in code — adding `.mediumLight` ripples through the adaptation note matrix. The slider position is the source of truth.

**Gradient banding quality gate (Day 1):** Test flavor sliders on iPhone 11/SE. 5-color gradient stops default. If LCD banding visible, reduce to 3 stops. Binary decision — no ambiguity.

### Template Variable Safety Rule

**Never render a template variable to the user.** `{lastPeakDay}` showing as literal text would be the single most trust-destroying bug in the app. Write unit tests for every veteran copy variant with nil data. The fallback to familiar-tier copy must be enforced in code, not just in spec prose.

Test matrix for veteran copy:
- User reaches Day 31 having never peaked (all mid/low) → `{lastPeakDay}` is nil → fallback fires
- User reaches Day 31 with only one tier visited → partial nil data → fallback fires
- User reaches Day 31 with all tiers visited → all data populated → veteran copy renders
- User data is cleared mid-session → graceful degradation to familiar tier

---

## 3. New Data Model Fields — BeanSeed Additions

The existing `BeanSeed` struct needs these fields to support the coffee marketplace layer. The current struct has 16 fields (name through sortOrder). Add 9 fields:

```swift
struct BeanSeed {
    // === EXISTING FIELDS (keep all 16) ===
    let name: String
    let origin: String
    let region: String
    let processing: String
    let roastLevel: String
    let flavorFeeling: String
    let characterTag: String
    let pairingNote: String
    let originStory: String
    let originHeadline: String
    let originOneLiner: String
    let sensoryCue: String
    let profileKeys: [String]
    let altitude: String
    let harvest: String
    let sortOrder: Int16

    // === NEW FIELDS (add these 9) ===
    let roasterName: String          // "STITCH COFFEE", "REGENT COFFEE", "CORVUS COFFEE ROASTERS"
    let price: Double?               // 27.00 for Regent, nil for Stitch/Corvus (TBD)
    let shopifyUrl: String?          // "https://brezicoffee.com/products/regent-ethiopia-guji-hambela" or nil
    let shopifyProductID: String?    // "10151173947683" for Regent, nil for others
    let roastScore: Double           // 0.0–1.0 (Stitch: 0.9, Regent: 0.15, Corvus: 0.45)
    let sweetnessScore: Double       // 0.0–1.0 (Stitch: 0.55, Regent: 0.8, Corvus: 0.85)
    let acidityScore: Double         // 0.0–1.0 (Stitch: 0.2, Regent: 0.75, Corvus: 0.5)
    let tastingNoteIcons: [String]   // SF Symbol names: ["cup.and.saucer.fill", "leaf.fill", ...]
    let bagImageName: String         // Asset catalog name: "stitch_dark_field", "regent_ethiopia_guji", "corvus_sadayana"
}
```

**Also add to BeanSeed (from Picasso's locked spec):**

```swift
    let emotionalTag: String         // "Dense, structured, no apologies."
    let showEmotionalTagOnCard: Bool // true for Stitch, false for Regent/Corvus
    let imageBgTint: String          // "#2E2022" (Stitch), "#F5F0E8" (Regent), "#2A2418" (Corvus)
    let cardVariant: String          // "dark" (Stitch), "light" (Regent), "light" (Corvus)
    let bestForProfiles: [String]    // ["deep_work", "voltage"] — may overlap with profileKeys
    let beanSubtitle: String         // "" (Stitch), "Hambela Washed" (Regent), "Winey Anaerobic Natural Ateng" (Corvus)
    let roasterLocation: String      // "Singapore" (Stitch), "" (Regent), "Denver, Colorado" (Corvus)
    let description: String          // Full description paragraph
```

**Note:** The existing `profileKeys` field maps beans to brew profiles for the 11-bean emotional layer. The new `bestForProfiles` field maps the 3 roaster beans to profiles for the marketplace recommendation engine. For the 3 roaster beans, these should match. For the 8 origin beans, `bestForProfiles` can be empty — they don't appear in the marketplace.

### BrewLog Additions (for bean tracking)

```swift
// Add to existing BrewLog entity
@NSManaged var selectedBeanID: String?      // ref to BeanSeed.name (or new id field)
@NSManaged var selectedBeanRoaster: String? // "REGENT COFFEE" — denormalized for quick lookup
@NSManaged var brewSettingsAdapted: Bool     // true if bean adjustments were applied
```

---

## 4. Priority Build Order

### Phase 1 — Foundation (Days 1-2)

Jensen opens Xcode. Nothing else matters until this compiles.

| Task | Details | Est. |
|------|---------|------|
| `Localizable.strings` setup | All 85+ coffee-side strings. Paste from Picasso's spec verbatim. | 0.5d |
| Color + font extensions | Coffee palette: `#FAF6F0`, `#D4A96A`, `#5B8C5A`, `#1A1714`, `#8C8279`, `#B5A99A`, `#E5DDD3`, `#3A3530`. Per-roaster tints: `#2E2022`, `#F5F0E8`, `#2A2418`. | 0.25d |
| Core Data additions | Add `selectedBeanID`, `selectedBeanRoaster`, `brewSettingsAdapted` to `BrewLog`. | 0.25d |
| `BeanSeed` expansion | Add 9+ new fields to struct. Seed 3 roaster beans with real data (Section 5 below). | 0.5d |
| Gradient banding quality gate | Test 5-color flavor slider on iPhone 11/SE. Binary: keep 5 or reduce to 3. | 0.5d |

### Phase 2 — Bean Selection & Cards (Days 3-4)

| Task | Details | Est. |
|------|---------|------|
| `BeanSelectionView.swift` | Horizontal scroll, 3 bean cards + General Profile card. Header copy per profile. | 0.5d |
| `BeanCardView.swift` | Three contexts: `.selection`, `.marketplace`, `.detail`. Dark/light variants. Roaster name, bean name, tasting note icons, flavor sliders, emotional tag (Stitch only). Image area 280pt with `imageBgTint`. | 1d |
| State-bean bridge line | One sentence below characterTag on recommended (first) bean card only. 12 variants: 3 beans × 4 states (high/mid/low/blank). Use roaster emotional tag vocabulary per micro-fix above. | 0.25d |
| General Settings shortcut gate | "Use general settings >" visible only after `completedBrewFlows >= 3`. | 0.25d |

### Phase 3 — Bean Detail & Marketplace (Days 5-6)

| Task | Details | Est. |
|------|---------|------|
| `BeanDetailView.swift` | Full bean info: image, origin, tasting notes, flavor sliders (labeled), serving infographic, brew pairing chips. | 0.75d |
| `BeanBrewHistoryCard` | 3+ brews threshold. "You've brewed this 5×. Best day: recovery 82%." CTA flip. | 0.25d |
| Marketplace browse | Bean cards in `.marketplace` context. Filter by profile. Price display / TBD state. | 0.5d |
| Subscribe & Save | Banner, `UserDefaults` subscription state, "SUBSCRIBED ✓" button state. | 0.25d |
| `SFSafariViewController` | Add to Cart / Subscribe flows open Shopify in-app browser. | 0.25d |

### Phase 4 — Palette Transition & Why Today Maturation (Days 7-8)

| Task | Details | Est. |
|------|---------|------|
| `PaletteTransitionView` | `#0F0F0F` → `#FAF6F0` crossfade around typewriter text. `WhyTodayConfig` tuning struct. Once-per-day gating. Skip link. | 0.5d |
| Why Today maturation | Three tiers: new (1-7), familiar (8-30), veteran (31+). 9 familiar + 9 veteran need text variants. Session count tracking. | 0.5d |
| Veteran dynamic references | `{lastPeakDay}`, `{midCount}`, `{daysSinceLow}` — display-time resolution. Nil-fallback to familiar tier. Unit tests for all nil combinations. | 0.5d |
| Return moments | `daysSinceLastSession() >= 3` detection. 3 return need text strings + 1 return bridge. `checkReturnReset()` before `shouldShowWhyToday()`. | 0.25d |
| Full-day emotional arc | Three post-brew phases. 10 new copy strings (5 afternoon + 5 evening). Phase transition on app foreground. Rating CTA hides in evening phase. | 0.25d |

### Phase 5 — Integration & TestFlight (Day 9)

| Task | Details | Est. |
|------|---------|------|
| Brew settings bean adaptation | `BeanAdjustment` deltas applied when bean is selected. Adaptation note at hinge position inside card. | 0.25d |
| End-to-end flow testing | Health check-in → Why Today → Bean Selection → Brew Settings → Active Brew → Post-Brew Rating. Full loop. | 0.5d |
| TestFlight prep | Clean build, no warnings, no template variables in any copy path. | 0.25d |

**Total: ~9 working days.** Realistic TestFlight target: first week of April.

### What Can Slip Without Damage

Cut in this order (last = cut first):

1. **Full-day emotional arc** — post-brew card can stay static. Users don't expect it to change at 3pm.
2. **Return moments** — app works without noticing you left. Emotional, not functional.
3. **Bridge lines** — connection between state and bean is implicit. Making it explicit is better but not structural.
4. **Why Today maturation** — existing copy works at any tenure. Shorter copy over time is earned familiarity, not a requirement.

**Do not cut:** Bean cards, marketplace, Subscribe & Save, palette transition, brew history card, or bean-adapted settings. These are the product.

---

## 5. Exact Shopify Product URLs for the 3 Beans

### Regent Coffee — Ethiopia Guji Hambela Washed

| Field | Value |
|-------|-------|
| **Store** | brezicoffee.com |
| **Product ID** | `10151173947683` |
| **Product URL** | `https://brezicoffee.com/products/ethiopia-guji-hambela-washed` |
| **Price** | $27.00 |
| **Status** | LIVE — ready for SFSafariViewController |

### Stitch Coffee — Dark Field

| Field | Value |
|-------|-------|
| **Store** | brezicoffee.com (pending) |
| **Product ID** | TBD — Jackson to confirm |
| **Product URL** | TBD |
| **Price** | TBD |
| **Status** | NOT LIVE — use Price TBD state with "NOTIFY ME" banner |

### Corvus Coffee Roasters — Sadayana Winey

| Field | Value |
|-------|-------|
| **Store** | brezicoffee.com (pending) |
| **Product ID** | TBD — Jackson to confirm |
| **Product URL** | TBD |
| **Price** | TBD |
| **Status** | NOT LIVE — use Price TBD state with "NOTIFY ME" banner |

**Implementation:** When `bean.price == nil`, show the contextual banner: *"These beans are on deck. Tap NOTIFY ME and we'll ping you when they drop."* Toast on tap: *"We'll let you know when {beanName} drops."* Subscribe banner qualifier: *"Available on select beans."*

**Action item for Jackson:** Get Stitch and Corvus Shopify product IDs and pricing before Phase 3 starts (~Day 5). The marketplace UI works without them (Price TBD state handles it), but Subscribe & Save only functions on Regent until the other two are live.

### Bean Seed Data — All 3 Roaster Beans

```swift
// Stitch Coffee — Dark Field
BeanSeed(
    name: "Dark Field",
    origin: "Blend", region: "Sydney, Australia",
    processing: "Blend", roastLevel: "Dark",
    flavorFeeling: "Dense, structured, no apologies. For when you mean business.",
    characterTag: "Dense, structured, no apologies",
    pairingNote: "Dark roast intensity survives cold extraction. At high dose, this is espresso energy in a cold cup.",
    originStory: "The darkest roast in the Stitch Coffee collection. For the Stoics among us. Inspired by classic Italian roasts.",
    originHeadline: "Classic Italian. No compromises.",
    originOneLiner: "The darkest roast. For the Stoics among us.",
    sensoryCue: "dark chocolate, toasted almond, caramel",
    profileKeys: ["deep_work", "voltage"],
    altitude: "N/A", harvest: "N/A",
    sortOrder: 1,
    // New fields
    roasterName: "STITCH COFFEE",
    price: nil,
    shopifyUrl: nil,
    shopifyProductID: nil,
    roastScore: 0.9,
    sweetnessScore: 0.55,
    acidityScore: 0.2,
    tastingNoteIcons: ["chocolate_bar", "almond", "caramel_drip"],
    bagImageName: "stitch_dark_field",
    emotionalTag: "Dense, structured, no apologies.",
    showEmotionalTagOnCard: true,
    imageBgTint: "#2E2022",
    cardVariant: "dark",
    bestForProfiles: ["deep_work", "voltage"],
    beanSubtitle: "",
    roasterLocation: "Singapore",
    description: "The darkest roast in the Stitch Coffee collection. For the Stoics among us. Inspired by classic Italian roasts."
)

// Regent Coffee — Ethiopia Guji Hambela Washed
BeanSeed(
    name: "Ethiopia Guji Hambela",
    origin: "Ethiopia", region: "Guji — Hambela",
    processing: "Washed", roastLevel: "Light",
    flavorFeeling: "The cleanest thing you'll drink today. Floral, bright, effortless.",
    characterTag: "Floral, bright, effortless",
    pairingNote: "Bright, floral, stone fruit. Exceptional cold clarity. Gentle extraction unlocks the florals.",
    originStory: "Ethiopia, Guji — Hambela. Varietal: Paru Biyu. Washed process strips everything back to the seed, leaving only what the altitude put there.",
    originHeadline: "Ethiopian clarity.",
    originOneLiner: "Bright, floral, stone fruit. Exceptional cold clarity.",
    sensoryCue: "cocoa, jasmine, strawberry, lime",
    profileKeys: ["tide_pool", "first_light"],
    altitude: "1,800-2,200 masl", harvest: "Oct-Jan",
    sortOrder: 1,
    // New fields
    roasterName: "REGENT COFFEE",
    price: 27.00,
    shopifyUrl: "https://brezicoffee.com/products/ethiopia-guji-hambela-washed",
    shopifyProductID: "10151173947683",
    roastScore: 0.15,
    sweetnessScore: 0.8,
    acidityScore: 0.75,
    tastingNoteIcons: ["cocoa_pod", "jasmine_flower", "strawberry", "lime_wedge"],
    bagImageName: "regent_ethiopia_guji",
    emotionalTag: "Floral, bright, effortless.",
    showEmotionalTagOnCard: false,
    imageBgTint: "#F5F0E8",
    cardVariant: "light",
    bestForProfiles: ["tide_pool", "first_light"],
    beanSubtitle: "Hambela Washed",
    roasterLocation: "",
    description: "Bright, floral, stone fruit. Exceptional cold clarity."
)

// Corvus Coffee Roasters — Sadayana Winey
BeanSeed(
    name: "Sadayana Winey",
    origin: "West Java, Indonesia", region: "Indragiri — Papandayan",
    processing: "72-hour natural anaerobic fermentation", roastLevel: "Medium",
    flavorFeeling: "Wine-dark. Fruit-forward. The kind of brew you linger over.",
    characterTag: "Wine-dark. Fruit-forward.",
    pairingNote: "A striking example of innovation from Indonesia. Wine-like when cupped — shared with pride after dinner.",
    originStory: "Sadayana Washing Station, Indragiri, near Papandayan volcano. 72-hour natural anaerobic fermentation in sealed barrels, dried on raised beds and patios. West Java's answer to Ethiopian naturals.",
    originHeadline: "Indonesian innovation.",
    originOneLiner: "Wine-like. Anaerobic. Volcanic terroir.",
    sensoryCue: "raspberry yogurt, cherry syrup, dark chocolate",
    profileKeys: ["deep_work", "voltage"],
    altitude: "1,200-1,500 masl", harvest: "May-Sep",
    sortOrder: 2,
    // New fields
    roasterName: "CORVUS COFFEE ROASTERS",
    price: nil,
    shopifyUrl: nil,
    shopifyProductID: nil,
    roastScore: 0.45,
    sweetnessScore: 0.85,
    acidityScore: 0.5,
    tastingNoteIcons: ["raspberry", "cherry", "chocolate_bar"],
    bagImageName: "corvus_sadayana",
    emotionalTag: "Wine-dark. Fruit-forward. The kind of brew you linger over.",
    showEmotionalTagOnCard: false,
    imageBgTint: "#2A2418",
    cardVariant: "light",
    bestForProfiles: ["deep_work", "voltage"],
    beanSubtitle: "Winey Anaerobic Natural Ateng",
    roasterLocation: "Denver, Colorado",
    description: "A striking example of innovation from Indonesia. Wine-like when cupped — shared with pride after dinner."
)
```

---

## Final Word

Three design iterations. Five rounds each. Forty-two total decisions audited against the locked brief. Three roaster partnerships confirmed. One purchasable bean live on Shopify. Two more pending pricing.

The spec builds a coffee marketplace inside a health-aware cold brew app. The v1 extensions — bridge lines, maturation, temporal arc, return moments — make it feel alive from Day 1. The v1.1 extensions — four-node chain, flavor framing, pattern vocabulary — add depth for users who stay past week 2.

Jensen: build Phase 1-2 first. Get the bean cards on screen. Get the palette transition feeling right. Everything after that is compounding on a foundation that's already locked.

Jackson: get Stitch and Corvus pricing before Day 5. The marketplace ships either way, but three purchasable beans is better than one.

Picasso: one copy pass on the bridge lines. Borrow each roaster's emotional tag vocabulary. The rest is done.

Lock it. Build it. Ship it.

— Elon
