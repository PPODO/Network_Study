#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>

bool ConnectWait(int Socket, sockaddr* Address, int Sec) {
    int NewSocketStat;
    if((NewSocketStat = fcntl(Socket, F_GETFL)) < 0) {
        return false;
    }

    int OriginSocketStat = NewSocketStat;
    NewSocketStat |= O_NONBLOCK;

    if(fcntl(Socket, F_SETFL, NewSocketStat) < 0) {
        return false;
    }

    int Result;
    if((Result = connect(Socket, Address, sizeof(sockaddr))) < 0) {
        if(errno != EINPROGRESS) {
            return false;
        }
    }
    if(Result == 0) {
        fcntl(Socket, F_SETFL, OriginSocketStat);
        return true;
    }

    fd_set ReadFDS;
    timeval Timeout;
    FD_ZERO(&ReadFDS);
    FD_SET(Socket, &ReadFDS);
    Timeout.tv_usec = 0;
    Timeout.tv_sec = Sec;

    if(select(Socket + 1, &ReadFDS, NULL, NULL, &Timeout) < 0) {
        return false;
    }

    if(FD_ISSET(Socket, &ReadFDS)) {
        int Error = 0;
        socklen_t Size = sizeof(int);
        if(getsockopt(Socket, SOL_SOCKET, SO_ERROR, &Error, &Size) < 0) {
            return false;
        }
    }

    if(fcntl(Socket, F_SETFL, OriginSocketStat) < 0) {
        return false;
    }
    return true;
}

int main() {


    return 0;
}