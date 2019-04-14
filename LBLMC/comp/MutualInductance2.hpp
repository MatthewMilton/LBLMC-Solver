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

 #ifndef MUTUALINDUCTANCE2_HPP
 #define MUTUALINDUCTANCE2_HPP

 #include "LBLMC/Params.hpp"
 #include "LBLMC/DataTypes.hpp"

 namespace LBLMC
 {

 /**
    @brief component model of a 2-coil mutual inductance (coupled inductors) for LB-LMC simulator

    This component can be used as a general transformer with pure inductance.

    This component uses Euler Forward to explicitly discretize its state equations

    The state equation for the model is:

    V1 = L1 * dI1/dt + M * dI2/dt\n
    V2 = M  * dI1/dt + L2 * dI2/dt

    In matrix form, this model becomes:

    [V] = [L]*d/dt([I]), where\n
    [L] = [ L1 M; M L2; ]

    The [L] matrix must be invertible (non-ideal mutual inductance).

    @author Matthew Milton
    @date 2018
 **/
 class MutualInductance2
 {
 private:
	const NumType dt;        ///< simulation time step
    const NumType L1;        ///< inductance of 1st inductor
    const NumType L2;        ///< inductance of 2nd inductor
    const NumType M;         ///< mutual inductance between inductors, M = k*sqrt(L1*L2), k coupling coefficient 0.0-1.0
    const NumType k1;        ///< internal constant
    const NumType k2;        ///< internal constant
    const NumType k3;        ///< internal constant
    NumType voltage1;        ///< voltage across 1st inductor
    NumType voltage2;        ///< voltage across 2nd inductor
    NumType current1;        ///< present current through 1st inductor
    NumType current2;        ///< present current through 2nd inductor
    NumType current_comp1;   ///< companion source current of 1st inductor
    NumType current_comp2;   ///< companion source current of 2nd inductor

 public:

	/**
        @brief constructor
        @param dt time step length
        @param L1 inductance of 1st coil
        @param L2 inductance of 2nd coil
        @param M mutual inductance between coils
	**/
	MutualInductance2(NumType dt, NumType L1, NumType L2, NumType M);

	/**
        @brief updates state of component and the b-vector currents from it
        @param epos1 input voltage at terminal 1 of 1st coil
        @param eneg1 input voltage at terminal 2 of 1st coil
        @param epos2 input voltage at terminal 1 of 2nd coil
        @param eneg2 input voltage at terminal 2 of 2nd coil
        @param bout1 output current through 1st coil
        @param bout2 output current through 2nd coil
	**/
	void update(NumType epos1, NumType eneg1, NumType epos2, NumType eneg2, NumType* bout1, NumType* bout2);

	/**
        @brief stamps conductance of component into system conductance matrix

        This component has no conductance (using EF for integration) so, has no effect

        @param conduct_mat conductance matrix of system component resides
        @param dim dimension of (square) conductance matrix
        @param npos1 node index of terminal 1 of 1st coil
        @param nneg1 node index of terminal 2 of 1st coil
        @param npos2 node index of terminal 1 of 2nd coil
        @param nneg2 node index of terminal 2 of 2nd coil

	**/
	int stampConductance(NumType* conduct_mat, unsigned int dim, unsigned int npos1, unsigned int nneg1, unsigned int npos2, unsigned int nneg2);
 };

 }

 #endif
