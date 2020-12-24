#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
//#include <sys/types.h>
#include <sys/stat.h>

#define buf_size 100
#define buf_searching 10

int main(void)
{ 
	int fd = 0;
	int fd_1 = 0;
	int len = 0;
	char *temp = "hihi\n";
	char buf[buf_size] = {0,};

	char *hextemp[buf_searching];
	int hexflag = 0;
	char filepath[30] = "hexfile/";
	
	char contents[200][50] = {0,};
	int Eof = buf_size;
	int contents_line=-1, y=0;


	printf("Start iap test\n");
	printf("\n");
	printf("\n");

	DIR *dir_info;
 	struct dirent *dir_entry;


	dir_info = opendir("./hexfile");

	for(int i=0;i<buf_searching;i++)//copy to hextemp
	{
		dir_entry = readdir(dir_info);
		hextemp[i]=dir_entry->d_name;
	}

	for(int i=0;i<buf_searching;i++)//hex file finding
	{
		for(int j=1;j<20;j++)
		{
			if(hextemp[i][j]=='.' && hextemp[i][j+1]=='h' && hextemp[i][j+2]=='e' && hextemp[i][j+3]=='x')
			{
				hexflag = i;
				break;
			} 
		}
		if(hexflag != 0){ break;}
	}
	closedir(dir_info);

#if 1
	
	memcpy(&filepath[8],hextemp[hexflag],sizeof(filepath)); 
	printf(filepath);
	printf("\n\n");

	

	if((fd = open(filepath, O_RDONLY | O_CREAT , 0666)) == -1)//Searched file open 
	{
		perror("open failed");
		exit(1);
	}
	else
	{		
		read(fd, buf, buf_size);

		do
		{
			for(int i=0;i<Eof;i++)
			{
				if(buf[i]==':')
				{
					contents_line++;
					y=0;
					contents[contents_line][y]=buf[i];
				}
				else
				{
					y++;
					contents[contents_line][y]=buf[i];
				}
			}

			for(int i=0;i<buf_size;i++){buf[i]=0;}	//buf init
		}while((Eof=read(fd, buf, buf_size))>0);

		/* if((fd_1 = open("hexfile/writetest.txt", O_WRONLY | O_CREAT , 0666)) == -1)	//here  
		{ 
			perror("open failed"); 
			exit(1); 
		} 
		else {printf("file open success\n");}  */

	/*	printf("\n-------------------------\n");
		printf("%d", strlen(contents[1]));
		printf(contents[1]);
		printf("\n-------------------------\n"); */

		//parsing
		int temp = 0;
		char tmp_buf[21] = {0,};
		
		for(int i=0;i<44;i++)
		{
			contents[0][i]=contents[0][i+1];
			printf("%x\n", contents[0][i]);//
		}

		while(contents[0][temp] != 0x0A)
		{
			if(temp%2 == 0)	//number of ten
			{
				if((contents[0][temp]>=0x30) && (contents[0][temp]<=0x39))//0~9
				{
					tmp_buf[(temp/2)] = (contents[0][temp]-0x30)*0x10;
				}
				if((contents[0][temp]>=0x41) && (contents[0][temp]<=0x46))//A~F
				{
					tmp_buf[(temp/2)] = (contents[0][temp]-0x37)*0x10;
				}
			}

			else			//number of one
			{
				if((contents[0][temp]>=0x30) & (contents[0][temp]<=0x39))//0~9
				{
					tmp_buf[(temp/2)] += contents[0][temp]-0x30;
				}	
				if((contents[0][temp]>=0x41) && (contents[0][temp]<=0x46))//A~F
				{
					tmp_buf[(temp/2)] += contents[0][temp]-0x37;
				}
			}

			temp++;
		}

			printf("\n-------------------------\n");
			for(int i=0;i<22;i++)
			{
				printf("%d\n", tmp_buf[i]);//
			} 
			printf("\n-------------------------\n");

		#if 0
		for(int i=0;i<(contents_line+1);i++)
		{
			if(len=write(fd_1, contents[i], strlen(contents[i]))==-1) //아스키 파일로 되어있는 헥사 파일을 16진수로 바꿔서 다른 배열에 저장할 것
			{ 
				perror("!!!!!open failed"); 
				exit(1); 
			}
			else {printf("file write success\n");} 
		}
		#endif

		close(fd_1);


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

