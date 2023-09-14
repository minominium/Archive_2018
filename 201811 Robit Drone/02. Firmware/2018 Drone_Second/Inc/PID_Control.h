/*==============================================================================
 *
 *   	File Name   	: PID_Control.c
 *  	Version        	: 0.1
 *    	Date           	: Jan 25, 2017
 *	Modified       	: Jan 25, 2017 By LeeMinHo
 *	Author         	: LeeMinHo  (ROBIT 11th, Kwangwoon University)
 *	MPU_Type       	: STM32F446RE
 *	Compiler	: IAR Embedded Workbench IDE - ARM 8.20.1
 *	Copyright(c) 2017 ROBIT, Kwangwoon University.
 *    	All Rights Reserved.
 *
==============================================================================*/

/*!
** @file PID_Control.h
** @version 0.1
** @brief
**
*/         

#ifndef PID_CONTROL_H_
#define PID_CONTROL_H_


/* Part of PID Struct */

/** PID 제어기 데이터 구조체 */
typedef struct _DOUBLEPID{
        double nowAngle;		        //!< 현재 값
        double pastAngle;		        //!< 이전 값
	double targetAngle;
        
        double nowAngularS;                //!< 각속도
        double pastAngularS;               //!< 이전 각속도
        
        double angleError;                         //!< 각도 에러값
        double pastAngleError;
  
        double nowASError;		        //!< 현재 에러값
        double pastASError;	        	//!< 이전 에러값
        double target;                   	//!< 목표값
	
        double errorSum;	        	//!< 에러 누적값

        double asErrorSum;
        double asErrorDiff;
        
        double errorSumLimit;            	//!< 에러 누적값 제한 (0일 경우 제한 없음)
        double errorDiff;		        //!< 에러 미분값
        
        int nowOutput;                          //!< 현재 출력값
        int outputLimit;                       //!< 출력 제한값
	
        double underOfPoint;                //!< kP, kI, kD에 공통으로 들어가는 나눗셈 (underOfPoint=1000, kP=1이면 P이득값 = 0.001

        double outerP;                      //!< 이중 PID 제어용 외부 루프 P 이득값

        double kP;			        //!< P(비례)이득값
        double kI;			          //!< I(적분)이득값
        double kD;			        //!< D(미분)이득값
}DPID;  


/* Part of PID Function */

/** PID 구조체 초기화 함수 */
void PID_Control_Initialize(DPID* dst, double gain_P, double gain_I, double gain_D);

/** PID 제어기 함수 */
void PID_Control(DPID* dst, double target, double input, double GyroV);

#endif /* PID_CONTROL_LONG_H_ */
