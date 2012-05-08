#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <utmpx.h>
#include <openssl/md5.h>

#define POLLING_INTERVAL 20

typedef struct node_ {
        struct utmpx *data;
        struct node_ *next;
} node;

node* append(node *next, const struct utmpx *up) {
        node *n = malloc(sizeof(node));
        n->data = malloc(sizeof(struct utmpx));
        memcpy(n->data, up, sizeof(struct utmpx));
        n->next = next;
        return n;
}

void delutxents(node *head) {
        node *p = head;
        while(p) {
                node *tmp = p->next;
                free(p->data);
                free(p);
                p = tmp;
        }
}

int is_target_type(const short type) {
        return type == LOGIN_PROCESS || type == USER_PROCESS;
}

node* getutxents(unsigned char *md5) {
        // init md5 context
        MD5_CTX ctx;
        MD5_Init(&ctx);
        // load utmpx entries
        struct utmpx *up;
        node *head = NULL;
        setutxent();
        while((up = getutxent()) != NULL) {
                if(!is_target_type(up->ut_type)) continue;
                MD5_Update(&ctx, up, sizeof(struct utmpx));
                head = append(head, up);
        }
        endutxent();
        // finalize md5 context
        MD5_Final(md5, &ctx);
        return head;
}

void print_utxents(const node *head) {
        // print current time
        time_t now;
        time(&now);
        printf("updated %s", ctime(&now));
        // print utmpx entries
        const node *p = head;
        while(p) {
                printf("%8.8s|%16.16s|%8.8s|%s",
                        p->data->ut_user, p->data->ut_host,
                        p->data->ut_line, ctime(&p->data->ut_tv.tv_sec));
                p = p->next;
        }
}

int main(void) {
        unsigned char md5_prev[MD5_DIGEST_LENGTH] = {0};
        while(1) {
                unsigned char md5[MD5_DIGEST_LENGTH];
                node *head = getutxents(md5);
                // check whether entries has been updated
                if(!memcmp(md5, md5_prev, MD5_DIGEST_LENGTH)) {
                        delutxents(head);
                        sleep(POLLING_INTERVAL);
                        continue;
                }
                // print entries and update md5_prev
                memcpy(md5_prev, md5, MD5_DIGEST_LENGTH);
                print_utxents(head);
                delutxents(head);
                sleep(POLLING_INTERVAL);
        }
        return 0;
}

