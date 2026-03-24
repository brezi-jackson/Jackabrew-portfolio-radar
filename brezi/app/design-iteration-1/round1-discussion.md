# Round 1 Discussion — Picasso Responds to Elon
**Re:** Elon's Critique of Wireframe & Design Spec v2.0
**Date:** March 19, 2026
**From:** Picasso (Product Design)
**To:** Elon, Jensen, Jackson

---

Elon — good notes. Most of them land. A few I want to push back on. Let me go point by point.

---

## Where You're Right and What I'm Fixing

### 1. Post-Brew Dashboard State — Agreed, fixing it.

This is the biggest miss in the spec. You're correct that the dashboard dies after the user brews. I was so focused on the recommendation flow that I forgot the day doesn't end at "Start Brew." It ends at the rating.

**Round 2 will add a three-state dashboard:**

- **Pre-brew (morning):** Current spec. Data → Insight → Recommendation. This stays.
- **Brewing (active):** Progress overlay while the machine runs. Already specced.
- **Post-brew (rest of day):** Card 1 stays (your data is your data). Card 2 shifts to reflection copy — acknowledges the choice, nudges toward rating. Card 3 flips: brew record replaces recommendation, "Rate This Brew" replaces "Start Brew." If rated, shows the rating with edit option and a line like "See you tomorrow morning."

This is the right call. The daily loop is: check in → brew → live → reflect → sleep → repeat. I had the first two. I was missing the middle.

### 2. Insight Strip Copy — Agreed, rewriting everything.

You're right that the insight text reads like a fitness dashboard. I wrote placeholder copy to demonstrate the *structure* and forgot that structure without voice is just a wireframe that talks. Brezi's voice is direct, a little warm, a little blunt. Not clinical. Not cute. Like a coach who doesn't waste your time.

**Round 2 will include 3 variants per tier (9 total minimum) plus a rotation system so the user doesn't see the same line twice in a week.**

Here's the direction:

**High recovery (7–10):**
- "You're sharp today. Full signal. Let's use it."
- "Clean night. Your body's ready for something real."
- "Everything's online. Don't waste this one."

**Mid recovery (4–6):**
- "Rough edges from last night. Functional, not sharp. Easy start."
- "Not your best, not your worst. Keep it clean today."
- "Your system's running but it's not asking for more. Listen to it."

**Low recovery (1–3):**
- "Yesterday hit you. Go light. Let your body come back on its own terms."
- "You're rebuilding. This isn't the morning to push. Gentle brew, gentle day."
- "Low signal. That's data, not a verdict. Rest and regroup."

These sound like a person. I'll write the full set in the v2.1 spec with rotation logic.

### 3. Device Pairing Failure States — Agreed, speccing all of them.

I was too optimistic. You're right that the first 5 minutes of ownership are fragile, and I only designed the happy path. Round 2 will spec these states:

- **Bluetooth off (system-level):** Inline card with "Bluetooth is off. Brezi needs it to connect." + "Open Settings" link. No modal — keep them in context.
- **Device not found (30s timeout):** "Can't find your Brezi yet." + troubleshooting checklist: "Is it plugged in? Is it within 10 feet? Try turning it off and back on." + "Try again" button that resets the scan. + "Skip for now" remains available.
- **Device paired to another phone:** "This Brezi is connected to another device. Disconnect it from the other phone first, then try again." This is a Bluetooth limitation — we can't force-unpair.
- **Firmware update required:** "Your Brezi needs a quick update before pairing. This takes about 2 minutes." + progress bar + "Update Now" CTA. Block pairing until firmware is current.

Each state gets a dedicated layout in the spec. No more "we'll find it" without showing what happens when we don't.

### 4. Onboarding Body Copy — Agreed, cutting it.

You're right. Four sentences explaining what's wrong with other coffee is defensive. The user already bought the machine. They don't need the pitch. They need the promise.

Adopting your suggestion almost verbatim:

> "Brezi reads your body and builds your brew. Cold. Precise. Personal. Every morning."

That's the line. It's forward-looking, it's specific, and it doesn't insult anyone's drip machine.

### 5. Tutorial CTA — Agreed, changing it.

"GOT IT" is a cookie banner dismissal. You're right. Changing to **"I SEE IT"** — it mirrors the visual chain they just watched, and it's a moment of comprehension, not dismissal. "Let's go" was the other option but it doesn't acknowledge what they just learned. "I see it" does.

### 6. Other Copy Changes — Mostly adopting.

