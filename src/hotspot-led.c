#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include<wiringPi.h>

#define ledPin 0

const char *wlan0_status_cmd = "/usr/bin/nmcli -f GENERAL.STATE,GENERAL.CONNECTION device show wlan0";
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

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "no connection name provided.\n");
		return 1;
	}
	char *connectionName;
	connectionName = argv[1];

	wiringPiSetup();

	pinMode(ledPin, OUTPUT);

	if (wlan0_status(connectionName) == 0) {
		printf("wlan0 %s down\n", connectionName);
		digitalWrite(ledPin, LOW);
	} else {
		printf("wlan0 %s up\n", connectionName);
		digitalWrite(ledPin, HIGH);
	}
}
		
