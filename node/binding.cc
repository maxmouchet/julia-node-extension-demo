#include <julia.h>
#include <napi.h>

#include <algorithm>

#include "./extra.h"
#include "./init.h"

// `Init` and `NODE_API_MODULE` comments are from
// https://github.com/nodejs/node-addon-api/blob/master/doc/node-gyp.md.

using namespace Napi;

void rethrow_julia_exception(Env env) {
  if (jl_exception_occurred()) {
    jl_function_t *showerror = jl_get_function(jl_base_module, "showerror");
    jl_function_t *sprint = jl_get_function(jl_base_module, "sprint");
    jl_value_t *err = jl_call2(sprint, showerror, jl_exception_occurred());
    throw Error::New(env, jl_string_data(err));
  }
}

Value Kmeans(const CallbackInfo &info) {
  Env env = info.Env();

  if (info.Length() != 3) {
    throw TypeError::New(env, "kmeans(X: Float64Array, d: Number, k: Number)");
  }

  // (1) Extract JS arguments
  Float64Array X = info[0].As<Float64Array>();
  Number d = info[1].As<Number>();
  Number k = info[2].As<Number>();

  // (2) Wrap in Julia types
  jl_value_t *jl_k = jl_box_int64(k.Int64Value());
  jl_array_t *jl_X = jl_ptr_to_matrix(jl_float64_type, X.Data(), d.Int64Value(),
                                      X.ElementLength() / d.Int64Value(), 0);

  // (3) Load Julia module
  jl_module_t *jl_clustering = jl_require(jl_main_module, "Clustering");
  rethrow_julia_exception(info.Env());
  jl_function_t *jl_clustering_kmeans =
      jl_get_function(jl_clustering, "kmeans");

  // (4) Call Julia function
  jl_value_t *jl_result =
      jl_call2(jl_clustering_kmeans, (jl_value_t *)jl_X, jl_k);
  rethrow_julia_exception(info.Env());

  jl_value_t *jl_assignments = jl_get_field(jl_result, "assignments");
  jl_value_t *jl_centers = jl_get_field(jl_result, "centers");
  jl_value_t *jl_converged = jl_get_field(jl_result, "converged");
  jl_value_t *jl_iterations = jl_get_field(jl_result, "iterations");

  // We convert Int64s to Int32s since Node.js has limited Int64 support.
  jl_value_t *jl_assignments_i32 =
      jl_convert_vector(jl_int32_type, jl_assignments);

  // (5) Convert Julia values
  // We need to copy Julia GC managed arrays to Node GC managed arrays.
  Int32Array assignments =
      Int32Array::New(env, jl_array_len(jl_assignments_i32));
  std::copy_n((int32_t *)jl_array_data(jl_assignments_i32),
              jl_array_len(jl_assignments_i32), assignments.Data());

  Float64Array centers = Float64Array::New(env, jl_array_len(jl_centers));
  std::copy_n((double *)jl_array_data(jl_centers), jl_array_len(jl_centers),
              centers.Data());

  bool converged = jl_unbox_bool(jl_converged);
  int64_t iterations = jl_unbox_int64(jl_iterations);

  Object obj = Object::New(info.Env());
  obj.Set("assignments", assignments);
  obj.Set("centers", centers);
  obj.Set("converged", converged);
  obj.Set("iterations", iterations);
  return obj;
}

/**
 * This code is our entry-point. We receive two arguments here, the first is the
 * environment that represent an independent instance of the JavaScript runtime,
 * the second is exports, the same as module.exports in a .js file.
 * You can either add properties to the exports object passed in or create your
 * own exports object. In either case you must return the object to be used as
 * the exports for the module when you return from the Init function.
 */
Object Init(Env env, Object exports) {
  init_julia();

  // TODO: exit_julia()
  // Not implemented (yet), see
  // https://github.com/nodejs/node-addon-api/pull/397.

  exports.Set(String::New(env, "kmeans"), Function::New(env, Kmeans));

  return exports;
}

/**
 * This code defines the entry-point for the Node addon, it tells Node where to
 * go once the library has been loaded into active memory. The first argument
 * must match the "target" in our *binding.gyp*. Using NODE_GYP_MODULE_NAME
 * ensures that the argument will be correct, as long as the module is built
 * with node-gyp (which is the usual way of building modules). The second
 * argument points to the function to invoke. The function must not be
 * namespaced.
 */
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
