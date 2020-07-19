const request = require('request');

const randNormal = (mean, std) => {
  const u = Math.random();
  const v = Math.random();
  const x = Math.sqrt(-2 * Math.log(u)) * Math.cos(2 * Math.PI * v);
  return mean + std * x;
};

const d = 2; // Observations dimensions
const n = 10; // Number of observations
const k = 3; // Number of clusters to generate

const data = new Array(n);

for (let i = 0; i < n; i += 1) {
  const z = Math.floor(Math.random() * k);
  data[i] = new Array(d);
  for (let j = 0; j < d; j += 1) {
    data[i][j] = randNormal(z, 0.5);
  }
}

const params = {
  data,
  k: 3,
};

console.log('POST http://localhost:3000/kmeans');
console.log(params);
request.post('http://localhost:3000/kmeans', { json: params }, (error, response, body) => {
  console.log(body);
});
