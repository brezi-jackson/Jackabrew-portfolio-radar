# Brezi iOS App — Visual Coffee Layer & Marketplace v1.0
**Author:** Picasso (Product Design) | March 20, 2026 | Iteration 3 — Round 1
**For:** Jackson (Product) · Jensen (Engineering) · Elon (Growth/Copy)
**Status:** DRAFT. Covers the coffee-side visual redesign, bean-first flow, marketplace, and dual palette system.

---

## 0. What This Document Does

Jackson's iteration 3 feedback calls for a fundamental split: the health side stays WHOOP — dark, precise, numerical. The coffee side becomes a **specialty coffee brand experience** — warm, illustrated, cheerful, visual.

This document redesigns everything the user touches once their health data has been read and the brew profile has been suggested. Six deliverables:

1. **Bean Card Visual Design** — Full component spec with the 3 real roaster beans
2. **Bean-First Flow** — 4-screen redesign: health → profile → bean selection → brew settings
3. **Serving Infographic Card** — Visual clock arc, cup icons, timing chart
4. **Flavor Visualization** — Horizontal slider bars + tasting note illustrations
5. **Marketplace Screen** — Bean Shop with filtering, subscription, Shopify links
6. **Dual Palette** — Two connected color worlds with a clear crossover point

**What this does NOT touch:** Onboarding (Screens 1-5), health data card (Card 1), Brezi's Read (Card 2), device hub, settings, emotional layer (R4/R5). Those stay locked.

---

## 1. Dual Palette System — The Two Worlds

### The Principle

The app is two brands sharing one shell. The health side is a wearable dashboard. The coffee side is a specialty roaster's shelf. They meet at one seam: the brew profile suggestion. Everything above the seam is cool. Everything below is warm.

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

