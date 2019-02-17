# zjubca.vote
Proposals voting application. 针对[zjubca.proposals](https://github.com/Blockchain-zju/zjubca.proposals)的提案投票工具。

## Directory Structure

- `app` - voting application, based on Vue.js.
- `contract` - voting smart contracts.

## How to test
0. `cd contract`.
1. Compile and deploy `zjubca.token` contract with account `zjubcatokent`.
2. Create and issue `ZJUBCA` to account `eosio`.
3. Compile and deploy `vote` contract with account `zjubcavote11`.
4. Run `js4eos dapp test -g vote`.
