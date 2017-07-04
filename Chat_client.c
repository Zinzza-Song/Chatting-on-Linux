// chat_client.c
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <string.h>

#define    MAXLINE        512
#define    MAX_SOCK    128

char *escapechar = "exit";
char *list = "list";
char *happy = "행복";
char *forbiden1 = "개새끼";
char *forbiden2 = "ㅅㅂ";
char *forbiden3 = "ㅄ";
char *forbiden4 = "ㅈㄹ";
char name[10];        //채팅 닉네임        

 int main(int argc, char *argv[])
{
     char line[MAXLINE], msg[MAXLINE + 1];
     int n, pid;
     int maxfd1;
     int s;
     fd_set read_fds;
     struct sockaddr_in server_addr;

 if (argc != 4)
 {
         fprintf(stderr, "사용법: %s 'ip주소 port번호 사용자이름'을 입력하세요. \n", argv[0]);
         exit(0);
     }
//채팅 참가자 이름 저장
 sprintf(name, "[%s]", argv[3]);

 if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
 {
         fprintf(stderr, "Client: Can't open stream socket.\n");
         exit(0);
     }
//채팅 서버 소켓주소 저장
 bzero((char *)&server_addr, sizeof(server_addr));
     server_addr.sin_family = AF_INET;
     server_addr.sin_addr.s_addr = inet_addr(argv[1]);
     server_addr.sin_port = htons(atoi(argv[2]));
//연결 요청
 if (connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
 {
        fprintf(stderr, "Server: Can't bind local address.\n");
         exit(0);
    }
 else
     {
         printf("접속 성공! \n");
     }


 maxfd1 = s + 1;            //최대 소켓번호 + 1
	    FD_ZERO(&read_fds);
	
	 while (1)
	 {
	         FD_SET(0, &read_fds);
	         FD_SET(s, &read_fds);
	
	 if (select(maxfd1, &read_fds, (fd_set *)0, (fd_set *)0, (struct timeval *)0) < 0)
			{
			            fprintf(stderr, "select error <= 0\n");
			            exit(0);
			         }
		
			if (FD_ISSET(s, &read_fds))
			 {
		               int size;
			           if ((size = recv(s, msg, MAXLINE, 0)) > 0)
				 {
		                    msg[size] = '\0';
			                printf("%s \n", msg);
				             }
		}
	 if (FD_ISSET(0, &read_fds))
		 {
		             if (fgets(msg, MAXLINE, stdin))
				 {
				                 sprintf(line, "%s %s", name, msg);
			                 if (send(s, line, strlen(line), 0) < 0)
					 printf("send() error \n");
			
				 if (strstr(msg, escapechar) != NULL)  //종료문자 처리  
					 {
					                     printf("Good bye. \n");
					                     close(s);
					                     exit(0);
				                 }
				if (strstr(msg, list) != NULL)  //리스트 확인 
					 {
					                     printf("리스트를 불러옵니다!\n");
				                 }

				if ((strstr(msg, forbiden1) != NULL) || (strstr(msg, forbiden2) != NULL) || (strstr(msg, forbiden3) != NULL) || (strstr(msg, forbiden4) != NULL))
					{

						printf("욕석사용\n");
						close(s);
						exit(0);

					}

				 }
				}	
			
			        }
		    

	 return 0;
	}
