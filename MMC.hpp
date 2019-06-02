
//*======================================================================================================================================
//*											FIVE LEVELS MODULAR MULTILEVEL CONVERTER
//*======================================================================================================================================		
//*		
//* 
//*   
//* Copyright (C) 2016 Michele Difronzo
//*
//*  difronzomichele@gmail.com
//*/

#ifndef MMC_HPP
#define MMC_HPP

class MMC
{
private:
    const  NumType dt,      // Simulation timestep
	               R,       // DC bus upper and lower multivalve resistence
	               init,    
	               L,       // Upper and lower multivalve inductance	               
	               C,       // SM capacitance
                   N,       // Number of switching module per multivalve (2 multivalve x N = 8 switching modules			       
				   dtoC,    // dt/C
			       dtoL;    // dt/L
          NumType
				   Rarm,    //Arm resistance 
				   Rpre,    //Precharger resistance
				   a,
                   mula[2*6],mulb[2*6],mulc[2*6],
				   ia,ib,ic,
			       Vca[2*6],Vcb[2*6],Vcc[2*6],			       			   
				   upa,upb,upc,lowa,lowb,lowc,
				   Ilupapast, Ilupbpast, Ilupcpast,Illowapast, Illowbpast, Illowcpast,
			       Ilupa, Ilupb, Ilupc,Illowa, Illowb, Illowc,
				   Ic_upa, Ic_upb, Ic_upc,Ic_lowa, Ic_lowb, Ic_lowc,
				   tsim;

public:	   
        MMC(NumType dt, NumType R, NumType L, NumType C, NumType init);
		
		void update(bool swp, bool Sa[2*6], bool Sb[2*6],bool Sc[2*6],
			            NumType Vup, NumType Vlow, NumType Vouta, NumType Voutb, NumType Voutc,
					    NumType Vcaout[2*6], NumType Vcbout[2*6], NumType Vccout[2*6],
                        NumType* Ilupaout, NumType* Illowaout, NumType* Ilupbout, NumType* Illowbout, NumType* Ilupcout, NumType* Illowcout,
					    NumType* bpos, NumType* bneg, NumType* bout1, NumType* bout2, NumType* bout3,
			            NumType* itot, NumType* icirca, NumType* icircb, NumType* icircc, NumType *Varmup_a, NumType *Varmlow_a);
};

#endif
