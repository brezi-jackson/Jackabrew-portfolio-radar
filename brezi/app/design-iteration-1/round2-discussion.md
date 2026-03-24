# Round 2 Discussion — Picasso Responds to Elon
**Re:** Elon's Round 2 Critique of Wireframe & Design Spec v2.1
**Date:** March 19, 2026
**From:** Picasso (Product Design)
**To:** Elon, Jensen, Jackson

---

Clean critique. Most of it is right. Let me commit to exactly what Round 3 (final) will contain.

---

## The 3 Must-Fix Items — All Accepted

### 1. Double-Rating Ask — Fixed. One moment, one fallback.

Elon's right. The brew completion overlay and the post-brew dashboard both ask for a rating. That's two prompts for the same action. Here's how Round 3 handles it:

**Primary rating moment:** The brew completion overlay (§8). When the brew finishes, a full-screen overlay appears with the profile name, brew time, and a star rating row. This is the natural emotional peak — the brew just finished, the user is holding their cup. Rate here.

**Fallback:** If the user dismisses the completion overlay without rating (taps "Skip" or swipes down), the post-brew dashboard shows "RATE THIS BREW" as the CTA on Card 3. If they rated in the overlay, Card 3 shows the rating with "Edit ›" and "See you tomorrow morning." No second ask.

**State awareness:** A single `BrewLog.rating` field. The completion overlay writes to it. The dashboard reads from it. If non-nil, show the rating. If nil, show the CTA. One source of truth.

**Completion overlay spec update:**
```
┌─────────────────────────────────┐
│                                 │
│                                 │
│         Brewed.                 │  ← CondensedBold, 48px, white
│                                 │
│         FIRST LIGHT             │  ← Mono, 14px, #888888
│         7:14am · 12 min         │
│                                 │
│  How was it?                    │  ← Regular, 16px, #CCCCCC
│  ☆ ☆ ☆ ☆ ☆                     │  ← tappable stars, 36px
│                                 │
│  ┌─────────────────────────┐    │
│  │  DONE  →                │    │  ← blue filled, saves rating + dismisses
│  └─────────────────────────┘    │
│                                 │
│  Skip for now ›                 │  ← text link, #888888, dismisses without rating
│                                 │
└─────────────────────────────────┘
```

Stars default to unselected. Tapping "DONE" with no stars selected = skip (same as tapping "Skip for now"). This keeps it one tap for people who don't want to rate.

Done. No ambiguity for Jensen.

### 2. Manual-User Experience Gap — Fixed. No more upgrade nudge on the daily screen.

Elon's framing is correct. "Connect a wearable ›" on Card 1 every morning tells manual users they're doing it wrong. These are people who bought a Brezi without a WHOOP. They're our earliest believers. The app should validate their input, not upsell them.

**Round 3 changes:**

**Remove** "Connect a wearable ›" from Card 1 manual state entirely. The manual Card 1 becomes:

```
│  ┌─ 1  YOUR DATA ─────────────┐ │
│  │                             │ │
│  │         [  5  ]             │ │  ← 72px number, CondensedBold
│  │    ○──────●──────○          │ │  ← recovery slider (1–10)
│  │    1               10      │ │
│  │                             │ │
│  │  Quick set:                 │ │
│  │  [1–3 Rest] [4–6 OK] [7–10]│ │  ← chip row
│  │                             │ │
│  │  Manual check-in logged.    │ │  ← Mono, 11px, #888888
│  │                             │ │
│  └─────────────────────────────┘ │
```

"Manual check-in logged." — confirms the action without apologizing for the method. The slider is the interaction. The confirmation is the response. That's it.

**Move** "Connect a wearable ›" to two places where it's discoverable but not confrontational:
1. **Device Hub** — a row below the Brezi device card: "Add a Wearable › WHOOP · Oura · Apple Health". This is where you manage devices. It belongs here.
2. **Settings** — under a "Data Sources" section: "Wearable: None connected. Connect ›". Discoverable for the user who goes looking.

**Manual-user contextual suffix on Card 2:** Elon suggested this as optional. I'm including it. When a manual user has 3+ days of check-in history, Card 2 gets a suffix line:

```
"Not your best, not your worst. Keep it clean today."
3-day trend: improving                ← Mono, 13px, #888888
```

Or:

```
"Low signal. That's data, not a verdict. Rest and regroup."
Checked in 7 days straight            ← Mono, 13px, #888888
```

**Suffix logic for manual users:**
- Days 1–2: no suffix (not enough data)
- Days 3+: show one of two suffix types, alternating:
  - **Trend suffix:** "3-day trend: improving / steady / declining" — based on the direction of their last 3 manual scores
  - **Streak suffix:** "Checked in N days straight" — counts consecutive days with a manual check-in

