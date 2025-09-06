#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>

void pieces(){

}


int main(int MainArgc, char *MainArgv[]){
	char error_message[30] = "An error has occurred\n";
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t nread; 
	//write(STDERR_FILENO, error_message, strlen(error_message));  --ERROR MESSAGE 

	if (MainArgc == 1){ //no arguments - interactive mode

		while (true) {
			printf("witsshell> ");
			nread = getline(&line, &len, stdin);

			if (strcmp(line, "exit\n") == 0) {  //0 means true here
				exit(0);
				free(line);
			} else {
				printf("witsshell> ");
				getline(&line, &len, stdin);
				// parse & dispatch
			}

			//printf("%s", line);
			//free(line);
		}

	}else if (MainArgc == 2){ //one argument - batch mode
		printf("You're in batch mode - yay!\n");
		fp = fopen(MainArgv[1], "r");  
		if (fp == NULL){
			write(STDERR_FILENO, error_message, strlen(error_message));
		}

		//batch mode loop
		while (true){
			nread = getline(&line, &len, fp);
			if (strcmp(line, "exit\n") == 0 || nread == -1) {
				printf("\n");
				exit(0);
			}else{
				printf("%s", line);
			}

		}
		free(line);   // free memory allocated by getline
    	fclose(fp);   // close file

	}else{
		write(STDERR_FILENO, error_message, strlen(error_message));
		exit(1);
	}

	exit(0); // EOF in either mode: exit(0)

	return(0);
}
