//
//  sylva-string.c
//  sylva
//
//  Created by Yanke Guo on 2016/9/21.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#define __SYLVA_SOURCE__
#include "sylva/util.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sylva/util.h>

sl_string_ref sl_string_create_il(char *string, sl_index index, sl_index length) {
  assert(index + length <= strlen(string));
  sl_string_ref ref = malloc(sizeof(sl_string));
  ref->length = length;
  ref->string = malloc(sizeof(char) * (ref->length + 1));
  memcpy(ref->string, &string[index], sizeof(char) * length);
  ref->string[length] = 0;
  return ref;
}

sl_string_ref sl_string_create_l(char *string, sl_index length) {
  return sl_string_create_il(string, 0, length);
}

sl_string_ref sl_string_create(char *string) {
  return sl_string_create_l(string, strlen(string));
}

sl_index sl_string_seek_no_blank(sl_string_ref string, sl_index start) {
  for (sl_index i = start; i < string->length; i++) {
    if (!isblank(string->string[i])) {
      return i;
    }
  }
  return sl_index_not_found;
}

sl_index sl_string_seek_blank(sl_string_ref string, sl_index start) {
  for (sl_index i = start; i < string->length; i++) {
    if (isblank(string->string[i])) {
      return i;
    }
  }
  return sl_index_not_found;
}

sl_index sl_string_seek_new_line(sl_string_ref string, sl_index start) {
  for (sl_index i = start; i < string->length; i++) {
    if (string->string[i] == '\n' || string->string[i] == '\r') {
      return i;
    }
  }
  return sl_index_not_found;
}

sl_index sl_string_seek_id(sl_string_ref string, sl_index start) {
  for (sl_index i = start; i < string->length; i++) {
    if (!isalnum(string->string[i]) && string->string[i] != '_') {
      return i;
    }
  }
  return sl_index_not_found;
}

char sl_string_get_last(sl_string_ref string) {
  if (string->length > 0) {
    return string->string[string->length - 1];
  }
  return '\0';
}

void sl_string_append(sl_string_ref string, char *src) {
  size_t src_len = strlen(src);
  string->length += src_len;
  string->string = realloc(string->string, sizeof(char) * string->length);
  strncat(string->string, src, 200);
}

void sl_string_destroy(sl_string_ref string) {
  if (string == NULL)
    return;
  free(string->string);
  free(string);
}

sl_array_ref sl_array_create(sl_index capacity) {
  assert(capacity > 0);
  sl_array_ref array = malloc(sizeof(sl_array));
  array->capacity = capacity;
  array->count = 0;
  array->values = malloc(sizeof(void *) * capacity);
  return array;
}

void sl_array_append(sl_array_ref array, void *value) {
  //  expand array by 2 if needed
  if (array->count >= array->capacity) {
    array->values = realloc(array->values, array->capacity * sizeof(void *) * 2);
    assert(array->values != NULL);
    array->capacity *= 2;
  }
  array->values[array->count] = value;
  array->count++;
}

void sl_array_clear(sl_array_ref array) {
  if (array->value_deallocator) {
    for (sl_index i = 0; i < array->count; i++) {
      array->value_deallocator(array->values[i]);
    }
  }
  array->count = 0;
}

void sl_array_destroy(sl_array_ref array) {
  if (array == NULL)
    return;
  if (array->value_deallocator) {
    for (sl_index i = 0; i < array->count; i++) {
      array->value_deallocator(array->values[i]);
    }
  }
  free(array->values);
  free(array);
}
