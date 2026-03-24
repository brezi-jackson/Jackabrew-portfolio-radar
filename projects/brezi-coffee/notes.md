# Brezi Coffee — Project Notes

> Shared context file. Updated by the agent across sessions/channels.
> Last updated: 2026-03-17

---

## Background

Brezi is a Hong Kong-based hardware startup. Core product: the world's first cold-temperature cold brew machine (PrecisionBrew™ — brews in 10–15 min at cold temperature, no ice, thermoelectric cooling). Founded by Jackson (UC Berkeley) and Graves Ho in Causeway Bay, HK. The team are all sports and wellness enthusiasts. Engineering team comes from the semiconductor industry and top-tier consumer electronics companies.

Launched via Kickstarter. Currently selling on brezicoffee.com (Shopify). Also on Taobao for China market.

---

## Product / Offering

**Hardware:** Brezi cold brew machine
- PrecisionBrew™ thermoelectric cooling system
- Brews in ~12 minutes at cold temperature (no ice, no hot water)
- Single-serve precision, variable strength (light to super strong)
- 67% less acidic than hot coffee (medically relevant claim)

**Price point:** ~$350–400 (comparable to espresso machines in customer perception)

**Current partner products (Roaster Collab collection):**
- Regent Coffee — Ethiopia Guji Hambela Washed ($27)
- Regent Coffee — Brew Pack ($30)
- Brezi × Stitch Coffee — Cold Brew Set (from $399, was $475)

---

## Target Market

**4 customer personas (from Phase 1 research — 7 interviews + 181 backer surveys):**

1. **Cold Brew Devotee** (Devin, Erick) — already making cold brew, Brezi saves them 1hr of filtering mess. Daily users.
2. **Espresso Enthusiast** (Jan, Jay) — precision-oriented, single-serve, treats it like espresso.
3. **Convenience Upgrader** (Jim, Tony) — wants speed and simplicity, came from instant/pod coffee.
4. **Stomach-Sensitive Coffee Lover** (Ivan) — cold brew's low acid is medically relevant, not just a feature.

**Discovery path:** Instagram ad → YouTube review (Brian Chan) → Kickstarter

**Emerging target:** Biohacker-athletes (WHOOP/Oura users, Hyrox, CrossFit) — the strategic growth segment

---

## Current Status

### Shopify (brezicoffee.com)
- ✅ Roaster Partnerships page — live at /pages/roaster-partnerships
- 📝 Grind Guide page — drafted, unpublished (needs Jackson review)
- ✅ About Us page — original restored
- ⚠️ Nav fix needed: "Roaster Collab" nav item currently points to wrong page. Jackson to fix: Admin → Online Store → Navigation → Main menu → edit "Roaster Collab" → link to Pages → Roaster Partnerships

### iOS App
- ✅ Technical spec complete (Jensen)
- ✅ UX copy brief complete (CMO) — tone, onboarding, 5 brew profiles, push notifications
- ⏳ Wireframe spec in progress (yongping, 24hr delivery)
- ⏳ Awaiting design lock before Jensen codes

### Research
- ✅ Phase 1 complete — all 7 interviews transcribed + synthesized
- ✅ Saved at: /brezi/surveys/ and /brezi/phase1-synthesis.md

---

## Strategy & Goals

**Repositioning:** Brezi is not a coffee appliance. It's an **AI health hardware company built around cold wellness.**

**3-year roadmap:**
- Year 1: App + core machine + cold brew education (grind guide, roaster community)
- Year 2: Health wearable partnerships (WHOOP, Oura, RingConn) + functional ingredients (adaptogens)
- Year 3: Full cold wellness hardware platform (Brezi OS)

**Brand line:** "Cold is a practice."
**Campaign concept:** "Start Cold" / "Cold is Your Edge"
**Viral mechanic:** #TheColdStack — users post morning ritual with Brezi at center
**Wearable pitch:** "WHOOP knows your body. Brezi knows your morning."

