# Final Engineering Sign-Off — Brezi Emotional Layer v1
**Author:** Elon (COO / Growth + Copy) | March 19, 2026
**To:** Jensen (Engineering)
**CC:** Jackson (Product) · Picasso (Design)
**Status:** BUILD. This is the last document before code.

---

## 1. Is This Ready to Build?

Yes. Ship it.

Five rounds of design iteration produced a locked spec (Round 4) and six additive extensions (Round 5). No open questions remain on architecture, copy, or interaction design. Every user-facing string is paste-ready. Every animation has duration and easing. Every conditional has a trigger and a fallback.

**What you're building:**
- Five-job emotional architecture: Why Today → Card 3 → Flavor Preview → Bean Card → Check-In
- 11-bean database with origin stories, character tags, and sensory descriptions across 15 profile×tier combinations
- Six-option emotional check-in (Hit right / Needed more / Too much / Surprising / Flat / Not yet)
- Smart suppression on Why Today with three-stage cooldown
- Bean rotation on 2+ "Flat" ratings
- "Your Yirgacheffe" relationship line at 5+ brews
- "Not yet" deferred rating with 60-minute notification
- Six Round 5 extensions: bridge lines, four-node chain, maturation, flavor framing, temporal arc, return moments

**What you're not building (deferred to v2):**
- Pattern Vocabulary (one-time pattern summary card) — cut from R5. Competes with maturation copy, premature trigger, needs Core Data migration for cross-dimensional queries.
- Dynamic bean matching from emotional preference history
- Share mechanic for bean stories
- Observational surprise lines with pattern detection
- Core Data migration for emotional feedback accumulation

**Confidence level:** The architecture survived five rounds without a structural change. The five-job separation has never been violated. Every addition since Round 2 has been copy, timing, or conditional logic — not new screens, not new navigation, not new data models. The spec is stable.

---

## 2. Micro-Fixes to Apply While Coding

These are small enough that another design round would be wasteful. Apply them inline during implementation.

### Copy Fixes (3 strings — from Elon R5 review)

| Location | Current | Fix To | Why |
|---|---|---|---|
| Pattern intro (v2, but fix the template now) | `"{N} brews. You're consistent. Here's the data:"` | `"{N} brews. You're consistent. Here's what that looks like:"` | "Here's the data" sounds like a dashboard export. Pattern summary describes a portrait, not a spreadsheet. |
| Pattern detail (v2, but fix the template now) | `"{M} brews on {profileName}. Still finding the sweet spot — and that's normal at this stage."` | `"{M} brews on {profileName}. Still dialing. The data's getting sharper each time."` | "Sweet spot" is generic. "That's normal" is therapy voice — killed in R2. "Still dialing" is active, mechanical, Brezi-voiced. |
| Post-brew afternoon, Tide Pool | `"Brewed this morning. Gentle start, done its job."` | `"Brewed this morning. Gentle start. That's all it needed to be."` | "Done its job" is what you say about a vacuum. "That's all it needed to be" reframes simplicity as intentional. Matches Tide Pool's register across all other touchpoints. |

### Behavioral Micro-Fixes

**Maturation increment trigger:** Increment `whyTodaySessionCount` on Why Today **display**, not on app open or skip. A skipped Why Today should not count toward the familiarity that earns shorter copy. Only the user who watches earns the tighter register. Implementation: call the increment inside `recordWhyTodayInteraction(wasSkip: false)`, not inside `shouldShowWhyToday()`.

**Veteran template resolution:** Resolve `{lastPeakDay}`, `{midCount}`, `{daysSinceLow}` at **display time**, never at storage time. Store templates as templates. If "Best signal since Tuesday" gets cached in UserDefaults and displayed on Thursday, the temporal awareness becomes temporal confusion. Picasso's `resolveVeteranCopy()` function implies this but never says it explicitly. Make it explicit in your implementation: render-time only.

**Post-reintroduction permanent suppression:** The R4 spec's suppression logic has a comment (`// After 30-day suppress + another skip = set suppression to "9999-12-31"`) but the code path isn't fully written. Complete it: if `reintroduced == true` AND the user was already suppressed for 30 days AND they skip again on the second reintroduction, set `whyTodaySuppressedUntil = "9999-12-31"`. This is the permanent kill. Three chances given, three chances declined.

**Return session suppression reset:** Call `checkReturnReset()` **before** `shouldShowWhyToday()` on app launch. Order matters. The reset clears any active suppression so the returning user always sees Why Today fresh.

**"Not yet" notification content:** The R4 spec says the notification reads `"That First Light brew / How did it land?"` — use the actual profile name from the brew session. `"That \(profileDisplayName) brew\nHow did it land?"`. Two lines. No emoji. No exclamation.

