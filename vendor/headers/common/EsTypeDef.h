#ifndef _ES_TYPEDEF_H_
#define _ES_TYPEDEF_H_

#ifndef IN
#	define	IN
#endif

#ifndef OUT
#	define	OUT
#endif
#ifndef INOUT
#	define	INOUT
#endif

#ifdef _WIN32_WINNT
#	undef	_WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0501

#if defined(_WINDOWS)||defined(WIN32)
	#ifdef _WP
		#define	_OS_WPHONE_
	#else
		#define	_OS_WINDOWS_
	#endif
#endif

//////////////////////////////////////////////////////////////////////////
// zhouxiao@20181219
// 关于整型别名的定义问题
// 1. 有些项目使用比特表示法，u8/s8会出现冲突
// 2. 若全部采用新命名体系，不管是字节还是比特表示，现有代码修改量巨大，难以估量
// 3. u8/s8的使用较少，故采用混合方式，保留1、2和4字节整型的命名法
// 4. 混合命名会给开发者带来一定困扰，但这是目前最优策略
//////////////////////////////////////////////////////////////////////////
#ifndef _U1U2U4_
#define _U1U2U4_
typedef unsigned char		u1;
typedef unsigned short		u2;
typedef signed char			s1;
typedef signed short		s2;

#if defined(_OS_WINDOWS_) || defined(_OS_WPHONE_)
	typedef unsigned long	    u4;
	typedef signed long		    s4;
    typedef unsigned __int64    u64t;
    typedef signed __int64      s64t;
#else
	typedef unsigned int	    u4;
	typedef signed int		    s4;
    typedef unsigned long long  u64t;
    typedef signed long long    s64t;
#endif
#endif	// END _U1U2U4_

typedef void*				ESHANDLE;

#ifndef ES_PTR
#define ES_PTR *
#endif // END ES_PTR

#ifndef NULL
#	ifdef __cplusplus
#		if !defined(__MINGW32__) && !defined(_MSC_VER)
#			define NULL __null
#		else
#			define NULL 0
#		endif
#	else
#		define NULL ((void*)0)
#	endif
#endif

#ifndef _ES_UTFCHAR_
#define _ES_UTFCHAR_
typedef char ES_UTF8CHAR;
typedef ES_UTF8CHAR ES_PTR ES_UTF8CHAR_PTR;
#endif // END _ES_UTFCHAR_

typedef enum EnBool
{
	ESFALSE = 0,
	ESTRUE = 1,
}ESBOOL;

// KEY charset & language
typedef enum EnCharSet
{
	CHAR_SET_UNKNOWN	= -1,
	CHAR_SET_UTF8		= 0,
	CHAR_SET_GBK		= 1,
	CHAR_SET_GB18030	= 2,
	CHAR_SET_GB2312		= 3,
}EnCharSet;

typedef enum EnLangId
{
	LANGUAGE_ID_UNKNOWN				= -1,
	LANGUAGE_ID_SIMPLIFIED_CHINESE	= 0,
	LANGUAGE_ID_ENGLISH				= 1,
	LANGUAGE_ID_TRADITIONAL_CHINESE = 2,
	LANGUAGE_ID_FRENCH				= 3,
	LANGUAGE_ID_RUSSIAN				= 4,
	LANGUAGE_ID_JAPANESE			= 5,
	LANGUAGE_ID_INDONESIAN			= 6,
	LANGUAGE_ID_GERMAN				= 7,
	LANGUAGE_ID_ARABIC				= 8,
	LANGUAGE_ID_KOREAN				= 9,
	LANGUAGE_ID_KAZAKH				= 10,
	LANGUAGE_ID_THAI				= 11,
	LANGUAGE_ID_POLISH				= 12,
}EnLangId;


#ifdef _OS_WINDOWS_
	#define CALL_TYPE			__stdcall
#else
	#define CALL_TYPE
#endif

#ifndef MAX
#define MAX(x,y) (((x)>(y))? (x):(y))
#endif

#ifndef MIN
#define MIN(x,y) (((x)<(y))? (x):(y))
#endif

#if defined(__cplusplus)
#	define	_BEGIN_C_DECL_	extern "C" {
#	define	_END_C_DECL_	};
#else
#	define	_BEGIN_C_DECL_
#	define	_END_C_DECL_
#endif

#endif // _ES_TYPEDEF_H_
