#!/bin/bash
# Brezi App — Collaborative Design Pipeline
# Elon (product) ↔ Picasso (design) × 3 rounds → Jensen implements

WORKSPACE="/Users/jackson/.openclaw/workspace/brezi"
APP_DIR="$WORKSPACE/ios-app"
D="$WORKSPACE/app/design-iteration-1"
LOG="$D/pipeline.log"

log() { echo "[$(date '+%H:%M:%S')] $1" | tee -a "$LOG"; }

log "═══ BREZI APP — Elon × Picasso Collaborative Pipeline ═══"

CONTEXT_WIREFRAME=$(cat "$WORKSPACE/app/wireframe-spec.md")
CONTEXT_CMO=$(cat "$WORKSPACE/app/cmo-ux-copy-brief.md")
CONTEXT_TECH=$(cat "$WORKSPACE/app/jensen-tech-spec.md")

JACKSON_BRIEF="
## Jackson's Core Product Feedback
1. CORRELATION INVISIBLE: Score → brew name is a black box. Make data→insight→brew chain EXPLICIT and visible. Also expand recommendation: bean origin, grind, quantity, timing ('1 serve before 2pm').
2. WRONG ONBOARDING ORDER: (1) Pair Brezi device via Bluetooth, (2) Connect wearable (WHOOP/Oura/Apple Health/Skip), (3) Tutorial showing data→insight→brew, (4) Dashboard landing.
3. FUTURE PLATFORM: Extensible for Brezi Blender → smoothie ingredients from same health data.
"

# ═══════════════════════════════════════════════════════
# ROUND 1 — Picasso designs → Elon critiques → discuss
# ═══════════════════════════════════════════════════════
log "ROUND 1 — Picasso initial design"

claude --permission-mode bypassPermissions --print "
You are Picasso — world-class product designer, ex-Apple Watch, Oura Ring, WHOOP. You design health + hardware interfaces.

CURRENT WIREFRAME SPEC:
$CONTEXT_WIREFRAME

CMO COPY BRIEF:
$CONTEXT_CMO

JACKSON'S FEEDBACK:
$JACKSON_BRIEF

Design the v2 Brezi app. Address all of Jackson's feedback. Be opinionated — this is your craft.

Output: A complete wireframe spec covering:
1. Information architecture + nav map
2. New 5-screen onboarding (brand → device pair → wearable → tutorial → dashboard)
3. New home dashboard (health card + insight strip + brew card)
4. Brew recommendation screen (data chain visible)
5. History/correlation view
6. Device hub (extensible)
7. All component specs, spacing, colors, copy

Write to: $D/round1-picasso.md
" > "$D/round1-picasso.log" 2>&1

log "Picasso round 1 done ($(wc -l < $D/round1-picasso.md 2>/dev/null || echo 0) lines)"

log "ROUND 1 — Elon critiques Picasso's design"

PICASSO_R1=$(cat "$D/round1-picasso.md")

claude --permission-mode bypassPermissions --print "
You are Elon — COO of Brezi, the person responsible for product strategy, user experience, and brand. You've worked with Picasso on the v2 app design. You are sharp, direct, and think like a founder.

You just reviewed Picasso's first design pass. Your job is to critique it hard — from a product and user experience perspective. Not as a designer, but as the person who knows the customer, knows the brand, and knows what will make this product win.

PICASSO'S DESIGN:
$PICASSO_R1

JACKSON'S ORIGINAL FEEDBACK (must be addressed):
$JACKSON_BRIEF

Write a direct critique covering:
1. What works well — be specific
2. What's missing or weak — be ruthless
3. Product questions Picasso needs to answer (e.g. what happens when the user has no wearable? Is the device pairing UX clear enough for a non-techie runner?)
4. Brand/tone issues — any screen that doesn't feel like Brezi (sharp, active, earned — not wellness-y)
5. Specific copy lines that need to change and suggested alternatives
6. One big structural suggestion if needed

End with: a numbered list of '3 things Picasso must fix in round 2'

Write to: $D/round1-elon-critique.md
" > "$D/round1-elon.log" 2>&1

log "Elon critique round 1 done"

log "ROUND 1 — Picasso responds and discusses"

ELON_R1=$(cat "$D/round1-elon-critique.md")

claude --permission-mode bypassPermissions --print "
You are Picasso. Elon (COO) just reviewed your first design pass and gave you sharp feedback.

YOUR DESIGN:
$PICASSO_R1

ELON'S CRITIQUE:
$ELON_R1

Respond like a confident designer who respects the feedback but also defends your decisions when you're right. For each of Elon's points:
- Agree and explain how you'll fix it, OR
- Push back with a design rationale if you disagree

Then state: what you're changing for round 2, and what you're keeping and why.

Write to: $D/round1-discussion.md
" > "$D/round1-discuss.log" 2>&1

log "Round 1 discussion done"

# ═══════════════════════════════════════════════════════
# ROUND 2 — Picasso refines → Elon critiques → discuss
# ═══════════════════════════════════════════════════════
log "ROUND 2 — Picasso refines based on discussion"

R1_DISCUSSION=$(cat "$D/round1-discussion.md")

claude --permission-mode bypassPermissions --print "
You are Picasso. Based on the round 1 discussion with Elon, produce a refined design.

ROUND 1 DISCUSSION (what you agreed to change):
$R1_DISCUSSION

ORIGINAL DESIGN:
$PICASSO_R1

Now produce the improved v2 design. This should be noticeably better — tighter correlation visualization, stronger onboarding emotional arc, clearer device pairing UX, better empty states.