**Where it lives:** Dashboard Cards 1-2 (Your Data, Brezi's Read), onboarding, device hub, settings, Why Today, all health metrics.

### Coffee Side Palette (NEW)

```swift
// COFFEE SIDE — warm, tactile, specialty-roaster feel
let coffee_bg             = Color(hex: "#FAF6F0")  // warm cream — the "shelf"
let coffee_surface        = Color(hex: "#FFFFFF")  // clean white card
let coffee_card_bg        = Color(hex: "#F5F0E8")  // warm off-white for nested elements
let coffee_card_dark      = Color(hex: "#2A2520")  // dark roast card variant (for dark roast beans)
let coffee_text_primary   = Color(hex: "#1A1714")  // almost-black, warm undertone
let coffee_text_secondary = Color(hex: "#8C8279")  // warm grey — replaces #888888 on coffee side
let coffee_text_tertiary  = Color(hex: "#B5A99A")  // lighter warm grey for metadata
let coffee_accent         = Color(hex: "#D4A96A")  // warm amber — the "gold" of specialty coffee
let coffee_accent_hover   = Color(hex: "#C49A5A")  // amber darkened for pressed states
let coffee_green          = Color(hex: "#5B8C5A")  // muted sage — not neon, not clinical
let coffee_tag_bg         = Color(hex: "#E8DFD2")  // tag/chip background
let coffee_divider        = Color(hex: "#E5DDD3")  // warm divider line
```

### The Seam — Where Worlds Meet

The transition happens at **Card 3 (Your Brew)**. Card 3 itself is the bridge:

```
┌─ HEALTH WORLD ──────────────────────┐
│  Card 1: Your Data        (#000000) │  dark, mono, metrics
│  Card 2: Brezi's Read     (#1A1A1A) │  dark, blue accent
├─ THE SEAM ──────────────────────────┤
│  Profile Badge             (#0F0F0F) │  dark card, profile name
│  "Brezi suggests: First Light"       │  last dark element
├─ COFFEE WORLD ──────────────────────┤
│  Bean Selection            (#FAF6F0) │  warm cream background
│  Bean Cards                (#FFFFFF) │  warm white cards
│  Flavor Sliders            (#F5F0E8) │  warm off-white
│  Marketplace               (#FAF6F0) │  warm cream
└──────────────────────────────────────┘
```

The transition is a **background color crossfade** from `#000000` to `#FAF6F0` as the user scrolls past the profile badge. This happens in the bean-first flow (Section 3), not on the dashboard itself. The dashboard stays dark. The coffee world opens when you tap into it.

### Typography on Coffee Side

```swift
// Coffee side uses the same type scale but with warm-toned colors
// and adds one new style for roaster names
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

## 2. Bean Card Visual Design — `BeanCardView.swift`

### The Card

Each bean card is a self-contained product card. It lives in the bean selection screen (flow Screen B), the marketplace, and the brew detail screen. One component, three contexts.

The card has **two visual variants** based on roast level:
- **Light/Medium roast:** white card on cream background
- **Dark roast:** dark card (`#2A2520`) — inverts the text colors

### Layout — Light Roast Variant (Regent, Corvus)

```
┌─────────────────────────────────────┐
│                                     │  bg: #FFFFFF
│  ┌─────────────────────────────┐    │  radius: 20pt
│  │                             │    │  shadow: coffee_card_shadow
│  │      [BEAN BAG IMAGE]       │    │
│  │      220 × 220pt            │    │  AsyncImage placeholder: #F5F0E8
│  │      contentMode: .fit      │    │  with subtle grain texture overlay
│  │                             │    │
│  └─────────────────────────────┘    │  image container: #F5F0E8, radius: 16pt
│                                     │  horizontal padding: 16pt, top: 16pt
│  ┌──────────────────────┐           │
│  │  REGENT COFFEE       │           │  t_roaster_name, #8C8279, ALL CAPS
│  └──────────────────────┘           │  tracking: 15%, top padding: 16pt
│                                     │
│  Ethiopia Guji                      │  t_bean_name, #1A1714
│  Hambela Washed                     │  line-height: 1.2
│                                     │  top padding: 4pt
│  ┌──────┐ ┌──────┐ ┌──────┐        │
│  │  🍫  │ │  🌸  │ │  🍓  │        │  TASTING NOTE ICONS
│  │Cocoa │ │Jasmin│ │Straw-│        │  See Section 5 for full spec
│  │      │ │  e   │ │berry │        │
│  └──────┘ └──────┘ └──────┘        │  top padding: 12pt
│                                     │
│  ROAST   ░░▓░░░░░░░░░░░░░░░░       │  FLAVOR SLIDERS
│  SWEET   ░░░░░░░░▓░░░░░░░░░░       │  See Section 5 for full spec
│  ACID    ░░░░░░░░░░░▓░░░░░░░       │  top padding: 12pt
│                                     │
│  ┌──────────────────────────────┐   │
│  │  ★ RECOMMENDED FOR YOU      │   │  RECOMMENDATION TAG
│  └──────────────────────────────┘   │  (conditional — see spec below)
│                                     │  top padding: 12pt
│  ┌─────────┐                        │
│  │  $27.00 │          ┌──────────┐  │
│  └─────────┘          │ ADD TO   │  │  PRICE + CTA
│                       │  CART →  │  │
│                       └──────────┘  │  top padding: 16pt
│                                     │  bottom padding: 20pt
└─────────────────────────────────────┘
```

### Layout — Dark Roast Variant (Stitch)

Same structure. Inverted palette:

```
┌─────────────────────────────────────┐
│                                     │  bg: #2A2520
│  ┌─────────────────────────────┐    │  radius: 20pt
│  │                             │    │  shadow: coffee_card_shadow_dark
│  │      [BEAN BAG IMAGE]       │    │
│  │      Stitch bear + Fiat     │    │  AsyncImage placeholder: #3A3530
│  │                             │    │
│  └─────────────────────────────┘    │  image container: #3A3530, radius: 16pt
│                                     │
│  STITCH COFFEE                      │  t_roaster_name, #B5A99A, ALL CAPS
│                                     │
│  Dark Field                         │  t_bean_name, #FFFFFF
│  Blend                              │
│                                     │
│  ┌──────┐ ┌──────┐ ┌──────┐        │
│  │  🍫  │ │  🌰  │ │  🍮  │        │  icons on dark bg
│  │Dark  │ │Toast-│ │Cara- │        │  labels: #B5A99A
│  │Choc  │ │ed    │ │mel   │        │
│  │      │ │Almond│ │      │        │
│  └──────┘ └──────┘ └──────┘        │
│                                     │
│  ROAST   ░░░░░░░░░░░░░░░░░▓░       │  slider track: #3A3530, fill: #D4A96A
│  SWEET   ░░░░░░░░▓░░░░░░░░░░       │
│  ACID    ░▓░░░░░░░░░░░░░░░░░       │
│                                     │
│  ┌──────────────────────────────┐   │
│  │  ★ RECOMMENDED FOR YOU      │   │  tag bg: rgba(212,169,106,0.2)
│  └──────────────────────────────┘   │  text: #D4A96A
│                                     │
│  ┌─────────┐                        │
│  │  TBD    │          ┌──────────┐  │
│  └─────────┘          │ ADD TO   │  │  button bg: #D4A96A, text: #2A2520
│                       │  CART →  │  │
│                       └──────────┘  │
│                                     │
└─────────────────────────────────────┘
```

### Component Spec — `BeanCardView.swift`

```swift
struct BeanCardSpec {
    // Container
    let cardRadius: CGFloat = 20
    let cardPadding: CGFloat = 16          // inner horizontal
    let cardPaddingTop: CGFloat = 16
    let cardPaddingBottom: CGFloat = 20

    // Image container
    let imageHeight: CGFloat = 220
    let imageRadius: CGFloat = 16
    let imagePlaceholderLight = Color(hex: "#F5F0E8")
    let imagePlaceholderDark  = Color(hex: "#3A3530")

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

    // Tasting notes row
    let tastingNoteTopPadding: CGFloat = 12
    let tastingNoteIconSize: CGFloat = 36
    let tastingNoteGap: CGFloat = 12
    // See Section 5 for full tasting note spec

    // Flavor sliders
    let sliderTopPadding: CGFloat = 12
    // See Section 5 for full slider spec

    // Recommendation tag
    let tagTopPadding: CGFloat = 12
    let tagHeight: CGFloat = 28
    let tagRadius: CGFloat = 8
    let tagFont = Font.system(size: 11, weight: .semibold)  // ALL CAPS
    let tagBgLight    = Color(hex: "#E8DFD2")
    let tagTextLight  = Color(hex: "#8C8279")
    let tagBgDark     = Color(hex: "D4A96A").opacity(0.2)
    let tagTextDark   = Color(hex: "#D4A96A")
    let tagIcon       = "star.fill"  // SF Symbol, 10pt, same color as text

    // Price
    let priceFont = Font.system(size: 18, weight: .bold, design: .monospaced)
    let priceTopPadding: CGFloat = 16
    let priceColorLight = Color(hex: "#1A1714")
    let priceColorDark  = Color(hex: "#FFFFFF")

    // Add to Cart button
    let cartButtonHeight: CGFloat = 44
    let cartButtonRadius: CGFloat = 12
    let cartButtonFont = Font.system(size: 14, weight: .bold)  // ALL CAPS
    let cartButtonBgLight    = Color(hex: "#1A1714")
    let cartButtonTextLight  = Color(hex: "#FFFFFF")
    let cartButtonBgDark     = Color(hex: "#D4A96A")
    let cartButtonTextDark   = Color(hex: "#2A2520")

    // Shadow
    let cardShadowLight = Shadow(color: .black.opacity(0.06), radius: 12, x: 0, y: 4)
    let cardShadowDark  = Shadow(color: .black.opacity(0.3), radius: 16, x: 0, y: 6)
}
```

### Recommendation Tag Logic

The tag `"★ RECOMMENDED FOR YOU"` only appears when the bean matches the user's current brew profile suggestion.

```swift
func showRecommendationTag(bean: Bean, suggestedProfile: BrewProfile) -> Bool {
    return bean.bestForProfiles.contains(suggestedProfile.key)
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
    roastLevel: .dark,            // triggers dark card variant
    origin: "Blend — globally sourced",
    process: "Espresso-oriented",
    description: "The darkest roast in the STITCH COFFEE collection. For the Stoics among us. Inspired by classic Italian roasts. Tailored for a strong start to the day.",
    emotionalTag: "For when you mean business. Dense, structured, no apologies.",
    tastingNotes: [
        TastingNote(icon: "icon_dark_chocolate", label: "Dark Chocolate"),
        TastingNote(icon: "icon_toasted_almond", label: "Toasted Almond"),
        TastingNote(icon: "icon_caramel", label: "Caramel")
    ],
    flavorProfile: FlavorProfile(roast: 0.9, sweetness: 0.5, acidity: 0.15),
    bestForProfiles: ["deep_work", "voltage"],
    price: nil,                    // TBD — display "COMING SOON"
    shopifyURL: nil,               // TBD
    imageAsset: "stitch_dark_field_bag",  // bear in red Fiat 500
    cardVariant: .dark
)
```

#### Bean 2: Corvus Coffee Roasters — Ethiopia Guji Natural (Barrel Aged)

```swift
let corvus_guji = Bean(
    id: "corvus_ethiopia_guji_barrel",
    roasterName: "CORVUS COFFEE ROASTERS",
    roasterLocation: "Denver, Colorado",
    beanName: "Ethiopia Guji",
    beanSubtitle: "Barrel Aged · Natural",
    roastLevel: .light,
    origin: "Ethiopia, Guji Zone — Gigesa washing station",
    process: "Natural, Barrel Aged",
    altitude: "1950–2200 masl",
    varietal: "Mixed Heirloom",
    grade: "Grade 1",
    score: "92pt Coffee Review",
    description: "Barrel Aged Ethiopia Guji Natural Processed Coffee. Explosive fruit. A whiskey finish that lingers. The most complex thing in the lineup.",
    emotionalTag: "Explosive fruit. A whiskey finish that lingers. The most complex thing in the lineup.",
    tastingNotes: [
        TastingNote(icon: "icon_whiskey", label: "Whiskey"),
        TastingNote(icon: "icon_berry", label: "Berry"),
        TastingNote(icon: "icon_cherry", label: "Cherry")
    ],
    flavorProfile: FlavorProfile(roast: 0.2, sweetness: 0.75, acidity: 0.7),
    bestForProfiles: ["voltage"],
    price: nil,                    // TBD
    shopifyURL: nil,               // TBD
    imageAsset: "corvus_guji_barrel_bag",  // ornate gold mandala on black
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
    tastingNotes: [
        TastingNote(icon: "icon_cocoa", label: "Cocoa"),
        TastingNote(icon: "icon_jasmine", label: "Jasmine"),
        TastingNote(icon: "icon_strawberry", label: "Strawberry")
    ],
    flavorProfile: FlavorProfile(roast: 0.2, sweetness: 0.6, acidity: 0.65),
    bestForProfiles: ["tide_pool", "first_light"],
    price: 27.00,
    shopifyURL: "https://brezicoffee.com",  // product ID: 10151173947683
    shopifyProductID: "10151173947683",
    imageAsset: "regent_guji_hambela_bag",  // art deco line illustration
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
    let tastingNotes: [TastingNote]
    let flavorProfile: FlavorProfile
    let bestForProfiles: [String]
    var price: Double? = nil
    var shopifyURL: String? = nil
    var shopifyProductID: String? = nil
    let imageAsset: String
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

### Bean Card — Expanded Detail View

Tapping a bean card pushes to `BeanDetailView.swift`. This is a full-screen scroll view on the warm `#FAF6F0` background.

```
┌─────────────────────────────────────┐
│  [←]              bre:zi        🛒  │  NavBarView (warm variant)
│─────────────────────────────────────│  bg: #FAF6F0 (not #000000)
│                                     │  back arrow + cart icon
│  ┌─────────────────────────────────┐│
│  │                                 ││
│  │        [BEAN BAG IMAGE]         ││  full-width image
│  │         300 × 300pt             ││  bg: #F5F0E8
│  │         contentMode: .fit       ││
│  │                                 ││
│  └─────────────────────────────────┘│  radius: 0 (full bleed)
│                                     │
│  CORVUS COFFEE ROASTERS             │  t_roaster_name, #8C8279
│  Denver, Colorado                   │  t_bean_body, 12px, #B5A99A
│                                     │
│  Ethiopia Guji                      │  CondensedBold, 28px, #1A1714
│  Barrel Aged · Natural              │  Regular, 16px, #8C8279
│                                     │
│  ┌─ ORIGIN ────────────────────────┐│
│  │  Region: Shakiso, Guji Zone    ││  Mono, 13px, #8C8279
│  │  Altitude: 1950–2200 masl      ││
│  │  Varietal: Mixed Heirloom      ││
│  │  Process: Natural, Barrel Aged ││
│  │  Grade: 1 · Score: 92pt       ││
│  └─────────────────────────────────┘│  bg: #F5F0E8, radius: 12pt
│                                     │  padding: 16pt inner
│  ┌─ WHAT YOU'LL TASTE ─────────────┐│
│  │                                 ││
│  │  ┌──────┐ ┌──────┐ ┌──────┐   ││  tasting note icons (large: 48pt)
│  │  │  🥃  │ │  🫐  │ │  🍒  │   ││
│  │  │Whisk-│ │Berry │ │Cherry│   ││
│  │  │ey    │ │      │ │      │   ││
│  │  └──────┘ └──────┘ └──────┘   ││
│  │                                 ││
│  │  Whiskey aroma, berry, floral,  ││  Regular, 15px, #1A1714
│  │  gentle fermented taste, cherry,││  line-height: 1.6
│  │  tropical fruit, milk chocolate,││
│  │  wine-like peppery finish,      ││
│  │  syrupy body.                   ││
│  │                                 ││
│  │  ROAST   ░░▓░░░░░░░░░░░░░░░░   ││  flavor sliders (large variant)
│  │          Light                  ││
│  │  SWEET   ░░░░░░░░░░░░▓░░░░░░   ││
│  │          High                   ││
│  │  ACID    ░░░░░░░░░░░▓░░░░░░░   ││
│  │          Medium-High            ││
│  │                                 ││
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │  padding: 20pt inner
│  ┌─ BREW PAIRING ──────────────────┐│
│  │                                 ││
│  │  Best for:                      ││  Mono, 11px, #B5A99A
│  │  ┌──────────┐ ┌──────────┐     ││
│  │  │ VOLTAGE  │ │          │     ││  profile chips
│  │  └──────────┘ └──────────┘     ││  bg: #E8DFD2, text: #8C8279
│  │                                 ││  radius: 8pt, height: 28pt
│  │  "Explosive fruit. A whiskey    ││  Regular, 15px, #1A1714
│  │   finish that lingers. The most ││  line-height: 1.5
│  │   complex thing in the lineup." ││  italic
│  │                                 ││
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │
│  ┌─ SERVING GUIDE ─────────────────┐│
│  │                                 ││  INFOGRAPHIC CARD
│  │        [See Section 4]          ││  (full spec in Section 4)
│  │                                 ││
│  └─────────────────────────────────┘│
│                                     │
│  ┌─────────────────────────────────┐│
│  │                                 ││  PRICE + PURCHASE
│  │  $27.00                         ││  t_price, #1A1714
│  │  or subscribe & save 15%        ││  Mono, 12px, #5B8C5A
│  │                                 ││
│  │  ┌─────────────────────────┐   ││  primary CTA
│  │  │   ADD TO CART  →        │   ││  bg: #1A1714, text: #FFFFFF
│  │  └─────────────────────────┘   ││  height: 52pt, radius: 12pt
│  │                                 ││  full width
│  │  ┌─────────────────────────┐   ││  secondary CTA
│  │  │   SUBSCRIBE & SAVE     │   ││  bg: transparent, border: 1px #1A1714
│  │  └─────────────────────────┘   ││  text: #1A1714, height: 48pt
│  │                                 ││  radius: 12pt
│  │                                 ││
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │  padding: 20pt inner
│                                     │  bottom safe area padding: 32pt
└─────────────────────────────────────┘
```

### Section Labels on Coffee Side

```swift
// Section labels follow the same Mono, 11px, ALL CAPS pattern
// but use coffee_text_secondary (#8C8279) instead of #888888
let sectionLabel_origin       = "ORIGIN"
let sectionLabel_whatYoullTaste = "WHAT YOU'LL TASTE"
let sectionLabel_brewPairing  = "BREW PAIRING"
let sectionLabel_servingGuide = "SERVING GUIDE"
```

### "Add to Cart" Behavior

```swift
func addToCart(bean: Bean) {
    guard let shopifyURL = bean.shopifyURL,
          let productID = bean.shopifyProductID else {
        // Price TBD state — button shows "COMING SOON" and is disabled
        return
    }
    // Opens Shopify product page in SFSafariViewController
    let url = URL(string: "\(shopifyURL)/cart/add?id=\(productID)")!
    present(SFSafariViewController(url: url))
}
```

**Price TBD state:** When `bean.price == nil`:
- Price area shows: `"PRICE COMING SOON"` — Mono, 14px, #B5A99A
- "ADD TO CART" button becomes: `"NOTIFY ME"` — same style but bg: #E8DFD2, text: #8C8279
- Tap "NOTIFY ME" → stores email/notification preference in UserDefaults, shows toast: `"We'll let you know."`

### "Subscribe & Save" Behavior

Opens Shopify subscription page in SFSafariViewController. If the roaster doesn't support subscriptions, this button is hidden.

```swift
func showSubscribeButton(bean: Bean) -> Bool {
    return bean.price != nil && bean.shopifyURL != nil
}
```

---

## 3. Bean-First Flow — The Core Redesign

### The Problem with the Old Flow

```
OLD: Health Data → Brew Profile → Brew Settings → Brew
```

The bean is invisible. The user gets "First Light: 4-8°C, 12min, 60g/L" and has no idea which coffee to put in the machine. The profile talks about extraction parameters. The user wants to know: **what bag do I grab?**

### The New Flow

```
NEW: Health Data → Brew Profile Suggested → BEAN SELECTION → Brew Settings (adapted to bean) → Brew
```

The bean is the pivot. Health data tells you *how* to brew. The bean tells you *what* to brew. Brew settings adapt to both.

### Screen A: Health Data + Profile Suggestion (Dashboard)

**This is the existing dashboard Cards 1-3, with Card 3 modified.**

Card 3 no longer shows full brew parameters. It shows the **profile suggestion** and a CTA to enter the coffee world.

```
┌─────────────────────────────────────┐
│  ⟨📡⟩            bre:zi           ⚙  │  NavBarView.swift (dark)
│─────────────────────────────────────│
│                                     │  #F5F5F5 background
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
│  │  Your everyday standard.        │ │  Regular, 14px, #CCCCCC
│  │                                 │ │
│  │  4–8°C · 12 min · 60 g/L       │ │  Mono, 14px, #888888
│  │                                 │ │
│  │  ┌─────────────────────────┐   │ │  PRIMARY CTA [CHANGED]
│  │  │  CHOOSE YOUR BEAN  →   │   │ │  bg: #FFFFFF, text: #000000
│  │  └─────────────────────────┘   │ │  CondensedBold, 16px, ALL CAPS
│  │                                 │ │  height: 48pt, radius: 12pt
│  │  Use general settings ›        │ │  SECONDARY CTA [NEW]
│  │                                 │ │  Regular, 14px, #888888
│  │                                 │ │  44×44pt tap area
│  └─────────────────────────────────┘ │
│                                     │
│  ┌─────────────────────────┐        │  outlined button
│  │  VIEW BREW HISTORY      │        │  (unchanged)
│  └─────────────────────────┘        │
│                                     │
└─────────────────────────────────────┘
```

**Copy (exact strings):**
```swift
let card3_label        = "BREZI SUGGESTS"
let card3_cta_bean     = "CHOOSE YOUR BEAN"
let card3_cta_general  = "Use general settings"
```

**"CHOOSE YOUR BEAN" behavior:** Pushes to Screen B (Bean Selection). This is where the visual world transitions from dark to warm.

**"Use general settings" behavior:** Pushes to Screen C (Brew Settings) with `selectedBean = nil`. Screen C shows generic profile parameters without bean-specific optimization. The `"Optimized for"` line reads `"Using general First Light settings"`.

---

### Screen B: Bean Selection — `BeanSelectionView.swift`

**This is the NEW screen. The pivot point. The moment the app becomes a coffee brand.**

Background transitions from `#000000` (nav bar) to `#FAF6F0` (content area). The warm world opens.

```
┌─────────────────────────────────────┐
│  [←]              bre:zi        🛒  │  NavBarView — transitional
│─────────────────────────────────────│  bg stays #000000 (nav is always dark)
│                                     │  bg: #FAF6F0 (warm cream)
│  FIRST LIGHT                        │  CondensedBold, 14px, #8C8279
│  RECOMMENDED BEANS                  │  Mono, 11px, #B5A99A, ALL CAPS
│                                     │  top padding: 20pt
│  Beans that match your              │  Regular, 15px, #8C8279
│  morning signal today.              │  line-height: 1.5
│                                     │  bottom padding: 16pt
│  ┌─────────────────────────────────┐│
│  │                                 ││
│  │  ┌───────────────────────────┐  ││  RECOMMENDED BEAN CARD 1
│  │  │      [REGENT BAG]         │  ││  BeanCardView.swift
│  │  │                           │  ││
│  │  │  REGENT COFFEE            │  ││  (full card — see Section 2)
│  │  │  Ethiopia Guji            │  ││
│  │  │  Hambela Washed           │  ││
│  │  │                           │  ││
│  │  │  🍫 Cocoa  🌸 Jasmine     │  ││
│  │  │  🍓 Strawberry            │  ││
│  │  │                           │  ││
│  │  │  ROAST ░░▓░░░░░░░░░░░░    │  ││
│  │  │  SWEET ░░░░░░░░▓░░░░░░    │  ││
│  │  │  ACID  ░░░░░░░░░░░▓░░    │  ││
│  │  │                           │  ││
│  │  │  ★ RECOMMENDED FOR YOU    │  ││
│  │  │                           │  ││
│  │  │  $27.00      [ADD TO CART]│  ││
│  │  │                           │  ││
│  │  │  ┌───────────────────┐    │  ││  BREW WITH THIS BEAN [NEW]
│  │  │  │ BREW WITH THIS  → │    │  ││  bg: #5B8C5A, text: #FFFFFF
│  │  │  └───────────────────┘    │  ││  CondensedBold, 14px, ALL CAPS
│  │  │                           │  ││  height: 44pt, radius: 10pt
│  │  └───────────────────────────┘  ││  full width within card
│  │                                 ││
│  └─────────────────────────────────┘│
│                                     │  gap: 16pt between cards
│  ┌─────────────────────────────────┐│
│  │                                 ││  RECOMMENDED BEAN CARD 2
│  │  (next matching bean card)      ││  (if another bean matches profile)
│  │                                 ││
│  └─────────────────────────────────┘│
│                                     │
│  ── or ──────────────────────────── │  divider with "or"
│                                     │  line: 1px #E5DDD3
│                                     │  "or": Mono, 12px, #B5A99A, centered
│                                     │  padding: 24pt vertical
│  ┌─────────────────────────────────┐│
│  │                                 ││  GENERAL PROFILE OPTION
│  │  Don't have a specific bean?    ││  Regular, 15px, #1A1714
│  │                                 ││
│  │  Use general First Light        ││  Regular, 14px, #8C8279
│  │  settings — optimized for the   ││  line-height: 1.5
│  │  profile, not a specific origin.││
│  │                                 ││
│  │  ┌───────────────────────┐     ││
│  │  │  USE GENERAL PROFILE  │     ││  bg: transparent
│  │  └───────────────────────┘     ││  border: 1px #1A1714
│  │                                 ││  text: #1A1714, height: 44pt
│  │                                 ││  CondensedBold, 14px, ALL CAPS
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │  padding: 20pt inner
│  ── browse ─────────────────────── │  divider
│                                     │  padding: 24pt vertical
│  BROWSE ALL BEANS                   │  Mono, 11px, #B5A99A, ALL CAPS
│                                     │  bottom padding: 12pt
│  ┌─────────────────────────────────┐│
│  │                                 ││  ALL BEANS — horizontal scroll
│  │  [Stitch]  [Corvus]  [Regent]  ││  BeanCardCompact.swift
│  │                                 ││  (compact variant — see below)
│  └─────────────────────────────────┘│
│                                     │
│  ┌─────────────────────────┐        │
│  │  VISIT BEAN SHOP  →    │        │  outlined button
│  └─────────────────────────┘        │  border: 1px #8C8279
│                                     │  text: #8C8279, Mono, 13px
│                                     │  height: 44pt, radius: 12pt
│                                     │
└─────────────────────────────────────┘
```

**Copy (exact strings):**
```swift
let beanSelection_label         = "RECOMMENDED BEANS"
let beanSelection_subtitle      = "Beans that match your morning signal today."
let beanSelection_brewCTA       = "BREW WITH THIS"
let beanSelection_generalTitle  = "Don't have a specific bean?"
func beanSelection_generalBody(_ profileName: String) -> String {
    "Use general \(profileName) settings — optimized for the profile, not a specific origin."
}
let beanSelection_generalCTA    = "USE GENERAL PROFILE"
let beanSelection_browseLabel   = "BROWSE ALL BEANS"
let beanSelection_shopCTA       = "VISIT BEAN SHOP"
let beanSelection_dividerOr     = "or"
let beanSelection_dividerBrowse = "browse"
```

### Bean Card Compact — `BeanCardCompactView.swift`

For horizontal scroll in "Browse All Beans". Narrower, no "Add to Cart", shows enough to identify the bean.

```
┌───────────────────┐
│  [BEAN BAG]       │  width: 180pt
│  140 × 140pt      │  bg: #FFFFFF
│                   │  radius: 16pt
│  STITCH COFFEE    │  t_roaster_name, 10px
│  Dark Field       │  Bold, 16px, #1A1714
│                   │
│  🍫 🌰 🍮         │  tasting note icons (28pt)
│                   │
│  ROAST ░░░░░░░▓░  │  mini sliders
│                   │
│  TBD              │  price or "COMING SOON"
│                   │  Mono, 14px
└───────────────────┘
```

**Spec:**
- Width: 180pt
- Height: intrinsic (content-driven)
- Image: 140 × 140pt, `contentMode: .fit`
- Horizontal scroll: `.scrollTargetBehavior(.viewAligned)`, padding 16pt, gap 12pt
- Tap anywhere on compact card → pushes `BeanDetailView`
- Mini sliders: same logic as full sliders but 120pt wide, no value labels

### Recommendation Matching Logic

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
// deep_work   → Stitch Dark Field
// voltage     → Stitch Dark Field, Corvus Ethiopia Guji Barrel
// the_blank   → all beans shown, no "recommended" tags
```

**"BREW WITH THIS" behavior:** Stores `selectedBean` in memory, pushes to Screen C with bean context.

**"USE GENERAL PROFILE" behavior:** Sets `selectedBean = nil`, pushes to Screen C.

---

### Screen C: Brew Settings — `BrewSettingsView.swift`

**Brew parameters, adapted to the selected bean. Two variants: bean-specific and general.**

Background: `#FAF6F0` (stays in coffee world).

#### Variant 1: Bean-Specific Settings

```
┌─────────────────────────────────────┐
│  [←]              bre:zi        🛒  │  NavBarView
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
│  │     [See Section 4 — full       ││  (cup icons + clock arc +
│  │      infographic card]          ││   timing guidance)
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
│  │                                 │ │  padding: 16pt below
│  │  ┌──────────────────────────┐  │ │
│  │  │  ROAST    Medium         │  │ │  parameter rows
│  │  │  GRIND    Medium         │  │ │  Mono, 13px
│  │  │  WATER    Filtered       │  │ │  label: #B5A99A, value: #1A1714
│  │  └──────────────────────────┘  │ │  bg: #F5F0E8, radius: 10pt
│  │                                 │ │  row height: 36pt, padding: 12pt
│  │  Adapted for this bean's light  │ │  Mono, 12px, #8C8279, italic
│  │  roast — lower temp, longer     │ │  line-height: 1.5
│  │  steep to pull floral notes.    │ │  top padding: 12pt
│  │                                 │ │
│  └─────────────────────────────────┘ │
│                                     │
│  ┌─────────────────────────────────┐│  FLAVOR PREVIEW
│  │                                 ││
│  │  WHAT YOU'LL TASTE              ││  Mono, 11px, #B5A99A, ALL CAPS
│  │                                 ││
│  │  ┌──────┐ ┌──────┐ ┌──────┐   ││  tasting note icons
│  │  │  🍫  │ │  🌸  │ │  🍓  │   ││
│  │  │Cocoa │ │Jasmin││ │Straw-│   ││
│  │  └──────┘ └──────┘ └──────┘   ││
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

#### Variant 2: General Profile Settings

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
- Adaptation note reads: `"Standard extraction for this profile. Works with most beans in the medium roast range."`
- "Change bean" link becomes `"Select a specific bean ›"` — pops back to Screen B

**Copy (exact strings):**
```swift
let brewSettings_label           = "YOUR BREW"
let brewSettings_optimizedFor    = "Optimized for:"
func brewSettings_beanLine(_ beanName: String) -> String { beanName }
func brewSettings_profileLine(_ profileName: String) -> String { "× \(profileName) profile" }
let brewSettings_startBrew       = "START BREW"
let brewSettings_changeBean      = "Change bean"
let brewSettings_selectBean      = "Select a specific bean"

// General variant
func brewSettings_generalTitle(_ profileName: String) -> String {
    "Using general\n\(profileName) settings"
}
let brewSettings_generalBody     = "No specific bean selected. These parameters work with most medium roasts."
let brewSettings_generalNote     = "Standard extraction for this profile. Works with most beans in the medium roast range."

// Bean-specific adaptation notes (per profile × roast level)
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

When a bean is selected, brew parameters may shift slightly from the generic profile values. These adjustments are small — the profile still dominates. The bean fine-tunes.

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
    "corvus_ethiopia_guji_barrel": [
        "voltage":     BeanAdjustment(tempDelta: 0,  timeDelta: +2, doseDelta: +5, roastSuggestion: "Light",  grindSuggestion: "Medium"),
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

### Screen D: Brewing + Confirmation

**Unchanged from R3 spec.** The brewing progress screen, brew completion overlay, and post-brew state remain the same. The only addition: the post-brew card now shows the bean name if one was selected.

```
│  FIRST LIGHT                        │  CondensedBold, 32px, #FFFFFF
│  Regent Ethiopia Guji Hambela       │  Mono, 13px, #888888 [NEW — bean name]
│  7:14am · 12 min                    │  Mono, 14px, #888888
```

If no bean selected: bean name line is omitted (same as current R3 behavior).

**Copy:**
```swift
func postBrew_beanLine(_ beanName: String?) -> String? {
    return beanName  // nil = omit the line
}
```

---

## 4. Serving Infographic Card — `ServingInfoCard.swift`

### The Principle

Serving size and cutoff time are currently text: `"1 serve · Best before 2pm"`. Jackson wants infographics, not settings text. This card replaces all text-only serving information with visual elements.

### Layout

```
┌─────────────────────────────────────┐
│                                     │  bg: #FFFFFF, radius: 16pt
│  SERVING GUIDE                      │  Mono, 11px, #B5A99A, ALL CAPS
│                                     │  padding: 20pt inner
│  ┌───────────────┬─────────────────┐│
│  │               │                 ││  TWO COLUMNS
│  │   SERVING     │   BEST BEFORE   ││
│  │               │                 ││
│  │   ┌─────┐    │    ┌───────┐    ││
│  │   │ ☕  │    │    │       │    ││
│  │   │     │    │    │  ╭──╮ │    ││  CLOCK ARC
│  │   └─────┘    │    │  │╲ │ │    ││  See spec below
│  │              │    │  ╰──╯ │    ││
│  │   1 cup      │    │       │    ││
│  │   ~250ml     │    └───────┘    ││
│  │              │                 ││
│  │              │    2:00 PM       ││
│  │              │                 ││
│  └───────────┬──┴─────────────────┘│
│              │                     │
│  ┌───────────┴─────────────────────┐│
│  │                                 ││  TIMING BAR
│  │  ☀️ 6am ────────────── 2pm 🌙   ││  See spec below
│  │       ████████████████░░░░░     ││
│  │       ↑ brew window             ││
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

### Cutoff Time — Clock Arc

A simple analog clock face with an arc showing the brew window.

```swift
struct ClockArcSpec {
    let diameter: CGFloat = 72
    let strokeWidth: CGFloat = 6
    let trackColor = Color(hex: "#E5DDD3")       // warm divider — the "empty" part
    let fillColor = Color(hex: "#5B8C5A")         // sage green — the "OK to brew" arc
    let warningColor = Color(hex: "#D4A96A")      // amber — approaching cutoff
    let pastColor = Color(hex: "#E8DFD2")         // muted — past cutoff
    let hourHandLength: CGFloat = 18
    let minuteHandLength: CGFloat = 24
    let handColor = Color(hex: "#1A1714")
    let handWidth: CGFloat = 2
    let centerDotSize: CGFloat = 6
    let centerDotColor = Color(hex: "#1A1714")
}
```

**Clock arc behavior:**
- Arc starts at 6:00 AM position (bottom-left of clock face)
- Arc ends at the cutoff time (e.g., 2:00 PM = top-right)
- Fill color: `#5B8C5A` (sage green) for the brew-safe window
- Track color: `#E5DDD3` for the rest of the circle
- Hour/minute hands point to current time
- Cutoff time label below clock: `"2:00 PM"` — Mono, 14px, #1A1714

**Clock rendering:**
```swift
struct ClockArcView: View {
    let cutoffHour: Int      // e.g., 14 for 2pm
    let currentHour: Int
    let currentMinute: Int

    var brewWindowFraction: Double {
        // 6am to cutoff, mapped to 0.0–1.0
        let start = 6.0
        let end = Double(cutoffHour)
        let windowHours = end - start
        return windowHours / 12.0  // 12-hour clock face
    }

    var startAngle: Angle {
        // 6am = 180° on standard clock
        .degrees(180)
    }

    var endAngle: Angle {
        // Map cutoff hour to clock position
        let hoursFromTop = Double(cutoffHour % 12)
        return .degrees(hoursFromTop * 30 - 90)
    }
}
```

### Timing Bar — Day Timeline

A horizontal bar showing the brew window across the day.

```swift
struct TimingBarSpec {
    let barHeight: CGFloat = 8
    let barRadius: CGFloat = 4
    let barTrackColor = Color(hex: "#E5DDD3")
    let barFillColor = Color(hex: "#5B8C5A")
    let barWidth: CGFloat = .infinity  // full width of container

    // Labels
    let startLabel = "6am"
    let endLabel: String   // dynamic: "2pm", "4pm", etc.
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
```
6am ━━━━━━━━━━━━━━━━━━━━━━━━━━░░░░░░ 2pm
    ↑ brew window
```

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
    "the_blank":   1   // default
]
```

---

## 5. Flavor Visualization — Sliders + Tasting Note Icons

### Flavor Slider Bars — `FlavorSliderView.swift`

Three horizontal bars: Roast, Sweetness, Acidity. Each is a spectrum, not a toggle.

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

    // Track gradient (NOT a solid fill — a spectrum)
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

    // Indicator dot
    let dotSize: CGFloat = 12
    let dotColor = Color(hex: "#1A1714")
    let dotBorderColor = Color(hex: "#FFFFFF")
    let dotBorderWidth: CGFloat = 2
    let dotShadow = Shadow(color: .black.opacity(0.15), radius: 3, x: 0, y: 1)

    // Labels
    let labelFont = Font.system(size: 11, weight: .medium)
    let categoryLabelColor = Color(hex: "#8C8279")  // "ROAST", "SWEET", "ACID"
    let valueLabelColor = Color(hex: "#B5A99A")      // "Light", "Medium", etc.
    let labelTracking: CGFloat = 0.05  // 5%

    // Layout
    let rowHeight: CGFloat = 32        // per slider row
    let categoryLabelWidth: CGFloat = 48  // fixed width for alignment
    let gapBetweenRows: CGFloat = 8

    // Value labels (range endpoints)
    let roastLabels = ("Light", "Dark")
    let sweetnessLabels = ("Low", "High")
    let acidityLabels = ("Low", "High")
}
```

### Slider Rendering

The dot position is calculated from the `FlavorProfile` values (0.0–1.0):

```swift
func dotPosition(value: Double, trackWidth: CGFloat) -> CGFloat {
    let padding: CGFloat = 6  // half dot size — don't clip at edges
    return padding + (value * (trackWidth - padding * 2))
}
```

### Value Label Logic

The slider shows a text label below the track indicating the position. These are derived from the value:

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

### Full Slider vs Mini Slider

| Context | Track width | Show value label | Show endpoints | Row height |
|---|---|---|---|---|
| Bean card (full) | card width - 32pt | Yes (below) | No | 32pt |
| Bean detail | card width - 40pt | Yes (below) | Yes | 36pt |
| Bean card compact | 120pt | No | No | 20pt |

### Tasting Note Icons — `TastingNoteView.swift`

Flat, minimal illustrations in a modern specialty coffee style. Think Stitch Coffee's tasting note icons — simple, recognizable, warm.

Each icon is a **custom illustration asset** in the asset catalog. NOT emoji. NOT SF Symbols. Custom vector art.

### Icon Design Direction

Style: flat, single-color (or two-tone), rounded shapes, thick strokes (2-3pt), warm palette. Think: what you'd see on a specialty coffee bag's tasting notes section.

Each icon sits in a circular container:

```swift
struct TastingNoteIconSpec {
    // Container
    let containerSize: CGFloat = 48      // detail view
    let containerSizeCompact: CGFloat = 36   // bean card
    let containerSizeMini: CGFloat = 28      // compact card
    let containerBg = Color(hex: "#F5F0E8")  // warm off-white
    let containerBgDark = Color(hex: "#3A3530")  // dark card variant
    let containerRadius: CGFloat = 12    // rounded square, not circle

    // Icon
    let iconPadding: CGFloat = 8         // inset from container edge
    let iconColor = Color(hex: "#8C6B3E")  // warm brown — primary icon color
    let iconColorDark = Color(hex: "#D4A96A")  // amber — dark card variant

    // Label
    let labelFont = Font.system(size: 10, weight: .medium)
    let labelColor = Color(hex: "#8C8279")
    let labelColorDark = Color(hex: "#B5A99A")
    let labelTopPadding: CGFloat = 4
    let labelMaxWidth: CGFloat = 56      // truncate long labels

    // Row layout
    let iconGap: CGFloat = 12
    let maxIconsPerRow: Int = 4          // wrap if more
}
```

### Complete Icon Asset List

These are the required illustration assets for the current 3-bean catalog. Each is a vector PDF or SVG in the asset catalog.

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
        TastingNoteDisplay(icon: .darkChocolate, label: "Dark Chocolate"),
        TastingNoteDisplay(icon: .toastedAlmond, label: "Toasted Almond"),
        TastingNoteDisplay(icon: .caramel, label: "Caramel")
    ],
    "corvus_ethiopia_guji_barrel": [
        TastingNoteDisplay(icon: .whiskey, label: "Whiskey"),
        TastingNoteDisplay(icon: .berry, label: "Berry"),
        TastingNoteDisplay(icon: .cherry, label: "Cherry")
    ],
    "regent_ethiopia_guji_hambela": [
        TastingNoteDisplay(icon: .cocoa, label: "Cocoa"),
        TastingNoteDisplay(icon: .jasmine, label: "Jasmine"),
        TastingNoteDisplay(icon: .strawberry, label: "Strawberry")
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

**Deliverables:** 14 icons × 2 color variants (light + dark) = 28 assets. Format: PDF vector in Xcode asset catalog, 1x/2x/3x.

---

## 6. Marketplace Screen — `MarketplaceView.swift`

### Access Points

The marketplace ("Bean Shop") is accessible from:
1. **Bean Selection Screen B** — "VISIT BEAN SHOP" button at bottom
2. **Navigation bar cart icon** — persistent 🛒 icon on coffee-side screens
3. **Dashboard** — new link below brew history: "BROWSE BEANS ›"
4. **Settings** — "Bean Shop" row

### Tab Concept

The marketplace is NOT a separate tab. It's a **pushed view** from any of the access points above. Brezi's navigation stays single-stack. Adding a tab bar would fight the dark/warm palette split.

### Layout — `MarketplaceView.swift`

```
┌─────────────────────────────────────┐
│  [←]              bre:zi        🛒  │  NavBarView
│─────────────────────────────────────│
│                                     │  bg: #FAF6F0
│  ┌─────────────────────────────────┐│
│  │                                 ││  HEADER
│  │  BEAN SHOP                      ││  CondensedBold, 28px, #1A1714
│  │  Roaster collabs. Curated       ││  Regular, 15px, #8C8279
│  │  for Brezi cold brew.           ││  line-height: 1.5
│  │                                 ││
│  └─────────────────────────────────┘│  padding: 20pt
│                                     │
│  ┌─ FILTER CHIPS ──────────────────┐│
│  │                                 ││
│  │  [All] [Tide Pool] [First      ││  horizontal scroll
│  │   Light] [Deep Work] [Voltage] ││  FilterChipRow.swift
│  │                                 ││
│  └─────────────────────────────────┘│
│                                     │
│  ┌─ RECOMMENDED FOR YOU ───────────┐│  (only when filter = "All")
│  │                                 ││  section label: Mono, 11px, #B5A99A
│  │  Beans that match today's       ││  Regular, 13px, #8C8279
│  │  First Light profile.           ││
│  │                                 ││
│  │  ┌───────────────────────────┐  ││
│  │  │   [REGENT BEAN CARD]      │  ││  BeanCardView (full)
│  │  │   (full card with         │  ││  with "★ RECOMMENDED" tag
│  │  │    recommendation tag)    │  ││
│  │  │                           │  ││
│  │  │   [BREW WITH THIS →]      │  ││  + "BREW WITH THIS" button
│  │  └───────────────────────────┘  ││
│  │                                 ││
│  └─────────────────────────────────┘│  gap: 24pt
│                                     │
│  ┌─ ALL BEANS ─────────────────────┐│
│  │                                 ││  section label
│  │  ┌───────────────────────────┐  ││
│  │  │   [STITCH BEAN CARD]      │  ││  BeanCardView (full)
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
│  │  Subscribe to your favorite     ││  Regular, 14px, #8C8279
│  │  bean and save 15% on every     ││  line-height: 1.5
│  │  delivery. Cancel anytime.      ││
│  │                                 ││
│  │  ┌─────────────────────────┐   ││
│  │  │  LEARN MORE  →          │   ││  outlined button
│  │  └─────────────────────────┘   ││  border: 1px #D4A96A
│  │                                 ││  text: #D4A96A, Mono, 13px
│  │                                 ││  height: 44pt, radius: 12pt
│  └─────────────────────────────────┘│  bg: #FFFFFF, radius: 16pt
│                                     │  padding: 20pt inner
│                                     │  top/bottom accent border:
│                                     │  2px #D4A96A top
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

    // Chips
    let chipLabels = ["All", "Tide Pool", "First Light", "Deep Work", "Voltage"]
}
```

**Filter behavior:**
- "All" shows all beans, with "Recommended For You" section at top
- Profile-specific filter shows only beans matching that profile
- If no beans match the selected profile: show empty state

**Empty state (filtered, no matches):**
```swift
let marketplace_emptyFilter = "No beans matched this profile yet.\nMore roasters coming soon."
// Mono, 14px, #B5A99A, centered, padding: 48pt vertical
```

### Copy (exact strings)

```swift
let marketplace_title         = "BEAN SHOP"
let marketplace_subtitle      = "Roaster collabs. Curated for Brezi cold brew."
let marketplace_recommended   = "RECOMMENDED FOR YOU"
func marketplace_recommendedSub(_ profileName: String) -> String {
    "Beans that match today's \(profileName) profile."
}
let marketplace_allBeans      = "ALL BEANS"
let marketplace_subscribe_title = "Never run out."
let marketplace_subscribe_body  = "Subscribe to your favorite bean and save 15% on every delivery. Cancel anytime."
let marketplace_subscribe_cta   = "LEARN MORE"
let marketplace_footer_line1    = "More roasters coming soon."
let marketplace_footer_line2    = "Want to partner?"
let marketplace_footer_email    = "hello@brezicoffee.com"
let marketplace_emptyFilter     = "No beans matched this profile yet.\nMore roasters coming soon."

