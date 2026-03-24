# Round 3 Sign-Off — Elon (COO)
**Re:** Picasso's Final Design Spec v3.0
**Date:** March 19, 2026
**For:** Jensen (Engineering) · Jackson (Product) · Picasso (Design)
**Status:** APPROVED TO BUILD.

---

## 1. Verdict: Ship It.

This is ready. Every structural issue from Round 1 is solved. Every content gap from Round 2 is closed. The spec is the most complete first-version build doc I've seen for a hardware companion app — and I'm saying that knowing what's *not* in it, which matters just as much.

The three things I demanded in Round 2:

| Issue | Status | Notes |
|---|---|---|
| Double-rating ask | **Fixed.** Single `BrewLog.rating` field. Completion overlay is primary. Dashboard Card 3 is fallback only if skipped. States are aware of each other. Clean. | — |
| Manual-user experience gap | **Fixed.** "Connect a wearable ›" removed from Card 1. Manual suffix system (trend + streak, alternating) gives manual users their own contextual depth. "Manual check-in logged." validates the input instead of apologizing for it. These users are seen now. | — |
| Missing post-brew reflection copy | **Fixed.** All 15 tier × profile combinations written. No fallback logic needed — Jensen maps directly. The edge cases sound right: "Low signal, full send. Bold. Rate this one honest." — that's Brezi telling you it noticed you overrode, without lecturing. | — |

The additional issues I flagged are also resolved: firmware update failure state exists, brew cancel has a two-step inline confirmation with hardware edge case handling, OAuth has three states (cancelled/failed/syncing), interruptible animation on Screen 4, and the "BREW ANYWAY" button is outside the Rest Day card where it belongs.

**Fix rate across two rounds: 100%.** Picasso didn't patch — she redesigned where it mattered and held scope where it didn't.

---

## 2. Micro-Fixes for Jensen During Implementation

These are too small for another design round but worth getting right in code:

1. **Toast dismiss timing on Screen 2 skip.** Spec says 2.5s auto-dismiss. On slower devices or if the push animation to Screen 3 takes 0.3s, the toast might flash and vanish before registering. Jensen: if the toast is still visible when Screen 3 appears, let it finish its 2.5s on Screen 3 rather than killing it on navigation. Cross-screen toast persistence.

2. **Star rating tap target overlap.** At 36pt stars with 8pt gap on the completion overlay, adjacent star tap targets are 44pt each but overlap by 8pt in the gap zone. Jensen: resolve conflicts toward the higher star (right wins). Users reaching to rate 4 sometimes land between 3 and 4 — giving them 4 is the charitable read.

3. **Recovery slider haptics.** The spec doesn't mention haptics. Add a light impact (`UIImpactFeedbackGenerator(style: .light)`) on each integer step of the recovery slider and on Quick Set chip taps. One line of code, makes the manual input feel physical.

4. **Caffeine cutoff edge case at exactly the hour.** `shiftedProfile` should fire when `currentHour >= caffeineCutoffHour`, not `>`. A user whose cutoff is 2:00 PM opening the app at 2:00 PM should see the shifted recommendation, not the original. Picasso's spec implies this but doesn't state it.

5. **BrewLog.brewedAt vs createdAt.** Both fields exist. `brewedAt` is the actual brew timestamp (set when brew completes or is logged). `createdAt` is the record creation time. For the "Brewed at 7:14am" display, always use `brewedAt`. For sort order in History, use `brewedAt`. `createdAt` is for sync conflict resolution only.

6. **Streak calculation over timezone changes.** If a user travels across timezones, "consecutive days" should use calendar days in the user's *current* timezone, not UTC. A brew at 11pm EST and a brew at 7am PST the next morning are two different days. Don't break someone's streak because they flew to California.

---

## 3. What to Watch in the Build — UX Risks

### Risk 1: The insight strip will feel repetitive faster than we think.

Three variants per tier means a daily user in a stable recovery range sees the same line every 3 days. For TestFlight this is fine — our testers know it's beta. For production, we need 7–10 variants per tier within the first month. Jensen: build the rotation system exactly as specced (UserDefaults index, modulo), but structure `insightVariants` as a plist or JSON file that can be updated without a code deploy. When we write more variants, it's a config push, not an App Store submission.

### Risk 2: The BLE disconnect during brew is the highest-stakes moment in the app.

The user has coffee in the machine. The app loses connection. The copy says "Your brew may still be running on the machine." That's honest, but it's also anxiety-inducing. Jensen: the "RECONNECT" button needs to feel instant — show the spinner within 100ms of tap, not after the BLE scan starts. And if reconnection succeeds and the brew already finished on the machine side, skip the completion overlay and go straight to post-brew with a toast: "Brew finished while disconnected." Don't make the user re-experience a brew they already drank.

### Risk 3: Manual Override Sheet slider precision on small screens.

The bounded sliders on `ManualOverrideSheet` have tight ranges (e.g., Tide Pool temp: 2–6°C, 4 integer steps over a slider width of ~280pt). That's 70pt per step — fine on iPhone 15 Pro Max, tight on iPhone SE. Jensen: if the device screen width is < 375pt, switch to a stepper (– / value / +) instead of a slider for ranges with fewer than 6 steps. Preserve the slider for wider ranges (The Blank's 1–25°C).

### Risk 4: The first-open-to-first-brew time is the number we're optimizing.

Everything in onboarding exists to minimize the gap between "I just unboxed this" and "I'm drinking cold brew that knows my HRV." If BLE pairing takes 30 seconds and OAuth takes 45 seconds, we're at 2+ minutes before the dashboard. That's acceptable. But if the BLE search times out at 30 seconds and then the user has to troubleshoot, we're at 3–4 minutes in a frustrated state. Jensen: consider dropping the BLE timeout to 20 seconds and making the troubleshooting checklist appear inline *during* the scan (after 10 seconds), not after timeout. Let the user self-diagnose while the scan is still running.

### Risk 5: The "Reading your signal..." transition can feel like fake loading.

It's 0.8s max. But if the recommendation calculation takes 50ms (it will — it's a lookup table), the user sees a spinner that exists purely for theater. Some users clock this. Jensen: set a *minimum* of 0.5s (so it doesn't flash) and a maximum of 0.8s, but if the calculation finishes in under 0.5s, hold the animation until 0.5s. The alternative — no transition at all — feels too abrupt after 5 onboarding screens. The pause is earned. Just don't make it longer than it needs to be.

---

## 4. v1 vs v2 — One Sentence

**v1 is a personal cold brew practice that reads your body and builds your morning; v2 is when the practice learns your preferences, expands to the Blender, and the dashboard handles two recommendations.**

---

*Three rounds. Zero open issues. The chain is the product, the voice is found, the manual user is respected, and every failure state has a recovery path. Picasso delivered. Jensen — build it exactly as specced. 14 days to TestFlight. Let's go.*

— Elon, COO