This gives manual users a sense of continuity. Their data is thinner, but their commitment is acknowledged. Wearable users get HRV baselines. Manual users get consistency recognition. Both feel seen.

### 3. Post-Brew Reflection Copy — All 15 combinations written.

The table had 6. Elon's right that the other 9 would produce either a generic fallback or a wrong-sounding line. No fallback logic needed — I'm writing all 15.

Here's the complete table. Each line references the specific brew choice so it can't be used generically:

| Recovery | Brew Chosen | Reflection Copy |
|---|---|---|
| **High (7–10)** | Voltage | "Peak morning, peak brew. You matched the signal." |
| High | Deep Work | "Strong signal, focused pour. You're building something today." |
| High | First Light | "Strong signal, light brew. Playing it steady." |
| High | Tide Pool | "Everything's firing and you went gentle. Deliberate. Interesting." |
| High | The Blank | "Full signal, custom brew. You know what you wanted." |
| **Mid (4–6)** | Voltage | "You pushed past the signal. Let's see how it lands." |
| Mid | Deep Work | "Mid morning, focused brew. Solid read on yourself." |
| Mid | First Light | "You went First Light on a mid-recovery morning. That tracks." |
| Mid | Tide Pool | "Playing it safe on a mid day. Nothing wrong with that." |
| Mid | The Blank | "Mid recovery, custom build. You're running your own experiment." |
| **Low (1–3)** | Voltage | "Low signal, full send. Bold. Rate this one honest." |
| Low | Deep Work | "Rough night but you went for it. Respect. Rate it honest." |
| Low | First Light | "Low morning, standard brew. Keeping the practice without pushing." |
| Low | Tide Pool | "Low morning, gentle brew. You listened." |
| Low | The Blank | "Low recovery, custom call. Your body, your rules." |

15 lines. Jensen maps `(recoveryTier, brewProfile) → reflectionCopy`. No fallback logic. No guessing.

---

## The 5 New Problems — My Responses

### Problem 1 (Manual user) — Addressed above in Must-Fix #2.

Fully committed. No wearable nudge on Card 1. Contextual suffix for manual users with 3+ days history. The door to connect a wearable lives in Device Hub and Settings.

### Problem 2 (Reflection copy hole) — Addressed above in Must-Fix #3.

All 15 written. Done.

### Problem 3 (Brew cancel flow) — Agreed, speccing it in Round 3.

This is a real gap. "[Cancel Brew]" was a placeholder pointing at nothing. Here's the full cancel flow:

**Step 1: Confirmation.** Tapping "[Cancel Brew]" shows an inline confirmation (not a modal — consistent with our no-modal-during-frustration principle):

```
│                                 │
│  Cancel this brew?              │  ← Regular, 16px, white
│  Coffee in the machine will     │
│  be discarded.                  │  ← Regular, 14px, #CCCCCC
│                                 │
│  ┌──────────┐  ┌────────────┐  │
│  │  GO BACK  │  │  CANCEL IT  │  │  ← "GO BACK": outlined white
│  └──────────┘  └────────────┘  │  ← "CANCEL IT": filled, statusRed
│                                 │
```

**Step 2: After cancel.** Dashboard returns to pre-brew state. The brew is not logged. The recommendation stays the same. The user can tap "START BREW" again or choose a different profile. No "Cancelled" record — it didn't happen.

**Hardware question for Jensen:** Can the Brezi machine safely stop mid-brew? If yes, the app sends a cancel command via Bluetooth and waits for acknowledgment before transitioning back to pre-brew. If the machine can't stop mid-dispense (likely for the last few seconds of a pour), the confirmation copy changes to: "Stop after the current pour finishes." and the progress bar runs to the next safe stopping point.

**Edge case — Bluetooth disconnects during brew:** If the app loses BLE connection to the machine mid-brew, show:

```
│  Lost connection to Brezi.      │  ← Regular, 16px, white
│                                 │
│  Your brew may still be running │
│  on the machine.                │  ← Regular, 14px, #CCCCCC
│                                 │
│  ┌─────────────────────────┐    │
│  │  RECONNECT              │    │  ← blue outlined
│  └─────────────────────────┘    │
│  Mark as brewed ›               │  ← text link, #888888
```

"Mark as brewed" transitions to post-brew state and logs the brew with a `connectionLost: true` flag. The user still gets to rate it. "Reconnect" re-initiates BLE scan.

### Problem 4 (OAuth failure states during onboarding) — Agreed, adding them.

Same class as the Bluetooth failure states. I missed it. Round 3 adds three OAuth states to Screen 3:

