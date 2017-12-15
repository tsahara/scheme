#include <stdint.h>

typedef intptr_t scm_obj_t;

/*
 * xx....xx xxxx0000 : Fixnum  (upper bits => signed integer)
 * xx....xx xxxx0001 : Pair    (upper bits => pointer)
 * xx....xx xxxx0010 : String  (upper bits => pointer)
 * xx....xx xxxx0011 : Symbol  (upper bits => pointer)
 * xx....xx xxxx0100 : Symbol  (upper bits => pointer)
 * xx....xx xxxx0101 : Closure (upper bits => pointer)
 *
 * xx....xx 00000111 : Character (upper 32bits = Unicode code point)
 * 00....00 00001011 : '()
 * 00....00 00001101 : #t
 * 00....00 00001110 : #f
 * 00....00 00001111 : #unspecified
 */

#define SCM_OBJ_NIL		0x0c
#define SCM_OBJ_TRUE		0x0d
#define SCM_OBJ_FALSE		0x0e
#define SCM_OBJ_UNSPECIFIED	0x0f


void scm_init(void);

#include "fixnum.h"
