/****
  COMMONER; int.c, integer related functions

  Copyright (C) 2015-2019 Alexej G. Magura

  This file is a part of Commoner

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
#ifndef COMMONER_INT_C_GUARD
# define COMMONER_INT_C_GUARD 1

# if HAVE_CONFIG_H
#  include <config.h>
# endif

# if !defined(INT_MAX)
#  include <limits.h>
# endif

# include "commoner.h"
# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include <sys/time.h>
# include <math.h>

/*int COMMONER_NS(numlen)(const long double n)*/
/*{*/
     /*const long double posn = fabsl(n);*/
     /*[>char *buf = rmchar(COMNR_MKSTR(posn), '.', NULL);<]*/
     /*char *buf = NULL;*/
     /*size_t sz = floor(log10(posn)) + 1;*/
     /*buf = malloc(sz);*/
     /*bzero(buf, sz);*/
     /*snprintf("%Lf", buf, sz - 1, posn);*/
     /*size_t len = strlen(buf);*/
     /*free(buf);*/
     /*return len;*/
/*}*/

int COMMONER_NS(intlen)(const int n)
{
     int r = 0;
     int copy = n;

     while (copy) {
	  ++r;
       copy /= 10;
     }
     return r;
}

/* XXX The `randm' function is licensed under the terms of the Creative Commons License, said terms can be found here:
 * https://creativecommons.org/licenses/by-sa/3.0/
 *
 * Please see NOTICE file for authorship details.
 */

/* assumes 0 <= max <= RAND_MAX */
long COMMONER_NS(randm)(long max) {
     /* max <= RAND_MAX < ULONG_MAX, so this is okay */
     unsigned long nbin = (unsigned long) max + 1,
                   nrand = (unsigned long) RAND_MAX + 1,
                   binsz = nrand / nbin,
                   defect = nrand % nbin;
     long x0;
     do {
          x0 = random();
     } // this is carefully written not to overflow
     while (nrand - defect <= (unsigned long)x0);
     // truncated division is intentional
     return x0/binsz;
}

# if 0
/* returns, hopefully, a bunch of random bits based on the current timestamp */
uint64_t COMMONER_NS(getrandom)()
{
     struct timeval tv;
     gettimeofday(&tv, NULL);
     uint64_t bits = ((uint64_t) tv.tv_usec << 16) ^ tv.tv_sec;
     return bits;
}
# endif

int COMMONER_NS(rseed)()
{
     struct timeval t;
     gettimeofday(&t, NULL);
     srand(t.tv_usec * t.tv_sec);
     return 0;
}
/////////////////////////////////////////
// Taken from defunct itoa.c
/////////////////////////////////////////
void COMMONER_NS(itoa)(char *dst, int src)
{
     size_t len = COMMONER_NS(intlen)(src) + 1;
     char tmp[len];
     char *wp = tmp;

     for (; src != 0; ++wp, src /= 10) {
	  if (src >= 0)
	       *wp = '0' + (src % 10);
	  else
	       *wp = '0' - (src % 10);
# if COINT_INTERNAL_DLVL > 1
	  COINT_DBG("*wp: `%c`\n", *wp);
# endif
     }
     *wp++ = '\0';
# if COINT_INTERNAL_DLVL > 1
     COINT_DBG("len: %lu\n", len);
     COINT_DBG("strlen(tmp): %lu\n", strlen(tmp));
     COINT_DBG("sizeof(tmp): %lu\n", sizeof(tmp));
     COINT_DBG("tmp: `%s'\n", tmp);
# endif
     rev(tmp);
# if COINT_INTERNAL_DLVL > 1
     COINT_DBG("strlen(tmp)#2: %lu\n", strlen(tmp));
     COINT_DBG("sizeof(tmp)#2: %lu\n", sizeof(tmp));
     COINT_DBG("tmp#2: `%s'\n", tmp);
# endif
     COINT_DBG("tmp#3: `%s'\n", tmp);
     memcpy(dst, tmp, len);
}

char *COMMONER_NS(itoap)(const int src)
{
     COINT_DBG("src: %d\n", src);
     size_t len = COMMONER_NS(intlen)(src) + 1;
     int idx = src;
     char *dst = malloc(len);
     COMMONER_NS(bzero)(dst, len);
     char *wp = dst;

     for (; idx != 0; idx /= 10) {
	  if (idx >= 0)
	       *wp++ = '0' + (idx % 10);
	  else
	       *wp++ = '0' - (idx % 10);
     }
     wp = COMMONER_NS(revnp)(dst, len);
     *wp = '\0';
     COINT_DBG("dst: `%s'\n", dst);

     return dst;
}
#endif
