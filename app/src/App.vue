<template>
  <div id="app">
    <md-toolbar class="md-primary" id="nav">
      <router-link to="/home">
        <md-icon>home</md-icon>
      </router-link>
      <router-link to="/about">
        <md-icon>help_outline</md-icon>
      </router-link>
      <span class="name">{{accountName}}</span>
    </md-toolbar>
    <router-view/>
  </div>
</template>

<script>
  import EosService from './services/eos'

  export default {
    data() {
      return {
        accountName: ''
      }
    },
    async created() {
      await EosService.init();
      this.accountName = EosService.name;
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
    float: right;
  }
</style>
