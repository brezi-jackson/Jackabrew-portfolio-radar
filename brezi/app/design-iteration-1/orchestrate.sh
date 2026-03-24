#!/bin/bash
# Brezi App — Automated Design → Build Pipeline
# Picasso x3 iterations → Jensen implements → build verify → notify Jackson

set -e
WORKSPACE="/Users/jackson/.openclaw/workspace/brezi"
APP_DIR="$WORKSPACE/ios-app"
DESIGN_DIR="$WORKSPACE/app/design-iteration-1"
LOG="$DESIGN_DIR/pipeline.log"

log() { echo "[$(date '+%H:%M:%S')] $1" | tee -a "$LOG"; }

log "═══════════════════════════════════════════"
log "BREZI APP PIPELINE — Starting"
log "═══════════════════════════════════════════"

# ──────────────────────────────────────────────
# PICASSO ITERATION 1 — Full redesign from brief
# ──────────────────────────────────────────────
log "PICASSO ITERATION 1 — Full UX redesign"

claude --permission-mode bypassPermissions --print "
You are Picasso, a world-class app UI/UX designer who has designed interfaces for Apple Watch, Oura Ring, WHOOP, and Garmin. You understand health data visualization, wearable UX, and making complex data feel effortless. Your specs are so precise that engineers can implement without asking a single question.

Read these files first:
- $WORKSPACE/app/wireframe-spec.md (current spec)
- $WORKSPACE/app/cmo-ux-copy-brief.md (tone & copy)
- $WORKSPACE/app/jensen-tech-spec.md (tech constraints)

## Jackson's Core Feedback
1. CORRELATION IS INVISIBLE: HRV score → brew name is a black box. Make the chain explicit on screen: raw wearable data → physiological insight → brew recommendation + reason. Also expand recommendation: bean origin suggestion, grind, quantity, best time to drink.
2. ONBOARDING ORDER IS WRONG: Must be: (1) pair Brezi device via Bluetooth, (2) connect wearable (WHOOP/Oura/Apple Health/Skip), (3) animated tutorial showing data→insight→brew, (4) land on dashboard.
3. FUTURE PLATFORM: Design must be extensible — future Brezi blender will recommend smoothie ingredients using same health data pipeline.

## New Architecture to Design

### Home Dashboard (replaces morning check-in form)
- Health card: live wearable metrics (HRV in ms, strain 0-21, sleep hours/quality)
- Manual input fallback if no wearable connected
- Insight strip: one sharp sentence interpreting the data
- Brew recommendation card: profile + settings + explicit 'because' reason
- Quick-brew CTA

### Onboarding (5 screens)
1. Brand hero — cold, dark, sharp
2. Device pairing — Bluetooth scan animation, pair Brezi machine
3. Wearable connection — WHOOP / Oura Ring / Apple Health / Skip
4. Tutorial — 3-step animation: 'Your data' → 'Your insight' → 'Your brew'
5. Ready state — first dashboard preview

### Brew Recommendation Screen
- Show: actual wearable data used (HRV: 68ms ↓14%, Strain: 12.3, Sleep: 6h 40m)
- Show: insight layer ('Nervous system under load. Recovery mode.')
- Show: brew recommendation WITH visible connector ('So your brew today →')
- Expand: bean origin recommendation, grind setting, serving size, best time to drink
- Show: what NOT to do ('Skip the double shot before 2pm')

### History / Correlation View
- Side-by-side: HRV trend line + brew history
- Pattern callout: 'On your Voltage days, your next-day HRV averages 8% higher'
- Brew streak counter

### Device Hub (extensible for future blender)
- My Devices: Brezi Cold Brew Machine [connected] / Brezi Blend [coming soon]
- Each device shows its recommendation in context of same health data

Write the complete wireframe spec to: $DESIGN_DIR/wireframe-v1-draft.md

Be exhaustive. Every screen. Every component. Exact pixel/pt values, hex colors, copy strings, interaction patterns, motion specs. No vague descriptions.
" > "$DESIGN_DIR/picasso-iter1.log" 2>&1

log "Iteration 1 complete. File: $(wc -l < $DESIGN_DIR/wireframe-v1-draft.md 2>/dev/null || echo 0) lines"

# ──────────────────────────────────────────────
# PICASSO ITERATION 2 — Critical review + improve
# ──────────────────────────────────────────────
log "PICASSO ITERATION 2 — Self-critique and improve"

claude --permission-mode bypassPermissions --print "
You are Picasso, the same world-class designer. You just completed iteration 1 of the Brezi app redesign.

Read your iteration 1 output: $DESIGN_DIR/wireframe-v1-draft.md