**OAuth cancelled by user:**
No error state. Return to the wearable selection screen silently. They changed their mind. The selected wearable row deselects. That's it.

**OAuth failed (network/server):**
```
│  ┌─────────────────────────────┐│
│  │  Couldn't reach WHOOP       ││  ← Regular, 16px, white
│  │  right now.                  ││
│  │                              ││
│  │  Their servers might be      ││
│  │  busy. Try again or skip     ││  ← Regular, 14px, #CCCCCC
│  │  and connect later from      ││
│  │  Settings.                   ││
│  │                              ││
│  │  ┌─────────────────────┐    ││
│  │  │  TRY AGAIN           │    ││  ← blue outlined
│  │  └─────────────────────┘    ││
│  └─────────────────────────────┘│  ← inline card, #1A1A1A, amber left border
│                                 │
│  I'll log manually ›            │  ← text link, #888888
```

Inline card, no modal. Same pattern as the Bluetooth failure states. "I'll log manually" is the skip — same as it was before, but now it's also the escape from a broken OAuth.

**OAuth succeeded, data pull failed:**
```
│  ┌─────────────────────────────┐│
│  │  ✓  WHOOP  ·  Connected     ││  ← green accent
│  │     ◌ Syncing your data...  ││  ← Mono, 11px, statusYellow, spinner
│  │     We'll pull it when       ││
│  │     the app opens.           ││  ← Mono, 11px, #888888
│  └─────────────────────────────┘│
│                                 │
│  ┌─────────────────────────┐    │
│  │  CONTINUE  →            │    │  ← blue filled, enabled
│  └─────────────────────────┘    │
```

Connected is connected. The data will come. Don't block onboarding. The "CONTINUE" button is enabled — the user moves to Screen 4. When they land on the dashboard, if wearable data still hasn't synced, Card 1 falls back to manual mode with a note: "WHOOP connected · Waiting for first sync. Check in manually for now."

### Problem 5 (Build order — onboarding priority) — Noted. Jensen's call.

Elon's right that a polished dashboard behind a rough front door is backwards. But this is an engineering prioritization question, not a design question. I'll make sure the onboarding spec in Round 3 is complete enough to build in Week 1 if Jensen decides to sequence it that way. The spec won't be the bottleneck.

I will note: onboarding has real dependencies (Bluetooth stack, OAuth integration) that the dashboard doesn't. Building onboarding first means those integrations have to be ready first. Building the dashboard first with mock data lets Jensen validate the data layer and UI framework while the integration layer matures. Both sequences are defensible. Jensen should pick based on what unblocks the most work downstream.

---

## Copy & Voice Notes — Responses

### "Brewed." — Settled. Moving on.

### Rest Day copy — Accepted. Rewriting.

Elon's right. "Let your body come back on its own" is yoga-app voice. Brezi is direct. New Rest Day copy for Round 3:

**Rest Day card body:**
> "Not today. Recovery's low and the window's closed. Tomorrow."

That's the one. It's blunt. It respects the user's discipline by being honest. It ends with forward momentum — "tomorrow" implies the practice continues.

**Rest Day Card 2 insight (replaces the insight strip in post-cutoff + low recovery):**
> "Your body's telling you to sit this one out. Listen to it."

Two lines, both from Elon's suggestions. They're better than mine.

### Notification Tier 1 micro-variation — Accepted.

Two extra strings. Worth it. Round 3 notification copy:

```
Day 1:  "Your first signal is ready."
Day 2:  "Day 2. Your signal's ready."
Day 3+: "Your signal's ready. Time to check in."
```

Jensen tracks `notificationDayCount` in UserDefaults, increments daily, switches copy at thresholds 1 and 2. Falls through to the default after Day 2. Ten minutes of work.

### Post-cutoff "Choose a different brew" placement — Accepted.

Moving it outside the Rest Day card. New placement:

```
│  ┌─ 3  REST DAY ──────────────┐ │
│  │                             │ │
│  │  No brew recommended.      │ │
│  │                             │ │
│  │  Not today. Recovery's low  │ │
│  │  and the window's closed.   │ │
│  │  Tomorrow.                  │ │
│  │                             │ │
│  └─────────────────────────────┘ │  ← darkCard
│                                 │
│  ┌─────────────────────────┐    │
│  │  BREW ANYWAY             │    │  ← outlined button, white border, full-width
│  └─────────────────────────┘    │
│                                 │
│  ┌─────────────────────────┐    │
│  │  VIEW BREW HISTORY      │    │  ← outlined button, black border, full-width
│  └─────────────────────────┘    │
```

"BREW ANYWAY" instead of "Choose a different brew ›" — it's shorter, it's deliberate, and it matches the tone. Same behavior: opens the profile picker. But it's outside the card, full-width, visible. The user who overrides at 4pm is making a choice, not finding a hidden escape hatch.

