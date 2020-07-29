const clustering = require('@maxmouchet/julia-node-extension-demo');
const express = require('express');

const app = express();
const port = 3000;

// Convert and transpose a JS array of rows to a matrix (column-major storage),
// based on JS TypedArray.
const toJuliaMatrix = (Type, data) => {
  const nr = data.length;
  const nc = data[0].length;
  const matrix = new Type(nr * nc);
  for (let j = 0; j < nr; j += 1) {
    for (let i = 0; i < nc; i += 1) {
      matrix[j * nc + i] = data[j][i];
    }
  }
  return matrix;
};

// Pretty-printing of JS TypedArray
function replacer(key, value) {
  if (key === 'assignments') {
    return Array.from(value.values());
  }
  if (key === 'centers') {
    const res = [];
    for (let i = 0; i < value.length; i += 2) {
      res.push([value[i], value[i + 1]]);
    }
    return res;
  }
  return value;
}

process.on('SIGINT', () => process.exit());

app.use(express.json());

app.post('/kmeans', (req, res) => {
  const d = req.body.data[0].length;
  const { k } = req.body;
  const matrix = toJuliaMatrix(Float64Array, req.body.data);
  const result = clustering.kmeans(matrix, d, k);
  res.send(JSON.stringify(result, replacer));
});

app.listen(port, () => console.log(`Example app listening at http://localhost:${port}`));
