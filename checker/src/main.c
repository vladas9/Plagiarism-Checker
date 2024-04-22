#include "../include/buff.h"
#include "../include/parser.h"
#include <assert.h>
#include <clang-c/Index.h>
#include <dirent.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <zlib.h>

int main(int argc, char *argv[]) {
  BuffList *progs = crawlDir("./solutions/");
  BuffDB *db = makeDB(progs, progs);
  symmDB(db);

  FILE *fd_db = fopen("./tmp/all_files.csv", "w");
  if (fd_db) {
    writeDB(fd_db, db);
    fclose(fd_db);
  }

  // printf("filename: %s\n", progs->list[0]->name);
  // BuffList *test = parseCodeTxt(progs->list[0]->name, 1);
  // printf("%s\n", test->list[0]->dat);
  // return 0;

  for (int i = 0; i < progs->len; i++) {
    char json_filename[256];
    sprintf(json_filename, "./tmp/%s.json",
            basename(strdup(progs->list[i]->name)));

    FILE *fd = fopen(json_filename, "w");
    if (fd) {
      BuffList *blocks = parseCodeTxt(progs->list[i]->name, 0);
      writeList(fd, blocks);
      printf("created: %s\n", json_filename);
      fclose(fd);
    }
  }

  // for (int i = 0; i < db->buffs2->len; i++) {
  //   char json_filename[256];
  //   sprintf(json_filename, "./tmp/%s.json",
  //           basename(strdup(db->buffs2->list[i]->name)));
  //   FILE *fd = fopen(json_filename, "w");
  //   if (fd) {
  //     BuffList *blocks = parseCodeTxt(db->buffs2->list[i]->name, 0);
  //     writeList(fd, blocks);
  //     printf("created: %s\n", json_filename);
  //     fclose(fd);
  //   }
  // }

  for (int i = 0; i < progs->len; i++) {
    for (int j = 0; j < progs->len; j++) {
      char csv_filename[256];
      sprintf(csv_filename, "./tmp/AST_%s_vs_%s.csv",
              basename(strdup(progs->list[i]->name)),
              basename(strdup(progs->list[j]->name)));
      FILE *fd_blocks_db = fopen(csv_filename, "w");
      if (fd_blocks_db) {
        BuffDB *blocksDB = makeDB(parseCodeTxt(progs->list[i]->name, 1),
                                  parseCodeTxt(progs->list[j]->name, 1));
        writeDB(fd_blocks_db, blocksDB);
        printf("created: %s\n", csv_filename);
        fclose(fd_blocks_db);
      }
    }
  }

  for (int i = 0; i < progs->len; i++) {
    for (int j = 0; j < progs->len; j++) {
      char csv_filename[256];
      sprintf(csv_filename, "./tmp/%s_vs_%s.csv",
              basename(strdup(progs->list[i]->name)),
              basename(strdup(progs->list[j]->name)));
      FILE *fd_blocks_db = fopen(csv_filename, "w");
      if (fd_blocks_db) {
        BuffDB *blocksDB = makeDB(parseCodeTxt(progs->list[i]->name, 0),
                                  parseCodeTxt(progs->list[j]->name, 0));
        writeDB(fd_blocks_db, blocksDB);
        printf("created: %s\n", csv_filename);
        fclose(fd_blocks_db);
      }
    }
  }

  // TODO extept not rlly: Free memory for BuffList, BuffDB, and contents

  return 0;
}
