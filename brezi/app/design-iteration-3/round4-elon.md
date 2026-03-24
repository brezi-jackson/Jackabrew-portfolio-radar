# Elon's Round 4 Critique — Locked Brief vs. Shipped Spec
**Reviewer:** Elon (COO / Product & Growth) | March 20, 2026
**Reviewing:** Picasso's Round 3 spec (FINAL) against Jackson's locked product brief
**For:** Jackson (Product) · Picasso (Design) · Jensen (Engineering)

---

## Context

Picasso said there is no Round 4. The spec is locked. Jensen should start Phase 1. I agreed with that in Round 3.

Jackson then dropped a locked brief with every decision finalized — three confirmed beans, flow order, new features, visual language, packaging aesthetics. No more questions. This is the product bible.

So this critique isn't "what's wrong with the design." It's "does the locked spec cover the locked brief?" I'm auditing alignment. Where the spec and brief match, I'll confirm. Where they diverge, I'll flag it as a gap Jensen needs to close before build, not a design revision.

---

## 1. Palette Transition Hero Moment — Nailed or Still Weak?

**Verdict: Nailed. The spec and the brief are saying the same thing in different words.**

Jackson's brief calls the palette transition a "BRAND MOMENT" and describes it as *"the core brand story compressed into a gesture: 'This is your body. This is what it deserves.'"* He wants this designed as a "hero moment — not just navigation."

Picasso's spec already does this. The Why Today screen IS the hero moment. The `#0F0F0F` → `#FAF6F0` crossfade happens around the typewriter text, not on a navigation push. The user reads about their body while the world warms around them. The config struct makes it tunable. The skip link respects returning users. The animation plays once per day.

But here's the gap Jackson's brief surfaces: he frames this as a *"transition screen"* — a distinct, designable moment between the health world and the coffee world. Picasso's spec implements it as the *opening state* of Why Today. These are functionally identical, but the brief's language suggests Jackson might expect a dedicated visual treatment — a half-second pause, a gradient wash, maybe a Brezi wordmark moment — before the typewriter starts. Something the user *sees* as a threshold, not just *experiences* as a text render.

**My read:** The current implementation is correct. The typewriter IS the threshold. Adding a separate transition screen before it would add friction without adding meaning. The brief's language is aspirational, not prescriptive. But if Jackson reads the spec and says *"I expected a visual beat before the text starts,"* the fix is a 0.6s branded gradient wash (dark → warm) with the Brezi wordmark at 20% opacity, fading as the typewriter begins. That's a `WhyTodayConfig` addition, not a structural change. Don't build it unless Jackson asks for it.

**Gap: None. Monitor.**

---

## 2. Bean Cards — Stitch/Regent/Corvus Visually Distinct and Brand-Honoring?

**Verdict: Yes. The spec's bean data matches the brief exactly. One small correction needed.**

I checked every field. Here's the alignment:

| Field | Brief | Spec | Match? |
|-------|-------|------|--------|
| Stitch name | Dark Field | Dark Field | Yes |
| Stitch roast | Dark | .dark | Yes |
| Stitch notes | Dark chocolate, toasted almond, caramel | Dark Chocolate, Toasted Almond, Caramel | Yes |
| Stitch flavor | 90/55/20 | 0.9/0.55/0.2 | Yes |
| Stitch best for | Deep Work / Voltage | deep_work, voltage | Yes |
| Stitch packaging | Bear in red Fiat 500 | "bear in red Fiat 500" | Yes |
| Regent name | Ethiopia Guji Hambela Washed | Ethiopia Guji / Hambela Washed | Yes |
| Regent roast | Light | .light | Yes |
| Regent notes | Cocoa, jasmine, strawberry, lime | Cocoa, Jasmine, Strawberry, Lime | Yes |
| Regent flavor | 15/80/75 | 0.15/0.8/0.75 | Yes |
| Regent price | $27.00 | 27.00 | Yes |
| Regent Shopify | Product ID 10151173947683 | 10151173947683 | Yes |
| Corvus name | Sadayana Winey | Sadayana Winey | Yes |
| Corvus subtitle | Winey Anaerobic Natural Ateng | Winey Anaerobic Natural Ateng | Yes |
| Corvus roast | Medium-Light | .medium | **Close** |
| Corvus notes | Raspberry yogurt, cherry syrup, dark chocolate | Raspberry Yogurt, Cherry Syrup, Dark Chocolate | Yes |
| Corvus flavor | 45/85/50 | 0.45/0.85/0.5 | Yes |
| Corvus best for | Deep Work / Voltage | deep_work, voltage | Yes |
| Corvus packaging | Gold temple/mandala on black | "gold temple/mandala on black with red accent" | Yes |

