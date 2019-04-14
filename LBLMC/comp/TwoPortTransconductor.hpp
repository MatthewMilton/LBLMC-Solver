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

#ifndef TWOPORTTRANSCONDUCTOR_HPP
#define TWOPORTTRANSCONDUCTOR_HPP

#include "LBLMC/DataTypes.hpp"

namespace LBLMC
{

/**
 * @brief Two-Port Transconductance/Coupling Model
 *
 * encapsulates the fixed transconductances that couple two (2-terminal) ports together in voltage
 * controlled current source fashion.
 *
 * The 2-port transconductance coupling can be modeled with VCCS:
 *
 *  p1a -------   --------- p2a
 * I=g_12*vp2 |   | I = g_21*vp1
 *            V   V
 *  p1b -------   --------- p2b
 *
 *  where p1a,plb are terminals of port 1, p2a,p2b are terminals of port 2. g_12 and g_21 are the transconductances
 *  and vp1 and vp2 are the voltage across each respective port.
 *
 * Useful in zonal decomposition of large models into smaller models where ports are coupled by transconductance.
 *
 * This component can also be used for multi-port models with more than 2 ports.  Simply use this component between
 * every 2 ports that are coupled by transconductance.
 *
 * @note This class is NOT intended for RTL Synthesis
 */
class TwoPortTransconductor
{
private:
	const NumType transconductance12; ///< transconductance (S) seen by port 1 from port 2
	const NumType transconductance21; ///< transconductance (S) seen by port 2 from port 1

public:
	/**
	 * parameter constructor
	 * @param transconductance12 transconductance (S) seen by port 1 from port 2; VCCS gain across port 1
	 * @param transconductance21 transconductance (S) seen by port 2 from port 1; VCCS gain across port 2
	 */
	TwoPortTransconductor(NumType transconductance12, NumType transconductance21);

	/**
	 * stamps conductance of component into given conductance matrix (Non-Synthesis ONLY)
	 *
	 * This method expects the conductance matrix to be correct size and it to be a square matrix
	 *
	 * @note This method is NOT intended to be synthesizable to RTL.
	 *
	 * @param conduct_mat conductance matrix to stamp
	 * @param dim dimension of square conductance matrix (width or height)
	 * @param port1a index of (a) positive terminal of port 1 of component; zero is ground
	 * @param port1b index of (b) negative terminal of port 1 of component; zero is ground
	 * @param port2a index of (a) positive terminal of port 2 of component; zero is ground
	 * @param port2b index of (b) negative terminal of port 2 of component; zero is ground
	 * @return
	 */
	int stampConductance(NumType* conduct_mat, unsigned int dim, unsigned int port1a, unsigned int port1b,
			unsigned int port2a, unsigned int port2b);
};

} //namespace LBLMC

#endif //TWOPORTTRANSCONDUCTOR_HPP
