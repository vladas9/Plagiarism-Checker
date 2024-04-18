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

  BuffList *blocks_a = parseCodeTxt(db->buffs1->list[10]->name);
  BuffList *blocks_b = parseCodeTxt(db->buffs1->list[10]->name);
  for (int i = 0; i < blocks_a->len; i++) {
    printf("%s\n", blocks_a->list[i]->name);
  }
  BuffDB *blocksDB = makeDB(blocks_a, blocks_b);

  writeDB(stdout, blocksDB);

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
