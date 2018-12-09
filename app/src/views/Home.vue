<template>
  <div class="home">
    <!--<img alt="Vue logo" src="../assets/logo.png">-->
    <!--<HelloWorld msg="Welcome to Your Vue.js App"/>-->
    <div class="container">
      <div class="main">
        <propose-list>

        </propose-list>
      </div>
      <div class="right">
        <button-group></button-group>
      </div>
    </div>
  </div>
</template>

<style scoped lang="scss">
  .container {
    margin: 0 auto;
    width: 800px;
    display: flex;
    flex-direction: row;
  }

  .main {
    flex: 2.5;
  }

  .right {
    flex: 1;
  }
</style>

<script>
  // @ is an alias to /src
  import ProposeList from '@/components/ProposeList.vue'
  import ButtonGroup from '@/components/ButtonGroup.vue'

  export default {
    name: 'home',
    components: {
      ProposeList,
      ButtonGroup
    }
  }
</script>
