#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>

#define PARENT 0
#define CHILD  1
#define INPUT  0
#define OUTPUT 1


void do_parent(int pipeline[2][2], const int count) {
   char ch;
   close(pipeline[PARENT][INPUT]);
   close(pipeline[CHILD] [OUTPUT]);
   for(int i = 0, idx = 0; i < count; ++i) {
      fputc(idx + 'a', stderr);
      idx = (idx+1) % 26;
      char tmp;
      read(pipeline[CHILD][INPUT], &tmp, 1);
      write(pipeline[PARENT][OUTPUT], &tmp, 1);
   }
   int stat;
   if(wait(&stat) < 0) {
      perror("wait");
      exit(EXIT_FAILURE);
   }
}

void do_child(int pipeline[2][2], const int count) {
   close(pipeline[PARENT][OUTPUT]);
   close(pipeline[CHILD] [INPUT]);
   for(int i = 0, idx = 0; i < count; ++i) {
      fputc(idx + '0', stderr);
      idx = (idx+1) % 10;
      char tmp = 't';
      write(pipeline[CHILD][OUTPUT], &tmp, 1);
      read(pipeline[PARENT][INPUT], &tmp, 1);
   }
}

int create_pipeline(int pipeline[2][2]) {
   if(pipe(pipeline[0]) < 0) return -1;
   if(pipe(pipeline[1]) < 0) {
      close(pipeline[0][0]);
      close(pipeline[0][1]);
      return -1;
   }
   return 1;
}

int main(int argc, char *argv[]) {
   if(argc < 2) return 1;
   int pipeline[2][2];
   if(create_pipeline(pipeline) < 0) {
      perror("pipe");
      exit(EXIT_FAILURE);
   }
   int pid;
   if((pid = fork()) < 0) {
      perror("fork");
      exit(EXIT_FAILURE);
   }
   const double count = (double)atoi(argv[1]) / 2.0;
   if(pid) do_parent(pipeline, (int)ceil(count));
   else do_child(pipeline, (int)floor(count));
   return 0;
}

