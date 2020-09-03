#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "./libft/libft.h"

#define  BUFF_SIZE   1024

int   main( int argc, char **argv)
{
   (void)argc;
   int   client_socket;

   struct sockaddr_in   server_addr;

   char   buff[BUFF_SIZE+5];

   if ((client_socket  = socket( PF_INET, SOCK_STREAM, 0)) == -1)
   {
      printf( "socket 생성 실패\n");
      exit( 1);
   }

   ft_memset(&server_addr, 0, sizeof( server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons(4000);
   server_addr.sin_addr.s_addr= inet_addr("127.0.0.1");

   if( -1 == connect(client_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
   {
      printf( "접속 실패\n");
      exit( 1);
   }
   write(client_socket, argv[1], strlen(argv[1]) + 1);      // +1: NULL까지 포함해서 전송
   read (client_socket, buff, BUFF_SIZE);
   printf("%s\n", buff);
   close(client_socket);
   
   return 0;
}

// ]$ gcc server.c -o server    // 서버 프로그램을 server 이름으로 컴파일
// ]$ gcc client.c -o client    // 클라이언트 프로그램을 client 이름으로 컴파일
// ]$ ./server &                // 서브 프로그램을 백그라운드로 실행
// [1] 25869
// ]$ ./client test_string      // 클라이언트를 문자열을 입력하여 실행
// receive: test_string
// 11 : test_string
// ]$ ./client forum.falinux.com
// receive: forum.falinux.com
// 17 : forum.falinux.com
// ]$