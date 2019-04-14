/*

Copyright (C) 2017-2019 Matthew Milton
Copyright (C) 2017-2019 Andrea Benigni

This file is part of the LB-LMC Solver C++ Library.

LB-LMC Solver C++ Library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LB-LMC Solver C++ Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LB-LMC Solver C++ Library.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef LBLMCDATATYPES_HPP
#define LBLMCDATATYPES_HPP

#include "LBLMC/Params.hpp"

#ifdef LBLMC_XILINX_VIVADO_HLS
#include <ap_int.h>
#include <ap_fixed.h>
#include <hls_half.h>
#endif

namespace LBLMC
{

///////////////////////////////////////////////////////////////////////////////////////////////////


#if defined LMC_USE_FIXED_POINT_TYPES

//ap_fixed< size, int_bits, quant., overflow, wrap_bits?> (frac_bits = size - int_bits)

typedef ap_fixed<NUM_FIXED_POINT_SIZE, NUM_FIXED_POINT_INT, AP_RND> NumType;	///< fixed point type used for general storage of values
typedef ap_fixed<NUM_FIXED_POINT_SIZE, NUM_FIXED_POINT_INT, AP_RND> MulType;	///< fixed point type used for all multiplications
typedef ap_fixed<NUM_FIXED_POINT_SIZE, NUM_FIXED_POINT_INT, AP_RND> AddSubType; ///< fixed point type used for all additions or subtractions

#define LMC_NUM_TYPE_STRING  "Fixed Point" ///< string description of NumType for logging/printing purposes

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////


#if defined LMC_USE_HALF_FLOAT_POINT_TYPES

//half floats don't work well with the simulation engines except in few possible cases

typedef half NumType;
typedef half ResultType;
typedef half ComputeType;
typedef half MulType;
typedef half AddSubType;

#define LMC_NUM_TYPE_STRING  "Half Precision 16-bit float" ///< string description of NumType for logging/printing purposes

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////


#if defined LMC_USE_SINGLE_FLOAT_POINT_TYPES

typedef float NumType;
typedef float ResultType;
typedef float ComputeType;
typedef float MulType;
typedef float AddSubType;

#define LMC_NUM_TYPE_STRING  "Single Precision 32-bit float" ///< string description of NumType for logging/printing purposes

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////


#if defined LMC_USE_DOUBLE_FLOAT_POINT_TYPES

	//double types
typedef double NumType;
typedef double ResultType;
typedef double ComputeType;
typedef double MulType;
typedef double AddSubType;

#define LMC_NUM_TYPE_STRING  "Double Precision 64-bit float" ///< string description of NumType for logging/printing purposes

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace LBLMC

#endif // LBLMCDATATYPES_HPP
