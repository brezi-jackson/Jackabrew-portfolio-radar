# Elon — Round 3 Critique (Iteration 4)
**Author:** Elon (COO / Product & Growth) | March 20, 2026
**Spec reviewed:** Picasso Round 3, Iteration 4
**Lens:** Would a wellness-first user (WHOOP band, 28-38yo, tracks HRV/sleep) open this app every morning?

---

## Overall Verdict

Ship it.

All four Round 2 flags resolved correctly. Brief corrections applied. Manual recovery daily reset is fully specified — daily gate, disabled START BREW, same-day read-only return with Update link. The history progressive states are now explicit: insight line at 3–4, pattern card replaces it at 5+. No ambiguity left.

This is the first round where I don't have a structural concern. The architecture holds. The copy holds. The caffeine math holds. What I have below are refinements, not blockers — but two of them matter enough for the engineering handoff that I'm flagging them as non-negotiable anyway.

---

## Section-by-Section

### 1. BREW CARD — Max 3 taps to START BREW? Dose +/- natural? Caffeine estimate trusted?

**Locked. No notes.** Two taps from open, dose control is clean, caffeine estimate is trusted. Long-press acceleration, boundary glow, and all R2 additions are correctly specified. Nothing to add.

### 2. CAFFEINE TIMER — Genuinely useful for wellness user? Would you check this before bed?

**Locked. Formula confirmed.** `brew_end + 15h` everywhere. The "Why 15h" rationale paragraph survived into R3. Good — that paragraph is load-bearing documentation.

One observation that doesn't require a spec change: the four-state caffeine card (ACTIVE → PEAK → CLEARING → CLEARED) is the single most "WHOOP-like" moment in the app. It mirrors how WHOOP transitions your recovery status through the day. The user will recognize this cadence intuitively. This is the feature that earns daily opens.

### 3. CHECK-IN — Lightweight? Feels optional, not obligatory?

**Triad confirmed: Right / Needed more / Too much.** The brief correction in Section 13 is clear. Data integrity concern resolved — the pattern engine can now read the signal cleanly.

The 30-minute delay before the check-in appears is the right call. It means the user evaluates the brew after the caffeine has actually landed, not while they're still drinking it. A WHOOP user understands delayed feedback — your recovery score reflects yesterday, not right now. Same principle.

**One thing I want to call out that's working well:** the check-in is three pills and a skip link. No text input. No star rating. No "rate 1-5." The constraint of three options is what makes the data useful — it forces a clear signal, and users learn the vocabulary fast. Don't ever expand this to more options.

### 4. TAB BAR — Dock style premium? 4 tabs obviously named?

**Locked. No notes.** The sliding pill animation with spring damping is the right energy. Four tabs, four obvious labels. The warm-cream Beans tab background shift is the moment that signals "you've left the health cockpit."

### 5. BEANS TAB — Truly non-blocking? Separate world feel?

**Locked. Confirmed non-blocking.** You can use this app for months without ever tapping the Beans tab. The brew card works with or without a bean selection. The "For You Today" section is a nice touch — it bridges the health data into bean recommendations without requiring anything from the user.

### 6. HISTORY — Pattern cards feel personal, not generic?

**R3 clarification is exactly what I needed.** The progression is now unambiguous:

| Check-ins | What renders |
|---|---|
| 0 | Empty state |
| 1–2 | Log entries only |
| 3–4 | Insight line + log entries |
| 5+ | Pattern card + log entries (insight line gone) |

The key insight: **the insight line is scaffolding, not content.** It exists to signal "we're paying attention" during the gap between first use and first pattern. Once the pattern card earns its place, the scaffolding comes down. Clean transition. No awkward "insight line AND pattern card" stacking.

### 7. ONBOARDING — Under 2 min? No unnecessary friction?

**Locked since R1, and still correct.** Four screens, progressive disclosure, skip on every screen. The R3 addition of setting `lastRecoveryInputDate` on Screen 4 completion is the right fix — it ensures the manual recovery gate is satisfied on first launch. Without this, a new manual-input user would see a disabled START BREW immediately after onboarding, which would feel like a bug.

### 8. COPY — Warm and direct, not clinical or coffee-nerdy?

**Copy is locked and it reads well.** The Why Today variants have the right range — peaked copy is confident ("This is what discipline earns you"), rebuilding copy is empathetic without being soft ("Don't fight it, work with it"). The gate copy for disabled START BREW ("Log your recovery to unlock today's brew") is functional without being punitive.

The complete copy strings index in Section 7 is the kind of thing that saves 20 hours of back-and-forth during engineering. Every string has a key. Every key has a purpose. Ship the index as-is.

### 9. OVERALL — Wellness app that happens to have coffee, or coffee app with wellness features?

**Wellness app. Definitively.** The recovery card is the first thing you see. The brew recommendation adapts to your body. The caffeine timer tracks your metabolic state. The check-in feeds a learning loop. Coffee is the medium, not the message.

The Beans tab is where the coffee brand lives, and it's correctly quarantined behind its own tab with its own visual language. A WHOOP user who never cares about single-origin Ethiopian beans will never need to open it. A specialty coffee person who discovers the Beans tab will feel at home. Both users get what they need without the other's world leaking in.

---

## Non-Negotiable Fixes for Engineering Handoff

