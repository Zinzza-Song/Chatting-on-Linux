// chat_server.c

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <string.h>

#define    MAXLINE        512
#define    MAX_SOCK    64
	
int getmax(int);
void removeClient(int);            //채팅 탈퇴
void showlistClient(int);	//리스트 불러오기
char *replaceValue(char *strInput, const char *strTarget, const char *strChange); //치환

char *escapechar = "exit";
char *whisper = "/w";
char *list = "list";
char *happy = "행복";
char *happy2 = "좋아";
char *happy3 = "기뻐";
char *sad = "슬픔";
char *sad2 = "슬퍼";
char *sad3 = "우울";

int max_fd1;                    //최대 소켓번호 +1
int num_chat = 0;                //채팅 참가자 
int client_s[MAX_SOCK];            //채팅 참가자 소켓번호 목록


 int main(int argc, char *argv[])
 {
     char rline[MAXLINE], my_msg[MAXLINE];
     char *start = "연결되었습니다! \n";
     char *happyy = "^o^\n";
     char *sadd = "(ㅠ.,ㅠ)\n";

     int i, j, n;
	 int s;
	 int client_fd, client_len;

	fd_set read_fds;
	     struct sockaddr_in client_addr, server_addr;
	
	 if (argc != 2)
	 {
		  fprintf(stderr, "사용법: %s 포트번호 \n", argv[0]);
		  exit(0);
 }

		 if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		 {
		   fprintf(stderr, "Server: Can't open stream socket.\n");
		   exit(0);
 }
	
	 bzero((char *)&server_addr, sizeof(server_addr));
	 server_addr.sin_family = AF_INET;
	 server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	 server_addr.sin_port = htons(atoi(argv[1]));
	
 if (bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		 {
		     fprintf(stderr, "Server: Can't bind local address.\n");
	         exit(0);
	     }
	
	listen(s, 5);
	max_fd1 = s + 1;            //최대 소켓번호 + 1
	
	while (1)
		 {
		       FD_ZERO(&read_fds);
		       FD_SET(s, &read_fds);
		
		for (i = 0; i < num_chat; i++)
			 FD_SET(client_s[i], &read_fds);
		     max_fd1 = getmax(s) + 1;        //max_fd1 재계산

		       if (select(max_fd1, &read_fds, (fd_set *)0, (fd_set *)0, (struct timeval *)0) < 0)
			 {
			            fprintf(stderr, "select error <= 0\n");
			            exit(0);
}
		
			 if (FD_ISSET(s, &read_fds))
			 {
			            client_len = sizeof(client_addr);
			            client_fd = accept(s, (struct sockaddr *)&client_addr, &client_len);
			            if (-1 == client_fd)
{
				               fprintf(stderr, "accept error\n");
			                exit(0);
				         }
			
			             //채팅 클라이언트 목록에 추가
			client_s[num_chat] = client_fd;
			     num_chat++;
		         send(client_fd, start, strlen(start), 0);
			     printf("%d번째 사용자 입장하셨습니당^.^ \n", num_chat);
}
		        //클라이언트가 보낸 메시지를 모든 클라이언트에게 방송
			 for (i = 0; i < num_chat; i++)
		 {
			            if (FD_ISSET(client_s[i], &read_fds))
				{
			                 if ((n = recv(client_s[i], rline, MAXLINE, 0)) <= 0)
					 {
					                     removeClient(i);
				                   continue;
					              }
			
					 if (strstr(rline, escapechar) != NULL)   //종료 처리
					 {
					                   removeClient(i);
				                  continue;
				               }

					 if (strstr(rline, happy) != NULL)   //행복이모티콘
					 {
						rline[n] = '\0';
				              for (j = 0; j < num_chat; j++){
						char *hch = replaceValue(rline, happy, happyy);
						send(client_s[j], hch, strlen(hch), 0);
							}
				                  continue;
				               }
					 if (strstr(rline, happy2) != NULL)   //행복이모티콘
					 {
						rline[n] = '\0';
				              for (j = 0; j < num_chat; j++){
						char *hch = replaceValue(rline, happy2, happyy);
						send(client_s[j], hch, strlen(hch), 0);
							}
				                  continue;
				               }
					 if (strstr(rline, happy3) != NULL)   //행복이모티콘
					 {
						rline[n] = '\0';
				              for (j = 0; j < num_chat; j++){
						char *hch = replaceValue(rline, happy3, happyy);
						send(client_s[j], hch, strlen(hch), 0);
							}
				                  continue;
				               }



					 if (strstr(rline, sad) != NULL)   //슬픈이모티콘
					 {
							rline[n] = '\0';
				              for (j = 0; j < num_chat; j++){
						char *sch = replaceValue(rline, sad, sadd);
						send(client_s[j], sch, strlen(sch), 0);
							}
				
				                  continue;
				               }
					 if (strstr(rline, sad2) != NULL)   //슬픈이모티콘
					 {
							rline[n] = '\0';
				              for (j = 0; j < num_chat; j++){
						char *sch = replaceValue(rline, sad2, sadd);
						send(client_s[j], sch, strlen(sch), 0);
							}
				
				                  continue;
				               }
					 if (strstr(rline, sad3) != NULL)   //슬픈이모티콘
					 {
							rline[n] = '\0';
				              for (j = 0; j < num_chat; j++){
						char *sch = replaceValue(rline, sad3, sadd);
						send(client_s[j], sch, strlen(sch), 0);
							}
				
				                  continue;
				               }


					//귓속말
					if(strstr(rline, whisper) != NULL){
						rline[n] = '\0';
						char wname[50];
						strtok(rline, "/w:  ");
						sprintf(wname, "%s", strtok(NULL, ":/w "));
						for(j = 0; j < num_chat; j++)
							if(!strcmp(client_s[j],wname)) break;
							sprintf(rline, "%s : %s",rline,strtok		(NULL, ""));
							printf("[To %s, From %s]",wname,rline);
							send(client_s[j], rline, n, 0);
							
						}else{		
				                //모든 채팅 참가자에게 메시지 출력
					 rline[n] = '\0';
				              for (j = 0; j < num_chat; j++)
					 send(client_s[j], rline, n, 0);
				
				 printf("%s", rline);
				            }
				}
		       }

	    }
	
return 0;
	 }


//채팅 나가기
void removeClient(int i)
	 {
	 close(client_s[i]);
	
	 if (i != num_chat - 1)
	client_s[i] = client_s[num_chat - 1];
	
	 num_chat--;
	     printf("채팅 참가자 1명이 나갔습니다. 현 참가자수는 %d 입니다.\n", num_chat);
	 }
//리스트 불러오기
void showlistClient(int l)
	{
		
		printf("리스트");
	}
//client_s[]내의 최대 소켓번호 얻기
	 int getmax(int k)
	 {
   int max = k;
    int r;

	 for (r = 0; r < num_chat; r++)
		 {
	        if (client_s[r] > max)
		max = client_s[r];
		     }
		 return max;
	 }

//문자열 치환
char *replaceValue(char *strInput, const char *strTarget, const char *strChange)
{
    char* strResult;
    char* strTemp;
    int i = 0;
    int nCount = 0;
    int nTargetLength = strlen(strTarget);
 
    if (nTargetLength < 1)
        return strInput;
 
    int nChangeLength = strlen(strChange);
 
    if (nChangeLength != nTargetLength)
    {
        for (i = 0; strInput[i] != '\0';)
        {
            if (memcmp(&strInput[i], strTarget, nTargetLength) == 0)
            {
                nCount++;
                i += nTargetLength;
            }
            else i++;
        }
    }
    else
    {
        i = strlen(strInput);
    }
 
    strResult = (char *) malloc(i + 1 + nCount * (nChangeLength - nTargetLength));
    if (strResult == NULL) return NULL;
 
 
    strTemp = strResult;
    while (*strInput)
    {
        if (memcmp(strInput, strTarget, nTargetLength) == 0)
        {
            memcpy(strTemp, strChange, nChangeLength);
            strTemp += nChangeLength;
            strInput  += nTargetLength;
        }
        else
        {
            *strTemp++ = *strInput++;
        }
    }
 
    *strTemp = '\0';
 
    return strResult;
}

