#!/bin/bash
# Brezi App — Iteration 3: Visual Coffee Layer + Marketplace + Bean-First Flow
# Elon × Picasso × 5 rounds → Jensen implements

WORKSPACE="/Users/jackson/.openclaw/workspace/brezi"
APP_DIR="$WORKSPACE/ios-app"
D="$WORKSPACE/app/design-iteration-3"
PREV="$WORKSPACE/app/design-iteration-2"
BEANS="$WORKSPACE/app/roaster-beans/beans.md"
LOG="$D/pipeline.log"

log() { echo "[$(date '+%H:%M:%S')] $1" | tee -a "$LOG"; }
log "═══ BREZI ITERATION 3 — Visual Coffee + Marketplace + Bean-First ═══"

PREV_DESIGN=$(cat "$PREV/round5-picasso.md" 2>/dev/null || cat "$WORKSPACE/app/design-iteration-1/round3-final-design.md")
CMO=$(cat "$WORKSPACE/app/cmo-ux-copy-brief.md")
TECH=$(cat "$WORKSPACE/app/jensen-tech-spec.md")
BEAN_DATA=$(cat "$BEANS")

JACKSON_BRIEF="
## Jackson's Iteration 3 Feedback (6 points — all must be addressed)

### 1. Keep the first-time guide ✅ (no changes needed)

### 2. Health/Fitness side: stay techy and data-driven ✅
Keep it feeling like WHOOP — numbers, precision, recovery percentages. This is intentional contrast with the coffee side. Users who use WHOOP understand data. Don't soften this side.

### 3. Coffee side: CHEERFUL, warm, illustrative — NOT cold data
The coffee recommendation side needs a completely different energy from the health side. Think:
- Stitch Coffee's illustrated bear in the red Fiat — playful, personality-driven
- Regent's art deco line illustration — elegant, crafted
- Corvus's ornate mandala/temple motif — premium, evocative

