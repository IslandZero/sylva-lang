// foundation.h
//
// Header for sylva-runtime system library, provides base class, string, array and hash.
//
// Created by Yanke Guo on 2016/9/27.
//

#ifndef __SYLVA_FOUNDATION_H__
#define __SYLVA_FOUNDATION_H__

#include "sylva/runtime.h"

/***********************************************************************************************************************
 * Number
 **********************************************************************************************************************/

/**
 * Number, virtual class for primitives (float, integer, boolean, nil)
 *
 * !!WARN!! this is a placeholder, should be overriden by target source
 * !!WARN!! this is a virtual class, it will never create instances, instead, Sylva runtime will resolve functions for
 * all numeric primitive values into instance functions of Number class
 */
SYLVA_EXTERN sylva_class SYLVA_Number;

SYLVA_EXTERN sylva_value SYLVA_Number_S_new(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_init(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_deinit(sylva_value context, sylva_args args);

/**
 * Not
 */
SYLVA_EXTERN sylva_value SYLVA_Number_I_not(sylva_value context, sylva_args args);

/**
 * Basic Operation
 */
SYLVA_EXTERN sylva_value SYLVA_Number_I_add(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_sub(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_mul(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_div(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_mod(sylva_value context, sylva_args args);

/**
 * Comparison
 */
SYLVA_EXTERN sylva_value SYLVA_Number_I_compare(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_lt(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_lt_eq(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_gt(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_gt_eq(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_eq(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_not_eq(sylva_value context, sylva_args args);

/**
 * Logical Operators
 */
SYLVA_EXTERN sylva_value SYLVA_Number_I_or(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_and(sylva_value context, sylva_args args);

/**
 * Bit Operators
 */
SYLVA_EXTERN sylva_value SYLVA_Number_I_bit_or(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_bit_and(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_bit_xor(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_rshift(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_lshift(sylva_value context, sylva_args args);

/**
 * Number Convertion
 */
SYLVA_EXTERN sylva_value SYLVA_Number_I_to_i(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_to_f(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Number_I_to_b(sylva_value context, sylva_args args);

/**
 * Other Functions
 */

SYLVA_EXTERN sylva_value SYLVA_Number_I_abs(sylva_value context, sylva_args args);

/***********************************************************************************************************************
 * Object
 **********************************************************************************************************************/

/**
 * Object
 *
 * !!WARN!! this is a placeholder, should be overridden by target source
 */
SYLVA_EXTERN sylva_class SYLVA_Object;

/**
 * default init does nothing
 */
SYLVA_EXTERN sylva_value SYLVA_Object_I_init(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Object_I_deinit(sylva_value context, sylva_args args);

SYLVA_EXTERN sylva_value SYLVA_Object_I_class(sylva_value context, sylva_args args);

/***********************************************************************************************************************
 * String
 **********************************************************************************************************************/

#endif //__SYLVA_FOUNDATION_H__