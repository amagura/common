/****
  COMMON; str.c, String related functions

  Copyright (C) 2015, 2016, 2017 Alexej G. Magura

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
****/
#ifndef COMMON_STRING_C_GUARD
# define COMMON_STRING_C_GUARD 1
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "common.h"
# include "repeat.c"
# include "rev.c"
# include "concat.c"

/////////////////////////////////////////
// End of defunct mem.c
/////////////////////////////////////////

/* does not change the contents of s0 or change
 * the memory location that s0 points to.
 *
 * returns the end of s0 (not the null terminator)
 */
const char *strend(const char *const s0)
{
# if defined(_GNU_SOURCE)
     const char *endp = strchr(s0, '\0') - 1;
# else
     const char *endp = (const char *)&s0[strlen(s0)];
# endif
     if (s0 != NULL && *s0 != '\0')
          return (s0 == endp ? s0 : endp);
     return NULL;
}

# if defined(COM_EXPOSE_OLD_CPEEK)
char old_cpeek(const char *c, const char *s, const short fwd)
{
     if (fwd > 0) {
	  if (*c == '\0'
# if defined(_GNU_SOURCE)
	      || c == strchr(s, '\0') - 1
# else
	      || c == &s[strlen(s)]
# endif
	       )
	       return *c;
	  else
	       return *(c + 1);
     }
     return (c == s) ? *c : *(c - 1);
}
# endif

const char cpeek(const char *const sp0, const char *const head)
{
     if (!head)
          return (*sp0 == '\0' || sp0 == strend(sp0) ? *sp0 : *(sp0 + 1));
     else
          return (sp0 == head) ? *sp0 : *(sp0 - 1);
}

int *strndelim(const char *s0, const char od, const char cd, int count[2])
{
     memset(count, 0, sizeof(*count)*2);
# if defined(_GNU_SOURCE)
     char *c = strchr(s0, '\0');
# else
     char *c = (char *)&s0[strlen(s0)];
# endif
     if (c == s0) // we are looking at the end of the string
          goto fail;

     do {
          /* FIXME: this was recently changed from the old cpeek
           * to the newer cpeek, but it has __not__ been tested!!
           */
          if (c != s0 && cpeek(c, s0) == '\\')
               continue;
          if (*c == cd)
               ++count[1];
          else if (*c == od)
               ++count[0];
     } while (c-- != s0);

     if (od == cd && count[1] > 0) {
          if (count[1] % 2 == 1)
               while (count[0]++ < --count[1]);
          else {
               count[0] = count[1] * 0.5;
               count[1] *= 0.5;
          }
     }
     return count;
fail:
     return NULL;
}

char *strwodqp(const char *src)
{
     size_t n = strlen(src) + 1;
     int c[2] = {0, 0}, even = 0;
     char *tmp, *token, *rest, *newp;
     tmp = token = rest = newp = NULL;

     if (!strndelim(src, '"', '"', c))
          return NULL;

     if (c[0] == 0)
          return NULL;

     tmp = strdup(src);
     newp = malloc(n);
     even = c[0] - abs(c[0] - c[1]);

     token = strtok_r(tmp, "\"", &rest);

     if (token == NULL) {
          free(newp);
          return NULL;
     }

     catl(newp, n, token);
     while ((token = strtok_r(NULL, "\"", &rest)) != NULL) {
          if (even % 2 == 1) {
               catm(newp, n, newp, token);
               --even;
          } else {
               ++even;
          }
     }

     free(tmp);
     return newp;
}

int strwodq(char *dst, const char *src, size_t n)
{
     int c[2] = {0, 0};
     int even = 0;
     int r = 0;
     char *tmp = NULL;
     char *token = NULL;
     char *rest = NULL;
     char *newp = NULL;

     if (!strndelim(src, '"', '"', c)) {
          r += 1;
          goto end;
     }

     if (c[0] == 0) {
          r += 2;
          goto end;
     }

     tmp = strdup(src);
     newp = malloc(n);
     even = c[0] - abs(c[0] - c[1]);

     token = strtok_r(tmp, "\"", &rest);
     if (token == NULL) {
          r += 3;
          goto free;
     }
     catl(newp, n, token);

     while ((token = strtok_r(NULL, "\"", &rest)) != NULL) {
          if (even % 2 == 1) {
               catm(newp, n, newp, token);
               --even;
          } else {
               ++even;
          }
     }

     memcpy(dst, newp, n);

free:
     free(tmp);
     free(newp);
end:
     return r;
}

/////////////////////////////////////////
// Taken from defunct mem.c
/////////////////////////////////////////
int memlen(const char *s)
{
     char *a = NULL;
     if ((a = strchr(s, '\0')))
	  return (int)(a - s);
     return -1;
}

char *strterm(char *s, size_t sz)
{
     char *tmp = NULL;
     tmp = s;
     s += sz;
     --s; --s;
     *s = '\0';
     s = tmp;
     return s;
}
#endif
