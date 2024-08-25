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
void getFileContents(char *buffer, char *filename){
  FILE *filePtr;
  long filesize;

  // printf("Opening file"); F;
  filePtr = fopen(filename, "r");
  if (filePtr == NULL){
    fprintf(stderr, "Error opening file\n");
    return;
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
    return;
  }

  // read whole file into buffer
  fread(buffer, 1, filesize, filePtr);
  buffer[filesize] = "\0"; // null terminator for buffer safety
  fclose(filePtr);
}


// take encrypted data and output it to new file
void outputContent(int8 *buffer, char *filename){
  FILE *toFile;
  long filesize = strlen((char *)buffer);

  // char new_filename[] = "encrypted_" + *filename;
  // char new_filename[] = filename;

  toFile = fopen(filename, "w");
  if (toFile == NULL){
    fprintf(stderr, "Could not output contents");
    return;
  }
  printf("Writing ciphertext to file '%s'\n", filename); F;
  fwrite(buffer, 1, filesize, toFile);

  printf("Closing file\n"); F;
  fclose(toFile);

  free(buffer);
  printf("Freeing buffer\n"); F;
}


void encryptFile(){

}

// encrypts user input and stores in encrypted argument
void encryptText(Arcfour *rc4, char *key, int16 *skey, int8 *encrypted, int16* stext){
  printf("init outputFile()..."); F;

  int8 *decrypted;
  char plaintext[1024];

  printf("Please enter the text you would like to encrypt:\n"); F;

  if (fgets(plaintext, sizeof(plaintext), stdin) != NULL){
    stext = strlen(plaintext);
    encrypted = rc4encrypt(rc4, (int8 *)plaintext, stext);
    printbin(encrypted, stext);  F;    
    rc4uninit(rc4);

    // printf("Initializing decryption..."); F;
    // rc4 = rc4init((int8 *)key, skey); // check if rc4 returns 0, in such a case malloc returns an error
    // printf("done\n");

    // decrypted = rc4encrypt(rc4, encrypted, stext);
    // printf(" ->'%s'", decrypted);
    // rc4uninit(rc4);
  } else {
    printf("Error getting user input!\n"); F;
  }  
  return;
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
  int8 *encrypted;
  // int8 *decrypted;
  // char from[1024];
  char key[24] = "default_key";
  skey = strlen(key);
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
                getFileContents(buffer, chosenFile);
                // stext = strlen(buffer);
                printf("%s\n",buffer); F;

                printf("Initializing encryption...\n"); F;
                rc4 = rc4init((int8 *)key, skey); // check if rc4 returns 0, in such a case malloc returns an error
                printf("done\n");

                // printf("'%s' \n ->", from);
                encrypted = rc4encrypt(rc4, (int8 *)buffer, stext);
                printbin(encrypted, stext);  
                outputContent(encrypted, "encrypted_file.txt"); // temp
                // outputContent(encrypted, chosenFile);
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
        printf("init rc4..."); F;
        rc4 = rc4init((int8 *)key, skey);
        printf("init encryptText()...\n"); F;
        encryptText(rc4, key, skey, encrypted, stext); // sets encrypted to new value
        // printbin(encrypted, stext);  

        // printf("init outputFile()...\n"); F;
        // outputContent(encrypted, "encrypted.txt");
        // printf("init rc4unit()..."); F;
        rc4uninit(rc4);
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