<template>
  <div class="settings-container">
    <header class="settings-header">
      <h1>System Settings</h1>
      <p>Configure your hardware and account preferences.</p>
    </header>

    <div class="settings-grid">
      <section class="settings-card">
        <h3>Hardware Configuration</h3>
        <div class="form-group">
          <label>Device Name</label>
          <input type="text" v-model="settings.deviceName" placeholder="e.g. ESP32_Controller_01">
        </div>
        <div class="form-group">
          <label>Polling Interval (seconds)</label>
          <input type="number" v-model="settings.interval">
        </div>
      </section>

      <section class="settings-card">
        <h3>Network Settings</h3>
        <div class="form-group">
          <label>Server Endpoint (API)</label>
          <input type="text" v-model="settings.apiEndpoint">
        </div>
        <div class="form-group">
          <label>MQTT Broker Address</label>
          <input type="text" v-model="settings.mqttBroker">
        </div>
      </section>

      <section class="settings-card">
        <h3>Account Settings</h3>
        <div class="form-group">
          <label>Admin Email</label>
          <input type="email" v-model="settings.email">
        </div>
        <button class="btn-save" @click="saveSettings">Save Changes</button>
      </section>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue'

// Mock data for now - later this will come from your Backend/Hardware
const settings = ref({
  deviceName: 'Hardware_Node_Alpha',
  interval: 30,
  apiEndpoint: 'http://192.168.1.100:5000',
  mqttBroker: 'broker.hivemq.com',
  email: 'admin@project.local'
})

const saveSettings = () => {
  // We will implement the API call here later
  alert(`Settings saved for ${settings.value.deviceName}!`)
  console.log('Sending to backend:', settings.value)
}
</script>

<style scoped>
.settings-container {
  max-width: 900px;
  margin: 0 auto;
}

.settings-header {
  margin-bottom: 2rem;
}

.settings-header h1 {
  color: #1e293b;
  font-size: 1.8rem;
}

.settings-header p {
  color: #64748b;
}

.settings-grid {
  display: flex;
  flex-direction: column;
  gap: 1.5rem;
}

.settings-card {
  background: white;
  padding: 1.5rem;
  border-radius: 12px;
  box-shadow: 0 1px 3px rgba(0,0,0,0.1);
}

.settings-card h3 {
  margin-bottom: 1.2rem;
  color: #334155;
  border-bottom: 1px solid #f1f5f9;
  padding-bottom: 0.5rem;
}

.form-group {
  margin-bottom: 1rem;
}

.form-group label {
  display: block;
  font-size: 0.875rem;
  font-weight: 600;
  color: #475569;
  margin-bottom: 0.5rem;
}

input {
  width: 100%;
  padding: 0.6rem;
  border: 1px solid #cbd5e1;
  border-radius: 6px;
  font-size: 1rem;
}

input:focus {
  outline: none;
  border-color: #38bdf8;
  ring: 2px solid #e0f2fe;
}

.btn-save {
  background-color: #38bdf8;
  color: white;
  border: none;
  padding: 0.8rem 1.5rem;
  border-radius: 6px;
  font-weight: bold;
  cursor: pointer;
  transition: background 0.2s;
}

.btn-save:hover {
  background-color: #0ea5e9;
}
</style>