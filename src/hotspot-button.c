#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include<wiringPi.h>

#define buttonPin 2

int wlan0State=0;
int buttonState=HIGH;
int lastButtonState=HIGH;
long lastChangeTime;
long captureTime=50;
int reading;

const char *wlan0_status_cmd = "/usr/bin/nmcli -f GENERAL.STATE,GENERAL.CONNECTION device show wlan0";
const char *wlan0_up_cmd = "/usr/bin/nmcli connection up";
const char *wlan0_down_cmd = "/usr/bin/nmcli device down wlan0";
const char *nmcli_state = "GENERAL.STATE"; 
const char *nmcli_state_connected = "connected"; 
const char *nmcli_connection = "GENERAL.CONNECTION"; 

int wlan0_status(char *name) {
	FILE *status;
  	if (NULL == (status = popen(wlan0_status_cmd, "r"))) {
    		perror("popen");
    		exit(EXIT_FAILURE);
  	}

	int res = 1;

	char *line = NULL;
	char *line2 = NULL;
    	size_t len = 0;
    	ssize_t read;

   	while ((read = getline(&line, &len, status)) != -1) {
		if (NULL != (line2 = strstr(line, nmcli_state))) {
			if (NULL != (line2 = strstr(line, nmcli_state_connected))) {
				res = 1;
        		} else {
				res = 0;
			}
		} else if (NULL != (line2 = strstr(line, nmcli_connection))) {
			if (NULL != (line2 = strstr(line, name))) {
				if (res == 1) {
					res = 1;
				}
			} else {
				res = 0;
			}
		}

   		free(line);
    	}

	pclose(status);
	return res;
}

void payloadUp(char *name) {
	int len;
	len = strlen(wlan0_up_cmd) + strlen(name) + 1;

	char *cmd = malloc(len);
	cmd = strcat(strcat(strcpy(cmd, wlan0_up_cmd), " "), name);
	printf("%s\n", cmd);

	system(cmd);
	free(cmd);

	return;
}

void payloadDown(void) {
	system(wlan0_down_cmd);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "no connection name provided.\n");
		return 1;
	}
	char *connectionName;
	connectionName = argv[1];

	wiringPiSetup();

	pinMode(buttonPin, INPUT);

	pullUpDnControl(buttonPin, PUD_UP);

	while(1) {
		reading = digitalRead(buttonPin);
		if (reading != lastButtonState) {
			lastChangeTime = millis();
			lastButtonState = reading;
		}

		if (millis() - lastChangeTime > captureTime) {
			lastChangeTime = millis();
			if (reading != buttonState) {
				buttonState = reading;

				if (buttonState == LOW) {
					wlan0State = wlan0_status(connectionName);

					if (wlan0State == 0) {
						printf("%s@wlan0 up\n", connectionName);
						payloadUp(connectionName);
					} else {
						printf("wlan0 down\n");
						payloadDown();
					}
				}
			}
		}
	}
}


