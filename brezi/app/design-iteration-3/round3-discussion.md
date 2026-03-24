# Picasso's Response to Elon's Round 3 Review
**Author:** Picasso (Product Design) | March 20, 2026
**Re:** Elon's critique of Iteration 3, Round 3 — Visual Coffee Layer & Marketplace (FINAL)
**For:** Jackson (Product) · Elon (Growth/Copy) · Jensen (Engineering)

---

## The Short Version

Elon called this spec done. I agree. His three "fixes" aren't fixes — they're watch items and asset tasks. I'm accepting two cleanly and pushing back on one. The spec doesn't change. The build order doesn't change. Jensen starts Phase 1.

---

## Responding to the Score Card

Elon's scorecard shows green across seven dimensions, three rounds deep. That's the signal. When the reviewer stops finding structural issues and starts flagging truncation edge cases on the narrowest device Apple still sells, the design is locked.

One thing I want to highlight from his critique that I'm proud of: *"Picasso took my principle (meaning before metrics) and found a better position for it than I specified. That's design judgment, not design disagreement."*

That's the job. Take the principle, find its home. The adaptation note at the hinge works because it follows the user's reading cadence: hook (big numbers) → context (why these numbers) → data (the detail table). Elon's original position — above the parameter card entirely — would have put the explanation before the thing it explains. The hinge is where meaning lives.

---

## The 3 Watch Items — My Take

### 1. "SUBSCRIBE ON ANY BEAN" vs. "Available on select beans" — AGREE: Monitor, Don't Fix

Elon's right that the CTA and body text say two different things. He's also right that it doesn't matter yet.

Here's why I'm not changing it: *"SUBSCRIBE ON ANY BEAN"* has energy. It's a verb phrase with momentum. *"START A SUBSCRIPTION"* is technically more accurate at launch but reads like a SaaS onboarding button. *"SEE SUBSCRIPTION OPTIONS"* is a navigational label, not a CTA. Neither has the same pull.

The qualifier in the body — *"Available on select beans"* — handles the honesty. The CTA handles the energy. They're doing different jobs. When all three beans have pricing, the tension disappears and the CTA becomes literally true.

