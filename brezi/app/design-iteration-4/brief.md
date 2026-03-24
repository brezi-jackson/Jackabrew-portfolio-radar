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

### THE BREW CARD (most important screen)
Not a settings screen. A recommendation card.
- Profile name large: "First Light" / "Tide Pool" / "Deep Work" / "Voltage"
- One-line emotional tagline: "Your everyday standard."
- Dose control: [−] 60g [+] with label "(medium strength)" — updates caffeine live
- Estimated caffeine: "~120mg" — updates as dose changes (dose_g × 2.0)
- Brew window: arc or bar showing 6am–2pm, "Best before 2pm"
- ONE button: START BREW
- Advanced toggle (collapsed): grind + temp (cold/4°C default) + ratio
- NO temp on main card. NO bean selection required. NO ratio shown by default.

### Caffeine Countdown Timer Card (post-brew, persistent on Home)
- "⚡ Peak in 38 min" → counts down to peak (45 min after brew start)
- "½ life at 1:15 PM" → static (brew_start + 5h)
- "🌙 Sleep-safe by 10:12 PM" → static (brew_end + 15h, i.e. ~3 half-lives to ~12.5%)
- Amber/yellow accent
- Dismissible (X button)
- Auto-fades when sleep-safe time passes

### Check-In Card (optional, dismissible, below caffeine timer)
Copy: "How did today's brew land?"
3 pill buttons: "👍 Right" · "➕ Needed more" · "➖ Too much"
Skip link below. Disappears after rating or skip.
Purpose: personalization over time ("You rate First Light highest on 65–75% recovery days")

### Home Screen States
Morning (pre-brew): Recovery summary + Brew card + START BREW
Brewing: Live brew timer + END BREW button
Post-brew: Caffeine countdown card + optional check-in card
Evening/cleared: "Caffeine cleared. Recovery window open."

### Bottom Tab Navigation (macOS Dock style)
4 tabs: 🏠 Home | ☕ Beans | 📊 History | ⚙️ Settings
Icons + labels. Rounded pill/capsule selection indicator.
Dark (#0F0F0F) background, #488BED pill for selected tab.

### Beans Tab (SEPARATE — non-blocking)
Warm cream (#F5F0E8) background. A different world entirely.
- "For You Today" section: beans matching today's profile
- "Browse All" 2-column compact grid
- Bean detail: flavor sliders, origin story, Add to Cart (in-app web view), Subscribe & Save
- Optional "Set as my bean" → quietly influences recommendations
3 real beans: Stitch Dark Field / Regent Ethiopia Guji Hambela / Corvus Sadayana Winey

### History Tab
Brew log + pattern cards.
Pattern card example: "You rate First Light highest on 65–75% recovery days"
Per-bean history after 3+ brews.

### Colors
Health/recovery: #000000 / #0F0F0F, #488BED blue accent
Brew card: dark base, amber (#D4A96A) accents for profile name
Beans tab: warm cream (#F5F0E8) background
Caffeine card: dark with amber/yellow accent
Check-in: minimal cream, 3 pill buttons

### Copy Tone
Health side: precise, data-forward ("68% recovery · HRV 62ms · 7h 20m sleep")
Brew card: warm but direct ("Your everyday standard." / "~120mg")
Caffeine timer: useful, not alarming ("Sleep-safe by 7:00 PM")
Check-in: casual ("How did today's brew land?")
Beans tab: story-forward, specialty coffee brand energy

### Hard Rules
- Max 3 taps from open to START BREW
- No temp on main brew card (Advanced only)
- No bean selection as required step
- No g/L jargon without translation (always add "medium strength" / "strong" / "light")
- No clutter — wellness users will abandon complex apps
