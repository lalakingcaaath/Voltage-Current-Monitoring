<template>
  <div class="page-container">
    
    <header class="page-header">
      <div class="header-text">
        <h1>Daily Consumption & Cost</h1>
        <p>Monitor your energy usage day by day.</p>
      </div>
      
      <div class="rate-control">
        <span class="rate-label">Electricity Rate:</span>
        <div class="rate-input-group">
          <span class="currency">₱</span>
          <input type="number" value="12.00" class="rate-input" />
          <span class="unit">/ kWh</span>
        </div>
      </div>
    </header>

    <section class="stats-grid">
      <div class="card stat-card">
        <div class="icon-wrapper today-icon">📅</div>
        <div class="stat-info">
          <span class="label">Today's Usage</span>
          <h2 class="value">4.2 kWh</h2>
          <span class="cost-preview">≈ ₱ 50.40</span>
        </div>
      </div>

      <div class="card stat-card">
        <div class="icon-wrapper yesterday-icon">⏮</div>
        <div class="stat-info">
          <span class="label">Yesterday</span>
          <h2 class="value">3.8 kWh</h2>
          <span class="cost-preview">≈ ₱ 45.60</span>
        </div>
      </div>

      <div class="card stat-card">
        <div class="icon-wrapper project-icon">📊</div>
        <div class="stat-info">
          <span class="label">Projected (Month)</span>
          <h2 class="value">₱ 1,512.00</h2>
          <span class="subtext">Based on avg. usage</span>
        </div>
      </div>
    </section>

    <section class="chart-section">
      <div class="card chart-card">
        <div class="card-header">
          <h3>Last 7 Days Consumption</h3>
          <div class="legend">
            <span class="dot usage"></span> kWh
            <span class="dot cost"></span> Cost (₱)
          </div>
        </div>
        
        <div class="bar-chart-container">
          <div class="bar-group" v-for="(day, index) in dailyData" :key="index">
            <div class="bar-wrapper">
              <div class="bar" :style="{ height: day.height + '%' }">
                <div class="tooltip">
                  <strong>{{ day.date }}</strong><br>
                  {{ day.kwh }} kWh<br>
                  ₱{{ day.cost }}
                </div>
              </div>
            </div>
            <span class="day-label">{{ day.shortDate }}</span>
          </div>
        </div>
      </div>
    </section>

    <section class="table-section">
      <div class="card table-card">
        <div class="card-header">
          <h3>Daily Logs</h3>
          <button class="export-link">Export CSV</button>
        </div>
        <div class="table-responsive">
          <table>
            <thead>
              <tr>
                <th>Date</th>
                <th>Energy Consumed</th>
                <th>Est. Cost (₱12/kWh)</th>
                <th>Usage Level</th>
              </tr>
            </thead>
            <tbody>
              <tr v-for="(day, index) in dailyData" :key="index">
                <td>{{ day.date }}</td>
                <td>
                  <strong>{{ day.kwh }} kWh</strong>
                </td>
                <td>₱ {{ day.cost }}</td>
                <td>
                  <span :class="['status-badge', getUsageClass(day.kwh)]">
                    {{ getUsageLabel(day.kwh) }}
                  </span>
                </td>
              </tr>
            </tbody>
          </table>
        </div>
      </div>
    </section>

  </div>
</template>

<script>
export default {
  name: 'BillingsPage',
  data() {
    return {
      // Static Data: Simulating the last 7 days of usage
      dailyData: [
        { date: 'Dec 06, 2025', shortDate: 'Mon', kwh: 4.2, cost: '50.40', height: 60 },
        { date: 'Dec 05, 2025', shortDate: 'Sun', kwh: 3.5, cost: '42.00', height: 50 },
        { date: 'Dec 04, 2025', shortDate: 'Sat', kwh: 5.8, cost: '69.60', height: 85 }, // High usage
        { date: 'Dec 03, 2025', shortDate: 'Fri', kwh: 4.0, cost: '48.00', height: 55 },
        { date: 'Dec 02, 2025', shortDate: 'Thu', kwh: 6.2, cost: '74.40', height: 90 }, // Peak
        { date: 'Dec 01, 2025', shortDate: 'Wed', kwh: 5.5, cost: '66.00', height: 75 },
        { date: 'Nov 30, 2025', shortDate: 'Tue', kwh: 4.8, cost: '57.60', height: 65 },
      ]
    }
  },
  methods: {
    // Helper to style the badges
    getUsageClass(kwh) {
      if (kwh > 6.0) return 'high';
      if (kwh > 4.5) return 'moderate';
      return 'low';
    },
    getUsageLabel(kwh) {
      if (kwh > 6.0) return 'High';
      if (kwh > 4.5) return 'Moderate';
      return 'Low';
    }
  }
}
</script>

<style scoped>
/* --- 1. Layout --- */
.page-container {
  padding: 30px;
  background-color: #F5F7FA;
  min-height: 100vh;
  font-family: 'Segoe UI', sans-serif;
}

/* --- 2. Header & Rate Control --- */
.page-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 25px;
}

