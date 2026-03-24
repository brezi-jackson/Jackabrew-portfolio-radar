#!/bin/bash
# Brezi App — Iteration 4: Simplified Wellness-First Architecture
# 3 rounds Picasso × Elon → Jensen builds
# Token limit raised from the start to prevent truncation

export CLAUDE_CODE_MAX_OUTPUT_TOKENS=100000

WORKSPACE="/Users/jackson/.openclaw/workspace/brezi"
APP_DIR="$WORKSPACE/ios-app"
D="$WORKSPACE/app/design-iteration-4"
BEANS=$(cat "$WORKSPACE/app/roaster-beans/beans.md")
LOG="$D/pipeline.log"

mkdir -p "$D"
log() { echo "[$(date '+%H:%M:%S')] $1" | tee -a "$LOG"; }
log "═══ BREZI ITERATION 4 — Simplified Wellness-First (3 rounds, token limit: 100K) ═══"

LOCKED_BRIEF='
## LOCKED ARCHITECTURE — Iteration 4

### Who This Is For
Wellness people who happen to drink cold brew. NOT specialty coffee enthusiasts.
WHOOP users. Oura users. People who track HRV and sleep. They want to feel good, not become baristas.

### Core Daily Flow (FINAL — do not add steps)
1. Recovery score screen (techy, WHOOP-energy: HRV, sleep hours, recovery %)
2. Why Today screen (emotional bridge — warm, 1–2 sentences for the day)
3. Brew card screen (the heart — see below)
4. START BREW → brew timer on Home
5. END BREW → Home shows caffeine countdown timer (live, persistent card)
6. Optional "How did it land?" check-in card (dismissible, 3 taps)

### THE BREW CARD (Screen 3 — most important screen)
This is NOT a settings screen. It is a recommendation card.
Shows:
- Profile name in large type: "First Light" / "Tide Pool" / "Deep Work" / "Voltage"
- A one-line emotional description: "Your everyday standard."
- Dose control: [−] 60g [+] with label "(medium strength)" — updates caffeine estimate live
- Estimated caffeine: "~120mg" — updates as dose changes
- Brew window visual: simple arc or bar showing 6am–2pm, "Best before 2pm"
- ONE button: START BREW

Advanced toggle (collapsed by default, tap to reveal):
- Grind setting
- Temperature (cold is default — shown as "Cold / 4°C")
- Ratio details
Users who want this can find it. Everyone else never sees it.

NO bean selection in this flow. Bean = optional, lives in Beans tab.
NO temp shown by default.
NO ratio shown by default.
NO roast level shown by default.

### Caffeine Countdown Timer (Post-brew persistent card on Home)
After END BREW, Home shows a live updating card:
- "⚡ Peak in 38 min" → counts down to peak
- "½ life at 1:15 PM" → static based on brew time
- "Sleep-safe by 7:00 PM 🌙" → static based on brew time + 5 half-lives
Card fades/clears automatically when caffeine is sleep-safe.
This is a WELLNESS feature. Users obsess over sleep quality. Show them exactly when coffee stops competing with their recovery.

### Optional Check-In Card
Appears below caffeine timer after brew ends. Fully dismissible.
Copy: "How did today's brew land?"
3 options (not stars): 👍 Right · ➕ Needed more · ➖ Too light
One tap, done. Never required. Card disappears after rating or dismiss.
Over time: app shows "You rate First Light highest on 65–75% recovery days" in History.

### Home Screen States
| State | Content |
|-------|---------|
| Morning (pre-brew) | Recovery score summary + Brew card + START BREW |
| Brewing | Live brew timer + END BREW button + brew card summary |
| Post-brew | Caffeine countdown card + optional check-in card |
| Evening/cleared | "Ready for tomorrow." + sleep readiness note |

### Bottom Tab Navigation (macOS Dock style)
4 tabs with icon + label. Clean pill/capsule selection indicator.
1. 🏠 Home — daily flow
2. ☕ Beans — discover, browse, buy (non-blocking, separate world)
3. 📊 History — brew log + patterns
4. ⚙️ Settings — wearable connection, preferences

