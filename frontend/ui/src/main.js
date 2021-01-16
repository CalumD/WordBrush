import {createApp} from 'vue'
import App from './App.vue'
import useClickAway from "@/components/util/useClickAway";

export const BASE_URL = 'ubuntu.vms.local:3000';

createApp(App)
    .directive('click-away', useClickAway)
    .mount('#app');
