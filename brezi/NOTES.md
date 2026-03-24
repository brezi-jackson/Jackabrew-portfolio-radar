# Brezi — Project Notes
*Last updated: 2026-03-17*

---

## What Brezi Is (Repositioned)

Brezi is not a coffee appliance company. It's an **AI health hardware company built around cold as a wellness philosophy.**

- Core product: world's first cold-temperature cold brew machine (PrecisionBrew™ — brews in 10–15 min, no ice, thermoelectric cooling)
- Strategic direction: cold brew → cold infusion (adaptogens, mushrooms) → full cold wellness hardware platform
- Wearable integration: WHOOP / Oura / RingConn as the health data layer; Brezi as the responsive tool layer
- Brand line: **"Cold is a practice."**

---

## Team (Internal / Autonomous)

| Role | Person | Responsibility |
|---|---|---|
| UX + Product + Program Lead | yongping (me) | Synthesizes strategy, owns deliverables, coordinates team |
| CTO / iOS Dev | Jensen | Technical strategy, hardware roadmap, solo iOS developer |
| CMO | CMO | Brand, viral growth, UX copy, campaign strategy |
| Founder / Final Approver | Jackson | Reviews outputs, gives direction, approves spend |

Jackson does not participate in day-to-day. Updates every 24 hours.

---

## Customer Research Summary (Phase 1 Complete)

**7 interviews + 181 backer surveys + Amazon/Taobao reviews**

**4 personas:**
1. **Cold Brew Devotee** (Devin, Erick) — already making cold brew, Brezi saves them 1 hour of filtering mess
2. **Espresso Enthusiast** (Jan, Jay) — precision-oriented, single serve, treats it like espresso
3. **Convenience Upgrader** (Jim, Tony) — wants speed and simplicity, came from instant/pod coffee
4. **Stomach-Sensitive Coffee Lover** (Ivan) — cold brew's 67% lower acid is medically relevant, not a feature

**Top pain point:** No grind guidance at purchase → #1 cause of bad first brews → kills reviews
**Top loved features:** Speed, smooth low-acid flavor, build quality, single-serve precision
**Discovery path:** Instagram ad → YouTube (Brian Chan) → Kickstarter

**Health claim in use:** Cold brew = 67% less acidic than hot coffee (scientifically validated)

---

## Brand Guidelines (from Brandbook 2025)

- **Colors:** Black #000000, White #FFFFFF, Light Grey #F5F5F5, Brand Blue #488BED (PMS 2727 C)
- **Font:** ABC Social — Condensed Bold (headlines), Regular (body), Mono Regular (eyebrow, ALL CAPS, 20% tracking), Condensed (CTAs ALL CAPS)
- **Logo:** bre:zi — colon = cold brew time notation. Min 120px. "b" bean mark for avatars.
- **Brand grammar:** Colon used in all caps — COLD:BREW TIME, IT'S COLD:BREW TIME
- **Tech name:** PrecisionBrew™
- **Consumer language:** Home Brewers / Brezi Barista

---

## iOS App — Status

**Concept:** Morning recovery check-in → personalized cold brew recommendation → brew log → data flywheel

**5 Brew Profiles (CMO names + Jensen tech specs):**
| Profile | Recovery Score | Temp | Steep | Ratio |
|---|---|---|---|---|
| Voltage | 9–10 | 15–20°C | 12–16h | 60–80 g/L |
| Deep Work | 7–8 | 15–20°C | 12–16h | 80 g/L |
| First Light | 5–6 | 4–8°C | 12–14h | 60 g/L |
| Tide Pool | 3–4 | 4°C | 8–10h | 50 g/L |
| The Blank | Manual | — | — | User-set |

**Tech stack:** SwiftUI + Core Data + Combine, iOS 16+
**MVP scope:** Screen 1 (recovery slider) + Screen 2 (brew recommendation)
**Estimated build:** 3–4 weeks from design lock → TestFlight
**Design lock needed before Jensen codes:** color palette, font sizes, icon style, card/button components

**Wearable integration plan:**
- Phase 1: Manual HRV/recovery input (rule-based, shippable now)
- Phase 2: Oura API (more developer-friendly, start here)
- Phase 3: WHOOP API (invite-only, can take 6–12 months)

**Files:** `/brezi/app/jensen-tech-spec.md` + `/brezi/app/cmo-ux-copy-brief.md`

---

## Shopify — Status

**Store:** brezicoffee.com (myshopify: 021d3f-d0.myshopify.com)
**API access:** Connected via custom app token (shpat) — scopes: content, themes, products

**Pages built:**
| Page | URL | Status |
|---|---|---|
| Grind Guide | /pages/grind-guide | Draft (unpublished) |
| Roaster Partnerships | /pages/roaster-partnerships | Live ✅ |
| About Us | /pages/about-us | Original restored ✅ |

**Roaster Partnerships page includes:**
- 4-column product grid (Regent Ethiopia, Regent Brew Pack, Brezi × Stitch Set, Coming Soon slot)
- "For Coffee Roasters" section with 3-tier partnership model (white/blue/black cards)
- Application form (name, location, website, contact, origins, message)

**Nav fix needed (manual — no API scope):**
- Shopify Admin → Online Store → Navigation → Main menu
- Edit "Roaster Collab" item → change link to Pages → Roaster Partnerships
- Drag to position between About Us and Brew Guide

**Current partners in "roaster Collab" collection:**
- Regent Coffee (Ethiopia Guji Hambela Washed $27, Brew Pack $30)
- Stitch Coffee (Brezi × Stitch Cold Brew Set, from $399, was $475)

---

## Strategic Direction (Locked Decisions)

- **Year 1 priority:** App first → Shopify pages → grind guide content
- **No lab/patent/influencer spend until results exist**
- **Cartridge/pod system** = future business model (razor/blade) — NOT v1
- **Primary target community:** biohacker-athletes (WHOOP users, Hyrox, CrossFit)
- **Viral mechanic:** #TheColdStack — users post morning ritual with Brezi at center
- **Campaign:** "Start Cold" / "Cold is Your Edge"
- **Wearable pitch line:** "WHOOP knows your body. Brezi knows your morning."

---

## Pending / Blockers

- [ ] App: Design lock (colors, fonts, component specs) before Jensen can code
- [ ] App: Wireframe spec from yongping (in progress, 24hr delivery)
- [ ] Shopify nav: Jackson to manually fix "Roaster Collab" link
- [ ] Grind Guide page: currently a draft — needs Jackson review + publish approval
- [ ] Product descriptions: Regent Coffee products have minimal copy — needs tasting notes

---

## Files & Credentials

All files in: `/Users/jackson/.openclaw/workspace/brezi/`

```
brandbook.pdf               — full 64-page brand PDF
brand-snapshot.md           — extracted brand guidelines
phase1-synthesis.md         — customer research synthesis
NOTES.md                    — this file
.env                        — Shopify API token (DO NOT SHARE)
gdrive-credentials.json     — Google Drive service account
surveys/                    — all raw interview audio + transcripts
app/
  jensen-tech-spec.md       — iOS technical spec
  cmo-ux-copy-brief.md      — UX copy brief (onboarding, profiles, push notifications)
```
