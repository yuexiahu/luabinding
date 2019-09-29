#include "luabinding.h"

#define LUA_DO_ERROR(L, fmt, ...)\
    lua_pushfstring(L, fmt, ##__VA_ARGS__);\
    lua_error(L)

static const char* traceback(lua_State *L) {
    lua_getfield(L, LUA_GLOBALSINDEX, "debug");
    lua_getfield(L, -1, "traceback");
    lua_pcall(L, 0, 1, 0);
    return lua_tostring(L, -1);
}

int lua_get_value(lua_State* L, int index, char type, void* value)
{
    switch(type)
    {
    case 'd':
    case 'u':
        if(!lua_isnumber(L, index))
            goto fail;
        *((int*)value) = (int)lua_tointeger(L, index);
        break;
    case 'f':
        if(!lua_isnumber(L, index))
            goto fail;
        *((float*)value) = (float)lua_tonumber(L, index);
        break;
    case 'D':
    case 'U':
        if(!lua_isnumber(L, index))
            goto fail;
        *((long long*)value) = lua_tointeger(L, index);
        break;
    case 'F':
        if(!lua_isnumber(L, index))
            goto fail;
        *((double*)value) = lua_tonumber(L, index);
        break;
    case 's':
        if(!lua_isstring(L, index))
            goto fail;
        *((const char**)value) = lua_tostring(L, index);
        break;
    case 'p':
        if(!lua_isuserdata(L, index))
            goto fail;
        *((const void**)value) = lua_topointer(L, index);
        break;
    default:
        goto fail;
    }
    return 1;

fail:
    LUA_DO_ERROR(L, "lua_get_value failed! index=%d, c_type=%c, lua_type=\"%s\"\n %s",
                 index, type, lua_typename(L, lua_type(L, index)), traceback(L));
    return 0;
}

int lua_set_value(lua_State* L, char type, void* value)
{
    switch(type)
    {
    case 'd':
        lua_pushnumber(L, *((int*)value));
        break;
    case 'u':
        lua_pushnumber(L, *((unsigned*)value));
        break;
    case 'f':
        lua_pushnumber(L, (double)*((float*)value));
        break;
    case 'D':
        lua_pushnumber(L, *(long long*)(value));
        break;
    case 'U':
        lua_pushnumber(L, *(unsigned long long*)(value));
        break;
    case 'F':
        lua_pushnumber(L, *(double*)(value));
        break;
    case 's':
        lua_pushstring(L, *((const char**)value));
        break;
    case 'p':
        lua_pushlightuserdata(L, *((void**)value));
        break;
    default:
        LUA_DO_ERROR(L, "lua_set_value failed! c_type=%c\n %s", type, traceback(L));
        return 0;
    }
    return 1;
}
