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

#ifndef LBLMCPARAMS_HPP
#define LBLMCPARAMS_HPP

//==================================================================================================
//	Build/Synthesis
//==================================================================================================

//#define LBLMC_XILINX_VIVADO_HLS ///< uncomment to enable Xilinx Vivado HLS C/C++ libraries for FPGA HL synthesis

//==================================================================================================
//	Simulation Timing
//==================================================================================================

#define LMC_TIMESTEP 100.0e-9 ///< time step period for testbench and engine
#define LMC_SIM_TIME 600.0e-3 ///< total simulation time to run for testbenches

//#define LMC_DT_40NS ///< used to select conductance matrix and elements based on time step
//#define LMC_DT_45NS ///< used to select conductance matrix and elements based on time step
//#define LMC_DT_50NS ///< used to select conductance matrix and elements based on time step
//#define LMC_DT_55NS ///< used to select conductance matrix and elements based on time step
//#define LMC_DT_60NS ///< used to select conductance matrix and elements based on time step

//==================================================================================================
//	Data Logging and Sampling
//==================================================================================================

#define LMC_SAMPLE_MEMORY 1500e6 ///< amount of memory to use for sampling in bytes
#define LMC_SAMPLE_START_TIME 0.0 ///< simulation time to start sampling
#define LMC_SAMPLE_LOG_CSV_FILENAME "LBLMCModelOutputVHLS.csv" ///< filename of CSV file to store sampled data
#define LMC_SAMPLE_PERIOD 1 ///< integer, nonzero number of time steps between each sample; set to 1 to sample each time step; set to 100 for every 100 time steps

//==================================================================================================
//	General Control Handling
//==================================================================================================

#define LMC_CONTROL_UPDATE_PERIOD 1 ///< integer, nonzero number of time steps between updating the internal control for models; use 1 for every time step, 100 for every 100 steps

//==================================================================================================
//	Data Type Settings
//==================================================================================================

//72b, 29i, 43f defaults

	//fixed-point data type resolutions in number of bits
#define NUM_FIXED_POINT_SIZE 64	//number of bits in fixed-point NumType
#define NUM_FIXED_POINT_INT  29	//number of integer bits in fixed-point NumType
#define NUM_FIXED_POINT_FRAC (NUM_FIXED_POINT_SIZE - NUM_FIXED_POINT_INT)	//number of fractional bits in fixed-point NumType

	//uncomment one (only) to enable use of given numerical type; sets data type of Alias NumType
//#define LMC_USE_FIXED_POINT_TYPES	//Use AP/HLS fixed point type
#define LMC_USE_DOUBLE_FLOAT_POINT_TYPES	//Use 64-bit double-precision floating point type
//#define LMC_USE_SINGLE_FLOAT_POINT_TYPES	//Use 32-bit single-precision floating point type
//#define LMC_USE_HALF_FLOAT_POINT_TYPES	//Use 16-bit half-precision floating point type

//==================================================================================================
//	Code Operation Parameters
//==================================================================================================

	// only uncomment ONE of these definitions
//#define LMC_OFFLINE_SIMULATION_MODE	//use the codebase for offline simulation
//#define LMC_OFFLINE_COSIMULATION_MODE //use the codebase for offline C++/RTL co-simulation
//#define LMC_CODE_GENERATION_MODE    //use the codebase to generate sim engine source files for FPGA synthesis
//#define LMC_MODEL_DECOMPOSITION_MODE    //use the codebase to decompose a model into subnetworks under LB-LMC
#define LMC_FPGA_SYNTHESIS_MODE     //use the codebase for FPGA synthesis

//==================================================================================================
//	FPGA Implementation Specific Parameters
//==================================================================================================

#define LMC_DAC_CONVERSION_SIGNED_RANGE 32767	//max magnitude for D/A converter signed input
#define LMC_DAC_CONVERSION_UNSIGNED_RANGE 65535 //max magnitude for D/A converter unsigned input
#define LMC_XILINX_DDS_UNIT_RANGE 16383	//max magnitude for Xilinx DDS 16-bit output with unit (-0.5 to 0.5) range

//==================================================================================================
//==================================================================================================

#endif //LBLMCPARAMS_HPP
