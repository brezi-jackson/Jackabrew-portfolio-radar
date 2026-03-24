# Round 2 Discussion — Picasso Response to Elon Critique
**Author:** Picasso (Design) | March 20, 2026
**Status:** Resolving Round 2 feedback → Round 3 spec

---

## Overall

"This is ready to build. Almost." — I'll take it. Three fixes, all document-level or behavioral gaps rather than design direction issues. That's where we should be at Round 2.

Two I accept immediately. One I want to tighten before accepting.

---

## FIX 1: Brief still says `brew_start + 25h` — ACCEPTED

This is a documentation hygiene issue and Elon's right to call it non-negotiable. A locked brief that contradicts the spec is a latent bug. I flagged "kill stale `brew_start + 25h` everywhere" in the Round 2 changelog but didn't update the brief itself. My mistake — "everywhere" needs to mean everywhere, including the source document.

**Round 3 change:** Update the brief's Caffeine Countdown Timer Card section. Replace `brew_start + 25h (~5 half-lives to <3%)` with `brew_end + 15h (~3 half-lives to ~12.5%)`. One line. Five-minute fix. No design implications — the spec already has the correct formula.

---

## FIX 2: Check-in pill copy — "Too light" vs "Too much" — ACCEPTED

Elon's right, and this one's embarrassing. The brief says "➖ Too light." The spec says "➖ Too much." These are semantic opposites. The spec is correct — the triad needs to be:

- **Right** = dose landed correctly
- **Needed more** = dose was insufficient
- **Too much** = dose was excessive

"Too light" paired with ➖ and sitting next to "Needed more" creates two options that mean roughly the same thing. The whole point of three pills is a clean signal triangle: correct / under / over. "Too much" completes that triangle. "Too light" collapses it.

**Round 3 change:** Update the brief to "➖ Too much" to match the spec. The spec, string index, data model, and pattern engine are already internally consistent on `"too_much"`. This is brief-only.

And for the record — if this had shipped with "Too light" in the brief and a dev built from that, the pattern engine would have told users to *increase* their dose when they reported feeling overcaffeinated. That's not a copy bug, that's a health bug. Good catch.

---

## FIX 3: Manual recovery slider persistence — ACCEPTED, with a tighter spec than proposed

Elon's diagnosis is correct: if a dev persists yesterday's slider value as a "convenience," data quality degrades silently. Every downstream calculation — profile, dose, Why Today copy, pattern history — runs on stale input. The manual path needs the same daily intentionality as the wearable path.

**I accept Option 1 (reset to no selection each day) but I want to spec it tighter than Elon's recommendation.**

Elon suggested: "Slider and chip selection reset to no selection on each new day (defined as after midnight local time or after the first app launch following a sleep window)."

The "sleep window" phrasing is ambiguous without a wearable connected — we don't have sleep data on the manual path. And "after midnight" is technically correct but misses the real trigger. Here's what I'm writing into the spec:

**Round 3 spec — Manual recovery reset logic:**

| Property | Value |
|---|---|
| Reset trigger | First app launch where `Calendar.current.isDate(Date(), inSameDayAs: lastRecoveryInputDate) == false` |
| `lastRecoveryInputDate` | Stored in `UserDefaults`, set when user submits a recovery selection |
| Reset behavior | Chips: no selection (all default style). Slider: hidden until a chip is tapped. No value pre-filled. |
| START BREW gate | `START BREW` button is **disabled** (`#333333` bg, `#666666` text) until a recovery selection is made |
| Gate copy | "Log your recovery to unlock today's brew." — `t_caption` (13px), `#888888`, centered below disabled button |
| Returning same day | If user already logged recovery today and returns to Home, recovery card shows their selection as read-only with an "Update" text link (`t_small`, `#488BED`) |

Why this is tighter:

1. **Calendar day, not midnight.** "After midnight" means a user who opens the app at 11:58 PM sees their value, closes, opens at 12:01 AM and it's gone. Calendar day comparison is the same logic but reads cleaner in code and handles timezone edge cases.

2. **Slider hidden until chip tapped.** Elon's right that a bare slider with no selection invites "leave it in the middle." Hiding the slider until a chip anchors the range forces the user to make one conscious choice. Chip sets the slider. Slider refines. Two-step, but the first step is a 1-second tap.

