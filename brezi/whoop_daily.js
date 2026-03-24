#!/usr/bin/env node
// Daily WHOOP data fetch — run by cron every morning
const fs = require('fs');
const path = require('path');

const ENV_FILE = path.join(__dirname, '.env');

const env = {};
fs.readFileSync(ENV_FILE, 'utf8').split('\n').forEach(line => {
  const [k, ...v] = line.split('=');
  if (k && v.length) env[k.trim()] = v.join('=').trim();
});

async function refreshToken() {
  const res = await fetch('https://api.prod.whoop.com/oauth/oauth2/token', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: new URLSearchParams({
      grant_type: 'refresh_token',
      refresh_token: env.WHOOP_REFRESH_TOKEN,
      client_id: env.WHOOP_CLIENT_ID,
      client_secret: env.WHOOP_CLIENT_SECRET,
    }).toString(),
  });
  const tokens = await res.json();
  if (tokens.error) throw new Error(`Refresh failed: ${tokens.error}`);

  let envContent = fs.readFileSync(ENV_FILE, 'utf8');
  const saveKey = (key, val) => {
    if (envContent.includes(`${key}=`)) {
      envContent = envContent.replace(new RegExp(`${key}=.*`), `${key}=${val}`);
    } else { envContent += `\n${key}=${val}`; }
  };
  saveKey('WHOOP_ACCESS_TOKEN', tokens.access_token);
  saveKey('WHOOP_REFRESH_TOKEN', tokens.refresh_token);
  saveKey('WHOOP_TOKEN_EXPIRY', Date.now() + tokens.expires_in * 1000);
  fs.writeFileSync(ENV_FILE, envContent);
  return tokens.access_token;
}

async function fetchWhoopData(token) {
  const headers = { Authorization: `Bearer ${token}` };

  const [recovery, sleep, profile] = await Promise.all([
    fetch('https://api.prod.whoop.com/developer/v1/recovery/?limit=1', { headers }).then(r => r.json()),
    fetch('https://api.prod.whoop.com/developer/v1/activity/sleep/?limit=1', { headers }).then(r => r.json()),
    fetch('https://api.prod.whoop.com/developer/v1/user/profile/basic', { headers }).then(r => r.json()),
  ]);

  const today = new Date().toISOString().split('T')[0];
  const data = {
    date: today,
    user: profile,
    recovery: recovery.records?.[0] || null,
    sleep: sleep.records?.[0] || null,
    raw: { recovery, sleep },
  };

  // Recovery score (0-100)
  const recoveryScore = data.recovery?.score?.recovery_score;
  // HRV (ms)
  const hrv = data.recovery?.score?.hrv_rmssd_milli;
  // Sleep performance (%)
  const sleepPerf = data.sleep?.score?.sleep_performance_percentage;

  // Map to Brezi app score (1-10)
  let breziScore = null;
  if (recoveryScore != null) {
    breziScore = Math.max(1, Math.min(10, Math.round(recoveryScore / 10)));
  }

  data.brezi = { breziScore, recoveryScore, hrv, sleepPerf };

  const outDir = path.join(__dirname, 'whoop-data');
  fs.mkdirSync(outDir, { recursive: true });
  fs.writeFileSync(path.join(outDir, `${today}.json`), JSON.stringify(data, null, 2));
  fs.writeFileSync(path.join(outDir, 'latest.json'), JSON.stringify(data, null, 2));

  console.log(`✅ WHOOP data saved for ${today}`);
  console.log(`   Recovery: ${recoveryScore} → Brezi score: ${breziScore}/10`);
  console.log(`   HRV: ${hrv?.toFixed(1)} ms | Sleep: ${sleepPerf}%`);
}

(async () => {
  try {
    const expiry = parseInt(env.WHOOP_TOKEN_EXPIRY || '0');
    const needsRefresh = Date.now() > expiry - 300000; // refresh 5 min before expiry
    const token = needsRefresh ? await refreshToken() : env.WHOOP_ACCESS_TOKEN;
    await fetchWhoopData(token);
  } catch (e) {
    console.error('❌ WHOOP daily fetch failed:', e.message);
    process.exit(1);
  }
})();
