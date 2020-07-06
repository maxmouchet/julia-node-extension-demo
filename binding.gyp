# vim: set ft=python:
# References:
# https://github.com/nodejs/node-addon-api/blob/master/doc/setup.md
# https://github.com/nodejs/node-addon-api/blob/master/doc/node-gyp.md
# https://github.com/mapbox/node-pre-gyp#configuring
{
    "targets": [
        {
            "target_name": "binding",
            "sources": ["node/binding.cc"],
            "cflags_cc": ["-fPIC"],
            "cflags_cc!": ["-fno-exceptions"],
            "defines": ["NAPI_VERSION=<(napi_build_version)",],
            "include_dirs": [
                '<!@(julia -E "abspath(Sys.BINDIR, Base.INCLUDEDIR, \\"julia\\")")',
                '<!@(node -p "require(\\"node-addon-api\\").include")',
            ],
            "libraries": [
                "-ljulia",
                "-L'<(PRODUCT_DIR)/lib'",
                # Linux
                "-Wl,-rpath,'$$ORIGIN/lib'",
                "-Wl,-rpath,'$$ORIGIN/lib/julia'",
                # macOS
                "-Wl,-rpath,'@loader_path/lib'",
                "-Wl,-rpath,'@loader_path/lib/julia'",
            ],
            "actions": [
                {
                    "action_name": "Build Julia system image",
                    "inputs": ["julia/build.jl"],
                    "conditions": [
                        ['OS == "linux"', {"outputs": ["<(PRODUCT_DIR)/lib/sysimage.so"]},],
                        ['OS == "win"', {"outputs": ["<(PRODUCT_DIR)/lib/sysimage.dll"]},],
                        ['OS == "mac"', {"outputs": ["<(PRODUCT_DIR)/lib/sysimage.dylib"]},],
                    ],
                    "action": ["julia", "julia/build.jl", "<(PRODUCT_DIR)"],
                }
            ],
            "msvs_settings": {"VCCLCompilerTool": {"ExceptionHandling": 1},},
            "xcode_settings": {
                "CLANG_CXX_LIBRARY": "libc++",
                "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                "MACOSX_DEPLOYMENT_TARGET": "10.7",
            },
        },
        {
            "target_name": "action_after_build",
            "type": "none",
            "dependencies": ["<(module_name)"],
            "actions": [
                {
                    "action_name": "Copy extension and Julia libraries",
                    "action": ["cp", "-Rp", "<(PRODUCT_DIR)/.", "<(module_path)/"],
                    "inputs": ["<(PRODUCT_DIR)/<(module_name).node"],
                    "outputs": ["<(module_path)/<(module_name).node"],
                }
            ],
        },
    ]
}
