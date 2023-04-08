#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

void listRec(const char *path)
{
    DIR *dir=NULL;
    struct dirent *entry=NULL;
    char fullPath[512];
    struct stat statbuf;
   
    dir = opendir(path);
    if(dir==NULL)
    {
	perror("Invalid directory path");
	return;       
    }
    while((entry =readdir(dir)) != NULL)
    {
	if((strcmp(entry->d_name, ".")!=0) && (strcmp(entry->d_name, "..")!=0)) 
	{
	    snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
	    if(lstat(fullPath, &statbuf) == 0)
	    {
	        printf("%s\n", fullPath);
	        if(S_ISDIR(statbuf.st_mode))
	        {
	            listRec(fullPath);     
	        }
	    }
	}   
    }
    closedir(dir);
}


int main(int argc, char **argv)
{
    const char *path = "test_root/test_dir/";
    if(argc >= 2)
    {
        if(strcmp(argv[1], "variant") == 0)
        {
            printf("57022\n");
        }
    }
    listRec(path);
 
    return 0;
