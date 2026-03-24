#!/bin/bash
export CLAUDE_CODE_MAX_OUTPUT_TOKENS=100000

WORKSPACE="/Users/jackson/.openclaw/workspace/brezi"
APP_DIR="$WORKSPACE/ios-app"
D="$WORKSPACE/app/design-iteration-4"
LOG="$D/pipeline.log"

log() { echo "[$(date '+%H:%M:%S')] $1" | tee -a "$LOG"; }
log "═══ BREZI ITERATION 4 — Wellness-First (3 rounds, 100K tokens) ═══"

BRIEF=$(cat "$D/brief.md")
BEANS=$(cat "$WORKSPACE/app/roaster-beans/beans.md")
TECH=$(cat "$WORKSPACE/app/jensen-tech-spec.md")

# ── ROUND FUNCTIONS ──────────────────────────────────────────────────────────

run_picasso() {
  local ROUND=$1
  local PR=$((ROUND-1))
  local PREV_P="" PREV_E="" PREV_D=""
  [ $ROUND -gt 1 ] && PREV_P=$(cat "$D/round${PR}-picasso.md" 2>/dev/null)
  [ $ROUND -gt 1 ] && PREV_E=$(cat "$D/round${PR}-elon.md" 2>/dev/null)
  [ $ROUND -gt 1 ] && PREV_D=$(cat "$D/round${PR}-discussion.md" 2>/dev/null)

  local PROMPT="You are Picasso — world-class product designer. Ex-Apple Watch, Oura, WHOOP and specialty coffee brand design.

LOCKED ARCHITECTURE:
$BRIEF

REAL BEAN DATA:
$BEANS

PREV PICASSO DESIGN:
$PREV_P

ELON CRITIQUE:
$PREV_E

DISCUSSION:
$PREV_D

ROUND $ROUND — Design the complete simplified wellness-first app.

REQUIRED IN SPEC:
1. Every Home screen state (pre-brew / brewing / post-brew / evening)
2. Brew Card — exact layout: profile name, tagline, dose +/− with live caffeine, brew window bar, START BREW, Advanced toggle
3. Caffeine Countdown Timer card — all states (counting to peak / at peak / counting to clear / cleared)
4. Check-in card — layout, 3 pill options, dismiss
5. Bottom tab bar — macOS Dock style, pill indicator, all 4 tabs
6. Beans tab — full design (For You section, 2-col browse grid, bean detail)
7. History tab — brew log + pattern cards
8. Settings screen — wearable connect, wake time, caffeine cutoff goal
9. Onboarding — 4 screens max, under 2 min to first brew
10. ALL exact copy strings for every element
11. ALL component specs: sizes, spacing, typography, colors, corner radii
12. ALL animations: dose tap feedback, timer counting, caffeine card fade-in

Write complete spec to: $D/round${ROUND}-picasso.md"

  echo "$PROMPT" | claude --permission-mode bypassPermissions --print > "$D/r${ROUND}-plog.txt" 2>&1
}

run_elon() {
  local ROUND=$1
  local P=$(cat "$D/round${ROUND}-picasso.md" 2>/dev/null)

  local PROMPT="You are Elon — product COO. Wearable product background (WHOOP, Oura). Specialty coffee knowledge.
Lens: would a wellness-first user (WHOOP user, 28-38yo, tracks HRV/sleep) use this app daily?

PICASSO ROUND $ROUND:
$P

LOCKED BRIEF:
$BRIEF

Critique:
1. BREW CARD — max 3 taps to START BREW? Dose +/− natural? Caffeine estimate trusted?
2. CAFFEINE TIMER — genuinely useful for wellness user? Would you check this before bed?
3. CHECK-IN — lightweight? Feels optional, not obligatory?
4. TAB BAR — Dock style premium? 4 tabs obviously named?
5. BEANS TAB — truly non-blocking? Separate world feel?
6. HISTORY — pattern cards feel personal, not generic?
7. ONBOARDING — under 2 min? No unnecessary friction?
8. COPY — warm and direct, not clinical or coffee-nerdy?
9. OVERALL — wellness app that happens to have coffee, or coffee app with wellness features?

Top 3 non-negotiable fixes for next round.
Write to: $D/round${ROUND}-elon.md"

  echo "$PROMPT" | claude --permission-mode bypassPermissions --print > "$D/r${ROUND}-elog.txt" 2>&1
}

run_discussion() {
  local ROUND=$1
  local P=$(cat "$D/round${ROUND}-picasso.md" 2>/dev/null)
  local E=$(cat "$D/round${ROUND}-elon.md" 2>/dev/null)
  local NR=$((ROUND+1))

  local PROMPT="You are Picasso. Elon reviewed round $ROUND.

YOUR DESIGN:
$P

ELON CRITIQUE:
$E

Respond as confident designer. Agree where valid, push back with rationale where you disagree.
State exactly: what changes in round $NR, what stays and why.
Write to: $D/round${ROUND}-discussion.md"

  echo "$PROMPT" | claude --permission-mode bypassPermissions --print > "$D/r${ROUND}-dlog.txt" 2>&1
}

