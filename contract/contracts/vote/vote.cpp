#include "vote.hpp"

/**
 *  setvote sets user's vote and change the attitude if vote exists.
 */
void Vote::setvote(name voter, uint8_t attitude, uint64_t issueNum,
                   asset deposit) {
  require_auth(voter);
  eosio_assert(voter != _self, "voter cannot be contract itself");
  eosio_assert(deposit.is_valid(), "deposit is not a valid asset");
  eosio_assert(deposit.symbol.code() == symbol_code("ZJUBCA"),
               "please use ZJUBCA tokens to vote");
  eosio_assert(attitude == 0 || attitude == 1,
               "attitude value should be 0(pros) and 1(cons) ");
  eosio_assert(deposit.amount != 0, "deposit value should not be zero");

  // check the ZJUBCA token balance is enough or not
  const auto balance =
      Vote::get_balance("zjubcatokens"_n, voter, symbol_code("ZJUBCA"));
  eosio_assert(deposit <= balance, "deposit exceeds the balance of voter");

  votes votes(_self, issueNum);
  auto vote = votes.find(voter.value);
  uint8_t oldAttitude;
  uint64_t oldValue;
  bool voteExisted = vote != votes.end();
  if (!voteExisted) {
    votes.emplace(_self, [&](auto &row) {
      row.number = issueNum;
      row.voter = voter;
      row.attitude = attitude;
      row.value = deposit.amount;
    });
  } else {
    oldAttitude = vote->attitude;
    oldValue = vote->value;
    votes.modify(vote, same_payer, [&](auto &row) {
      row.attitude = attitude;
      row.value = deposit.amount;  // one token stands for one votes
    });
  }

  issues issues(_self, issueNum);
  auto issue = issues.find(issueNum);
  if (issue == issues.end()) {
    issues.emplace(_self, [&](auto &row) {
      row.number = issueNum;
      if (attitude == 0) {
        row.proValue = deposit.amount;
      } else {
        row.conValue = deposit.amount;
      }
      row.isPassed = false;
      row.isClosed = false;
    });
  } else {
    eosio_assert(issue->isClosed == false,
                 "issue has been closed and cannot be voted anymore");
    issues.modify(issue, same_payer, [&](auto &row) {
      // if voter has voted the issue
      if (voteExisted) {
        if (oldAttitude == 0) {
          row.proValue -= oldValue;
        } else {
          row.conValue -= oldValue;
        }
      }
      if (attitude == 0) {
        row.proValue += deposit.amount;
      } else {
        row.conValue += deposit.amount;
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

  votes votes(_self, issueNum);
  auto vote = votes.find(voter.value);
  eosio_assert(vote != votes.end(),
               "you have not voted for or against this issue");
  votes.erase(vote);

  issues issues(_self, issueNum);
  auto issue = issues.find(issueNum);
  eosio_assert(issue != issues.end(), "issue is not found");
  eosio_assert(issue->isPassed == false,
               "issue has been passed and cannot be withdrown");
  issues.modify(issue, same_payer, [&](auto &row) {
    if (vote->attitude == 0) {
      row.proValue -= vote->value;
    } else {
      row.conValue -= vote->value;
    }
  });
}

/**
 *  close issue with a passed flag. 0 (passed), 1(not passed)
 */
void Vote::close(uint64_t issueNum, uint8_t passed) {
  // only contract account can invoke the actoin.
  require_auth(_self);
  issues issues(_self, issueNum);
  auto issue = issues.find(issueNum);
  eosio_assert(issue != issues.end(), "issue is not found");
  eosio_assert(issue->isClosed == false, "issue has been closed");
  issues.modify(issue, same_payer, [&](auto &row) {
    row.isPassed = !passed;
    row.isClosed = true;
  });
}

/**
 * reopen a closed issue, and set the state to `not passed`.
 */
void Vote::reopen(uint64_t issueNum) {
  require_auth(_self);
  issues issues(_self, issueNum);
  auto issue = issues.find(issueNum);
  eosio_assert(issue != issues.end(), "issue is not found");
  eosio_assert(issue->isClosed == true, "issue has not been closed");
  issues.modify(issue, same_payer, [&](auto &row) {
    row.isPassed = false;
    row.isClosed = false;
  });
}

EOSIO_DISPATCH(Vote, (setvote)(withdraw)(close)(reopen))
