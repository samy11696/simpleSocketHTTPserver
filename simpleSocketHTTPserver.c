/**********************
 * author: samy11696
 * simpleSocketHTTPServer.c
 **********************/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#pragma comment(lib, "libwsock32.a")

#define myPort_ 8888
#define myAddr_ "127.0.0.1"

typedef struct myTime_{
    WORD Secs_;
    WORD Mins_;
    WORD Hours_;
}myTime_;

void Time_(myTime_ *ptr_);


int main()
{

    myTime_ ticks_;
    char buff_[5555] = "";
    char sbuff_[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=iso-8859-1\r\nConnection: Close\r\n\r\n";
    char html_[] = "<html><body><h1 style='color:red'>Helo !</h1></body></html>";
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2,2),&wsadata);
    SOCKET mySocket_;
    mySocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN sin_;
    SOCKADDR_IN info_;

    sin_.sin_family = AF_INET;
    sin_.sin_addr.s_addr = inet_addr(myAddr_);
    sin_.sin_port = htons(myPort_);


    bind(mySocket_, (SOCKADDR*)&sin_, sizeof(sin_));

    if(listen(mySocket_,5) != SOCKET_ERROR){
        printf("Listening.....(On port:%d)\n\n",myPort_);
    }else{
        printf("Listening ERROR.\n\n");
        closesocket(mySocket_);
        WSACleanup();
        return 0;
        };

    SOCKET myASocket_;
    int forever_ = TRUE;
    while(forever_){

            if((myASocket_ = accept(mySocket_, NULL, NULL)) != SOCKET_ERROR){
                getsockname(myASocket_,(SOCKADDR*)&info_,sizeof(info_));
                recv(myASocket_, buff_, sizeof(buff_),NULL);

                if(send(myASocket_, sbuff_, sizeof(sbuff_),NULL) != SOCKET_ERROR){
                        send(myASocket_, html_, sizeof(html_),NULL);
                        closesocket(myASocket_);
                        Time_(&ticks_);
                        printf("%d:%d:%d Adresse:%s  Port: %d Connected\n", ticks_.Hours_,ticks_.Mins_,ticks_.Secs_,inet_ntoa(info_.sin_addr), info_.sin_port);
                }else{
                    printf("Sending ERROR.\n\n");
                    closesocket(myASocket_);
                    closesocket(mySocket_);
                    WSACleanup();
                    return 0;
                    }
            }else{
                printf("Accepting ERROR.\n\n");
                closesocket(mySocket_);
                WSACleanup();
                return 0;
                }



    }
    closesocket(myASocket_);
    closesocket(mySocket_);
    WSACleanup();
    return 0;
}

void Time_(myTime_ *ptr_){
    SYSTEMTIME time;
    GetLocalTime(&time);
    ptr_->Secs_ = time.wSecond;
    ptr_->Mins_ = time.wMinute;
    ptr_->Hours_ = time.wHour;
}

