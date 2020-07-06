# julia-node-extension-demo

[![CI](https://github.com/maxmouchet/julia-node-extension-demo/workflows/CI/badge.svg)](https://github.com/maxmouchet/julia-node-extension-demo/actions?query=workflow%3ACI)
[![JuliaCon 2020](https://img.shields.io/badge/Talk-JuliaCon%202020-761c7c)](https://pretalx.com/juliacon2020/talk/Q88P8U/)

**Work in progress.**

#### TODO

- [ ] Release workflow
- [ ] CI build/test steps (test on fresh Julia installation)
- [ ] Windows not supported (build errors, cross-compilation not supported by node-gyp, PR welcome!)
- [ ] Example project (Express API)
- [ ] README
- [ ] Document node-pre-gyp paths
- [ ] Find a package that uses system libs (SpecialFunctions ?) in order to test lib. relocation.
- [ ] npm package

```bash
# Requires Node 14 (for BigInt64Array) (N-API > 5).

# Pre-built binaries are avaiable for Linux, macOS and Windows (x86-64).
# If you are on a different architecture, node-pre-gyp will trigger a build.
npm install julia-node-demo
```

## Overview

- Describe tools (PackageCompiler.jl, node-gyp, ...)

## Resources

- https://docs.julialang.org/en/v1/manual/embedding/
- https://github.com/JuliaLang/julia/blob/master/src/julia.h
- https://github.com/JuliaLang/julia/blob/master/src/jlapi.c
- https://github.com/nodejs/node-addon-api/blob/master/doc/node-gyp.md
- https://github.com/mapbox/node-pre-gyp
- [julia #32614 - Make it easier for embedded Julia to use a custom system image](https://github.com/JuliaLang/julia/issues/32614)
- [julia #28886 - Test and document embedding with dynamically loaded libjulia.](https://github.com/JuliaLang/julia/pull/28886)

```
julia/ # build a julia system image
node/ # C++ extension
```

## Adapt to your package

- In julia/build.jl: replace project path, and precompile_execution_file if needed.

## Project Status

The package is tested against xxx.