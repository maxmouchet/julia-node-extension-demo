// const clustering = require('./build/Release/binding.node');
const clustering = require('./index.js').binding;
// const clustering = require('./build-tmp-napi-v6/Release/binding.node');
// const clustering = require('./napi-v6-linux-x64/binding.node');

// TODO: Lint

const randNormal = (mean, std) => {
  const u = Math.random();
  const v = Math.random();
  const x = Math.sqrt(-2 * Math.log(u)) * Math.cos(2 * Math.PI * v);
  return mean + std * x;
};

const randDataset = (d, n, k) => {
  // No matrix type in JS, so manually.
  // Julia is column-major (columns after columns).
  // kmeans expects a dxn data matrix (each column is a d-dimensional data point).
  // data[i,j] = data[j*d+i]
  const data = new Float64Array(d * n);

  for (let j = 0; j < n; j += 1) {
    const z = Math.floor(Math.random() * k);
    for (let i = 0; i < d; i += 1) {
      data[j * d + i] = randNormal(z, 0.5);
    }
  }

  return data;
};

// Generate some random data.
const dataset = randDataset(2, 100, 3);

// Keyword arguments?
console.log(clustering.kmeans(dataset, 2, 3));

// Show exception example with k > n.