**iOS App — 5 Brew Profiles:**
| Profile | Recovery Score | Temp | Steep | Ratio |
|---|---|---|---|---|
| Voltage | 9–10 | 15–20°C | 12–16h | 60–80 g/L |
| Deep Work | 7–8 | 15–20°C | 12–16h | 80 g/L |
| First Light | 5–6 | 4–8°C | 12–14h | 60 g/L |
| Tide Pool | 3–4 | 4°C | 8–10h | 50 g/L |
| The Blank | Manual | — | — | User-set |

**Wearable integration phasing:**
- Phase 1: Manual HRV/recovery input → rule-based brew recommendation (shippable now)
- Phase 2: Oura API integration (most developer-friendly)
- Phase 3: WHOOP API (invite-only, 6–12 months)

---

## Key Decisions

- No lab/patent/influencer spend until results exist
- Jensen is sole iOS coder — no contractors yet
- Cartridge/pod system = future business model (razor/blade) — not v1
- App-first approach before hardware revision
- Save Shopify theme changes as drafts; all other modifications need Jackson approval
- Update cadence: every 24 hours

---

## Open Questions

- App design lock: what colors/fonts/components to confirm before Jensen starts coding?
- Grind Guide: approve for publishing?
- Roaster partners: write product descriptions for Regent Coffee products?
- WHOOP developer API access: has anyone applied?

---

## Brand Quick Reference

- **Colors:** Black #000000 / White #FFFFFF / Grey #F5F5F5 / Blue #488BED
- **Font:** ABC Social (Condensed Bold headlines, Regular body, Mono Regular eyebrow)
- **Logo:** bre:zi (colon = cold brew time). Min 120px.
- **Tech name:** PrecisionBrew™
- **Consumer terms:** Home Brewers / Brezi Barista

---

## Files & Credentials

> The files section has a full table for Phase 1 research — every interview audio, transcript, survey file, and the synthesis doc — plus separate tables for brand, app, and credentials. Anyone opening that notes file in a new channel will know exactly where everything lives.

All project files: `/Users/jackson/.openclaw/workspace/brezi/`

### Phase 1 Research Documents
| File | Description |
|---|---|
| `phase1-synthesis.md` | Full written synthesis — personas, insights, priorities, recommendations |
| `surveys/backer_survey.xlsx` | 181 Kickstarter backer survey responses |
| `surveys/barista_feedback.xlsx` | Barista feedback data |
| `surveys/taobao_reviews.docx` | 27 Taobao customer reviews (China market) |
| `surveys/audio/devin.m4a` | Interview audio — Devin |
| `surveys/audio/Erick.m4a` | Interview audio — Erick |
| `surveys/audio/Jan.m4a` | Interview audio — Jan |
| `surveys/audio/Ivan.m4a` | Interview audio — Ivan |
| `surveys/audio/ganesh.m4a` | Interview audio — Ganesh |
| `surveys/audio/jay.m4a` | Interview audio — Jay |
| `surveys/audio/jim.m4a` | Interview audio — Jim |
| `surveys/audio/tony.m4a` | Interview audio — Tony |
| `surveys/transcripts/` | Full text transcripts for all 8 interviews |

### Brand
| File | Description |
|---|---|
| `brandbook.pdf` | Full 64-page brand PDF (downloaded from Google Drive) |
| `brand-snapshot.md` | Extracted brand guidelines — colors, fonts, voice, logo rules |

### App
| File | Description |
|---|---|
| `app/jensen-tech-spec.md` | iOS technical spec — stack, data models, brew profiles, decision logic, build time |
| `app/cmo-ux-copy-brief.md` | UX copy brief — tone, onboarding screens, brew profile names/descriptions, push notifications, app store copy |

### Credentials (do not share)
| File | Description |
|---|---|
| `.env` | Shopify Admin API token (shpat) |
| `gdrive-credentials.json` | Google Drive service account |

Shopify store: 021d3f-d0.myshopify.com (brezicoffee.com)
