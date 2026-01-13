<template>
  <section class="login-container">
    <div class="login-card">
      <div class="brand-header">
        <div class="icon-wrapper">⚡</div>
        <h1>Voltage & Current Monitor</h1>
        <p class="subtitle">IoT Safety & Energy Management</p>
      </div>

      <form @submit.prevent="handleLogin">
        <div class="form-group">
          <label for="username">Username</label>
          <input 
            type="text" 
            id="username" 
            v-model="username" 
            placeholder="Enter your admin ID"
            required 
          />
        </div>

        <div class="form-group">
          <label for="password">Password</label>
          <input 
            type="password" 
            id="password" 
            v-model="password" 
            placeholder="••••••••"
            required 
          />
        </div>

        <button type="submit" class="login-btn">
          Sign In
        </button>
      </form>
      
      <div class="footer-note">
        <small>Group 4 • Final Project</small>
      </div>
    </div>
  </section>
</template>

<script>
export default {
  name: 'LoginPage',
  data() {
    return {
      username: '',
      password: ''
    }
  },
  methods: {
    handleLogin() {
      // STATIC LOGIN: No backend check for now.
      // Simply redirect to the dashboard route.
      // Ensure you have a route named 'dashboard' or path '/dashboard' in your router/index.js
      this.$router.push('/pages/dashboard'); 
    }
  }
}
</script>

<style scoped>
/* 1. Main Container: Holds the background image and centers the card */
.login-container {
  display: flex;
  justify-content: center;
  align-items: center;
  min-height: 100vh;
  width: 100%;
  
  /* Background Setup - Uses the image you uploaded */
  background-image: url('@/assets/pexels-mikhail-nilov-6592549.jpg');
  background-size: cover;
  background-position: center;
  background-repeat: no-repeat;
  
  /* Fallback color */
  background-color: #f0f2f5; 
}

/* 2. Glassmorphism Card */
.login-card {
  background: rgba(255, 255, 255, 0.85); /* High opacity white for readability */
  backdrop-filter: blur(12px); /* The "Frosted Glass" effect */
  border: 1px solid rgba(255, 255, 255, 0.5);
  box-shadow: 0 8px 32px 0 rgba(31, 38, 135, 0.15);
  
  border-radius: 16px;
  padding: 40px;
  width: 100%;
  max-width: 400px;
  text-align: center;
}

/* 3. Typography & Branding */
.brand-header {
  margin-bottom: 30px;
}

.icon-wrapper {
  font-size: 3rem;
  margin-bottom: 10px;
}

h1 {
  font-size: 1.5rem;
  color: #2D3436; /* Charcoal */
  margin: 0 0 5px 0;
  font-weight: 700;
}

.subtitle {
  color: #636e72;
  font-size: 0.9rem;
  margin: 0;
}

/* 4. Form Styling */
.form-group {
  margin-bottom: 20px;
  text-align: left;
}

label {
  display: block;
  font-size: 0.85rem;
  color: #2D3436;
  margin-bottom: 8px;
  font-weight: 600;
}

input {
  width: 100%;
  padding: 12px;
  border-radius: 8px;
  border: 1px solid #E0E0E0; /* Soft Gray */
  background-color: #F8F9FA; /* Off-White */
  font-size: 1rem;
  transition: all 0.3s ease;
  box-sizing: border-box;
}

input:focus {
  outline: none;
  border-color: #1B5E20; /* Forest Green */
  background-color: #FFFFFF;
  box-shadow: 0 0 0 3px rgba(27, 94, 32, 0.1);
}

/* 5. Button Styling */
.login-btn {
  width: 100%;
  padding: 12px;
  border: none;
  border-radius: 8px;
  background-color: #1B5E20; /* Forest Green */
  color: white;
  font-size: 1rem;
  font-weight: 600;
  cursor: pointer;
  transition: background-color 0.3s ease;
  margin-top: 10px;
}

.login-btn:hover {
  background-color: #144a18; /* Darker green on hover */
}

.footer-note {
  margin-top: 30px;
  color: #b2bec3;
  font-size: 0.75rem;
}
</style>