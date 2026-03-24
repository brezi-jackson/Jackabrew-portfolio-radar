# BREZI CTO — ITERATION 1
## AI Personal Chef Technical Architecture Document

### SYSTEM ARCHITECTURE — 5 LAYERS

```
LAYER 1: DATA INGESTION
  Apple HealthKit | Oura API v2 | WHOOP API | Ultrahuman (future)
  → HRV, sleep stages, resting HR, SpO2, readiness, strain, CGM

LAYER 2: NORMALIZATION ENGINE
  iOS App (Swift/HealthKit SDK) + Backend Normalization Service
  → Canonical BiometricSnapshot object (unified schema)
  → Source conflict resolution, staleness detection, outlier flagging
  → Rolling 30-day personal baseline calibration

LAYER 3: AI ANALYSIS ENGINE
  Rules Engine (hard constraints) → LLM (Claude 3.5 Sonnet)
  → Structured prompt: user baseline + today's snapshot
  → Protocol selection from approved library
  → Output: BrewProtocol object (JSON, schema-validated)

LAYER 4: DEVICE COMMAND LAYER
  iOS App ↔ BLE (CoreBluetooth) ↔ Brezi Wake Hardware
  → NFC cartridge read → BLE command packet → Brew state machine

LAYER 5: OUTCOME CAPTURE LAYER
  2h post-brew push notification → 3-question micro-survey
  → OutcomeRecord → fed back into user profile → closed loop
```

---

### V1 TECHNICAL STACK

**Backend:** Supabase (PostgreSQL + Edge Functions, Deno runtime)
- Singapore region for APAC latency
- Managed auth, row-level security (critical for health data)
- Edge Functions for AI orchestration (API key never leaves server)
- Free tier until >500 users; ~$0.02/recommendation call

**AI:** Anthropic Claude 3.5 Sonnet via API (server-side only)

**iOS:** Swift + HealthKit SDK + CoreBluetooth (Jensen builds)

**Cost at launch:** ~$0/month until 500 users

---

### DATABASE SCHEMA (KEY TABLES)

**biometric_snapshots:** user_id, captured_at, source[], hrv_ms, resting_hr, sleep_duration_min, sleep_score (0-100), deep_sleep_pct, rem_sleep_pct, spo2_pct, body_temp_deviation_c (Oura), recovery_score (WHOOP), readiness_score (Oura), glucose_mg_dl (Ultrahuman, nullable)

**brew_events:** user_id, biometric_snapshot_id, protocol_id, cartridge_id (NFC), started_at, completed_at, temp_target_c, actual_temp_curve (JSONB telemetry), ai_confidence_score, ai_rationale

**outcomes:** brew_event_id, energy_score (1-5), focus_score (1-5), mood_score (1-5), composite_score, notes_hash (SHA256, not raw)

**protocols:** name (Tide Pool / Voltage / etc), temp_target_c, brew_duration_sec, intended_state, contraindications (JSONB)

---

### THE AI RECOMMENDATION ENGINE — V1 DESIGN

**Architecture: Rules Engine + LLM hybrid**

Rules Engine handles hard constraints:
- HRV <30ms → NEVER Voltage (too stimulating)
- body_temp_deviation >+0.5°C → Tide Pool or rest (possible illness)
- sleep <4h → lowest caffeine protocol always
- If data stale >8h → fallback to manual check-in

LLM Layer (Claude 3.5 Sonnet) prompt structure:
```
You are Brezi's personalized morning protocol engine.

USER BASELINE (30-day rolling):
- Average HRV: {baseline_hrv}ms
- Average sleep: {baseline_sleep}min
- Average readiness: {baseline_readiness}

TODAY'S BIOMETRICS:
- HRV: {current_hrv}ms ({hrv_delta}% vs baseline)
- Sleep score: {sleep_score}/100
- Recovery: {recovery_score}/100
- Sleep duration: {sleep_duration}min

USER HISTORY (last 10 brews):
{brew_outcome_pairs}

USER PREFERENCES:
{override_patterns}

Select ONE protocol from: {available_protocols}
Return JSON: {protocol_id, confidence (0-1), rationale (1 sentence, user-facing)}
```

