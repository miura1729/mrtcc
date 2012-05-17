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

  mrb_get_args(mrb, "s", &prog);
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

void
mrb_init_time(mrb_state *mrb)
{
  struct RClass *tc;
  tc = mrb_define_class(mrb, "Tcc", mrb->object_class);
  mrb_define_class_method(mrb, tc, "new", mrb_tcc_new, ARGS_NONE());

  mrb_define_method(mrb, tc, "compile_string", mrb_tcc_compile_string, ARGS_REQ(1));
}
