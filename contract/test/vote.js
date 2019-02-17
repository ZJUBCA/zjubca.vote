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

    it('should success eosio set pro vote lower than balance to issue 10', async function () {
        let processed = await vote.setvote("eosio", 0, 10, "10.0000 ZJUBCA")
        assert.equal(Js4eos.okTransaction(processed), true)
    });

    it('should success eosio set con vote (lower than balance) to issue 20', async function () {
        let processed = await vote.setvote("eosio", 1, 20, "10.0000 ZJUBCA")
        assert.equal(Js4eos.okTransaction(processed), true)
    })

    it('should fail eosio set pro vote larger than balance to issue 10', async function () {
        let processed = await vote.setvote("eosio", 0, 10, "100000000.0000 ZJUBCA")
        assert.equal(Js4eos.okTransaction(processed), false)
    });

    it('should success eosio withdraw pro vote from issue 10', async function () {
        let processed = await vote.withdraw("eosio", 10)
        assert.equal(Js4eos.okTransaction(processed), true)
    });

    it('should success zjubcavote11 close issue 10 with `pass` state', async function () {
        await vote.setActor("zjubcavote11")
        let processed = await vote.close(10, 0)
        assert.equal(Js4eos.okTransaction(processed), true)
    });

    it('should fail zjubcavote11 close issue 10 with `not pass` state', async function () {
        await vote.setActor("zjubcavote11")
        let processed = await vote.close(10, 1)
        assert.equal(Js4eos.okTransaction(processed), false)
    });

    it('should success zjubcavote11 reopen issue 10', async function () {
        await vote.setActor("zjubcavote11")
        let processed = await vote.reopen(10)
        assert.equal(Js4eos.okTransaction(processed), true)
    });

    it('should fail zjubcavote11 reopen issue 20', async function () {
        await vote.setActor("zjubcavote11")
        let processed = await vote.reopen(20)
        assert.equal(Js4eos.okTransaction(processed), false)
    });
});

