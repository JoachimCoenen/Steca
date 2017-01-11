/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: licence.cpp                                                   $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/licence.cpp 1     19/08/04 11:31a Ian              $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file licence.cpp
 */

#include "licence.hpp"
#include <string>
#include <time.h>

namespace util {
namespace licence {

//-----------------------------------------------------------------------------
// MD5

struct sContext
{
	uint state[4];     // state (ABCD)
	uint count[2];     // number of bits, modulo 2^64 (lsb first)
	byte buffer[64];   // input buffer
};

// Constants for MD5Transform routine.
static uint const S11 = 7;
static uint const S12 = 12;
static uint const S13 = 17;
static uint const S14 = 22;
static uint const S21 = 5;
static uint const S22 = 9;
static uint const S23 = 14;
static uint const S24 = 20;
static uint const S31 = 4;
static uint const S32 = 11;
static uint const S33 = 16;
static uint const S34 = 23;
static uint const S41 = 6;
static uint const S42 = 10;
static uint const S43 = 15;
static uint const S44 = 21;

static byte PADDING[64] = 
{
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// F, G, H and I are basic MD5 functions.
static inline uint F(uint x, uint y, uint z) 
{
	return ((x & y) | ((~x) & z));
}

static inline uint G(uint x, uint y, uint z) 
{
	return ((x & z) | (y & ~z));
}

static inline uint H(uint x, uint y, uint z) 
{
	return (x ^ y ^ z);
}

static inline uint I(uint x, uint y, uint z) 
{
	return (y ^ (x | ~z));
}

// ROTATE_LEFT rotates x left n bits.
static inline uint ROTATE_LEFT(uint x, uint n) 
{
	return ((x << n) | (x >> (32-n)));
}

// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.
static inline void FF(uint& a, uint b, uint c, uint d, uint x, uint s, uint ac) 
{
	a += F(b,c,d) + x + ac;
	a  = ROTATE_LEFT(a,s);
	a += b;
}

static inline void GG(uint& a, uint b, uint c, uint d, uint x, uint s, uint ac) 
{
	a += G(b,c,d) + x + ac;
	a  = ROTATE_LEFT(a,s);
	a += b;
}

static inline void HH(uint& a, uint b, uint c, uint d, uint x, uint s, uint ac) 
{
	a += H(b,c,d) + x + ac;
	a  = ROTATE_LEFT(a,s);
	a += b;
}

static inline void II(uint& a, uint b, uint c, uint d, uint x, uint s, uint ac) 
{
	a += I(b,c,d) + x + ac;
	a  = ROTATE_LEFT(a,s);
	a += b;
}

// Encodes input (uint) into output (unsigned char). Assumes len is a multiple of 4.
static void Encode(pbyte output,uint* input,uint len)
{
	for(uint i=0, j=0; j<len; i++, j+=4) 
	{
	    output[j]   = (byte)(input[i] & 0xff);
	    output[j+1] = (byte)((input[i] >> 8) & 0xff);
	    output[j+2] = (byte)((input[i] >> 16) & 0xff);
	    output[j+3] = (byte)((input[i] >> 24) & 0xff);
	}
}

// Decodes input (unsigned char) into output (uint). Assumes len is a multiple of 4.
static void Decode(uint* output,pcbyte input,uint len)
{
	for(uint i=0, j=0; j<len; i++, j+=4)
	{
	    output[i] = ((uint)input[j]) | (((uint)input[j+1]) << 8) |
			(((uint)input[j+2]) << 16) | (((uint)input[j+3]) << 24);
	}
}

// MD5 initialization. Begins an MD5 operation, writing a new context.
static void MD5Init(sContext& context)
{
	typedef uint initial_state[4];

	// magic initialization constant
	static initial_state const is =
	{
	    0x67452301,
	    0xefcdab89,
	    0x98badcfe,
	    0x10325476
	};

	context.state[0] = is[0];
	context.state[1] = is[1];
	context.state[2] = is[2];
	context.state[3] = is[3];

	// initialize the counts
	context.count[0] = context.count[1] = 0;
}

// MD5 basic transformation. Transforms state based on block.
static void MD5Transform(uint state[4],byte const block[64])
{
	uint a = state[0], b = state[1], c = state[2], d = state[3], x[16];

	Decode(x,block,64);

	// Round 1
	FF(a,b,c,d,x[ 0],S11,0xd76aa478); // 1
	FF(d,a,b,c,x[ 1],S12,0xe8c7b756); // 2
	FF(c,d,a,b,x[ 2],S13,0x242070db); // 3
	FF(b,c,d,a,x[ 3],S14,0xc1bdceee); // 4
	FF(a,b,c,d,x[ 4],S11,0xf57c0faf); // 5
	FF(d,a,b,c,x[ 5],S12,0x4787c62a); // 6
	FF(c,d,a,b,x[ 6],S13,0xa8304613); // 7
	FF(b,c,d,a,x[ 7],S14,0xfd469501); // 8
	FF(a,b,c,d,x[ 8],S11,0x698098d8); // 9
	FF(d,a,b,c,x[ 9],S12,0x8b44f7af); // 10
	FF(c,d,a,b,x[10],S13,0xffff5bb1); // 11
	FF(b,c,d,a,x[11],S14,0x895cd7be); // 12
	FF(a,b,c,d,x[12],S11,0x6b901122); // 13
	FF(d,a,b,c,x[13],S12,0xfd987193); // 14
	FF(c,d,a,b,x[14],S13,0xa679438e); // 15
	FF(b,c,d,a,x[15],S14,0x49b40821); // 16

	// Round 2
	GG(a,b,c,d,x[ 1],S21,0xf61e2562); // 17
	GG(d,a,b,c,x[ 6],S22,0xc040b340); // 18
	GG(c,d,a,b,x[11],S23,0x265e5a51); // 19
	GG(b,c,d,a,x[ 0],S24,0xe9b6c7aa); // 20
	GG(a,b,c,d,x[ 5],S21,0xd62f105d); // 21
	GG(d,a,b,c,x[10],S22, 0x2441453); // 22
	GG(c,d,a,b,x[15],S23,0xd8a1e681); // 23
	GG(b,c,d,a,x[ 4],S24,0xe7d3fbc8); // 24
	GG(a,b,c,d,x[ 9],S21,0x21e1cde6); // 25
	GG(d,a,b,c,x[14],S22,0xc33707d6); // 26
	GG(c,d,a,b,x[ 3],S23,0xf4d50d87); // 27
	GG(b,c,d,a,x[ 8],S24,0x455a14ed); // 28
	GG(a,b,c,d,x[13],S21,0xa9e3e905); // 29
	GG(d,a,b,c,x[ 2],S22,0xfcefa3f8); // 30
	GG(c,d,a,b,x[ 7],S23,0x676f02d9); // 31
	GG(b,c,d,a,x[12],S24,0x8d2a4c8a); // 32

	// Round 3
	HH(a,b,c,d,x[ 5],S31,0xfffa3942); // 33
	HH(d,a,b,c,x[ 8],S32,0x8771f681); // 34
	HH(c,d,a,b,x[11],S33,0x6d9d6122); // 35
	HH(b,c,d,a,x[14],S34,0xfde5380c); // 36
	HH(a,b,c,d,x[ 1],S31,0xa4beea44); // 37
	HH(d,a,b,c,x[ 4],S32,0x4bdecfa9); // 38
	HH(c,d,a,b,x[ 7],S33,0xf6bb4b60); // 39
	HH(b,c,d,a,x[10],S34,0xbebfbc70); // 40
	HH(a,b,c,d,x[13],S31,0x289b7ec6); // 41
	HH(d,a,b,c,x[ 0],S32,0xeaa127fa); // 42
	HH(c,d,a,b,x[ 3],S33,0xd4ef3085); // 43
	HH(b,c,d,a,x[ 6],S34, 0x4881d05); // 44
	HH(a,b,c,d,x[ 9],S31,0xd9d4d039); // 45
	HH(d,a,b,c,x[12],S32,0xe6db99e5); // 46
	HH(c,d,a,b,x[15],S33,0x1fa27cf8); // 47
	HH(b,c,d,a,x[ 2],S34,0xc4ac5665); // 48

	// Round 4 
	II(a,b,c,d,x[ 0],S41,0xf4292244); // 49
	II(d,a,b,c,x[ 7],S42,0x432aff97); // 50
	II(c,d,a,b,x[14],S43,0xab9423a7); // 51
	II(b,c,d,a,x[ 5],S44,0xfc93a039); // 52
	II(a,b,c,d,x[12],S41,0x655b59c3); // 53
	II(d,a,b,c,x[ 3],S42,0x8f0ccc92); // 54
	II(c,d,a,b,x[10],S43,0xffeff47d); // 55
	II(b,c,d,a,x[ 1],S44,0x85845dd1); // 56
	II(a,b,c,d,x[ 8],S41,0x6fa87e4f); // 57
	II(d,a,b,c,x[15],S42,0xfe2ce6e0); // 58
	II(c,d,a,b,x[ 6],S43,0xa3014314); // 59
	II(b,c,d,a,x[13],S44,0x4e0811a1); // 60
	II(a,b,c,d,x[ 4],S41,0xf7537e82); // 61
	II(d,a,b,c,x[11],S42,0xbd3af235); // 62
	II(c,d,a,b,x[ 2],S43,0x2ad7d2bb); // 63
	II(b,c,d,a,x[ 9],S44,0xeb86d391); // 64

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	// Zero sensitive information
	memset(x,0,sizeof x);
}

// MD5 block update operation. Continues an MD5 message-digest
// operation, processing another message block, and updating the context.
static void MD5Update(sContext& context,pcbyte input,uint inputLen)
{
	// Compute number of bytes mod 64
	uint index = ((context.count[0] >> 3) & 0x3F);

	// Update number of bits
	if((context.count[0] += (inputLen << 3)) < (inputLen << 3))
	    context.count[1]++;
  
	context.count[1] += (inputLen >> 29);
	
	uint partLen = 64 - index;

	// Transform as many times as possible.
	uint i;

	if(inputLen >= partLen) 
	{
	    memcpy(&context.buffer[index],input,partLen);
	    MD5Transform(context.state,context.buffer);

	    for(i=partLen; i+63 < inputLen; i += 64)
	    {
		    MD5Transform(context.state,&input[i]);
	    }

	    index = 0;
	}
	else
	{
	    i = 0;
	}

	// Buffer remaining input
	memcpy(&context.buffer[index],&input[i],inputLen-i);
}


// MD5 finalization. Ends an MD5 message-digest operation, writing the
// the message digest and zeroizing the context.
static void MD5Final(pbyte digest/*[16]*/,sContext& context)
{
	byte bits[8];

	// Save number of bits
	Encode(bits,context.count,8);

	// Pad out to 56 mod 64.
	uint index  = ((context.count[0] >> 3) & 0x3f);
	uint  padLen = (index < 56) ? (56 - index) : (120 - index);
	MD5Update(context,PADDING,padLen);

	// Append length (before padding)
	MD5Update(context,bits,8);

	// Store state in digest
	Encode(digest,context.state,16);

	// Zero sensitive information.
	memset(&context,0,sizeof context);
}


static memory MD5(memory const& m)
{
	sContext context;
    byte     digest[16];

    MD5Init  (context);
	MD5Update(context,(pcbyte)m.data(),m.size());
	MD5Final (digest,context);

    return memory(digest,sizeof digest);
}


//----------------------------------------------------------------------------
    
// Two "random" messages, disguised as code.
static byte LICRND[2][256] =
{{0x55,0x8B,0xEC,0x6A,0xFF,0x68,0x18,0x44,0x40,0x00,0x68,0xA4,0x37,0x40,0x00,0x64,
  0xA1,0x00,0x00,0x50,0x64,0x89,0x25,0x00,0x00,0x00,0x00,0x83,0xEC,0x20,0x53,0x56,
  0x57,0x89,0x65,0xE8,0x83,0x65,0xFC,0x00,0x6A,0x01,0xFF,0x15,0x40,0x42,0x40,0x00,
  0x59,0x83,0x0D,0x1C,0x63,0x40,0x00,0xFF,0x83,0x0D,0x20,0x63,0x40,0x00,0xFF,0xFF,
  0x15,0x44,0x42,0x40,0x00,0x8B,0x0D,0x14,0x63,0x40,0x00,0x89,0x08,0xFF,0x15,0x48,
  0x42,0x40,0x00,0x8B,0x0D,0x10,0x63,0x40,0x00,0x89,0x08,0xA1,0x4C,0x42,0x40,0x00,
  0x8B,0x00,0xA3,0x18,0x63,0x40,0x00,0xE8,0xCF,0x00,0x83,0x3D,0xE8,0x61,0x40,0x00,
  0x00,0x75,0x0C,0x68,0xA0,0x37,0x40,0x00,0xFF,0x15,0x1C,0x42,0x40,0x00,0x59,0xE8,
  0xA0,0x00,0x00,0x00,0x68,0x14,0x60,0x40,0x00,0x68,0x10,0x60,0x40,0x00,0xE8,0x8B,
  0x00,0x00,0x00,0xA1,0x0C,0x63,0x40,0x00,0x89,0x45,0xD8,0x8D,0x45,0xD8,0x50,0xFF,
  0x35,0x08,0x63,0x40,0x00,0x8D,0x45,0xE0,0x50,0x8D,0x45,0xD4,0x50,0x8D,0x45,0xE4,
  0x50,0xFF,0x15,0x14,0x42,0x40,0x00,0x68,0x0C,0x60,0x40,0x00,0x68,0x00,0x60,0x40,
  0x00,0xE8,0x58,0x00,0x00,0x00,0xFF,0x15,0x10,0x42,0x40,0x00,0x8B,0x4D,0xE0,0x89,
  0x08,0xFF,0x75,0xE0,0xFF,0x75,0xD4,0xFF,0x75,0xE4,0xE8,0x27,0xE1,0xFF,0xFF,0x83,
  0xC4,0x30,0x89,0x45,0xDC,0x50,0xFF,0x15,0x0C,0x42,0x40,0x00,0x8B,0x45,0xEC,0x8B,
  0x08,0x8B,0x09,0x89,0x4D,0xD0,0x50,0x51,0xE8,0x1B,0x00,0x00,0x00,0x59,0x59,0xC3},
 {0x55,0x8B,0xEC,0x6A,0xFF,0x68,0x18,0x44,0x40,0x00,0x68,0xA4,0x37,0x40,0x00,0x64,
  0xA1,0x00,0x00,0x50,0x64,0x89,0x25,0x00,0x00,0x00,0x00,0x83,0xEC,0x20,0x53,0x56,
  0x57,0x89,0x65,0xE8,0x83,0x65,0xFC,0x00,0x6A,0x01,0xFF,0x15,0x40,0x42,0x40,0x00,
  0x59,0x83,0x0D,0x1C,0x63,0x40,0x00,0xFF,0x83,0x0D,0x20,0x63,0x40,0x00,0xFF,0xFF,
  0x15,0x44,0x42,0x40,0x00,0x8B,0x0D,0x14,0x63,0x40,0x00,0x89,0x08,0xFF,0x15,0x48,
  0x42,0x40,0x00,0x8B,0x0D,0x10,0x63,0x40,0x00,0x89,0x08,0xA1,0x4C,0x42,0x40,0x00,
  0x8B,0x00,0xA3,0x18,0x63,0x40,0x00,0xE8,0xCF,0x00,0x83,0x3D,0xE8,0x61,0x40,0x00,
  0x00,0x75,0x0C,0x68,0xA0,0x37,0x40,0x00,0xFF,0x15,0x1C,0x42,0x40,0x00,0x59,0xE8,
  0xA0,0x00,0x00,0x00,0x49,0x47,0x42,0x20,0x77,0x61,0x73,0x20,0x68,0x65,0x72,0x65,
  0x20,0x69,0x6E,0x20,0x43,0x43,0x49,0x54,0x20,0x30,0x30,0x34,0x20,0x69,0x6E,0x20,
  0x74,0x68,0x65,0x20,0x63,0x6F,0x6C,0x64,0x20,0x73,0x75,0x6D,0x6D,0x65,0x72,0x20,
  0x6F,0x66,0x20,0x32,0x30,0x30,0x34,0x2E,0x20,0x50,0x72,0x65,0x73,0x20,0x6C,0x69,
  0x76,0x65,0x73,0x21,0x00,0x00,0xFF,0x15,0x10,0x42,0x40,0x00,0x8B,0x4D,0xE0,0x89,
  0x08,0xFF,0x75,0xE0,0xFF,0x75,0xD4,0xFF,0x75,0xE4,0xE8,0x27,0xE1,0xFF,0xFF,0x83,
  0xC4,0x30,0x89,0x45,0xDC,0x50,0xFF,0x15,0x0C,0x42,0x40,0x00,0x8B,0x45,0xEC,0x8B,
  0x08,0x8B,0x09,0x89,0x4D,0xD0,0x50,0x51,0xE8,0x1B,0x00,0x00,0x00,0x59,0x59,0xC3}
};

  
static char const b64table[64] =
{
    'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
    '0','1','2','3','4','5','6','7','8','9','+','/'
};


static void EncodeB64Triplet(pcbyte pIn,uint inSize/*1..3*/,char out[4],uint& outSize/*2..4*/)
{
    ASSERT_EXC(inSize>=1 && inSize<=3)
    switch(inSize)
    {
    case 1:
	    outSize = 2;
	    break;
    case 2:
	    outSize = 3;
	    break;
    case 3:
	    outSize = 4;
	    break;
    }

    #pragma pack(push,1)
    union
    {
	    uint ui;
	    byte b[4];
    } 
    u;
    #pragma pack(pop)

    ASSERT_EXC(sizeof(u.ui)==sizeof(u.b))

    u.ui = 0; uint i;
    for(i=0; i<inSize; i++)
    {
	    u.b[i] = *(pIn++);
    }

    for(i=0; i<outSize; i++)
    {
	    uint b64i = u.ui%64;  
	    u.ui/=64;

	    out[i] = b64table[b64i];
    }
}


static std::string Base64Encode(memory const& m)
{
    std::string result;

    pcbyte p    = m.data();
    uint   size = m.size();

    while(size>0)
    {
        uint inSize = min(size,(uint)3);

        char out[4]; 
        uint outSize;

        EncodeB64Triplet(p,inSize,out,outSize);

        for(uint i=0; i<outSize; i++)
        {
            result += out[i];
        }

        p    += inSize;
        size -= inSize;
    }

    return result;
}

std::string Digest(pcstr s,byte rnd)
{
    ASSERT_EXC(!empty(s))
    memory md = MD5(memory(LICRND[0],sizeof LICRND[0])+memory(&rnd,sizeof(rnd)) + memory((pcbyte)s,strlen(s)));
    return Base64Encode(md);
}


std::string MakeKey(pcstr productId,uint licNo,pcstr licName)
{
    ASSERT_EXC(productId && licName)

    static bool seeded = false;
    if(!seeded)
    {
        srand(time(NULL)^0xA5E40F8B);
        seeded=true;
    }

    return Digest(sformat("%s%u%s",productId,licNo,licName).c_str(),rand()&63);
}


bool Verify(pcstr productId,uint licNo,pcstr licName,pcstr licKey)
{
    ASSERT_EXC(productId && licName && licKey)
    std::string s = sformat("%s%u%s",productId,licNo,licName);
    for(int i=0; i<=64; i++)
    {
        if(licKey==Digest(s.c_str(),i))
            return true;
    }

    return false;
}


//-----------------------------------------------------------------------------

} // namespace
} // namespace

//** eof **********************************************************************