**Confidence thresholds:**
- <0.4: Show recommendation with "Try this?" framing
- 0.4-0.7: Show with light rationale
- >0.7: Show with full confidence ("Your body is asking for this")
- <0.3 AND data stale: Show manual fallback card

---

### MVP iOS APP — 4 SCREENS (Jensen builds)

**Screen 1: Morning Dashboard**
- Today's biometric snapshot
- AI recommendation card: protocol name, 1-sentence rationale
- "Brew Now" CTA

**Screen 2: Brew Control**
- BLE connection state
- NFC cartridge scan prompt
- Real-time brew progress (temp curve, countdown)
- State machine: pre-chill → brewing → ready

**Screen 3: Outcome Check-in**
- Triggered 2h post-brew via local notification
- 3 sliders: Energy / Focus / Mood (1-5)

**Screen 4: Profile / Connections**
- HealthKit toggle
- Oura OAuth connect
- WHOOP OAuth connect
- Brew history

---

### BLE COMMAND STRUCTURE (Brezi Wake GATT Profile)

```
BREW_COMMAND (Write): protocol_id, temp_target (0.5°C increments), duration_sec, cartridge_validated
BREW_STATUS (Notify): state (idle/pre-chill/brewing/ready/error), current_temp, elapsed_sec
CARTRIDGE_INFO (Read after NFC): blend_id, batch_id, expiry_unix, calibration_offset
```

NFC flow: iOS reads tag → validates blend_id server-side → BLE sends BREW_COMMAND with cartridge_validated=true

---

### OAuth FLOWS

**HealthKit:** Native iOS permission sheet. NO OAuth. Data read on-device, normalized snapshot sent to backend. Raw HealthKit data never leaves device.

**Oura API v2:** OAuth2 Auth Code flow via ASWebAuthenticationSession. Tokens stored server-side (encrypted). Webhook registration for daily readiness (eliminates polling).

**WHOOP:** Identical OAuth2 pattern. WHOOP developer program requires app registration — submit now, 2-3 week approval. Scope: `read:recovery read:sleep read:workout`

---

### DATA COMPOUNDING — WHEN DOES ML BECOME REAL?

| Brew Triplets | Value Level |
|---|---|
| <500 | Rules only — too noisy for ML |
| 500–2,000 | Proves outcome scores correlate with protocols |
| 2,000–10,000 | Fine-tune classification model (protocol → biometrics) |
| 10,000+ | Population patterns emerge — cold tolerance clusters |
| 50,000+ | Genuinely proprietary. Cannot be bought or scraped. |

Target: ~2,000 triplets from ~200 engaged users = ~10 brews/user over 3-4 months

---

### PROTOCOL SPECS

**Tide Pool:** temp 4-6°C, 12min, for low HRV (<40ms delta), poor sleep (<60 score)
Contraindication: resting HR >90bpm, body temp deviation >+0.5°C

**Voltage:** temp 1-3°C, 10min, for high readiness (>70), adequate sleep (>70)
Contraindication: HRV <30ms absolute, sleep <5h

**First Light:** temp 6-8°C, 12min, baseline/moderate. Default fallback.

**Deep Work:** temp 3-5°C, 11min, moderate-high readiness + cognitive load signal

---

### LEARNING LOOP — 3 PHASES

**Phase 1 (0-100 users):** Prompt personalization via injected user history. "This user overrides on Mondays regardless of HRV — weight meeting context higher." No model training.

**Phase 2 (100-1,000 users):** Fine-tuning with outcome feedback. Outcome score prediction model (simple regression first). A/B testing protocol selection rules.

**Phase 3 (1,000+ users):** Proprietary classification model. Population-level clustering. Cold tolerance archetype identification. Genuinely differentiated from any LLM-only approach.

---

### PRIVACY ARCHITECTURE — HONEST VERSION

