<template>
  <div class="container">
    <md-progress-bar class="loading" md-mode="indeterminate" v-if="loading"></md-progress-bar>
    <div class="content">
      <md-card class="card">
        <div class="actionBar">
          <div class="tag">
            <div class="tagLabel" :class="tagBgColor">{{issue.state}}</div>
          </div>
          <div class="vote">
            <md-button class="md-icon-button" @click="showVoteDialog(0)">
              <md-icon :class="vote.attitude === 0 && 'blue'">thumb_up</md-icon>
            </md-button>
            <md-button class="md-icon-button" @click="showVoteDialog(1)">
              <md-icon :class="vote.attitude === 1 && 'red'">thumb_down</md-icon>
            </md-button>
          </div>
        </div>
        <div class="item">
          <span>提案编号</span>
          <span>{{issue.number}}</span>
        </div>
        <div class="item">
          <span>提案者</span>
          <span>{{issue.author}}</span>
        </div>
        <div class="item">
          <span>创建时间</span>
          <span>{{issue.createdAt}}</span>
        </div>
        <div class="item">
          <span>最后更新</span>
          <span>{{issue.createdAt}}</span>
        </div>
        <div class="voteStatus">
          <div class="voteWord">
            <div>支持 {{proRate}}%</div>
            <div>反对 {{conRate}}%</div>
          </div>
          <md-progress-bar style="width:100%" md-mode="determinate" :md-value="proRate"></md-progress-bar>
        </div>
      </md-card>
      <h3 class="title">{{issue.title}}</h3>
      <md-divider></md-divider>
      <vue-markdown :source="content"></vue-markdown>
    </div>
    <md-dialog :md-active.sync="showDialog">
      <md-dialog-title>为#{{voteProposal.number}}提案投票</md-dialog-title>
      <md-dialog-content>
        <div class="title">{{issue.title}}</div>
        <div class="attitude">
          <div>您将投出&nbsp;</div>
          <div class="tag">
            <div class="tagLabel" :class="voteProposal.attitude === 0?'blue':'red'">
              {{voteProposal.attitude === 0?'赞成票':'反对票'}}
            </div>
          </div>
        </div>
        <md-field>
          <label> ZJUBCA 数量（保留4位小数）</label>
          <md-input v-model="voteProposal.value"></md-input>
          <span class="md-helper-text">1 ZJUBCA 代表 1 票。投票并不消耗真实的Token。</span>
        </md-field>
      </md-dialog-content>

      <md-dialog-actions>
        <md-button @click="showDialog = false">关闭</md-button>
        <md-button class="md-primary" @click="submitVote">
          <md-progress-spinner :md-diameter="30" :md-stroke="3" md-mode="indeterminate"
                               v-if="txLoading"></md-progress-spinner>
          <span v-else>提交</span>
        </md-button>
      </md-dialog-actions>
    </md-dialog>
    <md-snackbar md-position="center" :md-duration="4000" :md-active.sync="showSnackbar">
      {{errMsg}}
    </md-snackbar>
  </div>
</template>

