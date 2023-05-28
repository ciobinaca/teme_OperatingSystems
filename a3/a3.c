#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    int f= -1, f2=-1;
    char con[8]="CONNECT";
   // char req[250]="";
    char t='$';
    //create pipe
    unlink("RESP_PIPE_44982");
    if(mkfifo("RESP_PIPE_44982", 0600) != 0)
    {
        perror("ERROR\ncannot create the response pipe");
        return 1;
    }


    f= open("REQ_PIPE_44982", O_RDONLY);
    if(f == -1)
    {
        perror("ERROR\ncannot open the request pipe");
        return 1;
    }
//    int i=0;
//    while(read(f,&req[i],sizeof(req[i])))
//     i++;
//
    close(f);
    //open, write and close fifo
    f2= open("RESP_PIPE_44982", O_WRONLY);
    if(f2==-1)
    {
        perror("ERROR\ncannot open the response pipe");
        return 1;
    }
    for(int i=0; i<strlen(con); i++)
    {
        write(f2, &con[i], sizeof(con[i]));
    }
    write(f2, &t, 1);
     printf("SUCCES");
    close(f2);
    unlink("RESP_PIPE_44982");
    return 0;
}
