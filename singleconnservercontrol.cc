#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include "servercommon.h"
#include "singleconnservercontrol.h"

using namespace std;

//const int BUFF_SIZE = 2048;
//bool VERBOSE;
//bool shutdownFlag = false;

/*
 * Constructor
 */
SingleConnServerControl::SingleConnServerControl(int sock, set<pthread_t> *webThreads):
	sock(sock), webThreads(webThreads){

}

/*
 * Destructor
 */
SingleConnServerControl::~SingleConnServerControl() {

}

/*
 * Send a message to the client over provided socket.
 */
int SingleConnServerControl::sendMsg(string msg) {
	char *m = (char *)msg.c_str();
	int i = write(sock, m, strlen(m));
	if (shutdownFlag || i < 0)
		die("Shutdown flag or write fail", sock);
	// if (VERBOSE)
	// 	fprintf(stderr, "[%d][CTRL] S: %s\n", sock, m);
	return 1;
}

/*
 * Main
 */
void SingleConnServerControl::backbone() {
	if (VERBOSE)
		fprintf(stderr, "[%d][CTRL] S: +OK server ready [localhost]\r\n", sock);

	while (true) {
		char c_requestLine[BUFF_SIZE];
		memset(c_requestLine, 0, sizeof(c_requestLine));

		if (shutdownFlag)
			die("shutdown");

		int i = read(sock, c_requestLine, sizeof(c_requestLine));
		//read() error
		if (i < -1)
			break;
		//client closed connection
		if (i == 0)
			break;

		if (VERBOSE)
			fprintf(stderr, "[%d][CTRL] C: {%s}\n", sock, c_requestLine);

		//from strtok single character delimiter, modify in-place, char * hell to string paradise
		string requestLine = c_requestLine;

		//parse requestLine
		string req;
		string reqBody;
		unsigned int i_space = requestLine.find(" ");
		if (i_space == std::string::npos) {
			req = requestLine.substr(0);
			reqBody = "";
		}
		else {
			req = requestLine.substr(0,i_space);
			reqBody = requestLine.substr(i_space+1);
		}

		//deal with different commands
		if (req.compare("GETLOAD") == 0) {
			int load = webThreads->size();
			string msg = "threads=" + to_string(load);
			sendMsg(msg);
		}
		else if (req.compare("DISABLE") == 0) {
			kill(getpid(), SIGTERM);
		}

	}
}
