<template>
  <div>
    <md-tabs @md-changed="tabChange">
      <md-tab id="tab-proposing" md-label="提案中"></md-tab>
      <md-tab id="tab-voting" md-label="投票中"></md-tab>
      <md-tab id="tab-pass" md-label="已通过"></md-tab>
    </md-tabs>

    <div class="list">
      <md-progress-bar
          v-show="loading"
          md-mode="indeterminate"
          class="progress"
      ></md-progress-bar>
      <propose-item
          v-for="item in issues"
          :key="item.id"
          :id="item.id"
          :number="item.number"
          :title="item.title"
          :url="item.html_url"
      ></propose-item>
    </div>
  </div>
</template>

<script>
  import ProposeItem from '@/components/ProposeItem.vue'
  import axios from '@/utils/axios'

  export default {
    name: "ProposeList",
    data() {
      return {
        loading: false,
        issues: [],
      }
    },
    async created() {
      this.fetchIssues('propose');
    },
    methods: {
      tabChange(id) {
        if (id === 'tab-proposing') {
          this.fetchIssues('propose')
        } else if (id === 'tab-voting') {
          this.fetchIssues('voting')
        } else {
          this.fetchIssues('pass')
        }
      },
      resetIssues() {
        this.issues = []
      },
      async fetchIssues(label) {
        this.resetIssues();
        this.loading = true;
        let query = '';
        if (label === 'propose') {
          query = `labels=valid`
        } else if (label === 'voting') {
          query = `labels=voting`
        } else {
          query = `state=closed&labels=pass`
        }
        try {
          let res = await axios.get(`repos/Blockchain-zju/zjubca.proposals/issues?${query}`);
          this.issues = res.data;
        } catch (e) {
          this.alert(e.message)
        } finally {
          this.loading = false
        }
      },
    },
    components: {
      ProposeItem
    }
  }
</script>

<style scoped lang="scss">
  .list {
    position: relative;
  }

  .progress {
    width: 100%;
    position: absolute;
  }

</style>