**v1 honest reality:**
- HealthKit data: processed on-device → only normalized snapshot leaves device ✅
- Oura/WHOOP data: fetched server-side (tokens stored encrypted) → cannot avoid server processing
- LLM prompt sent to Anthropic API — contains biometric snapshot (not raw, normalized)
- "On-device ML" is a v2+ promise, not v1 reality

**v1 honest privacy claim:** "Your raw biometric data never leaves your device. We send only a normalized health snapshot — no personally identifiable health records — to power your daily recommendation."

**GDPR/PDPO compliance:**
- Explicit consent before any wearable connection
- Data export endpoint (user can download everything)
- Right to deletion (CASCADE DELETE on user record)
- Data residency: Singapore Supabase region (adequate for HK/PDPO)

---

### BUILD ROADMAP

**Phase 1 (0-3 months) — Jensen solo:**
- iOS app (4 screens above)
- HealthKit integration
- Supabase backend + schema
- LLM recommendation engine (rules + Claude)
- BLE Brezi Wake communication
- NFC cartridge validation
- Outcome check-in loop

**Phase 2 (3-9 months) — need 1 backend hire:**
- Oura + WHOOP OAuth integrations + webhooks
- Automated overnight biometric pull (no manual check-in)
- Brew history analytics
- Personalization via prompt injection
- Apple HealthKit write-back (brew events → Health app)

**Phase 3 (9-18 months) — need ML engineer:**
- Fine-tuned protocol recommendation model
- Population-level clustering
- On-device Core ML inference (privacy brand promise fulfilled)
- Ultrahuman CGM integration (APAC)

---

### TOP 5 TECHNICAL RISKS

1. **Wearable API reliability** [H likelihood, H impact] — Oura/WHOOP webhooks fail, data arrives late. Mitigation: Always maintain yesterday's baseline as fallback. Never block brew execution on API response.

2. **BLE connection instability** [M, H] — Brezi Wake drops connection mid-brew. Mitigation: BLE state machine with automatic reconnect + local command queue on device.

3. **LLM latency** [M, M] — Claude API p95 is ~3s. User expects instant recommendation on wake. Mitigation: Pre-compute overnight (3am job), cache recommendation. User sees instant result.

4. **WHOOP API approval delay** [H, M] — 2-3 week approval. If rejected, launch without WHOOP. HealthKit + Oura covers 80% of target users.

5. **Protocol safety liability** [L, H] — If a user has a medical condition and follows Brezi's recommendation incorrectly. Mitigation: Prominent disclaimer. "Not medical advice." Contraindication hard rules. Never recommend outside safe parameters.

---

### COMPETITIVE DEFENSIBILITY

Someone tries to copy this with 10 engineers. Here's what they can't shortcut:

1. **The brew triplet dataset:** They start at zero. We have 18 months head start. At 50K triplets, the dataset is the product — not the app.
2. **NFC cartridge ecosystem:** They'd need to build an entire certified ingredient supply chain. That's 12-18 months minimum.
3. **Hardware + software co-design:** The BLE GATT profile, the thermoelectric calibration curves, the temp-to-outcome correlations — this requires hardware access. A pure software competitor can't reverse engineer our device's thermal characteristics.
4. **Wearable API relationships:** WHOOP in particular has selective API access. Our relationship with them is an asset.

---

### PATENT STRATEGY — 2 PRIORITY FILINGS

**Filing 1: Biometric-Adaptive Beverage Formulation System**
Claims: Method for determining beverage formulation parameters (temperature, extraction duration, concentration) from biometric input (HRV, sleep score, recovery score) via trained classification model. The biometric→formulation mapping is novel and non-obvious.

**Filing 2: Closed-Loop Biometric Actuation with Outcome Feedback**
Claims: System and method for physical actuation of a consumable preparation device based on biometric state, with structured outcome capture and feedback loop for model refinement. The *closed loop* (biometric → actuation → outcome → model update) is the patentable architecture.

File provisionals now (~$1,500 each). Full utility patents later when the system is built.
