/* include/setup.h.  Generated from setup.h.in by configure.  */
/* include/setup.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Size of away heap. */
#define AWAY_HEAP_SIZE 512

/* Size of the ban heap. */
#define BAN_HEAP_SIZE 4096

/* Size of the channel heap. */
#define CHANNEL_HEAP_SIZE 8192

/* Define this if you are profiling. */
/* #undef CHARYBDIS_PROFILE */

/* Version of charybdis on which this release is based */
#define CHARYBDIS_VERSION "charybdis-4"

/* Size of the client heap. */
#define CLIENT_HEAP_SIZE 8192

/* Size of the confitem heap. */
#define CONFITEM_HEAP_SIZE 256

/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
   */
/* #undef CRAY_STACKSEG_END */

/* Define to 1 if using `alloca.c'. */
/* #undef C_ALLOCA */

/* Size of the dlink_node heap. */
#define DNODE_HEAP_SIZE 8192

/* Prefix where config files are installed. */
#define ETC_DIR "/home/matty/ircd/etc"

/* Size of fd heap. */
#define FD_HEAP_SIZE 1024

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
   */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the <crypt.h> header file. */
#define HAVE_CRYPT_H 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `dlfunc' function. */
/* #undef HAVE_DLFUNC */

/* Define if the dlopen function is available. */
#define HAVE_DLOPEN 1

/* Define if your system supports the epoll system calls */
#define HAVE_EPOLL 1

/* Define to 1 if you have the `epoll_ctl' function. */
#define HAVE_EPOLL_CTL 1

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `kevent' function. */
/* #undef HAVE_KEVENT */

/* Define to 1 if you have the `crypto' library (-lcrypto). */
#define HAVE_LIBCRYPTO 1

/* Define to 1 if zlib (-lz) is available. */
/* #undef HAVE_LIBZ */

/* Define to 1 if you have the <machine/endian.h> header file. */
/* #undef HAVE_MACHINE_ENDIAN_H */

/* Define to 1 if you have the <mach-o/dyld.h> header file. */
/* #undef HAVE_MACH_O_DYLD_H */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mmap' function. */
#define HAVE_MMAP 1

/* Define if nanosleep exists */
#define HAVE_NANOSLEEP 1

/* Define to 1 if you have the `poll' function. */
#define HAVE_POLL 1

/* Define to 1 if you have the `port_getn' function. */
/* #undef HAVE_PORT_GETN */

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define if the shl_load function is available. */
/* #undef HAVE_SHL_LOAD */

/* Define to 1 if you have the `socketpair' function. */
#define HAVE_SOCKETPAIR 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlcat' function. */
/* #undef HAVE_STRLCAT */

/* Define to 1 if you have the `strlcpy' function. */
/* #undef HAVE_STRLCPY */

/* Define to 1 if you have the `strndup' function. */
#define HAVE_STRNDUP 1

/* Define to 1 if you have the <sys/epoll.h> header file. */
#define HAVE_SYS_EPOLL_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/resource.h> header file. */
#define HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/syslog.h> header file. */
#define HAVE_SYS_SYSLOG_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header file. */
#define HAVE_SYS_UIO_H 1

/* Define to 1 if you have the <sys/wait.h> header file. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if the system has the type `uintptr_t'. */
#define HAVE_UINTPTR_T 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Define to 1 if you have the <wait.h> header file. */
#define HAVE_WAIT_H 1

/* Prefix where help file are installed. */
#define HELP_DIR "/home/matty/ircd/help"

/* Prefix where the ircd is installed. */
#define IRCD_PREFIX "/home/matty/ircd"

/* Size of the local client heap. */
#define LCLIENT_HEAP_SIZE 1024

/* Size of the linebuf heap. */
#define LINEBUF_HEAP_SIZE 2048

/* Prefix where to write logfiles. */
#define LOG_DIR "/home/matty/ircd/logs"

/* Sizeof member heap. */
#define MEMBER_HEAP_SIZE 32768

/* Prefix where modules are installed. */
#define MODULE_DIR "/home/matty/ircd/modules"

/* Size of the monitor heap. */
#define MONITOR_HEAP_SIZE 1024

/* Define this to disable debugging support. */
#define NDEBUG 1

/* Size of the nick delay heap. */
#define ND_HEAP_SIZE 512

/* Nickname length */
#define NICKLEN (15+1)

/* Size of the WHOWAS array. */
#define NICKNAMEHISTORYLENGTH 15000

/* Define to 1 if you wish to disable the block allocator. */
/* #undef NOBALLOC */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "aspIRCd"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "aspIRCd 5.0.9-devel"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "aspircd"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "5.0.9-devel"

/* Path to /dev/null */
#define PATH_DEVNULL "/dev/null"

/* Size of the pre-client heap. */
#define PCLIENT_HEAP_SIZE 256

/* This is the type of IO loop we are using */
#define SELECT_TYPE "epoll"

/* Suffix for shared libraries on this platform. */
#define SHARED_SUFFIX ".so"

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 8

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* Define this to enable soft asserts. */
/* #undef SOFT_ASSERT */

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at runtime.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define to 1 if dynamic modules can't be used. */
/* #undef STATIC_MODULES */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if string.h may be included along with strings.h */
#define STRING_WITH_STRINGS 1

/* String containing extra underscores prepended to symbols loaded from
   modules. */
#define SYMBOL_PREFIX ""

/* Maximum topic length (<=390) */
#define TOPICLEN 390

/* Size of the topic heap. */
#define TOPIC_HEAP_SIZE 4096

/* Size of the user heap. */
#define USER_HEAP_SIZE 8192

/* Define this to enable IO Debug hooks. */
/* #undef USE_IODEBUG_HOOKS */

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
/* #undef YYTEXT_POINTER */

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */

/* This is needed to use strtok_r on Solaris. */
#define __EXTENSIONS__ 1

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* If system does not define in_port_t, define it to what it should be. */
/* #undef in_port_t */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* If system does not define sa_family_t, define it here. */
/* #undef sa_family_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* If we don't have a real socklen_t, unsigned int is good enough. */
/* #undef socklen_t */

/* If system does not define u_int16_t, define a usable substitute. */
/* #undef u_int16_t */

/* If system does not define u_int32_t, define to unsigned long int here. */
/* #undef u_int32_t */
