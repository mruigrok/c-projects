#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <libgen.h>

/*-------Function Declaration-------*/

char *get_user_id(uid_t);
char *get_group_id(uid_t);
void print_mode_as_string(struct stat);
void report_file_attributes(char *);
void report_file_attributes_oneline(char *);
int is_directory(const char *);
void traverse_directory(const char *);

/*-----------Main Program-----------*/

/*
 Program that returns file attributes if a file is passed as an argument, 
or traverses similar to the command ls -lRa on a Linux/Unix system
*/

int main(int argc, char *argv[])
{
	/*
	Get the argument passed into the program and store in entry.
    If the entry is a valid file or directory, it will print the attributes 
    for a file or traverse if it is a directory, and return otherwise
    */

    char *entry = argv[1];
    struct stat sb;

    if(stat(entry, &sb) == -1)
    {
        perror("stat error");
    }
   
    if (S_ISDIR(sb.st_mode))
    {
        traverse_directory(entry);
    }
    else if(S_ISREG(sb.st_mode))
    {	
        report_file_attributes(entry);
    }
    else
    {
        printf("Returning....\n");
        return (1);
    }

}//end of main

/*--------Function Definition-------*/

char* get_user_id(uid_t uid)
{
	/*Convert a uid_t into the user ID string*/
	struct passwd *pws;
	pws = getpwuid(uid);
	if(pws->pw_name != NULL)
	{
		return pws->pw_name;
	}
	else
	{
		return "Null";
	}	
}

char* get_group_id(uid_t gid)
{
	/*Convert a uid_t into the group ID string*/
	struct group *grp;
	grp = getgrgid(gid);
	if(grp->gr_name != NULL)
	{
		return grp->gr_name;
	}
	else
	{
		return "Null";
	}	
}

void print_mode_as_string(struct stat sb)
{
	/*Print to output the mode of the file as a string*/
	printf( (S_ISDIR(sb.st_mode)) ? "d" : "-");
    printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
    printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
    printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
    printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
    printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
    printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
    printf( (sb.st_mode & S_IROTH) ? "r" : "-");
    printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
    printf( (sb.st_mode & S_IXOTH) ? "x" : "-");
}

void report_file_attributes(char *filename)
{
	/*Since filename could be a link call lstat. If it is a link, lstat
	will return the attributes of the link and if it is not it will act as
	a regular stat call. From the outline of the lab this is the behavior
	we want. If lstat is unsucessful, exit the function*/
	
	struct stat sb;
	if(lstat(filename, &sb) == -1)
	{
		perror("lstat unsucessful\n");
		return;
	}
	
	/*Convert the date into a readable format and then print 
	the information about the file to the screen. If it is a 
	link print linkname -> , and if it is a file print filename
	and then return*/
		
	char date[36];
	strftime(date, 36, "%d.%m.%Y %H:%M:%S", localtime(&sb.st_mtime));
	printf("\n\n");
	printf("Mode (permissions): \t\t"); print_mode_as_string(sb);
	printf("\nNumber of Hard Links:\t\t%d\n", (int)sb.st_nlink);
	printf("Owner's Name: \t\t\t%s\n", get_user_id(sb.st_uid));
	printf("Group name: \t\t\t%s\n", get_group_id(sb.st_gid));
	printf("Size in bytes: \t\t\t%d bytes\n", (int)sb.st_size);
	printf("Size in blocks: \t\t%d blocks\n", (int)sb.st_blocks);
	printf("Last modification time (D/T): \t%s\n", date);
		
	if(S_ISLNK(sb.st_mode) == 1)
	{
		printf("File name: \t\t\t%s -> linked file\n", basename(filename));
		printf("\n\t      This file is a link!\n\n");	
	}
	else if(S_ISREG(sb.st_mode) == 1)
	{
		printf("File name: \t\t\t%s\n", basename(filename));
		printf("\n\n");	
	}
}

void report_file_attributes_oneline(char *filename)
{
	/*Same functionality as report_file_attributes, but printing on one line
	and it will print if filename is a directory as well*/
	
	struct stat sb;
	if(lstat(filename, &sb) == -1)
	{
		perror("lstat unsucessful\n");
		return;
	}

	/*Convert the date into a readable format and then print 
	the information about the file to the screen and return*/

	char date[36];
	strftime(date, 36, "%d.%m.%Y %H:%M:%S", localtime(&sb.st_mtime));
	print_mode_as_string(sb);
	printf("  %2d  ", (int)sb.st_nlink);
	printf("%s  ", get_user_id(sb.st_uid));
	printf("%s  ", get_group_id(sb.st_gid));
	printf("%6d  ", (int)sb.st_size);
	printf("%s  ", date);

	if(S_ISLNK(sb.st_mode) == 1)
	{
		printf("%s -> linked file\n", basename(filename));
	}
	else if(S_ISREG(sb.st_mode) == 1 || S_ISDIR(sb.st_mode) == 1)
	{
		printf("%s\n", basename(filename));
	}
}

int is_directory(const char *path)
{
	/*Check to see if the given path is a directory, return 0 if false*/
	struct stat sb;
	if (stat(path, &sb) != 0)
	{
		perror("couldn't open directory");
		return 0;
	}
	return S_ISDIR(sb.st_mode);
}

void traverse_directory(const char *dir)
{
	/*Try to open the directory 'dir' passed as a parameter, return if failure*/

	DIR *stream;
	struct dirent *entry;
	
	if(!(stream = opendir(dir)))
	{
		perror("couldn't open directory");
		return;
	}

	printf("%s\n", dir);

	/*Displaying the attributes for the all of the files/dirs/links in the directory
	on first call, then going through again and recursively traversing sub-directories.
	Excluding '.' and '..' entries */

	while((entry = readdir(stream)) != NULL)
	{
		if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name, "..") == 0 || entry->d_name[0] == '.')
		{
			continue;
		}
		/*Get the path and then display the file attributes*/
		char new[strlen(dir) + strlen(entry->d_name) + 1];
		strcpy(new, dir);
		strcat(new,"/");
		strcat(new, entry->d_name);
		report_file_attributes_oneline(new);
	}

	/*Rewind and go back through. First get the directory name, double check
	it is a directory and traverse that directory*/
	
	printf("\n");
	rewinddir(stream);

	while ((entry = readdir(stream)) != NULL)
	{	
		if(entry->d_type == DT_DIR)
		{
			if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name, "..") == 0 || entry->d_name[0] == '.')
			{
				continue;
			}
			else
			{
				/*Get the name of the path and traverse if it is a directory,
				after double checking with is_directory*/
				char new[strlen(dir) + strlen(entry->d_name) + 1];
				strcpy(new, dir);
				strcat(new,"/");
				strcat(new, entry->d_name);
				if(is_directory(new))
				{ 
					traverse_directory(new);
				}
			}	
		}
	}
	
	/*When the files are all printed and the sub-directories are traverssed,
	close the stream and then return from the function*/
	closedir (stream);	
	
}


