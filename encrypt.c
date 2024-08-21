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

// take a plaintext file and grab its data
char* getFileContents(char *buffer, char *filename){
  FILE *filePtr;
  long filesize;

  // printf("Opening file"); F;
  filePtr = fopen(filename, "r");
  if (filePtr == NULL){
    fprintf(stderr, "Error opening file\n");
    return buffer;
  }

  // Get file size
  fseek(filePtr, 0, SEEK_END);
  filesize = ftell(filePtr);
  fseek(filePtr, 0, SEEK_SET);

  // allocate memory to buffer
  buffer = (char *)malloc(filesize+1);
  if (buffer == NULL){
    fprintf(stderr, "Memory allocation error\n");
    fclose(filePtr);
    return buffer;
  }

  // read whole file into buffer
  fread(buffer, 1, filesize, filePtr);
  buffer[filesize] = "\0"; // null terminator for buffer safety
  fclose(filePtr);
  // printf("Closing file"); F;

  // printf("File contents are: \n%s", buffer); F;
  return buffer;
}

// take encrypted data and output it to new file
void outputFileContents(char *buffer, char *filename, long filesize){
  FILE *toFile;
  // char new_filename[] = "encrypted_" + *filename;
  char new_filename[] = "encrypted_file.txt";
  toFile = fopen(new_filename, "w");
  if (toFile == NULL){
    fprintf(stderr, "Could not output contents");
    return;
  }
  printf("Writing ciphertext to file\n"); F;
  fwrite(buffer, 1, filesize, toFile);
  printf("Closing file\n"); F;
  fclose(toFile);
  free(buffer);
  printf("Freeing buffer\n"); F;

}

void encryptFile(){

}

void encryptText(){

}

void decryptFile(){

}

void decrpytText(){
  
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
    printf("3. Encrypt text\n");
    printf("4. Decrypt text\n");
    printf("5. Exit\n");
    scanf("%d", &userInp);  // get user input
    fflush(stdin); F;

    switch(userInp) {
      // Set new key
      case 0:
        printf("Enter a key of 8-24 bits for cipher generation: "); F;
        fgets(key, sizeof(key), stdin);
        skey = strlen(key);
        // Remove trailing newline character if present
        key[strcspn(key, "\n")] = '\0';
        fflush(stdin); F;
        break;

      // encrypt a file
      case 1:
        printf("Place the file you want encrypted in the directory where this program is located.\n"); F;
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
        F;

        if (files > 0){
          printf("Choose a file (by number) to encrypt: ");
          scanf("%d", &choice);
          // ensures valid number chosen
          if (choice < 0 || choice > files){
            printf("Invalid value. Choose a number shown!\n ");
            break;
          }
          fflush(stdin); 

          dr = opendir(".");
          // if (de == NULL){
          //   perror("opendir");
          //   return EXIT_FAILURE;
          // }
          
          files=0;
          char *buffer;
          // finds chosen file
          while ((de = readdir(dr))) {
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
              files++;
              if (files == choice){
                strcpy(chosenFile, de->d_name);
                printf("You have chosen file %s\n", chosenFile); F;
                buffer = getFileContents(buffer, chosenFile);
                stext = strlen(buffer);
                printf("%s\n",buffer); F;

                printf("Initializing encryption...\n"); F;
                rc4 = rc4init((int8 *)key, skey); // check if rc4 returns 0, in such a case malloc returns an error
                printf("done\n");

                // printf("'%s' \n ->", from);
                encrypted = rc4encrypt(rc4, (int8 *)buffer, stext);
                printbin(encrypted, stext);  
                outputFileContents(encrypted, chosenFile, stext);
                rc4uninit(rc4);

                break;
              }
            }
          }
        } else {
          printf("Please place files you want encrypted into directory of program!");
        }     
        break;

      // Decrypt a File
      case 2:
        printf("Decrypt a file...");

        break;

      // Encrypt some user text
      case 3:
        printf("Encrypt text...");
        return 0;
        break;

      // Decrypt some user text
      case 4:
        printf("Decrypt text...");
        return 0;
        break;

      // Exit program
      case 5:
        printf("Exiting Program...");
        return 0;
        break;
    }
  }


  // printf("Initializing encryption..."); F;
  // rc4 = rc4init((int8 *)key, skey); // check if rc4 returns 0, in such a case malloc returns an error
  // printf("done\n");

  // printf("'%s' \n ->", from);
  // encrypted = rc4encrypt(rc4, (int8 *)from, stext);
  // printbin(encrypted, stext);  
  // rc4uninit(rc4);

  // printf("Initializing decryption..."); F;
  // rc4 = rc4init((int8 *)key, skey); // check if rc4 returns 0, in such a case malloc returns an error
  // printf("done\n");

  // decrypted = rc4encrypt(rc4, encrypted, stext);
  // printf(" ->'%s'", decrypted);
  // rc4uninit(rc4);

  return 0;
}