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

#include "TwoPhaseHBConverter.hpp"

namespace LBLMC
{

TwoPhaseHBConverter::TwoPhaseHBConverter(NumType dt, NumType cap, NumType ind, NumType res)
: dt(dt), cap(cap), ind(ind), res(res), hoc(dt/cap), hol(dt/ind), cap_conduct(10000.0)
  ,vc1(0.0),vc2(0.0),il1(0.0),il2(0.0),ipos(0.0),ineg(0.0)
  ,epos_past(0.0),eneg_past(0.0),eout1_past(0.0),eout2_past(0.0)
  ,il1_past(0.0),il2_past(0.0),vc1_past(0.0),vc2_past(0.0)
  ,sw1(false),sw2(false)
{}

void TwoPhaseHBConverter::update(NumType epos, NumType eneg, NumType eout1, NumType eout2,
		NumType* bpos, NumType* bneg, NumType* bout1, NumType* bout2,
		bool sw_ctrl1, bool sw_ctrl2)
{
	#pragma HLS latency min=0 max=0


	epos_past = epos;
	eneg_past = eneg;
	eout1_past = eout1;
	eout2_past = eout2;
	il1_past = il1;
	il2_past = il2;
	vc1_past = vc1;
	vc2_past = vc2;
	sw1 = sw_ctrl1;
	sw2 = sw_ctrl2;

	updateIPosNeg(epos, eneg);
	updateVC(sw1, sw2);
	updateIL(sw1, sw2, eout1, eout2);
	updateBElements(bpos, bneg, bout1, bout2);
};

int TwoPhaseHBConverter::stampConductance(NumType* conduct_mat, unsigned int dim,
		unsigned int np, unsigned int nn, unsigned int na, unsigned int nb)
{
	if( (conduct_mat == 0) || (dim < np) || (dim < nn) || (dim < na) || (dim < nb)) return -1;
	if( (np==nn)&&(np=na)&&(np==nb) ) return 0; //component is shorted out on all terminals

	if(np != 0) conduct_mat[dim*(np-1)+ (np-1)] += cap_conduct;
	if(nn != 0) conduct_mat[dim*(nn-1)+ (nn-1)] += cap_conduct;

	// no conductances for na,nb, so nothing to do for them!

	return 0;
}

void TwoPhaseHBConverter::updateVC(bool sw_ctrl1, bool sw_ctrl2)
{
	#pragma HLS INLINE
	//pragma HLS PIPELINE II=1
	//#pragma HLS latency min=0 max=0

//	AddSubType a, b;
	AddSubType a1, a2, b1, b2;

//	if(!sw_ctrl1) a = il1_past; else a= 0.0;
//	if(!sw_ctrl2) b = il2_past; else b = 0.0;
//
//	vc1 = hoc*(AddSubType(ipos) - a - b) + AddSubType(vc1_past);
//	vc2 = hoc*(AddSubType(ineg) - a - b) + AddSubType(vc2_past);

	if(sw_ctrl1)
	{
		a1 = il1_past;
		b1 = 0.0;
	}
	else
	{
		a1 = 0.0;
		b1 = il1_past;
	}

	if(sw_ctrl2)
	{
		a2 = il2_past;
		b2 = 0.0;
	}
	else
	{
		a2 = 0.0;
		b2 = il2_past;
	}

	vc1 = hoc*(AddSubType(ipos) - a1 - a2) + AddSubType(vc1_past);
	vc2 = hoc*(AddSubType(ineg) - b1 - b2) + AddSubType(vc2_past);

}

void TwoPhaseHBConverter::updateIPosNeg(NumType epos, NumType eneg)
{
	#pragma HLS INLINE
	//#pragma HLS PIPELINE II=1
	//#pragma HLS latency min=0 max=0

	ipos = cap_conduct*(AddSubType(epos_past) - AddSubType(vc1_past));
	ineg = cap_conduct*(AddSubType(eneg_past) - AddSubType(vc2_past));
}

void TwoPhaseHBConverter::updateIL(bool sw_ctrl1, bool sw_ctrl2, NumType eout1, NumType eout2)
{
	#pragma HLS INLINE
	//#pragma HLS PIPELINE II=1
	//#pragma HLS latency min=0 max=0

	AddSubType a, b;

	if(sw_ctrl1)
		a = vc1_past;
	else
		a = vc2_past;

	if(sw_ctrl2)
		b = vc1_past;
	else
		b = vc2_past;

	il1 = AddSubType(il1_past) + hol*( a - AddSubType(eout1_past) - res*(il1_past));
	il2 = AddSubType(il2_past) + hol*( b - AddSubType(eout2_past) - res*(il2_past));
}

void TwoPhaseHBConverter::updateBElements(NumType* bpos, NumType* bneg, NumType* bout1, NumType* bout2)
{
	#pragma HLS INLINE
	//#pragma HLS PIPELINE II=1
	//#pragma HLS latency min=0 max=0

	*bpos = (vc1)*cap_conduct;
	*bneg = (vc2)*cap_conduct;

	*bout1 = il1;
	*bout2 = il2;
}

} //namespace LBLMC
