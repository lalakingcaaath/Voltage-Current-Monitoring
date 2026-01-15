<template>
  <div class="page-container">
    
    <header class="page-header">
      <div class="header-text">
        <h1>System Dashboard</h1>
        <p>Live monitoring: 3-Gang Extension Outlet</p>
      </div>
      <div class="status-indicators">
        <div :class="['status-badge', isOnline ? 'online' : 'offline']">
          <span class="dot"></span> {{ isOnline ? 'Online' : 'Connecting...' }}
        </div>
        <div :class="['status-badge', status === 'ALARM' ? 'danger' : 'safe']">
          <span class="shield">🛡</span> {{ status === 'ALARM' ? 'OVERLOAD' : 'Protection Active' }}
        </div>
      </div>
    </header>

    <section class="metrics-grid">
      <div class="card metric-card">
        <div class="metric-header">
          <span class="label">Voltage (AC)</span>
          <span class="icon">⚡</span>
        </div>
        <div class="metric-value">
          <h2>{{ voltage }} <span class="unit">V</span></h2>
        </div>
        <div class="metric-footer">
          <span class="status-text normal">Normal Range</span>
        </div>
      </div>

      <div class="card metric-card">
        <div class="metric-header">
          <span class="label">Current (RMS)</span>
          <span class="icon">🔌</span>
        </div>
        <div class="metric-value">
          <h2>{{ current }} <span class="unit">A</span></h2>
        </div>
        <div class="metric-footer">
          <span v-if="current > 0.5" class="status-text warning">High Load</span>
          <span v-else class="status-text normal">Optimal</span>
        </div>
      </div>

      <div class="card metric-card">
        <div class="metric-header">
          <span class="label">Real-Time Power</span>
          <span class="icon">💡</span>
        </div>
        <div class="metric-value">
          <h2>{{ power }} <span class="unit">W</span></h2>
        </div>
        <div class="metric-footer">
          <span class="subtext">Live Calculation</span>
        </div>
      </div>
    </section>

    <section class="dashboard-main">
      
      <div class="card graph-card">
        <div class="card-header">
          <h3>Power Usage (Last 1 Hour)</h3>
          <div class="live-indicator">
            <span class="pulse"></span> Updating
          </div>
        </div>
        
        <div class="chart-container">
          <div class="y-labels">
            <span>{{ maxGraphPower }}W</span>
            <span>{{ (maxGraphPower / 2).toFixed(0) }}W</span>
            <span>0W</span>
          </div>

          <svg viewBox="0 0 500 150" class="line-chart">
            <line x1="0" y1="0" x2="500" y2="0" stroke="#eee" />
            <line x1="0" y1="75" x2="500" y2="75" stroke="#eee" />
            <line x1="0" y1="150" x2="500" y2="150" stroke="#eee" />
            
            <line x1="125" y1="0" x2="125" y2="150" stroke="#f5f5f5" stroke-dasharray="4" />
            <line x1="250" y1="0" x2="250" y2="150" stroke="#f5f5f5" stroke-dasharray="4" />
            <line x1="375" y1="0" x2="375" y2="150" stroke="#f5f5f5" stroke-dasharray="4" />

            <polyline 
              fill="none" 
              stroke="#1B5E20" 
              stroke-width="2" 
              stroke-linejoin="round"
              :points="graphPoints" 
            />
            
            <polygon 
              fill="rgba(27, 94, 32, 0.1)" 
              :points="graphPolygon" 
            />
          </svg>
        </div>
        
        <div class="x-labels">
          <span>60m ago</span>
          <span>45m</span>
          <span>30m</span>
          <span>15m</span>
          <span>Now</span>
        </div>
      </div>

      <div class="card control-card">
        <div class="card-header">
          <h3>Outlet Controls</h3>
          <span class="subtext">Remote Relay Switch</span>
        </div>
        <div class="switch-list">
          <div class="switch-row">
            <div class="switch-info">
              <span class="switch-label">Outlet 1</span>
              <span class="load-name">Main Load</span>
            </div>
            <label class="toggle-switch">
              <input type="checkbox" checked>
              <span class="slider round"></span>
            </label>
          </div>
        </div>
        <button class="emergency-btn">⚠️ EMERGENCY OFF (ALL)</button>
      </div>

    </section>
  </div>
</template>

<script>
import { db } from '../firebase'; 
import { doc, onSnapshot } from "firebase/firestore";

