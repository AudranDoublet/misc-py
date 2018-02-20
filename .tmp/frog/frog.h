/** File with a LOT of includes :D **/

#ifndef __FROG_H__
#define __FROG_H__

#include <stdlib.h>
#include <stdint.h>

typedef uint32_t fchar; /** UTF-32 character **/
typedef long long ssize_t; /** signed size_t **/

#include "common/strutils.h"

#include "object.h"
#include "objects/frogint.h"
#include "objects/frogstr.h"
#include "error.h"

#include "parser/lexer.h"

#endif
