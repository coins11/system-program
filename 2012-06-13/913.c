#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<gdbm-ndbm.h>

int main(void){
    DBM *counterdb;
    datum key, value;
    char *query_string;
    int value_body;
    fputs("Content-Type: text/plain; charset=utf-8\n\n", stdout);
    query_string = getenv("QUERY_STRING");
    if(!query_string) query_string = "";
    counterdb = dbm_open("counterdb", O_RDWR | O_CREAT,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(!counterdb){
        fputs("database error.\n", stdout);
        return 0;
    }
    if(query_string[0] == '\0'){
        for(key = dbm_firstkey(counterdb); key.dptr != NULL;
                key = dbm_nextkey(counterdb)){
            value = dbm_fetch(counterdb, key);
            fwrite(key.dptr, 1, key.dsize, stdout);
            fputs(" = ", stdout);
            printf("%d\n", *(int *)value.dptr);
        }
    }
    else{
        key.dptr = query_string;
        key.dsize = strlen(query_string);
        value = dbm_fetch(counterdb, key);
        if(value.dptr)
            (*(int *)value.dptr)++;
        else{
            value_body = 1;
            value.dptr = (char *)&value_body;
            value.dsize = sizeof(int);
        }
        dbm_store(counterdb, key, value, DBM_REPLACE);
        printf("%d\n", *(int *)value.dptr);
    }
    dbm_close(counterdb);
    return 0;
}
