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
#define FileSize 400	//나중에 파일사이즈를 알아내서 배열을 선언하는 식으로 바꿀 것
#define Num_data 32		//데이터 개수 최대 128까지
#define start_point = 0x9000

int main(void)
{ 
	int fd = 0;
	int fd_1 = 0;
	int len = 0;
	
	char buf[buf_size] = {0,};

	char *hextemp[buf_searching];
	int hexflag = 0;
	char filepath[30] = "hexfile/";
	
	char contents[FileSize][50] = {0,};
	int Eof = buf_size;
	int contents_line=-1, y=0;

	int temp = 0;
	unsigned char tmp_buf[21] = {0,};

	unsigned int checksum_tmp = 0;//지울수도 있음
	unsigned char send_buf[Num_data+9] = {0,};//9: 2= command, 4=address, 1=checksum of address, 1=number of data, Num_data, 1=chechsum of data

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
				}
				else
				{
					contents[contents_line][y]=buf[i];
					y++;
				}
			}

			for(int i=0;i<buf_size;i++){buf[i]=0;}	//buf init
		}while((Eof=read(fd, buf, buf_size))>0);
		
		#if 0
		if((fd_1 = open("hexfile/writetest.txt", O_WRONLY | O_CREAT , 0666)) == -1)	//here  
		{ 
			perror("open failed"); 
			exit(1); 
		} 
		else {printf("file open success\n");}
		#endif
		printf("%d\n", contents_line);//	마지막 배열 이후 끝점을 알아낼 것
		printf("%d\n", contents[190][0]);//	마지막 배열 이후 끝점을 알아낼 것
		
		//contents_line = 1;
		//parsing!!!!!!!!!!!!!!
		for(int i=0;i<=contents_line;i++)
		{
			while(contents[i][temp] != 0x0A)
			{
				if(temp%2 == 0)	//number of ten
				{
					if((contents[i][temp]>=0x30) && (contents[i][temp]<=0x39))//0~9
					{
						tmp_buf[(temp/2)] = (contents[i][temp]-0x30)*0x10;
					}
					if((contents[i][temp]>=0x41) && (contents[i][temp]<=0x46))//A~F
					{
						tmp_buf[(temp/2)] = (contents[i][temp]-0x37)*0x10;
					}
				}

				else			//number of one
				{
					if((contents[i][temp]>=0x30) & (contents[i][temp]<=0x39))//0~9
					{
						tmp_buf[(temp/2)] += contents[i][temp]-0x30;
					}	
					if((contents[i][temp]>=0x41) && (contents[i][temp]<=0x46))//A~F
					{
						tmp_buf[(temp/2)] += contents[i][temp]-0x37;
					}
				}
			temp++;
			}
			temp = 0;
			
			//hexfile data checksum check
			for(int i=0;i<4+tmp_buf[0];i++)
			{
				checksum_tmp += tmp_buf[i];
			}
			//4: 1=number of data, 2 = address of data, 1 = type of data (hexfile's structure)
			if((unsigned char)((~checksum_tmp)+1) != tmp_buf[tmp_buf[0]+4])//two's complement checksum check
			{
				perror("hexfile checksum error");
				exit(1);
			}
			else
			{
				checksum_tmp = 0;
			}
			
			/* printf("\n-------------------------\n");
			for(int i=0;i<22;i++)
			{
				printf("%1X ", tmp_buf[i]);//
				tmp_buf[i] = 0;
			} 
			printf("\n-------------------------\n"); */
			
			//unsigned char hex_address[5] = {0,};//0~3 = address, 4 = checksum(XOR)
			unsigned int test_ssingmin = 0xffffffff;
			

			//send_buf[0]=0x31;
			//send_buf[1]=0xCE;	//complement of send_buf[0]
			
			//memcpy(&test_ssingmin, send_buf, 4);
			test_ssingmin++;
			//*send_buf = 0x1111;
			printf("\n%X\n ", test_ssingmin);//헥사파일 정리해서 통신 버퍼로 보내기 'receive 데이터'처럼
			
			/* printf("\n-------------------------\n");
			for(int i=0;i<22;i++)
			{
				printf("%1X %1X ",0x31, 0xce );//
				printf("%1X ", tmp_buf[i]);//
				tmp_buf[i] = 0;
			} 
			printf("\n-------------------------\n"); */


		}
		

		
		#if 0
		for(int i=0;i<(contents_line+1);i++)
		{
			if(len=write(fd_1, contents[i], strlen(contents[i]))==-1)
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