### Beans Tab (SEPARATE from main flow — non-blocking)
Beautiful. Exploratory. Fun. Does NOT interrupt morning routine.
- Featured section: beans recommended for today'"'"'s profile
- Browse all: compact cards (like image 2 from Jackson — side by side compact grid)
- Bean detail: full card with tasting notes, origin story, flavor sliders, Add to Cart (in-app web view), Subscribe & Save
- User can optionally "set as my bean" — quietly influences recommendations but never required
- 3 real beans: Stitch Dark Field / Regent Ethiopia Guji Hambela / Corvus Sadayana Winey

### History Tab
- Brew log (date, profile, dose, rating)
- Pattern cards: "You rate First Light highest on 65–75% recovery days"
- Correlation: Recovery score vs brew rating over time
- "Your history with this bean" — per-bean personal stats

### Colors
Health/recovery side: #000000 / #0F0F0F background, #488BED blue accent — techy, precise
Coffee/brew side: same dark base but warm amber (#D4A96A) accents for profile name, brew card
Beans tab: warm cream (#F5F0E8) background — distinct world, cheerful, illustrative
Caffeine timer card: dark card with amber/yellow accent
Check-in card: minimal, cream, 3 pill buttons

### Copy Tone
Health side: precise, data-forward ("68% recovery · HRV 62ms · 7h 20m sleep")
Brew card: warm but direct ("Your everyday standard." / "~120mg")
Caffeine timer: useful, not alarming ("Sleep-safe by 7:00 PM")
Check-in: casual ("How did today'"'"'s brew land?")
Beans tab: story-forward, specialty coffee brand energy

### What NOT to do
- No temperature on the main brew card (Advanced only)
- No bean selection as a required step
- No dose in g/L jargon without translation (always add "medium strength" / "strong" / "light")
- No more than 3 taps from opening app to START BREW
- No cluttered bean detail cards in the main flow

### 3 Real Beans (Beans tab only)
'"$BEANS"'
'

picasso_prompt() {
  local ROUND=$1; local PREV_P=$2; local PREV_E=$3; local PREV_D=$4
  cat << PROMPT
You are Picasso — world-class product designer. Ex-Apple Watch, Oura, WHOOP and specialty coffee brand design.
You understand both health data interfaces AND the visual language of modern wellness apps.

LOCKED ARCHITECTURE (do not deviate from this):
$LOCKED_BRIEF

$([ -n "$PREV_P" ] && echo "YOUR PREVIOUS ROUND DESIGN:" && echo "$PREV_P")
$([ -n "$PREV_E" ] && echo "ELON'S CRITIQUE:" && echo "$PREV_E")
$([ -n "$PREV_D" ] && echo "DISCUSSION:" && echo "$PREV_D")

ROUND $ROUND — Design the complete app to this new simplified architecture.

REQUIRED IN YOUR SPEC:
1. Every screen, every state (pre-brew / brewing / post-brew / evening)
2. The Brew Card — exact layout, dose +/− interaction, live caffeine update, START BREW
3. Caffeine countdown timer card — layout, copy, states (counting to peak / at peak / counting to clear / cleared)
4. Check-in card — layout, 3 options, dismiss interaction
5. Bottom tab bar — exact design (macOS Dock style, pill indicator)
6. Beans tab — full design (featured, browse grid, bean detail with in-app purchase)
7. History tab — brew log + pattern cards
8. Settings screen
9. All exact copy strings (every label, button, heading, subheading, empty states)
10. All component specs: sizes, spacing, typography, colors, corner radii
11. All animations and transitions (palette crossfade, dose +/− feedback, caffeine card appearance)
12. Onboarding — 4 screens maximum, gets to first brew in under 2 minutes

Write the complete exhaustive spec to: $D/round${ROUND}-picasso.md
PROMPT
}

elon_prompt() {
  local ROUND=$1; local P=$2
  cat << PROMPT
You are Elon — product COO with wearable product experience (WHOOP, Oura, Apple Watch) and specialty coffee knowledge.
Your lens: would a wellness-first user (WHOOP user, 28-38yo, tracks sleep and HRV) use this app daily?

PICASSO'S ROUND $ROUND DESIGN:
$P

LOCKED ARCHITECTURE:
$LOCKED_BRIEF

Critique from a wellness user + product perspective:

1. BREW CARD — Is this genuinely simple? Max 3 taps from open to START BREW? Does dose +/− feel natural?
2. CAFFEINE TIMER — Is it genuinely useful for a wellness user? Would someone check this before bed?
3. CHECK-IN — Lightweight enough? Does it feel optional or obligatory?
4. TAB BAR — Does the Dock-style nav feel premium? Are the 4 tabs obviously named?
5. BEANS TAB — Does it feel like a separate world (not interrupting daily flow)? Would you browse it on a Sunday?
6. HISTORY — Does the pattern recognition feel personal or generic?
7. ONBOARDING — Under 2 minutes to first brew? No unnecessary friction?
8. COPY — Is it warm and direct, not clinical or coffee-nerdy?
9. OVERALL — Does this feel like the wellness app it should be, or does it still feel like a coffee app?

Top 3 non-negotiable fixes for next round.
Write to: $D/round${ROUND}-elon.md
PROMPT
}

discuss_prompt() {
  local ROUND=$1; local P=$2; local E=$3
  cat << PROMPT
You are Picasso. Elon reviewed round $ROUND.

YOUR DESIGN: $P
ELON'S CRITIQUE: $E

Respond as a confident designer. Agree where valid, push back where you have strong design rationale.
State exactly: what changes in round $((ROUND+1)), what stays and why.
Write to: $D/round${ROUND}-discussion.md
PROMPT
}

# ── 3 ROUNDS ─────────────────────────────────────────────────────────────────
for ROUND in 1 2 3; do
  PR=$((ROUND-1))
  PREV_P=$([ $ROUND -gt 1 ] && cat "$D/round${PR}-picasso.md" 2>/dev/null || echo "")
  PREV_E=$([ $ROUND -gt 1 ] && cat "$D/round${PR}-elon.md" 2>/dev/null || echo "")
  PREV_D=$([ $ROUND -gt 1 ] && cat "$D/round${PR}-discussion.md" 2>/dev/null || echo "")

  log "ROUND $ROUND — Picasso (token limit: $CLAUDE_CODE_MAX_OUTPUT_TOKENS)"
  claude --permission-mode bypassPermissions --print "$(picasso_prompt $ROUND "$PREV_P" "$PREV_E" "$PREV_D")" \
    > "$D/r${ROUND}-picasso-log.txt" 2>&1
  LINES=$(wc -l < "$D/round${ROUND}-picasso.md" 2>/dev/null || echo 0)
  log "Picasso round $ROUND done ($LINES lines)"

  log "ROUND $ROUND — Elon"
  P_OUT=$(cat "$D/round${ROUND}-picasso.md" 2>/dev/null || echo "")
  claude --permission-mode bypassPermissions --print "$(elon_prompt $ROUND "$P_OUT")" \
    > "$D/r${ROUND}-elon-log.txt" 2>&1
  log "Elon round $ROUND done"

  if [ $ROUND -lt 3 ]; then
    log "ROUND $ROUND — Discussion"
    E_OUT=$(cat "$D/round${ROUND}-elon.md" 2>/dev/null || echo "")
    claude --permission-mode bypassPermissions --print "$(discuss_prompt $ROUND "$P_OUT" "$E_OUT")" \
      > "$D/r${ROUND}-discuss-log.txt" 2>&1
    log "Discussion round $ROUND done"
  fi
done

# ── JENSEN BUILD ──────────────────────────────────────────────────────────────
log "JENSEN — Building v5 (wellness-first, simplified architecture)"

FINAL=$(cat "$D/round3-picasso.md" 2>/dev/null || echo "")
R3E=$(cat "$D/round3-elon.md" 2>/dev/null || echo "")

claude --permission-mode bypassPermissions --print "
You are Jensen, senior iOS engineer (SwiftUI, iOS 16+, Core Data, Combine).

PROJECT: $APP_DIR
FINAL DESIGN: $FINAL
ELON ROUND 3 NOTES: $R3E
BEAN DATA: $BEANS

This is a significant architecture simplification. Build from scratch on top of the existing project.
Key principle: wellness user, not coffee nerd. Max 3 taps to brew.

REQUIRED — NEW/REPLACED SCREENS:

1. HomeView.swift — Smart state machine:
   State A (pre-brew): RecoveryCard (score + HRV + sleep) + BrewRecommendationCard + START BREW
   State B (brewing): BrewTimerCard (live timer, dose summary) + END BREW button
   State C (post-brew): CaffeineTimerCard (live countdown) + CheckInCard (dismissible)
   State D (evening/cleared): SleepReadyCard ('Caffeine cleared. Recovery window open.')

2. BrewRecommendationCard.swift — the new core card:
   - Profile name (large, e.g. 'First Light')
   - Tagline ('Your everyday standard.')
   - Dose row: [−] [60g] [+] with strength label ('medium strength') — updates CaffeineEstimateView
   - CaffeineEstimateView: '~120mg' updates live as dose changes (simple linear model: dose_g × 2.0)
   - BrewWindowView: horizontal bar, green=brew window, marker=cutoff time, label='Best before 2pm'
   - START BREW button (full width, bold)
   - Advanced toggle (chevron, collapsed): reveals grind + temp (cold/4°C default) + ratio

3. CaffeineTimerCard.swift — post-brew persistent card:
   - Brew start time stored in AppState
   - Caffeine half-life = 5 hours
   - Shows 3 lines, live updating:
     '⚡ Peak in [X min]' (peak = 45 min after brew start)
     '½ life at [time]' (brew_start + 5h)
     '🌙 Sleep-safe by [time]' (brew_start + 25h, i.e. 5 half-lives to <3% caffeine)
   - Amber/yellow accent color
   - Dismissible (X button top right)
   - Card removes itself when sleep-safe time passes

4. CheckInCard.swift — optional, dismissible:
   - 'How did today's brew land?'
   - 3 pill buttons: '👍 Right' · '➕ Needed more' · '➖ Too light'
   - 'Skip' link (small, below buttons)
   - Saves to Core Data BrewLogEntity.rating
   - Disappears after tap or skip

5. MainTabView.swift — bottom tab bar:
   - 4 tabs: Home (house.fill) / Beans (leaf.fill) / History (chart.bar.fill) / Settings (gearshape.fill)
   - macOS Dock style: icons + labels, rounded pill/capsule selection background
   - Dark background (#0F0F0F) with white icons, selected tab has #488BED pill

6. BeansTabView.swift — separate world, warm cream (#F5F0E8) background:
   - 'For You Today' section: compact cards of beans matching today's profile
   - 'Browse All' grid: 2-column compact BeanCardCompactView
   - Each card: Add to Cart (SFSafariViewController) + Subscribe & Save toggle
   - BeanDetailView accessible via tap: full card with flavor sliders, origin story, brew pairing

7. HistoryView.swift:
   - List of BrewLogEntity sorted by date
   - Pattern cards above list: 'You rate First Light highest on 65–75% recovery days' (after 5+ logs)
   - Per-bean history: 'Ethiopia Guji Hambela: 4 brews, avg rating 4.2/5'

8. AppState.swift updates:
   - brewState: BrewState enum (.idle / .brewing / .postBrew / .cleared)
   - brewStartTime: Date?
   - peakCaffeineTime: Date? (brewStartTime + 45min)
   - halfLifeTime: Date? (brewStartTime + 5h)
   - sleepSafeTime: Date? (brewStartTime + 25h)
   - currentDose: Double (default from profile, adjustable)
   - estimatedCaffeine: Double (currentDose × 2.0)
   - checkInDismissed: Bool
   - func startBrew()
   - func endBrew()
   - func dismissCheckIn()

9. Fix all compile errors from previous iterations. The .bak files should remain as backups.
   Remove any duplicate struct/class definitions.

10. Update onboarding to 4 screens max:
    Screen 1: 'Cold brew, dialed in.' + hero image placeholder
    Screen 2: 'Connect your recovery' (WHOOP / Oura / Apple Watch / I'll enter manually)
    Screen 3: 'Set your mornings' (wake time, caffeine cutoff goal)
    Screen 4: 'Your first brew awaits' → GO

Write build summary to: $D/jensen-v5-summary.md
Then: openclaw system event --text 'Brezi v5 built — simplified wellness-first architecture. Ready for Jackson to test.' --mode now
" > "$D/jensen-v5-log.txt" 2>&1

log "Jensen done. Swift files: $(find $APP_DIR/Brezi -name '*.swift' | grep -v '\.bak' | wc -l | tr -d ' ')"
log "═══ ITERATION 4 COMPLETE ═══"

openclaw system event --text "Brezi iOS v5 done — wellness-first, 3-tap flow, caffeine timer, dock nav. Ping Jackson." --mode now
