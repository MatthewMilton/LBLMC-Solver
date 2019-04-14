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

#include "DCVoltageSource.hpp"

namespace LBLMC
{

DCVoltageSource::DCVoltageSource(NumType vs, NumType rs) :
	vs(vs), gs(NumType(1.0)/rs), bs(vs/rs)
{}

void DCVoltageSource::update(NumType* bout)
{
	//#pragma HLS inline
	#pragma HLS latency min=0 max=0

	*bout = bs;
}

int DCVoltageSource::stampConductance(NumType* conduct_mat, unsigned int dim, unsigned int npos, unsigned int nneg)
{
	if( (dim < npos) || (dim < nneg) || (conduct_mat == 0) ) return -1;
	if( npos == nneg ) return 0; //component is shorted out

	if( npos != 0 && nneg != 0)
	{
		conduct_mat[dim*(npos-1) + (npos-1)] += gs;
		conduct_mat[dim*(npos-1) + (nneg-1)] += -gs;
		conduct_mat[dim*(nneg-1) + (npos-1)] += -gs;
		conduct_mat[dim*(nneg-1) + (nneg-1)] += gs;
	}
	else if (npos != 0)
		conduct_mat[dim*(npos-1)+ (npos-1)] += gs;
	else if (nneg != 0)
		conduct_mat[dim*(nneg-1)+ (nneg-1)] += gs;

	return 0;
}

void DCVoltageSource::stampSources(std::vector<unsigned int>& sources, unsigned int npos, unsigned int nneg)
{
    sources.push_back(npos);
    sources.push_back(nneg);
}

int DCVoltageSource::stampSystem(NumType* conduct_mat, unsigned int dim, std::vector<unsigned int>& sources, unsigned int npos, unsigned int nneg)
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