Write the complete updated spec to: $D/round2-picasso.md
" > "$D/round2-picasso.log" 2>&1

log "Picasso round 2 done ($(wc -l < $D/round2-picasso.md 2>/dev/null || echo 0) lines)"

log "ROUND 2 — Elon critiques"

PICASSO_R2=$(cat "$D/round2-picasso.md")

claude --permission-mode bypassPermissions --print "
You are Elon — COO of Brezi. Round 2 review.

PICASSO'S ROUND 2 DESIGN:
$PICASSO_R2

WHAT WAS SUPPOSED TO BE FIXED (from round 1):
$R1_DISCUSSION

Critique round 2:
1. Did Picasso actually fix the round 1 issues?
2. New problems introduced?
3. Is the data→insight→brew chain NOW viscerally clear to a first-time user?
4. Is the onboarding tight enough — would a runner just back from a 6am workout bother with this?
5. Platform extensibility — does the device hub actually set up for future blender launch?

End with: '3 things Picasso must nail in round 3 (the final round)'

Write to: $D/round2-elon-critique.md
" > "$D/round2-elon.log" 2>&1

log "Elon critique round 2 done"

log "ROUND 2 — Picasso responds"

ELON_R2=$(cat "$D/round2-elon-critique.md")

claude --permission-mode bypassPermissions --print "
You are Picasso. Elon's round 2 critique:

$ELON_R2

This is the final discussion before round 3 (the last iteration). Respond and commit to exactly what the final version will look like. Be decisive.

Write to: $D/round2-discussion.md
" > "$D/round2-discuss.log" 2>&1

log "Round 2 discussion done"

# ═══════════════════════════════════════════════════════
# ROUND 3 — Final design → Elon signs off → handoff
# ═══════════════════════════════════════════════════════
log "ROUND 3 — Picasso final design"

R2_DISCUSSION=$(cat "$D/round2-discussion.md")

claude --permission-mode bypassPermissions --print "
You are Picasso. This is round 3 — the final design. It goes straight to Jensen for implementation.

ROUND 2 DISCUSSION (committed changes):
$R2_DISCUSSION

ROUND 2 DESIGN (base):
$PICASSO_R2

Produce the FINAL complete spec. This must be:
- Every screen fully specced (no 'similar to above' shortcuts)
- Every component named with exact file name
- Complete ASCII nav tree
- Every string of copy ready to paste into Swift
- All animations: duration, easing, trigger
- All edge cases handled
- Colors: #000000 #FFFFFF #F5F5F5 #488BED #888888 #E0E0E0 #0F0F0F #1A1A1A
- 8pt spacing grid throughout
- Min 44pt tap targets

Write to: $D/round3-final-design.md
" > "$D/round3-picasso.log" 2>&1

log "Picasso round 3 done ($(wc -l < $D/round3-final-design.md 2>/dev/null || echo 0) lines)"

log "ROUND 3 — Elon final sign-off"

PICASSO_R3=$(cat "$D/round3-final-design.md")

claude --permission-mode bypassPermissions --print "
You are Elon — COO of Brezi. This is the final sign-off before Jensen implements.

FINAL DESIGN:
$PICASSO_R3

Write a final sign-off note covering:
1. Overall verdict — is this ready to build?
2. Any last micro-fixes Jensen should make while implementing (small things not worth another design round)
3. What to watch for in the build — UX risks in implementation
4. One sentence summary of what v2 is vs v1

Write to: $D/round3-elon-signoff.md
" > "$D/round3-elon.log" 2>&1

log "Elon sign-off done"

# ═══════════════════════════════════════════════════════
# JENSEN — Implement the final signed-off design
# ═══════════════════════════════════════════════════════
log "JENSEN — Implementing final design"

FINAL_DESIGN=$(cat "$D/round3-final-design.md")
SIGNOFF=$(cat "$D/round3-elon-signoff.md")

claude --permission-mode bypassPermissions --print "
You are Jensen, iOS developer. Implement the final Brezi app v2 design.

FINAL DESIGN SPEC:
$FINAL_DESIGN

ELON'S IMPLEMENTATION NOTES:
$SIGNOFF

TECH SPEC:
$CONTEXT_TECH

Working codebase: $APP_DIR

Instructions:
- Implement all new screens and components from the final design
- For Bluetooth device pairing: simulate with a 3s scan animation, 'Brezi Machine Found', UserDefaults isPaired=true. No real BT needed.
- For WHOOP data: read UserDefaults key 'whoop_latest_strain' and 'whoop_latest_recovery' if available; otherwise show manual input
- For Apple Health: request HRV + sleep read permissions via HealthKit
- Use Swift Charts (iOS 16+) for the correlation history view
- Keep everything from v1 that still applies
- Update Core Data model if new fields are needed (add migration if changing existing entities)
- System fonts (SF Pro + SF Mono) — no custom fonts yet
- After implementing, review all files for compile errors and fix them

When done, run:
openclaw system event --text \"Done: Brezi v2 fully implemented — Picasso x3 + Elon review. Ready for Jackson.\" --mode now
" > "$D/jensen-impl.log" 2>&1

log "Jensen done"
log "Swift files: $(find $APP_DIR/Brezi -name '*.swift' | wc -l | tr -d ' ')"
log "═══ PIPELINE COMPLETE ═══"

openclaw system event --text "Brezi v2 pipeline complete — 3 Picasso×Elon rounds + Jensen built. Open Xcode to test." --mode now
