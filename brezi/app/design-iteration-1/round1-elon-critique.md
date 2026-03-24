# Round 1 Critique — Elon (COO)
**Re:** Picasso's Wireframe & Design Spec v2.0
**Date:** March 19, 2026
**For:** Picasso, Jensen, Jackson

---

## 1. What Works Well

**The chain is the right call.** Three cards — your data, Brezi's read, your brew — stacked vertically on the dashboard. That's the product thesis made visible. Jackson was right that v1 hid the correlation. This fixes it. The user opens the app and immediately sees *why* they're getting what they're getting. That's trust-building in a single scroll.

**Onboarding order is correct now.** Device first, wearable second, tutorial third, input fourth. This is the right sequence because it anchors the user in the physical product before asking them to connect data sources. The machine is the thing they paid for. The app is the companion. Picasso got this.

**The "Why This Brew" chain on the detail screen is strong.** Repeating the data-insight-brew chain with real user data on `BrewRecommendationView` is the right move. It turns transparency into a feature, not an afterthought. Users who care will scroll down. Users who don't will hit "Start Brew" and move on. Both paths work.

**Extensibility is architected, not bolted on.** The `RecommendationType` enum, the Device Hub's "Coming Soon" slot for Blender, the device-agnostic data layer — this is how you build a platform without over-building a v1. The Blender placeholder in Device Hub is smart marketing too. It tells early adopters we're thinking bigger.

**Profile expansion is what Jackson asked for.** Roast, grind, servings, timing window — all present. The caffeine cutoff logic that shifts recommendations one tier gentler after the user's set time is a genuinely thoughtful product decision. That's the kind of thing that makes someone feel like the app *gets* them.

**Empty states have personality.** "You're the last variable" is a good line. It's activating, not passive. Fits the brand.

---

## 2. What's Missing or Weak

**The morning re-engagement loop is underdesigned.** The dashboard assumes the user opens the app. But *why* do they open it? The push notification says "Your signal's ready. Time to check in." That's fine for day 2. What about day 30? Day 90? There's no evolution in the notification strategy. No personalization. No hook that makes tomorrow's notification more interesting than today's. The spec says "morning push notification" like it's a checkbox. It's not. It's the entire retention mechanism. Picasso needs to think about this as a system, not a single trigger.

**No post-brew feedback loop on the dashboard.** The user brews. They rate it (maybe). Then what? The dashboard doesn't change. There's no "Yesterday you rated Deep Work 5 stars on a recovery score of 8 — your pattern is forming" moment. The correlation card is buried in History. But the *feeling* of the correlation — "this is working" — needs to live on the home screen. The dashboard is static after the brew. It should evolve throughout the day.

**The manual input path is a second-class experience.** If someone doesn't have a wearable, Card 1 becomes a slider. That's fine mechanically. But the insight text for manual users will be thin — "Your recovery score is 5. Standard output." There's no depth because there's no data. The spec doesn't address how to make the manual path *feel* as rich as the wearable path. These are your early adopters who bought a machine but haven't bought a WHOOP yet. They need to feel like the app is still doing something intelligent for them, not just mapping a number to a brew name, which is exactly what v1 did.

**No social proof or community signal anywhere.** Brezi is a practice. Practices are social. There's no "1,247 people brewed First Light this morning" or "Most common recovery score today: 6." I'm not saying build a social feed. I'm saying a single ambient data point on the dashboard — anonymized, aggregated — would make the user feel like they're part of something. Right now the app feels like a solo tool. The brand says "practice." Practices have communities.

**Device pairing UX is too optimistic.** The spec shows three states: default, searching, found. What about: Bluetooth off systemwide? Device powered off? Device already paired to another phone? Firmware update required? These are the states that will actually happen in the first 5 minutes of ownership, and every one of them is a churn moment. "We'll find it" is a promise. The spec needs to show what happens when we don't.

**The "Adjust Settings" manual override has no guardrails.** The user can slide temperature, time, and dose to whatever they want. What if someone sets 25C, 5 minutes, 120g/L? That's not cold brew. That's mud. The override sheet needs either: (a) bounded ranges per profile, or (b) a warning when settings exit the recommended zone. Otherwise we're letting users make bad coffee and blame the product.

