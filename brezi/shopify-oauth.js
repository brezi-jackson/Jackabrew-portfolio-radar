#!/usr/bin/env node
// Brezi Shopify OAuth flow — run once to get access token
const http = require('http');
const https = require('https');
const crypto = require('crypto');
const url = require('url');
const fs = require('fs');

const CLIENT_ID = 'faec401f8dca18c1129029fd58f56d85';
const CLIENT_SECRET = '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';
const SHOP = '021d3f-d0.myshopify.com';
const PORT = 3456;
const REDIRECT_URI = `http://localhost:${PORT}/callback`;
const SCOPES = 'read_content,write_content,read_themes,write_themes,read_products,write_products';
const STATE = crypto.randomBytes(16).toString('hex');

const authUrl = `https://${SHOP}/admin/oauth/authorize?client_id=${CLIENT_ID}&scope=${SCOPES}&redirect_uri=${encodeURIComponent(REDIRECT_URI)}&state=${STATE}`;

console.log('\n=== BREZI SHOPIFY OAUTH ===');
console.log('\n👉 Open this URL in your browser to authorize:\n');
console.log(authUrl);
console.log('\nWaiting for callback on port', PORT, '...\n');

const server = http.createServer((req, res) => {
  const parsed = url.parse(req.url, true);
  if (parsed.pathname !== '/callback') {
    res.end('Not found'); return;
  }

  const { code, state: returnedState, error } = parsed.query;

  if (error) {
    res.end(`<h1>Error: ${error}</h1>`);
    console.error('OAuth error:', error);
    server.close();
    return;
  }

  if (returnedState !== STATE) {
    res.end('<h1>State mismatch — possible CSRF. Abort.</h1>');
    console.error('State mismatch!');
    server.close();
    return;
  }

  // Exchange code for access token
  const postData = JSON.stringify({ client_id: CLIENT_ID, client_secret: CLIENT_SECRET, code });
  const options = {
    hostname: SHOP,
    path: '/admin/oauth/access_token',
    method: 'POST',
    headers: { 'Content-Type': 'application/json', 'Content-Length': Buffer.byteLength(postData) }
  };

  const tokenReq = https.request(options, (tokenRes) => {
    let body = '';
    tokenRes.on('data', d => body += d);
    tokenRes.on('end', () => {
      const data = JSON.parse(body);
      if (data.access_token) {
        console.log('\n✅ ACCESS TOKEN OBTAINED:\n');
        console.log('SHOPIFY_ACCESS_TOKEN=' + data.access_token);
        // Append to .env
        fs.appendFileSync('/Users/jackson/.openclaw/workspace/brezi/.env',
          `\nSHOPIFY_SHOP=${SHOP}\nSHOPIFY_ACCESS_TOKEN=${data.access_token}\n`);
        console.log('\n✅ Saved to brezi/.env');
        res.end('<h1>✅ Brezi Shopify Connected!</h1><p>You can close this tab.</p>');
      } else {
        console.error('Token error:', body);
        res.end('<h1>Token exchange failed</h1><pre>' + body + '</pre>');
      }
      server.close();
    });
  });
  tokenReq.on('error', e => { console.error(e); server.close(); });
  tokenReq.write(postData);
  tokenReq.end();
});

server.listen(PORT);
