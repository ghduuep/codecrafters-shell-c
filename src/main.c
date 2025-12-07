#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  char commands[][10] = {"exit", "echo"};
  
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
       for(int i = 0; i < 2; i++) {
         if(strcmp(commands[i], linha) == 0) {
           printf("%s is a shell builtin\n", linha);
           encontrou = 1;
           break;
         }
        }

        if(encontrou == 0) {
          printf("%s: not found\n", linha);
         }

         continue;
      }
   }

    printf("%s: command not found\n", command);     
  
  }
  return 0;
}
