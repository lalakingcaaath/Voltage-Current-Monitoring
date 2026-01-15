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
          <input type="number" v-model.number="electricityRate" class="rate-input" @change="recalculateCosts" />
          <span class="unit">/ kWh</span>
        </div>
      </div>
    </header>

    <section class="stats-grid">
      <div class="card stat-card">
        <div class="icon-wrapper today-icon">📅</div>
        <div class="stat-info">
          <span class="label">Today's Usage</span>
          <h2 class="value">{{ todayKwh }} kWh</h2>
          <span class="cost-preview">≈ ₱ {{ todayCost }}</span>
        </div>
      </div>

      <div class="card stat-card">
        <div class="icon-wrapper yesterday-icon">⏮</div>
        <div class="stat-info">
          <span class="label">Yesterday</span>
          <h2 class="value">{{ yesterdayKwh }} kWh</h2>
          <span class="cost-preview">≈ ₱ {{ yesterdayCost }}</span>
        </div>
      </div>

      <div class="card stat-card">
        <div class="icon-wrapper project-icon">📊</div>
        <div class="stat-info">
          <span class="label">Projected (Month)</span>
          <h2 class="value">₱ {{ projectedCost }}</h2>
          <span class="subtext">Based on today's avg</span>
        </div>
      </div>
    </section>

    <section class="chart-section">
      <div class="card chart-card">
        <div class="card-header">
          <h3>Consumption History</h3>
          <div class="legend">
            <span class="dot usage"></span> kWh
          </div>
        </div>
        
        <div class="bar-chart-container">
          <div v-if="loading" class="loading-msg">Loading History...</div>
          
          <div v-else-if="dailyData.length === 0" class="no-data-msg">
            No history data yet.
          </div>

          <div v-else class="bar-group" v-for="(day, index) in dailyData" :key="index">
            <div class="bar-wrapper">
              <div class="bar" :style="{ height: (day.kwh / maxKwh * 100) + '%' }">
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
          <button class="export-link" @click="exportCSV">Export CSV</button>
        </div>
        <div class="table-responsive">
          <table>
            <thead>
              <tr>
                <th>Date</th>
                <th>Energy Consumed</th>
                <th>Est. Cost (₱{{ electricityRate }}/kWh)</th>
                <th>Usage Level</th>
              </tr>
            </thead>
            <tbody>
              <tr v-for="(day, index) in dailyData" :key="index">
                <td>{{ day.date }}</td>
                <td><strong>{{ day.kwh }} kWh</strong></td>
                <td>₱ {{ day.cost }}</td>
                <td>
                  <span :class="['status-badge', getUsageClass(day.kwh)]">
                    {{ getUsageLabel(day.kwh) }}
                  </span>
                </td>
              </tr>
              <tr v-if="dailyData.length === 0">
                <td colspan="4" style="text-align:center; padding: 20px;">
                  No data recorded yet.
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
import { db } from '../firebase'; 
// FIXED: Added 'doc' and 'getDoc' to imports
import { collection, query, orderBy, limit, getDocs, doc, getDoc } from "firebase/firestore";

export default {
  name: 'BillingsPage',
  data() {
    return {
      loading: true,
      electricityRate: 12.00, // Default rate
      rawLogs: [],
      dailyData: [], 
      
      // Stats
      todayKwh: 0,
      todayCost: 0,
      yesterdayKwh: 0,
      yesterdayCost: 0,
      maxKwh: 1, // Avoid divide by zero
    }
  },
  computed: {
    projectedCost() {
      // Simple projection: Today's Cost x 30 days
      return (parseFloat(this.todayCost) * 30).toFixed(2);
    }
  },
  async mounted() {
    try {
      // 1. Get the saved Electricity Rate from Settings
      const docRef = doc(db, "devices", "outlet-01");
      const docSnap = await getDoc(docRef);
      
      if (docSnap.exists()) {
        const data = docSnap.data();
        if (data.settings && data.settings.electricityRate) {
          this.electricityRate = data.settings.electricityRate;
        }
      }
    } catch (e) {
      console.log("Using default rate (12.00)");
    }

    // 2. NOW fetch the history calculations using that rate
    await this.fetchHistory();
  },
  methods: {
    async fetchHistory() {
      this.loading = true;
      try {
        const q = query(collection(db, "logs"), orderBy("timestamp", "desc"), limit(2000));
        const querySnapshot = await getDocs(q);
        
        const tempLogs = [];
        querySnapshot.forEach((doc) => {
          tempLogs.push(doc.data());
        });

        this.processLogsToDaily(tempLogs);
        
      } catch (error) {
        console.error("Error fetching logs:", error);
      } finally {
        this.loading = false;
      }
    },

    processLogsToDaily(logs) {
      const grouped = {};

      logs.forEach(log => {
        if (!log.timestamp) return;
        
        // Convert Firebase timestamp to JS Date
        const dateObj = log.timestamp.toDate(); 
        const dateKey = dateObj.toLocaleDateString('en-US'); // "1/15/2026"
        
        if (!grouped[dateKey]) {
          grouped[dateKey] = {
            dateObj: dateObj,
            totalWatts: 0,
            count: 0
          };
        }
        
        const power = log.power || (log.voltage * log.current);
        grouped[dateKey].totalWatts += power;
        grouped[dateKey].count++;
      });

      const processedData = Object.keys(grouped).map(dateKey => {
        const day = grouped[dateKey];
        
        // kWh Calculation
        const energyJoules = day.totalWatts * 2; // Assuming 2 second interval
        const kwh = energyJoules / 3600000;

        return {
          date: dateKey,
          shortDate: day.dateObj.toLocaleDateString('en-US', { weekday: 'short' }),
          kwh: kwh.toFixed(3),
          cost: (kwh * this.electricityRate).toFixed(2),
          rawValue: kwh
        };
      });

      processedData.sort((a, b) => new Date(a.date) - new Date(b.date));

      this.dailyData = processedData;
      this.maxKwh = Math.max(...processedData.map(d => d.rawValue), 0.1); 

      const todayKey = new Date().toLocaleDateString('en-US');
      const todayEntry = processedData.find(d => d.date === todayKey);
      
      if (todayEntry) {
        this.todayKwh = todayEntry.kwh;
        this.todayCost = todayEntry.cost;
      }
    },

    recalculateCosts() {
      this.dailyData.forEach(day => {
        day.cost = (day.rawValue * this.electricityRate).toFixed(2);
      });
      this.todayCost = (this.todayKwh * this.electricityRate).toFixed(2);
    },

    getUsageClass(kwh) {
      if (kwh > 5.0) return 'high';
      if (kwh > 2.0) return 'moderate';
      return 'low';
    },
    
    getUsageLabel(kwh) {
      if (kwh > 5.0) return 'High';
      if (kwh > 2.0) return 'Moderate';
      return 'Low';
    },

    exportCSV() {
      let csvContent = "data:text/csv;charset=utf-8,";
      csvContent += "Date,Energy (kWh),Cost (PHP)\n";
      
      this.dailyData.forEach(row => {
        csvContent += `${row.date},${row.kwh},${row.cost}\n`;
      });

      const encodedUri = encodeURI(csvContent);
      const link = document.createElement("a");
      link.setAttribute("href", encodedUri);
      link.setAttribute("download", "energy_report.csv");
      document.body.appendChild(link);
      link.click();
    }
  }
}
</script>

