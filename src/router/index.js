import Login from '@/auth/Login.vue'
import Billings from '@/pages/Billings.vue'
import Dashboard from '@/pages/Dashboard.vue'
import Logs from '@/pages/Logs.vue'
import Settings from '@/pages/Settings.vue'
import { createRouter, createWebHistory } from 'vue-router'

const routes = [

  {
    path: '/',
    redirect: '/auth/login'
  },

  {
    path: '/auth/login', 
    name: "Login",
    component: Login
  },

  {
    path: '/pages/dashboard', 
    name: "Dashboard",
    component: Dashboard
  },

  {
    path: '/pages/billings',
    name: "Billings",
    component: Billings
  },

  {
    path: '/pages/logs', 
    name: "Logs",
    component: Logs
  },

  {
    path: '/pages/settings', 
    name: "Settings",
    component: Settings
  }
  
]

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: routes,
})

export default router