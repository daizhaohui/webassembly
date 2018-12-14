import App from './components/app.js';
import {
  store,
  initializeStore
} from './store/store.js';

// This allows us to use the <vue-numeric> component globally:
Vue.use(VueNumeric.default);


window.$store = store;


window.AMOUNT_TYPE = {
  raw: 1,
  cooked: 2
};

initializeStore()
  .then(() => {
    new Vue({
      render: h => h(App),
      el: '#app'
    });
  })
  .catch(err => {
    console.error(err);
  });