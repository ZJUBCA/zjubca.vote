import Vue from 'vue'
import Router from 'vue-router'
import Home from './views/Home.vue'
import Issue from "./views/Issue";
import About from "./views/About"

Vue.use(Router);

export default new Router({
  routes: [
    {
      path: '/',
      redirect: '/home'
    },
    {
      path: '/home',
      name: 'home',
      component: Home
    },
    {
      path: '/issue/:id',
      name: 'issue',
      component: Issue
    },
    {
      path: '/about',
      name: 'about',
      // route level code-splitting
      // this generates a separate chunk (about.[hash].js) for this route
      // which is lazy-loaded when the route is visited.
      component: About
    }
  ]
})
