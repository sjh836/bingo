#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BOARD_SIZE 5

void error_check(int validation, char *message); //실행 오류 검사
int value_check(int number); //점수 유효값 검사
void game_init(); //빙고판 생성
void game_run(int number, int turn_count); //게임진행
void bingo_check();

int board[BOARD_SIZE][BOARD_SIZE]; //보드판 배열
int turn[BOARD_SIZE*BOARD_SIZE]; //턴마다 숫자 기록
int flag=1; //게임 승리여부
int check_number[BOARD_SIZE*BOARD_SIZE+1]={0};
int socket_fd;

void main(int argc, char *argv[])
{
	int array_len, recv_len, recv_count, i;
	struct sockaddr_in server_adr;

	if(argc!=3)
	{
		printf("./실행파일 IP주소 PORT번호 형식으로 실행해야 합니다.\n");
		exit(1);
	}

	socket_fd=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //TCP소켓 생성
	error_check(socket_fd, "소켓 생성");

	memset(&server_adr, 0, sizeof(server_adr)); //구조체 변수 값 초기화
	server_adr.sin_family=AF_INET; //IPv4
	server_adr.sin_port=htons(atoi(argv[2])); //포트 할당
	server_adr.sin_addr.s_addr=inet_addr(argv[1]); //IP 할당

	error_check(connect(socket_fd, (struct sockaddr *)&server_adr, sizeof(server_adr)), "연결 요청");

	printf("빙고게임을 시작합니다.\n");
	game_init();
	
	for(i=0;i<BOARD_SIZE*BOARD_SIZE&&flag;i++)
	{
		if(i%2==0) //클라이언트 차례
		{
			printf("숫자를 입력해주세요 : ");
			scanf("%d", &turn[i]);
			turn[i]=value_check(turn[i]);
			check_number[turn[i]]=1;

			array_len=write(socket_fd, &turn[i], sizeof(turn[i])); //숫자를 서버로 전송
			printf("%d 바이트를 전송하였습니다\n", array_len);
			error_check(array_len, "데이터전송");
		}
		else //서버 차례
		{
			recv_len=0;
			while(recv_len!=sizeof(int)) // 패킷이 잘려서 올수도 있으므로 예외처리를 위한 조건문
			{
				recv_count=read(socket_fd, &turn[i], sizeof(turn[i]));
				error_check(recv_count, "데이터수신");
				if(recv_count==-0) break;
				printf("%d 바이트를 수신하였습니다\n", recv_count);
				recv_len+=recv_count;
			}
			check_number[turn[i]]=1;
		}
		printf("turn[%d]=%d\n",i,turn[i]); //디버깅용
		game_run(turn[i], i);
	}
	close(socket_fd);

	printf("프로그램을 종료합니다\n");
}
void error_check(int validation, char* message)
{
	if(validation==-1)
	{
		fprintf(stderr, "%s 오류\n", message);
		exit(1);
	}
	else
	{
		fprintf(stdout, "%s 완료\n", message);
	}
}
int value_check(int number) //점수 유효값 검사
{
	if(number<1||number>25||check_number[number]==1)
	{
		printf("값이 유효하지 않습니다. 다시입력해주세요 : ");
		scanf("%d", &number);
		number=value_check(number); //유효값 입력할때까지 재귀호출
	}
	return number;
}
void game_init()
{
	int check_number[BOARD_SIZE*BOARD_SIZE]={0};
	int i, j; //카운트용 변수

	system("clear"); //화면 초기화
	srand(time(NULL)+1); //시간을 인자로 주어서 완전한 랜덤을 구현

	printf("%c[1;33m",27); //터미널 글자색을 노랑색으로 변경
	
	printf("@------ 빙고판 현황 ------@\n\n"); 
	printf("+----+----+----+----+----+\n"); 
	for(i=0; i < BOARD_SIZE; i++)
	{
		for(j=0; j < BOARD_SIZE; j++)
		{
			while(1)
			{
				int temp = rand()%25; //0~24 난수 발생
				
				if(check_number[temp]==0) //중복제거 알고리즘
				{
					check_number[temp]=1;
					board[i][j]=temp+1;
					printf("| %2d ", board[i][j]); 
					break;
				}
			}
		}
		printf("|\n");
		printf("+----+----+----+----+----+\n"); 
	}      
	printf("%c[0m", 27); //터미널 글자색을 원래색으로 변경
}
void game_run(int number, int turn_count)
{
	int i, j; //카운트용 변수

	//system("clear"); //화면 초기화
	printf("%c[1;33m",27); //터미널 글자색을 노랑색으로 변경
	
	printf("@------ 빙고판 현황 ------@\n");
	printf("진행 턴수: %d\n", turn_count+1); 
	printf("+----+----+----+----+----+\n"); 
	for(i=0; i < BOARD_SIZE; i++)
	{
		for(j=0; j < BOARD_SIZE; j++)
		{
			if(board[i][j]==number)
				board[i][j]=0; //X표 처리
			if(board[i][j]==0)
			{
				printf("| ");
				printf("%c[1;31m",27);
				printf("%2c ", 88); 
				printf("%c[1;33m",27);
			}
			else
				printf("| %2d ", board[i][j]); 
		}
		printf("|\n");
		printf("+----+----+----+----+----+\n"); 
	}      
	printf("%c[0m", 27); //터미널 글자색을 원래색으로 변경
	bingo_check();
}
void bingo_check()
{
	int i;
	int count=0;

	for(i=0; i < BOARD_SIZE; i++) //가로
	{
		if(board[i][0]==0&&board[i][1]==0&&board[i][2]==0&&board[i][3]==0&&board[i][4]==0) //가로
		{
			count++;
		}
		if(board[0][i]==0&&board[1][i]==0&&board[2][i]==0&&board[3][i]==0&&board[4][i]==0) //세로
			count++;
	}
	/*
	for(i=0; i < BOARD_SIZE; i++)
	{
		for(j=0; board[i][j]==0||j==BOARD_SIZE; j++)
		{
			if(j==BOARD_SIZE)
			{
				count++;
			}
		}
	}
	*/
	if(board[0][0]==0&&board[1][1]==0&&board[2][2]==0&&board[3][3]==0&&board[4][4]==0)
		count++;
	if(board[0][4]==0&&board[1][3]==0&&board[2][2]==0&&board[3][1]==0&&board[4][0]==0)
		count++;
	printf("빙고 수: %d\n", count);

	if(count>=5)
	{
		flag=0;
		printf("클라이언트 승리\n");
	}
}