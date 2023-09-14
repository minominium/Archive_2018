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

/** PID ����� ������ ����ü */
typedef struct _DOUBLEPID{
        double nowAngle;		        //!< ���� ��
        double pastAngle;		        //!< ���� ��
	double targetAngle;
        
        double nowAngularS;                //!< ���ӵ�
        double pastAngularS;               //!< ���� ���ӵ�
        
        double angleError;                         //!< ���� ������
        double pastAngleError;
  
        double nowASError;		        //!< ���� ������
        double pastASError;	        	//!< ���� ������
        double target;                   	//!< ��ǥ��
	
        double errorSum;	        	//!< ���� ������

        double asErrorSum;
        double asErrorDiff;
        
        double errorSumLimit;            	//!< ���� ������ ���� (0�� ��� ���� ����)
        double errorDiff;		        //!< ���� �̺а�
        
        int nowOutput;                          //!< ���� ��°�
        int outputLimit;                       //!< ��� ���Ѱ�
	
        double underOfPoint;                //!< kP, kI, kD�� �������� ���� ������ (underOfPoint=1000, kP=1�̸� P�̵氪 = 0.001

        double outerP;                      //!< ���� PID ����� �ܺ� ���� P �̵氪

        double kP;			        //!< P(���)�̵氪
        double kI;			          //!< I(����)�̵氪
        double kD;			        //!< D(�̺�)�̵氪
}DPID;  


/* Part of PID Function */

/** PID ����ü �ʱ�ȭ �Լ� */
void PID_Control_Initialize(DPID* dst, double gain_P, double gain_I, double gain_D);

/** PID ����� �Լ� */
void PID_Control(DPID* dst, double target, double input, double GyroV);

#endif /* PID_CONTROL_LONG_H_ */
