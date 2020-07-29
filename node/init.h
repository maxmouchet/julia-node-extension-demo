#ifndef NODE_INIT_H_
#define NODE_INIT_H_

#include <julia.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <libgen.h>
#endif

#ifdef _WIN32
#define LIBJULIA "libjulia.dll"
#elif __APPLE__
#define LIBJULIA "libjulia.dylib"
#else
#define LIBJULIA "libjulia.so"
#endif

// See https://github.com/JuliaLang/julia/issues/32614
// Adapted from jl_init()
// https://github.com/JuliaLang/julia/blob/master/src/jlapi.c
char *find_libjulia_dir() {
  char *libjuliadir = NULL;
#ifdef _WIN32
  void *hdl = (void *)jl_load_dynamic_library(NULL, JL_RTLD_DEFAULT, 0);
  if (hdl) {
    char *to_free = (char *)jl_pathname_for_handle(hdl);
    if (to_free) {
      libjuliadir = strdup(dirname(to_free));
      free(to_free);
    }
  }
#else
  Dl_info dlinfo;
  if (dladdr((void *)jl_init, &dlinfo) != 0 && dlinfo.dli_fname) {
    char *to_free = strdup(dlinfo.dli_fname);
    libjuliadir = strdup(dirname(to_free));
    free(to_free);
  }
#endif
  if (!libjuliadir) {
    printf("unable to find libjulia!\n");
    abort();
  }
  return libjuliadir;
}

void init_julia() {
#ifndef _WIN32
  // https://docs.julialang.org/en/latest/manual/embedding/
  // > Currently, dynamically linking with the libjulia shared library requires
  // > passing the RTLD_GLOBAL option.
  dlopen(LIBJULIA, RTLD_NOW | RTLD_GLOBAL);
#endif

  // https://github.com/JuliaLang/PackageCompiler.jl/blob/master/src/embedding_wrapper.c
  // => We're in a shared library, not an executable, so we need to find
  // libjuliadir.
  char *libjuliadir = find_libjulia_dir();

  char buf[PATH_MAX];
  snprintf(buf, sizeof(buf), "JULIA_LOAD_PATH=%s", libjuliadir);
  putenv(buf);

  jl_init_with_image(libjuliadir, "sysimage");

  free(libjuliadir);
}

void exit_julia() { jl_atexit_hook(0); }

#endif  // NODE_INIT_H_
