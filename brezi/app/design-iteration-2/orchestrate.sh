#!/bin/bash
# Brezi App — Iteration 2: Emotional Coffee Layer
# Elon × Picasso × 5 rounds → Jensen implements

WORKSPACE="/Users/jackson/.openclaw/workspace/brezi"
APP_DIR="$WORKSPACE/ios-app"
D="$WORKSPACE/app/design-iteration-2"
D1="$WORKSPACE/app/design-iteration-1"
LOG="$D/pipeline.log"

log() { echo "[$(date '+%H:%M:%S')] $1" | tee -a "$LOG"; }

log "═══ BREZI ITERATION 2 — Emotional Coffee Layer ═══"

FINAL_V2=$(cat "$D1/round3-final-design.md")
CMO=$(cat "$WORKSPACE/app/cmo-ux-copy-brief.md")
TECH=$(cat "$WORKSPACE/app/jensen-tech-spec.md")

JACKSON_BRIEF="
## Jackson's Feedback on v2
The app is functional but has no soul. After choosing how I feel, I have no idea WHY this coffee relates to me. 

What's missing:
1. EMOTIONAL CONNECTION — the recommendation needs to feel personal and felt, not clinical. Not 'HRV: 68ms → Tide Pool (50g/L, 8h steep)'. More like: 'You're rebuilding. Your nervous system needs something soft. Ethiopia Yirgacheffe — bergamot, jasmine, low acidity. The kind of cup that doesn't demand anything from you.'

2. BEAN STORYTELLING — Different specialty coffee beans carry different emotional and sensory qualities. The app should recommend SPECIFIC BEANS matched to your state:
   - High energy/peaked: Bold, bright, high-complexity beans (Kenya AA, Ethiopian Sidama natural) — flavors: blackcurrant, stone fruit, electric finish
   - Recovery/rebuilding: Gentle, floral, low-acid beans (Ethiopian Yirgacheffe washed, Colombian Huila) — flavors: bergamot, jasmine, honey, soft
   - Focus/deep work: Clean, structured, nutty-chocolatey beans (Brazil Cerrado, Guatemala Antigua) — flavors: dark chocolate, walnut, caramel, no distraction
   - Rest day: Naturally sweet, low-caffeine-feeling beans (Honduras, Peru) — flavors: brown sugar, mild fruit, velvet body

3. FLAVOR AS EMOTIONAL LANGUAGE — flavor notes aren't just taste descriptors. 'Blackcurrant and citrus' means energy. 'Bergamot and honey' means ease. The app should translate flavor profiles into emotional states the user can feel BEFORE they brew.

4. THE RITUAL — The act of choosing and brewing should feel intentional. Not transactional. The recommendation screen should feel like a friend who knows your body telling you: 'today, this. because you.'

5. ORIGIN AS STORY — Each bean origin has character. Ethiopia = birthplace of coffee, complexity, emotion. Kenya = bold, performance. Colombia = balance, reliable warmth. Japan-processed = precision. The app should carry a sentence of origin story.

The whole point: health data tells you your STATE. Coffee should meet you EXACTLY THERE — not just functionally, but emotionally and sensorially.
"

