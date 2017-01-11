/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: socket.hpp                                                    $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/socket.hpp 3     9/07/04 3:02p Ian                 $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  socket.hpp
 *  \brief tcp sockets
 */

#ifndef UTIL_SOCKET_HPP_INCLUDED
#define UTIL_SOCKET_HPP_INCLUDED

#include "../types/types.hpp"
#include "array.hpp"

namespace util {

//----------------------------------------------------------------------------

class Socket
{
    NO_GEN(Socket)

public:
    #define EXC_IDS  EMPTY_NAME,WSA_NOT_STARTED,CANNOT_RESOLVE_HOST,CANNOT_CONNECT,SEND_FAILED,RECV_FAILED
    DECLARE_EXC_ROOT
    #undef EXC_IDS

    typedef uint timeout_t;

	Socket(uint sendBufferSize);    // 0..nonbuffered
   ~Socket();

    bool IsValid() const;

    void Connect(pcstr name,ushort port);
    void Close();
    
    void Send(byte);
    void Send(pcvoid data,size_t);
    void Send(pcstr);
    void Send(std::string const& s)
    {
        Send(s.c_str());
    }

    void Flush();

public:
	bool WaitForData(timeout_t sec);

    enum eStatus
    {
        SOCK_RECEIVED,
        SOCK_CLOSED,
        SOCK_TIMEOUT
    };

	eStatus Receive(byte&,timeout_t sec);

    eStatus Receive(pvoid buf,size_t,timeout_t sec,uint& received);
    eStatus ReceiveLine(std::string& line,timeout_t sec);

    static std::string CgiEncode(pcstr); ///< Encode text for cgi query.
    static std::string MakeQuery(pcstr,...);

    static std::string SendQuery(pcstr host,ushort port,pcstr url,pcstr query,timeout_t);

private:
    void SendNonBuffered(pcvoid buf,size_t);

    array1d<byte> m_buffer;
    uint          m_buffered;
	uint          m_sock;
    bool          m_receiveClosed;
};

//-----------------------------------------------------------------------------

  } // namespace

#endif

//** eof **********************************************************************