// Dashboard access point (new link)
let dashboard_browseBeans       = "BROWSE BEANS"
```

### Cart Icon in NavBar

On coffee-side screens (bean selection, bean detail, brew settings, marketplace), the nav bar right slot adds a cart icon:

```swift
// Replaces the antenna icon on coffee-side screens
let navBar_cartIcon = "cart"  // SF Symbol
// Same position, size, and tap target as the antenna icon
// Tap → push MarketplaceView
```

The cart icon does NOT show a badge count. Brezi is not a cart-based app — each "Add to Cart" opens Shopify directly. The cart icon is a **navigation shortcut** to the marketplace, not a local cart indicator.

### "BREW WITH THIS" in Marketplace Context

When tapped from the marketplace (not from Screen B):
1. Store `selectedBean`
2. Pop to dashboard
3. Auto-push through Screen B → Screen C with the bean pre-selected
4. Net effect: user lands on brew settings optimized for their chosen bean

```swift
func brewWithBean(_ bean: Bean, profile: BrewProfile) {
    selectedBean = bean
    // Navigate: marketplace → pop to dashboard → push BrewSettingsView
    navigationPath.removeLast(navigationPath.count)  // pop to root
    navigationPath.append(.brewSettings(bean: bean, profile: profile))
}
```

---

## 7. Navigation Updates — Summary

### New NavBar Variant

The nav bar gains a warm variant for coffee-side screens:

```swift
enum NavBarVariant {
    case health   // bg: #000000, icons: #FFFFFF
    case coffee   // bg: #000000 (stays dark — the nav is always the brand shell)
                  // but right icons change: antenna → cart
}
```

The nav bar background stays `#000000` everywhere. The brand shell is always dark. Only the content area transitions.

