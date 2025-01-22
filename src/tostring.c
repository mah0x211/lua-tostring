/**
 *  Copyright (C) 2025 Masatoshi Fukunaga
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
// lua
#include <lauxlib.h>
#include <lua.h>

static int tostring_lua(lua_State *L)
{
    int narg = lua_gettop(L);

    for (int i = 1; i <= narg; i++) {
        if (luaL_callmeta(L, i, "__tostring")) {
            if (lua_type(L, -1) == LUA_TSTRING) {
                // replace the original value with the result
                lua_replace(L, i);
                continue;
            }
            // ignore the result if it is not a string
            lua_pop(L, 1);
        }

        int type = lua_type(L, i);
        switch (type) {
        case LUA_TNIL:
            lua_pushliteral(L, "nil");
            break;

        case LUA_TBOOLEAN:
            if (lua_toboolean(L, i)) {
                lua_pushliteral(L, "true");
            } else {
                lua_pushliteral(L, "false");
            }
            break;

        case LUA_TSTRING:
            lua_pushvalue(L, i);
            break;

        case LUA_TNUMBER:
#if LUA_VERSION_NUM >= 503
            if (lua_isinteger(L, i)) {
                lua_pushfstring(L, "%I", lua_tointeger(L, i));
            } else {
                lua_pushfstring(L, "%f", lua_tonumber(L, i));
            }
#else
            if (lua_tonumber(L, i) == (lua_Number)lua_tointeger(L, i)) {
                lua_pushfstring(L, "%d", lua_tointeger(L, i));
            } else {
                lua_pushfstring(L, "%f", lua_tonumber(L, i));
            }
#endif
            break;

        case LUA_TTABLE:
        case LUA_TUSERDATA:
            // determine the __name field of the metatable if it exists
            if (lua_getmetatable(L, i)) {
                lua_getfield(L, -1, "__name");
                if (lua_type(L, -1) == LUA_TSTRING) {
                    // use the __name field as the typename
                    char b[BUFSIZ];
                    size_t n =
                        snprintf(b, BUFSIZ, "%s: %p", lua_tostring(L, -1),
                                 lua_topointer(L, i));
                    lua_pushlstring(L, b, n);
                    lua_replace(L, i);
                    lua_pop(L, 2);
                    continue;
                }
                // ignore the result if it is not a string
                lua_pop(L, 2);
            }

        // case LUA_TFUNCTION:
        // case LUA_TTHREAD:
        // case LUA_TLIGHTUSERDATA:
        default: {
            char b[BUFSIZ];
            size_t n = snprintf(b, BUFSIZ, "%s: %p", lua_typename(L, type),
                                lua_topointer(L, i));
            lua_pushlstring(L, b, n);
        } break;
        }
        // replace the original value with the string representation
        lua_replace(L, i);
    }

    return narg;
}

LUALIB_API int luaopen_tostring(lua_State *L)
{
    lua_pushcfunction(L, tostring_lua);
    return 1;
}
