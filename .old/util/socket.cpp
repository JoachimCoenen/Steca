/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: socket.cpp                                                    $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/socket.cpp 5     27/07/04 3:42p Ian                $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file socket.cpp
 */

#include "socket.hpp"
#include <winsock2.h>

namespace util {

//-----------------------------------------------------------------------------

namespace WSA
{
    static bool    isStarted = false;
    static WSADATA wsaData;

	// API error codes
	struct sErrType
	{
		uint  errCode;
		pcstr errString;
    } 
    errType[] =
	{
		{ 10013, "Permission denied"								},
		{ 10048, "Address already in use"							},
		{ 10049, "Cannot assign requested address"					},
		{ 10047, "Address family not supported by protocol family"	},
		{ 10037, "Operation already in progress"					},
		{ 10053, "Software caused connection abort"					},
		{ 10061, "Connection refused"								},
		{ 10054, "Connection reset by peer"							},
		{ 10039, "Destination address required"						},
		{ 10014, "Bad address"										},
		{ 10064, "Host is down"										},
		{ 10065, "No route to host"									},
		{ 10036, "Operation now in progress"						},
		{ 10004, "Interrupted function call"						},
		{ 10022, "Invalid argument"									},
		{ 10056, "Socket is already connected"						},
		{ 10024, "Too many open files"								},
		{ 10040, "Message too long"									},
		{ 10050, "Network is down"									},
		{ 10052, "Network dropped connection on reset"				},
		{ 10051, "Network is unreachable"							},
		{ 10055, "No buffer space available"						},
		{ 10042, "Bad protocol option"								},
		{ 10057, "Socket is not connected"							},
		{ 10038, "Socket operation on non-socket"					},
		{ 10045, "Operation not supported"							},
		{ 10046, "Protocol family not supported"					},
		{ 10067, "Too many processes"								},
		{ 10043, "Protocol not supported"							},
		{ 10041, "Protocol wrong type for socket"					},
		{ 10058, "Cannot send after socket shutdown"				},
		{ 10044, "Socket type not supported"						},
		{ 10060, "Connection timed out"								},
		{ 10109, "Class type not found"								},
		{ 10035, "Resource temporarily unavailable"					},
		{ 11001, "Host not found"									},
		{ 10093, "Successful WSAStartup not yet performed"			},
		{ 11004, "Valid name, no data record of requested type"		},
		{ 11003, "This is a non-recoverable error"					},
		{ 10091, "Network subsystem is unavailable"					},
		{ 11002, "Non-authoritative host not found"					},
		{ 10092, "WINSOCK.DLL version out of range"					},
		{ 10094, "Graceful shutdown in progress"					},
		{ 0, NULL}
	};

    class WSAStarter
    {
    public:
        WSAStarter();
       ~WSAStarter();
    };

	WSAStarter::WSAStarter()
    {
        if(0 == ::WSAStartup(MAKEWORD(1,1),&wsaData))
        {
            isStarted = true;
        }
    }

	WSAStarter::~WSAStarter()
    {
        if(isStarted) 
        {
            ::WSACleanup();
            isStarted = false;
        }
    }

    bool Start()
    {
        static WSAStarter wsaStarter;
        return isStarted;
    }

    bool IsStarted()
    {
        return isStarted;
    }

