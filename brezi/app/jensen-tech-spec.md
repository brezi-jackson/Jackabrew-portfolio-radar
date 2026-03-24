# Brezi iOS App — CTO Technical Specification v0.1
**Author:** Jensen (CTO) | March 2026 | Internal Only

---

## Tech Stack

- **UI:** SwiftUI (fast iteration, modern, Combine-compatible)
- **State management:** Single `AppState: ObservableObject` (EnvironmentObject) + per-screen ViewModels
- **Persistence:** Core Data (NSPersistentContainer, SQLite-backed) — future iCloud sync ready
- **DTOs:** Codable structs mirroring Core Data entities (for tests + export)
- **Dependencies:** Swift Package Manager only (no CocoaPods)
- **Tooling:** SwiftLint, XCTest, Xcode 15+, iOS 16+ target
- **Analytics:** Local event logging to file for v0.1; remote analytics later

---

## Data Models

### Core Data Entities

**User**
- id: UUID (indexed)
- createdAt: Date
- preferredUnits: String? ("metric"/"imperial")
- lastCheckInAt: Date?
- notes: String?

**BrewProfile** (static seeded rows, not user-editable in v0.1)
- id: UUID
- key: String (unique — e.g. "gentle", "standard")
- name: String
- minRecovery: Int16
- maxRecovery: Int16
- tempC: Double
- steepHoursMin: Double
- steepHoursMax: Double
- coffeeGramsPerLiter: Double
- caffeineEstimate: String ("low", "medium", "high")
- rationale: String (one-line)
- notes: String? (longer explanation)
- createdAt: Date

**CheckIn**
- id: UUID
- userId: UUID
- recoveryScore: Int16 (1–10)
- source: String ("manual" for v0.1)
- createdAt: Date
- timezoneOffsetSeconds: Int32
- note: String?

**BrewLog**
- id: UUID
- profileKey: String
- brewedAt: Date
- brewSettingsJSON: String? (JSON snapshot)
- howFelt: Int16? (1–5 stars)
- note: String?
- createdAt: Date

---

## The 5 Brew Profiles

| Profile | Score | Temp | Steep | Ratio | Caffeine | Rationale |
|---|---|---|---|---|---|---|
| **Rest Day** | 1–2 | 4°C | 6–8h | 40 g/L | Low | Minimize caffeine — reduced ratio + shorter steep |
| **Gentle** | 3–4 | 4°C | 8–10h | 50 g/L | Low–Med | Easy on nervous system, mild body |
| **Standard** | 5–6 | 4–8°C | 12–14h | 60 g/L | Medium | Balanced everyday extraction |
| **Strong** | 7–8 | 15–20°C | 12–16h | 80 g/L | High | Higher ratio + room temp accelerates extraction |
| **Adaptogen Boost** | 9–10 | 15–20°C | 12–16h | 60–80 g/L | Med–High | Peak day — pairs with optional adaptogen add-in |

---

## Decision Logic (Rules Engine)

```swift
func profileKey(for score: Int) -> String {
    switch score {
    case ...2:   return "rest_day"
    case 3...4:  return "gentle"
    case 5...6:  return "standard"
    case 7...8:  return "strong"
    case 9...10: return "adaptogen_boost"
    default:     return "standard"
    }
}
```

**Future enhancements (not v0.1):**
- Weighted scoring: adjust by last brew's `howFelt` rating
- Time-of-day modulation (no strong brew after 2pm)
- Personalized ML after ~5,000 data points

---

## Screen Breakdown

### Screen 1 — Morning Check-in (`MorningCheckInView`)
- Title: "Morning check-in"
- Subtitle: "How recovered do you feel right now?"
- `RecoverySliderView` — 1–10 ticks, large, haptic feedback, numeric readout
- Quick presets: "1–3 Rest" / "4–6 Standard" / "7–10 Boost"
- Primary CTA: "Get Recommendation"
- Secondary: "Skip"
- Default value: last check-in score or 5
- **Est. build time: 3–4 days**

### Screen 2 — Brew Recommendation (`BrewRecommendationView`)
- Large `ProfileCardView`: name, icon, one-line rationale
- Brew settings row: Temp (human label), Steep time, Ratio
- Explanation: "Recommended because your recovery score was X"
- Primary: "Log Brew" (saves BrewLog)
- Secondary: "Adjust Settings" (manual override sliders)
- Tertiary: "Back"
- "Why this brew?" expandable footer
- **Est. build time: 3–4 days**

### Screen 3 — Brew Log (`BrewLogView`) — v0.1 stretch
- List of BrewLog cells: date, profile name, settings summary, star rating
- Tap to add/edit `howFelt` rating
- **Est. build time: 2–3 days**

### Screen 4 — Settings (`SettingsView`) — placeholder only
- Wearable connect placeholder (disabled toggle)
- Units preference
- **Est. build time: 1 day**

---

## Reusable Components
- `RecoverySliderView.swift` — large-tap slider 1–10 with haptics
- `ProfileCardView.swift` — profile card (name, icon, temp/steep/ratio, rationale)
- `SaveButton.swift` — primary CTA component
- `RatingStarsView.swift` — 1–5 star tap control

---

## File Structure
```
BreziApp.swift              — app entry, DI setup, seed profiles
PersistenceController.swift — Core Data stack
DataRepositoryCoreData.swift — Repository implementation
AppState.swift              — global ObservableObject
MorningCheckInView.swift + ViewModel
BrewRecommendationView.swift + ViewModel
BrewLogView.swift + ViewModel (stretch)
SettingsView.swift          — placeholder
RecoverySliderView.swift
ProfileCardView.swift
SaveButton.swift
RatingStarsView.swift
```

---

## Design Lock Requirements (before coding starts)
1. Color palette (hex values) — primary, secondary, background, accent
2. Typography: font names, weights, sizes for H1/H2/body/caption
3. Icon style for each profile (SF Symbols or custom?)
4. Card component design (corner radius, shadow, padding spec)
5. Slider visual style (track color, thumb design)

---

## Total Estimated Build Time (solo developer)
- Screen 1 + 2 (core MVP): **6–8 days**
- Screen 3 Brew Log (stretch): +2–3 days
- Screen 4 Settings placeholder: +1 day
- Core Data setup + Repository + seeding: +2 days
- Testing + TestFlight prep: +2 days

**Realistic TestFlight target: 3–4 weeks from design lock**

---

## Architecture Note
Build the logging infrastructure like it matters from day one. Even 50,000 users logging HRV + brew preferences creates a dataset no competitor has. The data flywheel is the moat — engineer it that way from v0.1.
