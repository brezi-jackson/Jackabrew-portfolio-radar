# Jensen v4 Build Summary — Coffee-Side Experience

## What Was Built

Full implementation of the Visual Coffee Layer & Marketplace v5.0 spec. Six new screens, eight new components, three real roaster beans seeded, dual-palette system wired end-to-end from dashboard through brew start.

---

## New Files Created (14)

### Components (8)
| File | Purpose |
|------|---------|
| `Components/FlavorSliderView.swift` | Horizontal gradient slider bars (Roast/Sweetness/Acidity) with 5-stop LinearGradient + positioned dot indicator. Supports `isDarkCard`, `showEndpoints`, `compact` modes. |
| `Components/TastingNoteView.swift` | SF Symbol tasting note icons in three sizes (.mini/.standard/.detail). Lead icon gets accent highlight. Includes `TastingNoteRow` convenience wrapper. |
| `Components/BeanHistoryCard.swift` | Brew history card (threshold: 3+ brews). Two variants: compact (single line) and full (amber left border + insight text). |
| `Components/FilterChipView.swift` | Capsule filter chips with spring selection animation. `FilterChipRow` with horizontal scroll + binding. |
| `Components/ServingInfographicView.swift` | Serving guide: cup icons (1-3), cutoff time, timing bar with green brew window / grey post-cutoff. |
| `Components/GeneralProfileCard.swift` | Equal-weight "no specific bean" card matching BeanCardView dimensions. Green "BREW GENERAL" CTA. |
| `Components/BeanCardView.swift` | Full visual treatment bean card with `BeanCardContext` enum (.selection/.marketplace/.detail). Two color variants (light/dark). Roaster badge, flavor sliders, tasting notes, recommendation tag, bridge line, inline BeanHistoryCard, SFSafariViewController for Shopify. |
| `Components/BeanCardCompactView.swift` | 180pt-wide compact card for horizontal scroll. Mini tasting notes, compact flavor sliders, no CTA (tap navigates to detail). |

