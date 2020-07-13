const clustering = require('.');

const randNormal = (mean, std) => {
  const u = Math.random();
  const v = Math.random();
  const x = Math.sqrt(-2 * Math.log(u)) * Math.cos(2 * Math.PI * v);
  return mean + std * x;
};

const randDataset = (d, n, k) => {
  // There is no matrix type in JavaScript:
  // [[...], [...], ...] is an array of array (array of pointers).
  // Julia is "column-major"; the columns of a matrix are stored
  // one after the other, contiguously, in memory: data[i,j] = data[j*d+i].
  // kmeans expects a dxn data matrix (each column is a d-dimensional data point).
  const data = new Float64Array(d * n);

  for (let j = 0; j < n; j += 1) {
    const z = Math.floor(Math.random() * k);
    for (let i = 0; i < d; i += 1) {
      data[j * d + i] = randNormal(z, 0.5);
    }
  }

  return data;
};

const dataset = randDataset(2, 100, 3);
console.log(clustering.kmeans(dataset, 2, 3));

// Example exception when k > n.
clustering.kmeans(dataset, 2, 300);
