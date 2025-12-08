#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#ifdef _WIN32
#define PATH_SEPARATOR ";"
#else
#define PATH_SEPARATOR ":"
#endif


int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  char commands[][10] = {"exit", "echo", "type"};
  
  while(1) {
    // TODO: Uncomment the code below to pass the first stage
    printf("$ ");

    char command[1024];
    fgets(command, sizeof(command), stdin);

    command[strcspn(command,"\n")] = '\0';

    char *token = strtok(command, " ");
  
    if(strcmp(token, "exit") == 0)
      break;

    if(strcmp(token, "echo") == 0) {
      char *argumento = strtok(NULL, " ");
      while(argumento != NULL) {
        printf("%s ", argumento);
        argumento = strtok(NULL, " ");
      }

      printf("\n");
      continue;
   }

   if(strcmp(token, "type") == 0) {
     char *linha = strtok(NULL, "");
     if (linha != NULL) {
       int encontrou = 0;
       for(int i = 0; i < 3; i++) {
         if(strcmp(commands[i], linha) == 0) {
           printf("%s is a shell builtin\n", linha);
           encontrou = 1;
           break;
         }
        }

        if(encontrou == 0) {
          char *path_variable = getenv("PATH");
          char *path_copy = strdup(path_variable);
          char *diretorio = strtok(path_copy, PATH_SEPARATOR);
          
          while(diretorio != NULL) {

             char *full_path = malloc(strlen(diretorio) + 1 + strlen(linha) + 1);
             sprintf(full_path, "%s/%s", diretorio, linha);
             if(access(full_path, X_OK) == 0) {
                printf("%s is %s\n", linha, full_path);
                encontrou = 1;
                break;
              }
             diretorio = strtok(NULL, ":");
             continue;
            }
          }

        if(encontrou == 0) {
          printf("%s: not found\n", linha);
          continue;
         }
      }
      continue;
   }

  char *args[100];
  args[0] = token;

  int i = 1;

  while((args[i] = strtok(NULL, " ")) != NULL) {
    i++;
  }

  args[i] = NULL;

  pid_t pid = fork();

  if (pid == 0) {
    execvp(args[0], args);
    printf("%s: command not found\n", args[0]);
    exit(1);
  }
  else if (pid > 0) {
    int status;
    wait(&status);
  } else {
    perror("fork failed");
  }
    
  
  }
  return 0;
}
