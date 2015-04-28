/****
Copyright 2015 Alexej Magura

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****/
#ifndef COMMON_MAIN_H_GUARD
# define COMMON_MAIN_H_GUARD 1
/* BEGIN_C_DECLS should be used at the beginning of your declarations,
so that C++ compilers don't mangle their names.  Use END_C_DECLS at
the end of C declarations. */
#undef BEGIN_C_DECLS
#undef END_C_DECLS
#ifdef __cplusplus
# define BEGIN_C_DECLS extern "C" {
# define END_C_DECLS }
#else
# define BEGIN_C_DECLS /* empty */
# define END_C_DECLS /* empty */
#endif

/* PARAMS is a macro used to wrap function prototypes, so that
  compilers that don't understand ANSI C prototypes still work,
  and ANSI C compilers can issue warnings about type mismatches. */
#undef PARAMS
#if defined (__STDC__) || defined (_AIX) \
       || (defined (__mips) && defined (_SYSTYPE_SVR4)) \
       || defined(WIN32) || defined(__cplusplus)
# define PARAMS(protos) protos
#else
# define PARAMS(protos) ()
#endif

BEGIN_C_DECLS

# include <stdio.h>
# include <stdlib.h>

# if !defined(COM_INCLUDE_STRING_H)
#  define COM_INCLUDE_STRING_H 0
# endif

# if COM_INCLUDE_STRING_H
#  include <string.h>
# else
void *memset PARAMS((void *s, int c, size_t n));
void *memcpy PARAMS((void *dest, const void *src, size_t n));
void *memmove PARAMS((void *dest, const void *src, size_t n));
# endif

# if !defined(COM_TESTING)
#  define COM_TESTING 0 /* use this to enable functions that are not yet deemed stable */
# endif

# if !defined(COM_DEBUG)
#  define COM_DEBUG 0 // XXX change this to turn debug messages on/off
# endif

# if COM_DEBUG
#  if !defined(COM_DLVL)
#   define COM_DLVL (COM_DEBUG + 1) // XXX change this to increase/decrease debug verbosity
#  endif
# endif

# if !defined(COM_INLINE)
#  define COM_INLINE 1 /* XXX change this to disable/enable use of inline
			* functions vs macros in certain situations */
# endif

# if !defined(COM_MACRO)
#  define COM_MACRO 0 /* XXX change this enable/disable use of macros vs inline
		       * functions in certain situations.
		       * XXX change both this and `COM_INLINE' to `0' to
		       * use external defintions */
# endif

# if !defined(PACKAGE_VERSION)
#  define PACKAGE_VERSION ""
# endif

# if !defined(PACKAGE)
#  define PACKAGE "common"
# endif

# if !defined(COM_PROGNAME)
#  define COM_PROGNAME PACKAGE
# endif

