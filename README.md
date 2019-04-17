# LB-LMC Solver C++ Library

Object-Oriented C++ Library for the development of real-time simulation solvers of multi-physics systems, such as electrical and power electronic systems, utilizing the Latency-Based Linear Multistep Compound (LB-LMC) method.

Utilizing this library and FPGA High-Level Synthesis (HLS) tools, real-time simulation solvers of sizable power electronic systems executing on FPGA devices with 50ns time steps are feasible.

## Requirements

This library requires C++03 or higher.

The main library depends on only C++ standard library.  The optional partial code generation components requires the following third-party libraries to compile:
  * Eigen 3 -- Linear Algebra Template Library http://eigen.tuxfamily.org/
