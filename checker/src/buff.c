#include "../include/buff.h"
#include "../include/parser.h"
#include <assert.h>
#include <dirent.h>
#include <libgen.h>
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

Buff *makeBuff(char *input, const char *name) {
  Buff *buff = malloc(sizeof(Buff));
  buff->len = strlen(input) + 1;
  buff->dat = strdup(input);
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
  buff->name = "";
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

BuffDB *makeDB(BuffList *b_list1, BuffList *b_list2) {
  BuffDB *db = malloc(sizeof(BuffDB));
  if (!db) {
    fprintf(stderr, "Failed to allocate memory for BuffDB\n");
    return NULL;
  }
  db->buffs1 = b_list1;
  db->buffs2 = b_list2;
  db->dists = malloc(sizeof(float *) * b_list2->len);
  if (!db->dists) {
    fprintf(stderr, "Failed to allocate memory for dists array\n");
    free(db);
    return NULL;
  }

  for (int i = 0; i < b_list2->len; i++) {
    db->dists[i] = malloc(sizeof(float) * b_list1->len);
    if (!db->dists[i]) {
      fprintf(stderr, "Failed to allocate memory for dists[%d]\n", i);
      for (int k = 0; k < i; k++) {
        free(db->dists[k]);
      }
      free(db->dists);
      free(db);
      return NULL;
    }
    for (int j = 0; j < b_list1->len; j++) {
      db->dists[i][j] = normCompDist(b_list2->list[i], b_list1->list[j]);
    }
  }
  return db;
}

void symmDB(BuffDB *db) {
  if (db->buffs1->len != db->buffs2->len) {
    printf("symmDB failed, lenghts differ\n");
    exit(1);
  }
  for (int i = 0; i < db->buffs1->len; i++) {
    for (int j = i; j < db->buffs1->len; j++) {
      if (i == j) {
        db->dists[i][j] = 0;
      } else {
        float avg = (db->dists[i][j] + db->dists[j][i]) / 2.0;
        db->dists[i][j] = avg;
        db->dists[j][i] = avg;
      }
    }
  }
}

// ChatGpt variant, maybe it's better, no idea..
// void writeDB(FILE *fd, BuffDB *db) {
//   if (fd == NULL || db == NULL || db->buffs1 == NULL || db->buffs2 == NULL)
//     fprintf(stderr, "db empty, not writing\n");
//   return;
//
//   // Print column headers
//   fprintf(fd, "-------,");
//   if (db->buffs1->len > 0) {
//     for (int i = 0; i < db->buffs1->len - 1; i++) {
//       if (db->buffs1->list[i]) {
//         fprintf(fd, "%s,", db->buffs1->list[i]->name);
//       }
//     }
//     if (db->buffs1->list[db->buffs1->len - 1]) {
//       fprintf(fd, "%s\n", db->buffs1->list[db->buffs1->len - 1]->name);
//     } else {
//       fprintf(fd, "\n");
//     }
//   } else {
//     fprintf(fd, "\n");
//   }
//
//   // Print matrix of distances
//   if (db->dists != NULL) {
//     for (int i = 0; i < db->buffs2->len; i++) {
//       if (db->buffs2->list[i]) {
//         fprintf(fd, "%s,", db->buffs2->list[i]->name);
//       }
//
//       if (db->buffs1->len > 0) {
//         for (int j = 0; j < db->buffs1->len - 1; j++) {
//           fprintf(fd, "%f,", db->dists[i][j]);
//         }
//         fprintf(fd, "%f\n", db->dists[i][db->buffs1->len - 1]);
//       } else {
//         fprintf(fd, "\n");
//       }
//     }
//   }
// }

void writeDB(FILE *fd, BuffDB *db) {
  fprintf(fd, "-------,");
  for (int i = 0; i < db->buffs1->len - 1; i++) {
    fprintf(fd, "%s,", basename(strdup(db->buffs1->list[i]->name)));
  }
  fprintf(fd, "%s\n",
          basename(strdup(db->buffs1->list[db->buffs1->len - 1]->name)));
  for (int i = 0; i < db->buffs2->len; i++) {
    fprintf(fd, "%s,", basename(strdup(db->buffs2->list[i]->name)));
    for (int j = 0; j < db->buffs1->len - 1; j++) {
      fprintf(fd, "%f,", db->dists[i][j]);
    }
    fprintf(fd, "%f\n", db->dists[i][db->buffs1->len - 1]);
  }
  return;
}

int compare(const void *a, const void *b) {
  const Buff *buff_a = *(const Buff **)a;
  const Buff *buff_b = *(const Buff **)b;
  return strcmp(buff_a->name, buff_b->name);
}

BuffList *crawlDir(char *dir_path) {
  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir(dir_path)) == NULL) {
    perror("Error opening directory");
    exit(EXIT_FAILURE);
  }

  BuffList *b_list = makeBuffList();
  while ((ent = readdir(dir)) != NULL) {
    if (ent->d_type == DT_REG) {
      char file_path[256];
      snprintf(file_path, sizeof(file_path), "%s%s", dir_path, ent->d_name);
      if (strstr(ent->d_name, ".c") != NULL) {
        Buff *buff = makeBuff(readCFile(file_path), file_path);
        pushBuff(b_list, buff);
      }
    }
  }
  closedir(dir);
  Buff **buffl = b_list->list;
  qsort(buffl, b_list->len - 1, sizeof(Buff *), compare);
  return b_list;
}

