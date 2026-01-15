const mqtt = require('mqtt');
const admin = require('firebase-admin');

// Load your downloaded Firebase key
// Make sure the file name matches exactly!
const serviceAccount = require('./serviceAccountKey.json');

// 1. Initialize Firebase
admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  projectId: "voltagecurrent-monitoring"
});

const db = admin.firestore();

// 2. HiveMQ Connection Details (from your screenshots)
const MQTT_URL = 'mqtts://b9f1563767a940eaaeb700bbbcb6f8c8.s1.eu.hivemq.cloud:8883';
const options = {
  username: 'caseeey',
  password: 'Kuyamopio17',
  rejectUnauthorized: false // Required for HiveMQ Cloud
};

console.log("Connecting to HiveMQ Broker...");
const client = mqtt.connect(MQTT_URL, options);

// 3. Connect & Subscribe
client.on('connect', () => {
  console.log("✅ Connected to HiveMQ!");
  
  // Subscribe to the topic your Arduino writes to
  client.subscribe('home/telemetry', (err) => {
    if (!err) {
      console.log("📡 Listening to 'home/telemetry'...");
    } else {
      console.error("❌ Subscription error:", err);
    }
  });
});

// 4. Listen for Messages & Save to Firebase
client.on('message', async (topic, message) => {
  const payloadStr = message.toString();
  console.log(`📥 Received: ${payloadStr}`);

  try {
    // Convert string to JSON object
    const data = JSON.parse(payloadStr);

    // Prepare data for database
    const logEntry = {
      ...data,
      deviceId: 'outlet-01',
      timestamp: admin.firestore.FieldValue.serverTimestamp() // Auto-date
    };

    // A. Add to "logs" (History)
    await db.collection('logs').add(logEntry);
    
    // B. Update "devices/outlet-01" (Current Status)
    await db.collection('devices').doc('outlet-01').set(logEntry);

    console.log("💾 Saved to Firebase!");

  } catch (error) {
    console.error("❌ Error parsing/saving:", error);
  }
});