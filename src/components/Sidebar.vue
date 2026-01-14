<template>
  <aside class="sidebar-container">
    <div class="brand">
      <h2>Voltage and Current Monitoring</h2>
    </div>
    
    <nav class="nav-links">
      <router-link to="/pages/dashboard" class="link">Dashboard</router-link>
      <router-link to="/pages/billings" class="link">Billings</router-link>
      <router-link to="/pages/logs" class="link">Logs</router-link>
      <router-link to="/pages/settings" class="link">Settings</router-link>
    </nav>

    <router-link to="/auth/login" class="logout-link">Logout</router-link>
  </aside>
</template>

<style scoped>
.sidebar-container {
  width: 260px;
  background-color: #0f172a; /* Dark Navy */
  color: white;
  display: flex;
  flex-direction: column;
  height: 100vh;
  position: sticky; /* Keeps it visible as you scroll the content */
  top: 0;
  padding: 1.5rem;
}

.brand {
  margin-bottom: 2rem;
  color: #38bdf8;
  letter-spacing: 1px;
}

.nav-links {
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
  flex-grow: 1;
}

.link {
  color: #94a3b8;
  text-decoration: none;
  padding: 0.75rem 1rem;
  border-radius: 6px;
  transition: 0.2s;
}

.link:hover, .router-link-active {
  background-color: #1e293b;
  color: #38bdf8;
}

.logout-link {
  color: #f87171;
  text-decoration: none;
  padding: 0.75rem 1rem;
  border-top: 1px solid #1e293b;
}
</style>