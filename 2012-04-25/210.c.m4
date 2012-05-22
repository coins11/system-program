define(
`exponential',
`ifelse(`$2', `0', `1', `eval(`$1 * 'exponential($1, decr(`$2')))')')

define(`allocation_unit', `12')

#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>

int drop_spaces(FILE *input)
{
    while(1){
        int ch = getc(input);
        if(ferror(input)) return -1;
        if(!isblank(ch)){
            ungetc(ch, input);
            return 0;
        }
    }
}

int read_command(char **r, FILE *input)
{
    char *buf = NULL;
    size_t bufsize = 0;
    while(1){
        int ch = getc(input);
        if(!(bufsize&
            decr(exponential(`2', allocation_unit)))){
            char *tmp = realloc(buf, bufsize+
                exponential(`2', allocation_unit));
            if(!tmp) goto ERR;
            buf = tmp;
        }
        if(ferror(input)) goto ERR;
        if(isalnum(ch) || isblank(ch)) buf[bufsize] = ch;
        else{
            ungetc(ch, input);
            for(; 1 <= bufsize && isblank(buf[bufsize-1]);
                bufsize--);
            buf[bufsize] = '\0';
            *r = buf;
            return 0;
        }
        bufsize++;
    }
ERR:
    free(buf);
    return -1;
}

int read_filename(char **r, FILE *input)
{
    char *buf = NULL;
    size_t bufsize = 0;
    while(1){
        int ch = getc(input);
        if(!(bufsize&
            decr(exponential(`2', allocation_unit)))){
            char *tmp = realloc(buf, bufsize+
                exponential(`2', allocation_unit));
            if(!tmp) goto ERR;
            buf = tmp;
        }
        if(ferror(input)) goto ERR;
        if(isalnum(ch)) buf[bufsize] = ch;
        else{
            ungetc(ch, input);
            buf[bufsize] = '\0';
            *r = buf;
            return 0;
        }
        bufsize++;
    }
ERR:
    free(buf);
    return -1;
}

int read_prefix(FILE *input)
{
    int ch = getc(input);
    if(ferror(input)) return -1;
    switch(ch){
        case '|': return 0;
        case '<': return 1;
        case '>': return 2;
        default: return -2;
    }
}

int main(void)
{
    char **commands = NULL, *input_name = NULL, *output_name = NULL;
    size_t commands_size = 0, i = 0;
    ungetc('|', stdin);
    while(1){
        int ch, prefix;
        char *str;
        if(drop_spaces(stdin) < 0) goto ERR;
        ch = getc(stdin);
        if(ferror(stdin)) goto ERR;
        if(feof(stdin) || ch == '\n') break;
        ungetc(ch, stdin);
        prefix = read_prefix(stdin);
        if(0 <= prefix && drop_spaces(stdin) < 0) goto ERR;
        switch(prefix){
        case 0:
            read_command(&str, stdin);
            if(!(commands_size&
                decr(exponential(`2', allocation_unit)))){
                char **tmp = realloc(commands,
                    sizeof(char *)*(commands_size+
                    exponential(`2', allocation_unit)));
                if(!tmp) goto ERR;
                commands = tmp;
            }
            commands[commands_size] = str;
            commands_size++;
            break;
        case 1:
        case 2:
            if((prefix == 1 ? input_name : output_name) ||
                read_filename(&str, stdin) < 0)
                    goto ERR;
            *(prefix == 1 ? &input_name : &output_name) = str;
            break;
        default:
            goto ERR;
        }
    }
    printf("commands:\n");
    for(i = 0; i != commands_size; i++){
        printf("%s\n", commands[i]);
        free(commands[i]);
    }
    input_name && printf("input: %s\n", input_name);
    output_name && printf("output: %s\n", output_name);
    free(commands);
    free(input_name);
    free(output_name);
    return 0;
ERR:
    for(i = 0; i != commands_size; i++) free(commands[i]);
    free(commands);
    free(input_name);
    free(output_name);
    fputs("An error has occurred.\n", stderr);
    return -1;
}
