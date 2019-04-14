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


#include "RLSwitch.hpp"

namespace LBLMC
{

RLSwitch::RLSwitch(NumType dt, NumType l, NumType r) :
	DT(dt),
	L(l),
	R(r),
	HOL(dt/l),
	current_past(0.0),
	sw_past(false)
{}


void RLSwitch::update(NumType epos, NumType eneg, bool sw, NumType* bout)
{
	//using Euler Forward discretization

	NumType current;

	if(sw_past)
	{
		current = current_past + HOL*(epos - R*current_past - eneg);
	}
	else
	{
		current = 0; //force de-energizing of inductor to zero when switch open
	}

	current_past = current;
	sw_past = sw;

	*bout = -current;
}

NumType RLSwitch::measureThroughCurrent(NumType* current)
{
	*current = current_past;
	return current_past;
}

int RLSwitch::stampConductance(NumType* conduct_mat, unsigned int dim, unsigned int npos, unsigned int nneg)
{
	//do nothing since component has no conductance with Euler Forward

	return 0;
}

void RLSwitch::stampSources(std::vector<unsigned int>& sources, unsigned int npos, unsigned int nneg)
{
	sources.push_back(npos);
    sources.push_back(nneg);
}

int RLSwitch::stampSystem(NumType* conduct_mat, unsigned int dim, std::vector<unsigned int>& sources, unsigned int npos, unsigned int nneg)
{
	int ret = stampConductance(conduct_mat,dim,npos,nneg);

    if(ret)
    {
        return ret;
    }

    stampSources(sources,npos,nneg);

    return ret;
}

}
