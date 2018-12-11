#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <zjubca.token/zjubca.token.hpp>
using namespace eosio;

void vote::setVote(name voter, uint64_t issueNum, asset deposit) {
  require_auth(voter);
  eosio_assert(voter != _self, "voter cannot be contract itself");
  eosio_assert(deposit.is_valid(), "deposit is not a valid asset");
  eosio_assert(deposit.symbol.name() != "ZJUBCA"_n,
               "please use ZJUBCA tokens to vote");

  // check the ZJUBCA token balance is enough or not
  auto balance = zjubca::token.get_balance("zjubcatokens"_n, voter, "ZJUBCA"_n);
  eosio_assert(deposit <= balance, "balance not enough");

  issue_index issues(_self, voter.value);
  auto issue = issues.find(issueNum);
  if (issue == issues.end()) {
    issues.emplace(voter, [&](auto &row) {
      row.number = issueNum;
      row.voter = voter;
      row.value = deposit.value;
    })
  } else {
    issues.modify(issue, same_payer, [&](auto &row) {
      row.value = deposit.amount  // one token stands for ten votes
    })
  }
}

void vote::withdraw(name voter, uint64_t issueNum) {
  require_auth(voter);
  eosio_assert(voter != _self, "voter cannot be contract itself");

  issue_index issues(_self, voter.value);
  auto issue = issues.find(issueNum);
  eosio_assert(issue != issues.end(), "you have not voted.");
  issues.erase(issue);
}

EOSIO_ABI(vote, (setVote)(withdraw))
