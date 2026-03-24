#!/bin/bash
# Brezi App — Iteration 3: Resume from Round 2 (Round 1 Picasso already done)
# All 5 decisions locked + 3 additions from Jackson

WORKSPACE="/Users/jackson/.openclaw/workspace/brezi"
APP_DIR="$WORKSPACE/ios-app"
D="$WORKSPACE/app/design-iteration-3"
BEANS="$WORKSPACE/app/roaster-beans/beans.md"
LOG="$D/pipeline-r2.log"

log() { echo "[$(date '+%H:%M:%S')] $1" | tee -a "$LOG"; }
log "═══ BREZI ITERATION 3 (ROUND 2→5) — Locked Decisions ═══"

R1_PICASSO=$(cat "$D/round1-picasso.md")
CMO=$(cat "$WORKSPACE/app/cmo-ux-copy-brief.md")
TECH=$(cat "$WORKSPACE/app/jensen-tech-spec.md")
BEAN_DATA=$(cat "$BEANS")

JACKSON_BRIEF="
## Jackson's Locked Decisions (all final, no further questions needed)

### 3 Confirmed Beans (corrected)
$BEAN_DATA

### Locked Answers:
1. Corvus bean = Sadayana Winey (Winey Anaerobic Natural Ateng, West Java — raspberry yogurt / cherry syrup / dark chocolate)
2. Add to Cart = IN-APP WEB VIEW (SafariViewController / SFSafariViewController — user stays in app)
3. Subscribe & Save = DESIGN IT FULLY — will be live at launch, not coming soon
4. Flow = Health data → Why Today screen (emotional bridge) → Bean selection → Brew settings
   - Why Today + Bean Selection are the EMOTIONAL half of the app. Warm palette, illustrative, personal
5. General Profile = EQUAL CARD alongside beans — 'No specific bean today' as a first-class option

### New Features to Implement:
- 'Your history with this bean' — after 3+ brews with a bean, show personal brew history card:
  'You've brewed this 3×. Best rated on recovery 7+.' Drives repeat purchase naturally.
- Palette transition as BRAND MOMENT — the shift from dark health data → warm coffee world
  is the core brand story compressed into a gesture: 'This is your body. This is what it deserves.'
  Design this transition screen as a hero moment — not just navigation.
- Push notifications = TABLE FOR NOW (design app first, notifications later)

