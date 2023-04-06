#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int listare_nerec(const char *path, char string[20], int perm)
 {
 DIR *dir = NULL;
 struct dirent *entry = NULL;
 dir = opendir(path);
 if(dir == NULL) {
   perror("ERROR\ninvalid directory path");
   return -1;
   }
   else printf("SUCCESS\n");
 while((entry = readdir(dir)) != NULL) {
 if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
  { 
     if(perm==1)
     { 
       char filePath[512];
       struct stat buff;
       snprintf(filePath, 512, "%s/%s", path, entry->d_name);
       if(lstat(filePath, &buff) == 0){
        if((buff.st_mode & S_IWUSR))
        printf("%s/%s\n", path, entry->d_name);
        }
     }
     else if(strlen(string)!=0)
     {
       int c=strlen(entry->d_name)-strlen(string);
       if(strcmp(entry->d_name+c,string)==0){
        printf("%s/%s\n", path, entry->d_name);
        }
        }
        else{
            printf("%s/%s\n", path, entry->d_name);
            }
   }
  }
 closedir(dir);
 return 0;
 }
 
 int listare_rec(const char *path, char string[20], int perm)
 {
 DIR *dir = NULL;
 struct dirent *entry = NULL;
 char fullPath[512];
 struct stat buff;

 dir = opendir(path);
    if(dir == NULL) {
     perror("ERROR\ninvalid directory path");
    return -1;
    }
    while((entry = readdir(dir)) != NULL) {
       if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
           snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
              if(perm==1){ 
               char filePath2[512];
               struct stat bufff;
               snprintf(filePath2, 512, "%s/%s", path, entry->d_name);
                if(lstat(filePath2, &bufff) == 0){
                 if((bufff.st_mode & S_IWUSR))
                  printf("%s/%s\n", path, entry->d_name);
                }
              }
     else if(strlen(string)!=0)
     {
       int c=strlen(entry->d_name)-strlen(string);
         if(strcmp(entry->d_name+c,string)==0){
           printf("%s/%s\n", path, entry->d_name);
         }
        }
        else{
            printf("%s/%s\n", path, entry->d_name);
            }
      if(lstat(fullPath, &buff) == 0)      
       if(S_ISDIR(buff.st_mode)){
         listare_rec(fullPath, string, perm);
          }
           }
       }
       closedir(dir);
       return 0;
    }
 

int main(int argc, char **argv){
    if(argc >= 2){
        if(strcmp(argv[1], "variant") == 0)
            printf("44982\n");
        else if(strcmp(argv[1], "list")==0)
            {
             char* path=calloc(512,sizeof(char));
             int perm=0;
             int rec=0;
             char string[20];
             for(int i=2;i<argc;i++)
             {if(strcmp (argv[i],"recursive")==0)
                rec=1;
             if(strcmp (argv[i],"has_perm_write")==0)
                perm=1;   
              
             if(strncmp(argv[i], "name_ends_with=",15)==0)
                  strcpy(string,argv[i]+15);           
                 
              if(strncmp(argv[i],"path=",5)==0)
                  
                   strcpy(path,argv[i]+5);
                       
            }
            if(rec==0)
            { 
              listare_nerec(path, string, perm);
            }
            else { if(listare_rec(path, string, perm)!=-1)
              printf("SUCCESS\n");
              listare_rec(path, string, perm);
            }
            
        }
}

    return 0;
}
