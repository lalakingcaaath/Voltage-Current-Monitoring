// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getAnalytics } from "firebase/analytics";

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
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
const analytics = getAnalytics(app);