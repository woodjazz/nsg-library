#include "Socket.h"
#include <stdexcept>
#include <cstring>
#include <thread>
#include <string>
#include <cstdio>

#ifdef IS_TARGET_WINDOWS
#include <Strsafe.h>
#else
#include <netinet/tcp.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits.h>
#endif

namespace NSG
{
	static std::string GetStringError()
	{
#if defined(IS_TARGET_WINDOWS)
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = WSAGetLastError();
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
			(lstrlen((LPCTSTR)lpMsgBuf) + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR)lpDisplayBuf,
			LocalSize(lpDisplayBuf),
			TEXT("Failed with error %d: %s"), dw, lpMsgBuf);
		std::string errorMsg((const char*)lpDisplayBuf);

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
		return errorMsg;
#else
		return strerror(errno);
#endif
	}

    #if defined(IS_TARGET_WINDOWS)
    struct SocketInit
    {
        SocketInit()
        {
			auto iResult = WSAStartup(MAKEWORD(2, 2), &wsaData_);
			if (iResult != 0)
				throw std::runtime_error(GetStringError()); 
        }
        ~SocketInit()
        {
            WSACleanup();
        }
		WSADATA wsaData_;
    };
    static SocketInit initialize_windows_sockets;
    #endif

    Socket::Socket(int port, const char* serverName)
        : port_(port)
    {
        if (-1 == (sockfd_ = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_IP)))
            throw std::runtime_error(GetStringError());
        memset((char*)&addr_, 0, sizeof(sockaddr_in));
        addr_.sin_family = AF_INET;
        if (!ValidPort (port_))
            throw std::runtime_error("invalid port");
        else
            addr_.sin_port = htons(port_);
        // Tries to get the address using DNS (Domain Name System)
        addr_.sin_addr = Socket::GetIPAdAddress (serverName);
        if (INADDR_NONE == addr_.sin_addr.s_addr)
            throw std::runtime_error("invalid server");
    }

    Socket::Socket(int port)
        : port_(port)
    {
        if (-1 == (sockfd_ = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_IP)))
            throw std::runtime_error(GetStringError());
        memset((char*)&addr_, 0, sizeof(sockaddr_in));
        addr_.sin_family = AF_INET;
        if (!ValidPort (port_))
            throw std::runtime_error("invalid port");
        else
            addr_.sin_port = htons(port_);
        addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    Socket::~Socket()
    {
        #if defined(IS_TARGET_WINDOWS)
        closesocket(sockfd_);
        #else
        close(sockfd_);
        #endif
    }

    int Socket::GetFD() const
    {
        return sockfd_;
    }

    void Socket::Listen(int connections)
    {
        if (-1 == listen(sockfd_, connections))
            throw std::runtime_error(GetStringError());
    }

    void Socket::Bind()
    {
        int activate = 1;
        if (-1 == setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, (char*)&activate, static_cast<int>(sizeof(activate))))
            throw std::runtime_error(GetStringError());
        if (-1 == ::bind(sockfd_, (sockaddr*)&addr_, sizeof(sockaddr)))
            throw std::runtime_error(GetStringError());
    }

    bool Socket::Select(int msec)
    {
        return Socket::Select(msec, sockfd_);
    }

    bool Socket::Select(int msec, int fd)
    {
        timeval tv;
        fd_set readfds;
        tv.tv_sec = msec / 1000;
        tv.tv_usec = (msec % 1000) * 1000;
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        // Waits (blocks) for msec until there is something to read.
        if (-1 == select(fd + 1, &readfds, 0, 0, msec == -1 ? 0 : &tv))
            throw std::runtime_error(GetStringError());
        return FD_ISSET(fd, &readfds) != 0;
    }

    bool Socket::SelectW(int msec)
    {
        return Socket::SelectW(msec, sockfd_);
    }

    bool Socket::SelectW(int msec, int fd)
    {
        timeval tv;
        fd_set writefds;
        tv.tv_sec = msec / 1000;
        tv.tv_usec = (msec % 1000) * 1000;
        FD_ZERO(&writefds);
        FD_SET(fd, &writefds);
        // Waits (blocks) for msec until it is possible to write
        if (-1 == select(fd + 1, 0, &writefds, 0, msec == -1 ? 0 : &tv))
            throw std::runtime_error(GetStringError());
        return FD_ISSET(fd, &writefds) != 0;
    }

    bool Socket::Connect()
    {
        if (-1 == connect(sockfd_, (sockaddr*)&addr_, sizeof(sockaddr)))
        {
            fprintf(stderr, "Connect failed: %s\n", GetStringError().c_str());
            return false;
        }
        // Delivering Messages Immediately
        int optVal = 1;
        if (-1 == setsockopt (sockfd_, IPPROTO_TCP, TCP_NODELAY, (char*)&optVal, sizeof (optVal)))
            throw std::runtime_error(GetStringError());
        return true;
    }

    static bool ErrorWouldBlock()
    {
        #if defined(IS_TARGET_WINDOWS)
        return WSAEWOULDBLOCK == WSAGetLastError();
        #else
        return errno == EWOULDBLOCK || errno == EAGAIN;
        #endif
    }

    void Socket::Send(int fd, const std::string& buffer)
    {
        int n = 0;
        int nBytes = (int)buffer.size();
        const char* p = &buffer[0];
        while (nBytes)
        {
            n = send(fd, p, nBytes, 0);
            if (n < 0)
            {
                if (ErrorWouldBlock())
                {
                    std::this_thread::yield();
                    continue;
                }
                throw std::runtime_error(GetStringError());
            }
            p += n;
            nBytes -= n;
        }
    }

    void Socket::Recv(int fd, std::string& buffer, int nBytes)
    {
        int n = 0;
        buffer.resize(nBytes);
        char* p = &buffer[0];
        while (nBytes)
        {
            n = recv(fd, p, nBytes, 0);
            if(n == 0)
                break;
            else if (n < 0)
            {
                if (ErrorWouldBlock())
                {
                    std::this_thread::yield();
                    continue;
                }
                throw std::runtime_error(GetStringError());
            }
            p += n;
            nBytes -= n;
        }
    }

    void Socket::Send(const std::string& buffer)
    {
        Socket::Send(sockfd_, buffer);
    }

    void Socket::Recv(std::string& buffer, int nBytes)
    {
        Socket::Recv(sockfd_, buffer, nBytes);
    }

    void Socket::SetRecvTimeout(int timeoutSecs)
    {
        Socket::SetRecvTimeout(sockfd_, timeoutSecs);
    }

    void Socket::SetSendTimeout(int timeoutSecs)
    {
        Socket::SetSendTimeout(sockfd_, timeoutSecs);
    }

    void Socket::SetRecvTimeout(int fd, int timeoutSecs)
    {
        #if defined(IS_TARGET_WINDOWS)
        int optRecVal = timeoutSecs * 1000; //ms
        if (-1 == setsockopt (fd, SOL_SOCKET,  SO_RCVTIMEO, (char*)&optRecVal, sizeof (optRecVal)))
            throw std::runtime_error(GetStringError());
        #else
        struct timeval trcv;
        socklen_t ntrcv = sizeof(trcv);
        if (getsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &trcv, &ntrcv) < 0)
            throw std::runtime_error(GetStringError());
        trcv.tv_sec = timeoutSecs;
        if (-1 == setsockopt (fd, SOL_SOCKET,  SO_RCVTIMEO, &trcv, ntrcv))
            throw std::runtime_error(GetStringError());
        #endif
    }

    void Socket::SetSendTimeout(int fd, int timeoutSecs)
    {
        #if defined(IS_TARGET_WINDOWS)
        int optRecVal = timeoutSecs * 1000; //ms
        if (-1 == setsockopt (fd, SOL_SOCKET,  SO_SNDTIMEO, (char*)&optRecVal, sizeof (optRecVal)))
            throw std::runtime_error(GetStringError());
        #else
        struct timeval tsnd;
        socklen_t nsnd = sizeof(tsnd);
        if (getsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tsnd, &nsnd) < 0)
            throw std::runtime_error(GetStringError());
        tsnd.tv_sec = timeoutSecs;
        if (-1 == setsockopt (fd, SOL_SOCKET,  SO_SNDTIMEO, &tsnd, nsnd))
            throw std::runtime_error(GetStringError());
        #endif
    }


    Socket::Accept::Accept(Socket& obj)
    {
        sockaddr_in theirAddr;
		#if defined(IS_TARGET_WINDOWS)
		int sockAddrSize = sizeof(sockaddr_in);
		#else
        socklen_t sockAddrSize = sizeof(sockaddr_in);
		#endif
        if (-1 == (fd_ = (int)accept(obj.sockfd_, (sockaddr*)&theirAddr, &sockAddrSize)))
            throw std::runtime_error(GetStringError());
    }

    Socket::Accept::~Accept()
    {
        #if defined(IS_TARGET_WINDOWS)
        closesocket(fd_);
        #else
        close(fd_);
        #endif
    }

    void Socket::Accept::Send(const std::string& buffer)
    {
        Socket::Send(fd_, buffer);
    }

    void Socket::Accept::Recv(std::string& buffer, int nBytes)
    {
        Socket::Recv(fd_, buffer, nBytes);
    }

    void Socket::Accept::SetRecvTimeout(int timeoutSecs)
    {
        Socket::SetRecvTimeout(fd_, timeoutSecs);
    }

    void Socket::Accept::SetSendTimeout(int timeoutSecs)
    {
        Socket::SetSendTimeout(fd_, timeoutSecs);
    }

    const char* const Socket::GetHostName()
    {
        #ifndef IS_TARGET_LINUX
        const int HOST_NAME_MAX = 256;
        #endif
        static char hostName[HOST_NAME_MAX];
        gethostname(hostName, HOST_NAME_MAX);
        return hostName;
    }

    const char* const Socket::GetIP(const char* hostName)
    {
        static sockaddr_in iaddr;
        auto hp = gethostbyname(hostName);
        if (!hp)
            throw std::runtime_error("gethostbyname has failed");
        memcpy(&(iaddr.sin_addr), hp->h_addr, hp->h_length);
        return inet_ntoa (iaddr.sin_addr);
    }

    bool Socket::ValidPort (int port)
    {
        return !(port < 0 || port > 0xFFFF);
    }

    in_addr Socket::GetIPAdAddress (const char* szAddress)
    {
        sockaddr_in addr;
        memset ((char*)&addr, 0, sizeof(sockaddr_in));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr (szAddress);
        if (INADDR_NONE == addr.sin_addr.s_addr)
        {
            addr.sin_addr.s_addr = inet_addr (Socket::GetIP (szAddress));
            return addr.sin_addr;
        }
        return addr.sin_addr;
    }

    bool Socket::ValidIPAddress (const char* szAddress)
    {
        in_addr addr = Socket::GetIPAdAddress (szAddress);
        return (addr.s_addr != INADDR_NONE);
    }
}
