// Import the functions you need
import { initializeApp } from "firebase/app";
// 1. ADD THIS LINE: We need Firestore for the dashboard
import { getFirestore } from "firebase/firestore";

const firebaseConfig = {
  apiKey: "AIzaSyArInqYx95PbbD2WXwJK3BP2-zbr5MvlTU",
  authDomain: "voltagecurrent-monitoring.firebaseapp.com",
  projectId: "voltagecurrent-monitoring",
  storageBucket: "voltagecurrent-monitoring.firebasestorage.app",
  messagingSenderId: "545362286464",
  appId: "1:545362286464:web:6208024610d886b35a5365",
  measurementId: "G-K63RF06SBG"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

// 2. INITIALIZE AND EXPORT THE DATABASE
const db = getFirestore(app);

// 3. Export 'db' so Dashboard.vue can use it
export { db };