export default {
  name: 'DashboardPage',
  data() {
    return {
      voltage: 0,
      current: 0,
      power: 0,
      status: 'Loading...',
      isOnline: false,
      unsub: null,
      
      // Graph Data
      powerHistory: [], 
      maxGraphPower: 30, // Starts at 0-30W scale
      windowSize: 60 * 60 * 1000 // 1 Hour in milliseconds
    }
  },
  computed: {
    graphPoints() {
      // If no data, draw a flat line at 0
      if (this.powerHistory.length === 0) return "0,150 500,150";
      
      const now = Date.now();
      const oneHourAgo = now - this.windowSize;

      // Filter points older than 1 hour
      // (We do this in computed so the display updates, even if data isn't arriving)
      const validPoints = this.powerHistory.filter(p => p.timestamp > oneHourAgo);

      return validPoints.map(point => {
        // Map Time (X-Axis): 
        // 1 hour ago -> 0px (Left)
        // Now -> 500px (Right)
        const timeDiff = point.timestamp - oneHourAgo;
        const x = (timeDiff / this.windowSize) * 500;
        
        // Map Power (Y-Axis):
        let y = 150 - (point.value / this.maxGraphPower * 150);
        if (y < 0) y = 0; // Clamp to top
        
        return `${x},${y}`;
      }).join(' ');
    },
    
    graphPolygon() {
      if (this.powerHistory.length === 0) return "";
      
      // Close the shape: Start at bottom-left, draw line, end at bottom-right
      const points = this.graphPoints;
      
      // Get the last X position to draw straight down properly
      const lastPoint = points.split(' ').pop();
      const lastX = lastPoint ? lastPoint.split(',')[0] : 500;

      return `0,150 ${points} ${lastX},150`;
    }
  },
  mounted() {
    this.unsub = onSnapshot(doc(db, "devices", "outlet-01"), (doc) => {
      if (doc.exists()) {
        const data = doc.data();
        
        this.voltage = data.voltage;
        this.current = data.current;
        this.status = data.status || 'NORMAL';
        
        const newPower = parseFloat((this.voltage * this.current).toFixed(1));
        this.power = newPower;
        this.isOnline = true;

        // --- GRAPH LOGIC ---
        const now = Date.now();

        // Add new point with TIMESTAMP
        this.powerHistory.push({ 
          value: newPower, 
          timestamp: now 
        });

        // Cleanup: Remove points older than 1 hour + buffer to save memory
        const cutoff = now - (this.windowSize + 60000); 
        if (this.powerHistory.length > 0 && this.powerHistory[0].timestamp < cutoff) {
          this.powerHistory.shift();
        }

        // Auto-Scale Y-Axis
        if (newPower > this.maxGraphPower) {
          this.maxGraphPower = Math.ceil(newPower / 10) * 10;
        } 
        else if (this.maxGraphPower > 30 && newPower < this.maxGraphPower - 20) {
           const historyMax = Math.max(...this.powerHistory.map(p => p.value));
           this.maxGraphPower = Math.max(30, Math.ceil(historyMax / 10) * 10);
        }
      }
    });

    // OPTIONAL: Force graph to update every second (to make it scroll smoothly)
    // even if no new data arrives from Arduino
    setInterval(() => {
      // Triggering a re-render by updating a dummy property or just rely on Vue reactivity
      // In this simple version, we rely on incoming data updates (every 2s).
      // Since Arduino sends every 2s, the "scroll" effect will look fine.
    }, 2000);
  },
  beforeUnmount() {
    if (this.unsub) {
      this.unsub();
    }
  }
}
</script>

<style scoped>
/* Reuse Standard Styles */
.page-container { padding: 30px; background-color: #F5F7FA; min-height: 100vh; font-family: 'Segoe UI', sans-serif; }
.page-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 30px; }
.header-text h1 { margin: 0; color: #2D3436; font-size: 1.8rem; }
.status-indicators { display: flex; gap: 15px; }
.status-badge { padding: 8px 16px; border-radius: 20px; font-size: 0.85rem; font-weight: 600; display: flex; align-items: center; gap: 8px; }
.status-badge.online { background: #E3F2FD; color: #1565C0; }
.status-badge.offline { background: #ECEFF1; color: #78909C; }
.status-badge.safe { background: #E8F5E9; color: #1B5E20; }
.status-badge.danger { background: #FFEBEE; color: #C62828; }
.dot { width: 8px; height: 8px; background: #1565C0; border-radius: 50%; box-shadow: 0 0 0 2px rgba(21, 101, 192, 0.2); }

/* Metrics */
.metrics-grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 20px; margin-bottom: 30px; }
.metric-card { background: white; padding: 25px; border-radius: 12px; border: 1px solid #E0E0E0; box-shadow: 0 4px 6px rgba(0,0,0,0.02); }
.metric-header { display: flex; justify-content: space-between; margin-bottom: 10px; }
.metric-value h2 { font-size: 2.2rem; margin: 0; color: #2D3436; }
.metric-value .unit { font-size: 1rem; color: #b2bec3; }
.status-text.normal { color: #1B5E20; }
.status-text.warning { color: #F57C00; }

/* Dashboard Layout */
.dashboard-main { display: grid; grid-template-columns: 2fr 1fr; gap: 20px; }
.card { background: white; border-radius: 12px; border: 1px solid #E0E0E0; padding: 20px; }
.card-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px; }

/* GRAPH STYLES */
.chart-container { position: relative; width: 100%; height: 200px; margin-left: 30px; width: calc(100% - 30px); }
.line-chart { width: 100%; height: 100%; overflow: visible; }
.y-labels { position: absolute; left: -35px; top: 0; height: 100%; display: flex; flex-direction: column; justify-content: space-between; font-size: 0.75rem; color: #999; text-align: right; width: 30px; }
.x-labels { display: flex; justify-content: space-between; margin-top: 10px; margin-left: 30px; font-size: 0.75rem; color: #999; }

/* Controls */
.switch-list { margin-bottom: 25px; }
.switch-row { display: flex; justify-content: space-between; align-items: center; padding-bottom: 15px; border-bottom: 1px solid #F5F7FA; }
.switch-label { font-weight: 700; color: #2D3436; }
.toggle-switch { position: relative; display: inline-block; width: 44px; height: 24px; }
.toggle-switch input { opacity: 0; width: 0; height: 0; }
.slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; transition: .4s; border-radius: 24px; }
.slider:before { position: absolute; content: ""; height: 18px; width: 18px; left: 3px; bottom: 3px; background-color: white; transition: .4s; border-radius: 50%; }
input:checked + .slider { background-color: #1B5E20; }
input:checked + .slider:before { transform: translateX(20px); }

.emergency-btn { width: 100%; padding: 12px; background-color: #FFEBEE; color: #C62828; border: 1px solid #FFCDD2; border-radius: 8px; font-weight: 700; cursor: pointer; }
.emergency-btn:hover { background-color: #FFCDD2; }

/* Mobile */
@media (max-width: 900px) {
  .metrics-grid { grid-template-columns: 1fr; }
  .dashboard-main { grid-template-columns: 1fr; }
}
</style>