<style scoped>
/* Reuse your exact CSS */
.page-container { padding: 30px; background-color: #F5F7FA; min-height: 100vh; font-family: 'Segoe UI', sans-serif; }
.page-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 25px; }
.header-text h1 { margin: 0; color: #2D3436; font-size: 1.8rem; }
.rate-control { background: white; border: 1px solid #E0E0E0; padding: 8px 15px; border-radius: 8px; display: flex; align-items: center; gap: 10px; }
.rate-input { width: 60px; border: none; border-bottom: 2px solid #E0E0E0; font-size: 1rem; font-weight: 700; color: #1B5E20; text-align: center; outline: none; }
.stats-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(240px, 1fr)); gap: 20px; margin-bottom: 30px; }
.stat-card { background: white; padding: 20px; border-radius: 12px; border: 1px solid #E0E0E0; display: flex; align-items: center; gap: 15px; }
.icon-wrapper { width: 45px; height: 45px; border-radius: 10px; display: flex; align-items: center; justify-content: center; font-size: 1.4rem; }
.today-icon { background: #E8F5E9; color: #1B5E20; }
.yesterday-icon { background: #F3E5F5; color: #8E24AA; }
.project-icon { background: #E3F2FD; color: #1565C0; }
.stat-info .value { font-size: 1.4rem; margin: 2px 0; color: #2D3436; }
.chart-card, .table-card { background: white; border-radius: 12px; border: 1px solid #E0E0E0; padding: 25px; margin-bottom: 30px; }
.card-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px; }
.bar-chart-container { display: flex; align-items: flex-end; justify-content: space-around; height: 220px; padding-top: 20px; border-bottom: 1px solid #E0E0E0; }
.bar-group { display: flex; flex-direction: column; align-items: center; gap: 10px; height: 100%; justify-content: flex-end; width: 100%; }
.bar-wrapper { height: 100%; width: 100%; display: flex; align-items: flex-end; justify-content: center; }
.bar { width: 40px; background-color: #1B5E20; border-radius: 6px 6px 0 0; position: relative; transition: height 0.5s ease; cursor: pointer; min-height: 4px; }
.bar:hover .tooltip { display: block; }
.tooltip { display: none; position: absolute; bottom: 100%; left: 50%; transform: translateX(-50%); background: #2D3436; color: white; padding: 8px; border-radius: 6px; font-size: 0.75rem; width: 100px; text-align: center; margin-bottom: 8px; z-index: 10; }
.table-responsive { overflow-x: auto; margin-top: 10px; }
table { width: 100%; border-collapse: collapse; text-align: left; }
thead th { background-color: #F8F9FA; padding: 12px 15px; font-size: 0.85rem; color: #636e72; font-weight: 600; }
tbody td { padding: 12px 15px; border-bottom: 1px solid #F1F2F6; font-size: 0.9rem; color: #2D3436; }
.status-badge { padding: 4px 10px; border-radius: 20px; font-size: 0.75rem; font-weight: 700; text-transform: uppercase; }
.status-badge.low { background-color: #E8F5E9; color: #1B5E20; }
.status-badge.moderate { background-color: #FFF3E0; color: #F57C00; }
.status-badge.high { background-color: #FFEBEE; color: #C0392B; }
.loading-msg, .no-data-msg { width: 100%; text-align: center; color: #999; padding-bottom: 20px;}
.export-link { background: none; border: none; color: #1B5E20; font-weight: 600; cursor: pointer; }
</style>