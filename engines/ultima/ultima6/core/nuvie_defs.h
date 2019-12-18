/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef ULTIMA6_CORE_NUVIE_DEFS_H
#define ULTIMA6_CORE_NUVIE_DEFS_H

#include "common/scummsys.h"
#include "ultima/ultima6/misc/sdl_compat.h"

namespace Ultima {
namespace Ultima6 {

typedef int8 sint8;
typedef int16 sint16;
typedef int32 sint32;

typedef uint8 nuvie_game_t; // Game type (1=u6,2=md,4=se)

#define NUVIE_GAME_NONE  0
#define NUVIE_GAME_U6    1
#define NUVIE_GAME_MD    2
#define NUVIE_GAME_SE    4

#define NUVIE_CONFIG_NAME_U6 "ultima6"
#define NUVIE_CONFIG_NAME_MD "martian"
#define NUVIE_CONFIG_NAME_SE "savage"

#define NUVIE_STYLE_ORIG 0
#define NUVIE_STYLE_NEW  1
#define NUVIE_STYLE_ORIG_PLUS_CUTOFF_MAP 2
#define NUVIE_STYLE_ORIG_PLUS_FULL_MAP   3

#define MAX(x, y)      ((x) > (y) ? (x) : (y))
#define MIN(x, y)      ((x) < (y) ? (x) : (y))
#define clamp_min(v, c)  (((v) < (c)) ? (c) : (v))
#define clamp_max(v, c)  (((v) > (c)) ? (c) : (v))
#define clamp(v, c1, c2) ( ((v) < (c1)) ? (c1) : (((v) > (c2)) ? (c2) : (v)) )

//FIXME fix for future maps which will probably be 1024 wide starting at level 6..
#define WRAPPED_COORD(c,level) ((c)&((level)?255:1023))
#define WRAP_COORD(c,level) ((c)&=((level)?255:1023))

#define MAP_SIDE_LENGTH(map_level) ((map_level > 0 && map_level < 6) ? 256 : 1024)

/*
 * on all levels, except level 0 (conveniently 'false') the map pitch is 256.
 * to properly wrap, mask the coordinate with the relevant bit-mask.
 * Another way to write this would be:

const uint16 map_pitch[2] = { 1024, 256 }; // width of 0:surface plane, and 1:all other planes
#define WRAPPED_COORD(c,level) ((c)&(map_pitch[(level==0)?0:1]-1)) // mask high bit, wrap C to map_pitch
#define WRAP_COORD(c,level) ((c)&=(map_pitch[(level==0)?0:1]-1)) // modifies C
*/

#define NUVIE_DIR_N    0
#define NUVIE_DIR_E    1
#define NUVIE_DIR_S    2
#define NUVIE_DIR_W    3

#define NUVIE_DIR_NE   4
#define NUVIE_DIR_SE   5
#define NUVIE_DIR_SW   6
#define NUVIE_DIR_NW   7

#define NUVIE_DIR_NONE 8

#define TRAMMEL_PHASE 1.75
#define FELUCCA_PHASE 1.1666666666666667

typedef enum { LEVEL_EMERGENCY = 0, LEVEL_ALERT, LEVEL_CRITICAL, LEVEL_ERROR, LEVEL_WARNING, LEVEL_NOTIFICATION, LEVEL_INFORMATIONAL, LEVEL_DEBUGGING } DebugLevelType;
#ifdef WITHOUT_DEBUG
#define DEBUG(...)
#else
#define DEBUG(...) debug(__func__,__FILE__,__LINE__,__VA_ARGS__)
DebugLevelType debug(const char *func, const char *file, const int line, const bool no_header, const DebugLevelType level, const char *format, ...);
#endif /* WITHOUT_DEBUG */

/*
#ifndef BOOL
typedef unsigned char BOOL;
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  1
#endif
*/

#ifndef WIN32
#define U6PATH_DELIMITER '/'
#else
#define U6PATH_DELIMITER '\\'
//#define U6PATH_DELIMITER '/' // win32 file functions work fine with /
#endif


#ifdef MACOSX
#define NUVIE_RAND random
#define NUVIE_RAND_MAX 0x7fffffff // POSIX: 2^(31)-1
#else
#define NUVIE_RAND rand
#define NUVIE_RAND_MAX RAND_MAX
#endif

#define MAXPATHLEN 256

#define nuprint(...) Game::get_game()->get_scroll()->printf(__VA_ARGS__)
/*#define nuprint(T) Game::get_game()->get_scroll()->display_string(T)*/
/*#define nuprint(T) Game::get_game()->get_scroll()->display_string(T); \
                     Game::get_game()->get_scroll()->display_string("\n");*/

} // End of namespace Ultima6
} // End of namespace Ultima

#endif