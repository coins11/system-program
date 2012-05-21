#include<stdio.h>
#include<sys/select.h>

int mygetchar(int *c, struct timeval *timeout)
{
    fd_set rfds;
    int retval;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    retval = select(1, &rfds, NULL, NULL, timeout);
    if(retval == -1) return -1;
    if(retval) *c = getchar();
    return retval;
}

int main(void)
{
    struct timeval timeout = {5, 0};
    int c, retval = mygetchar(&c, &timeout);
    if(retval == -1) return -1;
    if(retval) printf("success to read\n");
    else printf("timeout\n");
    return 0;
}