**The one discrepancy:** Jackson's brief says Corvus is "Medium-Light" roast. The spec uses `.medium`. The `RoastLevel` enum has three values: `.light`, `.medium`, `.dark`. There is no `.mediumLight`. At 45/100 on the roast scale, the slider position is correct — it reads as medium-light visually. But the adaptation notes and the detail view's "Roast: Medium" text should arguably say "Medium-Light" to match the brief.

**Fix:** Either add `.mediumLight` to the enum (which affects slider label rendering, adaptation note selection, and the detail table) or keep `.medium` and accept that the text label rounds up. I'd keep `.medium` — the slider at 0.45 communicates medium-light visually, and adding a fourth enum value ripples through the adaptation note matrix, the general settings copy, and the recommendation logic. The brief says 45/100 on the flavor profile, which IS medium. The "Medium-Light" label in the brief is a human description, not a system value. The spec's 0.45 roast slider position is the source of truth.

**Gap: Cosmetic. No action needed unless Jackson insists on the "Medium-Light" text label.**

---

## 3. Why Today Emotional Bridge — Landing or Generic?

**Verdict: Landing. The brief's example copy lives in the spec.**

Jackson's brief gives one example: *"You're at 62% today. Rebuilding. Your nervous system is doing the heavy lifting."* This is literally the `first_light` copy from the spec:

```swift
line1: "You're at \(recoveryPercent)% today.",
line2: "Rebuilding. Your nervous system is doing the heavy lifting.",
direction: "Clean start. Nothing to prove."
```

The brief also says Why Today + Bean Selection are the "EMOTIONAL half of the app" with "warm palette, illustrative, personal." The spec delivers on all three: warm palette (`#FAF6F0`), the bean card illustrations are roaster-specific (illustrative), and the copy is second-person singular throughout (personal). "Your nervous system." "Your call today." "You've got the runway."

The copy register across all five profiles is consistent. Each has the same three-part structure: data point, interpretation, coffee direction. The typewriter renders them identically. The pacing is uniform. This is voice, not filler.

**Gap: None.**

---

## 4. Flavor Visualization — Under 3-Second Scan?

**Verdict: Under 3 seconds. The brief's slider format matches the spec's implementation.**

Jackson's brief specifies: *"Flavor sliders per Stitch Coffee style: horizontal bars for Roast / Sweetness / Acidity."* The spec has exactly this — three gradient bars with positioned dots, value labels on full cards, no labels on compact. The gradient banding quality gate from Round 3 is the only open item, and it's a binary Jensen decision on Day 1.

Jackson also specifies *"Tasting note icons: flat illustration style"* and *"Serving infographic: clock arc for cutoff time, cup icons for serving size."* Both are in the spec. The tasting note icons are asset dependencies (SF Symbol placeholders ship first, real illustrations before public launch). The serving infographic is fully specced in Section 9.

**Gap: None.**

---

## 5. History Card — Personal and Warm?

**Verdict: Personal, warm, and exactly what the brief asks for.**

Jackson's brief introduces: *"'Your history with this bean' — after 3+ brews with a bean, show personal brew history card: 'You've brewed this 3x. Best rated on recovery 7+.' Drives repeat purchase naturally."*

