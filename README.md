# Integrate Julia and Javascript with Node.js extensions

[![CI](https://github.com/maxmouchet/julia-node-extension-demo/workflows/CI/badge.svg)](https://github.com/maxmouchet/julia-node-extension-demo/actions?query=workflow%3ACI)
[![Publish](https://github.com/maxmouchet/julia-node-extension-demo/workflows/Publish/badge.svg)](https://github.com/maxmouchet/julia-node-extension-demo/actions?query=workflow%3APublish)
[![npm](https://img.shields.io/npm/v/@maxmouchet/julia-node-extension-demo)](https://www.npmjs.com/package/@maxmouchet/julia-node-extension-demo)
[![JuliaCon 2020](https://img.shields.io/badge/Talk-JuliaCon%202020-761c7c)](https://pretalx.com/juliacon2020/talk/Q88P8U/)

**Work in progress.**

This repository shows how to build a Node.js native extension that calls Julia code from a precompiled system image.
For an introduction in video, see the [JuliaCon 2020 talk](https://pretalx.com/juliacon2020/talk/Q88P8U/).

[Overview](#overview)
• [Requirements](#requirements)
• [Running the extension](#running-the-extension)
• [Building the extension](#building-the-extension)
• [Resources](#resources)

#### TODO

- [ ] Windows not supported (build errors, cross-compilation not supported by node-gyp, PRs welcome!)
- [ ] Example project (Express API)
- [ ] README (+ table of contents)
- [ ] Document node-pre-gyp paths
- [ ] Find a package that uses system libs (SpecialFunctions?) in order to test lib. relocation.

## Overview

- Describe tools (PackageCompiler.jl, node-gyp, ...)

<p align="center">
  <img src="/assets/binding.png" alt="N-API diagram" width="800px"/>
</p>

## Requirements

- Node.js v10.16+ ([N-API v4](https://nodejs.org/api/n-api.html#n_api_n_api_version_matrix))
- Julia v1.4 (only for building the extension)
- x86-64 Linux or macOS; Windows is not (yet) supported due to build issues (PR are welcome!)

## Running the extension

### Installation

```bash
# Requires Node v10.16+ (N-API >= 4).
# Windows is not supported (yet).

# Pre-built binaries are avaiable for Linux and macOS (x86-64).
# If you are on a different architecture, node-pre-gyp will trigger a build.
npm install @maxmouchet/julia-node-extension-demo
```

### Example

```js
const clustering = require('@maxmouchet/julia-node-extension-demo')
// X: dxn matrix (n d-dimensional data points) in column-major order
// k: number of clusters
clustering.kmeans(X: Float64Array, d: Number, k: Number)
```

See [test.js](test.js) and [example/](example/) for detailed examples.

## Building the extension

```bash
npm install node-pre-gyp

# Run build.jl and compile binding.cc
# => dist/linux-x64-napi-v4/binding.node
./node_modules/node-pre-gyp/bin/node-pre-gyp build

# Package everything together
# => build/stage/julia-node-extension-demo/vx.y.z/Release/linux-x64-napi-v4.tar.gz
./node_modules/node-pre-gyp/bin/node-pre-gyp package

# Upload the package to Amazon S3
./node_modules/node-pre-gyp/bin/node-pre-gyp publish
```

## Resources

- https://docs.julialang.org/en/v1/manual/embedding/
- https://github.com/JuliaLang/julia/blob/master/src/julia.h
- https://github.com/JuliaLang/julia/blob/master/src/jlapi.c
- https://github.com/nodejs/node-addon-api/blob/master/doc/node-gyp.md
- https://github.com/mapbox/node-pre-gyp
- https://www.mikeash.com/pyblog/friday-qa-2009-11-06-linking-and-install-names.html
- [julia #32614 - Make it easier for embedded Julia to use a custom system image](https://github.com/JuliaLang/julia/issues/32614)
- [julia #28886 - Test and document embedding with dynamically loaded libjulia.](https://github.com/JuliaLang/julia/pull/28886)

```
julia/ # build a julia system image
node/ # C++ extension
```

## Adapt to your package

- In julia/build.jl: replace project path, and precompile_execution_file if needed.

## Debugging

- Manual build of the image
- Node-pre-gyp by hand

## Project Status

The package is tested against Julia 1.4 and Node.js v10/v14, on Ubuntu and macOS.
