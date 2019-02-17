<template>
  <div id="app">
    <md-toolbar class="md-primary" id="nav">
      <router-link to="/home">
        <md-icon>home</md-icon>
      </router-link>
      <router-link to="/about">
        <md-icon>help_outline</md-icon>
      </router-link>
      <div class="name" @click="login">{{accountName}}</div>
    </md-toolbar>
    <router-view/>
  </div>
</template>

<script>
  import EosService from './services/eos'

  export default {
    data() {
      return {
        accountName: '',
        isLogin: false,
      }
    },
    async created() {
      await this.login();
    },
    methods: {
      async login() {
        try {
          if (!this.isLogin) {
            await EosService.connect();
            this.accountName = EosService.name || '未登录';
            this.isLogin = true;
          }
        } catch (e) {
          console.log(e)
        }
      }
    }
  }
</script>

<style lang="scss">
  #app {
    font-family: 'Avenir', Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
  }

  #nav {
    a {
      color: #f5f5f5;
      margin: 0 5px;

      &:hover {
        text-decoration: none;
      }

      &.router-link-exact-active {
        color: #f5f5f5;
      }
    }
  }

  .name {
    flex: 1;
    text-align: right;
    padding-right: 10px;
  }
</style>
