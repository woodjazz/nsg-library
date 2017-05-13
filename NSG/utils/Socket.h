/*
---------------------------------------------------------------------------
This file is part of patterns.
http://github.com/woodjazz/patterns
Copyright (c) 2016-2017 Néstor Silveira Gorski

---------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
---------------------------------------------------------------------------
*/
#pragma once
#if defined(IS_TARGET_WINDOWS)
#include <windows.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#include <string>

namespace NSG {
class Socket {
public:
    Socket(int port, const char* serverName);
    Socket(int port);
    ~Socket();
    void Bind();
    void Listen(int connections);
    bool Connect();
    void SetRecvTimeout(int timeoutSecs);
    void SetSendTimeout(int timeoutSecs);
    static const char* GetHostName();
    static const char* GetIP(const char* hostName);
    static bool ValidPort(int port);
    static in_addr GetIPAdAddress(const char* szAddress);
    static bool ValidIPAddress(const char* szAddress);
    bool Select(int msec);
    static bool Select(int msec, int fd);
    bool SelectW(int msec);
    static bool SelectW(int msec, int fd);
    int GetFD() const;
    struct Accept {
        Accept(Socket& obj);
        ~Accept();
        void Send(const std::string& buffer);
        void Recv(std::string& buffer, int nBytes);
        void SetRecvTimeout(int timeoutSecs);
        void SetSendTimeout(int timeoutSecs);

    private:
        int fd_;
    };
    void Send(const std::string& buffer);
    void Recv(std::string& buffer, int nBytes);

private:
    static void Send(int fd, const std::string& buffer);
    static void Recv(int fd, std::string& buffer, int nBytes);
    static void SetRecvTimeout(int fd, int timeoutSecs);
    static void SetSendTimeout(int fd, int timeoutSecs);
    int port_;
    int sockfd_;
    sockaddr_in addr_;
};
}
