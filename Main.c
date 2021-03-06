/*
 * Main.c
 *
 *  Created on: 20 Jun 2020
 *      Author: mmartinez
 */

#include <stdio.h>
#include <stdlib.h>
#include "DriveComponent.h"

int main() {
  InputParams input;
  OutputParams output;
  while ((input = readInputParams()).value != EOF && input.value != 'q') {
  	system("CLS");
  	printInputParams(input);
  	output = processInputParams(input);
  	printOutputParams(output);
  }
  return 0;
}

void printInputParams(InputParams input) {
	printf("\nInput params:");
	printf("\nbtnBack: %d", input.btnBack);
	printf("\nbtnFwd: %d", input.btnFwd);
	printf("\njoystickPos: %d", input.joystickPos);
	printf("\ntemp: %d", input.temp);
}

void printOutputParams(OutputParams output) {
	printf("\nOutput params");
	printf("\nMotor enabled: %s", output.motorEnabled ? "true" : "false");
	printf("\nMotor throttle: %d", output.motorThrottle);
}