| Change | Verdict |
|---|---|
| Onboarding 2 subhead → "Power on your Brezi. We'll find each other." | **Adopting.** Warmer. |
| Onboarding 4 headline → "Your data becomes your brew." | **Adopting.** Better centers the user. |
| Onboarding 5 subhead → "Honest answer. Brezi adjusts." | **Adopting.** Tighter. |
| Dashboard empty → cut "Log your first recovery score" | **Adopting.** The UI is self-evident. |
| Brew confirmation → "Done." instead of "Brewed." | **Pushing back.** See below. |
| Settings headers → "Your Gear," "How You Brew," "Numbers" | **Adopting.** Low-effort, high-personality improvement. |

### 7. Manual Override Guardrails — Agreed, adding bounds.

You're right that unbounded sliders let users make mud. Round 2 will add:

- **Bounded ranges per profile.** Each profile defines min/max for temp, time, and dose. The sliders are constrained to that range.
- **If The Blank is selected (full manual):** Sliders have the full range but show an amber warning zone: "Outside recommended range" when values go extreme. No hard block — The Blank is for people who know what they're doing — but a visual signal.

This prevents bad coffee without patronizing power users.

---

## Where I'm Pushing Back

### 1. "Brewed." → "Done." — Keeping "Brewed."

"Done." is blunt, but it's generic. You say "done" when you finish a task, close a ticket, submit a form. "Brewed." is specific to the act. It's one word that only means one thing in this context. It's also a complete sentence that echoes the product — the machine brewed, the process completed, the practice was honored. "Done." could be anything. "Brewed." could only be this.

If Elon still feels strongly I'll revisit, but my instinct says the specificity wins here.

### 2. Community/Social Signal on Dashboard — Not for v1.

You said: "1,247 people brewed First Light this morning." I hear the intent — make the user feel like part of something. But I have two concerns:

**First, it's a lie at launch.** We'll have maybe 200 TestFlight users. "12 people brewed First Light this morning" doesn't feel like a movement. It feels like a ghost town. Ambient social data only works at scale. At our scale, it's a vanity metric that exposes how small we are.

**Second, it shifts attention away from the personal chain.** The entire v2 redesign was built on Jackson's insight: the correlation between *your* body and *your* cup is the product. The moment we put "what everyone else is doing" on the dashboard, we dilute that. The user starts comparing instead of reflecting. "Oh, most people got Voltage today and I got Tide Pool — am I broken?" That's the opposite of what we want.

**My counter-proposal:** Save community signal for the History screen, and only after we have real volume. A line like "First Light is the most common brew for mid-recovery days" in the correlation section — that's educational, not comparative. It enriches the pattern without competing with the personal chain.

If Jackson and Elon both want community on the dashboard, I'll design it. But I'd argue strongly for History instead.

### 3. Notification Evolution System — Partially pushing back on scope.

You're right that "morning push notification" as a single trigger is underdesigned. You're wrong that we need to solve Day 90 notifications in this spec.

Here's why: notification personalization depends on data we don't have yet. What makes tomorrow's notification more interesting than today's? The answer is pattern data — "Your HRV has been climbing all week" or "You've rated your last 3 brews 5 stars." We can't write those notification templates until we know what patterns actually emerge from real usage.

**What I will spec in Round 2:**

- **Day 1–7:** Static notification. "Your signal's ready. Time to check in." This works because everything is new.
- **Day 8–14:** Add the recovery trend. "Your recovery is up 12% this week. See what Brezi recommends." Now the notification carries data.
- **Day 15+:** Rotate between streak ("Day 14. Keep it going."), pattern ("You brew best on high-recovery mornings. Today's looking good."), and nudge ("Haven't checked in today. Your data's waiting.").

That's three tiers. It covers the first month. I'm not going to spec Day 90 when we don't know what Day 30 looks like with real users. We'll iterate on notification strategy based on retention data from TestFlight.

### 4. Trend Lines in History — Partially agree, scoping down.

You asked for "average recovery this week vs last week" and "HRV trend over 30 days." Both are good ideas. But a 30-day HRV trend chart is a full component with date range selection, axis scaling, and edge cases (missing days, wearable switches, manual vs. auto data mixing). That's a lot of design and engineering for a v1 History screen.

**What I will add in Round 2:**

- A simple **7-day recovery trend** — a spark line (no axes, no labels, just the shape) at the top of the History screen, showing the last 7 days of recovery scores. One glance tells you: trending up, trending down, or flat.
- Below it, a single line: "Avg recovery: 6.2 (↑ from 5.4 last week)."

That gives the trajectory feeling you're asking for without building a full charting component. If users want deeper analytics, that's a v2.x feature.

### 5. Offline State — Agree on principle, pushing back on spec depth.

You're right that we need to handle no-internet and no-Bluetooth. But this is more of an engineering concern than a design concern. The user-facing experience is simple:

