#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <stdbool.h>
#include <sys/time.h>
#include "Dtos.h"
#include "States.h"

float holdTimeToActivate = 3000;
timeval timeInMilliseconds();
float timedifference_msec(timeval t0, timeval t1);
int getMotorThrottle(int joystickPos);

struct StateMachine {
	timeval buttonHoldStart;
	bool buttonsHeld;
	bool allowEnableDisable;
	SystemState systemState;
};

static OutputParams output = {
	false,
	0
};
static int MIN_TEMPERATURE = -1000;
static int MAX_TEMPERATURE = 1000;

OutputParams processInputParams(InputParams input) {
	static StateMachine stateMachine = {
		0,
		false,
		true,
		DISABLED
	};

	if (input.btnFwd && input.btnBack && !stateMachine.buttonsHeld) {
		stateMachine.buttonsHeld = true;
		gettimeofday(&stateMachine.buttonHoldStart, NULL);
	} else if (!input.btnFwd || !input.btnBack) {
		stateMachine.buttonsHeld = false;
		stateMachine.allowEnableDisable = true;
	}

	if (stateMachine.buttonsHeld && stateMachine.allowEnableDisable) {
		timeval now;
		gettimeofday(&now, NULL);
		float timeSinceHoldStart = timedifference_msec(stateMachine.buttonHoldStart, now);
		printf("\nTime since hold start: %4.0f", timeSinceHoldStart);
		if (timeSinceHoldStart >= holdTimeToActivate) {
			stateMachine.systemState = stateMachine.systemState == ENABLED ? DISABLED : ENABLED;
			stateMachine.buttonHoldStart = now;
			stateMachine.allowEnableDisable = false;
		}
	}

	if (input.temp > MAX_TEMPERATURE || input.temp < MIN_TEMPERATURE) {
		stateMachine.systemState = DISABLED;
	}

	switch (stateMachine.systemState) {
		case ENABLED:
			output.motorEnabled = true;
			output.motorThrottle = getMotorThrottle(input.joystickPos);
			break;
		case DISABLED:
			output.motorEnabled = false;
			output.motorThrottle = 0;
			break;
	}

	return output;
}

timeval timeInMilliseconds() {
	struct timeval start, stop;
	gettimeofday(&start, NULL);
	printf("%ld\n", start.tv_usec);
	return start;
}

float timedifference_msec(struct timeval t0, struct timeval t1) {
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

int getMotorThrottle(int joystickPos) {
	static float slope = 10.000f/9.000f;
	static float b = 1000.000f/9.000f;
	if (joystickPos < -100) {
		//y = (10/9)x + 1000/9
		return (slope * joystickPos) + b;
	} else if (joystickPos > 100) {
		//y = (10/9)x - 1000/9
		return (slope * joystickPos) - b;
	}
	return 0;
}
#endif

