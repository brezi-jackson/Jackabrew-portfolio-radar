# Picasso's Response to Elon's Round 4 Audit
**Author:** Picasso (Product Design) | March 20, 2026
**Re:** Elon's Round 4 — Locked Brief vs. Shipped Spec alignment audit
**For:** Jackson (Product) · Elon (Growth/Copy) · Jensen (Engineering)

---

## The Short Version

Elon audited the spec against Jackson's locked brief. 14/14 aligned. His three "fixes" are: one I accept, one that's Jackson's call, and one that's not a design item. The spec gets one addition — a visibility gate on the general settings shortcut. Everything else stays.

---

## On the Audit Itself

This is the critique I wanted. Not "what's wrong" but "does this match the source of truth." Elon went line-by-line — bean data fields, hex values, copy strings, Shopify IDs — and confirmed alignment on every requirement in the brief. That's the most useful thing a reviewer can do at this stage: stop designing and start verifying.

14/14 is the number. The spec covers the brief. The brief covers the spec. No orphaned requirements. No speculative features. No drift.

---

## The 3 Fixes — My Take

### 1. Gate the "Use General Settings" Shortcut — ACCEPT

Elon's right. I should have caught this.

The brief says Why Today and Bean Selection are the emotional half of the app. I designed them as a hero moment — the palette transition, the typewriter, the bean cards. Then I put an escape hatch on Card 3 that lets new users skip all of it on day one.

That's a design contradiction. The shortcut exists to serve power users who've internalized the brand and just want their usual brew. It shouldn't be available to someone who hasn't experienced the brand moment yet.

**What changes in the spec:**

The "Use general settings >" secondary CTA on Card 3 (Your Brew) gains a visibility condition:

```swift
// Card 3 — Your Brew
var showGeneralShortcut: Bool {
    UserDefaults.standard.integer(forKey: "completedBrewFlows") >= 3
}
```

Before `completedBrewFlows >= 3`: Card 3 shows only "CHOOSE YOUR BEAN >" as the primary CTA. No secondary link. The only forward path is into Why Today.

After `completedBrewFlows >= 3`: Card 3 shows both "CHOOSE YOUR BEAN >" and "Use general settings >" as before. The user has earned the shortcut by completing the full flow three times.

`completedBrewFlows` increments when the user reaches Screen 5 (Active Brew) via the full path: Why Today > Bean Selection > Brew Settings > Brew. Skipping to general settings (once unlocked) does not increment the counter — the counter tracks full-flow completions only.

This is five lines of logic, zero new components, zero visual changes. Jensen can implement it in the Card 3 view without touching anything else. The copy, the layout, the animation — all unchanged. Just a conditional on visibility.

**Why I accept this without pushback:** The palette transition is the single most considered moment in this spec. Three rounds of refinement. Tunable config struct. Once-per-day replay logic. I designed it to land. Then I gave new users a door to walk around it. That's incoherent. The gate fixes the incoherence.

### 2. Corvus "Medium-Light" vs. `.medium` — JACKSON'S CALL, MY RECOMMENDATION: KEEP `.medium`

Elon framed this correctly. The `RoastLevel` enum has three values. The brief says "Medium-Light." The slider at 0.45 reads as medium-light visually. The question is whether the text label matters enough to add a fourth enum value.

My recommendation: keep `.medium`. Here's my rationale:

**The slider IS the label.** When a user looks at a bean card, they see a dot positioned at 45% on the roast gradient — clearly left of center, clearly not light, clearly not dark. That visual communicates "medium-light" more intuitively than any text label. The dot's position is the source of truth. The text is a fallback for accessibility.

**Adding `.mediumLight` has real cost.** It ripples to four places: the detail view origin table, the adaptation note key lookup (which drives copy selection), the general settings description copy, and the compact card metadata line. Each needs a new case. The adaptation note matrix gains a new row. The general settings copy needs a new paragraph. These aren't hard changes, but they're changes — in a spec I've locked.

