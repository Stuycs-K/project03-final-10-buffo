#ifndef SERVER_H
#define SERVER_H

struct file{
  char name[200];
  int w_file;
  struct file *nextfile;
};

struct message{
  char command[200];
  char filename[200];
  char text[1000];
};

void signal_handler(int signum, struct file *first);
void do_command(struct message m, struct message *answer);

#endif