### Screens (6)
| File | Purpose |
|------|---------|
| `Views/Dashboard/PaletteTransitionView.swift` | Screen 2: Hero palette transition (#0F0F0F -> #FAF6F0, 1.8s). Typewriter text animation for 3 copy lines. WhyToday maturation (new/familiar/veteran) + return session copy. "SEE YOUR BEANS" CTA + skip. |
| `Views/BrewRecommendation/BeanSelectionView.swift` | Screen 3: Profile-filtered marketplace beans with full BeanCardView. GeneralProfileCard as equal alternative. Horizontal "Browse all beans" scroll with compact cards. "VISIT BEAN SHOP" link to marketplace. |
| `Views/BrewRecommendation/BrewSettingsView.swift` | Screen 4: Brew settings with "Optimized for [Bean] x [Profile]" badge (bean-specific) or "Using general settings" (general). 3-box param summary, adaptation note at hinge, param detail table, flavor preview, ServingInfographicView. Records flow completion on brew start. |
| `Views/BrewRecommendation/BeanDetailView.swift` | Full bean detail: hero image, origin card, flavor card with sliders, brew pairing with profile chips, ServingInfographicView, BeanHistoryCard (3+ brews), CTA priority flip after 3+ brews, subscribe/notify states. SFSafariViewController for Shopify. |
| `Views/MarketplaceView.swift` | Full bean shop: FilterChipRow with profile filters, "RECOMMENDED FOR YOU" section, all-TBD pricing banner, subscribe banner, partner footer, empty filter state with nearby profile suggestion. |
| (PaletteTransitionView also contains) | `WhyTodayCopyResult` struct + resolver functions for new/familiar/veteran/return copy tiers. `DateFormatter.yyyyMMdd` extension. `Color.components` extension. |

---

## Modified Files (7)

| File | Changes |
|------|---------|
| `Models/BrewProfileData.swift` | BeanSeed struct: added `Identifiable`, 11 new fields (roasterName, price, shopifyUrl, roastScore, sweetnessScore, acidityScore, tastingNoteIcons, bagImageUrl, emotionalTag, showEmotionalTagOnCard, cardVariant). Added `marketplaceBeans` array (3 real roaster beans), lookup methods, bridge lines (12 strings), adaptation notes (12 strings), profileCutoffHours, profileServings. |
| `Design/BreziColors.swift` | Added 12 coffee-side palette tokens (coffeeBg, coffeeSurface, coffeeCardBg, coffeeCardDark, coffeeTextPrimary/Secondary/Tertiary, coffeeAccent/AccentHover, coffeeGreen, coffeeTagBg, coffeeDivider). |
| `AppState.swift` | Added: selectedBean, showPaletteTransition, showBeanSelection, showMarketplace, showBrewSettings, completedBrewFlows, showGeneralShortcut, recordFullFlowCompletion(), recommendedMarketplaceBean, marketplaceBeansForProfile. Added reset keys. |
| `Components/NavBarView.swift` | Added `onBag` callback, `NavBarVariant` enum (.health/.coffee). Coffee variant shows bag icon. |
| `Components/DashboardBrewCardView.swift` | EmotionalBrewCardView: added onChooseBean, onUseGeneralSettings, showGeneralShortcut. "CHOOSE YOUR BEAN" as primary CTA, gated "Use general settings" secondary. |
| `Views/Dashboard/DashboardView.swift` | Added navigation states + destinations for PaletteTransitionView, MarketplaceView, BrewSettingsView(general). Wired EmotionalBrewCardView with new callbacks. Updated beanSection to use new BeanCardView signature. Added coffeeSideCTAs section. |
| `Views/BrewRecommendation/BrewRecommendationView.swift` | Updated BeanCardView calls to new signature (context, isFirstRecommended, brewHistoryCount, onBrewWithThis). |
| `Models/CopyStrings.swift` | Added ~65 coffee-side copy strings across palette transition, bean selection, bean card, bean detail, brew settings, marketplace, general profile card, serving guide, bean history. |

---

## 3 Real Roaster Beans Seeded

| Bean | Roaster | Card | Roast/Sweet/Acid | Price | Profiles |
|------|---------|------|-------------------|-------|----------|
| Dark Field | Stitch Coffee | Dark | 90/55/20 | TBD | deep_work, voltage |
| Ethiopia Guji Hambela Washed | Regent Coffee | Light | 15/80/75 | $27.00 | tide_pool, first_light |
| Sadayana Winey | Corvus Coffee | Light | 45/85/50 | TBD | deep_work, voltage |

---

## Navigation Flow (End-to-End)

```
Dashboard
  -> "CHOOSE YOUR BEAN" (EmotionalBrewCardView)
    -> PaletteTransitionView (typewriter + bg crossfade)
      -> BeanSelectionView (profile-filtered beans)
        -> BeanDetailView (full detail)
          -> BrewSettingsView (optimized for bean)
            -> startBrew() -> Dashboard (brewing state)
        -> BrewSettingsView (bean-specific)
        -> BrewSettingsView (general, no bean)
      -> MarketplaceView
  -> "Use general settings" (gated, 3+ flows)
    -> BrewSettingsView (general)
  -> "BROWSE BEANS"
    -> MarketplaceView
```

---

## Key Design Decisions

1. **Typewriter animation**: Uses enum-based field dispatch instead of keypaths (SwiftUI struct constraint). Progressive character append via DispatchQueue scheduling.
2. **Bean card variants**: Single `BeanCardView` with `BeanCardContext` enum handles all three contexts (selection, marketplace, detail) with different CTA layouts.
3. **Dark card support**: `CardVariant` enum on BeanSeed. Dark cards (e.g. Stitch Dark Field) invert text colors and use amber accent instead of green.
4. **General settings gate**: `showGeneralShortcut` only appears after `completedBrewFlows >= 3` (tracked in UserDefaults).
5. **WhyToday maturation**: Copy resolves across three tiers (new <8 sessions, familiar 8-30, veteran 31+) plus return session (3+ day gap).
6. **SFSafariViewController**: Used for Shopify links (in-app browser, no context switch).
7. **Backward compatibility**: All new BeanSeed fields use default values so existing 11 bean initializations don't break.

---

## Known Limitations / v1 Gaps

- **Bean images**: Placeholder SF Symbol `bag.fill` — real bag photos need asset pipeline
- **Core Data brew history per bean**: `brewHistoryForBean()` exists but `bestRecovery` in BeanHistoryCard uses placeholder value (70)
- **Shopify URLs**: Empty for TBD-priced beans — needs real Shopify product IDs when pricing finalizes
- **Veteran copy tier**: Currently falls through to familiar copy (simplified for v1)
- **Subscribe state**: Checked via UserDefaults `subscribedBeans` key — needs real Shopify subscription API integration
- **Flavor sliders**: Static values from seed data — no dynamic adaptation based on brew parameters yet
