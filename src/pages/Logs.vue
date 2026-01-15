<template>
  <div class="page-container">
    
    <header class="page-header">
      <div class="header-text">
        <h1>Telemetry Logs</h1>
        <p>Real-time stream (Displaying last 100 records)</p>
      </div>
      <button class="export-btn" @click="exportCSV">
        <span>📥</span> Export CSV
      </button>
    </header>

    <div class="card table-card">
      <div class="table-actions">
        <input 
          type="text" 
          v-model="searchQuery" 
          placeholder="Search logs..." 
          class="search-input" 
        />
        
        <div class="filter-group">
          <select v-model="statusFilter" class="filter-select">
            <option value="All">All Events</option>
            <option value="NORMAL">Normal</option>
            <option value="ALARM">Overload / Alarm</option>
          </select>
        </div>
      </div>

      <div class="table-responsive">
        <table>
          <thead>
            <tr>
              <th>Timestamp</th>
              <th>Device ID</th>
              <th>Voltage (V)</th>
              <th>Current (A)</th>
              <th>Power (W)</th>
              <th>Status</th>
            </tr>
          </thead>
          <tbody>
            <tr v-if="loading">
              <td colspan="6" style="text-align: center; padding: 20px; color: #999;">
                Connecting to live stream...
              </td>
            </tr>

            <tr v-else-if="filteredLogs.length === 0">
              <td colspan="6" style="text-align: center; padding: 20px; color: #999;">
                No logs found matching your filters.
              </td>
            </tr>

            <tr v-for="log in filteredLogs" :key="log.id">
              <td class="timestamp">{{ formatDate(log.timestamp) }}</td>
              <td>{{ log.deviceId }}</td>
              <td>{{ log.voltage }} V</td>
              <td>{{ log.current }} A</td>
              <td>{{ (log.voltage * log.current).toFixed(1) }} W</td>
              <td>
                <span :class="['status-badge', getStatusClass(log.status)]">
                  {{ log.status }}
                </span>
              </td>
            </tr>
          </tbody>
        </table>
      </div>
      
      <div class="pagination">
        <span class="page-info">Showing latest {{ filteredLogs.length }} records</span>
      </div>
    </div>
    
  </div>
</template>

<script>
import { db } from '../firebase'; 
import { collection, query, orderBy, limit, onSnapshot } from "firebase/firestore";

export default {
  name: 'LogsPage',
  data() {
    return {
      logs: [],
      loading: true,
      searchQuery: '',
      statusFilter: 'All',
      unsub: null
    }
  },
  computed: {
    // Client-side filtering for instant search results
    filteredLogs() {
      return this.logs.filter(log => {
        // 1. Filter by Status
        if (this.statusFilter !== 'All' && log.status !== this.statusFilter) {
          return false;
        }

        // 2. Filter by Search Query (Date, ID, or Values)
        if (this.searchQuery) {
          const searchLower = this.searchQuery.toLowerCase();
          const dateStr = this.formatDate(log.timestamp).toLowerCase();
          
          return (
            dateStr.includes(searchLower) ||
            log.deviceId.toLowerCase().includes(searchLower) ||
            log.status.toLowerCase().includes(searchLower)
          );
        }

        return true;
      });
    }
  },
  mounted() {
    // --- LIVE LISTENER ---
    // This connects to Firestore and stays open. 
    // Whenever a new log is added, this runs automatically.
    const q = query(
      collection(db, "logs"), 
      orderBy("timestamp", "desc"), 
      limit(100) // Keep the table fast by only showing last 100
    );

    this.unsub = onSnapshot(q, (snapshot) => {
      this.logs = snapshot.docs.map(doc => ({
        id: doc.id,
        ...doc.data()
      }));
      this.loading = false;
    }, (error) => {
      console.error("Error fetching logs:", error);
      this.loading = false;
    });
  },
  beforeUnmount() {
    // Stop listening when user leaves the page to save data usage
    if (this.unsub) {
      this.unsub();
    }
  },
  methods: {
    formatDate(timestamp) {
      if (!timestamp) return '--';
      // Convert Firebase Timestamp to JS Date
      const date = timestamp.toDate ? timestamp.toDate() : new Date(timestamp);
      return date.toLocaleString('en-US'); // e.g. "1/15/2026, 2:30:00 PM"
    },
    
    getStatusClass(status) {
      // Returns CSS class based on status text
      if (status === 'NORMAL') return 'normal';
      if (status === 'ALARM') return 'overload';
      return '';
    },

    exportCSV() {
      // 1. Create CSV Header
      let csvContent = "data:text/csv;charset=utf-8,";
      csvContent += "Timestamp,Device ID,Voltage (V),Current (A),Power (W),Status\n";

      // 2. Add Rows
      this.filteredLogs.forEach(log => {
        const time = this.formatDate(log.timestamp).replace(',', ''); // Remove commas for CSV safety
        const power = (log.voltage * log.current).toFixed(1);
        csvContent += `${time},${log.deviceId},${log.voltage},${log.current},${power},${log.status}\n`;
      });

      // 3. Trigger Download
      const encodedUri = encodeURI(csvContent);
      const link = document.createElement("a");
      link.setAttribute("href", encodedUri);
      link.setAttribute("download", "telemetry_logs.csv");
      document.body.appendChild(link);
      link.click();
    }
  }
}
</script>

