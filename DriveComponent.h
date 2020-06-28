/*
 * DriveComponent.h
 *
 *  Created on: 20 Jun 2020
 *      Author: mmartinez
 */

#ifndef DRIVECOMPONENT_H_
#define DRIVECOMPONENT_H_

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

//Different states of the application
typedef enum
{
	DISABLED,
	ENABLED,
	CRUISE
} SystemState;

Configuration readConfiguration();
InputParams readInputParams();
void printInputParams(InputParams input);
OutputParams processInputParams(InputParams input);
void printOutputParams(OutputParams output);

#endif /* DRIVECOMPONENT_H_ */
