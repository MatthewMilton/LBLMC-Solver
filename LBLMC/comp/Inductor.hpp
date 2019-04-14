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

#ifndef INDUCTOR_HPP
#define INDUCTOR_HPP

#include <vector>

#include "LBLMC/DataTypes.hpp"

namespace LBLMC
{

/**
 * @brief Inductor Model
 *
 * Separates non-linear behavior from the conductance, moving it to the source computations.
 *
 */
//need to fix this since double/float can't be used as non-type template parameter!!!
class Inductor
{
private:
	const NumType dt;
	const NumType ind;

	const NumType hol2;

	NumType
		epos_past,
		eneg_past,
		delta_v,
		delta_v_past,
		current,
		current_eq,
		current_past,
		current_eq_past;

public:

	Inductor(NumType dt, NumType ind);

	void update(NumType epos, NumType eneg, NumType* bout);

	/**
		\brief measures current through this two-terminal component at present time
		\param current object to store the measured current value
	**/
	void measureThroughCurrent(NumType* current);

	/**
		\brief measures the voltage across this two-terminal component at present time
		\param epos positive terminal voltage
		\param eneg negative terminal voltage
		\param voltage object to store the measured voltage value
	**/
	void measureAcrossVoltage(NumType epos, NumType eneg, NumType* voltage);

	/**
	 * stamps conductance of component into given conductance matrix (Non-Synthesis ONLY)
	 *
	 * This method expects the conductance matrix to be correct size and it to be a square matrix
	 *
	 * @note This method is NOT intended to be synthesizable to RTL.
	 *
	 * @param conduct_mat conductance matrix to stamp
	 * @param dim dimension of square conductance matrix (width or height)
	 * @param npos index of positive terminal of component; zero is ground
	 * @param nneg index of negative terminal of component; zero is ground
	 *
	 * @return 0 if successful, -1 if cannot stamp conductance to matrix due to matrix dimension size
	 */
	int stampConductance(NumType* conduct_mat, unsigned int dim, unsigned int npos, unsigned int nneg);

	/**
        @brief stamps the nodes of source(s) of this component into a vector that can be used to
        construct equations for the system source vector b (out of Gx=b).

        This method appends node indices to the given sources vector that correspond to how the
        sources of this component is to be stamped into the system.

        @note This method is NOT intended to be synthesizable to RTL.

        @param sources the vector that will be stamped into
        @param npos index of positive terminal of component; zero is ground
        @param nneg index of negative terminal of component; zero is ground
    **/
    void stampSources(std::vector<unsigned int>& sources, unsigned int npos, unsigned int nneg);

    /**
        @brief stamps the conductances and sources of this component into the system (Gx=b)

        Effectively the same as calling stampConductance() and stampSources() sequentially

        @note This method is NOT intended to be synthesizable to RTL.

        @param conduct_mat conductance matrix to stamp
        @param dim dimension of square conductance matrix (width or height)
        @param sources the vector that will be stamped into
        @param npos index of positive terminal of component; zero is ground
        @param nneg index of negative terminal of component; zero is ground
        @return 0 if successful, -1 if cannot stamp conductance to matrix due to matrix dimension size


    **/
    int stampSystem(NumType* conduct_mat, unsigned int dim, std::vector<unsigned int>& sources, unsigned int npos, unsigned int nneg);

};

} //namespace LBLMC

#endif