<style scoped>
/* Reuse existing styles */
.page-container { padding: 30px; background-color: #F5F7FA; min-height: 100vh; font-family: 'Segoe UI', sans-serif; }
.page-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 25px; }
.header-text h1 { margin: 0; color: #2D3436; font-size: 1.8rem; font-weight: 700; }
.header-text p { margin: 5px 0 0; color: #636e72; font-size: 0.9rem; }

.export-btn { background-color: white; border: 1px solid #E0E0E0; padding: 10px 15px; border-radius: 8px; color: #2D3436; font-weight: 600; cursor: pointer; display: flex; align-items: center; gap: 8px; transition: background 0.2s; }
.export-btn:hover { background-color: #F1F2F6; }

.card { background: white; border-radius: 12px; box-shadow: 0 4px 6px rgba(0,0,0,0.02); border: 1px solid #E0E0E0; overflow: hidden; }
.table-actions { padding: 20px; border-bottom: 1px solid #F1F2F6; display: flex; gap: 15px; }
.search-input, .filter-select { padding: 10px; border: 1px solid #E0E0E0; border-radius: 6px; font-size: 0.9rem; outline: none; }
.search-input { width: 300px; }
.search-input:focus, .filter-select:focus { border-color: #1B5E20; }

.table-responsive { width: 100%; overflow-x: auto; height: 500px; overflow-y: auto; } /* Added scrolling height */
table { width: 100%; border-collapse: collapse; text-align: left; }
thead th { background-color: #F8F9FA; padding: 15px 20px; font-size: 0.85rem; color: #636e72; font-weight: 600; border-bottom: 1px solid #E0E0E0; position: sticky; top: 0; } /* Sticky header */
tbody td { padding: 15px 20px; border-bottom: 1px solid #F1F2F6; font-size: 0.9rem; color: #2D3436; }
.timestamp { font-family: 'Courier New', monospace; color: #636e72; }

.status-badge { padding: 4px 10px; border-radius: 20px; font-size: 0.75rem; font-weight: 700; text-transform: uppercase; display: inline-block; }
.status-badge.normal { background-color: #E8F5E9; color: #1B5E20; }
.status-badge.overload { background-color: #FFEBEE; color: #C0392B; }

.pagination { padding: 15px 20px; display: flex; justify-content: flex-end; align-items: center; gap: 15px; background-color: #FDFDFD; }
.page-info { font-size: 0.9rem; color: #636e72; }
</style>