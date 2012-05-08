#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <utmpx.h>

struct utmplist
{
    struct utmplist *next;
    struct utmpx u;
};

struct utmplist *read_utmp(struct utmplist *head)
{
    struct utmplist *ulprev, *ulp;

    for (;;){
        struct utmpx *utmptmp;
        ulp = calloc(1, sizeof(struct utmplist));
        if (ulp == NULL){
            perror("calloc");
            exit(-1);
        }

        utmptmp = getutxent();
        if(!utmptmp){
            free(ulp);
            break;
        }
        memcpy(&(ulp->u), utmptmp, sizeof(struct utmpx));

        if (head == NULL) head = ulp;
        else ulprev->next = ulp;
        ulprev = ulp;
    }

    return head;
}

void write_utmp(FILE *fp, struct utmplist *head)
{
    struct utmplist *ulprev;
    struct utmplist *ulp = head;

    while (ulp){
        fprintf(fp, "%8.8s|%16.16s|%8.8s|%s", ulp->u.ut_user,
            ulp->u.ut_host, ulp->u.ut_line, ctime(&ulp->u.ut_tv.tv_sec));
        ulprev = ulp;
        ulp = ulp->next;
        free(ulprev);
    }
}

int main()
{
    while(1){
        struct utmplist *ulhead = NULL;
        setutxent();
        ulhead = read_utmp(ulhead);
        write_utmp(stdout, ulhead);
        printf("================================================================================\n");
        sleep(20);
    }
}
