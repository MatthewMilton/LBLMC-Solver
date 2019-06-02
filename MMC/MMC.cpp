
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

 
 

#include "MMC.hpp"                    
#include "LBLMCParams.hpp"
#include <iostream>

MMC::MMC(NumType dt, NumType R, NumType L, NumType C, NumType init):      //constructor's initializer list 
	     dt(dt), R(R), init(init), L(L), C(C), N(6), dtoC(dt/C), dtoL(dt/L),
	     Rarm(0.1), Rpre(220.0),  
	     a(0),
	     mula(),mulb(), mulc(),
	     ia(0), ib(0), ic(0),
	     Vca(), Vcb(), Vcc(),
	     upa(0),upb(0),upc(0),lowa(0),lowb(0),lowc(0),
	     Ilupapast(0), Ilupbpast(0), Ilupcpast(0),Illowapast(0), Illowbpast(0), Illowcpast(0),  
	     Ilupa(0), Ilupb(0), Ilupc(0),Illowa(0), Illowb(0), Illowc(0),
	     Ic_upa(0), Ic_upb(0), Ic_upc(0),Ic_lowa(0), Ic_lowb(0), Ic_lowc(0),
	     tsim(0)
	   
		   
		{  
			for(int i = 0; i < 6; i++)
			{
				#pragma HLS UNROLL
				Vca[i] = init;
				Vca[i+6] = -init;
				Vcb[i] = init;
				Vcb[i+6] = -init;
				Vcc[i] = init;
				Vcc[i+6] = -init;
			}
		} 

   
