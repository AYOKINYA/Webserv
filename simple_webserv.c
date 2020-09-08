
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define SERVER_INFO "Server: webserv/0.1.0\r\n"

int  serv_init(char *);
void error_exit(const char *);
void resolve_req(int);
void serve_file(int,const char *,const char *,const char *);
void build_header(int,const char *,const char *);
void ls(int,const char *);
void exec_cgi(int,const char *, char *);
void sigchld_handler(int);
void send_status(int);

int  sock_counter;

void sigchld_handler(int sig) {
    /*used to restore errno after wpid*/
    int reaped;
    int saved_errno = errno;
    while ((reaped = waitpid(-1, NULL, WNOHANG)) > 0){
      printf("child process: %d reaped\n", reaped);
      sock_counter--;
    }
    errno = saved_errno;
}

void error_exit(const char *em)
{
  perror(em);
  exit(1);
}

void resolve_req(int client_sock)
{
  struct stat st_buf;
  char buf[1024];
  char request[512];
  char *req_head;
  char *req_body;
  char *http_ver;
  char *args_ptr;
  char args[512];
  char cmd_string[512];
  int  byte_rec, byte_sent, i;
  int  is_cgi = 0;/*flag to indicate we got a cgi file request*/

  /*resolve client request*/
  if (recv(client_sock, buf, sizeof(buf), 0)) {
    printf("client request: %s\n", buf);
    /*extract out request info*/
    for (i = 0; i < strlen(buf); i++) {
      if (buf[i] == '\r' && buf[i+1] == '\n') {
        break;
      }
      request[i] = buf[i];
    }
    /*printf("processed request: %s\n", request);*/
    req_head = strtok(request," ");
    req_body = strtok(NULL," ");
    http_ver = strtok(NULL," ");
    

    /* grab all arguments if there are any*/
    args_ptr = strchr(req_body, '?');
    if (args_ptr) {
      strcpy(args, args_ptr + 1);
      printf("args: %s\n",args);

      char *token = strtok(args, "&");
      if (token == NULL) {
        perror("No arguments after ?");
      }
      strcat(cmd_string, token);
      token = strtok(NULL, "&");
      while (token != NULL) {
        int len = strlen(cmd_string) + strlen(token) + 1;
        strcat(cmd_string, " ");
        strcat(cmd_string, token);
        token = strtok(NULL, "&");
      }
      *args_ptr = '\0';
      printf("cmd string: %s\n", cmd_string);
    } else {
      /*printf("here\n");*/
      args[0] = NULL; /*no arguments, first entry is null terminator*/
      printf("request body: %s\n", req_body);
    }

    if (strcmp(req_head, "GET") && strcmp(req_head, "POST")) {
      serve_file(client_sock, "public/501.html", "501 Not Implemented", "text/html");
      send_status(501);
    }

    if (strcmp(req_head, "GET") == 0) {
      if (strlen(req_body) == 1 && *req_body == '/') {
        ls(client_sock, "ls -l ./");
      }else {
        req_body++;
        if (lstat(req_body,&st_buf) < 0) {
          serve_file(client_sock, "public/404.html", "404 Not Found", "text/html");
          send_status(404);
        }
        /*list requested directory file*/
        if (S_ISDIR(st_buf.st_mode)) {
          sprintf(buf, "ls -l ./%s",req_body);
          ls(client_sock, buf);
        }else if (S_ISREG(st_buf.st_mode)) {
          if (strstr(req_body, ".jpg") || strstr(req_body, ".gif") || strstr(req_body, ".jpeg") || strstr(req_body, ".png")) {
            serve_file(client_sock, req_body, "200 OK", "image/jpg");
          }else if (strstr(req_body, ".html")) {
            serve_file(client_sock, req_body, "200 OK", "text/html");
            send_status(200);
          }else if (strstr(req_body, ".cgi")) {
            printf("here\n");
            is_cgi = 1;
            sprintf(buf, "perl %s ", req_body);
            if (cmd_string){
              sprintf(&buf[strlen(buf)], "%s", cmd_string);
            }
            printf("cgi buf: %s\n", buf);
            exec_cgi(client_sock,buf,args);
          }else{
            serve_file(client_sock, req_body, "200 OK", "text/plain");
            send_status(200);
          }
        }
      }
    }
    memset(&buf, '\0', sizeof buf);
    memset(&request, '\0', sizeof request);
    /*printf("%s\n", "client sock closed");*/
    /*sock_counter--;*/
    printf("closing client sock %d , remaining sockets: %d\n",client_sock, sock_counter);
    /*close(client_sock);*/
  }
}

