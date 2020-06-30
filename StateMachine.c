#define SLOPE 10.0000f/9.0000f
#define B 1000.0000f/9.0000f
#define ACTIVE_BACK -100
#define ACTIVE_FWD 100.0
#define MS_PER_INVOCATION 100

#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "DriveComponent.h"

struct StateMachine {
	int holdToStartDuration;
	bool buttonsHeld;
	bool allowEnableDisable;
	int cruiseThrottle;
	int holdToCruiseDuration;
	bool allowDisableCruiseByJoystick;
	bool allowDisableCruiseByBtn;
	int lastButtonConfig[2];
	struct Configuration config;
	SystemState systemState;
};

void processStartStop(InputParams input, StateMachine &stateMachine);
void calculateMotorThrottle(InputParams input, StateMachine &stateMachine);
int getMotorThrottle(int joystickPos);
void processTemperatureParams(InputParams input, StateMachine &stateMachine);

static OutputParams output = {
	false,
	0
};

OutputParams processInputParams(InputParams input) {
	static StateMachine stateMachine = {
		0,
		false,
		true,
		0,
		0,
		true,
		true,
		{0, 0},
		NULL,
		DISABLED,
	};

	stateMachine.config = readConfiguration();

	//start / stop check
	processStartStop(input, stateMachine);

	//calculate motor throttle & machine cruise input
	calculateMotorThrottle(input, stateMachine);

	//min/max temperature check
	processTemperatureParams(input, stateMachine);

	switch (stateMachine.systemState) {
		case DISABLED:
			output.motorEnabled = false;
			output.motorThrottle = 0;
			printf("\nSystem state = DISABLED");
			break;
		case ENABLED:
			output.motorEnabled = true;
			output.motorThrottle = getMotorThrottle(input.joystickPos);
			printf("\nSystem state = ENABLED");
			break;
		case CRUISE:
			output.motorEnabled = true;
			output.motorThrottle = stateMachine.cruiseThrottle;
			printf("\nSystem state = CRUISE");
			break;
	}
	return output;
}

void processStartStop(InputParams input, StateMachine &stateMachine) {
	switch (stateMachine.systemState) {
		case ENABLED:
		case DISABLED:
			if (input.btnFwd && input.btnBack && !stateMachine.buttonsHeld) {
				stateMachine.buttonsHeld = true;
			} else if (!input.btnFwd || !input.btnBack) {
				stateMachine.buttonsHeld = false;
				stateMachine.allowEnableDisable = true;
				stateMachine.holdToStartDuration = 0;
			}
		
			if (stateMachine.buttonsHeld && stateMachine.allowEnableDisable) {
				stateMachine.holdToStartDuration += MS_PER_INVOCATION;
				if (stateMachine.holdToStartDuration >= stateMachine.config.holdToStartTime) {
					stateMachine.systemState = stateMachine.systemState == ENABLED ? DISABLED : ENABLED;
					stateMachine.holdToStartDuration = 0;
					stateMachine.allowEnableDisable = false;
				}
			}
			break;
		case CRUISE:
			//do nothing
			break;
	}


	printf("\nHold to start duration: %d", stateMachine.holdToStartDuration);
}

void calculateMotorThrottle(InputParams input, StateMachine &stateMachine) {
	int motorThrottle = getMotorThrottle(input.joystickPos);
	bool outsideHoldzoneNegative, outsideHoldzonePositive;

	printf("\nHold to cruize joystick threshold: %d", stateMachine.config.holdZoneForJoystick);
	printf("\nHold to cruise duration: %d", stateMachine.holdToCruiseDuration);
	printf("\nHold to cruise time: %d", stateMachine.config.holdToCruiseTime);

	//check if cruise should be activated
	switch (stateMachine.systemState) {
		case ENABLED:
			if ((motorThrottle > stateMachine.config.holdZoneForJoystick && input.btnBack) ||
			    (motorThrottle < -stateMachine.config.holdZoneForJoystick && input.btnFwd)) {
				stateMachine.holdToCruiseDuration += MS_PER_INVOCATION;
				if (stateMachine.holdToCruiseDuration >= stateMachine.config.holdToCruiseTime) {
					stateMachine.systemState = CRUISE;
					stateMachine.cruiseThrottle = motorThrottle;
					stateMachine.holdToCruiseDuration = 0;
					stateMachine.allowDisableCruiseByJoystick = false;
					stateMachine.lastButtonConfig[0] = input.btnFwd;
					stateMachine.lastButtonConfig[1] = input.btnBack;
				}
			} else {
				stateMachine.holdToCruiseDuration = 0;
			}
			break;
		case CRUISE:
			outsideHoldzoneNegative = input.joystickPos <= 0 && input.joystickPos > -stateMachine.config.holdZoneForJoystick;
			outsideHoldzonePositive = input.joystickPos >= 0 && input.joystickPos < stateMachine.config.holdZoneForJoystick;
			if (outsideHoldzoneNegative || outsideHoldzonePositive) {
				stateMachine.allowDisableCruiseByJoystick = true;
			} else if (stateMachine.allowDisableCruiseByJoystick && !outsideHoldzoneNegative && !outsideHoldzonePositive) {
				stateMachine.systemState = ENABLED;
			}
			if (input.btnFwd != stateMachine.lastButtonConfig[0] || input.btnBack != stateMachine.lastButtonConfig[1]) {
				stateMachine.systemState = ENABLED;
			}
			break;
	}
}

int getMotorThrottle(int joystickPos) {
	if (joystickPos < ACTIVE_BACK) {
		//y = (10/9)x + 1000/9
		return round((SLOPE * joystickPos) + B);
	} else if (joystickPos > ACTIVE_FWD) {
		//y = (10/9)x - 1000/9
		return round((SLOPE * joystickPos) - B);
	}
	return 0;
}

void processTemperatureParams(InputParams input, StateMachine &stateMachine) {
	if (input.temp < stateMachine.config.minTemp || input.temp > stateMachine.config.maxTemp) {
		stateMachine.systemState = DISABLED;
	}
}

