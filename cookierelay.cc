#include <cstring>
#include <iostream>

#include "servercommon.h"
#include "cookierelay.h"

/*
 * Constructor
 */
CookieRelay::CookieRelay(int sock): sock(sock){
	//SETUP LOADBALANCER SOCK HERE
	latestCookie = -1;
	mutex_sock = PTHREAD_MUTEX_INITIALIZER;
}

/*
 * Nothing to see here
 */
CookieRelay::~CookieRelay() {
//	                               .....
//	                            ...........
//	                           .. ..........    ........
//	                        ,,.... @@@@@%...,*.*@@@@@.....
//	                   *,/#%%#,....@@@@@@..,/..(@@@@.......
//	               /,/((#%#%&,.....%(..,,,(,.............
//	            ,(/#(((((%####(###((/,,,*(%##,,............/
//	           *##%/#(###%#(((#((%%###%&%%#%%#%*...,......(((///
//	           %%%%#%#%%%%###%#%%%(%%##%######&(#((/*%#(%###//#/.
//	         ,*#&%((%##%%%#%##%%#(%(#(%#%###%#%%%##%%##(#(#*(#(#*/.
//	          *(%%&&(%#%#%%##%&%(####%%%&&%#%#%###((%##((##((/,
//	         *#/%&&@@@@@@@%%%%%%%%%%%#%%%#%%%%%#(%%%%#(#&%%%%%%((*/**
//	        ((#%%%&@@@@@@@@@@@@@@@@@@@@&%%%&%&%###%&%###&%%%%%##((#//*
//	        /##%%&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%&%%%%%%%&&(%%###///,
//	        .#%%&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&&%%%%%%%%#(%##*,
//	      .*(#%%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&%#%#/
//	       ,/##%%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&%#(#*
//	        ,##%%%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%,(
//	        ,(##%%&%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%(.
//	        ..(#%%%%%%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%#/*
//	        #%%(%%%%%#%%&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%&%%%###,,
//	       /%%%&%#%(&%%%%&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%&%%%&&%#(/
//	      .(%%%#%#&&&%&&&&%%&&@@@@@@@@@@&@@@@@@&%%&&&%%%%%(##*
//	     *(%%%%&&&%##%%&%&&%#%&&&%%&%&&&%%&%%&&%%&%%&&%%%%#%&(////*****//
//	   *(,%%&&&%&*#%%%%%&&&%&%%%%%%%#%&%%#%#(%%&&%%&&%%%(***,**/*#%%#(*,
//	   ,#%%&&%&&&&&&&&%&%%#%%&&&%%&@%%%%%%%&%%&%%%&&%&&(#&%%%%(***/*/*,****/**,
//	.*#(%%&&&%&&&&&&&&&&&&&%%&%%%%&%%%%&&%%&&%%%(#&%&&%%%####,/**//*,*/*//*,*,,
//	(%%%&&&&&%&&&&&&&&&&&&%&&%&%&@&&&&&&&%%%%%&%&&&%%%%%%*****%*&******/**/*,,
//	(%&%&&&%%&&&@&&&&&&&@&&&&&&&&&&&&&&%&&%&&&&&&&&&&&&&&/,**///%%%(//,((/******.
//	%&&&%&&&&&%&&&@&&&@&&&&%%@&&&&%#&&%#%&&&%%&%&&&&&&***,**/*,**,(//**#(#((//(**,,
//	#&%&&&%%&&&&&%&%&&%%%%&&&&&@&%&&&&%%%&%&&&&&&%*(**//(/((//#((((#&%###%##/*,
}

/*
 * Send command to cookie store (load balancer)
 */
string CookieRelay::sendCommand(string command) {
	char *c_command = (char *)command.c_str();
	char buff[BUFF_SIZE];
	memset(buff, 0, sizeof(buff));
	write(sock, c_command, command.length());
	if (VERBOSE)
		fprintf(stderr, "[%d][COOK] S: %s\n", sock, c_command);
	int i = read(sock, buff, BUFF_SIZE - 1);
	buff[i] = 0;
	if (VERBOSE)
		fprintf(stderr, "[%d][COOK] LB: %s\n", sock, buff);
	string result = buff;
	return result;
}

/*
 * Generate a cookie for a new browser
 */
int CookieRelay::genCookie(string browser) {
	string command = "GEN " + browser;
	return stoi(sendCommand(command));
}

/*
 * Delete cookie (logout)
 */
void CookieRelay::delCookie(int cookie) {
	string command = "DEL " + to_string(cookie);
	sendCommand(command);
}

/*
 * Fetch a browser using an existing cookie
 */
string CookieRelay::fetchBrowser(int cookie) {
	string command = "FETCH " + to_string(cookie);
	return sendCommand(command);
}