**History screen lacks a time dimension.** There's a streak card and a correlation scatter, but no trend line. "Your average recovery score this week vs last week" or "Your HRV trend over 30 days" — this is the data that makes someone feel like the app is *tracking* their progress, not just logging it. The scatter plot is interesting but it's a snapshot. People want trajectory.

**No offline state handling.** What happens when there's no internet? No Bluetooth? The spec assumes connectivity. Brezi machines will be in kitchens with spotty WiFi. The app needs to degrade gracefully — cached last recommendation, manual brew logging, sync-when-available. None of this is specced.

---

## 3. Product Questions Picasso Must Answer

1. **What happens at 3pm when the user hasn't brewed yet?** The dashboard shows a morning recommendation. Does it expire? Does it shift? Does it say "It's past your caffeine cutoff — skip today or go Tide Pool"? The spec is silent on time-of-day behavior after the morning window.

2. **Can the user re-check-in?** If I check in at 6am feeling Rebuilding, then take a nap and feel better at 10am, can I re-run the recommendation? Where? How? The spec implies one check-in per day.

3. **What's the Bluetooth pairing experience for someone who's never paired a Bluetooth device before?** Our customer is an active person who cares about their body and their coffee. They're not necessarily technical. "Bring your phone close" is Apple-speak. Does Brezi use NFC? BLE advertising? What's the actual discovery mechanism, and how do we make it feel magical instead of fiddly?

4. **How does the rating system feed back into future recommendations?** The user rates brews 1-5 stars. Does that data actually influence anything? If I consistently rate Voltage low, does Brezi stop recommending it at score 9? The spec shows data collection but no feedback loop into the engine.

5. **What's the wearable sync cadence?** Real-time? On app open? Scheduled background fetch? This matters because the dashboard shows "Source: WHOOP - 6:42am" — if the user opens the app at 6:45am and the data is 3 minutes old, fine. If they open at 9am and the data is still from 6:42am, that looks broken.

6. **Where does bean inventory or purchase flow live?** The app recommends "Medium roast, Medium grind." Does the user have that? Are we assuming they do? Is there a future path to "Order these beans" in the brew card? This feels like a monetization unlock that the architecture should anticipate even if v1 doesn't build it.

---

## 4. Brand / Tone Issues

**The insight text is too clinical.** "Mid recovery. HRV dipped but sleep was adequate. Standard output — don't push it." This reads like a fitness tracker, not Brezi. Brezi's brand is sharp and active. The tone should feel like a coach who respects you, not a dashboard readout. Compare:

- Clinical: "Mid recovery. HRV dipped but sleep was adequate."
- Brezi: "Not your best night. Not your worst. Let's keep it clean today."

The insight strip is one of the few places the brand *talks* to the user in natural language. It needs to sound like Brezi, not like MyFitnessPal.

**"Reading your signal..." is good.** Keep it. It's on-brand. The machine metaphor works.

**The onboarding brand screen body copy is too long and too defensive.** "Most people compromise their coffee. Ice dilutes it. Hot brew oxidizes it. You've been drinking the workaround this whole time." That's four sentences explaining what's wrong with other coffee. Brezi shouldn't spend its first impression attacking alternatives. It should spend it making a promise. The user already bought the machine. They don't need to be sold on cold brew. They need to be activated.

**"GOT IT" as a CTA on the tutorial screen is passive.** The user just saw the most important screen in the app — the chain that explains how Brezi thinks. "Got it" is what you tap to dismiss a cookie banner. This moment deserves more energy.

**"BUILD MY FIRST BREW" is strong.** Keep it. That's the right energy for the first check-in.

**Settings screen has no brand personality.** It's a standard iOS settings list. That's fine — settings don't need to be exciting. But the section headers ("Devices," "Brew Preferences," "Units") are generic. Small opportunity to inject voice: "Your Gear," "How You Brew," "Numbers."

---

## 5. Specific Copy Changes

