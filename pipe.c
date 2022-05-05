#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>



// File descriptors assigned by the pipe
// pipe_fd[0]: read
// pipe_fd[1]: write
int pipe_fd[2];


pid_t child_pid;

void parentTask();
void childTask();

int main(int argc, const char *argv[]){

	
	if(pipe(pipe_fd) == -1){
		perror("Cannot create the pipe");
		exit(EXIT_FAILURE);
	}

	printf("Atomic operations are guaranteed for message less then %d bytes\n", PIPE_BUF);
	printf("---------- Forking -----------\n");

	if((child_pid = fork()) == -1){
		perror("Process cannot be fork");
		exit(EXIT_FAILURE);
	}

	if(child_pid == 0){
		childTask();
	}
	else{
		parentTask();
	}
	return 0;
}


void parentTask(){
	
	close(pipe_fd[1]);

	char buffer[128];
	if(read(pipe_fd[0], buffer, 128) == -1){
		perror("Cannot read pipe");
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[0]);

	printf("PARENT - Received: %s\n", buffer);
	exit(EXIT_SUCCESS);
}


void childTask(){
	
	close(pipe_fd[0]);

	char *buffer = "Hello World";
	printf("CHILD - Sending : %s\n", buffer);

	write(pipe_fd[1], buffer, strlen(buffer) + 1);
	close(pipe_fd[1]);
	exit(EXIT_SUCCESS);
}