void build_header(int client_sock, const char *status, const char *type)
{
  char buf[1024];

  sprintf(buf, "HTTP/1.1 %s\n", status);
  send(client_sock, buf, strlen(buf), 0);
  sprintf(buf, SERVER_INFO);
  send(client_sock, buf, strlen(buf), 0);
  sprintf(buf, "Content-Type: %s\n", type);
  send(client_sock, buf, strlen(buf), 0);
  sprintf(buf, "\n");
  send(client_sock, buf, strlen(buf), 0);

  memset(&buf, '\0', sizeof buf);
}

void serve_file(int client_sock,const char *filename, const char *status, const char *type)
{
  int page;
  struct stat st_buf;

  page = open(filename, O_RDONLY);
  fstat(page, &st_buf);
  if (page < 0) {
    error_exit("No such file");
  }else {
    build_header(client_sock, status, type);
    sendfile(client_sock, page,0,st_buf.st_size);
  }
  close(page);
}

void ls(int client_sock, const char *cmd) {
  FILE *fp;
  char buf[1024];

  fp = popen(cmd,"r");
  if (fp == NULL) {
    error_exit("popen ls");
  }
  build_header(client_sock, "200 OK", "text/plain");
  while (fgets(buf, sizeof(buf) - 1, fp)) {
    send(client_sock, buf, strlen(buf), 0);
  }

  send_status(200);
  memset(&buf, '\0', sizeof buf);
  pclose(fp);
}

void exec_cgi(int client_sock, const char *cmd, char *args) {
  FILE *fp;
  char buf[1024];
  struct stat st_buf;
  int page;
  printf("cgi cmd: %s\n",cmd); 
  fp = popen(cmd,"r");
  if (fp == NULL) {
    error_exit("popen cgi");
  }
  build_header(client_sock, "200 OK", "text/html");
  while (fgets(buf, sizeof(buf) - 1, fp)) {
    send(client_sock, buf, strlen(buf), 0);
  }
  /*dup2(client_sock, 1); // redirect our output to the cgi file*/

  if (strstr(cmd, "my_histogram.cgi") != NULL) {
    while(lstat("histogram.png", &st_buf) != 0){
    }
    page = open("src/pretty_print.html", O_RDONLY);
    sendfile(client_sock, page,0,st_buf.st_size);
  }

  memset(&buf, '\0', sizeof buf);
  pclose(fp);
}

int serv_init(char *port_num)
{
  int status;
  struct addrinfo hints/*fill out this with relevant info*/, *servinfo, *p;
  int sockfd/*socket discriptor*/;
  int _true=1;

  memset(&hints, 0, sizeof hints);
  hints.ai_family   = AF_INET; //let's do ipv4 first
  hints.ai_socktype = SOCK_STREAM; //tcp socket
  hints.ai_flags    = AI_PASSIVE; //auto fill ip

  //setup addrinfo linked list with service http
  if ((status=getaddrinfo(NULL, port_num, &hints, &servinfo)) != 0) {
    error_exit("getaddrinfo");
  }

  //loop through all the serinfo we get and bind the first one we can bind
  for (p = servinfo; p != NULL; p= p->ai_next) {
    if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) < 0) {
      /*can't get a socket for this one*/
      perror("can't get socket");
      continue;
    }

    /*get rid of possible "Address already in use" error, bind to address immediately*/
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &_true, sizeof(int)) == -1) {
      error_exit("setsockopt");
    }

    if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
      /*bind to current socket failed, close it, find another*/
      close(sockfd);
      perror("bind");
      continue;
    }
    break;
  }

  //start listening to call of the specified port
  //setting incoming connection queue limit to 10
  if (listen(sockfd, 10) < 0) {
    error_exit("listen");
  }

  freeaddrinfo(servinfo);

  return sockfd;
}

void send_status(int status) {
  FILE* board;
  printf("sending status %d to board\n",status);
  if ((board = fopen("/dev/ttyACM0","w")) == NULL) {
    perror("open board");
  }
  fprintf(board,"%d",status);
  fclose(board);
}

int main(int argc, char *argv[]) {
  struct sockaddr_storage client_addr;
  struct sigaction sa;
  socklen_t client_addr_len;
  int serv_sock   = -1;
  int client_sock = -1;
  char *port;
  int cp;

  port = argv[1];
  serv_sock = serv_init(port);
  fprintf(stdout, "server runing on: %s socket num: %d\n", port, serv_sock);

  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD,&sa,NULL) == -1) {
    error_exit("sigaction");
  }

  //waiting for connection
  while (1) {
    client_addr_len = sizeof(client_addr);
    if((client_sock = accept(serv_sock, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
      error_exit("accept");
    }
    
    sock_counter++;
    cp = fork();
    if (!cp) {
      //in child process
      close(serv_sock);
      //printf("create child process with client socket: %d\n", client_sock);
      resolve_req(client_sock);
      //printf("exiting child process of client socket: %d\n", client_sock);
      exit(0);
    }
    close(client_sock);
  }

  close(serv_sock);
}