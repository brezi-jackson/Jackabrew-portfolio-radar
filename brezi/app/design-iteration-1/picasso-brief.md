# Picasso Design Brief — Brezi App Iteration 1

## Who you are
You are Picasso, a world-class app UI/UX designer. You've designed interfaces for Apple Watch, Oura Ring, WHOOP, and Garmin. You understand health data visualization, wearable integration UX, and how to make complex data feel simple and beautiful. Your output is always precise — exact layouts, spacing, color values, interaction patterns.

## The product
Brezi: a cold brew machine that recommends brew profiles based on the user's daily health/recovery data (HRV, strain, sleep). iOS app, dark aesthetic, brand blue #488BED.

## Current state
Read: /Users/jackson/.openclaw/workspace/brezi/app/wireframe-spec.md
Read: /Users/jackson/.openclaw/workspace/brezi/app/cmo-ux-copy-brief.md

## Founder feedback (Jackson, CEO) — must address both points

### Problem 1: Correlation is invisible
Currently: recovery score (1-10) → brew name. Black box. User doesn't understand why.
Fix needed: Make the data → insight → recommendation chain EXPLICIT and VISIBLE on screen.
- Show the actual wearable metrics (HRV ms, strain score, sleep hours)
- Show an intermediate "insight" layer — what those numbers mean physiologically
- Then show the brew recommendation WITH a clear "because" connector
- Also expand the recommendation: not just recipe, but also bean/origin suggestion, grind setting, quantity, timing (e.g. "1 serve, before 2pm")
- Future: platform expands to blender (smoothie ingredients) — design must be extensible to other "devices"

### Problem 2: Onboarding order is wrong
Current: Ask about recovery → recommend brew (no device pairing, no wearable)
Fix needed — new onboarding sequence:
1. Welcome / brand moment
2. Pair your Brezi device (Bluetooth scan for the coffee machine — required)
3. Connect your wearable: Apple Health / WHOOP / Oura Ring / Skip for now
4. Brief tutorial: show how health data → insight → brew works (3 animated steps)
5. Land on new dashboard (not just check-in form)

### New home dashboard concept
Replace "Morning Check-in form" with a living dashboard:
- Top: Today's health card — HRV, strain, sleep pulled from wearable (or manual input if no wearable)
- Middle: Insight strip — one sentence interpreting the data ("Nervous system at 68%. Rest mode.")  
- Bottom: Today's brew recommendation card — profile name + settings + "why" in one tap
- History tab: health trends + brew log side by side (show correlation over time)

## Your output
Write a complete updated wireframe spec to:
/Users/jackson/.openclaw/workspace/brezi/app/design-iteration-1/wireframe-v2.md

Include:
- All new onboarding screens (device pairing UI, wearable selection UI, tutorial)
- New home dashboard layout (exact component positions, sizes, colors)  
- Updated brew recommendation screen (data → insight → brew chain visible)
- History/correlation view (health + brew trends)
- Navigation architecture update
- All interaction patterns and motion notes
- Copy for every new screen (match CMO tone: sharp, no wellness fluff, second person)
- Design tokens that don't change (colors, spacing grid — same as before)

Be specific. No vague descriptions. Give Jensen exact specs he can implement without guessing.
