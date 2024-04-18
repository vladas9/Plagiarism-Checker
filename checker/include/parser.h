#ifndef PARSER_H
#define PARSER_H

#include "buff.h"
#include <clang-c/Index.h>

BuffList *parseCodeTxt(const char *filename);

#endif
