#!/usr/bin/env node
// WHOOP OAuth callback server — run this, then click the auth URL
const http = require('http');
const url = require('url');
const fs = require('fs');
const path = require('path');

const PORT = 8080;
const ENV_FILE = path.join(__dirname, '.env');

// Load env
const env = {};
fs.readFileSync(ENV_FILE, 'utf8').split('\n').forEach(line => {
  const [k, ...v] = line.split('=');
  if (k && v.length) env[k.trim()] = v.join('=').trim();
});

const CLIENT_ID = env.WHOOP_CLIENT_ID;
const CLIENT_SECRET = env.WHOOP_CLIENT_SECRET;
const REDIRECT_URI = 'http://localhost:8080/callback';

if (!CLIENT_ID || !CLIENT_SECRET) {
  console.error('❌ WHOOP_CLIENT_ID and WHOOP_CLIENT_SECRET must be set in .env');
  process.exit(1);
}

// Generate state
const state = require('crypto').randomBytes(16).toString('hex');

// Build auth URL
const authUrl = new URL('https://api.prod.whoop.com/oauth/oauth2/auth');
authUrl.searchParams.set('client_id', CLIENT_ID);
authUrl.searchParams.set('redirect_uri', REDIRECT_URI);
authUrl.searchParams.set('response_type', 'code');
authUrl.searchParams.set('scope', 'offline read:recovery read:sleep read:profile read:workout read:body_measurement');
authUrl.searchParams.set('state', state);

console.log('\n🔐 WHOOP OAuth Authorization');
console.log('─'.repeat(60));
console.log('\nOpen this URL in your browser:\n');
console.log(authUrl.toString());
console.log('\n─'.repeat(60));
console.log('Waiting for callback on http://localhost:8080/callback ...\n');

const server = http.createServer(async (req, res) => {
  const parsed = url.parse(req.url, true);
  if (parsed.pathname !== '/callback') { res.end('Not found'); return; }

  const { code, state: returnedState, error } = parsed.query;

  if (error) {
    res.end(`<h1>Error: ${error}</h1>`);
    console.error('❌ OAuth error:', error);
    server.close();
    return;
  }

  if (returnedState !== state) {
    res.end('<h1>State mismatch — possible CSRF</h1>');
    console.error('❌ State mismatch');
    server.close();
    return;
  }

  console.log('✅ Got code:', code);
  console.log('Exchanging for tokens...');

  // Exchange code for tokens
  const tokenRes = await fetch('https://api.prod.whoop.com/oauth/oauth2/token', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: new URLSearchParams({
      grant_type: 'authorization_code',
      code,
      redirect_uri: REDIRECT_URI,
      client_id: CLIENT_ID,
      client_secret: CLIENT_SECRET,
    }).toString(),
  });

  const tokens = await tokenRes.json();

  if (tokens.error) {
    res.end(`<h1>Token error: ${tokens.error}</h1>`);
    console.error('❌ Token error:', tokens);
    server.close();
    return;
  }

  // Save tokens to .env
  let envContent = fs.readFileSync(ENV_FILE, 'utf8');
  const saveKey = (key, val) => {
    if (envContent.includes(`${key}=`)) {
      envContent = envContent.replace(new RegExp(`${key}=.*`), `${key}=${val}`);
    } else {
      envContent += `\n${key}=${val}`;
    }
  };
  saveKey('WHOOP_ACCESS_TOKEN', tokens.access_token);
  saveKey('WHOOP_REFRESH_TOKEN', tokens.refresh_token);
  saveKey('WHOOP_TOKEN_EXPIRY', Date.now() + tokens.expires_in * 1000);
  fs.writeFileSync(ENV_FILE, envContent);

  console.log('✅ Tokens saved to .env');
  console.log('   Access token expires in:', tokens.expires_in, 'seconds');

  res.end('<h1>✅ WHOOP connected! You can close this tab.</h1><p>Tokens saved.</p>');
  server.close(() => process.exit(0));
});

server.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});