### Updated Nav Tree (additions in **bold**)

```
AppRoot
 └─ MainFlow
     ├─ DashboardView.swift
     │   ├─ State: preBrew → Card 3 CTA → "CHOOSE YOUR BEAN"
     │   │   └─ **BeanSelectionView.swift** [NEW — Screen B]
     │   │       ├─ "BREW WITH THIS" → **BrewSettingsView.swift** [NEW — Screen C]
     │   │       │   └─ "START BREW" → Brewing state (existing)
     │   │       ├─ "USE GENERAL PROFILE" → **BrewSettingsView.swift** (bean=nil)
     │   │       ├─ Bean card tap → **BeanDetailView.swift** [NEW]
     │   │       └─ "VISIT BEAN SHOP" → **MarketplaceView.swift** [NEW]
     │   │
     │   ├─ "Use general settings" → **BrewSettingsView.swift** (bean=nil)
     │   ├─ **"BROWSE BEANS"** → **MarketplaceView.swift** [NEW]
     │   ├─ State: brewing (unchanged)
     │   └─ State: postBrew (unchanged + bean name line)
     │
     ├─ **MarketplaceView.swift** [NEW — accessible from nav cart icon]
     │   ├─ Bean card tap → **BeanDetailView.swift**
     │   └─ "BREW WITH THIS" → BrewSettingsView (with bean)
     │
     ├─ BrewRecommendationView.swift (existing — may be deprecated or merged)
     ├─ HistoryView.swift (unchanged)
     ├─ DeviceHubView.swift (unchanged)
     └─ SettingsView.swift (+ "Bean Shop" row)
```

