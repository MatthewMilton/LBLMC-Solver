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

#ifndef TWOPHASEHBCONVERTER_HPP
#define TWOPHASEHBCONVERTER_HPP

#include "LBLMC/Params.hpp"
#include "LBLMC/DataTypes.hpp"

namespace LBLMC
{

/**
 * @brief Two Phase Half-Bridge Converter
 *
 * The model can be used for DC/DC converters, DC/AC inverters, rectifiers, etc.
 *
 * The neutral on the DC side is defaulted to ground (0V)
 */

class TwoPhaseHBConverter
{
private:

	 const NumType dt;
	 const NumType cap;
	 const NumType ind;
	 const NumType res;

	const NumType hoc;
	const NumType hol;
	const NumType cap_conduct;


	NumType
			vc1,	///< positive capacitor voltage
			vc2, 	///< negative capacitor voltage
			il1, 	///< phase 1 inductor current
			il2,	///< phase 2 inductor current
			ipos,	///< positive capacitor current
			ineg,	///< negative capacitor current
			epos_past,	///< stores past positive input voltage
			eneg_past, ///< stores past negative input voltage
			eout1_past, ///< stores past phase 1 voltage
			eout2_past, ///< stores past phase 2 voltage
			vc1_past,	///< stores past positive cap voltage
			vc2_past,	///< stores past negative cap voltage
			il1_past,	///< stores past phase 1 ind. current
			il2_past;	///< stores past phase 2 ind. current
	bool
			sw1,	///< registered switch control for phase 1
			sw2;	///< registered switch control for phase 2



public:

	TwoPhaseHBConverter(NumType dt, NumType cap, NumType ind, NumType res);

	/**
	 * @brief top-level function for this model
	 *
	 * @param epos in : DC positive voltage input
	 * @param eneg in : DC negative voltage input
	 * @param eout1 in : AC phase 1 voltage output
	 * @param eout2 in : AC phase 2 voltage output
	 * @param bpos out : DC positive b source current
	 * @param bneg out : DC negative b source current
	 * @param bout1 out : AC phase 1 b source current
	 * @param bout2 out : AC phase 2 b source current
	 * @param sw_ctrl1 in : phase 1 Switch Control signal
	 * @param sw_ctrl2 in : phase 2 Switch Control signal
	 */
	void update(NumType epos, NumType eneg, NumType eout1, NumType eout2,
			NumType* bpos, NumType* bneg, NumType* bout1, NumType* bout2,
			bool sw_ctrl1, bool sw_ctrl2);

	/**
	 * stamps conductance of component into given conductance matrix (Non-Synthesis ONLY)
	 *
	 * This method expects the conductance matrix to be correct size and it to be a square matrix
	 *
	 * @note This method is NOT intended to be synthesizable to RTL.
	 *
	 * @param conduct_mat conductance matrix to stamp
	 * @param dim dimension of square conductance matrix (width or height)
	 * @param np index of positive DC side terminal of component; zero is ground
	 * @param nn index of negative DC side terminal of component; zero is ground
	 * @param na index of phase a (out1) terminal of component; zero is ground
	 * @param nb index of phase b (out2) terminal of component; zero is ground
	 *
	 * @return 0 if successful, -1 if cannot stamp conductance to matrix due to matrix dimension size
	 */
	int stampConductance(NumType* conduct_mat, unsigned int dim,
			unsigned int np, unsigned int nn, unsigned int na, unsigned int nb);

private:

	void updateVC(bool sw_ctrl1, bool sw_ctrl2);

	void updateIPosNeg(NumType epos, NumType eneg);

	void updateIL(bool sw_ctrl1, bool sw_ctrl2, NumType eout1, NumType eout2);

	void updateBElements(NumType* bpos, NumType* bneg, NumType* bout1, NumType* bout2);

};

} //namespace LBLMC

#endif // TWOPHASEHBCONVERTER_HPP