### Fix 1: WHOOP/Oura recovery mapping must be explicit

**The problem:** The spec says "WHOOP API → real recovery %" and "Oura readiness score mapped to recovery %." But it doesn't specify the Oura mapping. WHOOP recovery is 0–100% and passes through directly — fine. Oura's readiness score is also 0–100 but the distributions are different. An Oura readiness of 70 does not feel the same as a WHOOP recovery of 70.

**The fix:** Add to Section 2A (Recovery Card, Data source priority):

```
WHOOP: recovery_pct = whoop_recovery_score (direct pass-through, 0–100)
Oura:  recovery_pct = oura_readiness_score (direct pass-through for v1.0)
```

For v1.0, direct pass-through on both is acceptable — the profile thresholds (0–49 Tide Pool, 50–74 First Light, 75–84 Deep Work, 85–100 Voltage) are wide enough to absorb the distribution differences. But this must be a **documented decision**, not an implicit assumption. If we find Oura users consistently mapping into the wrong profile tier in beta, we'll need a calibration offset — and we can only add one if the mapping is explicit and isolated in the code.

**Why this matters:** If a dev hardcodes `recoveryPercent = ouraReadinessScore` buried in a network call without a mapping function, recalibration later requires a refactor. If it's isolated in a `mapOuraToRecovery(_ readiness: Int) -> Int` function, it's a one-line change. Architecture decision, not a design decision — but it needs to be in the spec so engineers build the abstraction.

### Fix 2: Recovery update after brewing — edge case needs a rule

**The problem:** The manual recovery same-day return shows an "Update" link. A user can tap Update and change their recovery score after they've already brewed. The spec doesn't say what happens to the active brew or the brew log entry.

Consider: User logs recovery at 60% (Functional) → gets First Light profile → starts brew at 60g → ends brew → later taps "Update" on recovery card → changes to 80% (Deep Work). Now the brew log says "First Light, 60g, Recovery: 60%" but the recovery card says 80%.

**The fix:** Add a rule to the same-day return behavior:

```
Recovery update after brew: If a brew has been logged today (lastBrewEndTime is today),
updating recovery changes the displayed recovery percentage on the Home card but does NOT
retroactively modify the BrewLog entry. The BrewLog preserves the recovery score that was
active when the brew was started. This is the score that fed the recommendation.
```

The brew log is a record of what happened, not what should have happened. The recovery card shows current state. These are different things and they should stay different.

**Why this matters:** If the brew log retroactively updates, the pattern engine's data becomes unreliable — it would show "Deep Work at 80% recovery" for a brew that was actually recommended as First Light at 60%. The pattern "you rate First Light highest on 60% days" would never emerge because the data was rewritten. Immutable brew logs = trustworthy patterns.

### Fix 3: Stale brew state across midnight — what resets, what persists

**The problem:** The spec handles daily reset for manual recovery (`lastRecoveryInputDate` checked against today). But it doesn't address what happens to a caffeine countdown card that spans midnight. If I brew at 3 PM, my sleep-safe time is 6 AM the next day. When I open the app at 7 AM the next morning:

- The caffeine card should show "CLEARED" (it's past sleep-safe)
- The recovery card should show a fresh input prompt (new day)
- The Why Today card should show a new message
- But: does `lastBrewEndTime` from yesterday prevent a new brew today? The spec doesn't say.

**The fix:** Add a "new day" state resolution to Section 2A:

```
New day resolution (first launch where date > lastBrewEndTime's date):
1. If caffeine card is still showing: transition to CLEARED state, then auto-dismiss after 30s
2. Recovery card: daily reset per existing spec (chips cleared, slider hidden)
3. START BREW: re-enabled once recovery is logged (normal gate logic)
4. lastBrewEndTime from yesterday does NOT block today's brew
5. Why Today: new random selection from appropriate tier
```

The caffeine countdown from yesterday is informational — it tells you "you're clear." It is not a gate on today's brew. The only gate is today's recovery input.

**Why this matters:** Without this rule, a dev might reasonably implement "if caffeine card is active, hide START BREW" — which would mean a 3 PM brewer can't start their next-day brew until 6 AM the following morning. That's a broken daily loop for anyone who brews in the afternoon. The rule makes it explicit: yesterday's caffeine state is read-only context, not a gate.

---

## Summary

| # | Fix | Severity | Type |
|---|---|---|---|
| 1 | Explicit WHOOP/Oura recovery mapping function | Non-negotiable | Architecture |
| 2 | Immutable brew log on recovery update | Non-negotiable | Data integrity |
| 3 | Midnight state resolution for caffeine + new brew | Non-negotiable | Daily loop |

These three are all edge cases that won't surface in a demo but will surface in week 2 of daily use — exactly when you need the app to feel trustworthy. A WHOOP user who tracks their own data daily will notice inconsistencies that a casual user won't. Build for the WHOOP user. The casual user benefits from the rigor without knowing it's there.

Everything else in this spec is shippable. The architecture is right. The copy is right. The animations are right. The data model covers the happy path completely. These three fixes cover the edges.

**Verdict: Approve with three fixes. Then ship.**

---

*Round 3 review written March 20, 2026. Three non-negotiable fixes — all edge-case architecture, not design. Spec is otherwise engineering-ready.*
