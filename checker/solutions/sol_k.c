#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <zconf.h>
#include <zlib.h>

char *read_c_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Error opening file %s\n", filename);
    return NULL;
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  // Allocate memory for file content
  char *content = (char *)malloc(size + 1);
  if (content == NULL) {
    fprintf(stderr, "Memory allocation error\n");
    fclose(file);
    return NULL;
  }

  // Read file content into memory
  size_t bytesRead = fread(content, sizeof(char), size, file);
  if (bytesRead != size) {
    fprintf(stderr, "Error reading file %s\n", filename);
    fclose(file);
    free(content);
    return NULL;
  }

  // Add null terminator
  content[size] = '\0';

  fclose(file);
  return content;
}

typedef struct Buff {
  char *dat;
  ulong len;
} Buff;

typedef struct {
  Buff **buffs;
  float **dists;
  uint num;
} BuffDB;

void dbAppend(BuffDB *db, Buff *buff) {
  db->buffs = realloc(db->buffs, sizeof(Buff *) * (db->num + 1));
  db->dists = realloc(db->dists, sizeof(float *) * (db->num + 1));
  for (int i = 0; i < db->num; i++) {
    db->dists[i] = malloc(sizeof(float) * (db->num + 1));
  }
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

BuffDB *makeDB(uint num, Buff **buffs) {
  BuffDB *db = malloc(sizeof(BuffDB));
  db->num = num;
  db->buffs = buffs;
  db->dists = malloc(sizeof(float *) * num);

  for (int i = 0; i < num; i++) {
    db->dists[i] = malloc(sizeof(float) * num);
    for (int j = 0; j < num; j++) {
      db->dists[i][j] = normCompDist(buffs[i], buffs[j]);
    }
  }
  for (int i = 0; i < num; i++) {
    for (int j = i + 1; j < num; j++) {
      float avg = (db->dists[i][j] + db->dists[j][i]) / 2.0;
      db->dists[i][j] = avg;
      db->dists[j][i] = avg;
    }
  }
  return db;
}

void writeDB(FILE *fd, BuffDB *db) {
  for (int i = 0; i < db->num; i++) {
    for (int j = 0; j < db->num - 1; j++) {
      fprintf(fd, "%f,", db->dists[i][j]);
    }
    fprintf(fd, "%f\n", db->dists[i][db->num - 1]);
  }
}
Buff **crawl_dir(char *dir_path, int *idx) {
  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir(dir_path)) == NULL) {
    perror("Error opening directory");
    exit(EXIT_FAILURE);
  }

  int cap = 10;
  Buff **list = malloc(sizeof(Buff *) * 10);

  while ((ent = readdir(dir)) != NULL) {
    if (*idx >= cap) {
      cap = cap * 2;
      list = realloc(list, cap * sizeof(Buff *));
    }

    if (ent->d_type == DT_REG) {
      char file_path[256];
      snprintf(file_path, sizeof(file_path), "%s%s", dir_path, ent->d_name);
      if (strstr(ent->d_name, ".c") != NULL) {
        list[*idx] = makeBuff(read_c_file(file_path));
        (*idx)++;
      }
    }
  }
  closedir(dir);
  return list;
}

int main() {
  int num = 0;
  Buff **progs = crawl_dir("./solutions/", &num);
  BuffDB *db = makeDB(num, progs);

  writeDB(stdout, db);

  // Free mem.
  for (int i = 0; i < db->num; i++) {
    free(progs[i]->dat);
    free(progs[i]);
  }
  for (int i = 0; i < db->num; i++) {
    free(db->dists[i]);
  }
  free(db->dists);
  free(db);

  return 0;
}