### Settings Row Addition

```
│  ┌─────────────────────────────────┐│
│  │  ☕ Bean Shop                  › ││  new row in SettingsView
│  └─────────────────────────────────┘│  icon: SF Symbol "cup.and.saucer"
│                                     │  tap → push MarketplaceView
```

---

## 8. New Animations

| Location | Element | Type | Duration | Easing | Delay | Trigger |
|---|---|---|---|---|---|---|
| Screen B | Background transition | color crossfade (#F5F5F5 → #FAF6F0) | 0.3s | easeOut | 0.0s | Push transition |
| Screen B | Recommended bean cards | fadeIn + translateY(12→0) | 0.3s | easeOut | 0.1s stagger | Screen appear |
| Screen B | Browse section | fadeIn | 0.2s | easeOut | 0.3s | Scroll into view |
| Bean card | Recommendation tag | subtle pulse scale(1.0↔1.02) | 2.0s | easeInOut | — | Loop while visible |
| Bean detail | Image | fadeIn + scale(0.98→1.0) | 0.3s | easeOut | 0.0s | Screen load |
| Bean detail | Origin card | fadeIn + translateY(8→0) | 0.25s | easeOut | 0.15s | Screen load |
| Bean detail | Flavor section | fadeIn + translateY(8→0) | 0.25s | easeOut | 0.25s | Screen load |
| Flavor sliders | Dot position | spring(response: 0.4, dampingFraction: 0.8) | — | spring | 0.0s | Appear |
| Clock arc | Arc draw | trim(0→target) | 0.6s | easeOut | 0.1s | Appear |
| Timing bar | Fill draw | width(0→target) | 0.4s | easeOut | 0.2s | Appear |
| Marketplace | Filter chip select | spring(response: 0.3, dampingFraction: 0.7) | — | spring | 0.0s | Tap |
| Marketplace | Bean card filter | matchedGeometryEffect | 0.3s | easeInOut | 0.0s | Filter change |

---

## 9. New Components — Summary

| Component | File | Purpose |
|---|---|---|
| `BeanCardView.swift` | Bean product card (full) | Shows bean image, roaster, name, tasting notes, sliders, price, CTA |
| `BeanCardCompactView.swift` | Bean card (compact) | Horizontal scroll variant — image + name + mini sliders |
| `BeanDetailView.swift` | Bean detail screen | Full-screen bean info with origin, flavor, brew pairing, purchase |
| `BeanSelectionView.swift` | Screen B — bean picker | Recommended beans + general option + browse |
| `BrewSettingsView.swift` | Screen C — brew params | Bean-optimized or general brew parameters |
| `MarketplaceView.swift` | Bean Shop | All beans, filtered by profile, subscribe banner |
| `FlavorSliderView.swift` | Flavor bar chart | Horizontal gradient slider with dot indicator |
| `TastingNoteView.swift` | Tasting note icon | Illustrated icon in rounded container + label |
| `ServingInfoCard.swift` | Serving infographic | Cup icons + clock arc + timing bar |
| `ClockArcView.swift` | Clock arc component | Analog clock face with brew window arc |
| `TimingBarView.swift` | Day timeline bar | Horizontal bar showing brew-safe window |
| `FilterChipView.swift` | Profile filter pill | Pill-shaped filter chip for marketplace |

---

## 10. New Data Requirements

### Bean Entity (Core Data)

```swift
// New Core Data entity — or could be a bundled JSON decoded at launch
@objc(BeanEntity)
class BeanEntity: NSManagedObject {
    @NSManaged var id: String
    @NSManaged var roasterName: String
    @NSManaged var beanName: String
    @NSManaged var beanSubtitle: String
    @NSManaged var roastLevel: String       // "light", "medium", "dark"
    @NSManaged var origin: String
    @NSManaged var process: String
    @NSManaged var descriptionText: String
    @NSManaged var emotionalTag: String
    @NSManaged var tastingNotesJSON: String  // encoded [TastingNote]
    @NSManaged var flavorRoast: Double
    @NSManaged var flavorSweetness: Double
    @NSManaged var flavorAcidity: Double
    @NSManaged var bestForProfilesJSON: String  // encoded [String]
    @NSManaged var price: NSDecimalNumber?
    @NSManaged var shopifyURL: String?
    @NSManaged var shopifyProductID: String?
    @NSManaged var imageAsset: String
    @NSManaged var cardVariant: String      // "light", "dark"
}
```

**Recommendation:** For MVP with only 3 beans, skip Core Data. Bundle as a JSON file decoded at launch:

```swift
// beans.json in app bundle
let beans: [Bean] = Bundle.main.decode("beans.json")
```

### BrewLog Extension

Add optional bean reference to brew logs:

```swift
// Add to existing BrewLog entity
@NSManaged var selectedBeanID: String?      // nil = general profile
@NSManaged var selectedBeanName: String?    // denormalized for display
```

---

## 11. Decisions — Iteration 3, Round 1

1. **Dual palette splits at the brew profile suggestion.** Health side: dark (#000000), blue accent. Coffee side: warm cream (#FAF6F0), amber accent (#D4A96A). Nav bar stays dark everywhere.
2. **Bean cards have two visual variants** — light roast (white card) and dark roast (dark card #2A2520). Variant is determined by `roastLevel`.
3. **Bean-first flow: Dashboard → Profile Suggestion → Bean Selection → Brew Settings → Brew.** The bean is the pivot between health data and brew execution.
4. **"Use general settings" exists at every level** — on the dashboard card, on the bean selection screen, and in brew settings. The user is never forced to pick a bean.
5. **Flavor visualization uses gradient spectrum sliders** with a positioned dot indicator. Three axes: Roast, Sweetness, Acidity. NOT discrete steps — continuous spectrum.
6. **Tasting note icons are custom flat vector illustrations**, not emoji, not SF Symbols. 14 icons needed for current catalog. Warm brown (#8C6B3E) on light, amber (#D4A96A) on dark.
7. **Serving infographic replaces text-only serving info.** Cup icons for serving size, clock arc for cutoff time, horizontal timing bar for brew window.
8. **Marketplace is a pushed view, not a tab.** Accessible from bean selection, nav cart icon, dashboard link, and settings. Uses profile-based filtering.
9. **"Add to Cart" opens Shopify in SFSafariViewController.** No in-app cart. Each purchase flows through the roaster's Shopify store.
10. **Subscribe & Save is a Shopify flow.** 15% discount positioning. Button only shows when the bean has a Shopify URL.
11. **Bean-specific brew adjustments are small deltas** on top of profile defaults. The profile dominates; the bean fine-tunes temperature, time, and dose.
12. **Price TBD state shows "COMING SOON" + "NOTIFY ME"** instead of price + "ADD TO CART". No fake prices.
13. **Three real beans are the initial catalog.** Stitch Dark Field (dark, Deep Work/Voltage), Corvus Ethiopia Guji Barrel (light, Voltage), Regent Ethiopia Guji Hambela (light, Tide Pool/First Light).
14. **Post-brew card gains a bean name line** when a specific bean was selected. Otherwise unchanged.
15. **The Blank profile shows all beans** without recommendation tags. Manual mode = no curation, full browse.

---

## 12. Build Order — For Jensen

**Phase 1 (Days 1-2):** Dual palette tokens + `BeanCardView.swift` + `FlavorSliderView.swift` + `TastingNoteView.swift`. Get a single bean card rendering with real data. This is the visual proof-of-concept.

**Phase 2 (Days 3-4):** `BeanSelectionView.swift` (Screen B) + `BrewSettingsView.swift` (Screen C). Wire the bean-first flow. Dashboard Card 3 CTA changes. General profile fallback works.

**Phase 3 (Day 5):** `ServingInfoCard.swift` + `ClockArcView.swift` + `TimingBarView.swift`. The infographic components. Drop into brew settings and bean detail.

**Phase 4 (Day 6):** `BeanDetailView.swift` + `BeanCardCompactView.swift`. The full detail screen and compact horizontal scroll variant.

**Phase 5 (Day 7):** `MarketplaceView.swift` + `FilterChipView.swift`. The Bean Shop. Filter logic. Subscribe banner. Wire all access points.

**Phase 6 (Day 8):** Polish. Animations. Dark/light card variant edge cases. Navigation flow testing. Post-brew bean name line.

**Total: ~8 days of additional work** on top of R3 build. Can be parallelized — Phase 1 components are independent of Phase 2 flow work.

### Asset Dependencies (blocking)

- **14 tasting note illustrations** — need from designer before Phase 1 is visually complete. Can use SF Symbol placeholders (`leaf`, `drop.fill`, etc.) until then.
- **3 bean bag product photos** — need from roaster partners. Can use placeholder images.
- **Roaster logos** — optional. The roaster name in ALL CAPS serves as the identity. Logos are a nice-to-have.

---

## 13. Copy Audit — All New Strings

### Navigation & Labels (7 strings)
```swift
"BREZI SUGGESTS"
"CHOOSE YOUR BEAN"
"Use general settings"
"RECOMMENDED BEANS"
"BROWSE ALL BEANS"
"VISIT BEAN SHOP"
"BEAN SHOP"
```

### Bean Selection (6 strings)
```swift
"Beans that match your morning signal today."
"BREW WITH THIS"
"Don't have a specific bean?"
"Use general {profileName} settings — optimized for the profile, not a specific origin."
"USE GENERAL PROFILE"
"or"
```

### Brew Settings (8 strings)
```swift
"YOUR BREW"
"Optimized for:"
"× {profileName} profile"
"START BREW"
"Change bean"
"Select a specific bean"
"Using general\n{profileName} settings"
"No specific bean selected. These parameters work with most medium roasts."
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

### Marketplace (8 strings)
```swift
"Roaster collabs. Curated for Brezi cold brew."
"RECOMMENDED FOR YOU"
"Beans that match today's {profileName} profile."
"ALL BEANS"
"Never run out."
"Subscribe to your favorite bean and save 15% on every delivery. Cancel anytime."
"LEARN MORE"
"More roasters coming soon.\nWant to partner?\nhello@brezicoffee.com"
```

### Bean Detail Labels (4 strings)
```swift
"ORIGIN"
"WHAT YOU'LL TASTE"
"BREW PAIRING"
"SERVING GUIDE"
```

### Price States (4 strings)
```swift
"ADD TO CART"
"SUBSCRIBE & SAVE"
"or subscribe & save 15%"
"PRICE COMING SOON"
"NOTIFY ME"
"We'll let you know."
```

### Empty/Error States (2 strings)
```swift
"No beans matched this profile yet.\nMore roasters coming soon."
"BROWSE BEANS"
```

### Bean Emotional Tags (3 strings — from roaster data)
```swift
"For when you mean business. Dense, structured, no apologies."
"Explosive fruit. A whiskey finish that lingers. The most complex thing in the lineup."
"The cleanest thing you'll drink today. Floral, bright, effortless."
```

**Total new strings: 54**

---

## 14. What Doesn't Change

- Onboarding flow (Screens 1-5) — stays dark, stays locked
- Health Data Card (Card 1) — stays dark, stays WHOOP-like
- Brezi's Read (Card 2) — stays dark, blue left border
- Emotional layer (R4/R5) — Why Today, check-in, surprise moments, return moments
- Brew profiles (Voltage, First Light, etc.) — names, parameters, decision logic
- Device Hub — stays dark
- Settings — stays dark (except new "Bean Shop" row link)
- All R3 design tokens for the health side
- All R4/R5 copy and emotional layer strings
- Navigation pattern (single push stack, no tab bar)
- All animations from R3/R4/R5

---

*The health side reads your body. The coffee side reads like a menu at your favorite roaster. Two brands, one app, one morning.*

*Round 4 built the data engine. Round 5 taught it to feel time. This round gives it a second visual language — the warm one. The one that makes you want to open the bag.*

— Picasso
