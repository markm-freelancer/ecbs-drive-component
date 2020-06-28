#include <conio.h>
#include "DriveComponent.h"

InputParams readInputParams() {
	static InputParams input = { 
	  0,
	  0,
	  0,
	  0,
	  0
	};
	if (!kbhit()) {
		return input;
	}
	int key = getch();
	switch (key) {
	case 72://up arrow
		input.joystickPos += 50;
		if (input.joystickPos > 1000) {
			input.joystickPos = 1000;
		}
		break;
	case 80://down arrow
		input.joystickPos -= 50;
		if (input.joystickPos < -1000) {
			input.joystickPos = -1000;
		}
		break;
	case 75: //left arrow
		input.temp -= 100;
		break;
	case 77: //right arrow
		input.temp += 100;
		break;
	case 119: //w key
		input.btnFwd = !input.btnFwd;
		break;
	case 101: //e key
		input.btnBack = !input.btnBack;
		break;
	}
	input.value = key;
	return input;
}

