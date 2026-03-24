# BREZI — ITERATION 4 STRESS TEST
## Risk Registry & Mitigation Playbook — PM Synthesis
## Date: 2026-03-21

---

## THREAT TIER 1 — EXISTENTIAL (Must address before launch)

### T1-A: WHOOP / Oura Vertical Integration [Survivability: 4/10]
**The attack:** Oura acquires cold brew hardware startup, launches "Oura Morning" at $279 (subsidized by ring revenue). WHOOP follows. Both have direct push to millions of existing ring users. No API friction.
**What makes it fatal:** Brezi is building on rented land. WHOOP can revoke API access the day they announce a competing product.
**Mitigation — must do NOW:**
1. Negotiate long-term data partnership agreements with Oura + WHOOP before launching (not after)
2. Build wearable abstraction layer so any wearable can be swapped in — no single-source dependency
3. Prioritize Garmin + Samsung Health integration earlier (diversify away from Oura/WHOOP duopoly)
4. Frame Brezi as acquisition target for Oura/WHOOP, not a competitor — "we're the actuation layer you can't build"

### T1-B: Jensen Is a Single Point of Failure [Critical]
**The attack:** Jensen gets sick, leaves, or burns out. No documentation. No second brain. 6 weeks for any new engineer to understand the codebase.
**What makes it fatal:** Architectural decisions made in Month 2 take 6 months to undo. Technical debt baked in by solo developer is the company's invisible liability.
**Mitigation — must do NOW:**
1. Second engineer hire by Month 2, not Month 3
2. Mandatory architecture documentation as a deliverable alongside code (not optional)
3. All key design decisions logged in a technical decision record (TDR) document
4. Weekly architecture reviews — even if just Jensen + Jackson

### T1-C: "While you slept, it decided" — Creepiness Threshold [Brand]
**The attack:** The most powerful demo line is also the most dangerous. In Tokyo, it reads as overreach. In a privacy news cycle, it reads as confession. The brand vocabulary (knowing/listening/reading) evades the language of data collection — but the legal vocabulary can't evade it. When a privacy incident hits, the gap between poetic evasion and legal transparency destroys the brand retroactively.
**What makes it fatal:** The brand promise and a privacy scandal are not adjacent problems — they are the SAME problem. "Not a smart device, a knowing one" reframes to "We called it 'knowing' so we could avoid disclosing how it knows."
**Mitigation — must do NOW:**
1. Build a transparent privacy dashboard into the app: "Here's what Brezi knows about you" — simple, honest, exportable
2. Never share behavioral/engagement data with third-party marketing partners. Ever. Hard policy, not guidelines.
3. For Tokyo launch: replace "While you slept, it decided" with "While you slept, it listened." One word change. Huge connotation difference.
4. Consent architecture must be explicit, not buried in ToS

---

## THREAT TIER 2 — SERIOUS (Requires mitigation plan before Series A)

### T2-A: Apple "Morning Intelligence" at WWDC [Survivability: 5/10]
**The attack:** Apple ships a free iOS feature that reads Apple Health and suggests morning routines for HomeKit devices. Brezi's data moat claim evaporates — Apple has 1.5B devices.
**Reality check:** The moat was never the data. It's the closed loop (hardware actuation) + subculture ownership (Protocol Delegator tribe doesn't want Apple's mass-market solution) + 12-18 month head start.
**Mitigation:**
1. Sprint toward NFC cartridge ecosystem lock-in — proprietary formulations that work ONLY in Brezi
2. Community depth > feature parity. Own the Protocol Delegator identity before Apple normalizes the category.
3. If Apple ships this: "Apple just validated our category. They're building for 1.5B people. We're building for 50K who take it seriously."

### T2-B: The AI Fatigue / "Exposed Wizard" Video [Survivability: 6/10]
**The attack:** Well-followed biohacker publishes "Brezi is just IF/THEN dressed as AI" — 2M views, your early adopters share it.
**Reality check:** This video will happen. The question is when and how prepared you are.
**Response strategy:**
- DO NOT get defensive. "Our AI is real" invites more scrutiny.
- Radical transparency + reframe: "Here's exactly how v1 works. Here's where it goes in 12 months. We're building in public."
- Pull back brand language at launch: "Informed by your biology" is more defensible than "it decided." Save AGI-adjacent language for when the model is actually trained.
- Publish methodology. Let the community audit. Protocol Optimizers respect transparency, hate being patronized.

### T2-C: LLM Cost Spiral [Technical]
**The math:** At 10K users, 300K API calls/month. If Claude pricing increases 10x → $60K/month in LLM costs vs. ~$45K MRR at 30% attach. Company is insolvent on AI infrastructure costs alone.
**Mitigation:**
1. Multi-provider LLM strategy from Day 1 — abstract the AI layer so you can switch to cheaper models (Llama, Mistral, local inference) without rewriting everything
2. Aggressive prompt optimization — reduce token count as user base grows
3. Budget LLM costs explicitly in unit economics before Series A

### T2-D: Travel Break = Habit Break = Churn [UX]
**The attack:** Brezi's "Your morning is ready" notification during travel becomes a ghost ping. After 7 days of missed brews, re-engagement friction is high. Notification gets turned off.
**Mitigation — must build:**
1. Travel mode: "Pause for X days" with one tap. Product goes quiet. Resumes automatically.
2. Travel protocol: a simplified "anywhere" protocol using standard coffee + the app in recommendation-only mode (no device required)
3. Return welcome: "You're back. Let's pick up where we left off." — with a recalibration acknowledgment

