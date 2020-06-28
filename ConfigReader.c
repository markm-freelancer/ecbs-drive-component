/*
 * ConfigReader.h
 *
 *  Created on: 20 Jun 2020
 *      Author: mmartinez
 */

#include <stdlib.h>
#include <stdio.h>
#include "DriveComponent.h"

Configuration readConfiguration() {
    FILE *fp;
	fp = fopen("drive.conf" , "r");
	if(fp == NULL) {
	  perror("Error opening file 'drive.conf'");
	  exit(1);
	}

	Configuration conf = {0};
	char line[10];
	int linenum = 0;
	int configData[4];
	while(fgets(line, 10, fp) != NULL) {
        configData[linenum] = atoi(line);
        linenum++;
	}

	conf.holdToStartTime = configData[0];
	conf.holdZoneForJoystick = configData[1];
	conf.holdToCruiseTime = configData[2];
	conf.minTemp = configData[3];
	conf.maxTemp = configData[4];
	fclose(fp);
	return conf;
}

