#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <string>
using namespace std;
using namespace eosio;

namespace eosiosystem {
class system_contract;
}

class[[eosio::contract("vote")]] Vote : public contract {
 public:
  using contract::contract;

  [[eosio::action]] void setvote(name voter, uint8_t attitude,
                                 uint64_t issueNum, asset deposit);

  [[eosio::action]] void withdraw(name voter, uint64_t issueNum);

  [[eosio::action]] void close(uint64_t issueNum, uint8_t passed);

  [[eosio::action]] void reopen(uint64_t issueNum);

  static asset get_balance(name token_contract_account, name owner,
                           symbol_code sym_code) {
    accounts accountstable(token_contract_account, owner.value);
    const auto& ac = accountstable.get(sym_code.raw());
    return ac.balance;
  }

 private:
  struct [[eosio::table]] vote {
    uint64_t number;   // issue number
    name voter;        // voter name
    uint8_t attitude;  // 0(pros) or 1(cons)
    uint64_t value;    // deposit value

    uint64_t primary_key() const { return voter.value; }
    uint64_t get_number() const { return number; }
  };

  struct [[eosio::table]] issue {
    uint64_t number;    // issue number
    uint64_t proValue;  // total pros deposit value
    uint64_t conValue;  // total con deposit value
    bool isPassed;      // if issue has been passed,set true
    bool isClosed;      // issue has been closed

    uint64_t primary_key() const { return number; }
  };

  struct [[eosio::table]] account {
    asset balance;

    uint64_t primary_key() const { return balance.symbol.code().raw(); }
  };

  typedef multi_index<"accounts"_n, account> accounts;

  typedef multi_index<
      "votes"_n, vote,
      indexed_by<"bynum"_n, const_mem_fun<vote, uint64_t, &vote::get_number>>>
      votes;

  typedef multi_index<"issues"_n, issue> issues;
};