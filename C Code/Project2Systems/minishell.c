#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>

int parseArg(char *buffer, char** args);
int redirectCheck(char** args, int size);
int redirectIndexCheck(char** args, int size);
int main(){
	char go = 1;
	long size;
	char cwd[PATH_MAX];
	int argSize = 10;
	int nargs = 0;
	while(go == 1){
		if(getcwd(cwd, sizeof(cwd)) != NULL){
			printf("%s>", cwd);
		}
		else{
			printf("didnt work\n");
		}
		char input[40];
		fgets(input, sizeof(input), stdin);
		char** args;
		args = malloc(sizeof(char*)*argSize);
		
		nargs = parseArg(input, args);
		for(int i = 0; i <= nargs; i++){
			printf("%s ", args[i]);
		}
		printf("\n");
		if(strcmp("exit", args[0]) == 0){
			go = 0;
		}
		else if(strcmp("cd", args[0]) == 0){
			if(args[1] != NULL){
				chdir(args[1]);
			}
		}
		else if(strcmp("&", args[nargs]) == 0){
			if(redirectCheck(args, nargs) == 1){
				int index = redirectIndexCheck(args, nargs);
				if(index != 0){
					pid_t child = fork();
					if(child == 0){
						setpgid(0,0);
						printf("%s\n", args[index]);
						FILE* fp = freopen(args[index], "w", stdout);
						args[index - 1] = '\0';
						pid_t child = fork();
						execvp(args[0], args);
						fclose(fp);
					}
					else{
						waitpid(-1, NULL, WNOHANG);
					}
					
				}
			}
			else if(redirectCheck(args, nargs) == 2){
				int index = redirectIndexCheck(args, nargs);
				if(index != 0){
					pid_t child = fork();
					if(child == 0){
						setpgid(0,0);
						printf("%s\n", args[index]);
						FILE* fp = freopen(args[index], "r", stdin);
						args[index - 1] = '\0';
						pid_t child = fork();
						execvp(args[0], args);
						fclose(fp);
					}
					else{
						waitpid(-1, NULL, WNOHANG);
					}
					
				}
			}
			else{
				args[nargs] = '\0';
				pid_t child = fork();
				if(child == 0){
					setpgid(0,0);
					execvp(args[0], args);
				}
				else{
					waitpid(-1, NULL, WNOHANG);
				}
			}
		}
		else{
			if(redirectCheck(args, nargs) == 1){
				int index = redirectIndexCheck(args, nargs);
				if(index != 0){
					pid_t child = fork();
					if(child == 0){
						printf("%s\n", args[index]);
						FILE* fp = freopen(args[index], "w", stdout);
						args[index - 1] = '\0';
						pid_t child = fork();
						execvp(args[0], args);
						fclose(fp);
					}
					else{
						waitpid(-1, NULL, 0);
					}
					
				}
			}
			else if(redirectCheck(args, nargs) == 2){
				int index = redirectIndexCheck(args, nargs);
				if(index != 0){
					pid_t child = fork();
					if(child == 0){
						printf("%s\n", args[index]);
						FILE* fp = freopen(args[index], "r", stdin);
						args[index - 1] = '\0';
						pid_t child = fork();
						execvp(args[0], args);
						fclose(fp);
					}
					else{
						waitpid(-1, NULL, 0);
					}
					
				}
			}
			else{
				pid_t child = fork();
				if(child == 0){
					execvp(args[0], args);
				}
				else{
					waitpid(-1, NULL, 0);
				}
			}
		}
	}
}

int redirectCheck(char** args, int size){
	for(int i = 0; i < size; i++){
		if(strcmp(args[i], ">") == 0){
			return 1;
		}
		if(strcmp(args[i], "<") == 0){
			return 2;
		}
	}
	return 0;
}
int redirectIndexCheck(char** args, int size){
	for(int i = 0; i < size; i++){
		if(strcmp(args[i], ">") == 0 || strcmp(args[i], "<") == 0){
			return i+1;
		}
	}
	return 0;
}


//A better parse funciton because the given one   was not working
int parseArg(char* buffer, char** args){
	int i = 0, j = 0, k = 0;
	char* temp = malloc(sizeof(char) * 40);
	while(buffer[i] > 31 &&  buffer[i] < 127){
		if(buffer[i] == ' '){
			args[j] = malloc(sizeof(char) * (k + 1));
			for(int iter = 0; iter < k; iter++){
				args[j][iter] = temp[iter];
				/*if(iter == k){
					args[j][iter] = '\0';
				}*/
			}
			j++;
			i++;
			k = 0;
		}
		
		temp[k] = buffer[i];
		k++;
		i++;
		
		
	}
	args[j] = malloc(sizeof(char) * (k + 1));
		for(int iter = 0; iter < k; iter++){
			args[j][iter] = temp[iter];
		}
	return j;
}