//
// runtime_test.c
//
// Created by Yanke Guo on 2016/9/24.
//

/* *
 *  dog.sylva
 *  ------------------------------------------
 *  module Printer
 *    def print(x)
 *      // inline C code printf
 *    end
 *  end
 *
 *  class Dog
 *    include Printer
 *    static var count
 *
 *    static func main
 *       self.new.bark
 *    end
 *
 *    func name
 *      return 1
 *    end
 *
 *    func bark
 *      print self.name
 *    end
 *
 *    static print
 *      print self->count
 *    end
 *  end
 *
 *  dog.sylva - func_id table
 *  ------------------------------------------
 *  1 -> "name"
 *  2 -> "bark"
 *  3 -> "main"
 *  4 -> "print"
 *
 * */

#include <sylva/runtime.h>

#include <stdio.h>

// Use macro to declare func names and ids for debug purpose
//
// func ids should be dynamically assigned by sylva compiler

#define FUNC_NAME_NAME "name"
#define FUNC_NAME_BARK "bark"
#define FUNC_NAME_MAIN "main"
#define FUNC_NAME_PRINT "print"

#define MEMBER_NAME_COUNT "count"

#define FUNC_ID_NAME 1
#define FUNC_ID_BARK 2
#define FUNC_ID_MAIN 3
#define FUNC_ID_PRINT 4

#define MEMBER_ID_COUNT 1

sylva_class SYLVA_C_Dog;

/////////////////////   Function Declarations   ///////////////////////

sylva_value SYLVA_C_Dog_S_main(sylva_value self, sylva_args args) {
  sylva_value dog = sylva_value_object(sylva_object_create(&SYLVA_C_Dog));
  sylva_value total_count = sylva_class_members_get(SYLVA_C_Dog, MEMBER_ID_COUNT);
  sylva_value new_total_count = sylva_value_integer(total_count.integer_value + 1);
  sylva_class_members_set(SYLVA_C_Dog, MEMBER_ID_COUNT, new_total_count);
  sylva_retain(&dog);
  sylva_call(dog, FUNC_ID_BARK, 0);
  sylva_release(&dog);
  return sylva_value_integer(0);
}

sylva_value SYLVA_C_Dog_I_name(sylva_value self, sylva_args args) {
  return sylva_value_integer(1);
}

sylva_value SYLVA_C_Dog_I_bark(sylva_value self, sylva_args args) {
  sylva_value name = sylva_call(self, FUNC_ID_NAME, 0);
  sylva_call(self, FUNC_ID_PRINT, 1, name);
  return sylva_value_nil;
}

sylva_value SYLVA_M_Printer_I_print(sylva_value self, sylva_args args) {
  sylva_value content = sylva_args_get(args, 0);
  printf("%s:%ld/%ld\n",
         self.object_value->class->name,
         content.integer_value,
         sylva_class_members_get(*self.object_value->class, MEMBER_ID_COUNT).integer_value);
  return sylva_value_nil;
}

///////////////////////  Global Declarations ///////////////////////

sylva_func_id_registry sylva_runtime_func_id_registry = {
    .length = 4,
    .func_ids = (sylva_func_id[]) {FUNC_ID_NAME, FUNC_ID_BARK, FUNC_ID_MAIN, FUNC_ID_PRINT},
    .func_names = (char *[]) {FUNC_NAME_NAME, FUNC_NAME_BARK, FUNC_NAME_MAIN, FUNC_NAME_PRINT},
};

sylva_member_id_registry sylva_runtime_member_id_registry = {
    .length = 1,
    .member_ids = (sylva_member_id[]) {MEMBER_ID_COUNT},
    .member_names = (char *[]) {MEMBER_NAME_COUNT},
};

sylva_class SYLVA_C_Dog = {
    .name = "Dog",
    .super = NULL,
    .static_funcs = &(sylva_funcs) {
        .length = 1,
        .func_ids = (sylva_func_id[]) {FUNC_ID_MAIN},
        .funcs = (sylva_func[]) {&SYLVA_C_Dog_S_main},
    },
    .instance_funcs = &(sylva_funcs) {
        .length = 3,
        .func_ids = (sylva_func_id[]) {FUNC_ID_NAME, FUNC_ID_BARK, FUNC_ID_PRINT},
        .funcs = (sylva_func[]) {&SYLVA_C_Dog_I_name, &SYLVA_C_Dog_I_bark, &SYLVA_M_Printer_I_print},
    },
    .static_member_list = &(sylva_member_list) {
        .length = 1,
        .member_ids = (sylva_member_id[]) {MEMBER_ID_COUNT},
    },
    .instance_member_list = NULL,
    .members = &(sylva_members) {
        .length = 1,
        .member_ids = (sylva_member_id[]) {MEMBER_ID_COUNT},
        .member_values = (sylva_value[]) {sylva_bare_value_integer(0)},
        .member_options = (sylva_member_option[]) {sylva_member_normal},
    },
};

////////////////////// Main Entry  //////////////////////////////

int main(int argc, char **argv) {
  char *name = sylva_runtime_func_id_to_name(FUNC_ID_PRINT);
  printf("Resolve FuncName: %s", name);
  return (int) sylva_call(sylva_value_class(&SYLVA_C_Dog), FUNC_ID_MAIN, 0).integer_value;
}
