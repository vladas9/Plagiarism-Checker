#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addSnippet(BuffList *bl, char *code, char *name) {
  Buff *buff = makeBuff(code, name);
  pushBuff(bl, buff);
}

int cursorIsValid(enum CXCursorKind cursorKind) {
  switch (cursorKind) {
  case CXCursor_FunctionDecl:
  case CXCursor_StructDecl:
  case CXCursor_EnumDecl:
  case CXCursor_WhileStmt:
  case CXCursor_IfStmt:
  case CXCursor_DoStmt:
  case CXCursor_ForStmt:
    return 1;
  default:
    return 0;
  }
}

enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent,
                                CXClientData client_data) {
  enum CXCursorKind cursorKind = clang_getCursorKind(cursor);

  if (cursorIsValid(cursorKind)) {
    CXCursor bodyCursor = clang_getCursorDefinition(cursor);
    if (!clang_Cursor_isNull(bodyCursor)) {
      char *fileContent = (char *)client_data;
      if (fileContent) {
        CXSourceRange range = clang_getCursorExtent(bodyCursor);
        CXSourceLocation startLoc = clang_getRangeStart(range);
        CXSourceLocation endLoc = clang_getRangeEnd(range);
        unsigned int startOffset, endOffset;
        clang_getExpansionLocation(startLoc, NULL, NULL, NULL, &startOffset);
        clang_getExpansionLocation(endLoc, NULL, NULL, NULL, &endOffset);

        unsigned int length = endOffset - startOffset;
        char *code = (char *)malloc(length + 1);
        strncpy(code, fileContent + startOffset, length);
        code[length] = '\0';
        addSnippet(char *code) free(code);
      }
    }
    return CXChildVisit_Recurse;
  }
  return CXChildVisit_Continue;
}

int parse_source_code(const char *filename) {
  CXIndex index = clang_createIndex(0, 0);
  CXTranslationUnit unit = clang_parseTranslationUnit(
      index, filename, NULL, 0, NULL, 0, CXTranslationUnit_None);

  if (unit == NULL) {
    fprintf(stderr, "Unable to parse translation unit. Quitting.\n");
    clang_disposeIndex(index);
    return -1;
  }

  // Read the entire file once
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("Failed to open file");
    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
    return -1;
  }
  fseek(fp, 0, SEEK_END);
  long fileSize = ftell(fp);
  rewind(fp);
  char *fileContent = (char *)malloc(fileSize + 1);
  fread(fileContent, 1, fileSize, fp);
  fileContent[fileSize] = '\0';
  fclose(fp);

  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(cursor, visitor, fileContent);
  free(fileContent);
  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);
  return 0;
}
