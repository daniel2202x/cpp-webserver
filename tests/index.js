const { testCreateRoom } = require("./test-rooms");

async function runAllTests() {
    await testCreateRoom();
}

runAllTests();
