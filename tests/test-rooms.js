const { test } = require("./helper");

async function testCreateRoom() {
    await test('GET', '/');
}

module.exports = { testCreateRoom };