The spec has this. `BeanBrewHistoryCard` appears in BeanDetailView when `brewCount >= 3`. The copy template is *"You've brewed this 12x. Best day: recovery 78%."* The CTA priority flips after 3 brews — Subscribe & Save gets promoted above Add to Cart. The mini chart shows the user's brew history with that bean over time.

Jackson's brief example says *"Best rated on recovery 7+."* The spec says *"Best day: recovery 78%."* These are the same insight expressed differently. The spec's version is more specific (78% vs. 7+), which is better — specificity signals real data, not a templated message. The data model supports this: `bestRecoveryDay` is computed from the user's brew log.

The marketplace compact variant also includes the history card (condensed). The history card appears in both BeanDetailView and the marketplace bean page when the threshold is met. Consistent. Correct.

One thing the brief adds that I want to highlight: *"Drives repeat purchase naturally."* This is the strategic intent behind the history card. The spec implements the mechanic (show history, flip CTAs) but doesn't articulate the growth thesis. That's fine — the spec is for Jensen, not for investors. But it's worth noting that the CTA flip is the single most important conversion mechanic in the marketplace. A user who has brewed Regent 5 times and sees *"You've brewed this 5x. Best day: recovery 82%"* right above a promoted *"SUBSCRIBE & SAVE 15%"* button is the highest-intent subscriber Brezi will ever have. The spec builds this. The brief explains why.

**Gap: None.**

---

## 6. Marketplace + Subscribe & Save — Would You Actually Buy Here?

**Verdict: Yes, with one brief-spec tension that needs resolution.**

Jackson's brief says: *"Subscribe & Save = DESIGN IT FULLY — will be live at launch, not coming soon."*

The spec designs Subscribe & Save fully: subscribe banner in the marketplace, per-bean subscribe CTA in detail view, subscribed button state, savings display ($22.95/delivery), CTA priority flip after 3 brews. The design is complete.

But the spec also says: *"Available on select beans"* — because at launch, only Regent has pricing and a Shopify product ID. Stitch and Corvus are `price: nil`. Subscribe & Save can only function on beans that have a Shopify product to subscribe to.

Jackson's brief says Subscribe & Save will be "live at launch." If that means "the feature exists and works on beans that support it" — the spec is correct. If it means "all three beans will have pricing and Shopify integration at launch" — that's a business dependency, not a design gap. The spec handles both states gracefully: priced beans show full subscribe flow, unpriced beans show NOTIFY ME.

**The real question for Jackson:** Will Stitch and Corvus have pricing before launch? If yes, the "Available on select beans" qualifier disappears and the marketplace is fully live. If no, the spec's current handling is correct — subscribe works on Regent, the other two show NOTIFY ME, the all-TBD banner appears when filtering to Deep Work / Voltage only.

**The in-app web view decision is implemented correctly.** `SFSafariViewController` for Add to Cart. User stays in the app. Apple Pay supported. The brief says *"SafariViewController / SFSafariViewController — user stays in app."* Exact match.

**Gap: Business dependency (Stitch/Corvus pricing), not a design gap. Spec handles both states.**

---

## 7. Overall Brand Feel — Specialty Coffee Brand That Happens to Be Smart?

**Verdict: Yes. The brief and the spec tell the same brand story.**

Jackson's brief defines the visual language:

