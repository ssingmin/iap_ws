#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
//#include <sys/types.h>
#include <sys/stat.h>

#define buf_size 10


int main(void)
{ 
	int fd = 0;
	int len = 0;
	char *temp = "hihi\n";
	char buf[buf_size];

	char *hexfinder[10];
	int hexfinderflag = 0;

	printf("Start iap test\n");
	printf("\n");
	printf("\n");

	DIR *dir_info;
 	struct dirent *dir_entry;

	//mkdir("test_A", 0755);

	dir_info = opendir("./hexfile");

/*
	while(dir_entry = readdir(dir_info))
	{		
	 	printf("%s\n", dir_entry->d_name);
		printf("%lx\n", dir_entry->d_ino);
		
	}
*/	

	for(int i=0;i<4;i++)//copy to hexfinder
	{
		dir_entry = readdir(dir_info);
		hexfinder[i]=dir_entry->d_name;
	}


	for(int i=0;i<4;i++)
	{
		for(int j=1;j<20;j++)
		{
			if(hexfinder[i][j]=='.' && hexfinder[i][j+1]=='h' && hexfinder[i][j+2]=='e' && hexfinder[i][j+3]=='x')
			{
				printf("here!!!!!\n");
				printf("%s\n", hexfinder[i]);
				hexfinderflag = 1;
				break;
			}
			
		}
		if(hexfinderflag == 1){break; hexfinderflag = 0;}
	}
	closedir(dir_info);
	
#if 0
	if((fd = open("hexfile/test.txt", O_RDONLY | O_CREAT , 0666)) == -1)//here 
	{
		perror("open failed");
		exit(1);
	}
	else
	{
		read(fd, buf, buf_size);
		puts(buf);
		close(fd);
		//printf("file open success\n");
	}
#endif
	

#if 0
	if(len=write(fd, temp, strlen(temp))==-1)
	{
		perror("!!!!!open failed");
		exit(1);
	}
	else
	{
		printf("file write success\n");
	}
	
		close(fd);
#endif
	printf("\n");
	printf("\n");
	printf("end\n");

	return 0;
}

