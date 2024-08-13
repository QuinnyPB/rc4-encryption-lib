/* example.c */
#include <dirent.h>
#include <stdio.h> 
#include <stdbool.h> 
#include "arcfour.h"

#define F fflush(stdout)

int main(void);

// prints encrytped data
void printbin(int8 *input, const int16 size) {
  int16 i;
  int8 *p;

  // assert: hecks that condition is true, else halts program
  assert(size > 0);

  for (i=size, p=input; i; i--, p++){
    if (!(i % 2)) // if even (= 0)
      printf(" ");
    printf("%.02x", *p);
  }  
  printf("\n");

  return;
}

int main() {
  Arcfour *rc4;
  int16 skey, stext;
  // char *key, *from;
  // char *key;
  int8 *encrypted, *decrypted;
  char from[1024];
  char key[24] = "default_key";

  // static declaration

  // dynamic declaration
  // printf("Enter a key of 24 bits for cipher generation: ");
  // fgets(key, sizeof(key), stdin);
  // printf("The key for this session is: %s\n", key);
  // printf("Please enter anything you would like to encrpyt?: \n");
  // fgets(from, sizeof(from), stdin);
  // skey = strlen(key);
  // stext = strlen(from);

  while (true){
    int userInp=-1;
    printf("The key for this session is: '%s'\n", key);
    printf("Enter the number of desired operation: \n");
    printf("0. Set encryption key\n");
    printf("1. Encrypt a file\n");
    printf("2. Decrypt a file\n");
    printf("3. Exit\n");
    scanf("%d", &userInp);  // get user input
    fflush(stdin);

    switch(userInp) {
      case 0:
        printf("Enter a key of 8-24 bits for cipher generation: ");
        fgets(key, sizeof(key), stdin);
        // Remove trailing newline character if present
        key[strcspn(key, "\n")] = '\0';
        fflush(stdin);
        break;


      case 1:
        printf("Place the file you want encrypted in the directory where this program is located.\n");
        // printf("Select the file you wish to encrypt: ");

        struct dirent *de;  // Pointer for directory entry   
        DIR *dr = opendir(".");     
        if (dr == NULL)  // opendir returns NULL if couldn't open directory 
        { 
            perror("opendir");
            return EXIT_FAILURE;
        } 
      
        char chosenFile[124];
        int files=0, choice=0;
        printf("Files in the current directory:\n");
        while ((de = readdir(dr))) {
            // Skip "." and ".."
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
                printf("%d: %s\n", ++files, de->d_name);
            }
        }
        closedir(dr);

        if (files > 0){
          printf("Choose a file (by number) to encrypt: ");
          scanf("%d", &choice);
          // ensures valid number chosen
          if (choice < 0 || choice > files){
            printf("Invalid value. Choose a number shown!\n ");
            break;
          }

          dr = opendir(".");
          if (de == NULL){
            perror("opendir");
            return EXIT_FAILURE;
          }
          
          files=0;
          while ((de = readdir(dr))) {
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
              files++;
              if (files == choice){
                strcpy(chosenFile, de->d_name);
                printf("You have chosen file %s", chosenFile);
                break;
              }
            }
          }
        } else {
          printf("Please place files you want encrypted into directory of program!");
        }     
        break;


      case 2:
        printf("op 2");

        break;


      case 3:
        printf("Exiting Program...");
        return 0;
        break;
    }
  }


  printf("Initializing encryption..."); F;
  rc4 = rc4init((int8 *)key, skey); // check if rc4 returns 0, in such a case malloc returns an error
  printf("done\n");

  printf("'%s' \n ->", from);
  encrypted = rc4encrypt(rc4, (int8 *)from, stext);
  printbin(encrypted, stext);  
  rc4uninit(rc4);

  printf("Initializing decryption..."); F;
  rc4 = rc4init((int8 *)key, skey); // check if rc4 returns 0, in such a case malloc returns an error
  printf("done\n");

  decrypted = rc4encrypt(rc4, encrypted, stext);
  printf(" ->'%s'", decrypted);
  rc4uninit(rc4);

  return 0;
}