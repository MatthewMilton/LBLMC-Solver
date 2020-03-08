# LB-LMC Solver C++ Library

This library is deprecated and is replaced by the solver codegen tools within the Open Real-Time Simulation (ORTiS) framework found here:
https://github.com/OpenRealTimeSimulation

This library is kept here as an educational reference implementation for LB-LMC simulation solvers.

----

Object-Oriented C++ Library for the development of real-time simulation solvers of multi-physics systems, such as electrical and power electronic systems, utilizing the Latency-Based Linear Multistep Compound (LB-LMC) method.

Utilizing this library and FPGA High-Level Synthesis (HLS) tools, real-time simulation solvers of sizable power electronic systems executing on FPGA devices with 50ns simulation time steps are feasible.

## Requirements

This library requires C++03 or higher.

The main library depends on only C++ standard library.  The optional partial code generation components requires the following third-party libraries to compile:
  * Eigen 3 -- Linear Algebra Template Library http://eigen.tuxfamily.org/

## Compilation and High-Level Synthesis

The library supports software compilation using:
 * GCC
 * MinGW
 * Any C++ compiler suite that supports standard C++03 or higher
 
Also supported by this library is FPGA high-level synthesis (HLS) which converts C++ code into hardware description languages (HDL, such as VHDL or Verilog) to be implemented as logic designs on FPGAs.  The library supports the following HLS tools:
* Xilinx Vivado HLS 2016.1 and up (tool supports only Xilinx FPGA platforms)

## License

LB-LMC Solver C++ Library is licensed under the GNU General Public License (GPL) v3.0 (https://www.gnu.org/licenses/).
 
## Literature 
  
The LB-LMC simulation solver algorithm was first proposed in:  
 * A. Benigni and A. Monti, "A Parallel Approach to Real-Time Simulation of Power Electronics Systems," in *IEEE Transactions on Power Electronics*, vol. 30, no. 9, pp. 5192-5206, Sept. 2015.
doi: 10.1109/TPEL.2014.2361868

FPGA execution of this algorithm and first use of this C++ library is demonstrated in the following papers:  
 * Milton, M. A.(2016). *A Comparison Of FPGA Implementation Of Latency-Based Solvers For Power Electronic System Real-Time Simulation*. (Master's thesis). Retrieved from https://scholarcommons.sc.edu/etd/3903

 * M. Milton, A. Benigni and J. Bakos, "System-Level, FPGA-Based, Real-Time Simulation of Ship Power Systems," in *IEEE Transactions on Energy Conversion*, vol. 32, no. 2, pp. 737-747, June 2017.
doi: 10.1109/TEC.2017.2692525

 * M. Difronzo, M. Milton, M. Davidson and A. Benigni, "Hardware-in-the-loop testing of high switching frequency power electronics converters," *2017 IEEE Electric Ship Technologies Symposium (ESTS)*, Arlington, VA, 2017, pp. 299-304.
doi: 10.1109/ESTS.2017.8069297