3. **Disabled START BREW with copy.** This is the enforcement mechanism. Without it, a dev could argue "the default is 50% if nothing is selected" and bypass the gate. An explicitly disabled button with copy makes the requirement unambiguous. The copy — "Log your recovery to unlock today's brew." — is consistent with the app's tone: directive without being preachy.

4. **Same-day return shows read-only.** Without this, a user who leaves the app and returns mid-morning has to re-enter their recovery. That's friction, not intentionality. Show what they logged, let them update if they want.

**New UserDefaults key:**

| Key | Type | Default | Purpose |
|---|---|---|---|
| `lastRecoveryInputDate` | Date? | nil | Gate daily recovery reset |
| `lastRecoveryValue` | Int? | nil | Last submitted recovery (1–10), for same-day read-only display |
| `lastRecoveryChip` | String? | nil | "high" / "functional" / "rest" — for same-day read-only display |

---

## What Changes in Round 3

| # | Change | Source | Scope |
|---|---|---|---|
| 1 | Brief: `brew_start + 25h` → `brew_end + 15h` | Elon Fix 1 | Brief only |
| 2 | Brief: "➖ Too light" → "➖ Too much" | Elon Fix 2 | Brief only |
| 3 | Manual recovery: daily reset, disabled START BREW gate, same-day read-only return | Elon Fix 3 + Picasso tightening | Spec Section 2A (Recovery Card), Section 9 (Data Model) |

Three changes. Two are brief corrections (zero design impact). One is a behavioral spec addition for the manual input path.

---

## What Stays and Why

**Everything else in the Round 2 spec is locked.** Specifically:

- **Sleep-safe at `brew_end + 15h`** — Resolved in R2. The math, the rationale paragraph, and the implementation are all correct. No changes.

- **Long-press dose acceleration** — Resolved in R2. 0.5s trigger, 5g/0.2s, 1.08 scale. Elon confirmed "exactly right." No changes.

- **Boundary glow at min/max** — Elon called it "correct" and noted it works on both single-tap and long-press paths. No changes.

- **"BREW ANYWAY" with projected sleep-safe time** — Elon confirmed "exactly what I asked for and it's exactly right." No changes.

- **Insight line at 3–4 check-ins** — Elon accepted the counter to his original Fix 3. "Actually better than my original suggestion." No changes.

- **One-time toast after 3rd check-in** — Elon confirmed "right call." Copy, styling, gate all approved. No changes.

- **Pattern card stays at 5+ check-ins** — Elon agreed the insight line approach manages expectations without overpromising. No changes.

- **History progressive states** — Elon raised a transition question: what happens at exactly 5? His recommendation: "at 5+, skip the insight line entirely and show only the pattern card." **This is already the intended behavior.** The spec says the insight line "disappears when pattern card appears." I'll add one clarifying sentence to the Round 3 spec to make this explicit: "At 5+ non-skip check-ins, the insight line is not rendered. The pattern card takes its position." But this is a clarification, not a change.

- **Onboarding** — Unchanged since R1. Correctly unchanged.

- **Beans tab** — Unchanged since R1. Commerce stays quarantined.

- **Tab bar** — Locked. No notes from either round.

- **Evening state** — Stays minimal. "Restraint is the brand."

- **All copy** — Elon: "No copy fixes needed." Locked.

- **All animations** — No notes. Locked.

- **Data model** — Only addition is the three new `UserDefaults` keys for manual recovery reset.

---

## Note on Elon's "Resolved from Round 1" Table

Two items marked "Not addressed" in Elon's table:

**Flag 2 (Recovery dots vs loading indicator):** Elon says "Low risk. The 48px percentage above removes ambiguity. Fine to ship." Agreed. Not addressing.

**Flag 5 (Evening state minimal):** Elon says "Correctly deferred to v1.1. Restraint is the brand." Agreed. Not addressing.

These are acknowledged deferrals, not unresolved issues. Carrying them as v1.1 candidates if post-launch data says otherwise.

---

## Round 3 Scope

Round 3 is a tight spec: two brief corrections and one behavioral addition. No structural changes. No copy changes. No animation changes. No new screens or components.

If Elon confirms the manual recovery reset spec above (particularly the disabled START BREW gate and same-day read-only return), this is engineering-ready.

---

*Round 2 discussion written March 20, 2026. Three changes accepted. Design locked pending Elon confirmation on Fix 3 tightening.*
