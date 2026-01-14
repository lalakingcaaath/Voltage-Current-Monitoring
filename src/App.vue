<template>
  <div class="app-layout">
    <Sidebar v-if="!route.meta.hideSidebar" />

    <main :class="['main-content', { 'full-width': route.meta.hideSidebar }]">
      <router-view />
    </main>
  </div>
</template>

<script setup>
import Sidebar from '@/components/Sidebar.vue'
import { useRoute } from 'vue-router'
const route = useRoute()
</script>

<style>
/* Reset default browser spacing */
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

.app-layout {
  display: flex; /* This puts Sidebar and Main next to each other */
  min-height: 100vh;
  width: 100%;
}

.main-content {
  flex: 1; /* Takes up all remaining space */
  background-color: #f1f5f9;
}

/* If login page, remove the space for the sidebar */
.main-content.full-width {
  width: 100%;
}
</style>