| Screen | Current | Suggested | Why |
|---|---|---|---|
| Onboarding 1 — body | "Most people compromise their coffee. Ice dilutes it. Hot brew oxidizes it. You've been drinking the workaround this whole time." | "Brezi reads your body and builds your brew. Cold. Precise. Personal. Every morning." | Stop selling against alternatives. Sell the promise. They already own the machine. |
| Onboarding 1 — CTA | "START THE PRACTICE" | Keep as-is | It works. Strong verb, brand-aligned noun. |
| Onboarding 2 — subhead | "Turn on your machine and bring your phone close. We'll find it." | "Power on your Brezi. We'll find each other." | Warmer. Less instructional. Two sentences, not three. |
| Onboarding 4 — headline | "This is how Brezi thinks." | "Your data becomes your brew." | More active. Centers the user's data, not the brand's intelligence. |
| Onboarding 4 — CTA | "GOT IT" | "I SEE IT" or "LET'S GO" | "Got it" is dismissive. "I see it" mirrors the visual chain they just watched. "Let's go" has momentum. |
| Onboarding 5 — subhead | "No wrong answers. Brezi works with whatever you've got." | "Honest answer. Brezi adjusts." | Tighter. Four words instead of ten. Same meaning. |
| Dashboard — Card 2 insight (mid) | "Mid recovery. HRV dipped but sleep was adequate. Standard output — don't push it." | "Rough edges from last night. Your body's functional, not sharp. Clean brew, easy start." | Brand voice. Reads like a person, not a report. |
| Dashboard — Card 2 insight (low) | "Your body's rebuilding. Low stimulus, high patience." | "Yesterday hit you. Go light. Let your system come back on its own terms." | More specific, more human. |
| Dashboard — Card 2 insight (high) | "Strong recovery. Your system is ready for high output." | "You're sharp today. Full signal. Let's use it." | Active. First-person feeling. |
| History — empty | "Nothing brewed yet. Your data's ready. Your machine's ready. You're the last variable." | Keep as-is | This is excellent. Don't touch it. |
| Dashboard — empty first use | "Your practice starts blank. Log your first recovery score — give it three mornings." | "Your practice starts blank. Give it three mornings." | Cut the instruction. The UI already shows the recovery input. Don't narrate what's obvious. |
| Loading — post check-in | "Reading your signal..." | Keep as-is | On-brand. Clean. |
| Brew confirmation | "Brewed." | "Done." | One syllable. Brezi is blunt. |

---

## 6. One Big Structural Suggestion

**The dashboard needs a fourth state: post-brew.**

Right now the dashboard has two modes: "here's your recommendation" and "you haven't brewed yet today." But the most important moment for retention is *after* the brew. The user made coffee, drank it, and now they're living their day. The app should acknowledge that.

After the user brews (or logs a brew), the dashboard should transform:

- **Card 1 (Your Data):** Stays the same. This is today's baseline.
- **Card 2 (Brezi's Read):** Shifts from recommendation rationale to reflection. "You went with First Light on a mid-recovery day. That's the right call. Rate it when you're ready."
- **Card 3 (Your Brew):** Flips from recommendation to record. Shows what was brewed, when, and a prominent rating prompt. The "Start Brew" button becomes "Rate This Brew." If already rated, shows the rating with an option to edit.

This closes the daily loop: check in → brew → reflect → tomorrow. Without it, the dashboard is a one-shot tool. With it, the dashboard is a daily practice. And "practice" is the word we use in every piece of brand copy. The product should act like one.

---

## 3 Things Picasso Must Fix in Round 2

1. **Design the post-brew dashboard state.** The app currently goes dead after the user brews. Add a post-brew transformation to the three-card stack that shifts from recommendation to reflection + rating. This is the difference between a tool and a practice. Without it, the user has no reason to reopen the app until tomorrow morning.

2. **Rewrite all insight strip copy in Brezi's voice.** Every `insightText` example in the spec reads like a health dashboard. Brezi's voice is direct, warm, and a little bit blunt — like a coach who's also your friend. Write 3 examples per recovery tier (high/mid/low) that sound like a person talking, not a system reporting. Include variants so the copy doesn't feel robotic by day 5.

3. **Spec the failure states for device pairing.** Bluetooth off, device not found after 30 seconds, device paired to another phone, firmware update needed. Each of these needs a screen with clear copy and a recovery path. This is the first real interaction with the product. If it's frustrating, nothing else in the app matters.

---

*Picasso — the architecture is right. The chain is the product and you made it the UI. Now make it feel like Brezi, not like a wireframe. The bones are strong. The voice needs work. Fix these three and we're close.*

— Elon, COO
