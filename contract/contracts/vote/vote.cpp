#include "vote.hpp"

/**
 *  setvote sets user's vote and change the attitude if vote exists.
 */
void Vote::setvote(name voter, uint8_t attitude, uint64_t issueNum,
                   asset deposit) {
  require_auth(voter);
  eosio_assert(voter != _self, "voter cannot be contract itself");
  eosio_assert(deposit.is_valid(), "deposit is not a valid asset");
  eosio_assert(deposit.symbol.code() != symbol_code("ZJUBCA"),
               "please use ZJUBCA tokens to vote");
  eosio_assert(attitude == 0 || attitude == 1,
               "attitude value should be 0(pros) and 1(cons) ");

  // check the ZJUBCA token balance is enough or not
  const auto balance =
      Vote::get_balance("zjubcatokent"_n, voter, symbol_code("ZJUBCA"));
  eosio_assert(deposit <= balance, "deposit exceeds the balance of voter");

  votes votes(_self, voter.value);
  auto vote = votes.find(issueNum);
  uint64_t oldDeposit = 0;
  if (vote == votes.end()) {
    votes.emplace(_self, [&](auto &row) {
      row.number = issueNum;
      row.voter = voter;
      row.attitude = attitude;
      row.value = deposit.amount;
    });
  } else {
    oldDeposit = vote->attitude == 0 ? vote->value : -vote->value;
    votes.modify(vote, same_payer, [&](auto &row) {
      row.attitude = attitude;
      row.value = deposit.amount;  // one token stands for ten votes
    });
  }

  issues issues(_self, _self.value);
  auto issue = issues.find(issueNum);
  if (issue == issues.end()) {
    issues.emplace(_self, [&](auto &row) {
      row.number = issueNum;
      if (attitude == 0) {
        row.totalValue = deposit.amount;
      }
      row.isPassed = false;
    });
  } else {
    issues.modify(issue, same_payer, [&](auto &row) {
      row.totalValue += oldDeposit;
      if (attitude == 0) {
        row.totalValue += deposit.amount;
      } else {
        if (row.totalValue < deposit.amount) {
          row.totalValue = 0;
        } else {
          row.totalValue -= deposit.amount;
        }
      }
    });
  }
}

/**
 * withdraw draws back all the deposit.
 */
void Vote::withdraw(name voter, uint64_t issueNum) {
  require_auth(voter);
  eosio_assert(voter != _self, "voter cannot be contract itself");

  votes votes(_self, voter.value);
  auto vote = votes.find(issueNum);
  eosio_assert(vote != votes.end(),
               "you have not voted for or against this issue");
  votes.erase(vote);
}

void Vote::pass(uint64_t issueNum) {
  // only contract account can invoke the actoin.
  require_auth(_self);
  issues issues(_self, _self.value);
  auto issue = issues.find(issueNum);
  eosio_assert(issue != issues.end(), "issue is not found");
  issues.modify(issue, same_payer, [&](auto &row) { row.isPassed = true; });
}

EOSIO_DISPATCH(Vote, (setvote)(withdraw)(pass))