BuffList *makeBuffList() {
  BuffList *bl = malloc(sizeof(BuffList));
  if (bl == NULL) {
    return NULL;
  }
  bl->cap = 10;
  bl->len = 0;
  bl->list = malloc(sizeof(Buff *) * bl->cap);
  if (bl->list == NULL) {
    free(bl);
    return NULL;
  }
  return bl;
}

// void cleanupBuffList(BuffList *bl) {}

void pushBuff(BuffList *bl, Buff *buff) {
  if (bl->len > bl->cap) {
    bl->cap *= 2;
    bl->list = realloc(bl->list, bl->cap * sizeof(Buff *));
  }

  bl->list[bl->len] = buff;
  bl->len++;
}

char *escapeJsonString(const char *input) {
  if (!input)
    return NULL;

  int len = strlen(input);
  char *escaped =
      malloc(len * 2 + 1); // Worst case, every char needs to be escaped
  if (!escaped)
    return NULL;

  char *out = escaped;
  while (*input) {
    switch (*input) {
    case '\"':
      *out++ = '\\';
      *out++ = '\"';
      break;
    case '\\':
      *out++ = '\\';
      *out++ = '\\';
      break;
    case '\b':
      *out++ = '\\';
      *out++ = 'b';
      break;
    case '\f':
      *out++ = '\\';
      *out++ = 'f';
      break;
    case '\n':
      *out++ = '\\';
      *out++ = 'n';
      break;
    case '\r':
      *out++ = '\\';
      *out++ = 'r';
      break;
    case '\t':
      *out++ = '\\';
      *out++ = 't';
      break;
    default:
      *out++ = *input;
    }
    input++;
  }
  *out = '\0';
  return escaped;
}

void writeList(FILE *fd, BuffList *buffList) {
  if (!fd || !buffList->list || buffList->len == 0)
    return;

  fprintf(fd, "[\n");
  for (int i = 0; i < buffList->len; i++) {
    Buff *buff = buffList->list[i];
    if (!buff)
      continue;

    char *escapedDat = escapeJsonString(buff->dat);
    // char *escapedName = escapeJsonString(buff->name);

    fprintf(fd, "\"%s\"", escapedDat ? escapedDat : "");

    if (i < buffList->len - 1) {
      fprintf(fd, ",\n");
    } else {
      fprintf(fd, "\n");
    }

    free(escapedDat);
    // free(escapedName);
  }
  fprintf(fd, "]\n");
}
