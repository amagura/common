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
#include <stdio.h>
#include "intlen.h"

int intlen(int idx)
{
     int result = 0;
     while (idx) {
	  ++result;
	  idx /= 10;
     }
     return result;
}

int intlenc(const int idx)
{
     int copy = idx;
     int result = 0;
     while (copy) {
	  ++result;
	  copy /= 10;
     }
     return result;
}

size_t intlenm(int src)
{
     size_t dst = 1; /* XXX adds 1 for null-terminator */
     while (src) {
	  ++dst;
	  src /= 10;
     }
     return dst;
}