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

#include "TwoPortTransconductor.hpp"

namespace LBLMC
{

TwoPortTransconductor::TwoPortTransconductor(NumType transconductance12, NumType transconductance21) :
	transconductance12(transconductance12), transconductance21(transconductance21)
{

}

int TwoPortTransconductor::stampConductance(NumType* conduct_mat, unsigned int dim, unsigned int m, unsigned int n,
			unsigned int p, unsigned int q)
{
	if( (dim < m) || (dim < n) || (dim < p) || (dim < q) || (conduct_mat == 0) ) return -1;
	if( (m == n) && (m == p) && (m == q) ) return 0; //component ports are shorted out, so do nothing

	if( (m != 0) && (p != 0) )
	{
		conduct_mat[dim*(m-1) + (p-1)] += transconductance12;
		conduct_mat[dim*(p-1) + (m-1)] += transconductance21;
	}

	if( (m != 0) && (q != 0) )
	{
		conduct_mat[dim*(m-1) + (q-1)] -= transconductance12;
		conduct_mat[dim*(q-1) + (m-1)] -= transconductance21;
	}

	if( (n != 0) && (p != 0) )
	{
		conduct_mat[dim*(n-1) + (p-1)] -= transconductance12;
		conduct_mat[dim*(p-1) + (n-1)] -= transconductance21;
	}

	if( (n != 0) && (q != 0) )
	{
		conduct_mat[dim*(n-1) + (q-1)] += transconductance12;
		conduct_mat[dim*(q-1) + (n-1)] += transconductance21;
	}

	return 0;
}

} //namespace LBLMC