---

## Elon's Other Notes — Quick Responses

**Firmware OTA failure state:** Adding it. Round 3 will include:

```
│  │  Update interrupted.        ││  ← Regular, 14px, white
│  │  Your Brezi is fine —       ││
│  │  nothing was changed.       ││  ← Regular, 14px, #CCCCCC
│  │                             ││
│  │  ┌─────────────────────┐   ││
│  │  │  TRY AGAIN           │   ││  ← blue outlined
│  │  └─────────────────────┘   ││
```

Jensen/hardware: confirm the device rolls back to previous firmware on interrupted OTA. If it does, this copy is accurate. If it doesn't, we need a different recovery path and I need to know what that looks like before writing the copy.

**Post-brew completion overlay → post-brew dashboard transition timing:** The completion overlay appears when the brew finishes. It stays until the user taps "DONE" or "Skip for now." No auto-dismiss. The user controls when they leave the overlay. After dismissal, the dashboard is already in post-brew state underneath — no additional transition animation needed. The overlay was covering the dashboard; removing it reveals the post-brew state.

**Onboarding Screen 3 — "I'll log manually" visibility:** Elon says it's too buried at `#888888`. Agreed. Round 3 bumps it:
- Color: `#CCCCCC` (from `#888888`)
- Size: 15px (from 14px)
- Add a leading line: "or" in `#888888` above the link, creating visual separation from the Connect button

```
│  ┌─────────────────────────┐    │
│  │  CONNECT  →             │    │  ← blue filled
│  └─────────────────────────┘    │
│                                 │
│  or                             │  ← Mono, 12px, #888888
│  I'll log manually ›            │  ← Regular, 15px, #CCCCCC
│                                 │
```

Not equal to Connect. But not buried. A valid choice, visually.

**Onboarding Screen 4 — early dismiss:** If the user taps "I SEE IT" before the 1.5s animation completes, let them through. The animation cancels, the chain snaps to its final state, the screen advances. Don't trap an impatient user. Round 3 spec will note: "Animation is interruptible. Tapping CTA at any point during the sequence completes all pending animations immediately and advances."

**Blender future-proofing:** Elon's three gaps are all valid:
1. **"Notify me" action:** Setting `interestedInBlender: true` on the user profile. No email capture, no push permission. A boolean we sync when we have a backend. Simplest measurable signal.
2. **Dashboard structural redesign for dual recommendations:** Acknowledged. The current dashboard assumes one recommendation chain. When the Blender ships, this changes. I'm adding a note to the Round 3 spec: "The dashboard's single-chain layout is intentional for v1. A second device (Blender) will require a structural dashboard redesign — horizontal selector or parallel cards — not a bolt-on." Jensen should not build the dashboard in a way that prevents this, but he also shouldn't build for it now.
3. **BlendProfile stub:** Jensen should add a comment in the Core Data model: `// BlendProfile: future entity for Brezi Blender. See RecommendationType.blend. Structure TBD.` No stub entity — empty entities in Core Data create migration obligations for no current value. A comment is enough.

**Content depth (3 variants per insight tier):** Noted as content debt. Not blocking v2.2. Will expand to 7–10 variants per tier within the first month of production. Flagged.

---

## What Round 3 (Final) Will Contain

This is the complete list. No surprises.

**Fixes:**
1. Single rating moment (completion overlay primary, dashboard fallback)
2. Manual user Card 1 — no wearable nudge, "Manual check-in logged."
3. Manual user Card 2 suffix — trend or streak line after 3+ days
4. All 15 post-brew reflection copy lines (complete tier × profile table)
5. Brew cancel flow with confirmation, post-cancel state, BLE disconnect handling
6. OAuth failure states on onboarding Screen 3 (cancelled, failed, data-pull-delayed)
7. OTA firmware update failure state
8. Rest Day copy rewrite ("Not today. Recovery's low and the window's closed. Tomorrow.")
9. Notification Tier 1 micro-variation (Days 1, 2, 3+)
10. "BREW ANYWAY" button outside Rest Day card, full-width, visible
11. "I'll log manually" visibility bump on Screen 3
12. Interruptible tutorial animation on Screen 4
13. "Notify me" → `interestedInBlender: true` boolean
14. Dashboard single-chain structural note for Blender future
15. Completion overlay → post-brew dashboard transition spec

**Not in Round 3:**
- BlendProfile entity (comment only, no stub)
- 7–10 insight variants per tier (content debt, post-launch)
- Build order recommendation (Jensen's domain)
- Community/social signal (parked for v2)

Round 3 is the final spec. After this, Jensen builds.

— Picasso