<script>
  import axios from '../utils/axios';
  import moment from 'moment';
  import VueMarkdown from 'vue-markdown';
  import EosService, {CONTRACT} from '../services/eos';
  import event from '../utils/event';

  export default {
    name: "Issue",
    data() {
      return {
        tagBgColor: '',
        issue: {
          number: '',
          title: '',
          author: '',
          createAt: '',
          updatedAt: '',
          state: '',
        },
        issueStatus: {
          conValue: 1,
          isClosed: 0,
          isPassed: 0,
          proValue: 1,
        },
        issueChainFetching: false,

        content: '',
        loading: false,
        showSnackbar: false,
        errMsg: '',
        vote: {
          number: '',
          attitude: -1,
          value: 0
        },
        voteProposal: {
          number: '',
          attitude: -1,
          value: 0
        },
        showDialog: false,
        txLoading: false    // loading when transaction submit
      }
    },
    created() {
      this.fetchIssue(this.$route.params.id);
      this.fetchVote(this.$route.params.id);
      this.fetchIssueFromChain(this.$route.params.id);
    },
    computed: {
      proRate() {
        let {proValue, conValue} = this.issueStatus;
        if (proValue === 0 && conValue === 0) {
          return 50;
        }
        return +(proValue / (proValue + conValue) * 100).toFixed(2)
      },
      conRate() {
        return +(100 - this.proRate).toFixed(2);
      }
    },
    methods: {
      async fetchIssue(id) {
        try {
          this.loading = true;
          const res = await axios.get(`repos/Blockchain-zju/zjubca.proposals/issues/${id}`)
          const issue = res.data;
          this.content = issue.body;
          this.issue = {
            number: issue.number,
            title: issue.title,
            author: issue.user.login,
            createdAt: moment(issue.created_at).format('YYYY-MM-DD HH:mm:ss'),
            updatedAt: moment(issue.updated_at).format('YYYY-MM-DD HH:mm:ss')
          };
          // console.log(issue);
          this.getState(issue.labels);
        } catch (e) {
          console.log(e);
        } finally {
          this.loading = false;
        }
      },
      async fetchIssueFromChain(id) {
        try {
          this.issueChainFetching = true;
          const issue = await EosService.getIssue(id);
          console.log(issue)
          if (issue) {
            this.issueStatus = issue;
          }
        } catch (e) {
          if (e.message === 'nologin') {
            event.$on('login', () => {
              this.fetchIssueFromChain(id);
            })
          }
        } finally {
          this.issueChainFetching = false;
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
          this.issue.state = 'pass';
          this.tagBgColor = 'green';
          return
        }
        label = labels.find(x => x.name === 'voting');
        if (label) {
          this.issue.state = 'voting';
          this.tagBgColor = 'red';
          return
        }
        this.issue.state = 'valid';
        this.tagBgColor = 'gray'
      },
      async fetchVote(id) {
        try {
          const vote = await EosService.getVote(id);
          console.log(vote)
          if (vote) {
            this.vote = vote;
          }
        } catch (e) {
          console.log(e)
          if (e.message === 'nologin') {
            event.$on('login', () => {
              this.fetchVote(id);
            })
          }
        }
      },
      showVoteDialog(attitude) {
        if (this.issueChainFetching || this.issueStatus.isClosed) {
          return;
        }
        this.showDialog = true;
        this.voteProposal = {
          number: this.vote.number,
          attitude: attitude,
          value: (this.vote.value / 10000).toFixed(4)
        }
      },
      async submitVote() {
        if (!this.voteProposal.value) {
          this.alert('请填写你要投出的Token数量')
        }
        try {
          const res = await EosService.transaction(
            {
              actions: [
                {
                  account: CONTRACT,
                  name: 'setvote',
                  authorization: [{
                    actor: EosService.name,
                    permission: EosService.account.authority
                  }],
                  data: {
                    voter: EosService.name,
                    attitude: +this.voteProposal.attitude,
                    issueNum: +this.issue.number,
                    deposit: (+this.voteProposal.value).toFixed(4) + ' ZJUBCA'
                  }
                }
              ]
            }
          );
          // console.log(res);
          let result = null;
          this.txLoading = true;
          let intv = setInterval(async () => {
            try {
              result = await EosService.getTransaction(res.transaction_id)
              if (result.block_num > 0) {
                clearInterval(intv);
                this.txLoading = false;
                this.showDialog = false;
                this.alert('投票成功');

                this.fetchIssueFromChain(this.issue.number);
                this.fetchVote(this.issue.number);
              }
            } catch (e) {
              console.log(e);
            }
          }, 1000);
        } catch (e) {
          // console.log(e);
          if (e.message === 'nologin') {
            this.alert("请先登录")
          } else if (e.code !== 402) {
            this.alert(e.error.what);
          }
        }
      },
      alert(msg) {
        this.showSnackbar = true;
        this.errMsg = msg;
      }
    },
    components: {
      VueMarkdown
    }
  }
</script>

<style scoped lang="scss">
  @media screen and (min-width: 600px) {
    .container {
      width: 100%;
    }
  }

  .container {
    max-width: 680px;
    margin: 0 auto;
  }

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

    .voteStatus {
      margin-top: 10px;
      font-size: 12px;

      .voteWord {
        margin-bottom: 2px;
        display: flex;
        flex-direction: row;
        justify-content: space-between;
      }

      .md-progress-bar.md-theme-default.md-determinate {
        background-color: #ec5f59;
      }
    }

  }

  .actionBar {
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    align-items: center;

    .vote {
      .blue {
        color: #538bf7;
      }

      .red {
        color: #ec5f59;
      }
    }
  }

  .md-dialog {
    width: 425px;

    .title {
      font-size: 16px;
      margin-bottom: 20px;
    }
  }

  .attitude {
    margin: 10px 0 20px 0;
    display: flex;
    flex-direction: row;
    align-items: center;
  }

  .tag {
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

    .blue {
      color: white;
      background-color: #538bf7;
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
</style>
