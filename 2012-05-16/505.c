#include<stdio.h>
#include<time.h>
#include<sys/select.h>

int mygetchar(struct timeval *timeout)
{
    fd_set rfds;
    int retval;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    retval = select(1, &rfds, NULL, NULL, timeout);
    if(retval == -1) return -3;
    if(retval) return getchar();
    else return -2;
}

int main(void)
{
    struct timeval timeout = {5, 0};
    int c;
    time_t t;
    setvbuf(stdin, NULL, _IONBF, 0);
    time(&t);
    fputs(ctime(&t), stdout);
    c = mygetchar(&timeout);
    time(&t);
    fputs(ctime(&t), stdout);
    if(c == -1) return -1;
    if(c == -2) printf("timeout\n");
    else printf("success to read\n");
    return 0;
}
