
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <zconf.h>
#include <zlib.h>

typedef struct Buff {
  char *dat;
  ulong len;
} Buff;

typedef struct {
  Buff **buffs;
  float **dists;
  uint num;
} BuffDB;

BuffDB *makeDB(uint num, Buff **buffs) {
  BuffDB *db = malloc(sizeof(BuffDB));
  db->num = num;
  db->buffs = buffs;
  db->dists = malloc(sizeof(float *) * num);

  for (int i = 0; i < num; i++) {
    db->dists[i] = malloc(sizeof(float) * num);
    for (int j = 0; j < num; j++) {
      // TODO compute distances and place in DB. only do Upper half by diagonal,
      // then copy downwords.
    }
  }

  return db;
}

Buff *makeBuff(char *input) {
  Buff *buff = malloc(sizeof(Buff));
  buff->len = strlen(input) + 1;
  buff->dat = (char *)malloc(buff->len);
  strcpy(buff->dat, input);
  return buff;
}

Buff *makeZBuff(Buff *inputBuff) {
  Buff *buff = malloc(sizeof(Buff));
  // Compress the input data
  buff->len = compressBound(inputBuff->len);
  buff->dat = (char *)malloc(buff->len);
  compress((Bytef *)buff->dat, &buff->len, (const Bytef *)inputBuff->dat,
           inputBuff->len);
  return buff;
}

Buff *catBuffs(Buff *buff1, Buff *buff2) {
  Buff *result = malloc(sizeof(Buff));
  result->len = buff1->len + buff2->len - 1;
  result->dat = malloc(result->len);

  strcpy(result->dat, buff1->dat);
  strcat(result->dat, buff2->dat);

  return result;
}

Buff *maxBuff(Buff *x, Buff *y) {
  if (x->len > y->len)
    return x;
  return y;
}

Buff *minBuff(Buff *x, Buff *y) {
  if (x->len < y->len)
    return x;
  return y;
}

float normCompDist(Buff *x, Buff *y) {
  Buff *Cx = makeZBuff(x);
  Buff *Cy = makeZBuff(y);

  Buff *xy = catBuffs(x, y);
  Buff *Cxy = makeZBuff(xy);

  float ncd = (float)((float)Cxy->len - (float)minBuff(Cx, Cy)->len) /
              (float)maxBuff(Cx, Cy)->len;
  free(xy->dat);
  free(xy);
  return ncd;
}

int main() {
  Buff *inBuff_1 =
      makeBuff("Hello, this is a test string to compress using zlib!");
  Buff *inBuff_2 =
      makeBuff("Alien shot Donald in the foot. Now he cries obnoctiously");

  float ncd = normCompDist(inBuff_1, inBuff_2);
  printf("ncd: %f\n", ncd);

  // Free allocated memory
  free(inBuff_1->dat);
  free(inBuff_2->dat);
  free(inBuff_1);
  free(inBuff_2);

  return 0;
}
