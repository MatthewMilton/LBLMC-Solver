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

#include "MutualInductance2.hpp"

#include <cassert>

namespace LBLMC
{

MutualInductance2::MutualInductance2(NumType dt, NumType L1, NumType L2, NumType M) :
    dt(dt), L1(L1), L2(L2), M(M),
    k1( L2/(L1*L2-M*M) ), k2( -M/(L1*L2-M*M) ), k3( L1/(L1*L2-M*M) ),
    voltage1(0), voltage2(0),
    current1(0), current2(0), current_comp1(0), current_comp2(0)
{
    assert( ( (L1*L2 - M*M) != 0 )&&"MutualInductance2 inductance matrix parameters must be invertible; L1*L2-M^2 != 0" );
}

void MutualInductance2::update(NumType epos1, NumType eneg1, NumType epos2, NumType eneg2, NumType* bout1, NumType* bout2)
{
    voltage1 = epos1 - eneg1;
    voltage2 = epos2 - eneg2;

    current_comp1 = ( current_comp1 - dt*k1*voltage1 - dt*k2*voltage2 );
    current_comp2 = ( current_comp2 - dt*k2*voltage1 - dt*k3*voltage2 );

    *bout1 = current_comp1;
    *bout2 = current_comp2;
}

int MutualInductance2::stampConductance(NumType* conduct_mat, unsigned int dim, unsigned int npos1, unsigned int nneg1, unsigned int npos2, unsigned int nneg2)
{
    //do nothing as there is no conductance to stamp (Euler Forward)

    return 0;
}

} //namespace LBLMC
