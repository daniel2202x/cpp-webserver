const SERVER = 'http://localhost:1234';

async function test(method, url, body = null) {
    const start = performance.now();

    const res = await fetch(SERVER + url, { method, body });
    const text = await res.text();

    const duration = performance.now() - start;
    console.log(`${method} ${url} took ${duration} ms`);

    return { ...res, text };
}

module.exports = { test };
