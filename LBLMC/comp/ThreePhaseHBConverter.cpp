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

#include "ThreePhaseHBConverter.hpp"

namespace LBLMC
{

ThreePhaseHBConverter::ThreePhaseHBConverter(NumType dt, NumType cap, NumType ind, NumType res)
: dt(dt), cap(cap), ind(ind), res(res), hoc(dt/cap), hol(dt/ind), cap_conduct(10000.0)
  ,vc1(0.0),vc2(0.0),il1(0.0),il2(0.0),il3(0.0),ipos(0.0),ineg(0.0)
  ,epos_past(0.0),eneg_past(0.0),eout1_past(0.0),eout2_past(0.0),eout3_past(0.0)
  ,il1_past(0.0),il2_past(0.0),il3_past(0.0),vc1_past(0.0),vc2_past(0.0)
  ,sw1(false),sw2(false),sw3(false)
{

}

void ThreePhaseHBConverter::update(NumType epos, NumType eneg, NumType eout1, NumType eout2, NumType eout3,
		NumType* bpos, NumType* bneg, NumType* bout1, NumType* bout2, NumType* bout3,
		bool sw_ctrl1, bool sw_ctrl2, bool sw_ctrl3, bool sw_en)
{
	//#pragma HLS inline

	#pragma HLS latency min=0 max=0


	epos_past = epos;
	eneg_past = eneg;
	eout1_past = eout1;
	eout2_past = eout2;
	eout3_past = eout3;
	il1_past = il1;
	il2_past = il2;
	il3_past = il3;
	vc1_past = vc1;
	vc2_past = vc2;
	sw1 = sw_ctrl1;
	sw2 = sw_ctrl2;
	sw3 = sw_ctrl3;

		//a, b, c are for inductors, a#, b# are for caps
	NumType a1, a2, a3, b1, b2, b3, a, b, c;

	if(sw_en) //switches are enabled
	{
		if(sw1)
		{
			a1 = il1_past;
			b1 = 0.0;
			a = vc1_past;
		}
		else
		{
			a1 = 0.0;
			b1 = il1_past;
			a = vc2_past;
		}

		if(sw2)
		{
			a2 = il2_past;
			b2 = 0.0;
			b = vc1_past;
		}
		else
		{
			a2 = 0.0;
			b2 = il2_past;
			b = vc2_past;
		}

		if(sw3)
		{
			a3 = il3_past;
			b3 = 0.0;
			c = vc1_past;
		}
		else
		{
			a3 = 0.0;
			b3 = il3_past;
			c = vc2_past;
		}
	}
	else	//both switches are to be off
	{
		// assume there are anti-parallel diodes on switches

		if(il1_past > NumType(0.0))
		{
			a = vc2_past;
			a1 = 0.0;
			b1 = -il1_past;
		}
		else
		{
			if(il1_past < NumType(0.0))
				a = vc1_past;
			else
				a = eout1_past;
			a1 = il1_past;
			b1 = 0.0;
		}

		if(il2_past > NumType(0.0))
		{
			b = vc2_past;
			a2 = 0.0;
			b2 = -il2_past;
		}
		else
		{
			if(il2_past < NumType(0.0))
				b = vc1_past;
			else
				b = eout2_past;
			a2 = il2_past;
			b2 = 0.0;
		}

		if(il3_past > NumType(0.0))
		{
			c = vc2_past;
			a3 = 0.0;
			b3 = -il3_past;
		}
		else
		{
			if(il3_past < NumType(0.0))
				c = vc1_past;
			else
				c = eout3_past;
			a3 = il3_past;
			b3 = 0.0;
		}


	}

	ipos = cap_conduct*(NumType(epos_past) - NumType(vc1_past));
	ineg = cap_conduct*(NumType(eneg_past) - NumType(vc2_past));

	il1 = NumType(il1_past) + hol*( a - NumType(eout1_past) - res*(il1_past));
	il2 = NumType(il2_past) + hol*( b - NumType(eout2_past) - res*(il2_past));
	il3 = NumType(il3_past) + hol*( c - NumType(eout3_past) - res*(il3_past));

	vc1 = hoc*(NumType(ipos) - a1 - a2 - a3) + NumType(vc1_past);
	vc2 = hoc*(NumType(ineg) - b1 - b2 - b3) + NumType(vc2_past);

	*bpos = (vc1)*cap_conduct;
	*bneg = (vc2)*cap_conduct;
	*bout1 = il1;
	*bout2 = il2;
	*bout3 = il3;

};

void ThreePhaseHBConverter::measureInductorCurrents(NumType* current1, NumType* current2, NumType* current3)
{
	*current1 = il1;
	*current2 = il2;
	*current3 = il3;
}

void ThreePhaseHBConverter::measureCapacitorVoltages(NumType* voltage_p, NumType* voltage_n)
{
	*voltage_p = vc1;
	*voltage_n = vc2;
}

int ThreePhaseHBConverter::stampConductance(NumType* conduct_mat, unsigned int dim,
		unsigned int np, unsigned int nn, unsigned int na, unsigned int nb, unsigned int nc)
{
	if( (conduct_mat == 0) || (dim < np) || (dim < nn) || (dim < na) || (dim < nb) || (dim < nc)) return -1;
	if( (np==nn)&&(np=na)&&(np==nb)&&(np==nc) ) return 0; //component is shorted out on all terminals

	if(np != 0) conduct_mat[dim*(np-1)+ (np-1)] += cap_conduct;
	if(nn != 0) conduct_mat[dim*(nn-1)+ (nn-1)] += cap_conduct;

	// no conductances for na,nb,nc, so nothing to do for them!

	return 0;
}

void ThreePhaseHBConverter::stampSources(std::vector<unsigned int>& sources,
                                         unsigned int np, unsigned int nn, unsigned int na,
                                         unsigned int nb, unsigned int nc)
{
    sources.push_back(np);
    sources.push_back(0);

    sources.push_back(nn);
    sources.push_back(0);

    sources.push_back(na);
    sources.push_back(0);

    sources.push_back(nb);
    sources.push_back(0);

    sources.push_back(nc);
    sources.push_back(0);
}

int ThreePhaseHBConverter::stampSystem(NumType* conduct_mat, unsigned int dim, std::vector<unsigned int>& sources,
                                       unsigned int np, unsigned int nn, unsigned int na,
                                        unsigned int nb, unsigned int nc)
{
    int ret = stampConductance(conduct_mat,dim,np,nn,na,nb,nc);

    if(ret)
    {
        return ret;
    }

    stampSources(sources,np,nn,na,nb,nc);

    return ret;
}

} //namespace LBLMC