### Visual Micro-Fixes

**Bean card left border on first card:** R4 says "3px, profile gradient start color." Confirm this is the start color of the *active profile's* gradient, not the bean's associated profile. A Colombia Huila bean (which serves both Tide Pool and First Light) should show the gradient of whichever profile is currently active, not the bean's primary profile.

**Flavor framing line color:** R5 specifies #888888, same as the data line and other supporting text. Correct. Do not use #CCCCCC (that's for primary descriptive text like sensory copy and taglines). The framing line is subordinate to the sensory description it introduces.

**Four-node chain vertical line:** 1px #222222. This is the same divider weight used throughout the app. Don't make it heavier to "connect" the nodes visually. The nodes connect through content, not chrome.

---

## 3. Core Data for Emotional Feedback

### Existing Entity — Extend, Don't Create

`BrewLog` already exists. The emotional layer needs these fields on it:

```swift
@objc(BrewLog)
class BrewLog: NSManagedObject {
    // ... existing fields ...

    // Emotional feedback — ADD THESE
    @NSManaged var emotionalRating: String?    // "hit_right", "needed_more", "too_much",
                                                // "surprising", "flat", "pending", nil
    @NSManaged var beanName: String?           // "Ethiopia Yirgacheffe" — links brew to bean
    @NSManaged var profileKey: String?         // "tide_pool", "first_light", etc.
    @NSManaged var recoveryScore: Int16        // recovery tier score at time of brew
    @NSManaged var recoveryTier: String?       // "high", "mid", "low" — derived, stored for query
}
```

### New Entity — `BeanProfile`

Already specified in R4 Section 2. Seeded with 11 records. Here's the full entity:

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
    @NSManaged var sortOrder: Int16          // display order within profile (mutable for rotation)
    @NSManaged var createdAt: Date
}
```

### UserDefaults Keys — Complete Registry

Everything that doesn't belong in Core Data lives in UserDefaults. Here's the full list across R4 + R5:

```swift
// === ROUND 4 ===

// Why Today — display gating
"lastWhyTodayDate"                  // String (date) — skip if matches today

// Why Today — smart suppression
"consecutiveWhyTodaySkips"          // Int — resets to 0 on any watch
"whyTodaySuppressedUntil"           // String (date) — "" = not suppressed
"whyTodayReintroduced"              // Bool — true after first reintroduction

// Why Today — surprise moments
"whyTodaySessionCount"              // Int — total displays (NOT opens, NOT skips)
"lastSurpriseSession"               // Int — session count at last surprise

// Why Today — need text rotation
"lastWhyToday_high"                 // Int (0, 1, or 2) — R4 rotation index
"lastWhyToday_mid"                  // Int
"lastWhyToday_low"                  // Int

// Emotional check-in — "Surprising" variety weighting
"surprising_{beanName}"             // Int — per-bean surprising count
                                    // e.g. "surprising_Kenya AA": 2

// Emotional check-in — "Not yet" deferred rating
"pendingRatingBrewId"               // String (UUID) — brew awaiting deferred rating
"pendingRatingScheduledAt"          // String (date+time) — when notification fires

// Feedback acknowledgment
"emotionalCount_{profileKey}_{tier}_{rating}"  // Int — for pattern detection
                                    // e.g. "emotionalCount_first_light_mid_hit_right": 7

// === ROUND 5 (additive) ===

// Why Today — maturation-aware need text rotation
"lastWhyToday_high_new"             // Int — replaces "lastWhyToday_high" for matured users
"lastWhyToday_high_familiar"        // Int
"lastWhyToday_high_veteran"         // Int
"lastWhyToday_mid_new"              // Int
"lastWhyToday_mid_familiar"         // Int
"lastWhyToday_mid_veteran"          // Int
"lastWhyToday_low_new"              // Int
"lastWhyToday_low_familiar"         // Int
"lastWhyToday_low_veteran"          // Int

