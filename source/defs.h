#define __PAIR__(high, low) (((unsigned long)(high)<<sizeof(high)*8) | low)
#define __PAIR64__(high, low)   (((unsigned long long) (high) << 32) | (unsigned int)(low))
#define _mm_set_epi64x(m0, m1) _mm_set_epi64(_m_from_int64(m0), _m_from_int64(m1))

#define LAST_IND(x,part_type)    (sizeof(x)/sizeof(part_type) - 1)
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN
#  define LOW_IND(x,part_type)   LAST_IND(x,part_type)
#  define HIGH_IND(x,part_type)  0
#else
#  define HIGH_IND(x,part_type)  LAST_IND(x,part_type)
#  define LOW_IND(x,part_type)   0
#endif

#define BYTEn(x, n)   (*((unsigned char*)&(x)+n))
#define DWORDn(x, n)  (*((long long*)&(x)+n))

#define LODWORD(x) DWORDn(x,LOW_IND(x,long long))
#define HIDWORD(x) DWORDn(x,HIGH_IND(x,long long))
#define BYTE1(x)   BYTEn(x,  1)

#include <stddef.h> // for size_t

// memcpy() with determined behavoir: it always copies
// from the start to the end of the buffer
// note: it copies byte by byte, so it is not equivalent to, for example, rep movsd
inline void* qmemcpy(void* dst, const void* src, size_t cnt)
{
	char* out = (char*)dst;
	const char* in = (const char*)src;
	while (cnt > 0)
	{
		*out++ = *in++;
		--cnt;
	}
	return dst;
}

#include <string.h>     // for memcpy
#include <type_traits>  // for enable_if

template <typename T, typename F>
inline typename std::enable_if<sizeof(T) <= sizeof(F), T>::type __coerce(F f)
{
	T t;
	memcpy(&t, &f, sizeof(T));
	return t;
}
#define COERCE_FLOAT(v) __coerce<float>(v)
#define COERCE_DOUBLE(v) __coerce<double>(v)
#define COERCE_LONG_DOUBLE(v) __coerce<long double>(v)
#define COERCE_UNSIGNED_INT(v) __coerce<unsigned int>(v)
#define COERCE_UNSIGNED_INT64(v) __coerce<uint64>(v)