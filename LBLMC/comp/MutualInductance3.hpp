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

 #ifndef MUTUALINDUCTANCE3_HPP
 #define MUTUALINDUCTANCE3_HPP

 #include <vector>

 #include "LBLMC/Params.hpp"
 #include "LBLMC/DataTypes.hpp"

 namespace LBLMC
 {

 /**
    @brief component model of a 3-coil mutual inductance (coupled inductors) for LB-LMC simulator

    This component can be used as a general transformer with pure inductance but non-ideal coupling.

    This component uses Euler Forward to explicitly discretize its state equations

    The state equation for the model is:

    V1 = L1 * dI1/dt + M12 * dI2/dt + M13 * dI3/dt\n
    V2 = M12  * dI1/dt + L2 * dI2/dt + M23 * dI3/dt\n
    V3 = M13 * dI1/dt + M23 * dI2/dt + L3 * dI3/dt

    In matrix form, this model becomes:

    [V] = [L]*d/dt([I]), where\n
    [L] = [ L1 M12 M13; M12 L2 M23; M13 M23 L3; ]

    The [L] matrix must be invertible (non-ideal mutual inductance, aka coupling).


    @author Matthew Milton
    @date 2018
 **/
 class MutualInductance3
 {
 private:
	const NumType dt;   ///< simulation time step
	const NumType L1;   ///< inductance of 1st inductor
	const NumType L2;   ///< inductance of 2nd inductor
	const NumType L3;   ///< inductance of 3rd inductor
	const NumType M12;  ///< mutual inductance between 1st and 2nd inductors
	const NumType M23;  ///< mutual inductance between 2nd and 3rd inductors
	const NumType M31;  ///< mutual inductance between 3rd and 1st inductors
	const NumType d;    ///< internal constant
	const NumType k1;   ///< internal constant
	const NumType k2;   ///< internal constant
	const NumType k3;   ///< internal constant
	const NumType k4;   ///< internal constant
	const NumType k5;   ///< internal constant
	const NumType k6;   ///< internal constant
	const NumType k7;   ///< internal constant
	const NumType k8;   ///< internal constant
	const NumType k9;   ///< internal constant

	NumType voltage1;        ///< voltage across 1st inductor
    NumType voltage2;        ///< voltage across 2nd inductor
    NumType voltage3;        ///< voltage across 3rd inductor
    NumType current1;        ///< present current through 1st inductor
    NumType current2;        ///< present current through 2nd inductor
    NumType current3;        ///< present current through 3rd inductor
    NumType current_comp1;   ///< companion source current of 1st inductor
    NumType current_comp2;   ///< companion source current of 2nd inductor
    NumType current_comp3;   ///< companion source current of 3rd inductor

 public:

	MutualInductance3(NumType dt, NumType L1, NumType L2, NumType L3,
		NumType M12,NumType M23,NumType M31);
	//MutualInductance3(NumType dt, NumType L[9]);

	void update(NumType epos1, NumType eneg1, NumType epos2, NumType eneg2, NumType epos3, NumType eneg3,
		NumType* bout1, NumType* bout2, NumType* bout3);

	int stampConductance(NumType* conduct_mat, unsigned int dim,
		unsigned int npos1, unsigned int nneg1, unsigned int npos2, unsigned int nneg2, unsigned int npos3, unsigned int nneg3);


    void stampSources(std::vector<unsigned int>& sources,
        unsigned int npos1, unsigned int nneg1, unsigned int npos2, unsigned int nneg2, unsigned int npos3, unsigned int nneg3);

    int stampSystem(NumType* conduct_mat, unsigned int dim, std::vector<unsigned int>& sources,
        unsigned int npos1, unsigned int nneg1, unsigned int npos2, unsigned int nneg2, unsigned int npos3, unsigned int nneg3);
 };

 } //namespace LBLMC

 #endif
