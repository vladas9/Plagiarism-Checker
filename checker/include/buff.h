
// buff.h
#ifndef BUFF_H
#define BUFF_H

#include <stdio.h>
#include <zlib.h>

typedef unsigned long ulong;
typedef unsigned int uint;

typedef struct Buff {
  char *dat;
  char *name;
  ulong len;
  struct Buff *next;
} Buff;

typedef struct BuffList {
  Buff *head;
  Buff *tail;
} BuffList;

typedef struct {
  Buff **buffs1;
  Buff **buffs2;
  float **dists;
  uint num;
} BuffDB;

Buff *makeBuff(char *input, char *name);
Buff *makeZBuff(Buff *inputBuff);
Buff *catBuffs(Buff *buff1, Buff *buff2);
Buff *maxBuff(Buff *x, Buff *y);
Buff *minBuff(Buff *x, Buff *y);
Buff **crawl_dir(char *txt, int *idx);
BuffDB *makeDB(uint num, Buff **buffs1, Buff **buffs2);
float normCompDist(Buff *x, Buff *y);
void symmDB(BuffDB *db);
void writeDB(FILE *fd, BuffDB *db);

BuffList *makeBuffList();
void cleanupBuffList(BuffList *bl);
void pushBuff(BuffList *bList, Buff *buff);

#endif // BUFF_H