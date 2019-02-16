import Vue from 'vue'
import App from './App.vue'
import router from './router'
import {
  MdCard,
  MdButton,
  MdTabs,
  MdIcon,
  MdProgress,
  MdSnackbar,
  MdDialog,
  MdField,
  MdRadio,
  MdToolbar,
  MdList,
} from 'vue-material/dist/components'
import 'vue-material/dist/vue-material.min.css'


Vue.config.productionTip = false;
Vue.use(MdCard);
Vue.use(MdButton);
Vue.use(MdTabs);
Vue.use(MdIcon);
Vue.use(MdProgress);
Vue.use(MdSnackbar);
Vue.use(MdDialog);
Vue.use(MdField);
Vue.use(MdRadio);
Vue.use(MdToolbar);
Vue.use(MdList);


new Vue({
  router,
  render: h => h(App)
}).$mount('#app')
