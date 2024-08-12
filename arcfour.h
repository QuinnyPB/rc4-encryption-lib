/* arcfour.h */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#define MS                  200
#define export              __attribute__((visibility("default")))
#define rc4decrypt(x,y,z)   rc4encrypt(x,y,z) // the decryption method is just an encryption applied onto the already encrypted text
#define rc4uninit(x)        free(x)
#define rc4whitewash(x,y)     for (x=0; x<(MS*1000000); x++) \
                                  (volatile int8)rc4byte(y);

typedef unsigned char int8; // 8 bits
typedef unsigned short int int16; // 16 bits
typedef unsigned int int32; // 32 bits

// This encryption uses RC4 encryption schema
struct s_arcfour {
  int16 i, j, k;
  int8 s[256];  
};

typedef struct s_arcfour Arcfour;

export Arcfour *rc4init(int8*, int16);
int8 rc4byte(Arcfour*); // produces 1 char of encrypted text
export int8 *rc4encrypt(Arcfour*, int8*, int16);