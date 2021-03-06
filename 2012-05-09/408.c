#include<unistd.h>

pid_t fork_interactive(int *pipe_fd)
{
    int pipe_fd1[2], pipe_fd2[2];
    pid_t pid;
    if(pipe(pipe_fd1) < 0) return -1;
    if(pipe(pipe_fd2) < 0){
        close(pipe_fd1[0]);
        close(pipe_fd1[1]);
        return -1;
    }
    pid = fork();
    close(pipe_fd1[!pid]);
    close(pipe_fd2[!!pid]);
    pipe_fd[0] = pipe_fd1[!!pid];
    pipe_fd[1] = pipe_fd2[!pid];
    return pid;
}

ssize_t writeb(int fd, char c)
{
    char buf[1] = {c};
    return write(fd, buf, 1);
}

int main(int argc, char** argv)
{
    int pnum, onum, pcount = 0, ocount = 0, pipe_fd[4], status;
    pid_t pid;
    char buf[1];
    if(argc < 3) return -1;
    onum = atoi(argv[1]);
    pnum = atoi(argv[2]);
    if(onum < 0 || pnum < 1) return -1;
    for(; pcount != pnum; pcount++){
        if(pid = fork_interactive(pipe_fd+2)) break;
        pipe_fd[0] = pipe_fd[2];
        pipe_fd[1] = pipe_fd[3];
    }
    if(pid == -1) return -1;
    if(!pcount){
        for(; ocount != onum; ocount++){
            if(ocount&1){
                if(writeb(pipe_fd[2], 'A'+(ocount>>1)%26) < 0)
                    return -1;
                read(pipe_fd[3], buf, 1);
            }
            else if(writeb(1, '0'+(ocount>>1)%10) < 0) return -1;
        }
        close(pipe_fd[2]);
        close(pipe_fd[3]);
        if(writeb(1, '\n') < 0) return -1;
    }
    else{
        for(; read(pipe_fd[0], buf, 1) > 0; ocount++){
            if(ocount%(pnum+1-pcount)){
                if(writeb(pipe_fd[2], buf[0]) < 0) return -1;
                read(pipe_fd[3], buf, 1);
            }
            else if(writeb(1, buf[0]) < 0) return -1;
            if(writeb(pipe_fd[1], buf[0]) < 0) return -1;
        }
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        if(pcount != pnum){
            close(pipe_fd[2]);
            close(pipe_fd[3]);
        }
    }
    waitpid(pid, &status, 0);
    return 0;
}
