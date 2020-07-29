# Integrate Julia and Javascript with Node.js extensions

[![CI](https://github.com/maxmouchet/julia-node-extension-demo/workflows/CI/badge.svg)](https://github.com/maxmouchet/julia-node-extension-demo/actions?query=workflow%3ACI)
[![Publish](https://github.com/maxmouchet/julia-node-extension-demo/workflows/Publish/badge.svg)](https://github.com/maxmouchet/julia-node-extension-demo/actions?query=workflow%3APublish)
[![npm](https://img.shields.io/npm/v/@maxmouchet/julia-node-extension-demo)](https://www.npmjs.com/package/@maxmouchet/julia-node-extension-demo)
[![JuliaCon 2020](https://img.shields.io/badge/Talk-JuliaCon%202020-761c7c)](https://pretalx.com/juliacon2020/talk/Q88P8U/)

This repository shows how to build a Node.js native extension that calls Julia code from a precompiled system image.
For an introduction in video, see the [JuliaCon 2020 talk](https://pretalx.com/juliacon2020/talk/Q88P8U/).

[Overview](#overview)
• [Requirements](#requirements)
• [Running the extension](#running-the-extension)
• [Building the extension](#building-the-extension)
• [Resources](#resources)

## Overview

In this repository we consider the following use case:
- You have an algorithm written in Julia, and you want to use this algorithm in a Node.js environment.  
- You can't or you don't want to install Julia on your servers.  
- You want to avoid Julia compilation costs and you want to share data efficiently between the Julia and Node.js runtimes.

To achieve this goal, we propose a two-step approach:
1. Build a Julia system image which includes your precompiled code.
2. Build a Node.js native extension in C/C++ which glues together the Julia and Node.js runtimes.

The main drawbacks of this approach are:
- It requires to write C/C++ glue code.
- Changes to the Julia code requires re-building the system image, and (potentially) adapting the C/C++ code.

<p align="center">
  <img src="/assets/binding.png" alt="N-API diagram" width="800px"/><br/>
  The main goal of the Node.js extension is to convert between Julia and Node.js data types.
</p>

<!-- This approach is similar to [pyjulia](https://github.com/JuliaPy/pyjulia) for Python or [JuliaCall](https://cran.r-project.org/web/packages/JuliaCall/index.html) for R, with the addition of the system image. -->

We use the following tools:
- [PackageCompiler.jl](https://github.com/JuliaLang/PackageCompiler.jl) to build the Julia system image.
- [node-addon-api](https://github.com/nodejs/node-addon-api) a C++ wrapper of [N-API](https://nodejs.org/dist/latest/docs/api/n-api.html) to write the Node.js extension.
- [node-gyp](https://github.com/nodejs/node-gyp) to build the extension.
- [node-pre-gyp](https://github.com/mapbox/node-pre-gyp) to package and distribute the extension.

### Repository structure

```bash
├── binding.gyp        # Build system configuration
├── example            # Example Express API
│   ├── app.js           # Server
│   └── test.js          # Client
├── index.js           # Extension loader
├── julia              # Julia system image
│   ├── build.jl         # System image build script
│   └── Clustering.jl    # Target package
├── node               # Node.js extension
│   ├── binding.cc       # Extension code
│   ├── extra.h          # Helpers for the Julia C API
│   └── init.h           # Helpers for Julia initialization
└── test.js            # Extension test
```

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

See [test.js](test.js) and [example/](example/) for more complete examples.

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

- [Julia Documentation: Embedding](https://docs.julialang.org/en/v1/manual/embedding/)
- [julia.h](https://github.com/JuliaLang/julia/blob/master/src/julia.h) and [jlapi.c](https://github.com/JuliaLang/julia/blob/master/src/jlapi.c) source files to browse the Julia C API.
- [node-addon-api usage with node-gyp](https://github.com/nodejs/node-addon-api/blob/master/doc/node-gyp.md)
- [node-pre-gyp documentation](https://github.com/mapbox/node-pre-gyp)
- [julia #32614 - Make it easier for embedded Julia to use a custom system image](https://github.com/JuliaLang/julia/issues/32614)
- [julia #28886 - Test and document embedding with dynamically loaded libjulia.](https://github.com/JuliaLang/julia/pull/28886)

## Project Status

The package is tested against Julia 1.4 and Node.js v10/v14, on Ubuntu and macOS.
