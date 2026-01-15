<template>
  <div class="page-container">
    
    <header class="page-header">
      <div class="header-text">
        <h1>System Dashboard</h1>
        <p>Live monitoring: 3-Gang Extension Outlet</p>
      </div>
      <div class="status-indicators">
        <div class="status-badge online">
          <span class="dot"></span> Online
        </div>
        <div class="status-badge safe">
          <span class="shield">🛡</span> Protection Active
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
          <h2>224.5 <span class="unit">V</span></h2>
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
          <h2>3.20 <span class="unit">A</span></h2>
        </div>
        <div class="metric-footer">
          <span class="status-text warning">High Load</span>
        </div>
      </div>

      <div class="card metric-card">
        <div class="metric-header">
          <span class="label">Real-Time Power</span>
          <span class="icon">💡</span>
        </div>
        <div class="metric-value">
          <h2>718.4 <span class="unit">W</span></h2>
        </div>
        <div class="metric-footer">
          <span class="subtext">Today: 1.2 kWh</span>
        </div>
      </div>
    </section>

    <section class="dashboard-main">
      
      <div class="card graph-card">
        <div class="card-header">
          <h3>Live Power Consumption (Watts)</h3>
          <div class="live-indicator">
            <span class="pulse"></span> Updating
          </div>
        </div>
        <div class="chart-container">
          <svg viewBox="0 0 500 150" class="line-chart">
            <line x1="0" y1="112.5" x2="500" y2="112.5" stroke="#eee" />
            <line x1="0" y1="75" x2="500" y2="75" stroke="#eee" />
            <line x1="0" y1="37.5" x2="500" y2="37.5" stroke="#eee" />
            
            <polyline 
              fill="none" 
              stroke="#1B5E20" 
              stroke-width="3" 
              points="0,120 50,115 100,100 150,110 200,60 250,70 300,50 350,55 400,90 450,85 500,80" 
            />
            <polygon 
              fill="rgba(27, 94, 32, 0.1)" 
              points="0,150 0,120 50,115 100,100 150,110 200,60 250,70 300,50 350,55 400,90 450,85 500,80 500,150" 
            />
          </svg>
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
              <span class="load-name">PC Monitor</span>
            </div>
            <label class="toggle-switch">
              <input type="checkbox" checked>
              <span class="slider round"></span>
            </label>
          </div>

          <div class="switch-row">
            <div class="switch-info">
              <span class="switch-label">Outlet 2</span>
              <span class="load-name">System Unit</span>
            </div>
            <label class="toggle-switch">
              <input type="checkbox" checked>
              <span class="slider round"></span>
            </label>
          </div>

          <div class="switch-row">
            <div class="switch-info">
              <span class="switch-label">Outlet 3</span>
              <span class="load-name">Fan / Aux</span>
            </div>
            <label class="toggle-switch">
              <input type="checkbox">
              <span class="slider round"></span>
            </label>
          </div>
        </div>
        
        <button class="emergency-btn">
          ⚠️ EMERGENCY OFF (ALL)
        </button>
      </div>

    </section>
  </div>
</template>

<script>
export default {
  name: 'DashboardPage'
}
</script>

<style scoped>
/* --- Layout --- */
.page-container {
  padding: 30px;
  background-color: #F5F7FA;
  min-height: 100vh;
  font-family: 'Segoe UI', sans-serif;
}

/* --- Header --- */
.page-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 30px;
}

.header-text h1 { margin: 0; color: #2D3436; font-size: 1.8rem; }
.header-text p { margin: 5px 0 0; color: #636e72; }

.status-indicators { display: flex; gap: 15px; }

.status-badge {
  padding: 8px 16px;
  border-radius: 20px;
  font-size: 0.85rem;
  font-weight: 600;
  display: flex;
  align-items: center;
  gap: 8px;
}

.status-badge.online { background: #E3F2FD; color: #1565C0; }
.status-badge.safe { background: #E8F5E9; color: #1B5E20; }

.dot {
  width: 8px; height: 8px; background: #1565C0; border-radius: 50%;
  box-shadow: 0 0 0 2px rgba(21, 101, 192, 0.2);
}

/* --- Metrics Grid --- */
.metrics-grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 20px;
  margin-bottom: 30px;
}

.metric-card {
  background: white;
  padding: 25px;
  border-radius: 12px;
  border: 1px solid #E0E0E0;
  box-shadow: 0 4px 6px rgba(0,0,0,0.02);
}

.metric-header { display: flex; justify-content: space-between; margin-bottom: 10px; }
.metric-header .label { color: #636e72; font-weight: 600; font-size: 0.9rem; }
.metric-header .icon { font-size: 1.2rem; }

.metric-value h2 { font-size: 2.2rem; margin: 0; color: #2D3436; }
.metric-value .unit { font-size: 1rem; color: #b2bec3; font-weight: 500; }

.metric-footer { margin-top: 10px; font-size: 0.85rem; }
.status-text.normal { color: #1B5E20; }
.status-text.warning { color: #F57C00; }

/* --- Dashboard Main (Graph + Controls) --- */
.dashboard-main {
  display: grid;
  grid-template-columns: 2fr 1fr;
  gap: 20px;
}

.card {
  background: white;
  border-radius: 12px;
  border: 1px solid #E0E0E0;
  padding: 20px;
}

.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
}
.card-header h3 { margin: 0; font-size: 1.1rem; color: #2D3436; }

/* Graph Area */
.live-indicator { font-size: 0.75rem; color: #1B5E20; display: flex; align-items: center; gap: 5px; }
.pulse {
  width: 6px; height: 6px; background: #1B5E20; border-radius: 50%;
  animation: pulse 1.5s infinite;
}

.chart-container { width: 100%; height: 200px; }
.line-chart { width: 100%; height: 100%; }

/* Outlet Controls */
.switch-list { display: flex; flex-direction: column; gap: 20px; margin-bottom: 25px; }

.switch-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding-bottom: 15px;
  border-bottom: 1px solid #F5F7FA;
}

.switch-info { display: flex; flex-direction: column; }
.switch-label { font-weight: 700; color: #2D3436; font-size: 0.95rem; }
.load-name { font-size: 0.8rem; color: #636e72; }

/* Toggle Switch CSS */
.toggle-switch { position: relative; display: inline-block; width: 44px; height: 24px; }
.toggle-switch input { opacity: 0; width: 0; height: 0; }
.slider {
  position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0;
  background-color: #ccc; transition: .4s; border-radius: 24px;
}
.slider:before {
  position: absolute; content: ""; height: 18px; width: 18px; left: 3px; bottom: 3px;
  background-color: white; transition: .4s; border-radius: 50%;
}
input:checked + .slider { background-color: #1B5E20; }
input:checked + .slider:before { transform: translateX(20px); }

/* Emergency Button */
.emergency-btn {
  width: 100%;
  padding: 12px;
  background-color: #FFEBEE;
  color: #C62828;
  border: 1px solid #FFCDD2;
  border-radius: 8px;
  font-weight: 700;
  cursor: pointer;
  transition: background 0.2s;
}
.emergency-btn:hover { background-color: #FFCDD2; }

@keyframes pulse {
  0% { opacity: 1; }
  50% { opacity: 0.4; }
  100% { opacity: 1; }
}

/* Mobile Responsive */
@media (max-width: 900px) {
  .metrics-grid { grid-template-columns: 1fr; }
  .dashboard-main { grid-template-columns: 1fr; }
}
</style>