/*
 * States.h
 *
 *  Created on: 20 Jun 2020
 *      Author: mmartinez
 */

#ifndef STATES_H_
#define STATES_H_

//Different states of the application
typedef enum
{
	DISABLED,
	ENABLED
} SystemState;

//Different states of the motor
typedef enum
{
    IDLE,
    FORWARD,
	REVERSE,
} MotorState;

#endif /* STATES_H_ */
