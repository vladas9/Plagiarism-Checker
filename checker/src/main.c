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
  // TODO integrate
  // initialize_code_snippets();
  // if (parse_source_code(argv[1]) == 0) {
  //   for (CodeSnippet *snippet = get_code_snippets_head(); snippet != NULL;
  //        snippet = snippet->next) {
  //     printf("Snippet:\n%s\n", snippet->content);
  //   }
  // }

  int num = 0;
  Buff **progs = crawl_dir("./solutions/", &num);
  BuffDB *db = makeDB(num, progs, progs);

  symmDB(db);

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
