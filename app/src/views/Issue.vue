<template>
  <div>
    <md-progress-bar class="loading" md-mode="indeterminate" v-if="loading"></md-progress-bar>
    <div class="content">
      <md-card class="card">
        <div class="tag">
          <div class="tagLabel" :class="tagBgColor">{{meta.state}}</div>
        </div>
        <div class="item">
          <span>提案编号</span>
          <span>{{meta.id}}</span>
        </div>
        <div class="item">
          <span>提案者</span>
          <span>{{meta.author}}</span>
        </div>
        <div class="item">
          <span>创建时间</span>
          <span>{{meta.createdAt}}</span>
        </div>
        <div class="item">
          <span>最后更新</span>
          <span>{{meta.createdAt}}</span>
        </div>
      </md-card>
      <vue-markdown :source="content"></vue-markdown>
    </div>
  </div>
</template>

<script>
  import axios from '../utils/axios';
  import moment from 'moment';
  import VueMarkdown from 'vue-markdown';

  export default {
    name: "Issue",
    data() {
      return {
        tagBgColor: '',
        meta: {
          id: '',
          author: '',
          createAt: '',
          updatedAt: '',
          state: ''
        },
        content: '',
        loading: false
      }
    },
    created() {
      this.fetchIssue(this.$route.params.id)
    },

    methods: {
      async fetchIssue(id) {
        try {
          this.loading = true;
          const res = await axios.get(`repos/Blockchain-zju/zjubca.proposals/issues/${id}`)
          const issue = res.data
          this.content = issue.body;
          this.meta = {
            id: issue.id,
            author: issue.user.login,
            createdAt: moment(issue.created_at).format('YYYY-MM-DD HH:mm:ss'),
            updatedAt: moment(issue.updated_at).format('YYYY-MM-DD HH:mm:ss')
          }
          console.log(issue)
          this.getState(issue.labels);
        } catch (e) {
          console.log(e);
        } finally {
          this.loading = false;
        }
      },
      /**
       * getState get the issue state through label name.
       *
       * @param labels label[]
       */
      getState(labels) {
        let label = labels.find(x => x.name === 'pass');
        if (label) {
          this.meta.state = 'pass';
          this.tagBgColor = 'green';
          return
        }
        label = labels.find(x => x.name === 'voting');
        if (label) {
          this.meta.state = 'voting';
          this.tagBgColor = 'red';
          return
        }
        this.meta.state = 'valid';
        this.tagBgColor = 'gray'
      }
    },
    components: {
      VueMarkdown
    }
  }
</script>

<style scoped lang="scss">
  .content {
    padding: 15px;
  }

  .loading {
    position: absolute;
    width: 100%;
  }

  .card {
    padding: 11px 16px;
    margin-bottom: 30px;

    .item {
      margin: 5px 0;
      display: flex;
      flex-direction: row;
      justify-content: space-between;

      span:first-child {
        color: #5e5e5e;
      }
    }

    .tag {
      position: relative;
      height: 26px;

      .tagLabel {
        font-weight: bold;
        position: absolute;
        padding: 2px 8px;
        border-radius: 3px;
      }

      .green {
        color: white;
        background-color: #67ac5b;
      }

      .red {
        color: white;
        background-color: #ec5f59;
      }

      .gray {
        color: white;
        background-color: #666666;
      }
    }
  }
</style>
