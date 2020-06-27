/*
 * Dtos.h
 *
 *  Created on: 20 Jun 2020
 *      Author: mmartinez
 */

#ifndef DTOS_H_
#define DTOS_H_

struct InputParams {
	int btnFwd;
	int btnBack;
	int joystickPos;
	int temp;
	int value;
};

struct OutputParams {
	bool motorEnabled;
	int motorThrottle;
};

struct Configuration {
	int holdToStartTime;
	int holdZoneForJoystick;
	int holdToCruiseTime;
	int maxTemp;
	int minTemp;
};

#endif /* DTOS_H_ */