### T2-E: Brew Triplet Data Quality Problem [Technical/Strategic]
**The attack:** Self-reported outcomes are unreliable. Next-day wearable delta has 48-hour lag with confounders. The model trained on this data may just be an expensive placebo detector.
**Mitigation:**
1. Reframe the data moat claim: moat is NOT "model trained on proprietary data." Moat is "longitudinal individual user history that personalizes to you specifically over time" — this can't be open-sourced because it belongs to each user.
2. Partner with HKUST/HKU for controlled-condition validation studies — clean academic data alongside consumer data
3. Be honest in investor materials: "The model improves with scale. The individual personalization layer is valuable from Day 1."

### T2-F: "Tomorrow is Different" Becomes a Punchline [Brand]
**The attack:** Three wrong recommendations in two weeks. Each time: "I'm already adjusting. Tomorrow is different." By the third time this is a broken record.
**Mitigation — must build:**
1. Recovery copy must NEVER repeat verbatim after second use. Each miss generates a different response.
2. After 2 consecutive misses: escalate to a different intervention. "Let's recalibrate. Tell me one thing about your last few days." — ONE question, no survey.
3. After 3 consecutive misses: offer to reset the model. "Your patterns seem to have shifted. Let's start fresh."

---

## THREAT TIER 3 — MANAGEABLE (Monitor and prepare)

### T3-A: Protocol Name Translation Problem
"Tide Pool" in Cantonese (潮池) loses resonance. "Voltage" in Mandarin (电压) is too industrial.
**Mitigation:** Chinese market protocol names should be localized, not translated. Keep English names for English-first markets. Commission proper Cantonese/Mandarin naming for HK/TW/China launches. This is a $5K localization budget item, not a strategic threat.

### T3-B: NFC Failure Rate (5-8% in consumer products)
50+ users/day unable to brew without retry at 1,000 users = unsustainable support volume.
**Mitigation:** Build graceful "tap again" UX with clear visual feedback. Track NFC failure rate as a key hardware metric. Above 3% → escalate to hardware team immediately.

### T3-C: Multi-User Household Data Corruption
Partner uses device without NFC auth → corrupts User A's recommendation baseline.
**Mitigation:** Build household/multi-profile mode into Phase 2 roadmap. NFC tap authentication is the mechanism — enforce it, don't make it optional.

### T3-D: $399 Price Justification Against $249 Fast-Follower
**Mitigation:** The first 30-day experience creates switching cost the fast-follower can't offer. Speed to market + Protocol Pack ecosystem = price premium is justified after trial. Lead generation strategy must create trial before fast-follower exists.

### T3-E: Market Size (SAM ~12K-30K Year 1)
**Reframe for VCs:** "Brezi is not a cold brew machine company. The cold brew machine is the beachhead for a biometric-triggered functional nutrition OS. SAM at cold brew: $40M. SAM for biometric-triggered functional nutrition delivery: $4.2B."

---

## NON-NEGOTIABLE BRAND GUARDRAILS (before first investor takes a board seat)

These must be written into company documents BEFORE Term Sheet Day:

1. **No social features without explicit user-initiated sharing.** Default = private. Always.
2. **No streak mechanics.** Streaks punish travel and disruption — exactly when Brezi should be most helpful.
3. **No leaderboards. Ever.** Protocol Delegator is competing with their own baseline, not others.
4. **One notification per day, maximum.** The knowing device doesn't nag.
5. **The brew button stays on screen one.** Every additional tap = broken brand promise.
6. **Never share behavioral data with third-party marketing partners.**

---

## THE SINGLE NON-SURVIVABLE SCENARIO (Bobo's finding)

**The headline:** "Brezi's 'Waking Intelligence' App Found Sharing Sleep Pattern Data with Third-Party Marketing Partners"

This scenario is non-survivable because the brand vocabulary (knowing/listening/reading) was deliberately constructed to avoid the language of data collection. In a privacy incident, that gap reads as deception, not elegance. "Not a smart device, a knowing one" retroactively becomes: "We called it 'knowing' to avoid disclosing how it knows."

**Structural prevention:** Hard policy, audited annually — no behavioral or biometric data sold or shared with third parties. Period. Not guidelines. Policy. With teeth.

---

## UNIT ECONOMICS DEATH SPIRAL — LEADING INDICATOR

The scenario that kills Brezi: 20% subscription attach + 25% hardware GM + $0.05/call LLM costs + 3 market-rate hires.

**The leading indicator to watch:** **Month 3 subscription attach rate.**

If Month 3 attach is below 30%, the entire model breaks. At 20% attach with $25/month:
- 10K units × 20% × $25 = $50K MRR
- vs. $60K/month LLM costs (at scale) + $240K/year team = company is insolvent

Month 3 attach rate below 30% is the fire alarm. If you see it, pivot pricing or cut LLM costs before Month 6.

---

## WHAT THE STRESS TEST CONFIRMED IS REAL

Despite all attacks, these moats survive scrutiny:
1. **The closed loop** — no competitor (including Apple) can replicate the hardware → AI → physical actuation → outcome capture loop without building both hardware and software
2. **Protocol Pack ecosystem** — NFC-locked certified consumables create switching cost the data alone cannot
3. **Brand equity with Protocol Delegator tribe** — first mover with a specific subculture. Apple's mass market solution is exactly why they won't use Apple's solution.
4. **The team** — semiconductor background + sports/wellness practitioners = unfakeable domain expertise
