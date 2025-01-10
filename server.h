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

void signal_handler(int signum);
void do_command(struct message m, struct message *answer);

#endif
