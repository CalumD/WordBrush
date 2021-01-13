import {createApp} from 'vue'
import App from './App.vue'
import useClickAway from "@/components/util/useClickAway";

createApp(App)
    .directive('click-away', useClickAway)
    .mount('#app');