// Why Today — veteran copy dynamic references
"lastSessionDate_high"              // String (date) — last time user saw high tier
"lastSessionDate_mid"               // String (date)
"lastSessionDate_low"               // String (date)
"tierSessionCount_high"             // Int — total high-tier displays
"tierSessionCount_mid"              // Int
"tierSessionCount_low"              // Int
```

**Migration note:** When `whyTodaySessionCount > 7` (user enters familiar tier), stop writing to `"lastWhyToday_high"` and start writing to `"lastWhyToday_high_familiar"`. The old keys can be ignored — new keys start at 0. No migration needed.

### What Does NOT Go in Core Data (v1)

- Emotional count accumulators (`emotionalCount_*`) — UserDefaults is fine for single-integer counters
- Suppression state — UserDefaults, three keys
- Maturation state — UserDefaults, session count
- Surprise moment tracking — UserDefaults, two keys
- Pattern summary dismissal — UserDefaults, one bool (v2 feature, but key is defined)

### What WILL Need Core Data (v2)

- Cross-dimensional queries: "show me all brews where profile=first_light AND bean=Guatemala Antigua AND rating=hit_right AND recovery_tier=mid" — this is a fetch request, not a UserDefaults scan
- Bean-level feedback aggregation: "average rating for Kenya AA across all profiles" — needs indexed queries
- Pattern detection over time windows: "your most common state in the last 14 days" — needs date-range predicates

For v1, the existing `BrewLog` entity with the five new fields handles everything. The cross-dimensional queries wait for v2.

---

## 4. Verbatim Copy Strings — The Ones That Must Ship Exactly As Written

These are the emotional high points. The lines that make the user feel known. Change the architecture, change the timing, change the color — but ship these strings exactly as they appear here. They've survived four rounds of copy audit.

### Why Today — Tier Words (3 strings, locked since R2)
```
Peaked.
Steady.
Rebuilding.
```

### Why Today — Need Text, the Anchors (3 strings, one per tier, locked since R2)
```
Clean night. Full signal. Go.
Not sharp, not broken. Just a normal morning doing normal morning things.
Nervous system's working hard. Don't add to its job.
```

### Why Today — Bridge (1 string, locked since R1)
```
This is what meets you there.
```

### Card 3 — Taglines That Define the Voice (5 strings)
```
The gentlest extraction Brezi makes. Low dose, no bite, no agenda.
Balanced extraction, clean finish. Your everyday standard, set high.
Dense extraction on a low morning. This is a bet. Rate it honest.
Concentrated, aggressive, not what the data recommends. You asked for it.
Low signal, manual override. You know what you're doing — or you're finding out.
```

### Cup Feelings — The Three-Word Standard (5 strings)
```
Quiet. Warm. Simple.
Clean. Steady. Familiar.
Dense. Heavy. Committed.
Electric. Complex. Alive.
Manual. Exact. Yours.
```

### Check-In Options (6 strings, exact labels)
```
Hit right
Needed more
Too much
Surprising
Flat
Not yet
```

### Feedback Acknowledgments — The Payoff Lines
```
{profile} on {tier}-recovery mornings. {count} for {count}. Your baseline's dialed.
Two flat ratings on {beanName}. We'll rotate the origin next time.
You keep rating too much at this level. Next time, we'll bump the dose down.
```

### Surprise Lines (8 strings, locked since R3)
```
You showed up. That's the whole thing.
Whatever you did last night — noted.
{N} days straight. We see that.
Same time tomorrow.
Brew. Then whatever today is.
Your morning. Your data. Your call. Always.
Not every day needs to be a peak day. This one counts too.
The routine is the thing. You already know that.
```

### The Relationship Line (1 string, locked since R4)
```
Your {region}. At this point, basically.
```

### "Not Yet" Notification (1 string, locked since R4)
```
That {profileName} brew
How did it land?
```

### The Blank's Identity (3 strings, locked since R3)
```
Your parameters, your bean, your morning. Brezi just holds the temperature.
Mid-range signal, manual brew. You're testing something. Brezi doesn't need to know what.
Low signal, manual override. You know what you're doing — or you're finding out.
```

### State-Bean Bridge Lines — The Best of R5 (4 strings)
```
Gentle origin for a low morning. That's the match.
Dense origin on a low day. The dose is the bet. The bean holds up.
Concentrated origin on low recovery. Not what the data suggests. Bean's ready if you are.
Clean bean on a rough morning. Honest cup, honest day.
```

### Return Moments — The Come-Back Lines (3 + 1 strings)
```
You're back. And you came back strong. Pick up where you left off.
You're back. Mid-range signal. Normal morning. Start there.
You're back. Low signal. Ease in. The routine remembers you.
Same place. Same morning. Let's go.
```

### Veteran Maturation — The Payoff for Staying (3 strings)
```
Full signal. You know what to do with this.
Steady. Your most common state. Brezi knows what works here.
Low signal. You've seen this before. It passes.
```

---

## 5. Implementation Priority

### Phase 1 — The Core Loop (Build First)

This is the app's daily heartbeat. Nothing else matters until this works.

**Day 1-2: Why Today + Emotional Check-In**
- `WhyTodayView.swift` — full Phase 1-3 animation sequence with tier words, data lines, need text, bridge
- Haptic on tier word
- Surprise moment system (8 variants, trigger at session 12+, every 10-15 after)
- Smart suppression (3-skip → 7-day → reintroduce → 30-day → permanent)
- Six-option `EmotionalRatingView.swift` (Hit right / Needed more / Too much / Surprising / Flat / Not yet)
- "Not yet" → pending state → 60-minute `UNNotificationRequest` → 24-hour expiration
- `recordWhyTodayInteraction()` + `shouldShowWhyToday()`
- All UserDefaults keys for suppression, rotation, session counting

**Day 3: Card 3 Emotional Layer + Bean Cards**
- `EmotionalBrewCardView.swift` — profile name, tagline, "This cup is:" strip, gradient strip
- All 15 taglines, all 15 cup feelings
- `BeanCardView.swift` — character tag, flavor feeling, sensory cue
- `BeanProfile` Core Data entity + 11-bean seed data
- Bean sort order display (position 1 = recommended)

**Day 4: Flavor Preview + Origin Stories**
- `FlavorPreviewView.swift` — expanded detail with all 15 sensory descriptions
- `OriginStorySheet.swift` — half-sheet with origin story, metadata, brew-history badge
- "Your Yirgacheffe" relationship line (conditional at 5+ brews)
- `brewHistoryForBean()` query

**Day 5: Feedback Loop + Bean Rotation**
- Feedback acknowledgment copy (locked_in, rotation, too_much patterns)
- `emotionalToStars` mapping with `surprising = 0` decoupling
- `surprising_{beanName}` variety weighting
- `checkBeanRotation()` + `rotateBeanForProfile()` — sort-order demotion on 2+ Flat
- `BrewLog` emotional fields (emotionalRating, beanName, profileKey, recoveryScore, recoveryTier)

### Phase 2 — Temporal Extensions (Build After Phase 1 Ships)

These are the Round 5 additions. All additive. No Phase 1 changes required.

**Day 6: Why Today Maturation + Return Moments**
- Maturation tiers: new (1-7), familiar (8-30), veteran (31+)
- 18 new need text variants (9 familiar + 9 veteran)
- Veteran dynamic references (`{lastPeakDay}`, `{midCount}`, `{daysSinceLow}`) with display-time resolution
- `resolveVeteranCopy()` with nil-fallback to familiar tier
- `recordWhyTodaySession()` for tier tracking
- Return detection (`daysSinceLastSession() >= 3`)
- Return-specific need text (3 strings), return bridge (1 string), data line prefix
- `checkReturnReset()` — suppression clear on 3+ day absence

**Day 7: Bridge Lines + Flavor Framing + Four-Node Chain**
- State-Bean Bridge Lines: 12 strings on first bean card, below characterTag
- Flavor framing lines: 12 strings prepended to sensory description in detail screen
- `WhyThisBrewView.swift` — four-node reasoning chain (signal → profile → bean → parameters)
- 13 bean pairing rationale strings
- Sequential node animation (0.25s per node, 0.2s delay, vertical line draw)

**Day 8: Full-Day Emotional Arc**
- Three post-brew phases: recent (0-2h), afternoon (2-6h), evening (6h+)
- 10 new post-brew copy strings (5 afternoon + 5 evening)
- Rating CTA visibility logic (shows recent/afternoon, hides evening)
- Phase transition on app foreground (crossfade 0.3s)
- Apply afternoon Tide Pool copy fix: "That's all it needed to be."

### What Can Slip Without Damage

If you need to cut scope, cut in this order (last = cut first):

1. **Full-Day Emotional Arc** — the post-brew card can stay static for v1. Users don't expect the card to change at 3pm. Nice-to-have, not load-bearing.
2. **Four-Node Chain** — the three-node chain (signal → profile → parameters) works. The bean node adds clarity but the bridge line on the bean card already closes the gap.
3. **Flavor Framing Lines** — the sensory description stands alone. One framing sentence is polish, not structure.
4. **State-Bean Bridge Lines** — the connection between state and bean is implicit. Making it explicit is better. Losing it doesn't break comprehension.
5. **Return Moments** — the app works fine without noticing you left. The return acknowledgment is emotional, not functional.
6. **Why Today Maturation** — the existing copy works at any tenure. Getting shorter over time is earned familiarity, but the default copy doesn't expire.

**Do not cut from Phase 1.** The core loop (Why Today → Card 3 → Check-In → Feedback → Rotation) is the product. Everything in Phase 1 is load-bearing.

---

## Final Word

Five rounds. Forty decisions. Over a hundred user-facing strings audited to the word. Zero open design questions. The architecture is locked. The voice is locked. The copy is locked.

What you're building isn't a feature set. It's an emotional system — one that recognizes your body state, matches it to a coffee, tells you why in language that passes the text-message test, learns from your feedback, and gets quieter as it knows you better. That's the whole product thesis in one loop.

Build the core loop first. Make it airtight. Then layer the temporal extensions. If the core loop doesn't feel right, nothing else matters. If it does, everything else is compounding.

Lock it. Build it. Ship it.

— Elon
