#ifndef INDEX_HTML_H
#define INDEX_HTML_H

#include <pgmspace.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="tr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Akıllı Ev Kontrolü</title>
  <meta name="theme-color" content="#0f172a">
  <meta name="apple-mobile-web-app-capable" content="yes">
  <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent">
  <meta name="apple-mobile-web-app-title" content="Akıllı Ev">
  <link rel="apple-touch-icon" href="https://img.icons8.com/isometric-folders/512/home.png">
  <link rel="icon" type="image/png" href="https://img.icons8.com/isometric-folders/512/home.png">
  <link rel="manifest" href='data:application/manifest+json,{"name":"Akıllı Ev Kontrolü","short_name":"Akıllı Ev","start_url":"/","display":"standalone","background_color":"#0f172a","theme_color":"#0f172a","icons":[{"src":"https://img.icons8.com/isometric-folders/512/home.png","sizes":"512x512","type":"image/png"}]}'>
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
  <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700&family=Outfit:wght@400;600;700&display=swap" rel="stylesheet">
  <style>
    :root {
      --bg-gradient: linear-gradient(135deg, #0f172a 0%, #1e1b4b 50%, #0f172a 100%);
      --card-bg: rgba(30, 41, 59, 0.75);
      --card-border: rgba(255, 255, 255, 0.12);
      --card-blur: blur(16px);
      --accent-blue: #38bdf8;
      --accent-green: #22c55e;
      --accent-purple: #a855f7;
      --accent-orange: #f97316;
      --accent-red: #ef4444;
      --text-main: #f8fafc;
      --text-muted: #94a3b8;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; font-family: 'Inter', sans-serif; -webkit-tap-highlight-color: transparent; }
    body { background: var(--bg-gradient); min-height: 100vh; color: var(--text-main); padding: 16px; display: flex; flex-direction: column; align-items: center; }
    .container { width: 100%; max-width: 1200px; }
    header { display: flex; justify-content: space-between; align-items: center; padding: 20px; background: var(--card-bg); backdrop-filter: var(--card-blur); border: 1px solid var(--card-border); border-radius: 20px; margin-bottom: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); }
    .brand { display: flex; align-items: center; gap: 14px; }
    .brand-icon { width: 48px; height: 48px; background: linear-gradient(135deg, #38bdf8, #818cf8); border-radius: 14px; display: flex; align-items: center; justify-content: center; font-size: 24px; box-shadow: 0 0 20px rgba(56, 189, 248, 0.4); }
    .brand h1 { font-family: 'Outfit', sans-serif; font-size: 22px; font-weight: 700; letter-spacing: 0.5px; }
    .brand p { font-size: 13px; color: var(--text-muted); }
    .header-badges { display: flex; gap: 10px; flex-wrap: wrap; }
    .badge { display: flex; align-items: center; gap: 8px; padding: 8px 14px; border-radius: 30px; font-size: 13px; font-weight: 500; background: rgba(15, 23, 42, 0.6); border: 1px solid var(--card-border); }
    .dot { width: 8px; height: 8px; border-radius: 50%; background: var(--accent-green); box-shadow: 0 0 10px var(--accent-green); }
    .dot.offline { background: var(--accent-red); box-shadow: 0 0 10px var(--accent-red); }
    .dot.pulse { animation: pulse 2s infinite; }
    @keyframes pulse { 0% { opacity: 1; transform: scale(1); } 50% { opacity: 0.4; transform: scale(1.2); } 100% { opacity: 1; transform: scale(1); } }
    .voice-bar { background: linear-gradient(135deg, rgba(168, 85, 247, 0.3), rgba(56, 189, 248, 0.3)); border: 1px solid rgba(168, 85, 247, 0.4); backdrop-filter: var(--card-blur); border-radius: 20px; padding: 16px 20px; display: flex; align-items: center; justify-content: space-between; margin-bottom: 24px; box-shadow: 0 8px 24px rgba(168, 85, 247, 0.2); }
    .voice-info { display: flex; align-items: center; gap: 12px; }
    .voice-btn { width: 48px; height: 48px; border-radius: 50%; background: linear-gradient(135deg, #a855f7, #ec4899); border: none; color: #fff; font-size: 22px; cursor: pointer; display: flex; align-items: center; justify-content: center; box-shadow: 0 0 20px rgba(168, 85, 247, 0.5); transition: transform 0.2s ease; }
    .voice-btn:hover { transform: scale(1.1); }
    .voice-btn.listening { animation: mic-pulse 1.2s infinite; background: linear-gradient(135deg, #ef4444, #f97316); }
    @keyframes mic-pulse { 0% { box-shadow: 0 0 0 0 rgba(239, 68, 68, 0.7); } 70% { box-shadow: 0 0 0 15px rgba(239, 68, 68, 0); } 100% { box-shadow: 0 0 0 0 rgba(239, 68, 68, 0); } }
    .dashboard-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(330px, 1fr)); gap: 20px; }
    .card { background: var(--card-bg); backdrop-filter: var(--card-blur); border: 1px solid var(--card-border); border-radius: 24px; padding: 24px; display: flex; flex-direction: column; gap: 20px; transition: transform 0.3s ease, box-shadow 0.3s ease; box-shadow: 0 8px 24px rgba(0,0,0,0.2); }
    .card:hover { transform: translateY(-4px); box-shadow: 0 16px 32px rgba(0,0,0,0.3); border-color: rgba(255, 255, 255, 0.2); }
    .card-header { display: flex; justify-content: space-between; align-items: center; }
    .card-title { display: flex; align-items: center; gap: 10px; font-family: 'Outfit', sans-serif; font-size: 18px; font-weight: 600; }
    .card-title-icon { font-size: 20px; }
    .rgb-preview-box { display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 12px; padding: 20px; background: rgba(15, 23, 42, 0.5); border-radius: 16px; border: 1px solid var(--card-border); }
    .rgb-orb { width: 80px; height: 80px; border-radius: 50%; background: #000; border: 3px solid rgba(255,255,255,0.2); box-shadow: 0 0 30px rgba(0,0,0,0.5); transition: all 0.4s ease; }
    .color-presets { display: flex; gap: 10px; justify-content: center; flex-wrap: wrap; }
    .color-btn { width: 38px; height: 38px; border-radius: 50%; border: 2px solid rgba(255,255,255,0.3); cursor: pointer; transition: transform 0.2s ease, border-color 0.2s ease; }
    .color-btn:hover { transform: scale(1.15); border-color: #fff; }
    .btn { padding: 12px 20px; border-radius: 14px; border: none; font-weight: 600; font-size: 14px; cursor: pointer; display: flex; align-items: center; justify-content: center; gap: 8px; transition: all 0.2s ease; background: linear-gradient(135deg, #38bdf8, #0284c7); color: #fff; box-shadow: 0 4px 15px rgba(56, 189, 248, 0.3); }
    .btn:hover { opacity: 0.95; transform: translateY(-2px); box-shadow: 0 6px 20px rgba(56, 189, 248, 0.4); }
    .btn:active { transform: translateY(0); }
    .btn-secondary { background: rgba(51, 65, 85, 0.8); color: #f8fafc; box-shadow: none; border: 1px solid var(--card-border); }
    .btn-secondary:hover { background: rgba(71, 85, 105, 1); box-shadow: 0 4px 12px rgba(0,0,0,0.2); }
    .stat-gauge { display: flex; flex-direction: column; gap: 8px; }
    .stat-label { display: flex; justify-content: space-between; font-size: 14px; color: var(--text-muted); }
    .progress-bar-bg { width: 100%; height: 12px; background: rgba(15, 23, 42, 0.7); border-radius: 6px; overflow: hidden; border: 1px solid var(--card-border); }
    .progress-bar-fill { height: 100%; width: 0%; background: linear-gradient(90deg, #38bdf8, #22c55e); border-radius: 6px; transition: width 0.6s ease; }
    .distance-display { text-align: center; padding: 24px; background: rgba(15, 23, 42, 0.5); border-radius: 16px; border: 1px solid var(--card-border); }
    .distance-value { font-family: 'Outfit', sans-serif; font-size: 42px; font-weight: 700; color: var(--accent-blue); }
    .distance-unit { font-size: 16px; color: var(--text-muted); margin-left: 4px; }
    .distance-badge { display: inline-block; margin-top: 10px; padding: 4px 12px; border-radius: 20px; font-size: 12px; font-weight: 600; background: rgba(34, 197, 94, 0.2); color: var(--accent-green); border: 1px solid rgba(34, 197, 94, 0.4); }
    .tv-remote-grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 10px; }
    .remote-btn { padding: 14px; border-radius: 12px; background: rgba(51, 65, 85, 0.6); border: 1px solid var(--card-border); color: #fff; font-weight: 600; font-size: 14px; cursor: pointer; display: flex; align-items: center; justify-content: center; transition: all 0.2s ease; }
    .remote-btn:hover { background: rgba(71, 85, 105, 0.9); border-color: var(--accent-blue); }
    .remote-btn.power { background: linear-gradient(135deg, #ef4444, #dc2626); color: #fff; }
    footer { text-align: center; margin-top: 40px; padding: 20px; color: var(--text-muted); font-size: 13px; }
    @media (max-width: 768px) { header { flex-direction: column; align-items: flex-start; gap: 16px; } .header-badges { width: 100%; justify-content: space-between; } }
  </style>
</head>
<body>
  <div class="container">
    <header>
      <div class="brand">
        <div class="brand-icon">🏠</div>
        <div>
          <h1>Akıllı Ev & Saksı Kontrolü</h1>
          <p>ESP32-C6 & NodeMCU (ESP8266) Ana Kontrol Paneli</p>
        </div>
      </div>
      <div class="header-badges">
        <div class="badge">
          <div class="dot pulse"></div>
          <span>ESP32-C6: <strong>Online</strong></span>
        </div>
        <div class="badge" id="nodemcu-badge">
          <div class="dot offline" id="nodemcu-dot"></div>
          <span id="nodemcu-status-text">NodeMCU: <strong>Çevrimdışı (Güç Yok)</strong></span>
        </div>
      </div>
    </header>

    <div class="voice-bar">
      <div class="voice-info">
        <button class="voice-btn" id="voice-trigger-btn" onclick="startVoiceRecognition()">🎤</button>
        <div>
          <strong style="font-size: 15px; display: block;">Türkçe Sesli Kontrol</strong>
          <span style="font-size: 13px; color: var(--text-muted);" id="voice-status-text">Butona basıp konuşun (Örn: "Işığı aç", "Karakter yap", "Klimayı aç", "TV kapat")</span>
        </div>
      </div>
      <button class="btn btn-secondary" style="font-size: 12px; padding: 8px 14px;" onclick="showPwaInstructions()">📲 Uygulama Yap</button>
    </div>

    <div class="dashboard-grid">

      <!-- 💡 RGB LED & AYDINLATMA -->
      <div class="card">
        <div class="card-header">
          <div class="card-title">
            <span class="card-title-icon">💡</span>
            <span>ESP32 RGB LED Kontrolü</span>
          </div>
          <button class="btn btn-secondary" onclick="toggleRgb()">Durum Değiştir</button>
        </div>

        <div class="rgb-preview-box">
          <div class="rgb-orb" id="rgb-orb"></div>
          <div style="font-size: 13px; color: var(--text-muted);">
            Durum: <strong id="rgb-status-text" style="color: var(--accent-green);">AÇIK</strong> | 
            RGB: <span id="rgb-value-text">255, 120, 50</span>
          </div>
        </div>

        <div>
          <div style="font-size: 13px; color: var(--text-muted); margin-bottom: 10px;">Hızlı Renk Seçenekleri:</div>
          <div class="color-presets">
            <div class="color-btn" style="background: #38bdf8;" onclick="setRgb(56, 189, 248)"></div>
            <div class="color-btn" style="background: #22c55e;" onclick="setRgb(34, 197, 94)"></div>
            <div class="color-btn" style="background: #f97316;" onclick="setRgb(249, 115, 22)"></div>
            <div class="color-btn" style="background: #a855f7;" onclick="setRgb(168, 85, 247)"></div>
            <div class="color-btn" style="background: #ef4444;" onclick="setRgb(239, 68, 68)"></div>
            <div class="color-btn" style="background: #facc15;" onclick="setRgb(250, 204, 21)"></div>
            <div class="color-btn" style="background: #ffffff;" onclick="setRgb(255, 255, 255)"></div>
            <div class="color-btn" style="background: #000000; border-color: #666;" onclick="setRgb(0, 0, 0)"></div>
          </div>
        </div>

        <button class="btn" onclick="randomRgb()">🎲 Rastgele Renk Üret</button>
      </div>

      <!-- ❄️ KLİMA IR KUMANDASI -->
      <div class="card">
        <div class="card-header">
          <div class="card-title">
            <span class="card-title-icon">❄️</span>
            <span>Klima IR Kontrolü</span>
          </div>
          <span class="badge" id="klima-status-badge" style="color: var(--accent-blue);">Kapalı</span>
        </div>

        <div style="display: flex; gap: 12px;">
          <button class="btn" style="flex: 1; background: linear-gradient(135deg, #22c55e, #16a34a);" onclick="klimaCmd('1')">⚡ Klimayı AÇ (1)</button>
          <button class="btn" style="flex: 1; background: linear-gradient(135deg, #ef4444, #dc2626);" onclick="klimaCmd('0')">🛑 Klimayı KAPAT (0)</button>
        </div>
      </div>

      <!-- 🌱 SAKSI & BİTKİ OTOMASYONU -->
      <div class="card">
        <div class="card-header">
          <div class="card-title">
            <span class="card-title-icon">🌱</span>
            <span>Saksı & Toprak Nemi</span>
          </div>
          <span class="badge" id="plant-status-badge" style="color: var(--accent-green); border-color: rgba(34,197,94,0.4);">Nem İdeal</span>
        </div>

        <div class="stat-gauge">
          <div class="stat-label">
            <span>Toprak Nem Seviyesi</span>
            <strong id="moisture-val">68%</strong>
          </div>
          <div class="progress-bar-bg">
            <div class="progress-bar-fill" id="moisture-bar" style="width: 68%;"></div>
          </div>
        </div>

        <div class="stat-gauge">
          <div class="stat-label">
            <span>Otomatik Sulama Modu</span>
            <strong style="color: var(--accent-blue);">Aktif (Otomatik)</strong>
          </div>
        </div>

        <div style="display: flex; gap: 10px;">
          <button class="btn" style="flex: 1;" onclick="waterPlant()">💧 5sn Manuel Sula</button>
          <button class="btn btn-secondary" onclick="fetchStatus()">🔄 Yenile</button>
        </div>
      </div>

      <!-- 📏 GENEL AMAÇLI MESAFE SENSÖRÜ -->
      <div class="card">
        <div class="card-header">
          <div class="card-title">
            <span class="card-title-icon">📏</span>
            <span>Mesafe Sensörü (HC-SR04)</span>
          </div>
          <span class="badge" style="color: var(--accent-blue);">Aktif</span>
        </div>

        <div class="distance-display">
          <div>
            <span class="distance-value" id="distance-val">24.5</span>
            <span class="distance-unit">cm</span>
          </div>
          <div class="distance-badge" id="distance-badge">Orta Mesafede (Algılandı)</div>
        </div>

        <div class="stat-gauge">
          <div class="stat-label">
            <span>Mesafe Skalası (0 - 100 cm)</span>
            <span id="distance-meter-txt">24.5 cm</span>
          </div>
          <div class="progress-bar-bg">
            <div class="progress-bar-fill" id="distance-bar" style="width: 24.5%; background: linear-gradient(90deg, #a855f7, #38bdf8);"></div>
          </div>
        </div>
      </div>

      <!-- 📺 TV & MEDYA KONTROL ÜNİTESİ -->
      <div class="card">
        <div class="card-header">
          <div class="card-title">
            <span class="card-title-icon">📺</span>
            <span>TV & Medya Kumandası</span>
          </div>
          <span class="badge" id="tv-status" style="color: var(--accent-green);">Açık</span>
        </div>

        <div class="tv-remote-grid">
          <button class="remote-btn power" onclick="tvCmd('power')">⏻ Güç</button>
          <button class="remote-btn" onclick="tvCmd('source')">🔌 Kaynak</button>
          <button class="remote-btn" onclick="tvCmd('mute')">🔇 Sessiz</button>

          <button class="remote-btn" onclick="tvCmd('vol_up')">🔊 Sesi +</button>
          <button class="remote-btn" onclick="tvCmd('ch_up')">🔼 Kanal +</button>
          <button class="remote-btn" onclick="tvCmd('home')">🏠 Home</button>

          <button class="remote-btn" onclick="tvCmd('vol_down')">🔉 Sesi -</button>
          <button class="remote-btn" onclick="tvCmd('ch_down')">🔽 Kanal -</button>
          <button class="remote-btn" onclick="tvCmd('menu')">⚙️ Menü</button>
        </div>
      </div>

    </div>

    <footer>
      <p>ESP32-C6 & NodeMCU ESP8266 Akıllı Ev Otomasyonu © 2026</p>
    </footer>
  </div>

  <script>
    const baseUrl = window.location.origin;

    function updateNodeMCUStatus(isOnline, lastSeenSec) {
      const dot = document.getElementById('nodemcu-dot');
      const text = document.getElementById('nodemcu-status-text');
      if (isOnline) {
        dot.className = 'dot pulse';
        text.innerHTML = `NodeMCU: <strong>Çevrimiçi (${lastSeenSec}sn önce)</strong>`;
      } else {
        dot.className = 'dot offline';
        text.innerHTML = 'NodeMCU: <strong>Çevrimdışı (Güç Yok)</strong>';
      }
    }

    function updateRgbUI(r, g, b, isOff) {
      const orb = document.getElementById('rgb-orb');
      const statusText = document.getElementById('rgb-status-text');
      const valText = document.getElementById('rgb-value-text');
      if (isOff || (r === 0 && g === 0 && b === 0)) {
        orb.style.backgroundColor = '#000000';
        orb.style.boxShadow = '0 0 10px rgba(0,0,0,0.5)';
        statusText.textContent = 'KAPALI';
        statusText.style.color = 'var(--accent-red)';
        valText.textContent = '0, 0, 0 (Kapalı)';
      } else {
        const colorStr = `rgb(${r}, ${g}, ${b})`;
        orb.style.backgroundColor = colorStr;
        orb.style.boxShadow = `0 0 35px ${colorStr}`;
        statusText.textContent = 'AÇIK';
        statusText.style.color = 'var(--accent-green)';
        valText.textContent = `${r}, ${g}, ${b}`;
      }
    }

    function toggleRgb() {
      fetch(`/api/rgb?action=toggle`)
        .then(res => res.json())
        .then(data => updateRgbUI(data.r, data.g, data.b, !data.led))
        .catch(err => console.error('RGB Toggle Hata:', err));
    }

    function setRgb(r, g, b) {
      fetch(`/api/rgb?r=${r}&g=${g}&b=${b}`)
        .then(res => res.json())
        .then(data => updateRgbUI(data.r, data.g, data.b, !data.led))
        .catch(err => console.error('RGB Set Hata:', err));
    }

    function randomRgb() { toggleRgb(); }

    function waterPlant() {
      fetch(`/api/plant?action=water`)
        .then(res => res.json())
        .then(data => alert('🌱 Manuel sulama komutu gönderildi!'))
        .catch(err => console.error('Plant Water Hata:', err));
    }

    function tvCmd(cmd) {
      fetch(`/api/tv?cmd=${cmd}`)
        .then(res => res.json())
        .then(data => console.log('TV Komutu:', cmd, data))
        .catch(err => console.error('TV Komut Hata:', err));
    }

    function klimaCmd(cmd) {
      fetch(`/api/klima?cmd=${cmd}`)
        .then(res => res.json())
        .then(data => {
          const badge = document.getElementById('klima-status-badge');
          if (data.klima_power) {
            badge.textContent = 'AÇIK';
            badge.style.color = 'var(--accent-green)';
          } else {
            badge.textContent = 'KAPALI';
            badge.style.color = 'var(--accent-red)';
          }
        })
        .catch(err => console.error('Klima Komut Hata:', err));
    }

    function fetchStatus() {
      fetch(`/api/status`)
        .then(res => res.json())
        .then(data => {
          if (data.r !== undefined) updateRgbUI(data.r, data.g, data.b, !data.led);
          if (data.nodemcu_online !== undefined) updateNodeMCUStatus(data.nodemcu_online, data.nodemcu_last_seen_sec || 0);
          if (data.klima_power !== undefined) {
            const badge = document.getElementById('klima-status-badge');
            if (data.klima_power) {
              badge.textContent = 'AÇIK';
              badge.style.color = 'var(--accent-green)';
            } else {
              badge.textContent = 'KAPALI';
              badge.style.color = 'var(--accent-red)';
            }
          }
          if (data.moisture !== undefined) {
            document.getElementById('moisture-val').textContent = `${data.moisture}%`;
            document.getElementById('moisture-bar').style.width = `${data.moisture}%`;
          }
          if (data.distance !== undefined) {
            document.getElementById('distance-val').textContent = data.distance;
            document.getElementById('distance-meter-txt').textContent = `${data.distance} cm`;
            const pct = Math.min(100, Math.max(0, data.distance));
            document.getElementById('distance-bar').style.width = `${pct}%`;
          }
        })
        .catch(err => console.log('Sistem durumu alınıyor...'));
    }

    function startVoiceRecognition() {
      const SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;
      const btn = document.getElementById('voice-trigger-btn');
      const statusText = document.getElementById('voice-status-text');
      if (!SpeechRecognition) {
        alert('Tarayıcınız sesli komut özelliğini desteklemiyor. Lütfen Chrome veya Safari kullanın.');
        return;
      }
      const recognition = new SpeechRecognition();
      recognition.lang = 'tr-TR';
      recognition.interimResults = false;

      recognition.onstart = function() {
        btn.classList.add('listening');
        statusText.textContent = '🎙️ Dinleniyor... Şimdi konuşun!';
        statusText.style.color = '#a855f7';
      };

      recognition.onresult = function(event) {
        const transcript = event.results[0][0].transcript.toLowerCase();
        statusText.textContent = `Algılandı: "${transcript}"`;
        statusText.style.color = '#38bdf8';

        if (transcript.includes('klima') && (transcript.includes('aç') || transcript.includes('çalıştır'))) klimaCmd('1');
        else if (transcript.includes('klima') && (transcript.includes('kapat') || transcript.includes('durdur'))) klimaCmd('0');
        else if (transcript.includes('ışık') && (transcript.includes('aç') || transcript.includes('yala'))) setRgb(255, 255, 255);
        else if (transcript.includes('ışık') && (transcript.includes('kapat') || transcript.includes('söndür'))) setRgb(0, 0, 0);
        else if (transcript.includes('mavi')) setRgb(56, 189, 248);
        else if (transcript.includes('kırmızı')) setRgb(239, 68, 68);
        else if (transcript.includes('yeşil')) setRgb(34, 197, 94);
        else if (transcript.includes('mor')) setRgb(168, 85, 247);
        else if (transcript.includes('turuncu')) setRgb(249, 115, 22);
        else if (transcript.includes('sula') || transcript.includes('su ver')) waterPlant();
        else if (transcript.includes('tv') && (transcript.includes('kapat') || transcript.includes('aç'))) tvCmd('power');
        else if (transcript.includes('ses') && transcript.includes('artır')) tvCmd('vol_up');
        else if (transcript.includes('ses') && transcript.includes('azalt')) tvCmd('vol_down');
        else statusText.textContent = `Anlaşılamadı: "${transcript}". Örn: "Klimayı aç", "Işığı aç", "TV kapat"`;
      };

      recognition.onerror = function() {
        btn.classList.remove('listening');
        statusText.textContent = 'Ses algılanamadı. Tekrar denemek için mikrofona basın.';
        statusText.style.color = 'var(--accent-red)';
      };

      recognition.onend = function() { btn.classList.remove('listening'); };
      recognition.start();
    }

    function showPwaInstructions() {
      alert('📲 UYGULAMA YAPMA REHBERİ:\n\n' +
            'iPhone (iOS):\n' +
            '1. Safari\'den http://172.20.10.2 girin.\n' +
            '2. Alttaki Paylaş butonuna basın.\n' +
            '3. "Ana Ekrana Ekle" seçeneğine tıklayın.\n\n' +
            'Android:\n' +
            '1. Chrome\'dan http://172.20.10.2 girin.\n' +
            '2. Sağ üstteki 3 noktaya basın.\n' +
            '3. "Ana Ekrana Ekle" veya "Uygulamayı Yükle" deyin.\n\n' +
            'Cihazınız ana ekranınızda bağımsız mobil uygulama olarak görünecektir!');
    }

    fetchStatus();
    setInterval(fetchStatus, 2000);
  </script>
</body>
</html>
)rawliteral";

#endif // INDEX_HTML_H


