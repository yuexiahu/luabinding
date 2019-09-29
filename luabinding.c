#include "luabinding.h"

int lua_get_value(lua_State* L, int index, char type, void* value)
{
    switch(type)
    {
    case 'd':
    case 'u':
        if(!lua_isnumber(L, index)) return 0;
        *((int*)value) = (int)lua_tointeger(L, index);
        break;
    case 'f':
        if(!lua_isnumber(L, index)) return 0;
        *((float*)value) = (float)lua_tonumber(L, index);
        break;
    case 'D':
    case 'U':
        if(!lua_isnumber(L, index)) return 0;
        *((long long*)value) = lua_tointeger(L, index);
        break;
    case 'F':
        if(!lua_isnumber(L, index)) return 0;
        *((double*)value) = lua_tonumber(L, index);
        break;
    case 's':
        if(!lua_isstring(L, index)) return 0;
        *((const char**)value) = lua_tostring(L, index);
        break;
    case 'p':
        if(!lua_isuserdata(L, index)) return 0;
        *((const void**)value) = lua_topointer(L, index);
        break;
    default:
        return 0;
    }
    return 1;
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
        return 0;
    }
    return 1;
}
