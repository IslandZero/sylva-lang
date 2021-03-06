//
//  sylva-lexer.c
//  sylva
//
//  Created by Yanke Guo on 2016/9/20.
//  Copyright © 2016年 IslandZERO. All rights reserved.
//

#define __SYLVA_SOURCE__
#include "sylva/util.h"
#include "sylva/lexer.h"

#include <stdlib.h>
#include <string.h>
#include <sylva/lexer.h>
#include <sylva/util.h>
#include <sylva/token.h>
#include <sylva/sylva.h>

sl_lexer_ref sl_lexer_create(sl_string_ref source) {
  sl_lexer_ref lexer = malloc(sizeof(sl_lexer));
  lexer->source = source;
  lexer->index = 0;
  return lexer;
}

sl_token_ref sl_lexer_next_token(sl_lexer_ref lexer) {
  sl_index idx = lexer->index;

  //  check index boundary
  if (idx >= lexer->source->length) {
    sl_token_ref token = sl_token_create(sl_token_eof);
    token->mark.column = lexer->source->length;
    return token;
  }

  //  seek first no blank char
  idx = sl_string_seek_no_blank(lexer->source, lexer->index);

  //  check index boundary
  if (idx == sl_index_not_found) {
    lexer->index = lexer->source->length;
    sl_token_ref token = sl_token_create(sl_token_eof);
    token->mark.column = lexer->source->length;
    return token;
  }

  //  get first char
  char first = lexer->source->string[idx];

  //  check new line, \n or \r\n
  if (first == '\n') {
    lexer->index = idx + 1;
    sl_token_ref token = sl_token_create(sl_token_new_line);
    token->mark.column = idx;
    return token;
  }

  if (first == '\r') {
    if (lexer->source->length > idx + 1 && lexer->source->string[idx + 1] == '\n') {
      lexer->index = idx + 2;
      sl_token_ref token = sl_token_create(sl_token_new_line);
      token->mark.column = idx;
      return token;
    }
  }

  //  inline c
  if (first == '`') {
    //  Find endIndex
    sl_index endIdx = sl_string_seek_new_line(lexer->source, idx);
    if (endIdx == sl_index_not_found) {
      endIdx = lexer->source->length;
    }
    lexer->index = endIdx;
    sl_token_ref token = sl_token_create_string_il(sl_token_inline_c, lexer->source->string, idx + 1, endIdx - idx - 1);
    token->mark.column = idx;
    return token;
  }

  //  sharp started (comment)
  if (first == '#') {
    //  Find endIndex
    sl_index endIdx = sl_string_seek_new_line(lexer->source, idx);
    if (endIdx == sl_index_not_found) {
      endIdx = lexer->source->length;
    }
    lexer->index = endIdx;
    sl_token_ref token = sl_token_create_string_il(sl_token_comment, lexer->source->string, idx + 1, endIdx - idx - 1);
    token->mark.column = idx;
    return token;
  }

  //  alphabet or underscore started
  if (isalpha(first) || first == '_') {
    sl_index endIdx = sl_string_seek_id(lexer->source, idx);
    if (endIdx == sl_index_not_found) {
      endIdx = lexer->source->length;
    }
    sl_string_ref sema = sl_string_create_il(lexer->source->string, idx, endIdx - idx);
    //  check keywords
    sl_token_ref token = NULL;
    if (strcmp(sema->string, "class") == 0) {
      token = sl_token_create(sl_token_class);
    } else if (strcmp(sema->string, "var") == 0) {
      token = sl_token_create(sl_token_var);
    } else if (strcmp(sema->string, "weak") == 0) {
      token = sl_token_create(sl_token_weak);
    } else if (strcmp(sema->string, "end") == 0) {
      token = sl_token_create(sl_token_end);
    } else if (strcmp(sema->string, "module") == 0) {
      token = sl_token_create(sl_token_module);
    } else if (strcmp(sema->string, "native") == 0) {
      token = sl_token_create(sl_token_native);
    } else if (strcmp(sema->string, "require") == 0) {
      token = sl_token_create(sl_token_require);
    } else if (strcmp(sema->string, "include") == 0) {
      token = sl_token_create(sl_token_include);
    } else if (strcmp(sema->string, "static") == 0) {
      token = sl_token_create(sl_token_static);
    } else if (strcmp(sema->string, "func") == 0) {
      token = sl_token_create(sl_token_function);
    } else if (strcmp(sema->string, "super") == 0) {
      token = sl_token_create(sl_token_super);
    } else if (strcmp(sema->string, "self") == 0) {
      token = sl_token_create(sl_token_self);
    } else if (strcmp(sema->string, "nil") == 0) {
      token = sl_token_create(sl_token_nil);
    } else if (strcmp(sema->string, "true") == 0) {
      token = sl_token_create(sl_token_true);
    } else if (strcmp(sema->string, "false") == 0) {
      token = sl_token_create(sl_token_false);
    } else if (strcmp(sema->string, "if") == 0) {
      token = sl_token_create(sl_token_if);
    } else if (strcmp(sema->string, "else") == 0) {
      token = sl_token_create(sl_token_else);
    } else if (strcmp(sema->string, "unless") == 0) {
      token = sl_token_create(sl_token_unless);
    } else if (strcmp(sema->string, "while") == 0) {
      token = sl_token_create(sl_token_while);
    } else if (strcmp(sema->string, "break") == 0) {
      token = sl_token_create(sl_token_break);
    } else if (strcmp(sema->string, "for") == 0) {
      token = sl_token_create(sl_token_for);
    } else if (strcmp(sema->string, "in") == 0) {
      token = sl_token_create(sl_token_in);
    } else if (strcmp(sema->string, "return") == 0) {
      token = sl_token_create(sl_token_return);
    } else {
      //  no keyword found, make it ID
      token = sl_token_create_string(sl_token_id, sema->string);
    }
    sl_string_destroy(sema);
    lexer->index = endIdx;
    token->mark.column = idx;
    return token;
  }

  //  marks and operators
  {
    sl_index nextIdx = 0, endIdx = idx;
    sl_token_ref token = NULL;
    switch (first) {
    case ':': {
      endIdx += 1;
      token = sl_token_create(sl_token_colon);
    }
      break;
    case ';': {
      endIdx += 1;
      token = sl_token_create(sl_token_semicolon);
    }
      break;
    case ',': {
      endIdx += 1;
      token = sl_token_create(sl_token_comma);
    }
      break;
    case '+': {
      endIdx += 1;
      token = sl_token_create(sl_token_plus);
    }
      break;
    case '-': {
      endIdx += 1;
      nextIdx = sl_string_seek_no_blank(lexer->source, endIdx);
      if (nextIdx != sl_index_not_found && lexer->source->string[nextIdx] == '>') {
        endIdx = nextIdx + 1;
        token = sl_token_create(sl_token_arrow);
      } else {
        token = sl_token_create(sl_token_minus);
      }
    }
      break;
    case '*': {
      endIdx += 1;
      token = sl_token_create(sl_token_multiply);
    }
      break;
    case '/': {
      endIdx += 1;
      token = sl_token_create(sl_token_divide);
    }
      break;
    case '%': {
      endIdx += 1;
      token = sl_token_create(sl_token_mod);
    }
      break;
    case '=': {
      endIdx += 1;
      //  seek ==
      nextIdx = sl_string_seek_no_blank(lexer->source, endIdx);
      if (nextIdx != sl_index_not_found && lexer->source->string[nextIdx] == '=') {
        //  seek ===
        sl_index nextNextIdx = sl_string_seek_no_blank(lexer->source, nextIdx + 1);
        if (nextNextIdx != sl_index_not_found && lexer->source->string[nextNextIdx] == '=') {
          endIdx = nextNextIdx + 1;
          token = sl_token_create(sl_token_identical_to);
        } else {
          endIdx = nextIdx + 1;
          token = sl_token_create(sl_token_eq_to);
        }
      } else {
        token = sl_token_create(sl_token_eq);
      }
    }
      break;
    case '<': {
      endIdx += 1;
      nextIdx = sl_string_seek_no_blank(lexer->source, endIdx);
      if (nextIdx != sl_index_not_found && lexer->source->string[nextIdx] == '<') {
        endIdx = nextIdx + 1;
        token = sl_token_create(sl_token_bit_lshift);
      } else if (nextIdx != sl_index_not_found && lexer->source->string[nextIdx] == '=') {
        endIdx = nextIdx + 1;
        token = sl_token_create(sl_token_lt_eq);
      } else if (nextIdx != sl_index_not_found && lexer->source->string[nextIdx] == '>') {
        endIdx = nextIdx + 1;
        token = sl_token_create(sl_token_compare);
      } else {
        token = sl_token_create(sl_token_lt);
      }
    }
      break;
    case '>': {
      endIdx += 1;
      nextIdx = sl_string_seek_no_blank(lexer->source, endIdx);
      if (nextIdx != sl_index_not_found && lexer->source->string[nextIdx] == '>') {
        endIdx = nextIdx + 1;
        token = sl_token_create(sl_token_bit_rshift);
      } else if (nextIdx != sl_index_not_found && lexer->source->string[nextIdx] == '=') {
        endIdx = nextIdx + 1;
        token = sl_token_create(sl_token_gt_eq);
      } else {
        token = sl_token_create(sl_token_gt);
      }
    }
      break;
    case '!': {
      endIdx += 1;
      nextIdx = sl_string_seek_no_blank(lexer->source, endIdx);
      if (nextIdx != sl_index_not_found && lexer->source->string[nextIdx] == '=') {
        endIdx = nextIdx + 1;
        token = sl_token_create(sl_token_not_eq);
      } else {
        token = sl_token_create(sl_token_not);
      }
    }
      break;
    case '|': {
      endIdx += 1;
      nextIdx = sl_string_seek_no_blank(lexer->source, endIdx);
      if (nextIdx != sl_index_not_found && lexer->source->string[nextIdx] == '|') {
        endIdx = nextIdx + 1;
        token = sl_token_create(sl_token_or);
      } else {
        token = sl_token_create(sl_token_bit_or);
      }
    }
      break;
    case '&': {
      endIdx += 1;
      nextIdx = sl_string_seek_no_blank(lexer->source, endIdx);
      if (nextIdx != sl_index_not_found && lexer->source->string[nextIdx] == '&') {
        endIdx = nextIdx + 1;
        token = sl_token_create(sl_token_and);
      } else {
        token = sl_token_create(sl_token_bit_and);
      }
    }
      break;
    case '^': {
      endIdx += 1;
      token = sl_token_create(sl_token_bit_xor);
    }
      break;
    case '~': {
      endIdx += 1;
      token = sl_token_create(sl_token_tilde);
    }
      break;
    case '?': {
      endIdx += 1;
      token = sl_token_create(sl_token_question);
    }
      break;
    case '(': {
      endIdx += 1;
      token = sl_token_create(sl_token_paren_l);
    }
      break;
    case ')': {
      endIdx += 1;
      token = sl_token_create(sl_token_paren_r);
    }
      break;
    case '[': {
      endIdx += 1;
      token = sl_token_create(sl_token_bracket_l);
    }
      break;
    case ']': {
      endIdx += 1;
      token = sl_token_create(sl_token_bracket_r);
    }
      break;
    case '{': {
      endIdx += 1;
      token = sl_token_create(sl_token_brace_l);
    }
      break;
    case '}': {
      endIdx += 1;
      token = sl_token_create(sl_token_brace_r);
    }
      break;
    case '.': {
      endIdx += 1;
      token = sl_token_create(sl_token_dot);
    }
      break;
    default:break;
    }

    if (token != NULL) {
      lexer->index = endIdx;
      token->mark.column = idx;
      return token;
    }
  }

  sl_token_ref token = sl_token_create(sl_token_invalid);
  token->mark.column = idx;
  return token;
}

void sl_lexer_reset(sl_lexer_ref lexer) {
  lexer->index = 0;
}

void sl_lexer_destroy(sl_lexer_ref lexer) {
  free(lexer);
}