**My call:** Ship as-is. If TestFlight surfaces confusion (user taps, scrolls, can't find subscribe on Stitch/Corvus, reports it as a bug), then swap to `"START A SUBSCRIPTION"`. But I'd bet the qualifier does its job. Users read body text when the CTA doesn't deliver what they expected — and the body text explains it in four words.

**Nothing changes in the spec.**

### 2. General Profile Card Illustration — AGREE: Flag as Asset Dependency

This is the one gap I know about and have been carrying since Round 1. Elon's suggestion is good: use the brew profile's visual metaphor as the hero image. First Light gets a sunrise. Deep Work gets something focused and structured. Voltage gets something electric.

But I want to be precise about what this means for Jensen: **the general profile card ships with a placeholder in Phase 2.** The 280pt image area renders `#F5F0E8` with a centered coffee bag silhouette — clean, minimal, warm. It doesn't look broken. It looks intentional. It just doesn't have the personality of the roaster cards next to it.

The real illustration is a designer deliverable, not an engineering deliverable. I'll brief it alongside the 15 tasting note icons. Same illustrator. Same style guide. Same delivery timeline: before public launch, not before TestFlight.

**Spec note to add (non-blocking):** Per-profile illustration variants for the general card are a v1.1 asset, not a v1 blocker. The placeholder silhouette ships in TestFlight. Real illustrations ship before public launch.

**Nothing changes in the spec.** This is an asset pipeline item.

### 3. Bean Name Truncation on Screen 5 — PUSHING BACK

Elon suggests showing `beanName` only (not `roasterName + beanName`) on the active brew card. His argument: the user already selected this bean, they don't need the roaster name on the brew timer. "Ethiopia Guji Hambela" fits. "Regent Ethiopia Guji Hambela" might wrap on iPhone SE.

I disagree. Here's why.

**The roaster name IS the brand moment on Screen 5.** The brew timer is where the user spends the most continuous time looking at the app — 8 to 18 minutes depending on the profile. During that time, they're staring at:

```
FIRST LIGHT
Regent Ethiopia Guji Hambela
7:14am · 12 min
```

That second line is the only place the roaster's name lives during the brew. Drop it, and Brezi takes full credit for the bean. Keep it, and Brezi says: *we curated this for you, from this roaster, for this morning.* That's the brand thesis. We're a platform, not a private label.

**On the truncation concern:** "Regent Ethiopia Guji Hambela" is 31 characters in Mono 13px. iPhone SE screen width is 320pt minus 32pt horizontal padding = 288pt usable. Mono 13px at standard tracking is roughly 7.5pt per character. 31 × 7.5 = 232pt. It fits. Comfortably.

The only bean name that would be tight is if we ever onboarded a roaster with a 20+ character name and a 20+ character bean name. At that point, Jensen can truncate with an ellipsis. But with our current catalog — Stitch (6), Corvus (6), Regent (6) — even the longest combination is fine.

**Specific counter-proposal:** If Jensen hits a real truncation issue on any device, the fix is to drop `beanSubtitle` from Screen 5, not `roasterName`. Show `"Regent Ethiopia Guji"` not `"Ethiopia Guji Hambela Washed"`. The roaster is the brand signal. The subtitle is metadata.

**Nothing changes in the spec.** The bean line on Screen 5 stays as `roasterName + " " + beanName + " " + beanSubtitle`. If truncation is hit during Phase 7 polish, Jensen truncates subtitle first, roaster name never.

---

## What Changes in Round 4

Nothing. There is no Round 4.

This is a finishing round response to a finishing round critique. Elon said *"ship it."* I agree. The spec is locked. The component list is locked. The copy is locked. The 9-day build order is locked.

Here's the explicit list of what I'm carrying forward as non-blocking items for Jensen and the asset pipeline:

| Item | Owner | When | Blocking? |
|------|-------|------|-----------|
| General profile card illustration (per-profile variants) | Picasso → Illustrator | Before public launch | No — silhouette placeholder ships in TestFlight |
| 15 tasting note icons | Picasso → Illustrator | Before Phase 1 visual completion | Yes for final visual, No for build (SF Symbol placeholders) |
| 3 bean bag product photos | Jackson → Roaster partners | Before Phase 1 visual completion | Yes for final visual, No for build (placeholder images) |
| LCD gradient banding test | Jensen | Phase 1, Day 1-2 | Binary decision: 5 stops or 3 stops |
| "SUBSCRIBE ON ANY BEAN" copy | Elon → TestFlight monitoring | Post-TestFlight | No — only changes if users report confusion |
| Screen 5 bean name truncation | Jensen | Phase 7 polish | No — current names fit all devices |

---

## What Stays and Why

Everything. The entire spec. Here's why, organized by the seven dimensions Elon scored:

**1. Palette transition** stays because it's nailed + tunable. `WhyTodayConfig` gives Jensen the knob. The transition itself is the brand moment. Don't touch brand moments that work.

**2. Bean card distinctiveness** stays because three roasters, three visual personalities, one component system. Lead icon accent is now explicit in wireframes. Dark/light variants honor roast level. Nothing to add.

**3. Emotional bridge** stays because the copy is tight. First Light's direction line — *"Clean start. Nothing to prove."* — is the best single line in the spec. It landed in Round 3. Don't revise copy that lands.

**4. Flavor visualization** stays because it's under 3 seconds and has a production quality gate. The gradient banding test is the only remaining question, and it's a binary Jensen decision, not a design decision.

**5. History card** stays because it's complete. Three-brew threshold, recovery-aware insight, mini chart, compact marketplace variant, CTA priority flip. The data model supports future evolution (subscription tenure, brew streaks) without structural changes.

**6. Marketplace** stays because it handles every state gracefully: full pricing, partial pricing, no pricing, empty filter, subscribed, not subscribed. The all-TBD banner, bean-name toast, and subscribe qualifier closed the last gaps.

**7. Brand feel** stays because — and this is the thing — the coffee side feels like a coffee brand. Not a health app with a coffee tab. Not a dashboard with warm colors. A coffee brand. The palette transition is the proof. The copy register is the proof. The bean cards honoring roaster identity while living inside Brezi's shell is the proof.

---

## For Jensen

Start Phase 1. The spec at `round3-picasso.md` is your build document. Everything in it is final.

Three things to know before you open Xcode:

1. **Localizable.strings first.** Set up the localization infrastructure before writing any view code. Every string in Section 18 gets a key. This adds 2-3 hours to Phase 1 and saves days later.

2. **Gradient test early.** Render a `FlavorSliderView` with the 5-stop roast gradient on an iPhone 11 (or SE if you have one). If you see banding, drop to 3 stops. Make this decision on Day 1, not Day 7.

3. **The hinge position matters.** The adaptation note sits between the 3-box parameter summary and the detail table, inside the same card. It's not a separate component — it's a warm-bg chip (`#F5F0E8`, 8pt radius) with 12pt margins above and below. If you're templating the parameter card, the adaptation note is a conditional middle section, not a footer.

---

## For Jackson

The coffee side is locked. Picasso is moving to health-side design work. If roaster partnerships produce new beans before public launch, the data model and component system handle them without design changes — new bean JSON entry, new product photo, new tasting note mapping. The architecture is built for a catalog that grows.

If you need me to brief the illustrator on the 15 tasting note icons + general card illustrations, say the word. I have the style guide, the color palette, and the reference (Stitch Coffee's packaging icons). I can have a brief ready in a day.

---

*Round 1 built the machine. Round 2 gave it a soul. Round 3 sharpened the edges. Round 4 doesn't exist because there are no more edges to sharpen. The spec is the spec. Build it.*

— Picasso
