<template>
  <div class="page-container">
    
    <header class="page-header">
      <div class="header-text">
        <h1>System Settings</h1>
        <p>Configure device preferences and manage data.</p>
      </div>
    </header>

    <div v-if="loading" class="loading-state">
      Loading settings...
    </div>

    <div v-else class="settings-grid">
      
      <section class="card settings-card">
        <div class="card-header">
          <h3>Device Configuration</h3>
          <span class="icon">⚙️</span>
        </div>
        
        <div class="form-group">
          <label>Device Name (Label)</label>
          <input type="text" v-model="settings.deviceName" placeholder="e.g. Living Room Outlet">
          <p class="hint">This name will appear on your dashboard.</p>
        </div>

        <div class="form-group">
          <label>Location / Room</label>
          <input type="text" v-model="settings.location" placeholder="e.g. Bedroom">
        </div>
      </section>

      <section class="card settings-card">
        <div class="card-header">
          <h3>Billing & Costs</h3>
          <span class="icon">💰</span>
        </div>
        
        <div class="form-group">
          <label>Electricity Rate (₱ per kWh)</label>
          <div class="input-with-unit">
            <span class="currency">₱</span>
            <input type="number" step="0.01" v-model.number="settings.electricityRate">
          </div>
          <p class="hint">Used to calculate costs on the Billings page.</p>
        </div>
      </section>

      <section class="card settings-card danger-zone">
        <div class="card-header">
          <h3 class="text-danger">Data Management</h3>
          <span class="icon">🗑️</span>
        </div>
        
        <div class="form-group">
          <label>Clear History Logs</label>
          <p class="hint">Permanently remove all recorded voltage/current logs from the database. This cannot be undone.</p>
          
          <button class="btn-danger" @click="clearLogs" :disabled="clearing">
            {{ clearing ? 'Deleting...' : '⚠️ Delete All Logs' }}
          </button>
        </div>
      </section>

      <div class="action-bar">
        <button class="btn-save" @click="saveSettings" :disabled="saving">
          {{ saving ? 'Saving...' : '💾 Save Changes' }}
        </button>
      </div>

    </div>
  </div>
</template>

<script>
import { db } from '../firebase';
import { doc, getDoc, updateDoc, setDoc, collection, getDocs, writeBatch } from "firebase/firestore";

export default {
  name: 'SettingsPage',
  data() {
    return {
      loading: true,
      saving: false,
      clearing: false,
      // Default Settings
      settings: {
        deviceName: '3-Gang Outlet',
        location: 'Main Room',
        electricityRate: 12.00
      }
    }
  },
  async mounted() {
    await this.fetchSettings();
  },
  methods: {
    async fetchSettings() {
      try {
        // We store settings inside the device document itself
        const docRef = doc(db, "devices", "outlet-01");
        const docSnap = await getDoc(docRef);

        if (docSnap.exists()) {
          const data = docSnap.data();
          // Merge existing data with defaults
          if (data.settings) {
            this.settings = { ...this.settings, ...data.settings };
          }
        }
      } catch (error) {
        console.error("Error loading settings:", error);
      } finally {
        this.loading = false;
      }
    },

    async saveSettings() {
      this.saving = true;
      try {
        const docRef = doc(db, "devices", "outlet-01");
        
        // Update the 'settings' field inside the document
        // We use setDoc with merge:true so we don't overwrite the live voltage/current data
        await setDoc(docRef, { 
          settings: this.settings 
        }, { merge: true });

        alert("✅ Settings saved successfully!");
      } catch (error) {
        console.error("Error saving:", error);
        alert("Error saving settings.");
      } finally {
        this.saving = false;
      }
    },

    async clearLogs() {
      if (!confirm("Are you sure? This will delete ALL history logs forever.")) return;

      this.clearing = true;
      try {
        // Firestore requires deleting documents one by one (or in batches)
        const logsRef = collection(db, "logs");
        const snapshot = await getDocs(logsRef);
        
        // Create a batch
        const batch = writeBatch(db);
        snapshot.docs.forEach((doc) => {
          batch.delete(doc.ref);
        });

        // Commit the delete
        await batch.commit();
        
        alert("✅ All logs have been cleared.");
      } catch (error) {
        console.error("Error clearing logs:", error);
        alert("Failed to delete logs.");
      } finally {
        this.clearing = false;
      }
    }
  }
}
</script>

<style scoped>
/* Reuse Standard Layout */
.page-container { padding: 30px; background-color: #F5F7FA; min-height: 100vh; font-family: 'Segoe UI', sans-serif; }
.page-header { margin-bottom: 25px; }
.header-text h1 { margin: 0; color: #2D3436; font-size: 1.8rem; }
.header-text p { margin: 5px 0 0; color: #636e72; }

/* Grid */
.settings-grid { display: flex; flex-direction: column; gap: 20px; max-width: 600px; margin: 0 auto; }

/* Cards */
.card { background: white; padding: 25px; border-radius: 12px; border: 1px solid #E0E0E0; box-shadow: 0 4px 6px rgba(0,0,0,0.02); }
.card-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px; border-bottom: 1px solid #F5F7FA; padding-bottom: 10px; }
.card-header h3 { margin: 0; color: #2D3436; font-size: 1.1rem; }
.icon { font-size: 1.2rem; }

/* Forms */
.form-group { margin-bottom: 20px; }
.form-group label { display: block; font-weight: 600; color: #2D3436; margin-bottom: 8px; font-size: 0.9rem; }
.form-group input { width: 100%; padding: 10px; border: 1px solid #E0E0E0; border-radius: 6px; font-size: 1rem; outline: none; transition: border 0.2s; box-sizing: border-box;}
.form-group input:focus { border-color: #1B5E20; }
.hint { font-size: 0.8rem; color: #999; margin-top: 5px; }

/* Specific Inputs */
.input-with-unit { position: relative; display: flex; align-items: center; }
.currency { position: absolute; left: 10px; color: #636e72; font-weight: bold; }
.input-with-unit input { padding-left: 25px; }

/* Buttons */
.action-bar { margin-top: 10px; display: flex; justify-content: flex-end; }
.btn-save { background-color: #1B5E20; color: white; border: none; padding: 12px 25px; border-radius: 8px; font-weight: 600; cursor: pointer; font-size: 1rem; transition: background 0.2s; }
.btn-save:hover { background-color: #144919; }
.btn-save:disabled { background-color: #ccc; cursor: not-allowed; }

/* Danger Zone */
.danger-zone { border-color: #FFEBEE; background-color: #FFFBFB; }
.text-danger { color: #C62828 !important; }
.btn-danger { background-color: #FFEBEE; color: #C62828; border: 1px solid #FFCDD2; padding: 10px 15px; border-radius: 6px; font-weight: 600; cursor: pointer; width: 100%; text-align: left; }
.btn-danger:hover { background-color: #FFCDD2; }

.loading-state { text-align: center; color: #999; padding-top: 50px; }
</style>