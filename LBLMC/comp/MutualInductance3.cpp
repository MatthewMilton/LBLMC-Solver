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

#include "LBLMC/comp/MutualInductance3.hpp"

namespace LBLMC
{

MutualInductance3::MutualInductance3(NumType dt, NumType L1, NumType L2, NumType L3,
	NumType M12,NumType M23,NumType M31) :
    dt(dt), L1(L1), L2(L2), L3(L3), M12(M12), M23(M23), M31(M31),
    d( dt / (L3*M12*M12 - NumType(2)*M12*M23*M31 + L1*M23*M23 + L2*M31*M31 - L1*L2*L3 ) ),
    k1(M23*M23 - L2*L3  ),
    k2(L3*M12  - M23*M31),
    k3(L2*M31  - M12*M23),
    k4(L3*M12  - M23*M31),
    k5(M31*M31 - L1*L3  ),
    k6(L1*M23  - M12*M31),
    k7(L2*M31  - M12*M23),
    k8(L1*M23  - M12*M31),
    k9(M12*M12 - L1*L2  ),
    voltage1(0), voltage2(0), voltage3(0),
    current1(0), current2(0), current3(0),
    current_comp1(0), current_comp2(0), current_comp3(0)
{
    //do nothing else
}

//	MutualInductance3::MutualInductance3(NumType dt, NumType L[9])
//	{
//
//	}

void MutualInductance3::update(NumType epos1, NumType eneg1, NumType epos2, NumType eneg2, NumType epos3, NumType eneg3,
	NumType* bout1, NumType* bout2, NumType* bout3)
{
     voltage1 = epos1 - eneg1;
     voltage2 = epos2 - eneg2;
     voltage3 = epos3 - eneg3;

     current_comp1 = current_comp1 - d*( k1*voltage1 + k2*voltage2 + k3*voltage3 );
     current_comp2 = current_comp2 - d*( k4*voltage1 + k5*voltage2 + k6*voltage3 );
     current_comp3 = current_comp3 - d*( k7*voltage1 + k8*voltage2 + k9*voltage3 );

     *bout1 = current_comp1;
     *bout2 = current_comp2;
     *bout3 = current_comp3;
}

int MutualInductance3::stampConductance(NumType* conduct_mat, unsigned int dim,
	unsigned int npos1, unsigned int nneg1, unsigned int npos2, unsigned int nneg2, unsigned int npos3, unsigned int nneg3)
{
    //do nothing as there is no conductance to stamp (Euler Forward)

    return 0;
}

void MutualInductance3::stampSources(std::vector<unsigned int>& sources,
        unsigned int npos1, unsigned int nneg1, unsigned int npos2, unsigned int nneg2, unsigned int npos3, unsigned int nneg3)
{
    sources.push_back(npos1);
    sources.push_back(nneg1);

    sources.push_back(npos2);
    sources.push_back(nneg2);

    sources.push_back(npos3);
    sources.push_back(nneg3);
}

int MutualInductance3::stampSystem(NumType* conduct_mat, unsigned int dim, std::vector<unsigned int>& sources,
    unsigned int npos1, unsigned int nneg1, unsigned int npos2, unsigned int nneg2, unsigned int npos3, unsigned int nneg3)
{
    int ret = stampConductance(conduct_mat,dim,npos1,nneg1,npos2,nneg2,npos3,nneg3);

    if(ret)
    {
        return ret;
    }

    stampSources(sources,npos1,nneg1,npos2,nneg2,npos3,nneg3);

    return ret;
}

} //namespace LBLMC
