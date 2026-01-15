<template>
  <div class="page-container">
    
    <header class="page-header">
      <div class="header-text">
        <h1>Telemetry Logs</h1>
        <p>Real-time voltage and current readings (2s Interval)</p>
      </div>
      <button class="export-btn">
        <span>📥</span> Export CSV
      </button>
    </header>

    <div class="card table-card">
      <div class="table-actions">
        <input type="text" placeholder="Search logs..." class="search-input" />
        <div class="filter-group">
          <select class="filter-select">
            <option>All Events</option>
            <option>Normal</option>
            <option>Overload</option>
            <option>Voltage Spike</option>
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
            <tr v-for="(log, index) in dummyLogs" :key="index">
              <td class="timestamp">{{ log.timestamp }}</td>
              <td>{{ log.deviceId }}</td>
              <td>{{ log.voltage }} V</td>
              <td>{{ log.current }} A</td>
              <td>{{ log.power }} W</td>
              <td>
                <span :class="['status-badge', log.status.toLowerCase()]">
                  {{ log.status }}
                </span>
              </td>
            </tr>
          </tbody>
        </table>
      </div>
      
      <div class="pagination">
        <button disabled>&laquo; Prev</button>
        <span class="page-info">Page 1 of 24</span>
        <button>Next &raquo;</button>
      </div>
    </div>
    
  </div>
</template>

<script>
export default {
  name: 'LogsPage',
  data() {
    return {
      // Static data simulating 2-second updates
      dummyLogs: [
        { timestamp: '2025-12-06 14:30:10', deviceId: 'Outlet-01', voltage: '220.5', current: '1.20', power: '264.6', status: 'Normal' },
        { timestamp: '2025-12-06 14:30:08', deviceId: 'Outlet-01', voltage: '220.4', current: '1.21', power: '266.6', status: 'Normal' },
        { timestamp: '2025-12-06 14:30:06', deviceId: 'Outlet-01', voltage: '220.1', current: '1.19', power: '261.9', status: 'Normal' },
        { timestamp: '2025-12-06 14:30:04', deviceId: 'Outlet-01', voltage: '228.0', current: '12.5', power: '2850.0', status: 'Overload' },
        { timestamp: '2025-12-06 14:30:02', deviceId: 'Outlet-01', voltage: '219.8', current: '1.20', power: '263.7', status: 'Normal' },
        { timestamp: '2025-12-06 14:30:00', deviceId: 'Outlet-01', voltage: '220.0', current: '1.18', power: '259.6', status: 'Normal' },
        { timestamp: '2025-12-06 14:29:58', deviceId: 'Outlet-01', voltage: '220.1', current: '1.20', power: '264.1', status: 'Normal' },
        { timestamp: '2025-12-06 14:29:56', deviceId: 'Outlet-01', voltage: '220.2', current: '1.20', power: '264.2', status: 'Normal' },
        { timestamp: '2025-12-06 14:29:54', deviceId: 'Outlet-01', voltage: '220.1', current: '1.21', power: '266.3', status: 'Normal' },
        { timestamp: '2025-12-06 14:29:52', deviceId: 'Outlet-01', voltage: '220.3', current: '1.19', power: '262.1', status: 'Normal' },
      ]
    }
  }
}
</script>

<style scoped>
/* 1. Page Container - Fills the router-view area */
.page-container {
  padding: 30px;
  background-color: #F5F7FA; /* Light gray background to match your theme */
  min-height: 100vh; /* Ensures it covers the full height */
  font-family: 'Segoe UI', sans-serif;
}

/* 2. Header Styles */
.page-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 25px;
}

.header-text h1 {
  margin: 0;
  color: #2D3436;
  font-size: 1.8rem;
  font-weight: 700;
}

.header-text p {
  margin: 5px 0 0;
  color: #636e72;
  font-size: 0.9rem;
}

.export-btn {
  background-color: white;
  border: 1px solid #E0E0E0;
  padding: 10px 15px;
  border-radius: 8px;
  color: #2D3436;
  font-weight: 600;
  cursor: pointer;
  display: flex;
  align-items: center;
  gap: 8px;
  transition: background 0.2s;
}

.export-btn:hover {
  background-color: #F1F2F6;
}

/* 3. Table Card Styles */
.card {
  background: white;
  border-radius: 12px;
  box-shadow: 0 4px 6px rgba(0,0,0,0.02);
  border: 1px solid #E0E0E0;
  overflow: hidden; /* Ensures corners clip content */
}

.table-actions {
  padding: 20px;
  border-bottom: 1px solid #F1F2F6;
  display: flex;
  gap: 15px;
}

.search-input, .filter-select {
  padding: 10px;
  border: 1px solid #E0E0E0;
  border-radius: 6px;
  font-size: 0.9rem;
  outline: none;
}

.search-input {
  width: 300px;
}

.search-input:focus, .filter-select:focus {
  border-color: #1B5E20; /* Brand green focus */
}

/* 4. Table Styles */
.table-responsive {
  width: 100%;
  overflow-x: auto;
}

table {
  width: 100%;
  border-collapse: collapse;
  text-align: left;
}

thead th {
  background-color: #F8F9FA;
  padding: 15px 20px;
  font-size: 0.85rem;
  color: #636e72;
  font-weight: 600;
  border-bottom: 1px solid #E0E0E0;
}

tbody td {
  padding: 15px 20px;
  border-bottom: 1px solid #F1F2F6;
  font-size: 0.9rem;
  color: #2D3436;
}

.timestamp {
  font-family: 'Courier New', monospace;
  color: #636e72;
}

/* 5. Status Badges */
.status-badge {
  padding: 4px 10px;
  border-radius: 20px;
  font-size: 0.75rem;
  font-weight: 700;
  text-transform: uppercase;
  display: inline-block;
}

.status-badge.normal {
  background-color: #E8F5E9; /* Light Green */
  color: #1B5E20; /* Dark Green */
}

.status-badge.overload {
  background-color: #FFEBEE; /* Light Red */
  color: #C0392B; /* Dark Red */
}

/* 6. Pagination */
.pagination {
  padding: 15px 20px;
  display: flex;
  justify-content: flex-end;
  align-items: center;
  gap: 15px;
  background-color: #FDFDFD;
}

.pagination button {
  background: white;
  border: 1px solid #E0E0E0;
  padding: 5px 10px;
  border-radius: 4px;
  cursor: pointer;
  color: #636e72;
}

.pagination button:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

.page-info {
  font-size: 0.9rem;
  color: #636e72;
}
</style>