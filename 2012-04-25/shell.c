#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define BUFBLOCK_SIZE 4
typedef int (*CheckFun)(int);

struct command {
   char *str;
   struct command *next;
};

int is_skip_chars(int ch) {
   return ch != '\n' && ch != EOF && !isgraph(ch);
}

int is_command_chars(int ch) {
   return isalnum(ch) || ch == ' ' || ch == '-' || ch == '_';
}

int is_file_chars(int ch) {
   return isalnum(ch) || ch == '_';
}

void skip_whitespaces(void) {
   int ch;
   while(is_skip_chars(ch = getchar()));
   ungetc(ch, stdin);
}

char *read_string(CheckFun check) {
   char *str;
   if((str = malloc(BUFBLOCK_SIZE)) == NULL) {
      fputs("FATAL ERROR: Out of memory\n", stderr);
      return NULL;
   }
   int ch, idx = 0;
   while(check(ch = getchar())) {
      str[idx++] = ch;
      // バッファ拡張(失敗したらNULLを返す)
      if(idx % BUFBLOCK_SIZE) continue;
      char *tmp = str;
      if((str = realloc(str, idx + BUFBLOCK_SIZE)) == NULL) {
         fputs("FATAL ERROR: Out of memory\n", stderr);
         free(tmp);
         return NULL;
      }
   }
   ungetc(ch, stdin);
   // 有効な文字列が1文字も取れなかったらNULLを返す
   if(idx > 0) {
      str[idx] = '\0';
      return str;
   } else {
      fputs("SYNTAX ERROR: Invalid input\n", stderr);
      free(str);
      return NULL;
   }
}

// 行をパースしながらリストにappend、リダイレクト先を格納する
bool parse_line(struct command *prev, char **file_in, char **file_out) {
   skip_whitespaces();
   switch(getchar()) {
   case '|': // pipeline + command
      skip_whitespaces();
      struct command *cmd;
      if((cmd = malloc(sizeof(struct command))) == NULL) {
         fputs("FATAL ERROR: Out of memory\n", stderr);
         break;
      }
      if((cmd->str = read_string(is_command_chars)) == NULL) break;
      prev->next = cmd;
      return parse_line(cmd, file_in, file_out);
   case '<': // redirect_in filename
      skip_whitespaces();
      if(*file_in) {
         fputs("SYNTAX ERROR: Duplicated symbol '<'\n", stderr);
         break;
      }
      if((*file_in = read_string(is_file_chars)) == NULL) break;
      return parse_line(prev, file_in, file_out);
   case '>': // redirect_out filename
      skip_whitespaces();
      if(*file_out) {
         fputs("SYNTAX ERROR: Duplicated symbol '>'\n", stderr);
         break;
      }
      if((*file_out = read_string(is_file_chars)) == NULL) break;
      return parse_line(prev, file_in, file_out);
   case '\n': // 行末に到達したのでリストを締める
   case EOF :
      prev->next = NULL;
      return true;
   default:
      fputs("SYNTAX ERROR: Expected '|' or '>' or '<'\n", stderr);
   }
   // 失敗時。リストを締めて、入力バッファをクリアする
   prev->next = NULL;
   while(!iscntrl(getchar()));
   return false;
}

void exec(void) {
   struct command head;
   char *file_input = NULL, *file_output = NULL;
   head.next = NULL;
   ungetc('|', stdin);
   // パースに成功した時のみprintする
   if(parse_line(&head, &file_input, &file_output)) {
      for(struct command *p = head.next; p != NULL; p = p->next) {
         printf("command: %s\n", p->str);
      }
      printf("input:  %s\n", file_input ? file_input :"console");
      printf("output: %s\n", file_output? file_output:"console");
   }
   // メモリ解放
   while(head.next != NULL) {
      struct command *tmp = head.next;
      head.next = head.next->next;
      free(tmp);
   }
   free(file_input);
   free(file_output);
}

int main(void) {
   while(1) {
      printf("$ ");
      exec();
   }
   return 0;
}