**The brief's "Medium-Light" is descriptive, not prescriptive.** Jackson was describing the coffee to us. He wasn't specifying an enum value. The flavor profile numbers (45/85/50) are the prescriptive data. The slider at 0.45 renders that data faithfully.

But if Jackson reads "Roast: Medium" in the detail view and says "that should say Medium-Light" — then we add `.mediumLight` to the enum. It's a contained change. I'll spec the four touch points in an addendum if he asks.

**Nothing changes in the spec unless Jackson says otherwise.**

### 3. Stitch/Corvus Pricing Timeline — AGREE: NOT A DESIGN ITEM

Elon's right to flag this, and right that it's not a design or engineering question. The spec handles both states — priced beans get full purchase flow, unpriced beans get NOTIFY ME. The subscribe banner says "Available on select beans." The TBD banner appears when all visible beans lack pricing. The bean-name toast confirms the specific notification. Every state is designed.

Whether Stitch and Corvus have pricing at launch determines the marketplace's *commercial* completeness, not its *design* completeness. One purchasable bean is honest. Three purchasable beans is better. But the spec doesn't change either way.

**Jackson:** when you know the pricing timeline, tell Jensen so he can set the `price` and `shopifyProductId` fields in the bean data. The components are ready for either state.

**Nothing changes in the spec.**

---

## The One Spec Addition — Summary

Round 4 produces exactly one change to the locked spec:

| Change | Section | Impact |
|--------|---------|--------|
| General settings shortcut gated behind `completedBrewFlows >= 3` | Card 3 (Your Brew) | 5 lines of logic. No new components. No visual changes. |

That's it. One conditional. The rest of the spec — every screen, every component, every animation, every copy string, every data model — stays exactly as Round 3 shipped it.

---

## What Stays and Why

Everything from Round 3 stays. Elon confirmed alignment on all 14 brief requirements. I'm not going to re-justify decisions that passed a line-by-line audit. But I want to respond to two of his commentary notes:

**On the palette transition "visual beat" question:** Elon flagged that Jackson's brief uses the word "transition screen," which could imply a dedicated visual moment before the typewriter starts. Elon's own read is that the current implementation is correct — the typewriter IS the threshold. I agree. A branded gradient wash before the text starts would add 0.6 seconds of nothing. The user is already watching a palette shift while reading personal copy about their body. That IS the beat. Adding a prelude to the prelude dilutes it. If Jackson asks for it, it's a `WhyTodayConfig` property — but I'd argue against it.

**On post-launch "cheerful" interaction delight:** Elon noted that the coffee side is warm, tactile, and personal — but not *cheerful*. He's right, and his suggestions (scale-up pulse on bean tap, amber ring on CTA press) are good post-launch additions. But he's also right that v1 interactions should be standard push/present. Delight micro-animations that feel polished take iteration. Delight micro-animations that feel janky are worse than none. Ship clean, add personality when you can test it on real devices with real users. This goes on the v1.1 list alongside the general card illustrations and tasting note icons.

---

## What Happens Now

The spec is locked. One addition (the gate), zero revisions, zero open questions from design.

**For Jensen:** Start Phase 1. The spec at `round3-picasso.md` plus this one conditional is your build document. The gate is simple enough that I'm not rewriting the full spec — add the `showGeneralShortcut` check to Card 3's view body. Everything else is as specified.

**For Jackson:** Two questions are waiting on you, neither blocking the build:
1. Corvus roast label: "Medium" (current) or "Medium-Light" (brief)? If Medium-Light, I'll spec the four touch points.
2. Stitch/Corvus pricing timeline: when you know, tell Jensen. The components handle both states.

**For Elon:** Good audit. The gate was a real catch — the kind of thing that looks obvious in retrospect but only surfaces when someone reads the brief and the spec side by side with product intent in mind. That's your job and you did it well.

**For Picasso (me):** Moving to health-side design. The coffee side is done.

---

*Round 1 built the machine. Round 2 gave it a soul. Round 3 sharpened the edges. Round 4 was supposed to not exist — but Elon found one door that should have been locked. Now it is. Ship it.*

— Picasso
