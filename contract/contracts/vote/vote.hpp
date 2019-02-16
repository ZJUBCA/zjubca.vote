#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>
using namespace std;
using namespace eosio;

class[[eosio::contract("zjubca.vote")]] Vote : public contract {
 public:
  using contract::contract;

  [[eosio::action]] void setVote(name voter, uint8_t attitude,
                                 uint64_t issueNum, asset deposit);

  [[eosio::action]] void withdraw(name voter, uint64_t issueNum);

  [[eosio::action]] void setPass(uint64_t issueNum);

 private:
  struct [[eosio::table]] vote {
    uint64_t number;   // issue number
    name voter;        // voter name
    uint8_t attitude;  // 0(pros) or 1(cons)
    uint64_t value;    // deposit value

    uint64_t primary_key() const { return number; }
    uint64_t get_voter() const { return voter.value; }
  };

  struct [[eosio::table]] issue {
    uint64_t number;      // issue number
    uint64_t totalValue;  // total pros deposit value
    bool isPassed;        // if issue has been passed,set true

    uint64_t primary_key() const { return number; }
  };

  // @abi table accounts i64
  struct [[eosio::table]] account {
    asset balance;

    uint64_t primary_key() const { return balance.symbol.code().raw(); }
  };

  static asset get_balance(name token_contract_account, name owner,
                           symbol_code sym_code) {
    accounts accountstable(token_contract_account, owner.value);
    const auto& ac = accountstable.get(sym_code.raw());
    return ac.balance;
  }

  typedef multi_index<"accounts"_n, account> accounts;

  typedef multi_index<
      "votes"_n, vote,
      indexed_by<"voter"_n, const_mem_fun<vote, uint64_t, &vote::get_voter>>>
      votes;

  typedef multi_index<"issues"_n, issue> issues;
};