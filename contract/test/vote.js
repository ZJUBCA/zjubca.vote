var options = {
    keyProvider: [
        '5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3',//add private key of accounts used in below test case
    ]
}

Js4eos = require("js4eos")(options)
var expect = require('chai').expect;
var assert = require('chai').assert;

describe("vote contract test", function () {
    //Must set a large timeout
    this.timeout(10000);
    var vote;
    before(async function () {
        vote = await Js4eos.RequireContract("vote")
        await vote.setActor("eosio")
    });

    /* ===== before the test, zjubcatokent has issued 1000.0000 ZJUBCA to eosio ===== */

    it('should success eosio set pro vote lower than balance to issue 1', async function () {
        let processed = await vote.setvote("eosio", 0, 1, "10.0000 ZJUBCA")
        assert.equal(Js4eos.okTransaction(processed), true)
    });

    it('should success eosio set con vote (lower than balance) to issue 2', async function () {
        let processed = await vote.setvote("eosio", 1, 2, "10.0000 ZJUBCA")
        assert.equal(Js4eos.okTransaction(processed), true)
    })

    it('should fail eosio set pro vote larger than balance to issue 1', async function () {
        let processed = await vote.setvote("eosio", 0, 1, "100000000.0000 ZJUBCA")
        assert.equal(Js4eos.okTransaction(processed), false)
    });

    it('should success eosio withdraw pro vote from issue 1', async function () {
        let processed = await vote.withdraw("eosio", 1);
        assert.equal(Js4eos.okTransaction(processed), true)
    });

    it('should success zjubcavote11 close issue 1 with `pass` state', async function () {
        await vote.setActor("zjubcavote11")
        let processed = await vote.close(1, 0)
        assert.equal(Js4eos.okTransaction(processed), true)
    });

    it('should fail zjubcavote11 close issue 1 with `not pass` state', async function () {
        await vote.setActor("zjubcavote11")
        let processed = await vote.close(1, 1)
        assert.equal(Js4eos.okTransaction(processed), false)
    });

    it('should success zjubcavote11 reopen issue 1', async function () {
        await vote.setActor("zjubcavote11")
        let processed = await vote.reopen(1)
        assert.equal(Js4eos.okTransaction(processed), true)
    });

    it('should fail zjubcavote11 reopen issue 2', async function () {
        await vote.setActor("zjubcavote11")
        let processed = await vote.reopen(2)
        assert.equal(Js4eos.okTransaction(processed), false)
    });
});

