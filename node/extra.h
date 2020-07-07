#ifndef NODE_EXTRA_H_
#define NODE_EXTRA_H_

#include <julia.h>

// Convert `vec` to a `Vector{type}`.
jl_value_t *jl_convert_vector(jl_datatype_t *type, jl_value_t *vec) {
  jl_value_t *atype = jl_apply_array_type((jl_value_t *)type, 1);
  jl_function_t *convert = jl_get_function(jl_base_module, "convert");
  return jl_call2(convert, atype, vec);
}

jl_value_t *jl_tuple2(jl_datatype_t *type, void *a, void *b) {
  jl_value_t *ttype = jl_tupletype_fill(2, (jl_value_t *)type);
  return jl_new_struct((jl_datatype_t *)ttype, a, b);
}

jl_array_t *jl_ptr_to_matrix(jl_datatype_t *type, void *data, size_t nr,
                             size_t nc, int own_buffer) {
  jl_value_t *atype = jl_apply_array_type((jl_value_t *)type, 2);
  jl_value_t *dims =
      jl_tuple2(jl_int64_type, jl_box_int64(nr), jl_box_int64(nc));
  return jl_ptr_to_array(atype, data, dims, own_buffer);
}

jl_module_t *jl_require(jl_module_t *into, const char *module) {
  jl_function_t *require = jl_get_function(jl_base_module, "require");
  jl_sym_t *mod = jl_symbol(module);
  return (jl_module_t *)jl_call2(require, (jl_value_t *)into,
                                 (jl_value_t *)mod);
}

#endif  // NODE_EXTRA_H_
