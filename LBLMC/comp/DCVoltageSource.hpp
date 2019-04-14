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

#ifndef DCVOLTAGESOURCE_HPP
#define DCVOLTAGESOURCE_HPP

#include <vector>

#include "LBLMC/DataTypes.hpp"

namespace LBLMC
{

/**
 * @brief DC Voltage Source with series resistance
 *
 * Acts as a constant voltage source, modeled as its Norton Equivalent.
 */
class DCVoltageSource
{
private:

	const NumType vs;	///< value of the source voltage
	const NumType gs;	///< series conductance of the source
	const NumType bs;	///< current of the source as Norton equivalent

public:

	/**
	 * parameterized constructor
	 * @param vs value of the source voltage; default is 1.0
	 * @param rs value of the source series resistance; default is 0.001
	 */
	DCVoltageSource(NumType vs = 1.0, NumType rs = 0.001);

	void update(NumType* bout);

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
