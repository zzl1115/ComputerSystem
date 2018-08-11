//This is a client file which guess the number
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

//Get the user input
static int getLine (char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

//Connect with server
int do_connect(char *host, char *port){
	int s;
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct addrinfo hints, *result;

	// Allows "global"
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET; /* IPv4 only */
	hints.ai_socktype = SOCK_STREAM; /* TCP */

	s = getaddrinfo(host, port, &hints, &result);

	// If I can't get the address, write an error. 
	if (s != 0) {
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
			exit(1);
	}

	// Try to connect; if I can't, write an error. 
	if(connect(sock_fd, result->ai_addr, result->ai_addrlen) == -1){
		perror("connect");
		exit(2);
	}
	else{
		printf("Connection is good!\n"); 
	}

	return sock_fd; 
}

//Send message
void send_message(char *msg, int sock_fd){
	write(sock_fd, msg, strlen(msg));
}

//Receive message
int read_response(int sock_fd){
	// Response
	char resp[1000];
	int len = read(sock_fd, resp, 999);
	resp[len] = '\0';
	return atoi(resp);
}

int main(int argc, char **argv){

	int sock_fd = do_connect("localhost", "1115"); 
	int status = -1;
	while(1) {
		int rc;
	    char buff[1000];

	    rc = getLine ("Guess a number between 1-50> ", buff, sizeof(buff));
	    if (rc == NO_INPUT) {
	        // Extra NL since my system doesn't output that on EOF.
	        printf ("\nNo input\n");
	        return 1;
	    }

	    if (rc == TOO_LONG) {
	        printf ("Input too long [%s]\n", buff);
	        return 1;
	    }
		send_message(buff, sock_fd);
		int result = read_response(sock_fd);
		if(result > 0) {
			printf("You guessed my secret number! It only took you %d guesses, plus something I didn't understand.\n", result);
			break;
		}
		if(result == 0) {
			printf("Too high\n");
			continue;
		}
		if(result == -1) {
			printf("Too low\n");
			continue;
		}
		if (result == -2) {
			printf("I don't recognize that number. Please guess a number between 1 and 50\n");
		}
	}

	close(sock_fd);

	return 0;
}