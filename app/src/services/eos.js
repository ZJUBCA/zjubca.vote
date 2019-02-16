import ScatterJS from 'scatterjs-core'
import ScatterEOS from 'scatterjs-plugin-eosjs'
import Eos from 'eosjs';

ScatterJS.plugins(new ScatterEOS());

export const network = {
  blockchain: 'eos',
  protocol: 'https',
  host: 'api-kylin.eoslaomao.com',
  port: 443,
  chainId: '5fff1dae8dc8e2fc4d5b23b2c7665c97f9e9d8edf2b6485a86ba311c25639191'
};


export default class EosService {
  constructor() {

  }

  static async init() {
    return new Promise((resolve, reject) => {
      ScatterJS.scatter.connect('ZJUBCA.VOTING', {
        initTimeout: 10000,
      }).then(async connected => {
        if (!connected) {
          console.log('please unlock your scatter');
          reject(new Error('please unlock your scatter'))
        }
        const network = {
          blockchain: 'eos',
          protocol: 'https',
          host: 'api-kylin.eoslaomao.com',
          port: 443,
          chainId: '5fff1dae8dc8e2fc4d5b23b2c7665c97f9e9d8edf2b6485a86ba311c25639191'
        };

        let scatter = ScatterJS.scatter;
        await scatter.getIdentity({accounts: [network]});
        const account = scatter.identity.accounts.find(x => x.blockchain === 'eos');
        const eos = scatter.eos(network, Eos, {expireInSeconds: 20});

        console.log(account)
        console.log(eos)

        EosService.scatter = scatter;
        EosService.account = account;
        EosService.eos = eos;


        resolve()
        // Transaction Example
        // const transactionOptions = { authorization:[`${account.name}@${account.authority}`] };
        //
        // eos.transfer(account.name, 'helloworld', '1.0000 EOS', 'memo', transactionOptions).then(trx => {
        //   // That's it!
        //   console.log(`Transaction ID: ${trx.transaction_id}`);
        // }).catch(error => {
        //   console.error(error);
        // });
      });
    })
  }

  /**
   * returns the account name.
   *
   * @returns {*}
   */
  static get name() {
    return EosService.account.name;
  }
}
