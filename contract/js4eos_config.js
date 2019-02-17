module.exports = {
    networks: {
        // jungle: {
        //     chainId: 'e70aaab8997e1dfce58fbfac80cbbb8fecec7b99cf982a9444273cbc64c41473',
        //     httpEndpoint: 'http://jungle2.cryptolions.io:8888',
        //     keyPrefix: 'EOS',
        // },
        // mainnet: {
        //     chainId: 'aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906',
        //     httpEndpoint: 'https://mainnet.meet.one',
        //     keyPrefix: 'EOS',
        // },
        // kylin: {
        //     chainId: '5fff1dae8dc8e2fc4d5b23b2c7665c97f9e9d8edf2b6485a86ba311c25639191',
        //     httpEndpoint: 'https://api-kylin.eoslaomao.com',
        //     keyPrefix: 'EOS',
        // },
        localhost: {
            chainId: 'cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f',
            httpEndpoint: 'http://localhost:8888',
            keyPrefix: 'EOS'
        }
    },
    deploy: {
        vote: {
            localhost: 'zjubcavote11',
        },
        'zjubca.token': {
            localhost: 'zjubcatokent',
        }
        // hello2:{
        //     jungle:'testtest1122', //network:account
        //     kylin:'cpuramcpuram', //network:account
        // },
    },
    keyProvider: [
        '5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3', //replace private key to your contract account
    ],
    defaultNetwork: 'localhost',
    eosio_cdt: true,
}