#include "luabinding.h"
#include <string.h>

int add(int a, int b)
{
    return a+b;
}

int state()
{
    return 0;
}

int myprint(const char* str)
{
    return printf("myprint %s\n", str);
}

void myprint2(const char* str)
{
    printf("myprint2 %s\n", str);
}

int* getpoint(int a)
{
    static int x = 0;
    x = a;
    printf("getpoint:%d\n", a);
    return &x;
}

void printpoint(int* p)
{
    printf("printpoint: %d\n", *p);
}

void hello()
{
    printf("hello\n");
}

CLUA_DEF_VOID(myprint2, "s", const char*)
CLUA_DEF_VOID(printpoint, "p", int*)
CLUA_DEF_VOID(hello, "")

CLUA_DEF(myprint, "ds", int, const char*)
CLUA_DEF(getpoint, "pd", int*, int)
CLUA_DEF(add, "ddd", int, int, int)
CLUA_DEF(state, "d", int)

static int load_clua(lua_State* L)
{
    static luaL_Reg clua_lib[] = {
        CLUA_REG(add),
        CLUA_REG(myprint),
        CLUA_REG(myprint2),
        CLUA_REG(getpoint),
        CLUA_REG(printpoint),
        CLUA_REG(hello),
        CLUA_REG(state),

        {NULL, NULL}
    };

    luaL_register(L, "_G", clua_lib);
    return 1;
}

static void print_error(lua_State* L)
{
    fprintf(stderr, "Fatal error: %s\n", lua_tostring(L, -1));
}

int main()
{
    lua_State* L = lua_open();

    luaL_openlibs(L);
    load_clua(L);

    int ret = luaL_dofile(L, "/home/hzh/test.lua");
    if(ret != 0)
    {
        print_error(L);
    }
    lua_close(L);
    printf("leave\n");

    return 0;
}