Each bean should have its OWN visual character expressed through illustration/art. Flavor notes should not be a list of words — express them as a moment, a feeling, a small visual. Use modern, trendy, minimal illustration (think: flat design icons of actual flavor elements — a chocolate bar, a jasmine flower, a strawberry — like Stitch's tasting note icons).

The flavor profile should be VISUAL — show it as sliders/charts like Stitch Coffee does (Roast: Light→Dark bar, Sweetness: Low→High bar, Acidity: Low→High bar). Not text. Scannable, visual, immediate.

### 4. Serving recommendation = illustrative, not text
Serving size and cutoff time should use visuals:
- Serving size: icon of a cup (1 cup, 2 cups) not '1 serve'
- Cutoff time: a simple clock arc or timeline (e.g. a sun arc with a 'best before 2pm' marker) — not just '2pm text'
- Make it feel like an infographic card, not a settings screen

### 5. Marketplace integration
- Each bean card has an 'Add to Cart' button → links to brezicoffee.com Shopify product
- There is a 'Browse All Beans' section / tab called 'Bean Shop' or 'Marketplace'
- Featured beans are the ones Brezi recommends for YOUR recovery profile today
- Users can subscribe (subscribe & save) or one-time purchase
- Bean card shows: roaster logo/name, bean name, price, flavor notes visual, 'Recommended for you' tag if it matches today's profile

### 6. Bean-first flow — brew settings adapt to selected bean
CURRENT (wrong): health data → brew profile → brew settings
NEW (correct):
1. Health data read → brew profile suggested (Voltage/First Light/etc.)
2. User selects BEAN from marketplace (recommended + browse)  
3. Brew settings auto-adapt to the selected bean's characteristics
4. PARALLEL OPTION: 'Use General Profile' — if they don't have a specific bean, they can proceed with generic brew settings
- The bean selection step is the pivot point between health data and brew execution
- Brew settings should show 'Optimized for: [Bean Name]' or 'Using general Voltage settings'

## 3 Real Roaster Beans to Integrate (from Brezi marketplace)

$BEAN_DATA

## Design Aesthetic Reference
- Stitch Coffee packaging: playful character illustration (bear, Fiat car), feels like indie streetwear brand
- Regent Coffee packaging: art deco line illustration, architectural, earthy tones — feels like a craft spirits label  
- Corvus: ornate gold mandala on black — premium, ceremony, weight

The app's coffee side should feel like it belongs next to these brands. Not clinical. Not a fitness app that happens to have coffee. A coffee brand that happens to be smart.

## Color note
Health/data side: dark (#000000, #0F0F0F), blue accent (#488BED) — cool, precise, WHOOP-like
Coffee/bean side: warmer palette — cream (#F5F0E8), warm amber (#D4A96A), soft white — but still minimal and modern, not cheesy
"

picasso_round() {
  local ROUND=$1; local PREV_P=$2; local PREV_E=$3; local PREV_D=$4
  echo "
You are Picasso — world-class product designer. Ex-Apple Watch, Oura, WHOOP AND specialty coffee brand design. You understand health data interfaces AND the visual language of modern specialty coffee brands.

PREVIOUS APP DESIGN:
$PREV_DESIGN

JACKSON'S FULL BRIEF:
$JACKSON_BRIEF

CMO TONE:
$CMO

$([ -n "$PREV_P" ] && echo "YOUR PREVIOUS ROUND: $PREV_P")
$([ -n "$PREV_E" ] && echo "ELON'S CRITIQUE: $PREV_E")
$([ -n "$PREV_D" ] && echo "DISCUSSION: $PREV_D")

ROUND $ROUND — Design the visual coffee layer with marketplace and bean-first flow.

Focus areas for this round:
1. BEAN CARD VISUAL DESIGN — Each bean card shows: roaster art/logo style, bean bag photo placeholder, flavor slider charts (roast/sweetness/acidity), tasting note icons (flat illustrations), origin badge, price, 'Add to Cart', 'Recommended for you' tag. Use the 3 real beans as the actual data.

2. BEAN-FIRST FLOW — Complete screen-by-screen redesign:
   Screen A: Health data → brew profile suggested
   Screen B: [NEW] Bean Selection — 'Recommended Beans' (3 cards) + 'General Profile' option + 'Browse More' 
   Screen C: Brew settings (shows 'Optimized for [Bean Name]' or 'General [Profile Name] settings')
   Screen D: Brewing / confirmation

3. SERVING INFOGRAPHIC CARD — Visual clock arc for cutoff time, cup icon(s) for serving size, small chart for timing guidance. Replace text-only serving info entirely.

4. FLAVOR VISUALIZATION — Horizontal slider bars (like Stitch): Roast, Sweetness, Acidity. Plus 3 tasting note icons (flat illustration style). Warm, fun, scannable.

5. MARKETPLACE SCREEN — 'Bean Shop' tab or section: filter by brew profile, show featured/recommended at top, allow subscribe & save. Each card links to Shopify.

6. DUAL PALETTE — Health side stays dark/cool. Coffee/bean side uses warmer tones (cream, amber). The visual split should feel intentional — like two connected worlds.

Write the complete spec to: $D/round${ROUND}-picasso.md
Exhaustive. Every screen. All copy. All component specs.
"
}

elon_round() {
  local ROUND=$1; local P=$2
  echo "
You are Elon — COO, top product manager with wearable product experience. Also deeply knowledgeable about specialty coffee culture and what resonates with the Brezi customer (active, taste-conscious, slightly obsessive about optimization).

PICASSO'S ROUND $ROUND DESIGN:
$P

JACKSON'S BRIEF:
$JACKSON_BRIEF

Critique from product + user perspective:

1. VISUAL WARMTH — Does the coffee side actually feel cheerful? Or does it still feel like a health app? Be specific about which screens succeed and which don't.
2. BEAN CARDS — Are they desirable objects? Would a specialty coffee person stop and look, or scroll past? Are the 3 real beans (Stitch, Corvus, Regent) presented in a way that honors their brand identity?
3. FLAVOR VISUALIZATION — Are the sliders/icons fast to read? Can you understand a bean's flavor in under 3 seconds?
4. BEAN-FIRST FLOW — Is the transition from health data to bean selection natural? Is 'General Profile' clearly available without feeling like a lesser option?
5. MARKETPLACE — Would you actually buy a bean through this screen? Is Add to Cart prominent enough?
6. SERVING INFOGRAPHIC — Is it intuitive without explanation? Would a first-time user understand it instantly?
7. DUAL PALETTE — Does the warm/cool color split feel intentional or jarring?
8. BRAND ALIGNMENT — Does the coffee side feel like Stitch/Regent/Corvus would be proud to be listed here?

End with: 3 things Picasso must fix for round $((ROUND+1))
Write to: $D/round${ROUND}-elon.md
"
}

discuss_round() {
  local ROUND=$1; local P=$2; local E=$3
  echo "
You are Picasso. Elon reviewed round $ROUND.

DESIGN: $P
CRITIQUE: $E

Respond as a confident designer. Agree where valid, push back where you have strong design rationale.
State exactly: what changes in round $((ROUND+1)), what stays and why.
Write to: $D/round${ROUND}-discussion.md
"
}

# ── 5 ROUNDS ────────────────────────────────────────────────────────────────

for ROUND in 1 2 3 4 5; do
  log "ROUND $ROUND — Picasso"
  PR=$((ROUND-1))
  PREV_P=$([ $ROUND -gt 1 ] && cat "$D/round${PR}-picasso.md" 2>/dev/null || echo "")
  PREV_E=$([ $ROUND -gt 1 ] && cat "$D/round${PR}-elon.md" 2>/dev/null || echo "")
  PREV_D=$([ $ROUND -gt 1 ] && cat "$D/round${PR}-discussion.md" 2>/dev/null || echo "")

  claude --permission-mode bypassPermissions --print "$(picasso_round $ROUND "$PREV_P" "$PREV_E" "$PREV_D")" \
    > "$D/round${ROUND}-picasso-log.txt" 2>&1
  log "Picasso done ($(wc -l < $D/round${ROUND}-picasso.md 2>/dev/null || echo 0) lines)"

  log "ROUND $ROUND — Elon"
  P_OUT=$(cat "$D/round${ROUND}-picasso.md")
  claude --permission-mode bypassPermissions --print "$(elon_round $ROUND "$P_OUT")" \
    > "$D/round${ROUND}-elon-log.txt" 2>&1
  log "Elon done"

  if [ $ROUND -lt 5 ]; then
    log "ROUND $ROUND — Discussion"
    E_OUT=$(cat "$D/round${ROUND}-elon.md")
    claude --permission-mode bypassPermissions --print "$(discuss_round $ROUND "$P_OUT" "$E_OUT")" \
      > "$D/round${ROUND}-discuss-log.txt" 2>&1
    log "Discussion done"
  fi
done

# ── SIGN-OFF + JENSEN ────────────────────────────────────────────────────────
log "ELON — Sign-off"
FINAL=$(cat "$D/round5-picasso.md")
FINAL_E=$(cat "$D/round5-elon.md")

claude --permission-mode bypassPermissions --print "
You are Elon. Final sign-off.

FINAL DESIGN: $FINAL
ROUND 5 CRITIQUE: $FINAL_E

Write sign-off covering:
1. Ready to build verdict
2. Micro-fixes for Jensen while coding
3. New data model fields needed (bean marketplace data, serving infographic values, Add-to-cart URLs)
4. Priority build order (what users see first)
5. Shopify product URLs for the 3 real beans (use existing Brezi store product IDs from notes)

Write to: $D/final-signoff.md
" > "$D/signoff-log.txt" 2>&1
log "Sign-off done"

log "JENSEN — Building"
SIGNOFF=$(cat "$D/final-signoff.md")

claude --permission-mode bypassPermissions --print "
You are Jensen, iOS developer. Implement iteration 3 on the Brezi app.

EXISTING APP: $APP_DIR
FINAL DESIGN: $FINAL
SIGN-OFF NOTES: $SIGNOFF
TECH SPEC: $TECH
REAL BEAN DATA: $BEAN_DATA

Implement:
1. BeanSelectionView — new screen between health data and brew settings. Shows recommended beans (3 cards from marketplace matching today's profile) + 'Use General Profile' option. 
2. Updated BeanCardView — roaster name badge, bean bag image placeholder, flavor slider bars (Roast/Sweetness/Acidity using SwiftUI GeometryReader), tasting note icons (SF Symbols as placeholder for now), price, 'Add to Cart' (opens SafariServices to Shopify URL), 'Recommended for you' badge
3. Updated BrewSettingsView — shows 'Optimized for [Bean Name]' header when bean selected, or 'General [Profile] Settings' when not
4. ServingInfographicView — clock arc (SwiftUI Canvas/Path) showing cutoff time, cup icon(s) for serving size
5. MarketplaceView (tab/sheet) — browse all beans, filter by profile, featured section. Links to Shopify.
6. Update BeanData model — add: roasterName, price, shopifyUrl, roastScore (0-100), sweetnessScore (0-100), acidityScore (0-100), tastingNoteIcons ([String]), bagImageName (String)
7. Seed the 3 real beans with their actual data from the brief

Fix any remaining compile errors.

When done: openclaw system event --text \"Done: Brezi v4 visual coffee + marketplace + bean-first flow built\" --mode now
" > "$D/jensen-log.txt" 2>&1

log "Jensen done. Swift files: $(find $APP_DIR/Brezi -name '*.swift' | wc -l | tr -d ' ')"
log "═══ ITERATION 3 PIPELINE COMPLETE ═══"

openclaw system event --text "Brezi v4 done — visual coffee layer, marketplace, bean-first flow. Ready for Jackson." --mode now