Now do a brutal self-critique as if you were an Apple design reviewer:
1. Does every screen make the HRV→insight→brew correlation VISCERALLY clear? Not just present — obvious at a glance.
2. Is the device pairing UX as smooth as pairing AirPods? What's missing?
3. Is the wearable connection screen as trustworthy-feeling as WHOOP's own app?
4. Does the home dashboard feel alive (like Oura's readiness ring) or just a list of data?
5. Is every copy line as sharp as the CMO brief demands — no wellness fluff, no filler?
6. Is the extensibility (future blender) actually designed in, or just mentioned?
7. Are the empty states (no wearable, first launch, no brew history) handled with the right tone?
8. Mobile-first: will all of this actually work on a 390pt wide iPhone screen?

For each problem you find, fix it directly in the spec.

Write the improved spec to: $DESIGN_DIR/wireframe-v2-draft.md

This iteration should be noticeably better than v1 — tighter information hierarchy, stronger emotional arc through the onboarding, more precise component specs.
" > "$DESIGN_DIR/picasso-iter2.log" 2>&1

log "Iteration 2 complete. File: $(wc -l < $DESIGN_DIR/wireframe-v2-draft.md 2>/dev/null || echo 0) lines"

# ──────────────────────────────────────────────
# PICASSO ITERATION 3 — Final polish + handoff
# ──────────────────────────────────────────────
log "PICASSO ITERATION 3 — Final polish + Jensen handoff"

claude --permission-mode bypassPermissions --print "
You are Picasso, finalizing the Brezi app design for engineering handoff to Jensen (iOS developer).

Read iteration 2: $DESIGN_DIR/wireframe-v2-draft.md

Do the final pass:
1. Consistency check — are all spacing values on the 8pt grid? All colors using the exact brand hex values? (#000000, #FFFFFF, #F5F5F5, #488BED, #888888, #E0E0E0, #0F0F0F, #1A1A1A)
2. Component inventory — make sure every reusable component is named, specced, and listed with its file name
3. Navigation map — draw a complete ASCII navigation tree showing every screen and how they connect
4. Animation spec — every transition must have duration, easing, and trigger
5. Edge cases — what happens when: wearable disconnects mid-session? WHOOP returns no data? User skips device pairing? No brew history?
6. Accessibility — minimum tap target 44x44pt on all interactive elements
7. Final copy pass — every string on every screen, ready to copy-paste into Swift

Write the FINAL spec (this is the one Jensen will implement from) to:
$DESIGN_DIR/wireframe-v2-FINAL.md

This file must be complete, self-contained, and require zero clarification from Jensen to implement.
" > "$DESIGN_DIR/picasso-iter3.log" 2>&1

log "Iteration 3 complete. FINAL spec: $(wc -l < $DESIGN_DIR/wireframe-v2-FINAL.md 2>/dev/null || echo 0) lines"

# ──────────────────────────────────────────────
# JENSEN — Implement the new spec
# ──────────────────────────────────────────────
log "JENSEN — Implementing new spec"

claude --permission-mode bypassPermissions --print "
You are Jensen, the iOS developer building the Brezi app. You have a working v1 build at $APP_DIR.

Read the final design spec: $DESIGN_DIR/wireframe-v2-FINAL.md
Read the tech spec: $WORKSPACE/app/jensen-tech-spec.md

Implement all the changes from the v2 spec into the existing codebase at $APP_DIR. This is a significant update:

1. NEW ONBOARDING: 5 screens — brand hero, device pairing (Bluetooth simulation for prototype — no real BT needed, use a mock 'scanning' animation), wearable selection (WHOOP/Oura/Apple Health/Skip), tutorial (3-step), ready state
2. NEW HOME DASHBOARD: Replace MorningCheckInView with a dashboard showing health card + insight strip + brew recommendation card. Keep manual input as fallback.
3. NEW BREW RECOMMENDATION SCREEN: Show wearable data used + insight layer + brew recommendation with explicit 'because' connector + bean/grind/timing recommendations
4. NEW HISTORY VIEW: Trend chart (use Swift Charts, iOS 16+) showing HRV + brew log correlation
5. DEVICE HUB: Simple settings screen showing connected devices (mock Brezi machine as 'connected', blender as 'coming soon')
6. UPDATE CORE DATA: Add any new fields needed (wearable type preference, device pairing state, bean recommendation, grind setting, serve timing)

For the Bluetooth pairing simulation: fake a 3-second scan animation, then show 'Brezi Machine Found' with a connect button. Store isPaired = true in UserDefaults. No real BT code needed for prototype.

For wearable data in prototype: if WHOOP is selected, try to read from UserDefaults key 'whoop_latest_data' (JSON). If Oura selected, same pattern. If Apple Health, use HealthKit with HRV and sleep read permissions. Always provide manual fallback.

Keep everything that works from v1. Only replace/add what the new spec requires.

After implementing, do a final review of every file for compilation errors, then run:
openclaw system event --text \"Done: Brezi app v2 implemented by Jensen — ready for review\" --mode now
" > "$DESIGN_DIR/jensen-impl.log" 2>&1

log "Jensen implementation complete"

# ──────────────────────────────────────────────
# VERIFY — Check files built correctly
# ──────────────────────────────────────────────
log "Verifying build structure..."
SWIFT_COUNT=$(find "$APP_DIR/Brezi" -name "*.swift" | wc -l | tr -d ' ')
log "Swift files: $SWIFT_COUNT"
find "$APP_DIR/Brezi" -name "*.swift" | sort >> "$LOG"

log "═══════════════════════════════════════════"
log "PIPELINE COMPLETE"
log "Final spec: $DESIGN_DIR/wireframe-v2-FINAL.md"
log "App code: $APP_DIR"
log "═══════════════════════════════════════════"

openclaw system event --text "Done: Brezi v2 full pipeline complete — Picasso x3 + Jensen implemented. Ready for Jackson review." --mode now
