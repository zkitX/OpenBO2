#define __PAIR__(high, low) (((unsigned long)(high)<<sizeof(high)*8) | low)
#define _mm_set_epi64x(m0, m1) _mm_set_epi64(_m_from_int64(m0), _m_from_int64(m1))

#define LAST_IND(x,part_type)    (sizeof(x)/sizeof(part_type) - 1)
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN
#  define LOW_IND(x,part_type)   LAST_IND(x,part_type)
#  define HIGH_IND(x,part_type)  0
#else
#  define HIGH_IND(x,part_type)  LAST_IND(x,part_type)
#  define LOW_IND(x,part_type)   0
#endif

#define DWORDn(x, n)  (*((long long*)&(x)+n))

#define LODWORD(x) DWORDn(x,LOW_IND(x,long long))
#define HIDWORD(x) DWORDn(x,HIGH_IND(x,long long))