### Full App Flow:
Screen 1: Health data read (dark, techy, WHOOP-like — recovery score, HRV, sleep)
Screen 2: Why Today (EMOTIONAL BRIDGE — warm, illustrative — 'You're at 62% today. Rebuilding. Your nervous system is doing the heavy lifting.')
Screen 3: Bean selection (WARM, CHEERFUL — 3 recommended bean cards + 1 'General Profile' equal card)
  - Each bean card: roaster art style, flavor sliders (Roast/Sweetness/Acidity), tasting note icons, price, 'Add to Cart' (in-app web view), 'Recommended for you' tag
Screen 4: Brew settings (adapts to selected bean — shows 'Optimized for Sadayana Winey' or 'General Voltage Settings')
Screen 5: Brew confirmation / timer
Screen 6: Post-brew check-in (emotional rating — Hit right / Needed more / Too much today)

### Marketplace / Bean Shop:
- Tab or accessible sheet from bean selection screen
- Filter by brew profile
- Subscribe & Save option on each bean card
- 'Your history with this bean' card when user has 3+ brews logged with that bean

### Visual Language:
HEALTH SIDE (#000/#0F0F0F, blue #488BED): precise, data-forward, WHOOP energy
COFFEE SIDE (cream #F5F0E8, amber #D4A96A, warm white): cheerful, illustrative, specialty coffee brand energy

Flavor sliders per Stitch Coffee style: horizontal bars for Roast / Sweetness / Acidity
Tasting note icons: flat illustration style
Serving infographic: clock arc for cutoff time, cup icons for serving size

### Bean Packaging Aesthetics (must feel at home next to these brands):
- Stitch: illustrated bear in Fiat 500 — playful, indie, bold
- Regent: art deco line illustration — craft spirits, elegant, earthy
- Corvus Sadayana Winey: gold temple mandala on black — ceremony, premium, weight
"

elon_critique_r1() {
echo "
You are Elon — product COO, ex-wearable products (WHOOP, Oura), specialty coffee obsessive.

ROUND 1 PICASSO DESIGN (1935 lines):
$R1_PICASSO

JACKSON'S LOCKED BRIEF:
$JACKSON_BRIEF

Critique round 1 from product + user perspective. Be specific and direct.

1. PALETTE TRANSITION — Does round 1 treat the health→coffee shift as a brand hero moment? Or just navigation? If not, demand it.
2. WHY TODAY + BEAN SELECTION — Are these warm/emotional enough? Does Why Today set up the bean selection emotionally?
3. BEAN CARDS — Are the 3 beans (Stitch Dark Field / Regent Hambela / Corvus Sadayana Winey) visually distinct from each other? Do they honor each roaster's aesthetic?
4. GENERAL PROFILE CARD — Is it an equal first-class option alongside the beans?
5. FLAVOR SLIDERS — Fast to scan? Under 3 seconds to understand a bean's character?
6. IN-APP WEB VIEW — Is the Add to Cart experience designed for the right moment? Not disruptive?
7. YOUR HISTORY — Is the bean history feature natural and personal (not data-clinical)?
8. SUBSCRIBE & SAVE — Is it designed as a real feature, not an afterthought?
9. SERVING INFOGRAPHIC — Clock arc + cup icons: intuitive without explanation?
10. OVERALL — Does this feel like a specialty coffee brand that happens to be smart, or a fitness app that added coffee?

End with: 3 non-negotiable fixes for round 2.
Write to: $D/round1-elon.md
"
}

picasso_round() {
  local ROUND=$1; local PREV_P=$2; local PREV_E=$3; local PREV_D=$4
  echo "
You are Picasso — world-class product designer. Ex-Apple Watch, Oura, WHOOP AND specialty coffee brand design.

JACKSON'S LOCKED BRIEF:
$JACKSON_BRIEF

PREVIOUS ROUND DESIGN:
$PREV_P

ELON'S CRITIQUE:
$PREV_E

DISCUSSION:
$PREV_D

ROUND $ROUND — Refine the design. Address every critique. Keep what's working.

Must include:
- Complete screen-by-screen specs for all 6 screens
- The palette transition as a hero brand moment (not just navigation)
- Bean cards with distinct visual personality per roaster
- Flavor sliders + tasting note icons
- Serving infographic (clock arc + cup icons)
- Your history with this bean card
- Subscribe & Save fully designed
- General Profile as equal card
- Add to Cart → in-app web view
- All copy (exact strings for every label, button, heading, subheading)
- All component specs (sizes, spacing, typography, colors)

Write complete spec to: $D/round${ROUND}-picasso.md
"
}

elon_round() {
  local ROUND=$1; local P=$2
  echo "
You are Elon — product COO.

PICASSO'S ROUND $ROUND DESIGN:
$P

JACKSON'S LOCKED BRIEF:
$JACKSON_BRIEF

Critique. Be direct.
1. Palette transition hero moment — nailed or still weak?
2. Bean cards — Stitch/Regent/Corvus visually distinct and brand-honoring?
3. Why Today emotional bridge — landing or generic?
4. Flavor visualization — under 3-second scan?
5. History card — personal and warm?
6. Marketplace + Subscribe & Save — would you actually buy here?
7. Overall brand feel — specialty coffee brand that happens to be smart?

End with: top 3 fixes for next round.
Write to: $D/round${ROUND}-elon.md
"
}

discuss_round() {
  local ROUND=$1; local P=$2; local E=$3
  echo "
You are Picasso. Elon reviewed round $ROUND.

DESIGN: $P
CRITIQUE: $E

Respond as a confident designer. Agree where valid, push back where you have strong rationale.
State exactly: what changes in round $((ROUND+1)), what stays and why.
Write to: $D/round${ROUND}-discussion.md
"
}

# ── ROUND 1 ELON (critique of already-completed round 1 picasso) ─────────────
log "ROUND 1 — Elon critique"
claude --permission-mode bypassPermissions --print "$(elon_critique_r1)" > "$D/r1-elon-log.txt" 2>&1
log "Elon round 1 done"

# ── ROUNDS 2–5 ───────────────────────────────────────────────────────────────
for ROUND in 2 3 4 5; do
  PR=$((ROUND-1))
  PREV_P=$(cat "$D/round${PR}-picasso.md" 2>/dev/null || echo "")
  PREV_E=$(cat "$D/round${PR}-elon.md" 2>/dev/null || echo "")
  PREV_D=$(cat "$D/round${PR}-discussion.md" 2>/dev/null || echo "")

  log "ROUND $ROUND — Picasso"
  claude --permission-mode bypassPermissions --print "$(picasso_round $ROUND "$PREV_P" "$PREV_E" "$PREV_D")" > "$D/r${ROUND}-picasso-log.txt" 2>&1
  log "Picasso round $ROUND done ($(wc -l < $D/round${ROUND}-picasso.md 2>/dev/null || echo 0) lines)"

  log "ROUND $ROUND — Elon"
  P_OUT=$(cat "$D/round${ROUND}-picasso.md")
  claude --permission-mode bypassPermissions --print "$(elon_round $ROUND "$P_OUT")" > "$D/r${ROUND}-elon-log.txt" 2>&1
  log "Elon round $ROUND done"

  if [ $ROUND -lt 5 ]; then
    log "ROUND $ROUND — Discussion"
    E_OUT=$(cat "$D/round${ROUND}-elon.md")
    claude --permission-mode bypassPermissions --print "$(discuss_round $ROUND "$P_OUT" "$E_OUT")" > "$D/r${ROUND}-discuss-log.txt" 2>&1
    log "Discussion round $ROUND done"
  fi
done

# ── FINAL SIGN-OFF ─────────────────────────────────────────────────────────
log "ELON — Final sign-off"
FINAL=$(cat "$D/round5-picasso.md")
FINAL_E=$(cat "$D/round5-elon.md")

claude --permission-mode bypassPermissions --print "
You are Elon. Final sign-off on iteration 3.

FINAL DESIGN: $FINAL
ROUND 5 CRITIQUE: $FINAL_E

Write sign-off:
1. Ready to build verdict
2. Micro-fixes for Jensen while coding
3. New data model fields needed (BeanSeed additions: roasterName, price, shopifyUrl, roastScore, sweetnessScore, acidityScore, tastingNoteIcons, bagImageName, brewHistoryCount)
4. Priority build order
5. Exact Shopify product URLs for the 3 beans

Write to: $D/final-signoff.md
" > "$D/signoff-log.txt" 2>&1
log "Sign-off done"

# ── JENSEN ─────────────────────────────────────────────────────────────────
log "JENSEN — Building v4"
SIGNOFF=$(cat "$D/final-signoff.md")

claude --permission-mode bypassPermissions --print "
You are Jensen, iOS developer (SwiftUI, iOS 16+, Core Data).

PROJECT: $APP_DIR
FINAL DESIGN SPEC: $FINAL
SIGN-OFF: $SIGNOFF
TECH SPEC: $(cat $WORKSPACE/app/jensen-tech-spec.md)
BEAN DATA: $BEAN_DATA

Implement all of the following. Do not skip anything.

NEW SCREENS:
1. BeanSelectionView — bean-first screen between Why Today and brew settings
   - 3 bean cards (Stitch Dark Field, Regent Hambela, Corvus Sadayana Winey) + 1 General Profile equal card
   - Warm palette (#F5F0E8 background, amber accents)
   - 'Recommended for you' tag on beans matching today's profile
   
2. Updated BeanCardView — full visual treatment:
   - Roaster name badge
   - Bag image placeholder (AsyncImage-ready for CDN)
   - Flavor sliders: Roast / Sweetness / Acidity (0–100, SwiftUI GeometryReader bars)
   - Tasting note icons (SF Symbols as placeholder, 3 per bean)
   - Origin badge (country flag emoji + region name)
   - Price label
   - 'Add to Cart' → SFSafariViewController opening Shopify URL
   - 'Subscribe & Save' toggle/button
   
3. BeanHistoryCard — shown on bean card after 3+ brews logged with that bean:
   'You've brewed this Nx. Best rated on recovery 7+.'
   Pulls from BrewLogEntity, filters by beanName
   
4. PaletteTransitionView — hero brand moment between health data and Why Today:
   Full screen. Dark fades/morphs to warm cream.
   Copy: 'This is your body. This is what it deserves.'
   2-second auto-advance or tap to continue.
   
5. ServingInfographicView — SwiftUI Canvas/Path:
   - Clock arc showing cutoff time window (e.g. colored arc from 6am to 2pm)
   - Cup icon(s) for serving size
   - Replaces text-only serving info
   
6. Update BrewSettingsView header:
   - When bean selected: 'Optimized for [Bean Name]' + roaster badge
   - When general: 'General [Profile Name] Settings'
   
7. Updated BeanData model — add fields:
   roasterName: String
   price: String
   shopifyUrl: String
   roastScore: Int (0-100)
   sweetnessScore: Int (0-100)
   acidityScore: Int (0-100)
   tastingNoteIcons: [String] (SF Symbol names)
   bagImageUrl: String (CDN URL, empty for now)
   
8. Seed all 3 beans with real data from the brief above.

9. Update AppState to hold selectedBean: BeanSeed? and pass through flow.

10. Fix any remaining compile errors from previous iteration.

When done write a summary to: $D/jensen-summary.md
Then: openclaw system event --text 'Brezi v4 build complete — visual coffee + marketplace + bean-first flow. Ready for Jackson.' --mode now
" > "$D/jensen-log.txt" 2>&1

log "Jensen done. Swift files: $(find $APP_DIR/Brezi -name '*.swift' | wc -l | tr -d ' ')"
log "═══ ITERATION 3 PIPELINE COMPLETE ═══"

openclaw system event --text "Brezi iOS v4 done — visual coffee layer, bean-first flow, marketplace, palette transition. Ping Jackson." --mode now
