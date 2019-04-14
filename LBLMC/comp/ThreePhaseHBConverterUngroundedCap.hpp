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


#ifndef THREEPHASEHBCONVERTERUNGROUNDEDCAP_HPP
#define THREEPHASEHBCONVERTERUNGROUNDEDCAP_HPP

#include <vector>

#include "LBLMC/DataTypes.hpp"

namespace LBLMC
{

/**
 * @brief Three Phase Half-Bridge Converter
 *
 * The model can be used for DC/DC converters, DC/AC inverters, rectifiers, etc.
 *
 * The neutral on the DC side is ungrounded (0v) by default
 */
class ThreePhaseHBConverterUngroundedCap
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
			il3,	///< phase 3 inductor current
			ipos,	///< positive capacitor current
			ineg,	///< negative capacitor current
			epos_past,	///< stores past positive input voltage
			eneu_past, ///< stores past DC neutral input voltage
			eneg_past, ///< stores past negative input voltage
			eout1_past, ///< stores past phase 1 voltage
			eout2_past, ///< stores past phase 2 voltage
			eout3_past, ///< stores past phase 3 voltage
			vc1_past,	///< stores past positive cap voltage
			vc2_past,	///< stores past negative cap voltage
			il1_past,	///< stores past phase 1 ind. current
			il2_past,	///< stores past phase 2 ind. current
			il3_past;	///< stores past phase 3 ind. current
	bool
			sw1,	///< registered switch control for phase 1
			sw2,	///< registered switch control for phase 2
			sw3;	///< registered switch control for phase 3



public:

	ThreePhaseHBConverterUngroundedCap(NumType dt, NumType cap, NumType ind, NumType res);

	/**
	 * \brief top-level function for this model
	 *
	 * \param epos in : DC positive voltage input (capacitor side)
	 * \param eneu in : DC neutral/zero voltage input (capacitor side)
	 * \param eneg in : DC negative voltage input (capacitor side)
	 * \param eout1 in : AC phase 1 voltage output (inductor side)
	 * \param eout2 in : AC phase 2 voltage output (inductor side)
	 * \param eout3 in : AC phase 3 voltage output (inductor side)
	 * \param bpos out : DC positive b source current
	 * \param bneu out : DC neutral/zero source current
	 * \param bneg out : DC negative b source current
	 * \param bout1 out : AC phase 1 b source current
	 * \param bout2 out : AC phase 2 b source current
	 * \param bout3 out : AC phase 3 b source current
	 * \param sw_ctrl1 in : phase 1 Switch Control signal
	 * \param sw_ctrl2 in : phase 2 Switch Control signal
	 * \param sw_ctrl3 in : phase 3 Switch Control signal
	 * \param sw_en in : turns on (true) or off (false) all switches completely (dead band)
	 */
	void update(NumType epos, NumType eneu, NumType eneg, NumType eout1, NumType eout2, NumType eout3,
			NumType* bpos, NumType* bneu, NumType* bneg, NumType* bout1, NumType* bout2, NumType* bout3,
			bool sw_ctrl1, bool sw_ctrl2, bool sw_ctrl3, bool sw_en);

	/**
		\brief measures the currents through the converter inductors

		In a time step, this method should be called after update() is called to get present
		current measurements; otherwise, past currents are reported

		\param current1 the current through inductor of phase A (1)
		\param current2 the current through inductor of phase B (2)
		\param current3 the current through inductor of phase C (3)
	**/
	void measureInductorCurrents(NumType* current1, NumType* current2, NumType* current3);

	/**
		\brief measures the voltages across the converter capacitors

		In a time step, this method should be called after update() is called to get present
		voltage measurements; otherwise, past voltages are reported

		\param voltage_p the voltage across positive (upper) capacitor
		\param voltage_n the voltage across negative (lower) capacitor
	**/
	void measureCapacitorVoltages(NumType* voltage_p, NumType* voltage_n);

	/**
	 * stamps conductance of component into given conductance matrix (Non-Synthesis ONLY)
	 *
	 * This method expects the conductance matrix to be correct size and it to be a square matrix
	 *
	 * @note This method is NOT intended to be synthesizable to RTL.
	 *
	 * \param conduct_mat conductance matrix to stamp
	 * \param dim dimension of square conductance matrix (width or height)
	 * \param np index of positive DC side terminal of component; zero is ground
	 * \param nu index of neutral DC side terminal of component; zero is ground/common
	 * \param nn index of negative DC side terminal of component; zero is ground
	 * \param na index of phase a (out1) terminal of component; zero is ground
	 * \param nb index of phase b (out2) terminal of component; zero is ground
	 * \param nc index of phase c (out3) terminal of component; zero is ground
	 *
	 * @return 0 if successful, -1 if cannot stamp conductance to matrix due to matrix dimension size
	 */
	int stampConductance(NumType* conduct_mat, unsigned int dim,
			unsigned int np,
			unsigned int nu,
			unsigned int nn,
			unsigned int na,
			unsigned int nb,
			unsigned int nc
			);

    /**
        @brief stamps the nodes of source(s) of this component into a vector that can be used to
        construct equations for the system source vector b (out of Gx=b).

        This method appends node indices to the given sources vector that correspond to how the
        sources of this component is to be stamped into the system.

        @note This method is NOT intended to be synthesizable to RTL.

        \param sources the vector that will be stamped into
        \param np index of positive DC side terminal of component; zero is ground
        \param nu index of neutral DC side terminal of component; zero is ground/common
        \param nn index of negative DC side terminal of component; zero is ground
        \param na index of phase a (out1) terminal of component; zero is ground
        \param nb index of phase b (out2) terminal of component; zero is ground
        \param nc index of phase c (out3) terminal of component; zero is ground

    **/
    void stampSources(std::vector<unsigned int>& sources,
                unsigned int np,
                unsigned int nu,
                unsigned int nn,
                unsigned int na,
                unsigned int nb,
                unsigned int nc
                );

    /**
        @brief stamps the conductances and sources of this component into the system (Gx=b)

        Effectively the same as calling stampConductance() and stampSources() sequentially

        @note This method is NOT intended to be synthesizable to RTL.

        \param conduct_mat conductance matrix to stamp
        \param dim dimension of square conductance matrix (width or height)
        \param sources the vector that will be stamped into
        \param np index of positive DC side terminal of component; zero is ground
        \param nu index of neutral DC side terminal of component; zero is ground/common
        \param nn index of negative DC side terminal of component; zero is ground
        \param na index of phase a (out1) terminal of component; zero is ground
        \param nb index of phase b (out2) terminal of component; zero is ground
        \param nc index of phase c (out3) terminal of component; zero is ground

        @return 0 if successful, -1 if cannot stamp conductance to matrix due to matrix dimension size


    **/
    int stampSystem(NumType* conduct_mat, unsigned int dim, std::vector<unsigned int>& sources,
                unsigned int np,
                unsigned int nu,
                unsigned int nn,
                unsigned int na,
                unsigned int nb,
                unsigned int nc
                );

};

} //namespace LBLMC

#endif // THREEPHASEHBCONVERTERUNGROUNDEDCAP_HPP

