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

#include "Resistor.hpp"

namespace LBLMC
{

Resistor::Resistor(NumType res) :
conductance(NumType(1.0)/res)
{}

int Resistor::stampConductance(NumType* conduct_mat, unsigned int dim, unsigned int npos, unsigned int nneg)
{
	if( (dim < npos) || (dim < nneg) || (conduct_mat == 0) ) return -1;
	if( npos == nneg ) return 0; //component is shorted out

	if( npos != 0 && nneg != 0)
	{
		conduct_mat[dim*(npos-1) + (npos-1)] += conductance;
		conduct_mat[dim*(npos-1) + (nneg-1)] += -conductance;
		conduct_mat[dim*(nneg-1) + (npos-1)] += -conductance;
		conduct_mat[dim*(nneg-1) + (nneg-1)] += conductance;
	}
	else if (npos != 0)
		conduct_mat[dim*(npos-1)+ (npos-1)] += conductance;
	else if (nneg != 0)
		conduct_mat[dim*(nneg-1)+ (nneg-1)] += conductance;

	return 0;
}

void Resistor::stampSources(std::vector<unsigned int>& sources, unsigned int npos, unsigned int nneg)
{
        //does nothing
}

int Resistor::stampSystem(NumType* conduct_mat, unsigned int dim, std::vector<unsigned int>& sources, unsigned int npos, unsigned int nneg)
{
    return stampConductance(conduct_mat,dim,npos,nneg);
}

} //namespace LBLMC
