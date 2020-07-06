const binary = require('node-pre-gyp');
const path = require('path');

const bindingPath = binary.find(path.resolve(path.join(__dirname, './package.json')));
module.exports = require(bindingPath);
