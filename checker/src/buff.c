#include "../include/buff.h"
#include "../include/parser.h"
#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <zconf.h>
#include <zlib.h>

char *readCFile(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Error opening file %s\n", filename);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *content = (char *)malloc(size + 1);
  if (content == NULL) {
    fprintf(stderr, "Memory allocation error\n");
    fclose(file);
    return NULL;
  }

  size_t bytesRead = fread(content, sizeof(char), size, file);
  if (bytesRead != size) {
    fprintf(stderr, "Error reading file %s\n", filename);
    fclose(file);
    free(content);
    return NULL;
  }

  content[size] = '\0';

  fclose(file);
  return content;
}

Buff *makeBuff(char *input, char *name) {
  Buff *buff = malloc(sizeof(Buff));
  buff->len = strlen(input) + 1;
  buff->dat = (char *)malloc(buff->len);
  strcpy(buff->dat, input);
  buff->name = strdup(name);
  return buff;
}

Buff *makeZBuff(Buff *inputBuff) {
  Buff *buff = malloc(sizeof(Buff));
  // Compress the input data
  buff->len = compressBound(inputBuff->len);
  buff->dat = (char *)malloc(buff->len);
  compress((Bytef *)buff->dat, &buff->len, (const Bytef *)inputBuff->dat,
           inputBuff->len);
  buff->name = inputBuff->name;
  return buff;
}

Buff *catBuffs(Buff *buff1, Buff *buff2) {
  Buff *result = malloc(sizeof(Buff));
  result->len = buff1->len + buff2->len - 1;
  result->dat = malloc(result->len);
  result->name = "";

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

BuffDB *makeDB(uint num, Buff **buffs1, Buff **buffs2) {
  BuffDB *db = malloc(sizeof(BuffDB));
  db->num = num;
  db->buffs1 = buffs1;
  db->buffs2 = buffs2;
  db->dists = malloc(sizeof(float *) * num);

  for (int i = 0; i < num; i++) {
    db->dists[i] = malloc(sizeof(float) * num);
    for (int j = 0; j < num; j++) {
      db->dists[i][j] = normCompDist(buffs1[i], buffs2[j]);
    }
  }
  return db;
}

void symmDB(BuffDB *db) {
  for (int i = 0; i < db->num; i++) {
    for (int j = i + 1; j < db->num; j++) {
      float avg = (db->dists[i][j] + db->dists[j][i]) / 2.0;
      db->dists[i][j] = avg;
      db->dists[j][i] = avg;
    }
  }
}

void writeDB(FILE *fd, BuffDB *db) {
  fprintf(fd, "-------,");
  for (int i = 0; i < db->num - 1; i++) {
    fprintf(fd, "%s,", db->buffs1[i]->name);
  }
  fprintf(fd, "%s\n", db->buffs1[db->num - 1]->name);

  for (int i = 0; i < db->num; i++) {
    fprintf(fd, "%s,", db->buffs2[i]->name);
    for (int j = 0; j < db->num - 1; j++) {
      fprintf(fd, "%f,", db->dists[i][j]);
    }
    fprintf(fd, "%f\n", db->dists[i][db->num - 1]);
  }
}

int compare(const void *a, const void *b) {
  const Buff *buff_a = *(const Buff **)a;
  const Buff *buff_b = *(const Buff **)b;
  return strcmp(buff_a->name, buff_b->name);
}

Buff **crawl_dir(char *dir_path, int *idx) {
  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir(dir_path)) == NULL) {
    perror("Error opening directory");
    exit(EXIT_FAILURE);
  }

  int cap = 5;
  Buff **list = malloc(sizeof(Buff *) * cap);
  while ((ent = readdir(dir)) != NULL) {
    if (*idx >= cap) {
      cap = cap * 2;
      list = realloc(list, cap * sizeof(Buff *));
    }
    if (ent->d_type == DT_REG) {
      char file_path[256];
      snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, ent->d_name);
      if (strstr(ent->d_name, ".c") != NULL) {

        list[*idx] = makeBuff(readCFile(file_path), file_path);
        (*idx)++;
      }
    }
  }
  closedir(dir);
  qsort(list, *idx, sizeof(Buff *), compare);
  return list;
}
// initialize_code_snippets
BuffList *makeBuffList(void) {
  BuffList *bList = malloc(sizeof(BuffList *));
  bList->head = bList->tail = NULL;
  return bList;
}

// cleanup_code_snippets
void cleanupBuffList(BuffList *bl) {
  Buff *current = bl->head;
  while (current) {
    Buff *temp = current;
    current = current->next;
    free(temp->dat);
    free(temp->name);
    free(temp);
  }
  bl->head = bl->tail = NULL;
}

void pushBuff(BuffList *bl, Buff *buff) {
  if (buff.tail == NULL) {
    head = tail = makeBuff(snippet, char *name);
  } else {
    tail->next = snippet;
    tail = snippet;
  }
}
