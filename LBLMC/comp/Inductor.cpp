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

#include "Inductor.hpp"

namespace LBLMC
{

Inductor::Inductor(NumType dt, NumType ind)
: dt(dt), ind(ind), hol2(dt/NumType(2.0)/ind)
, epos_past(0.0), eneg_past(0.0),delta_v(0.0),delta_v_past(0.0)
, current(0.0),current_eq(0.0),current_past(0.0),current_eq_past(0.0)
{}

void Inductor::update(NumType epos, NumType eneg, NumType* bout)
{
	//#pragma HLS inline

	#pragma HLS latency min=0 max=0

	//register past values
	epos_past = epos;
	eneg_past = eneg;
	current_past = current;
	current_eq_past = current_eq;
	delta_v_past = delta_v;

	delta_v = AddSubType(epos_past) - AddSubType(eneg_past);

	current = hol2*delta_v - current_eq_past;

	current_eq = -current - hol2*delta_v;

	*bout = current_eq;

//	*bout = -(hol2*delta_v - current_eq_past) - hol2*delta_v;
}

void Inductor::measureThroughCurrent(NumType* current)
{
	*current = this->current;
}

void Inductor::measureAcrossVoltage(NumType epos, NumType eneg, NumType* voltage)
{
	*voltage = epos - eneg;
}

int Inductor::stampConductance(NumType* conduct_mat, unsigned int dim, unsigned int npos, unsigned int nneg)
{
	if( (dim < npos) || (dim < nneg) || (conduct_mat == 0) ) return -1;
	if( npos == nneg ) return 0; //component is shorted out

	if( npos != 0 && nneg != 0)
	{
		conduct_mat[dim*(npos-1) + (npos-1)] += hol2;
		conduct_mat[dim*(npos-1) + (nneg-1)] += -hol2;
		conduct_mat[dim*(nneg-1) + (npos-1)] += -hol2;
		conduct_mat[dim*(nneg-1) + (nneg-1)] += hol2;
	}
	else if (npos != 0)
		conduct_mat[dim*(npos-1)+ (npos-1)] += hol2;
	else if (nneg != 0)
		conduct_mat[dim*(nneg-1)+ (nneg-1)] += hol2;

	return 0;
}

void Inductor::stampSources(std::vector<unsigned int>& sources, unsigned int npos, unsigned int nneg)
{
    sources.push_back(npos);
    sources.push_back(nneg);
}

int Inductor::stampSystem(NumType* conduct_mat, unsigned int dim, std::vector<unsigned int>& sources, unsigned int npos, unsigned int nneg)
{
    int ret = stampConductance(conduct_mat,dim,npos,nneg);

    if(ret)
    {
        return ret;
    }

    stampSources(sources,npos,nneg);

    return ret;
}

} //namespace LBLMC