picasso_prompt() {
  local ROUND=$1
  local PREV_DESIGN=$2
  local ELON_CRITIQUE=$3
  local DISCUSSION=$4

  echo "
You are Picasso — world-class product designer, Apple Watch, Oura, WHOOP background. You also deeply understand specialty coffee culture, sensory design, and how emotional UX works.

CURRENT APP DESIGN (v2):
$FINAL_V2

JACKSON'S CORE FEEDBACK:
$JACKSON_BRIEF

CMO TONE GUIDE:
$CMO

$([ -n "$PREV_DESIGN" ] && echo "YOUR PREVIOUS DESIGN (round $((ROUND-1))): $PREV_DESIGN")
$([ -n "$ELON_CRITIQUE" ] && echo "ELON'S CRITIQUE: $ELON_CRITIQUE")
$([ -n "$DISCUSSION" ] && echo "DISCUSSION CONCLUSION: $DISCUSSION")

ROUND $ROUND TASK:
Design the emotional layer of the Brezi app. The health data is already there. Now make the coffee recommendation feel like it was made for this exact person on this exact day.

Design:
1. EMOTIONAL RECOMMENDATION CARD — not 'Tide Pool (50g/L, 8h)' but an experience: name + one emotional sentence + flavor as feeling + brew settings secondary. Show why this feels right today — not just why it's correct.

2. BEAN SELECTION LAYER — For each brew profile, 2-3 specific specialty bean recommendations with:
   - Origin + processing method (e.g. 'Ethiopia Yirgacheffe — Washed')
   - Flavor notes written as feeling (not list: 'bergamot, jasmine, honey' but 'soft, floral, asks nothing of you')
   - Emotional tag ('for when you need ease' / 'for when you mean business' / 'for focus that doesn't spike')
   - Brew profile pairing explanation (1 sentence, felt not clinical)

3. 'WHY TODAY' SCREEN — after health data is read, before the recommendation, show a transitional moment:
   - Your state in one line ('You're at 62% today. Rebuilding.')
   - What your body needs ('Your nervous system is doing the heavy lifting. Give it ease.')
   - Then: coffee as response ('This is what meets you there →')
   - Transition animation into the recommendation

4. FLAVOR PREVIEW — before brewing, let the user feel what they're about to drink:
   - Sensory description (not just notes — texture, temperature feel, moment)
   - 'This cup is: [3 feeling words]' e.g. 'Soft. Grounding. Unhurried.'
   - Pair with a minimal abstract visual (dark gradient, color-coded to the profile: blue=focus, warm amber=rest, electric=voltage)

5. POST-BREW EMOTIONAL CHECK-IN — after brew is logged:
   - 'How did that land?' (not stars — feeling words: 'Hit right / Needed more / Too much today')
   - This feeds back into future recommendations ('On rest days you prefer floral over chocolatey')

6. ORIGIN STORY SNIPPET — 1-2 sentences on the bean's character and origin, written like a film logline, not a catalog entry.

Write to: $D/round${ROUND}-picasso.md
Be exhaustive on the emotional UX. Copy strings matter enormously here — write them all out.
"
}

elon_prompt() {
  local ROUND=$1
  local PICASSO_DESIGN=$2

  echo "
You are Elon — COO of Brezi. You understand the product, the customer, and the brand deeply.

PICASSO'S ROUND $ROUND DESIGN:
$PICASSO_DESIGN

JACKSON'S FEEDBACK TO ADDRESS:
$JACKSON_BRIEF

Critique from a product and emotional resonance perspective:

1. Does the recommendation now feel PERSONAL — like it was made for this person today? Or still clinical?
2. Is the bean storytelling specific enough? Generic 'floral notes' vs vivid sensory language?
3. Does the 'Why Today' transition feel earned or forced?
4. Is the flavor preview something a runner would actually pause and read, or skip?
5. Post-brew check-in — is 'Hit right / Needed more / Too much today' better than stars? Does it actually improve future recs?
6. Copy audit: find every line that sounds like a wellness app and rewrite it to sound like Brezi (sharp, second person, earned)
7. Does the emotional layer ADD to the data layer or fight with it?
8. One thing that could make this genuinely surprising to a user — something they wouldn't expect from a coffee app

End with: '3 must-fix things for round $((ROUND+1))'

Write to: $D/round${ROUND}-elon.md
"
}

discuss_prompt() {
  local ROUND=$1
  local PICASSO_DESIGN=$2
  local ELON_CRITIQUE=$3

  echo "
You are Picasso. Elon reviewed your round $ROUND design.

YOUR DESIGN: $PICASSO_DESIGN
ELON'S CRITIQUE: $ELON_CRITIQUE

Respond decisively:
- What you're changing and how
- What you're defending and why (with design rationale)
- One thing you want to push that Elon didn't mention — your designer's instinct

Write to: $D/round${ROUND}-discussion.md
"
}

# ── ROUNDS 1-5 ──────────────────────────────────────────────────────────────

for ROUND in 1 2 3 4 5; do
  log "ROUND $ROUND — Picasso designs"

  if [ $ROUND -eq 1 ]; then
    PREV_DESIGN=""
    ELON_CRIT=""
    DISCUSSION=""
  else
    PREV_ROUND=$((ROUND-1))
    PREV_DESIGN=$(cat "$D/round${PREV_ROUND}-picasso.md" 2>/dev/null || echo "")
    ELON_CRIT=$(cat "$D/round${PREV_ROUND}-elon.md" 2>/dev/null || echo "")
    DISCUSSION=$(cat "$D/round${PREV_ROUND}-discussion.md" 2>/dev/null || echo "")
  fi

  claude --permission-mode bypassPermissions --print "$(picasso_prompt $ROUND "$PREV_DESIGN" "$ELON_CRIT" "$DISCUSSION")" \
    > "$D/round${ROUND}-picasso-log.txt" 2>&1
  log "Picasso round $ROUND done ($(wc -l < $D/round${ROUND}-picasso.md 2>/dev/null || echo 0) lines)"

  log "ROUND $ROUND — Elon critiques"
  PICASSO_OUT=$(cat "$D/round${ROUND}-picasso.md")
  claude --permission-mode bypassPermissions --print "$(elon_prompt $ROUND "$PICASSO_OUT")" \
    > "$D/round${ROUND}-elon-log.txt" 2>&1
  log "Elon round $ROUND critique done"

  if [ $ROUND -lt 5 ]; then
    log "ROUND $ROUND — Discussion"
    ELON_OUT=$(cat "$D/round${ROUND}-elon.md")
    claude --permission-mode bypassPermissions --print "$(discuss_prompt $ROUND "$PICASSO_OUT" "$ELON_OUT")" \
      > "$D/round${ROUND}-discuss-log.txt" 2>&1
    log "Round $ROUND discussion done"
  fi
done

# ── ELON FINAL SIGN-OFF ──────────────────────────────────────────────────────
log "ELON — Final sign-off"
FINAL_PICASSO=$(cat "$D/round5-picasso.md")
FINAL_ELON=$(cat "$D/round5-elon.md")

claude --permission-mode bypassPermissions --print "
You are Elon. Round 5 is the last design pass. 

ROUND 5 PICASSO DESIGN: $FINAL_PICASSO
ROUND 5 ELON CRITIQUE: $FINAL_ELON

Write a final sign-off for Jensen covering:
1. Is this ready to build? Overall verdict.
2. Micro-fixes Jensen should apply while coding (small things not worth another design round)
3. New Core Data fields needed for emotional feedback layer
4. Specific copy strings Jensen must use verbatim (most important emotional moments)
5. Implementation priority: what to build first vs what can be phase 2

Write to: $D/final-signoff.md
" > "$D/signoff-log.txt" 2>&1
log "Sign-off done"

# ── JENSEN IMPLEMENTS ────────────────────────────────────────────────────────
log "JENSEN — Implementing emotional layer"

FINAL_DESIGN=$(cat "$D/round5-picasso.md")
SIGNOFF=$(cat "$D/final-signoff.md")

claude --permission-mode bypassPermissions --print "
You are Jensen, iOS developer. Implement the emotional coffee layer on top of the existing Brezi app.

EXISTING APP: $APP_DIR
FINAL DESIGN: $FINAL_DESIGN
ELON'S IMPLEMENTATION NOTES: $SIGNOFF
TECH SPEC: $TECH

Implement:
1. EmotionalRecommendationCard — replaces bare profile card. Shows: emotional headline, flavor-as-feeling, origin story, brew settings as secondary info
2. WhyTodayView — transitional screen between health data read and recommendation. Shows state, body need, coffee response.
3. BeanSelectorView — 2-3 specialty bean options per profile, with origin, emotional flavor tag, pairing rationale
4. FlavorPreviewView — '3 feeling words' + abstract gradient visual (use SwiftUI Canvas for gradient)
5. PostBrewFeelingView — replaces star rating. Options: 'Hit right', 'Needed more', 'Too much today'. Saves to Core Data.
6. Update Core Data: add emotionalFeedback (String) to BrewLogEntity, add selectedBeanOrigin (String) and selectedBeanName (String)
7. Update ProfileEngine / InsightEngine to incorporate emotional language

Keep all existing functionality. Add emotional layer on top.
Fix any compile errors before finishing.

When done: openclaw system event --text \"Done: Brezi v3 emotional layer built — ready for Jackson review\" --mode now
" > "$D/jensen-log.txt" 2>&1

log "Jensen done. Swift files: $(find $APP_DIR/Brezi -name '*.swift' | wc -l | tr -d ' ')"
log "═══ ITERATION 2 PIPELINE COMPLETE ═══"

openclaw system event --text "Brezi v3 done — 5 Picasso×Elon emotional design rounds + Jensen built. Ready for Jackson." --mode now