# ── 3 ROUNDS ─────────────────────────────────────────────────────────────────
for ROUND in 1 2 3; do
  log "ROUND $ROUND — Picasso"
  run_picasso $ROUND
  LINES=$(wc -l < "$D/round${ROUND}-picasso.md" 2>/dev/null | tr -d ' ')
  log "Picasso round $ROUND done ($LINES lines)"

  log "ROUND $ROUND — Elon"
  run_elon $ROUND
  log "Elon round $ROUND done"

  if [ $ROUND -lt 3 ]; then
    log "ROUND $ROUND — Discussion"
    run_discussion $ROUND
    log "Discussion round $ROUND done"
  fi
done

# ── JENSEN BUILD ──────────────────────────────────────────────────────────────
log "JENSEN — Building v5"
FINAL=$(cat "$D/round3-picasso.md" 2>/dev/null)
R3E=$(cat "$D/round3-elon.md" 2>/dev/null)

JENSEN_PROMPT="You are Jensen, senior iOS engineer (SwiftUI, iOS 16+, Core Data, Combine).

PROJECT: $APP_DIR
TECH SPEC: $TECH
BEAN DATA: $BEANS
FINAL DESIGN: $FINAL
ELON NOTES: $R3E
BRIEF: $BRIEF

Build the simplified wellness-first app. This is a significant simplification.
Key principle: max 3 taps to brew. Wellness user, not coffee nerd.

REQUIRED NEW/REPLACED FILES:

1. MainTabView.swift — 4-tab bottom dock
   - TabBar with icons+labels, #488BED pill indicator
   - Tabs: HomeView / BeansTabView / HistoryView / SettingsView

2. HomeView.swift — smart state machine
   Uses AppState.brewState: .idle / .brewing / .postBrew / .cleared
   .idle: RecoveryCard + BrewRecommendationCard + START BREW
   .brewing: BrewTimerCard (live mm:ss) + END BREW
   .postBrew: CaffeineTimerCard + CheckInCard (if not dismissed)
   .cleared: SleepReadyCard

3. BrewRecommendationCard.swift
   - Profile name (32pt bold, amber #D4A96A)
   - Tagline (16pt, secondary)
   - Dose row: minus button / dose label (e.g. '60g · medium strength') / plus button
   - Live caffeine: '~120mg' (dose * 2.0, updates on tap)
   - BrewWindowBar: GeometryReader horizontal bar, green=window, grey=post-cutoff, text='Best before 2pm'
   - START BREW button (full width, #488BED background, white text, 56pt height, 14pt corner)
   - DisclosureGroup 'Advanced': grind + temp + ratio

4. CaffeineTimerCard.swift
   - brewStartTime, peakTime (start+45m), halfLifeTime (start+5h), sleepSafeTime (start+25h)
   - Timer updates every 60s
   - States: 'Peak in Xm' / 'At peak — stay sharp' / 'Half-life at X:XX PM' / 'Sleep-safe by X:XX PM'
   - Amber left border, dismiss X

5. CheckInCard.swift
   - 'How did today's brew land?'
   - 3 HStack pill buttons: '👍 Right' / '➕ Needed more' / '➖ Too light'
   - Skip button below
   - On tap: save to BrewLogEntity.rating (1/2/3), set appState.checkInDismissed = true

6. AppState.swift additions:
   enum BrewState { case idle, brewing, postBrew, cleared }
   var brewState: BrewState
   var brewStartTime: Date?
   var currentDose: Double (defaults from profile, ±5g per tap, min 20g max 120g)
   var estimatedCaffeine: Double (currentDose * 2.0)
   var checkInDismissed: Bool
   func startBrew() — sets brewState = .brewing, brewStartTime = Date()
   func endBrew() — sets brewState = .postBrew
   func dismissCheckIn() — sets checkInDismissed = true
   func clearCaffeine() — sets brewState = .cleared (called when sleepSafeTime passes)

7. BeansTabView.swift (warm cream #F5F0E8 background)
   - 'For You Today' horizontal scroll: beans matching currentProfileKey
   - 'Browse All' 2-column LazyVGrid: BeanCardCompactView
   - NavigationLink to BeanDetailView

8. Updated Onboarding — 4 screens:
   Screen 1: 'Cold brew. Dialed in.' + tagline
   Screen 2: 'How do you track recovery?' (WHOOP / Oura / Apple Watch / Manual)
   Screen 3: 'Set your mornings' (wake time picker + caffeine cutoff goal picker)
   Screen 4: 'Your first brew awaits.' + GET STARTED button

Fix all previous compile errors. Keep .bak files.

Write summary to: $D/jensen-v5-summary.md"

echo "$JENSEN_PROMPT" | claude --permission-mode bypassPermissions --print > "$D/jensen-v5-log.txt" 2>&1
log "Jensen done. Swift files: $(find $APP_DIR/Brezi -name '*.swift' | grep -v bak | wc -l | tr -d ' ')"
log "═══ ITERATION 4 COMPLETE ═══"

openclaw system event --text "Brezi v5 done — wellness-first, 3-tap flow, caffeine timer, dock nav. Ready for Jackson." --mode now
