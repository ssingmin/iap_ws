#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
//#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h> 


#define buf_size 100
#define buf_searching 10
#define FileSize 400	//나중에 파일사이즈를 알아내서 배열을 선언하는 식으로 바꿀 것
#define Num_data 32		//데이터 개수 최대 128까지
#define start_point = 0x9000

int main(int argc, char *argv[])	//추후에 인수 받아서 데이터 비트(ex. 현재는 32바이트, 나중에는 128바이트 전송으로 업그레이드 할 것)
{ 	
	int fd = 0;
	int fd_1 = 0;
	int fd_2 = 0;
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

	unsigned char hex_address[5] = {0,};//0~3 = address, 4 = checksum(XOR)

					
	int c = 0;
	unsigned char BootAck = 0;


	printf("Start iap test\n");
	printf("\n");
	printf("\n");

	DIR *dir_info;
 	struct dirent *dir_entry;

	struct termios newtio; 


	fd_2 = open( "/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK ); 

	memset( &newtio, 0, sizeof(newtio) ); 
	newtio.c_cflag |= CS8;      // 데이터 비트가 8bit  
	newtio.c_cflag = B9600;   // 통신 속도 115200  
	newtio.c_cflag |= CLOCAL;   // 외부 모뎀을 사용하지 않고 내부 통신 포트 사용  
	newtio.c_cflag |= CREAD;    // 쓰기는 기본, 읽기도 가능하게  
	newtio.c_cflag |= PARENB;   // parity 비트는 even
	newtio.c_iflag = 0;       
	newtio.c_oflag = 0; 
	newtio.c_lflag = 0; 
	newtio.c_cc[VTIME] = 0;  
	newtio.c_cc[VMIN] = 1;  

	tcflush (fd_2, TCIFLUSH );
	tcsetattr(fd_2, TCSANOW, &newtio );   // 포트에 대한 통신 환경을 설정합니다. 


	usleep(6);	
	send_buf[0]=0x7f;
	write( fd_2, send_buf, 1);//transmit data to ttyUSB0
	
	usleep(6);
	send_buf[0]=0x00;
	send_buf[1]=0xff;
	write( fd_2, send_buf, 2);//transmit data to ttyUSB0


	dir_info = opendir("./hexfile");


	for(int i=0;i<buf_searching;i++)//copy to hextemp
	{	
		*(hextemp+i) = (char*)malloc(sizeof(char) * 10);
		memset(*(hextemp+i), 0, sizeof(char)*10);
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
			

			for(int b=0;b<5;b++){hex_address[b] = 0;}//init

			memcpy(&hex_address[2], &tmp_buf[1], 2);


			for(int l=0;l<4;l++ )//checksum
			{
				hex_address[4]^=hex_address[l];//
			}


			if(((tmp_buf[2]/0x10)%2) == 0)
			{
				send_buf[0]=0x31;
				send_buf[1]=0xCE;	//complement of send_buf[0]

				memcpy(&send_buf[2], &hex_address[0], 5);
				memcpy(&send_buf[8], &tmp_buf[4], tmp_buf[0]);

				send_buf[7]=tmp_buf[0];
			}
			else
			{
				memcpy(&send_buf[24], &tmp_buf[4], tmp_buf[0]);

				send_buf[7]+=tmp_buf[0]-1;
				
				for(int l=7;l<send_buf[7]+9;l++ )//checksum
				{
					send_buf[send_buf[7]+9]^=send_buf[l];
				}


				while(1)
				{	
				write( fd_2, send_buf, send_buf[7]+10);//transmit data to ttyUSB0
				usleep(6);
				read(fd_2, &BootAck, 1);
				if(BootAck == 0x79)
				{
					for(int k=0;k<sizeof(send_buf);k++){send_buf[k]=0;}
					BootAck = 0;
					break;
				}//nack 많이 뜨는데 왜 많이 뜨는지 확인할 것 STM32F10x_AN2557_FW_V3.3.0 소스 확인할 것
				usleep(100);
				
				} 
					
			}

		}

		close(fd_1);
		close(fd_2);
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