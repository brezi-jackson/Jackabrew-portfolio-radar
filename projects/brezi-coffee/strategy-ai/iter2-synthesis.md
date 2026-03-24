# BREZI — ITERATION 2 SYNTHESIS
## Cross-Critique Summary — PM (Yongping)
## Date: 2026-03-21

---

## KILLS — Things Eliminated After Cross-Critique

1. **"Closed-Loop Biometric Nutrition"** — Bobo killed it. Systems engineering jargon, no human says this. Replace with consumer language.
2. **"Biological Obedience"** — Bobo killed it. Dystopian connotation, submission language, never leaves internal docs.
3. **"We don't make coffee. We make *your* coffee."** — Bobo killed it. Hallmark-level, same as every personalization brand. Replace with something with actual weight.
4. **"Brezi is what happens when your wearable data finally does something."** — Bobo flagged. "Finally" is Kickstarter language. "Does something" is anti-language.
5. **Dashboard showing biometric numbers as first screen** — Both CMO and Bobo killed it. Anxiety delivery, forces interpretation, contradicts "you don't have to think about this."
6. **3-slider outcome check-in** — Both CMO and Bobo killed it. Protocol Delegator doesn't fill out surveys. Infer from behavior instead.
7. **LLM showing reasoning ("Your HRV was 12% below baseline")** — Bobo killed it. Breaks the "knowing" magic. Never show the math.
8. **Confidence score shown to user ("Try this?" at <0.4)** — Bobo killed it. Showing uncertainty destroys trust. Either recommend or don't.
9. **3AM fixed pre-compute job** — CMO flagged. Fails for 4:30AM wakers, night shifts, jet lag. Must be dynamic based on sleep pattern data.
10. **"Different day?" override with 3 options (Moving/Still/Open)** — CMO killed it. Three decisions before coffee = same fatigue we promised to eliminate. Replace with ONE tap: "Not today."
11. **WHOOP live demo without pre-staging** — CTO flagged as unreliable. 45-90 second minimum latency on clean connection, doubles on public WiFi. Must pre-stage with controlled accounts.
12. **"You're calibrated" as good-day message** — CMO killed it. Sounds like a printer test page. Replace with positive valence: "Clear day. Nothing to override." or similar.
13. **0.3Hz heartbeat pulse during "thinking"** — CTO flagged as hardware/sync problem. Device has no internet during overnight processing — can't know it's "thinking." Defer to v2.
14. **NFC haptic actuator in bay mechanism** — CTO flagged as hardware revision, not in v1 spec. Substitute with phone-side haptic on NFC read event.
15. **Scent activation "guaranteed at 90 seconds"** — CTO flagged. Formulation-dependent, varies by altitude/humidity. Qualify as "approximately 90 seconds under standard conditions."

---

## KEEPERS — Validated and Strengthened

1. **"Not a smart device, a knowing one."** — CMO and CTO both love this. Best single line produced. Goes everywhere.
2. **Overnight pre-compute concept** — Right idea, wrong implementation. Fix: dynamic schedule + local cache + pre-warm serverless. Client-side cache for instant-on.
3. **The 90-second live demo format** — High-risk, high-reward, right for the product. BUT requires pre-staged accounts.
4. **Patent strategy (2 filings)** — Elevate from IP protection to category ownership narrative in deck.
5. **Phase 1 Jensen solo constraint** — Inadvertently protects brand purity. Fewer features = cleaner core loop.
6. **Protocol names (Tide Pool, Voltage, First Light)** — Right emotional register. Bobo flagged "Deep Work" as inconsistent; remove or rename.
7. **Federated outcome reporting for Protocol Seed Program** — CTO design: practitioners see cohort aggregate dashboard, share rendered summary cards, no individual health data exposed.
8. **Cultural market differentiation** — HK (slightly more AI-visible), SG (Brezi Certified quality mark), Tokyo (craft + ritual, AI implied never stated). BUT must share one brand foundation ("mastery") not split identities.

---

## CRITICAL GAPS — Neither CMO Nor CTO Nor Bobo Addressed

1. **The explanation problem** — How does a user explain Brezi to a friend? No word-of-mouth engineering in any document. Must design the "one sentence lay explanation" into the product experience.
2. **The churn architecture** — What happens when Brezi gets it wrong? No recovery UX designed. No brand response to a missed recommendation. This is a retention problem.
3. **The gifting economy** — $400+ device = perfect gift. No onboarding designed for gifted users with no prior biometric data. Free acquisition channel ignored.
4. **Android** — CTO built iOS only (Jensen). What's the explicit Android strategy/timeline? WHOOP Android users are a significant segment.
5. **Connectivity failure state** — What does the morning experience look like with no WiFi/data? No offline mode designed.
6. **The "wearable-free" user** — What happens with new users who don't own Oura/WHOOP yet? No onboarding path for manual-only mode defined.

---

## KEY DECISIONS NEEDED FROM JACKSON

1. **Category name**: "Closed-Loop Biometric Nutrition" is dead. What replaces it? Task force to propose 3 options in Iter 3.
2. **Dashboard first screen**: Conclusion-first design (protocol name only) vs. data-available design. Task force consensus = conclusion-first. Confirm.
3. **Override flow**: "Not today" single tap vs. any more options. Task force consensus = one tap. Confirm.
4. **LLM rationale**: Never show to user. Task force consensus = confirmed. But show in investor materials.
5. **Demo strategy**: Pre-staged accounts for pop-up. Accept this?

---

## FOR ITER 3: INTEGRATION

With all the kills and keepers above, Iter 3 must produce:
- Revised unified positioning statement (CMO)
- Revised MVP screen specs (CTO, no dashboard data first, no sliders, conclusion-first)
- Revised experience blueprint (Bobo, incorporating hardware constraints)
- Revised brand vocabulary (new category name, new killer demo line)
- Resolution of all 6 critical gaps above