> HEALTH SIDE (#000/#0F0F0F, blue #488BED): precise, data-forward, WHOOP energy
> COFFEE SIDE (cream #F5F0E8, amber #D4A96A, warm white): cheerful, illustrative, specialty coffee brand energy

The spec's dual palette system matches these hex values exactly. The health side uses `#000000`, `#0F0F0F`, `#488BED`. The coffee side uses `#F5F0E8`, `#D4A96A`, `#FAF6F0`. The palette transition at Why Today is the seam between these two worlds.

Jackson's brief also specifies packaging aesthetics that "must feel at home next to these brands":
- Stitch: illustrated bear in Fiat 500 — playful, indie, bold
- Regent: art deco line illustration — craft spirits, elegant, earthy
- Corvus: gold temple mandala on black — ceremony, premium, weight

The spec's `imageBgTint` values and `cardVariant` assignments honor these:
- Stitch: dark card (`#2A2520`), imageBgTint `#2E2022` — the dark card inverts the typography and makes the bear illustration pop against warm charcoal. Bold.
- Regent: light card (`#FFFFFF`), imageBgTint `#F5F0E8` — neutral warm cream lets the art deco line work breathe. Elegant.
- Corvus: light card (`#FFFFFF`), imageBgTint `#2A2418` — deep gold ground for the mandala. The card is light (tracks roast, not packaging), but the image area is dark and ceremonial. Premium.

Three roasters, three visual identities, one component system. The `BeanCardView` renders all three using the same layout, the same slider system, the same tasting note icon grid, the same CTA patterns. The variation comes from the data — card variant, image tint, tasting note count, emotional tag visibility. This is how a multi-roaster platform should work. The spec builds for a catalog that grows without new components.

**Where I'd push further (post-launch, not a gap):** The brief says the coffee side should be "cheerful." The spec is warm, tactile, personal — but it's not *cheerful*. There's no moment of delight. No micro-animation when a bean is selected. No playful transition when the user taps BREW WITH THIS. The copy has personality (*"All systems go. Pick your weapon."*), but the interaction layer is standard push/present. Post-launch, consider: a subtle scale-up pulse on the bean card when tapped. A warm amber ring that expands from the CTA on press. Something that says "this app is glad you're here." Not now. Later. The current interactions are correct for v1. But cheerful is a feeling, not a palette, and the feeling comes from motion as much as color.

**Gap: None for v1. Post-launch opportunity for interaction delight.**

---

## The One Structural Gap I Found

The brief introduces a flow detail the spec doesn't explicitly address:

> Flow = Health data → Why Today screen (emotional bridge) → Bean selection → Brew settings
> Why Today + Bean Selection are the EMOTIONAL half of the app. Warm palette, illustrative, personal

The spec implements this flow. But Card 3 on the dashboard has a **secondary CTA**: *"Use general settings ›"* — which pushes directly to Screen 4 (Brew Settings), **skipping Why Today and Bean Selection entirely.**

Jackson's brief frames Why Today and Bean Selection as the emotional half of the app. The "Use general settings" shortcut bypasses both emotional screens. A user who always taps "Use general settings" never experiences the palette transition, never reads the emotional bridge copy, never sees the bean cards.

This shortcut is correct for power users on day 30 who know their routine. It's incorrect for new users on day 1 who haven't experienced the brand moment yet.

**Suggestion:** Gate the "Use general settings" link behind a threshold. Show it only after the user has completed the full flow 3+ times. Before that, the only path forward from Card 3 is "CHOOSE YOUR BEAN" → Why Today → Bean Selection. This ensures every new user experiences the emotional half at least three times before they're offered the shortcut.

This is a small logic change, not a design change. The component stays. The copy stays. Add a conditional:

```swift
func showGeneralShortcut() -> Bool {
    let completedFlows = UserDefaults.standard.integer(forKey: "completedBrewFlows")
    return completedFlows >= 3
}
```

**This is the only structural recommendation in this critique.** Everything else is alignment confirmation.

---

## Score Card — Brief vs. Spec Alignment

| Brief Requirement | Spec Coverage | Status |
|---|---|---|
| 3 confirmed beans (Stitch/Regent/Corvus) | All three beans with correct data | Aligned |
| Corvus = Sadayana Winey | Confirmed in bean data model | Aligned |
| Add to Cart = in-app web view | SFSafariViewController | Aligned |
| Subscribe & Save = fully designed, live at launch | Full design with subscribe/subscribed states | Aligned (pending Stitch/Corvus pricing) |
| Flow: Health → Why Today → Beans → Brew | Spec implements exact flow | Aligned |
| Why Today + Bean Selection = emotional half | Warm palette, illustrative, personal copy | Aligned |
| General Profile = equal card | Same height, same shadow, same radius | Aligned |
| 'Your history with this bean' after 3+ brews | BeanBrewHistoryCard with CTA flip | Aligned |
| Palette transition = hero brand moment | WhyTodayView with tunable crossfade | Aligned |
| Push notifications = tabled | Not in spec | Aligned (correctly omitted) |
| Bean packaging aesthetics | imageBgTint + cardVariant per roaster | Aligned |
| Flavor sliders (Roast/Sweetness/Acidity) | Three gradient bars with positioned dots | Aligned |
| Tasting note icons: flat illustration | Asset dependency, SF Symbol placeholders | Aligned (pending assets) |
| Post-brew check-in (Screen 6) | In spec as PostBrewView | Aligned |

**Alignment: 14/14.** No gaps between brief and spec.

---

## 3 Fixes for Next Round

### 1. Gate the "Use general settings" Shortcut

The brief says Why Today and Bean Selection are the emotional half of the app. The "Use general settings" link on Card 3 bypasses both. New users shouldn't have an escape hatch from the brand moment before they've experienced it.

**Fix:** Show "Use general settings ›" only after the user has completed the full flow (Health → Why Today → Bean Selection → Brew) 3+ times. Before that, the only forward path is "CHOOSE YOUR BEAN." One conditional. No design changes. No new components.

**Why this matters:** The palette transition is Brezi's brand thesis compressed into a gesture. If 40% of new users skip it on day 1 because the shortcut is right there, Brezi never lands its emotional pitch. The shortcut is a convenience for repeat users, not an escape for new ones.

### 2. Resolve Corvus "Medium-Light" vs. ".medium" Label

Jackson's brief says Corvus Sadayana Winey is "Medium-Light" roast. The spec's `RoastLevel` enum has `.light`, `.medium`, `.dark` — no `.mediumLight`. The slider at 0.45 reads visually correct, but text labels (detail view, adaptation notes) will say "Medium" where the brief says "Medium-Light."

**Fix:** If Jackson considers "Medium-Light" a required label, add it to the enum and update the four places it renders: detail view origin section, adaptation note key lookup, general settings copy, and compact card price-line area. If "Medium" is acceptable (the slider position communicates the nuance), leave it. This is Jackson's call. Ask him.

### 3. Confirm Stitch/Corvus Pricing Timeline

The brief says Subscribe & Save will be live at launch. The spec handles both priced and unpriced beans gracefully. But if Stitch and Corvus launch without pricing, the marketplace has two of three beans showing NOTIFY ME instead of ADD TO CART, and the subscribe banner says "Available on select beans" — which means one bean.

**Fix:** This isn't a design fix. It's a business question. Jackson: will Stitch and Corvus have pricing before launch? If yes, the marketplace is fully live. If no, the spec handles it — but "Subscribe & Save" as a launch feature means "subscribe to Regent." That's honest but narrow. The TBD banner and bean-name toast handle the messaging. But Jackson should know that the marketplace's emotional impact scales with the number of purchasable beans. One buyable bean is a product page. Three buyable beans is a shop.

---

## Verdict

The spec covers the brief. Fourteen requirements, fourteen matches. The data is correct. The flow is correct. The palette, the copy, the components, the states — all aligned.

The three fixes above are: a logic gate (5 lines of code), a labeling question (Jackson's call), and a business timeline confirmation (not a design or engineering item). None of them block the build. None of them change the component list. None of them affect the 9-day estimate.

Round 3 said *"ship it."* Round 4 says *"the brief agrees — ship it."*

Jensen starts Phase 1. Picasso moves to health-side design. The coffee side is locked.

--- Elon
