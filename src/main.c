#include <linux/limits.h>
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

void execute_type(char *token, char commands[][20], int num_commands); 
void execute_pwd();  
void execute_echo(char *token);
void execute_cd(char *token);

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  char commands[][20] = {"exit", "echo", "type", "pwd"};
  int num_commands = sizeof(commands) / sizeof(commands[0]);
  
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
      execute_echo(token);
      continue;
   }

   if(strcmp(token, "type") == 0) {
     token = strtok(NULL, " ");
     execute_type(token, commands, num_commands);
     continue;
   }

  if(strcmp(token, "pwd") == 0) {
    execute_pwd();
    continue; 
  }

  if(strcmp(token, "cd") == 0) {
    token = strtok(NULL, " ");
    execute_cd(token);
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

void execute_cd(char *path) {
  int changed;
  if(strcmp(path, "~") == 0) {
    path = getenv("HOME");
  }
  changed = chdir(path);
  if (changed != 0) {
    printf("cd: %s: No such file or directory\n", path);
  }
}

void execute_echo(char *token) {
  char *argumento = strtok(NULL, " ");
      while(argumento != NULL) {
        printf("%s ", argumento);
        argumento = strtok(NULL, " ");
  }
  printf("\n");
}

void execute_pwd() {
  char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("%s\n", cwd);
    }
}

void execute_type(char *token, char commands[][20], int num_commands) {
 
   if (token != NULL) {
         int encontrou = 0;
         for(int i = 0; i < num_commands; i++) {
           if(strcmp(commands[i], token) == 0) {
             printf("%s is a shell builtin\n", token);
             encontrou = 1;
             break;
           }
          }

          if(encontrou == 0) {
            char *path_variable = getenv("PATH");
            char *path_copy = strdup(path_variable);
            char *diretorio = strtok(path_copy, PATH_SEPARATOR);
          
            while(diretorio != NULL) {

               char *full_path = malloc(strlen(diretorio) + 1 + strlen(token) + 1);
               sprintf(full_path, "%s/%s", diretorio, token);
               if(access(full_path, X_OK) == 0) {
                  printf("%s is %s\n", token, full_path);
                  encontrou = 1;
                  free(full_path);
                  break;
                }
               diretorio = strtok(NULL, PATH_SEPARATOR);
               free(full_path);
               continue;
              }

              free(path_copy);
            }

          if(encontrou == 0) {
            printf("%s: not found\n", token);
           }
         
        }

 } 