.header-text h1 { margin: 0; color: #2D3436; font-size: 1.8rem; }
.header-text p { margin: 5px 0 0; color: #636e72; }

.rate-control {
  background: white;
  border: 1px solid #E0E0E0;
  padding: 8px 15px;
  border-radius: 8px;
  display: flex;
  align-items: center;
  gap: 10px;
}

.rate-label { font-size: 0.9rem; color: #636e72; font-weight: 600; }
.rate-input-group { display: flex; align-items: center; }
.currency { font-weight: bold; color: #2D3436; margin-right: 4px; }

.rate-input {
  width: 60px;
  border: none;
  border-bottom: 2px solid #E0E0E0;
  font-size: 1rem;
  font-weight: 700;
  color: #1B5E20;
  text-align: center;
  outline: none;
}

.rate-input:focus { border-color: #1B5E20; }
.unit { font-size: 0.8rem; color: #b2bec3; margin-left: 5px; }

/* --- 3. Stats Cards --- */
.stats-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(240px, 1fr));
  gap: 20px;
  margin-bottom: 30px;
}

.stat-card {
  background: white;
  padding: 20px;
  border-radius: 12px;
  border: 1px solid #E0E0E0;
  display: flex;
  align-items: center;
  gap: 15px;
}

.icon-wrapper {
  width: 45px;
  height: 45px;
  border-radius: 10px;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 1.4rem;
}

.today-icon { background: #E8F5E9; color: #1B5E20; }
.yesterday-icon { background: #F3E5F5; color: #8E24AA; }
.project-icon { background: #E3F2FD; color: #1565C0; }

.stat-info .label { font-size: 0.85rem; color: #636e72; display: block; }
.stat-info .value { font-size: 1.4rem; margin: 2px 0; color: #2D3436; }
.stat-info .cost-preview { font-size: 0.85rem; color: #1B5E20; font-weight: 600; }
.stat-info .subtext { font-size: 0.8rem; color: #b2bec3; }

/* --- 4. Chart Section --- */
.chart-section { margin-bottom: 30px; }

.chart-card {
  background: white;
  border-radius: 12px;
  border: 1px solid #E0E0E0;
  padding: 25px;
}

.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
}

.card-header h3 { margin: 0; font-size: 1.1rem; color: #2D3436; }

.legend { font-size: 0.8rem; color: #636e72; display: flex; gap: 10px; }
.dot { width: 10px; height: 10px; border-radius: 50%; display: inline-block; }
.dot.usage { background-color: #1B5E20; }

/* Bar Chart */
.bar-chart-container {
  display: flex;
  align-items: flex-end;
  justify-content: space-around;
  height: 220px;
  padding-top: 20px;
  border-bottom: 1px solid #E0E0E0;
}

.bar-group {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 10px;
  height: 100%;
  justify-content: flex-end;
  width: 100%;
}

.bar-wrapper {
  height: 100%;
  width: 100%;
  display: flex;
  align-items: flex-end;
  justify-content: center;
}

.bar {
  width: 40px;
  background-color: #1B5E20; /* Brand Green */
  border-radius: 6px 6px 0 0;
  position: relative;
  transition: opacity 0.2s;
  cursor: pointer;
}

.bar:hover { opacity: 0.8; }

/* Tooltip */
.tooltip {
  display: none;
  position: absolute;
  bottom: 100%;
  left: 50%;
  transform: translateX(-50%);
  background: #2D3436;
  color: white;
  padding: 8px;
  border-radius: 6px;
  font-size: 0.75rem;
  width: 100px;
  text-align: center;
  margin-bottom: 8px;
  z-index: 10;
}
.tooltip::after {
  content: " ";
  position: absolute;
  top: 100%; /* At the bottom of the tooltip */
  left: 50%;
  margin-left: -5px;
  border-width: 5px;
  border-style: solid;
  border-color: #2D3436 transparent transparent transparent;
}

.bar:hover .tooltip { display: block; }
.day-label { font-size: 0.85rem; color: #636e72; font-weight: 600; }

/* --- 5. Table Section --- */
.table-card {
  background: white;
  border-radius: 12px;
  border: 1px solid #E0E0E0;
  padding: 20px;
  overflow: hidden;
}

.export-link {
  background: none;
  border: none;
  color: #1B5E20;
  font-weight: 600;
  cursor: pointer;
}

.table-responsive { overflow-x: auto; margin-top: 10px; }

table {
  width: 100%;
  border-collapse: collapse;
  text-align: left;
}

thead th {
  background-color: #F8F9FA;
  padding: 12px 15px;
  font-size: 0.85rem;
  color: #636e72;
  font-weight: 600;
}

tbody td {
  padding: 12px 15px;
  border-bottom: 1px solid #F1F2F6;
  font-size: 0.9rem;
  color: #2D3436;
}

/* Usage Badges */
.status-badge {
  padding: 4px 10px;
  border-radius: 20px;
  font-size: 0.75rem;
  font-weight: 700;
  text-transform: uppercase;
}

.status-badge.low { background-color: #E8F5E9; color: #1B5E20; }      /* Green */
.status-badge.moderate { background-color: #FFF3E0; color: #F57C00; } /* Orange */
.status-badge.high { background-color: #FFEBEE; color: #C0392B; }     /* Red */
</style>