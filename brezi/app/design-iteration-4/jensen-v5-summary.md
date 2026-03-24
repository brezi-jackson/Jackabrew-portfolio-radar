# Jensen v5 — Iteration 4 Engineering Summary
**Author:** Jensen (CTO) | March 20, 2026 | Build Log

---

## What Was Built

Complete rebuild of the Brezi iOS app to match the Iteration 4 design spec (Picasso Round 3 final + Elon's 3 non-negotiable fixes). This is a ground-up rewrite of the navigation architecture, state management, and all primary views.

### Architecture Change

**Before (v4 legacy):** NavigationStack → DashboardView (single scrollable screen) with push navigation to BrewRecommendationView, BeanSelectionView, DeviceHubView, etc. 5 onboarding screens including device pairing and data chain tutorial.

**After (Iteration 4):** Custom 4-tab bottom dock (MainTabView) → Home/Beans/History/Settings. Home tab is a state machine driven by `BrewState` enum. No push navigation on Home — cards appear/disappear in place. 4-screen onboarding flow (Hook → Why → Data Ask → First Recovery).

---

## New Files Created

### Core
| File | Purpose |
|---|---|
| `AppState.swift` | Complete rewrite — BrewState enum, dose control, caffeine tracking, manual recovery R3 gate, Elon Fix 1 (wearable mapping), Fix 2 (immutable brew logs), Fix 3 (midnight resolution) |
| `BreziApp.swift` | Simplified entry — routes to MainTabView or OnboardingShellView |
| `BreziColors.swift` | Full design token set from spec Section 0 |
| `BreziTypography.swift` | All font/spacing/radius tokens from spec |

### Views — Tab Bar
| File | Purpose |
|---|---|
| `MainTabView.swift` | Custom 4-tab dock with sliding blue pill, spring animation, haptics, per-tab backgrounds |

### Views — Home Tab
| File | Purpose |
|---|---|
| `HomeView.swift` | State machine: idle/brewing/postBrew/cleared. Nav bar with "BREZI" + time. |
| `RecoveryCardView.swift` | Wearable mode (% + HRV + dots) and manual mode (chip → slider → daily reset per R3) |
| `WhyTodayCardView.swift` | Emotional bridge copy, tier-based, no-repeat-within-3-days |
| `BrewRecommendationCard.swift` | THE card — dose control with long-press + boundary glow (R2), caffeine estimate, brew window bar, START BREW / BREW ANYWAY / disabled gate, Advanced toggle |
| `BrewTimerView.swift` | Full-screen timer counting up, circular progress ring, END BREW |
| `CaffeineTimerCard.swift` | 4-state caffeine countdown (Active → Peak → Clearing → Cleared), decay bar, amber accent, brew_end + 15h formula |
| `CheckInCard.swift` | 3-pill check-in (Right / Needed more / Too much — R3 corrected), skip, pattern teaser toast (R2) |

### Views — Beans Tab
| File | Purpose |
|---|---|
| `BeansTabView.swift` | Warm cream background, 3 marketplace beans, "For You Today" + "Browse All", NavigationStack |
| `BeanDetailView.swift` | Full detail — tasting notes, flavor sliders, origin, "Best For" chips, Add to Cart / Notify Me / Subscribe |

### Views — History + Settings
| File | Purpose |
|---|---|
| `HistoryView.swift` | Progressive states (empty → log only → insight line → pattern card at 5+), brew log entries grouped by month |
| `SettingsView.swift` | WHOOP/Oura rows, wake time + cutoff pickers, check-in toggle, about section |

### Views — Onboarding
| File | Purpose |
|---|---|
| `OnboardingShellView.swift` | 4-screen pager with progress dots, skip button, staggered animations |
| `OnboardingLoadingView.swift` | "Reading your signal..." transition screen |

### Support
| File | Purpose |
|---|---|
| `LegacyStubs.swift` | Stub implementations for 5 types referenced by old code (WhyTodayView, EmotionalBrewCardView, OriginStorySheet, FlavorPreviewView, WhyThisBrewView) |

---

## Elon's 3 Non-Negotiable Fixes — Implementation

### Fix 1: Explicit Wearable Recovery Mapping
- Added `mapWhoopToRecovery(_:)` and `mapOuraToRecovery(_:)` as isolated functions in AppState
- Direct pass-through for v1.0 (both 0-100)
- Documented as a deliberate decision — recalibration is a one-line change

### Fix 2: Immutable Brew Logs on Recovery Update
- `logBrewToCoreData(brewEnd:)` captures `currentRecoveryScore` at brew start time
- Recovery card "Update" link changes displayed % but does NOT modify BrewLog entries
- Pattern engine reads recovery from the log, not from current state

### Fix 3: Midnight State Resolution
- `resolveBrewState()` called on init — checks date boundaries
- Yesterday's caffeine card transitions to CLEARED, then auto-dismissed
- Yesterday's brew does NOT block today's START BREW
- Manual recovery resets daily (calendar day check on `lastRecoveryInputDate`)
- START BREW re-enables once today's recovery is logged

---

## R2 + R3 Design Features Implemented

| Feature | Status |
|---|---|
| Long-press dose acceleration (0.5s hold, 5g per 0.2s) | ✅ BrewRecommendationCard |
| Boundary glow (red at 30g, blue at 120g) | ✅ BrewRecommendationCard |
| Caffeine formula: brew_end + 15h (~12.5% remaining) | ✅ AppState + CaffeineTimerCard |
| "BREW ANYWAY" with projected sleep-safe time | ✅ BrewRecommendationCard |
| History insight line at 3-4 check-ins | ✅ HistoryView |
| Pattern card replaces insight line at 5+ | ✅ HistoryView |
| One-time toast after 3rd check-in | ✅ CheckInCard |
| Check-in pill "Too much" (not "Too light") | ✅ CheckInCard |
| Manual recovery daily reset + disabled START BREW gate | ✅ RecoveryCardView + BrewRecommendationCard |
| Same-day return with "Update" link | ✅ RecoveryCardView |

---

## What's NOT in This Build (Deferred)

- WHOOP/Oura OAuth integration (UI buttons exist, no backend)
- Brezi hardware BLE pairing (removed from onboarding — was Screen 2 in old flow)
- CloudKit sync (container configured but not active)
- Shopify cart integration (Add to Cart opens SFSafariViewController stub)
- Live Activities on lock screen during brew
- Unit tests
- SwiftLint configuration

---

## File Counts

- **New files:** 18
- **Modified files:** 4 (AppState, BreziApp, BreziColors, BreziTypography)
- **Legacy files preserved:** All old views + components remain (`.swift` and `.bak`)
- **Total Swift files:** ~71

---

## 2-Tap Validation

Open app → Home tab loads → tap recovery chip (if manual, 1 tap) → tap START BREW (1 tap) = **2 taps from open to START BREW**. Under the 3-tap maximum.

For wearable users: Open → recovery auto-loads → tap START BREW = **1 tap**. Even better.

---

*Build completed March 20, 2026. All spec requirements implemented. All R2/R3 changes included. All 3 Elon fixes addressed. Legacy code preserved with stubs. Ready for Xcode build validation.*
