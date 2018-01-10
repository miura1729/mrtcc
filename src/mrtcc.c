#include "mruby.h"
#include "libtcc.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"

static void
mrb_tcc_free(mrb_state *mrb, void *ptr)
{
  tcc_delete(ptr);
}

static struct mrb_data_type mrb_tcc_type = { "Tcc", mrb_tcc_free };

static mrb_value
mrb_tcc_wrap(mrb_state *mrb, struct RClass *tc,TCCState *tcc)
{
  return mrb_obj_value(Data_Wrap_Struct(mrb, tc, &mrb_tcc_type, tcc));
}

/* Allocates a new TCC object  */
static mrb_value
mrb_tcc_new(mrb_state *mrb, mrb_value self)
{
  TCCState *s;
  s = tcc_new();
  tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
  return mrb_tcc_wrap(mrb, mrb_class_ptr(self), s);
}

static mrb_value
mrb_tcc_compile_string(mrb_state *mrb, mrb_value self)
{
  TCCState *s;
  mrb_value prog;

  mrb_get_args(mrb, "o", &prog);
  s = DATA_PTR(self);
  return mrb_fixnum_value(tcc_compile_string(s, RSTRING_PTR(prog)));
}

static mrb_value
mrb_tcc_relocate(mrb_state *mrb, mrb_value self)
{
  TCCState *s;
  int size;
  void *mem;
  
  s = DATA_PTR(self);
  /* get needed size of the code */
  size = tcc_relocate(s, NULL);
  if (size == -1)
    return mrb_nil_value();

  /* allocate memory and copy the code into it */
  mem = mrb_malloc(mrb, size);
  tcc_relocate(s, mem);

  return mrb_fixnum_value(size);
}

static mrb_value
mrb_tcc_call(mrb_state *mrb, mrb_value self)
{
  TCCState *s;
  mrb_value fname;
  mrb_value *argv;
  int argc;
  int (*func)();
  
  mrb_get_args(mrb, "*", &argv, &argc);
  fname = *(argv++);
  argc--;
  s = DATA_PTR(self);

  func = tcc_get_symbol(s, RSTRING_PTR(fname));
  if (!func)
    return mrb_nil_value();

 switch (argc) {
  case 0:
    return mrb_fixnum_value(func());
    break;

  case 1:
    return mrb_fixnum_value(func(argv[0].value.i));
    break;

  case 2:
    return mrb_fixnum_value(func(argv[0].value.i, argv[1].value.i));
    break;

  case 3:
    return mrb_fixnum_value(func(argv[0].value.i, argv[1].value.i, argv[2].value.i));
    break;

  case 4:
    return mrb_fixnum_value(func(argv[0].value.i, argv[1].value.i, argv[2].value.i, argv[3].value.i));
    break;

  case 5:
    return mrb_fixnum_value(func(argv[0].value.i, argv[1].value.i, argv[2].value.i, argv[3].value.i, argv[4].value.i));
    break;
  }
 return mrb_nil_value();
}

void
mrb_mrtcc_gem_init(mrb_state *mrb)
{
  struct RClass *tc;
  tc = mrb_define_class(mrb, "Tcc", mrb->object_class);
  mrb_define_class_method(mrb, tc, "new", mrb_tcc_new, MRB_ARGS_NONE());

  mrb_define_method(mrb, tc, "compile_string", mrb_tcc_compile_string, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, tc, "relocate", mrb_tcc_relocate, MRB_ARGS_NONE());
  mrb_define_method(mrb, tc, "call", mrb_tcc_call, MRB_ARGS_ANY());
}

void
mrb_mrtcc_gem_final(mrb_state *mrb)
{
}
