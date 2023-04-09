#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct section_header
{
    char name[15];
    int type;
    int offset;
    int ssize;
} SH;

void listare_nerec(const char *path, char string[20], int perm)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    dir = opendir(path);
    if(dir == NULL)
    {
        perror("ERROR\ninvalid directory path");
        return;
    }
    else printf("SUCCESS\n");
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            if(perm==1)
            {
                char filePath[1000];
                struct stat buff;
                snprintf(filePath, 1000, "%s/%s", path, entry->d_name);
                if(lstat(filePath, &buff) == 0)
                {
                    if((buff.st_mode & S_IWUSR))
                        printf("%s/%s\n", path, entry->d_name);
                }
            }
            else if(strlen(string)!=0)
            {
                int c=strlen(entry->d_name)-strlen(string);
                if(strcmp(entry->d_name+c,string)==0)
                {
                    printf("%s/%s\n", path, entry->d_name);
                }
            }
            else
            {
                printf("%s/%s\n", path, entry->d_name);
            }
        }
    }
    closedir(dir);
}

void listare_rec(const char *path, char string[20], int perm)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[1000];
    struct stat buff;

    dir = opendir(path);
    if(dir == NULL)
    {
        perror("ERROR\ninvalid directory path");
        return;
    }
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 1000, "%s/%s", path, entry->d_name);
            if(perm==1)
            {
                char filePath2[1000];
                struct stat bufff;
                snprintf(filePath2, 1000, "%s/%s", path, entry->d_name);
                if(lstat(filePath2, &bufff) == 0)
                {
                    if((bufff.st_mode & S_IWUSR))
                        printf("%s/%s\n", path, entry->d_name);
                }
            }
            else if(strlen(string)!=0)
            {
                int c=strlen(entry->d_name)-strlen(string);
                if(strcmp(entry->d_name+c,string)==0)
                {
                    printf("%s/%s\n", path, entry->d_name);
                }
            }
            else
            {
                printf("%s/%s\n", path, entry->d_name);
            }
            if(lstat(fullPath, &buff) == 0)
                if(S_ISDIR(buff.st_mode))
                {
                    listare_rec(fullPath, string, perm);
                }
        }
    }
    closedir(dir);
}

void parse_file(const char* path)
{
    int fd1 = -1;
    fd1 = open(path, O_RDONLY);
    if(fd1 == -1)
    {
        perror("ERROR\ncould not open file");
        return;
    }
    else
    {
        int size=0;
        char magic=0;
        lseek(fd1, -3, SEEK_END);
        read(fd1, &size, 2);
        read(fd1, &magic, 1);


        if(magic!='x')
        {
            printf("ERROR\nwrong magic");
            return;
        }
        lseek(fd1,-size, SEEK_CUR);
        int version=0, nr_sect=0;
        read(fd1, &version, 2);
        if(version<113 || version>154)
        {
            printf("ERROR\nwrong version");
            return;
        }
        read(fd1,&nr_sect, 1);
        if(nr_sect<2 || nr_sect>20)
        {
            printf("ERROR\nwrong sect_nr");
            return;
        }
        SH sectiuni[nr_sect];
        for(int i=0; i<nr_sect; i++)
        {
            sectiuni[i].type=0;
            sectiuni[i].ssize=0;
            sectiuni[i].offset=0;
            read(fd1, &sectiuni[i].name,14);
            sectiuni[i].name[14]='\0';
            read(fd1, &sectiuni[i].type,2);
            if(!(sectiuni[i].type==94 || sectiuni[i].type==29 || sectiuni[i].type==75 || sectiuni[i].type==21 || sectiuni[i].type==99 ||sectiuni[i].type==25 ||sectiuni[i].type==89))
            {
                printf("ERROR\nwrong sect_types");
                return;
            }

            read(fd1, &sectiuni[i].offset,4);
            read(fd1, &sectiuni[i].ssize,4);
        }
        printf("SUCCESS\n");
        printf("version=%d\n", version);
        printf("nr_sections=%d\n", nr_sect);

        for(int i=0; i<nr_sect; i++)
        {
            printf("section%d: %s %d %d\n", i+1, sectiuni[i].name, sectiuni[i].type, sectiuni[i].ssize);
        }

        close(fd1);
    }
}

int main(int argc, char **argv)
{
    if(argc >= 2)
    {
        if(strcmp(argv[1], "variant") == 0)
            printf("44982\n");
        else
        {
            char path[512];
            int perm=0;
            int rec=0;
            int list=0;
            int parse=0;
            char string[20]="";
            for(int i=1; i<argc; i++)
            {
                if(strcmp(argv[i], "list")==0)
                    list=1;
                if(strcmp (argv[i],"recursive")==0)
                    rec=1;
                if(strcmp (argv[i],"has_perm_write")==0)
                    perm=1;
                if(strncmp(argv[i], "name_ends_with=",15)==0)
                    strcpy(string,argv[i]+15);

                if(strncmp(argv[i],"path=",5)==0)
                    strcpy(path,argv[i]+5);

                if(strcmp(argv[i], "parse")==0)
                    parse=1;
            
            }
            if(list==1)
            {
                if(rec==0)
                    listare_nerec(path, string, perm);
                else
                {
                    printf("SUCCESS\n");
                    listare_rec(path, string, perm);
                }
            }
            else if(parse==1)
            {
                parse_file(path);
            }
        }
    }
    return 0;
}
