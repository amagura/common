/****
COMMON; itoa.c, integer related functions (should be merged with similar functions)

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
#ifndef COMMON_ITOA_C_GUARD
# define COMMON_ITOA_C_GUARD 1
# include <stdio.h>
# include <stdlib.h>
# include "rev.c"
# include "intlen.c"
# include "common.h"

void itoa(char *dst, int src)
{
     size_t len = intlenm(src);
     char tmp[len];
     char *wp = tmp;

     for (; src != 0; ++wp, src /= 10) {
	  if (src >= 0)
	       *wp = '0' + (src % 10);
	  else
	       *wp = '0' - (src % 10);
# if COM_DLVL > 1
	  COM_DBG("*wp: `%c`\n", *wp);
# endif
     }
     *wp++ = '\0';
# if COM_DLVL > 1
     COM_DBG("len: %lu\n", len);
     COM_DBG("strlen(tmp): %lu\n", strlen(tmp));
     COM_DBG("sizeof(tmp): %lu\n", sizeof(tmp));
     COM_DBG("tmp: `%s'\n", tmp);
# endif
     rev(tmp);
# if COM_DLVL > 1
     COM_DBG("strlen(tmp)#2: %lu\n", strlen(tmp));
     COM_DBG("sizeof(tmp)#2: %lu\n", sizeof(tmp));
     COM_DBG("tmp#2: `%s'\n", tmp);
# endif
     COM_DBG("tmp#3: `%s'\n", tmp);
     memcpy(dst, tmp, len);
}

char *itoap(const int src)
{
     COM_DBG("src: %d\n", src);
     size_t len = intlenm(src);
     int idx = src;
     char *dst = malloc(len);
     bzero(dst, len);
     char *wp = dst;

     for (; idx != 0; idx /= 10) {
	  if (idx >= 0)
	       *wp++ = '0' + (idx % 10);
	  else
	       *wp++ = '0' - (idx % 10);
     }
     wp = revnp(dst, len);
     *wp = '\0';
     COM_DBG("dst: `%s'\n", dst);

     return dst;
}
#endif