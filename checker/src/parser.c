#include "../include/parser.h"
#include <clang-c/CXString.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  BuffList *buff_list;
  char *file_content;
} VisitorData;

void addSnippet(BuffList *bl, char *code, const char *name) {
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
    CXString name = clang_getCursorSpelling(cursor);
    if (!clang_Cursor_isNull(bodyCursor)) {
      BuffList *buffList = (BuffList *)client_data;
      CXSourceRange range = clang_getCursorExtent(bodyCursor);
      CXSourceLocation startLoc = clang_getRangeStart(range);
      CXSourceLocation endLoc = clang_getRangeEnd(range);
      unsigned int startOffset, endOffset;
      CXFile file;
      clang_getExpansionLocation(startLoc, &file, NULL, NULL, &startOffset);
      clang_getExpansionLocation(endLoc, NULL, NULL, NULL, &endOffset);

      CXString fileName = clang_getFileName(file);
      FILE *fp = fopen(clang_getCString(fileName), "r");
      if (fp) {
        fseek(fp, startOffset, SEEK_SET);
        char *code = (char *)malloc(endOffset - startOffset + 1);
        if (fread(code, 1, endOffset - startOffset, fp) ==
            endOffset - startOffset) {
          code[endOffset - startOffset] = '\0';
          addSnippet(buffList, code, clang_getCString(name));
        }
        free(code);
        fclose(fp);
      }
      clang_disposeString(fileName);
      clang_disposeString(name);
    }
    return CXChildVisit_Recurse;
  }
  return CXChildVisit_Continue;
}

BuffList *parseCodeTxt(const char *filename) {
  BuffList *buff_list = makeBuffList();
  CXIndex index = clang_createIndex(0, 0);
  CXTranslationUnit unit = clang_parseTranslationUnit(
      index, filename, NULL, 0, NULL, 0, CXTranslationUnit_None);

  if (unit == NULL) {
    fprintf(stderr, "Unable to parse translation unit. Quitting.\n");
    clang_disposeIndex(index);
    return NULL;
  }

  // Read file content
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("Failed to open file");
    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  long fileSize = ftell(fp);
  rewind(fp);
  char *fileContent = (char *)malloc(fileSize + 1);
  fread(fileContent, 1, fileSize, fp);
  fileContent[fileSize] = '\0';
  fclose(fp);

  // Pass VisitorData to the visitor function
  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(cursor, visitor, &buff_list);

  // Cleanup
  free(fileContent);
  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);

  return buff_list;
}