# if COM_DEBUG
#  if defined(__linux__) || defined(__gnu_linux__)
#   include <mcheck.h>
#  endif
#  define COM_DBG(format, ...)					\
     do {							\
	  fprintf(stderr, "## (%s)(%s)%d\n",			\
		  COM_PROGNAME, __FILE__, __LINE__);		\
	  fprintf(stderr, "#  `%s'\n", __FUNCTION__);		\
	  fprintf(stderr, (format), ##__VA_ARGS__);		\
	  fprintf(stderr, "\n");	       			\
     } while(0)
#  define COM_SDBG(format, exp)				\
     do {						\
	  fprintf(stderr, "## (%s)(%s)%d\n",		\
		  COM_PROGNAME, __FILE__, __LINE__);	\
	  fprintf(stderr, "#  `%s`\n", __FUNCTION__);	\
	  fprintf(stderr, (format), (exp));		\
	  fprintf(stderr, "\n");			\
     } while(0)
#  define COM_ONDBG(...) (__VA_ARGS__)
#  define COM_XONDBG(COM_X) COM_X
#  define com_ping COM_DBG("\n^^^^ %s ^^^^\n", "MARCO!")
#  define com_pong COM_DBG("\n$$$$ %s $$$$\n", "POLO!")
#  define com_neko(COM_F, ...) \
     do {								\
	  COM_DBG("\n%s: %s %s %s",					\
		  "neko-chan",						\
		  "nyaa, nya~",						\
		  "<3", "\n~(=^..^)/\n\n");					\
	  fprintf(stderr, (COM_F), ##__VA_ARGS__);			\
	  fprintf(stderr, "\n");					\
     } while(0)
# else
#  define COM_DBG(format, ...)
#  define COM_SDBG(format, exp)
#  define COM_ONDBG(...)
#  define COM_XONDBG(COM_X)
#  define com_ping
#  define com_pong
#  define com_neko(COM_F, ...)
# endif

# define COM_ERROR(format, ...)				\
     do {						\
	  fprintf(stderr, "%s:err: ", COM_PROGNAME);	\
	  fprintf(stderr, (format), __VA_ARGS__);	\
	  fprintf(stderr,				\
		  "\nin %s:{%d}:%s()\n",		\
		  __FILE__,				\
		  __LINE__,				\
		  __FUNCTION__);			\
     } while(0)

# define COM_FATAL(...)			\
     do {				\
	  fprintf(stderr,		\
		  "%s: %s\n",		\
		  (COM_PROGNAME),	\
		  ##__VA_ARGS__);	\
	  exit(EXIT_FAILURE);		\
     } while (0)

# define com_usage(format) (printf((format), (COM_PROGNAME)));

# define com_arg(opt, desc, tabs) (printf("  %s%s%s\n", (opt),(tabs),(desc)));

# define com_arg_eol_tabs "\n\t\t\t\t"
# define com_help(usage, tabs)				\
     do {						\
	  com_usage((usage));				\
	  com_arg("-h, --help",				\
		  "print this message and exit",	\
		  (tabs));				\
	  com_arg("-v, --version",			\
		  "print program version and exit",	\
		  (tabs));				\
     } while(0)

# if HAVE_LIBBSD
#  include <limits.h>
#  include <bsd/stdlib.h>
// FIXME, the following macros shouldn't call `exit' or `perror'.
#  define COM_STRTONUM(dst_num, const_string)			\
     do {							\
	  errno = 0;						\
	  ((dst_num) = strtonum((const_string),			\
				INT_MIN,			\
				INT_MAX,			\
				NULL));				\
	  if (errno != 0) {					\
	       perror(COM_PROGNAME);				\
	       exit(EXIT_FAILURE);				\
	  }							\
     } while(0)
# else
#  define COM_STRTONUM(dst_num, const_string)			\
     do {							\
	  errno = 0;						\
	  ((dst_num) = strtol((const_string), NULL, 10));	\
	  if (errno != 0) {					\
	       perror(COM_PROGNAME);				\
	       exit(EXIT_FAILURE);				\
	  }							\
     } while(0)
# endif

# ifndef bzero
#  define bzero(COM_B, COM_LEN)						\
     (memset((void *)(COM_B), '\0', (size_t)(COM_LEN)), (void)0)
# endif

# ifndef bcopy
#  define bcopy(COM_B1, COM_B2, COM_LEN)			\
     (memmove((void *)(COM_B2),					\
	      (const void *)(COM_B1),				\
	      (size_t)(COM_LEN)),				\
      (void)0)
# endif

# ifndef mempcpy
#  define mempcpy(COM_D, COM_S, COM_L)		\
     (memcpy((void *)(COM_D),			\
	     (const void *)(COM_S),		\
	     (size_t)(COM_L))			\
      + (size_t)(COM_L))
# endif

# if COM_INLNE
inline void *mempmove(void *dst, const void *src, size_t n)
{
     return (memmove(dst, src, n) + n);
}
# else
#  define mempmove(COM_D, COM_S, COM_L)		\
     (memmove((void *)(COM_D),			\
	      (const void *)(COM_S),		\
	      (size_t)(COM_L))			\
      + (size_t)(COM_L))
# endif


/** Function Prototypes **/

/** cpeek: access the character either before or after the current character
 ** in an array of characters.  **/
char cpeek PARAMS((const char *c, const char *s, const short fwd));

/** intlen: find how many digits a given integral contains. **/
int intlen PARAMS((int n));
int intlenc PARAMS((const int n));
size_t intlenm PARAMS((int src)); /* XXX for use with malloc'ing for
				   * calls to `itoa' */

/** rev: reverse an array of characters **/
void rev PARAMS((char *s));
char *revp PARAMS((const char *s)); /* XXX return value needs free */
void revn PARAMS((char *s, size_t n)); /* XXX `s' does not need to null-terminated */

/** itoa: convert a number to an atom (i.e. string) **/
void itoa PARAMS((char *dst, int src));
char *itoap PARAMS((int src));

/** concat: catenate several strings together **/
char *concat PARAMS((const char *s1, ...)); // XXX return value needs free
size_t concatl PARAMS((char *dest, size_t destsiz, const char *s1, ...));
size_t concatm PARAMS((char *dest, size_t destsiz, const char *s1, ...));
# undef cat
# define cat(...) (concat(__VA_ARGS__, (void *)NULL))
# undef catl
# define catl(...) (concatl(__VA_ARGS__, (void *)NULL))
# undef catm
# define catm(...) (concatm(__VA_ARGS__, (void *)NULL))

# if 0
/** concatl: catenate as many _s_ource strings into `buf'
 ** as will fit in `bufsiz' bytes **/
/** XXX if you _must_ use concatl directly: be sure to pass `(void *)NULL' as
 ** the last argument **/
size_t concatl PARAMS((char *buf, size_t bufsiz, const char *s1, ...));
# undef catl
# define catl(...) (concatl(__VA_ARGS__, (void *)NULL))
# endif

/** repeat: create an array of chars containing n-1 many _s_ chars **/
void repeat PARAMS((char *dst, const char s, size_t n));
# if COM_TESTING
int strrep PARAMS((char *dst, const char *s, size_t n));
char *strprep PARAMS((const char *s, int x));
# endif

/** strndelim: counts number of delimiters within a given string **/
int *strndelim PARAMS((const char *s, const char od, const char cd, int count[2]));

/** strwodq: remove double quotes from a string **/
int strwodq PARAMS((char *dst, const char *src, size_t n));
char *strwodqp PARAMS((const char *src));

END_C_DECLS

#endif /* COMMON_MAIN_H_GUARD */