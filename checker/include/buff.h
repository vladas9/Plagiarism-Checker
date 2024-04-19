
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
} Buff;

typedef struct BuffList {
  Buff **list;
  int cap, len;
} BuffList;

typedef struct {
  BuffList *buffs1;
  BuffList *buffs2;
  float **dists;
} BuffDB;

BuffList *crawlDir(char *txt);
Buff *makeBuff(char *input, const char *name);
Buff *makeZBuff(Buff *inputBuff);
Buff *catBuffs(Buff *buff1, Buff *buff2);
Buff *maxBuff(Buff *x, Buff *y);
Buff *minBuff(Buff *x, Buff *y);
BuffDB *makeDB(BuffList *b_list1, BuffList *b_list2);
float normCompDist(Buff *x, Buff *y);
void symmDB(BuffDB *db);

void writeDB(FILE *fd, BuffDB *db);
void writeList(FILE *fd, BuffList);

BuffList *makeBuffList();
void pushBuff(BuffList *b_list, Buff *buff);

// void cleanupBuffList(BuffList *bl);

#endif // BUFF_H
