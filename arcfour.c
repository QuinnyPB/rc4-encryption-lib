/* arcfour.c */
#define _GNU_SOURCE
#include "arcfour.h"

// RC4 implementation as defined by wikipedia: https://en.wikipedia.org/wiki/RC4
// see link for details on functions
export Arcfour *rc4init(int8 *key, int16 size) {
  int16 x;
  int8 t1;
  Arcfour *p;
  int32 n;

  if (!(p = malloc(sizeof(struct s_arcfour)))){
    // replacement for assert_perror()
    if (errno != 0) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    // assert(errno);
  } 

  // Initialize p->s
  for (x = 0; x < 256; x++) {
    p->s[x] = x;
  }

  p->i = p->j = 0;

  // Key-Scheduling Algorithm: KSA
  for (p->i = 0; p->i < 256; p->i++) {
    p->j = (p->j + p->s[p->i] + key[p->i % size]) % 256;
    
    // Swap p->s[p->i] and p->s[p->j]
    t1 = p->s[p->i];
    p->s[p->i] = p->s[p->j];
    p->s[p->j] = t1;
  }

  p->i = p->j = 0;  // Reset i and j after KSA
  rc4whitewash(n, p)

  return p;  
}

// Pseudo-random generation algorithm: PRGA
int8 rc4byte(Arcfour *p) {
  int16 t1, t2;

  p->i = (p->i + 1) % 256;
  p->j = (p->j + p->s[p->i]) % 256;

  t1 = p->s[p->i];
  t2 = p->s[p->j];
  p->s[p->i] = t2;
  p->s[p->j] = t1;

  t1 = (p->s[p->i] + p->s[p->j]) % 256;
  p->k = p->s[t1];

  return p->k;
}

export int8 *rc4encrypt(Arcfour *p, int8 *plaintext, int16 size) {
  int8 *ciphertext;
  int16 x;

  ciphertext = (int8 *)malloc(size+1); 
  if (!ciphertext){
    // replacement for assert_perror()
    if (errno != 0) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
  }

  for (x=0; x<size; x++){
    ciphertext[x] = plaintext[x] ^ rc4byte(p); // ^ = XOR
  }

  ciphertext[size] = '\0'; // null terminator

  return ciphertext;
}