	pcstr LastError()
	{
        static int lastError = 0;
		sErrType *p   = errType;

		int err = WSAGetLastError();
        if(0!=err)
            lastError = err;

		for(;;)
		{
			int errCode = p->errCode;

			if(0==errCode) 
				break;

			if(lastError==errCode)
				return p->errString;
				
			p++;
		}

		return NULL;
	}
}

//-----------------------------------------------------------------------------

Socket::Socket(uint sendBufferSize)
  : m_buffer(sendBufferSize>0?sendBufferSize:1), m_buffered(0),
    m_sock(INVALID_SOCKET), m_receiveClosed(false)
{                
}


Socket::~Socket()
{
    Close();
}


bool Socket::IsValid() const
{
    return INVALID_SOCKET!=m_sock;
}


void Socket::Connect(pcstr name,ushort port)
{
    Close();

    RUNTIME_CHECK_EXC3(!empty(name),EMPTY_NAME,"")
    RUNTIME_CHECK_EXC3(WSA::Start(),WSA_NOT_STARTED,"")

    typedef unsigned long IPAddress;

    IPAddress addr = inet_addr(name);
    if(INADDR_NONE==addr) 
    {
        hostent *host = gethostbyname(name);
        RUNTIME_CHECK_EXC3(host,CANNOT_RESOLVE_HOST,"")
        addr = ((in_addr*)host->h_addr)->s_addr;
    }

    sockaddr_in saddr;
    saddr.sin_family      = AF_INET;
    saddr.sin_port        = htons(port);
    saddr.sin_addr.s_addr = addr;

    m_sock = socket(AF_INET,SOCK_STREAM,0);
    RUNTIME_CHECK_EXC3(IsValid(),CANNOT_CONNECT,"WSA::LastError()")

    if(SOCKET_ERROR==connect(m_sock,(sockaddr*)&saddr,sizeof(saddr)))
    {
        Close();
		THROW_EXC2(CANNOT_CONNECT,WSA::LastError())
    }
}


void Socket::Close()
{
    if(IsValid())
    {
        if(!std::uncaught_exception())
            Flush();
        closesocket(m_sock);
        m_sock = INVALID_SOCKET;
    }
}


void Socket::Send(byte b)
{
    if(m_buffered>=m_buffer.size()) 
        Flush();
    m_buffer[m_buffered++] = b;
}


void Socket::Send(pcvoid data,size_t size)
{
    ASSERT_EXC(NULL!=data)

	pcbyte p = (pcbyte)data;
	while(size>0)
	{
		Send(*(p++));
        size--;
	}
}


void Socket::Send(pcstr buf)
{
    ASSERT_EXC(NULL!=buf)

    BYTE b;
    while(0!=(b = *(buf++)))
    {
        Send(b);
    }
}


void Socket::Flush()
{
    if(0==m_buffered) return;
    SendNonBuffered(m_buffer.ptr(),m_buffered);
    m_buffered=0;
}


bool Socket::WaitForData(timeout_t sec)
{
	FD_SET readfds; FD_ZERO(&readfds);
	FD_SET(m_sock,&readfds);

	timeval timeout = {sec,0};

	if(SOCKET_ERROR==select(0,&readfds,NULL,NULL,&timeout))
	{
		THROW_EXC2(RECV_FAILED,WSA::LastError())
	}

	return (0!=FD_ISSET(m_sock,&readfds));
}


Socket::eStatus Socket::Receive(byte& b,timeout_t sec)
{
    uint received;
	return Receive(&b,1,sec,received);
}


Socket::eStatus Socket::Receive(pvoid buf,size_t bufSize,timeout_t sec,uint& received)
{
    ASSERT_EXC(NULL!=buf)
    ASSERT_EXC(!m_receiveClosed)

    if(!WaitForData(sec)) 
        return SOCK_TIMEOUT;

    int lgt = recv(m_sock,(PSTR)buf,bufSize,0);
    switch(lgt)
    {
    case 0:
        m_receiveClosed = true;
        return SOCK_CLOSED;

    case SOCKET_ERROR:
		THROW_EXC2(RECV_FAILED,WSA::LastError())
    }

    ASSERT_EXC(lgt>=0)
    received = lgt;

    return SOCK_RECEIVED;
}


Socket::eStatus Socket::ReceiveLine(std::string& line,timeout_t sec)
{
    line.erase();

    if(m_receiveClosed) 
        return SOCK_CLOSED;

    std::string s;

    for(;;)
    {
        char c;

        size_t received;
        eStatus status = Receive(&c,1,sec,received);
        if(SOCK_RECEIVED==status)
        {
            ASSERT_EXC(1==received)
            if('\r'==c) continue;   // dump CR
            if('\n'==c) break;
            s += c;
        }
        else
        if(SOCK_CLOSED==status)
        {
            if(s.empty()) 
                return SOCK_CLOSED;
            else
                break;
        }
        else
        {
            ASSERT_EXC(SOCK_TIMEOUT==status)
            return SOCK_TIMEOUT;
        }
    }

    line = s;
    return SOCK_RECEIVED;
}


static char HexChar(uint value)
{
    static char hexChar[16]=
    {
	    '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
    };

    return hexChar[value & 0xF];
}


std::string Socket::CgiEncode(pcstr p)
{
    ASSERT_EXC(NULL!=p)

    static pcstr cgiSpecial = "<>#%{}|^~[]`;?:@=&!$+-\'\"";

    std::string s;
    unsigned char c;

    while(0 != (c=*(p++)))
    {
	    if(' '==c)
	    {
	        s += '+';
	        continue;
	    }

	    if(c<' ' || c>=(unsigned char)127 || NULL!=strchr(cgiSpecial,c))
	    {
		    ((s+='%') += HexChar(c>>4)) += HexChar(c);
		    continue;
	    }

	    s += c;
    }

    return s;
}


std::string Socket::MakeQuery(pcstr pairs,...)
{
    std::string s;

    va_list marker;
    va_start(marker,pairs);

    pcstr p = pairs;
    while(NULL!=p)
    {
        if(!s.empty())
            s += '&';

       (s+=p) += '=';
        p = va_arg(marker,pcstr); ASSERT_EXC(p)
        s+= CgiEncode(p);
        p = va_arg(marker,pcstr); 
    }
    
    va_end(marker);

    return s;
}


std::string Socket::SendQuery(pcstr host, ushort port, pcstr url, pcstr query, timeout_t tout)
{
    ASSERT_EXC(host && url)

    std::string get(url);
    if(!empty(query))
        (get += '?') += query;

    util::Socket socket(128); // 128 bytes buffer
    socket.Connect(host,port);
    socket.Send(sformat("GET %s HTTP/1.0\n"
                        "Host: %s\n"
                        // "User-Agent: ...\n",
                        "\n",
                        get.c_str(), host));
    socket.Flush();

    std::string line, res;
    while(util::Socket::SOCK_RECEIVED==socket.ReceiveLine(line,tout))
        res += (line + '\n');

    return res;
}


void Socket::SendNonBuffered(pcvoid buf,size_t count)
{
    ASSERT_EXC(NULL!=buf)

    pcstr pb = (pcstr)buf;
    if(SOCKET_ERROR==send(m_sock,pb,count,0))
    {
		THROW_EXC2(SEND_FAILED,WSA::LastError())
    }
}


//-----------------------------------------------------------------------------

} // namespace

//** eof **********************************************************************
