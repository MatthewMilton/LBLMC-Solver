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


#ifndef LBLMC_COMPONENT_RLSWITCH_HPP
#define LBLMC_COMPONENT_RLSWITCH_HPP

#include "LBLMC/DataTypes.hpp"
#include <vector>

namespace LBLMC
{

/**
	\brief switch component with series inductance and resistance

	<pre>
	o--/ --@@@@--^^^^^-o
	Vp  Sw   L     R   Vn
	</pre>

	This component can be useful to model switches with high inductance or to model
	faults and shorts in a network.

	solution inputs: positive terminal voltage, negative terminal voltage\n
	signal inputs: sw; switch is closed if true, switch is open if false\n
	signal outputs: inductor current; current of the series inductor\n
	contribution output: companion current through series switch\n
	discretization method: Euler Forward

**/
class RLSwitch
{
private:
	const NumType DT;
	const NumType L;
	const NumType R;
	const NumType HOL;

	NumType current_past;
	NumType sw_past;

public:

	RLSwitch(NumType dt, NumType l, NumType r);

	void update(NumType epos, NumType eneg, bool sw, NumType* bout);

	NumType measureThroughCurrent(NumType* current);

	int stampConductance(NumType* conduct_mat, unsigned int dim, unsigned int npos, unsigned int nneg);

	void stampSources(std::vector<unsigned int>& sources, unsigned int npos, unsigned int nneg);

	int stampSystem(NumType* conduct_mat, unsigned int dim, std::vector<unsigned int>& sources, unsigned int npos, unsigned int nneg);
};


} //namespace LBLMC

#endif // LBLMC_COMPONENT_RLSWITCH_HPP
