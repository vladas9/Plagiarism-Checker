#include "../include/buff.h"
#include "../include/parser.h"
#include <assert.h>
#include <clang-c/Index.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <zconf.h>
#include <zlib.h>

int main(int argc, char *argv[]) {

  BuffList *progs = crawlDir("./solutions/");
  BuffDB *db = makeDB(progs, progs);
  symmDB(db);

  FILE *fd_db = fopen("./tmp/all_files.csv", "w");
  writeDB(fd_db, db);
  fclose(fd_db);

  // db->buffs1->list[1]->name
  BuffList *blocks_a = parseCodeTxt("./solutions/sol_k.c");
  // db->buffs1->list[10]->name
  BuffList *blocks_b = parseCodeTxt("./solutions/sol_k.c");

  // for (int i = 0; i < blocks_b->len; i++) {
  //   printf("%s\n", blocks_b->list[i]->name);
  // }

  FILE *fd_blocks_db = fopen("./tmp/blocks.csv", "w");
  BuffDB *blocksDB = makeDB(blocks_a, blocks_b);
  writeDB(fd_blocks_db, blocksDB);
  fclose(fd_blocks_db);

  // Free mem.
  // for (int i = 0; i < progs->len; i++) {
  //  free(progs->list[i]->dat);
  //  free(progs->list[i]);
  //}
  // for (int i = 0; i < db->buffs1->len; i++) {
  //  free(db->dists[i]);
  //}
  // free(db->dists);
  // free(db);

  // return 0;
}