- **No internet:** App works normally. All data is local (Core Data). iCloud sync happens when connectivity returns. User never knows.
- **No Bluetooth (after pairing):** Dashboard shows recommendation normally (data is already synced). "Start Brew" button shows: "Can't reach your Brezi. Brew manually and log it here." Button becomes "LOG THIS BREW."
- **No Bluetooth (during pairing):** Covered in the failure states above.

I'll add a brief offline behavior section to the spec, but this doesn't need wireframes. It needs a few rules and Jensen's implementation.

### 6. Bean Inventory / Purchase Flow — Acknowledged, not designing.

You asked where "Order these beans" lives. The answer is: nowhere in v1, and correctly so. But I agree the architecture should anticipate it. I'll add a note in the brew card spec: the roast/grind recommendation row should be a tappable component with a placeholder hook. When the commerce layer exists, that row becomes a link. For now, it's display-only. Jensen should build the component with an optional `onTap` closure that defaults to nil.

### 7. Wearable Sync Cadence — This is Jensen's call, not mine.

You asked the right question. The answer affects perceived freshness of the dashboard. My design recommendation: sync on app open (foreground fetch) + background fetch every 4 hours. The timestamp on Card 1 ("Source: WHOOP · 6:42am") tells the user when data was last pulled. If it's stale, they can pull-to-refresh. I'll note this in the spec, but the implementation details are Jensen's domain.

### 8. Rating Feedback Loop into Engine — Agree it should exist, not in v1 engine.

You asked: if I rate Voltage low three times, does Brezi stop recommending it? Yes, eventually. But the v1 engine is rule-based (recovery score → profile). Adding rating feedback means building a preference-weighted scoring layer on top of the rule engine. That's the right v2.x feature.

**What I will note in Round 2:** The `BrewLog` already stores ratings per profile per recovery score. The data is being collected. When the engine gets a preference layer, it's ready. For now, the insight strip can acknowledge past ratings: "You went Voltage last time at this recovery level and loved it." That's a lighter version of the same idea.

### 9. Re-Check-In — Good question. Adding it.

Yes, the user should be able to re-check-in. If you nap at 10am and your recovery changes, you should be able to re-run. I'll add a small "Re-check" action to the dashboard — either a refresh icon on Card 1 or a text link below the insight strip: "Feeling different? Update your check-in." This re-runs the manual input (or re-fetches wearable data) and recalculates the recommendation.

### 10. 3pm Behavior — Good question. Speccing it.

After caffeine cutoff, the dashboard should acknowledge it. Card 2 shifts: "Past your caffeine window. If you're still brewing, go Tide Pool or skip today." Card 3 shows Tide Pool (the gentlest profile) or a "Rest Day" state with no brew recommendation. The user can override via "Choose a different brew" — we're not their parent.

---

## Summary: Round 2 Changes

### Adding:
1. **Post-brew dashboard state** — three-card stack flips from recommendation to reflection + rating
2. **Insight strip copy rewrite** — 3 variants per tier, rotation logic, Brezi voice
3. **Device pairing failure states** — Bluetooth off, not found, already paired, firmware update
4. **Manual override guardrails** — bounded ranges per profile, amber warning on The Blank
5. **Notification tiers** — Day 1–7, Day 8–14, Day 15+ with pattern-based copy
6. **7-day recovery spark line** in History
7. **Re-check-in action** on dashboard
8. **Post-cutoff dashboard behavior** (3pm+ state)
9. **Offline behavior rules** (brief section, not wireframes)
10. **Commerce-ready hook** on roast/grind row (noted, not built)
11. **All copy changes** adopted from Elon's table (except "Done" — keeping "Brewed")

### Keeping as-is:
1. **"Brewed."** over "Done." — specificity wins over brevity
2. **No community signal on dashboard** — saves it for History at scale. The personal chain is the product. Don't dilute it.
3. **No Day 90 notification strategy** — spec three tiers, iterate with real data from TestFlight
4. **No 30-day trend charts** — 7-day spark line gives trajectory without building a charting library
5. **No social feed or community features** — Brezi is a personal practice first. Community is a later layer.
6. **No bean commerce flow** — architecture-ready, not designed

### For Jackson's decision:
- Community signal: Dashboard vs. History vs. not at all. I'm arguing History. Elon wants dashboard. Your call.
- "Brewed" vs. "Done" — small thing, but flagging it since Elon and I disagree.

---

*The bones are strong. Elon's right about that. The three things he flagged as must-fix are the right three things. I'll have v2.1 spec updated by end of day tomorrow with all changes above. The chain is still the product. Now it just needs to breathe through the whole day, not just the morning.*

— Picasso
