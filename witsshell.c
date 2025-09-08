#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <ctype.h>

ssize_t nread;
char error_message[30] = "An error has occurred\n";


//FUNCTION FOR SPLITTING LINE INTO TOKENS - using strsep()
char **split_line(char *line){
	char **tokens;
	tokens = malloc(sizeof(char*) * (nread+1));
	if (tokens == NULL) {
		write(STDERR_FILENO, error_message, strlen(error_message));
        free(line);
        exit(1);
    }

	char *s = line;  // strsep needs a char* pointer
    char *token;
    int i = 0;

    while ((token = strsep(&s, " \t\n")) != NULL) {
        if (*token == '\0') continue; // skip empty tokens (caused by multiple spaces)
        tokens[i++] = token;
    }
    tokens[i] = NULL;  // NULL-terminate the array

    // Debug: print tokens
    for (int j = 0; tokens[j] != NULL; j++) {
        printf("Token[%d] = '%s'\n", j, tokens[j]);
    }

    // Free memory
    free(tokens);
    free(line);
    return 0;

}

/*
//FUNCTION FOR READING INPUT
char *read_line(void){
	char *buf;
	size_t bufsize;

	buf = NULL;
	printf("witsshell> ");

	nread = getline(&buf, &bufsize, stdin);
	if (nread == -1){
		buf = NULL;
		exit(0);
	}

	return buf;
}
*/

//FUNCTION FOR TRIMMING WHITESPACES
char *trim(char *str) {
    char *end;

    //skip leading spaces
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // all spaces?
        return str;

    //trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    //write new null terminator
    *(end + 1) = '\0';

    return str;
}


int main(int MainArgc, char *MainArgv[]){
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	//write(STDERR_FILENO, error_message, strlen(error_message));  --ERROR MESSAGE 

	if (MainArgc == 1){ //no arguments - interactive mode

		while (true) {
			/*
			//tutorial on YT
			line = read_line();
			printf("%s", line);

			split_line(line);
			//end of tut
			*/
			
			printf("witsshell> ");
			nread = getline(&line, &len, stdin);

			char *cmd = trim(line);

			if (strcmp(cmd, "exit") == 0) {  //0 means true here
				exit(0);
				free(line);
			}else{
				split_line(line);
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
			char *cmd = trim(line);
			if (strcmp(cmd, "exit") == 0 || nread == -1) {
				printf("\n");
				exit(0);
			}else{
				split_line(line);
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
