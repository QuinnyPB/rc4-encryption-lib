/* example.c */
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
  char key[24];
  char from[1024];
  int8 *encrypted, *decrypted;

  // static declaration
  // key = "tomatoes"; /* 8 Bits - 24 Bits */
  // skey = strlen(key);
  // from = "Shall I compare thee to a summer's day?";
  // stext = strlen(from);  

  // dynamic declaration
  printf("Enter a key of 24 bits for cipher generation: ");
  fgets(key, sizeof(key), stdin);
  printf("The key for this session is: %s\n", key);
  printf("Please enter anything you would like to encrpyt?: \n");
  fgets(from, sizeof(from), stdin);
  skey = strlen(key);
  stext = strlen(from);

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