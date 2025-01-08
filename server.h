#ifndef SERVER_H
#define SERVER_H

struct line{
  int lineind;
  char linecontents[200];
  struct line *nextline;
};

struct file{
  char name[200];
  int fileind;
  int w_file;
  struct file *nextfile;
};

struct message{
  char command[200];
  char text[200];
};

#endif