void MMC::update(bool swp, bool Sa[2*6], bool Sb[2*6],bool Sc[2*6],
				 NumType Vup, NumType Vlow, NumType Vouta, NumType Voutb, NumType Voutc,
				 NumType Vcaout[2*6], NumType Vcbout[2*6], NumType Vccout[2*6],
				 NumType* Ilupaout, NumType* Illowaout, NumType* Ilupbout, NumType* Illowbout, NumType* Ilupcout, NumType* Illowcout,
				 NumType* bpos, NumType* bneg, NumType* bout1, NumType* bout2, NumType* bout3,
				 NumType* itot, NumType* icirca, NumType* icircb, NumType* icircc, NumType *Varmup_a, NumType *Varmlow_a)	
				 
				{
				
					#pragma HLS inline       
					#pragma HLS latency min=0 max=0   
				
					#pragma HLS ARRAY_PARTITION variable = mula    dim=1  
					#pragma HLS ARRAY_PARTITION variable = mulb    dim=1
					#pragma HLS ARRAY_PARTITION variable = mulc    dim=1
					#pragma HLS ARRAY_PARTITION variable = Vca     dim=1
					#pragma HLS ARRAY_PARTITION variable = Vcb     dim=1
					#pragma HLS ARRAY_PARTITION variable = Vcc     dim=1
					#pragma HLS ARRAY_PARTITION variable = Vcaout  dim=1
					#pragma HLS ARRAY_PARTITION variable = Vcbout  dim=1
					#pragma HLS ARRAY_PARTITION variable = Vccout  dim=1
					#pragma HLS ARRAY_PARTITION variable = Sa      dim=1
					#pragma HLS ARRAY_PARTITION variable = Sb      dim=1
					#pragma HLS ARRAY_PARTITION variable = Sc	   dim=1
					
					const static NumType Lodt = 1/dtoL;    
					const static NumType invRfC = 1/NumType(5560.0*MMC_CAP);    
					
					
					for(int i = 0; i < 2*6; i++)
					{
						#pragma HLS UNROLL
						

						if(Sa[i])  
						{
						if(i<6)	        
							a = Ilupapast;
				
						else if(i>=6)
						{
							a = Illowapast;	
				
						}			 
						Vca[i] = Vca[i] + dtoC*(a);		 
						}			
						
						else 
							{
								a = 0.0;
								Vca[i] = Vca[i] *(1 - dt*invRfC);  
								}                                     
				
					
*/
						if(Sb[i])
						{
						if(i<6)	         
							a = Ilupbpast;
							
						else  if(i>=6)
						{
							a = Illowbpast;
						}	
						Vcb[i] = Vcb[i] + dtoC*(a);			  
						}			
						
						else
							{
								a = 0.0;
								Vcb[i] = Vcb[i] *(1 - dt*invRfC);
								}   

								
						if(Sc[i])
						{
						if(i<6)	         
							a = Ilupcpast;
				
						else  if(i>=6)
						{
							a = Illowcpast;
						}
						Vcc[i] = Vcc[i] + dtoC*(a);
						}			          
						else
							{
							a = 0.0;
							Vcc[i] = Vcc[i] *(1 - dt*invRfC);
							}   
						
						
						if(Sa[i])
						mula[i] = Vca[i];
						else
						mula[i] = 0;
						
						if(Sb[i])
						mulb[i] = Vcb[i];
						else
						mulb[i] = 0;
					
						if(Sc[i])
						mulc[i] = Vcc[i];
						else
						mulc[i] = 0;
				

						Vcaout[i] = Vca[i];
						Vcbout[i] = Vcb[i];
						Vccout[i] = Vcc[i];
						
					} 

				
					upa = mula[0]+mula[1]+mula[2]+mula[3]+mula[4]+mula[5];
					upb = mulb[0]+mulb[1]+mulb[2]+mulb[3]+mulb[4]+mulb[5];
					upc = mulc[0]+mulc[1]+mulc[2]+mulc[3]+mulc[4]+mulc[5];
			
					lowa = mula[6]+mula[7]+mula[8]+mula[9]+mula[10]+mula[11];
					lowb = mulb[6]+mulb[7]+mulb[8]+mulb[9]+mulb[10]+mulb[11];
					lowc = mulc[6]+mulc[7]+mulc[8]+mulc[9]+mulc[10]+mulc[11];	
				
					
				
				

				
					Ilupa = Ilupapast + dtoL * (Vup -upa - Rarm * Ilupapast   - Vouta );
							
					Illowa = Illowapast + dtoL * (Vlow - lowa - Rarm* Illowapast  - Vouta );
					
					//****************************************************************	
					Ilupb = Ilupbpast + dtoL * (Vup - upb - Rarm * Ilupbpast  - Voutb);
							
					Illowb = Illowbpast + dtoL  * (Vlow -lowb - Rarm * Illowbpast   - Voutb );
					
					//****************************************************************
					Ilupc = Ilupcpast + dtoL  * (Vup - upc - Rarm * Ilupcpast - Voutc);
							
					Illowc = Illowcpast + dtoL * (Vlow - lowc - Rarm *  Illowcpast - Voutc);		 
					
n:	
					upa  = 0; 
					upb  = 0;
					upc  = 0;    
					lowa = 0;
					lowb = 0;
					lowc = 0;
				
					
					Ilupapast = Ilupa;
					Ilupbpast = Ilupb;
					Ilupcpast = Ilupc;
					Illowapast = Illowa;
					Illowbpast = Illowb;
					Illowcpast = Illowc;				 
									
	
					ia = Ilupa + Illowa;
					ib = Ilupb + Illowb;
					ic = Ilupc + Illowc;
					
					*itot = 0;  
					
					//**************The Circulatings currents *****************************************
				
					
					*icirca = 0; 
					*icircb = 0; 
					*icircc = 0; 
						
					//*****sending the past currents in output to the Control  	
					*Ilupaout = Ilupa;
					*Ilupbout = Ilupb;
					*Ilupcout = Ilupc;
							
					*Illowaout = Illowa;
					*Illowbout = Illowb;
					*Illowcout = Illowc;     
					
					
					//*****OUTPUT CURRENTS FOR THE LB-LMC
					*bpos= Ilupa + Ilupb + Ilupc;
					*bneg = -(Illowa + Illowb + Illowc);
					*bout1 = Ilupa + Illowa ;
					*bout2 = Ilupb + Illowb ;
					*bout3 = Ilupc + Illowc ;

					tsim += dt;
									
				};
