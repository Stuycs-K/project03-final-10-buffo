#ifndef SERVER_H
#define SERVER_H

struct line{
  int lineind;
  char linecontents[200];
  struct line *nextline;
};

struct document{
  char name[200];
  int fileind;
  int w_file;
  struct document *nextdocument;
};

#endif
