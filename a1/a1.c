#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
typedef struct
{
    char SECT_NAME[16];
    int SECT_TYPE;
    int SECT_OFFSET;
    int SECT_SIZE;
}SECTION_HEADERS;
void listRec(const char *path, int size_greater, char* name_ends_with, int i)
{
    DIR *dir=NULL;
    struct dirent *entry=NULL;
    char fullPath[512];
    struct stat statbuf;
   
    dir = opendir(path);
    if(dir==NULL)
    {
	printf("ERROR\nInvalid directory path");
	return;       
    }
    if(i==0)
    {
    	printf("SUCCESS\n");
    	i++;
    }	
    while((entry =readdir(dir)) != NULL)
    {
	if((strcmp(entry->d_name, ".")!=0) && (strcmp(entry->d_name, "..")!=0)) 
	{
	    snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
	    if(lstat(fullPath, &statbuf) == 0)
	    {
	        
	        if(size_greater==0 && name_ends_with[0]=='\0')
	       	    printf("%s\n", fullPath);
	        if(S_ISREG(statbuf.st_mode))
	        {
	      	    if(size_greater!=0)
	      	    	{
	      	    	    if(statbuf.st_size>size_greater)
	      	    	        {
	      	    	            printf("%s\n", fullPath);
	      	    	        }
	      	    	}
	      	    if(name_ends_with[0]!='\0')
	      	    	{
	      	    	    if(strcmp(entry->d_name+strlen(entry->d_name)-strlen(name_ends_with), name_ends_with) == 0 )
	      	    	        printf("%s\n", fullPath);
	      	    	}
	        }	
	        if(S_ISDIR(statbuf.st_mode))
	        {
	            if(name_ends_with[0]!='\0')
	      	    	{
	      	    	    if(strcmp(entry->d_name+strlen(entry->d_name)-strlen(name_ends_with), name_ends_with) == 0 )
	      	    	        printf("%s\n", fullPath);
	      	    	}
	            listRec(fullPath, size_greater, name_ends_with, i);     
	        }
	    }
	}   
    }
    closedir(dir);
}

void listItr(const char *path, int size_greater, char* name_ends_with, int i)
{
    DIR *dir=NULL;
    struct dirent *entry=NULL;
    char fullPath[512];
    struct stat statbuf;
   
    dir = opendir(path);
    if(dir==NULL)
    {
	printf("ERROR\nInvalid directory path");
	return;       
    }
    if(i==0)
    {
    	printf("SUCCESS\n");
    	i++;
    }	
    while((entry =readdir(dir)) != NULL)
    {
	if((strcmp(entry->d_name, ".")!=0) && (strcmp(entry->d_name, "..")!=0)) 
	{
	    snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
	    if(lstat(fullPath, &statbuf) == 0)
	    {
	        if(size_greater==0 && name_ends_with[0]=='\0')
	       	    printf("%s\n", fullPath);
	        if(S_ISREG(statbuf.st_mode))
	        {
	      	    if(size_greater!=0)
	      	    	{
	      	    	    if(statbuf.st_size>size_greater)
	      	    	        {
	      	    	            printf("%s\n", fullPath);
	      	    	        }
	      	    	}
	      	    if(name_ends_with[0]!='\0')
	      	    	{
	      	    	    if(strcmp(entry->d_name+strlen(entry->d_name)-strlen(name_ends_with), name_ends_with) == 0 )
	      	    	        printf("%s\n", fullPath);
	      	    	}
	        }	
	        if(S_ISDIR(statbuf.st_mode))
	        {
	            if(name_ends_with[0]!='\0')
	      	    	{
	      	    	    if(strcmp(entry->d_name+strlen(entry->d_name)-strlen(name_ends_with), name_ends_with) == 0 )
	      	    	        printf("%s\n", fullPath);
	      	    	}
	        }
	    }
	}   
    }
    closedir(dir);
}

void parseFunction(char* path)
{
    int fisier= open(path, O_RDONLY);
    int NO_OF_SECTIONS=0;
    int VERSION=0;
    char MAGIC='\0';
    int HEADER_SIZE=0;
    if(fisier == -1)
    	return;

    lseek(fisier, -1 , SEEK_END);
    read(fisier, &MAGIC, 1);
    if(MAGIC!='Z')
    {    
        printf("ERROR\nwrong magic");
    	return;
    }
    
    
    lseek(fisier, -3, SEEK_END);
    read(fisier, &HEADER_SIZE, 2);
    
    lseek(fisier, -HEADER_SIZE, SEEK_END);
    read(fisier, &VERSION, 1);
    if((VERSION < 107) || (VERSION > 160))
    {    
        printf("ERROR\nwrong version");
        return;
    }
    read(fisier, &NO_OF_SECTIONS, 1);
    if((NO_OF_SECTIONS<6) || (NO_OF_SECTIONS>17))
    {
        printf("ERROR\nwrong sect_nr");
        return;
    }
    SECTION_HEADERS *SH=(SECTION_HEADERS*)malloc(sizeof(SECTION_HEADERS)*NO_OF_SECTIONS);
    for(int i=0; i<NO_OF_SECTIONS; i++)
    {
        read(fisier, SH[i].SECT_NAME, 15);
        SH[i].SECT_NAME[15]='\0';
        read(fisier, &SH[i].SECT_TYPE, 4);
    	if(SH[i].SECT_TYPE!=72 && SH[i].SECT_TYPE!=73 && SH[i].SECT_TYPE!=75 && SH[i].SECT_TYPE!=79 && SH[i].SECT_TYPE!=47 && SH[i].SECT_TYPE!=10)
    	{
    	    printf("ERROR\nwrong sect_types");
            return;
        }
        read(fisier, &SH[i].SECT_OFFSET, 4);
        read(fisier, &SH[i].SECT_SIZE, 4);
    }
    
    
    free(SH);
        
}


int main(int argc, char **argv)
{
    char path[200];
    int i=0;
    int OK=0;
    int size_greater=0;
    char name_ends_with[200]={0};
    if(argc >= 2)
    {
        if(strcmp(argv[1], "variant") == 0)
        {
            printf("57022\n");
        }
        else
        {
            for(int j=1; j<argc; j++)
            {
                 if(strcmp(argv[j], "list") == 0)
                 {
            	     OK=1;
                 }
                 if(strcmp(argv[j], "recursive") == 0)
                 {
            	     OK=2;
                 }
                 if(strncmp(argv[j], "path=", 5) == 0)
                 {
            	     sscanf(argv[j], "path=%s", path);
                 }
                 if(strncmp(argv[j], "size_greater=", 13)==0)
                 {
                     sscanf(argv[j], "size_greater=%d", &size_greater);
                 }
                 if(strncmp(argv[j], "name_ends_with=", 15)==0)
                 {
                     sscanf(argv[j], "name_ends_with=%s", name_ends_with);
                 }
                 if(strcmp(argv[j], "parse") == 0)
                 {
                     OK=3;
                 }
            }
            if(OK==2)
            { 
                 listRec(path, size_greater, name_ends_with, i);
            }
            if(OK==1)
            {
                 listItr(path, size_greater, name_ends_with, i);
            }
            
            if(OK==3)
            {
                 parseFunction(path);
            }
        }
    }
 
    return 0;
}
