/**
 * @file luabinding.h
 * @brief 将C和lua进行绑定
 */
#ifndef LUABINDING_H
#define LUABINDING_H

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"

/**
 * @brief 获取lua堆栈中的值
 * @param L lua状态机
 * @param index 堆栈位置
 * @param type 使用类似printf的占位符描述值类型,支持以下类型
 * - 'd' int
 * - 'u' unsigned int
 * - 'f' float
 * - 'D' long long
 * - 'U' unsigned long long
 * - 'F' double
 * - 's' const char*
 * - 'p' void*
 * @param value 输出值指针,指针类型则传入二级指针
 * @return 获取是否成功
 *  @retval 0 失败
 *  @retval 1 成功
 */
int lua_get_value(lua_State* L, int index, char type, void* value);

/**
 * @brief 将值压入lua堆栈
 * @param L lua状态机
 * @param type 使用类似printf的占位符描述值类型,支持以下类型
 * - 'd' int
 * - 'u' unsigned int
 * - 'f' float
 * - 'D' long long
 * - 'U' unsigned long long
 * - 'F' double
 * - 's' const char*
 * - 'p' void*
 * @param value 值指针,指针类型则传入二级指针
 * @return 设置是否成功
 *  @retval 0 失败
 *  @retval 1 成功
 */
int lua_set_value(lua_State* L, char type, void* value);

/**
 * @brief lua接口函数名
 * @param f C函数名
 */
#define CLUA_FNAME(f) _clua_##f

/**
 * @brief lua接口函数声明
 * @param f C函数名
 */
#define CLUA_DECL(f) int CLUA_FNAME(f)(lua_State * L)

/**
 * @brief lua接口函数注册时的luaL_Reg结构
 * @param f C函数名
 */
#define CLUA_REG(f) { #f, CLUA_FNAME(f) }

/**
 * @brief 定义无返回值的lua接口
 * @param f C函数名
 * @param fmt 参数类型列表字符串, 类型含义见 @ref lua_get_value 和 @ref lua_set_value
 * @param ... 参数类型,和 fmt 一一对应
 *
 * @note 示例
 * void myprint(const char* msg1, int count); //无返回值2参数
 * CLUA_DEF_VOID(myprint, "sd", const char*, int);
 *                  ^      ^          ^       ^
 *               函数名  类型列表    参数1    参数2
 */
#define CLUA_DEF_VOID(f, fmt, ...) IMPL_CLUA_CAT(IMPL_CLUA_DEF_VOID_, PP_NARG(__VA_ARGS__))(f, fmt, ##__VA_ARGS__)

/**
 * @brief 定义带返回值的lua接口
 * @param f C函数名
 * @param fmt 返回值+参数类型列表字符串, 类型含义见 @ref lua_get_value 和 @ref lua_set_value
 * @param argret 返回值类型,和 fmt 第一项对应
 * @param ... 参数类型,和 fmt 一一对应
 *
 * @note 示例
 * int myprint(const char* msg1, int count); //带返回值2参数
 * CLUA_DEF(myprint, "dsd", int, const char*, int);
 *            ^        ^     ^      ^          ^
 *          函数名  类型列表 返回值   参数1      参数2
 */
#define CLUA_DEF(f, fmt, argret,...) IMPL_CLUA_CAT(IMPL_CLUA_DEF_, PP_NARG(__VA_ARGS__))(f, fmt, argret, ##__VA_ARGS__)

/*******************
 * 实现部分
 ******************/
#define IMPL_CLUA_CAT(x, y) IMPL_CLUA_CAT_(x, y)
#define IMPL_CLUA_CAT_(x, y) x##y

#define PP_ARG_N( \
          _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
         _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
         _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
         _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
         _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
         _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
         _61, _62, _63, N, ...) N

#define PP_RSEQ_N()                                        \
         62, 61, 60,                                       \
         59, 58, 57, 56, 55, 54, 53, 52, 51, 50,           \
         49, 48, 47, 46, 45, 44, 43, 42, 41, 40,           \
         39, 38, 37, 36, 35, 34, 33, 32, 31, 30,           \
         29, 28, 27, 26, 25, 24, 23, 22, 21, 20,           \
         19, 18, 17, 16, 15, 14, 13, 12, 11, 10,           \
          9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define PP_NARG_(...)    PP_ARG_N(__VA_ARGS__)

#define PP_NARG(...)     PP_NARG_(_, ##__VA_ARGS__, PP_RSEQ_N())


#define IMPL_CLUA_DEF_VOID_0(f, fmt)                                                               \
    CLUA_DECL(f)                                                                                   \
    {                                                                                              \
        (void)(L);                                                                                 \
        (void)(fmt);                                                                               \
        f();                                                                                       \
        return 0;                                                                                  \
    }

#define IMPL_CLUA_DEF_VOID_1(f, fmt, arg1)                                                         \
    CLUA_DECL(f)                                                                                   \
    {                                                                                              \
        arg1 a1;                                                                                   \
        if(lua_get_value(L, 1, fmt[0], &a1))                                                       \
        {                                                                                          \
            f(a1);                                                                                 \
        }                                                                                          \
        return 0;                                                                                  \
    }

#define IMPL_CLUA_DEF_VOID_2(f, fmt, arg1, arg2)                                                   \
    CLUA_DECL(f)                                                                                   \
    {                                                                                              \
        arg1 a1;                                                                                   \
        arg2 a2;                                                                                   \
        if(lua_get_value(L, 1, fmt[0], &a1) && lua_get_value(L, 2, fmt[1], &a2))                   \
        {                                                                                          \
            f(a1, a2);                                                                             \
        }                                                                                          \
        return 0;                                                                                  \
    }

#define IMPL_CLUA_DEF_VOID_3(f, fmt, arg1, arg2, arg3)                                             \
    CLUA_DECL(f)                                                                                   \
    {                                                                                              \
        arg1 a1;                                                                                   \
        arg2 a2;                                                                                   \
        arg3 a3;                                                                                   \
        if(lua_get_value(L, 1, fmt[0], &a1) && lua_get_value(L, 2, fmt[1], &a2) &&                 \
           lua_get_value(L, 3, fmt[2], &a3))                                                       \
        {                                                                                          \
            f(a1, a2, a3);                                                                         \
        }                                                                                          \
        return 0;                                                                                  \
    }

#define IMPL_CLUA_DEF_VOID_4(f, fmt, arg1, arg2, arg3, arg4)                                       \
    CLUA_DECL(f)                                                                                   \
    {                                                                                              \
        arg1 a1;                                                                                   \
        arg2 a2;                                                                                   \
        arg3 a3;                                                                                   \
        arg3 a4;                                                                                   \
        if(lua_get_value(L, 1, fmt[0], &a1) && lua_get_value(L, 2, fmt[1], &a2) &&                 \
           lua_get_value(L, 3, fmt[2], &a3) && lua_get_value(L, 4, fmt[4], &a4))                   \
        {                                                                                          \
            f(a1, a2, a3, a4);                                                                     \
        }                                                                                          \
        return 0;                                                                                  \
    }

#define IMPL_CLUA_DEF_VOID_5(f, fmt, arg1, arg2, arg3, arg4, arg5)                                 \
    CLUA_DECL(f)                                                                                   \
    {                                                                                              \
        arg1 a1;                                                                                   \
        arg2 a2;                                                                                   \
        arg3 a3;                                                                                   \
        arg3 a4;                                                                                   \
        arg3 a5;                                                                                   \
        if(lua_get_value(L, 1, fmt[0], &a1) && lua_get_value(L, 2, fmt[1], &a2) &&                 \
           lua_get_value(L, 3, fmt[2], &a3) && lua_get_value(L, 4, fmt[3], &a4) &&                 \
           lua_get_value(L, 5, fmt[4], &a5))                                                       \
        {                                                                                          \
            f(a1, a2, a3, a4, a5);                                                                 \
        }                                                                                          \
        return 0;                                                                                  \
    }

#define IMPL_CLUA_DEF_0(f, fmt, argret)                                                            \
    CLUA_DECL(f)                                                                                   \
    {                                                                                              \
        argret ret = f();                                                                          \
        if(lua_set_value(L, fmt[0], &ret))                                                         \
        {                                                                                          \
            return 1;                                                                              \
        }                                                                                          \
        return 0;                                                                                  \
    }

#define IMPL_CLUA_DEF_1(f, fmt, argret, arg1)                                                      \
    CLUA_DECL(f)                                                                                   \
    {                                                                                              \
        arg1 a1;                                                                                   \
        argret ret;                                                                                \
        if(lua_get_value(L, 1, fmt[1], &a1))                                                       \
        {                                                                                          \
            ret = f(a1);                                                                           \
            return lua_set_value(L, fmt[0], &ret);                                                 \
        }                                                                                          \
        return 0;                                                                                  \
    }

#define IMPL_CLUA_DEF_2(f, fmt, argret, arg1, arg2)                                                \
    CLUA_DECL(f)                                                                                   \
    {                                                                                              \
        arg1 a1;                                                                                   \
        arg2 a2;                                                                                   \
        argret ret;                                                                                \
        if(lua_get_value(L, 1, fmt[1], &a1) && lua_get_value(L, 2, fmt[2], &a2))                   \
        {                                                                                          \
            ret = f(a1, a2);                                                                       \
            return lua_set_value(L, fmt[0], &ret);                                                 \
        }                                                                                          \
        return 0;                                                                                  \
    }

#define IMPL_CLUA_DEF_3(f, fmt, argret, arg1, arg2, arg3)                                          \
    CLUA_DECL(f)                                                                                   \
    {                                                                                              \
        arg1 a1;                                                                                   \
        arg2 a2;                                                                                   \
        arg3 a3;                                                                                   \
        argret ret;                                                                                \
        if(lua_get_value(L, 1, fmt[1], &a1) && lua_get_value(L, 2, fmt[2], &a2) &&                 \
           lua_get_value(L, 3, fmt[3], &a3))                                                       \
        {                                                                                          \
            ret = f(a1, a2, a3);                                                                   \
            return lua_set_value(L, fmt[0], &ret);                                                 \
        }                                                                                          \
        return 0;                                                                                  \
    }

#define IMPL_CLUA_DEF_4(f, fmt, argret, arg1, arg2, arg3, arg4)                                    \
    CLUA_DECL(f)                                                                                   \
    {                                                                                              \
        arg1 a1;                                                                                   \
        arg2 a2;                                                                                   \
        arg3 a3;                                                                                   \
        arg4 a4;                                                                                   \
        argret ret;                                                                                \
        if(lua_get_value(L, 1, fmt[1], &a1) && lua_get_value(L, 2, fmt[2], &a2) &&                 \
           lua_get_value(L, 3, fmt[3], &a3) && lua_get_value(L, 4, fmt[4], &a4))                   \
        {                                                                                          \
            ret = f(a1, a2, a3, a4);                                                               \
            return lua_set_value(L, fmt[0], &ret);                                                 \
        }                                                                                          \
        return 0;                                                                                  \
    }

#define IMPL_CLUA_DEF_5(f, fmt, argret, arg1, arg2, arg3, arg4, arg5)                              \
    CLUA_DECL(f)                                                                                   \
    {                                                                                              \
        arg1 a1;                                                                                   \
        arg2 a2;                                                                                   \
        arg3 a3;                                                                                   \
        arg4 a4;                                                                                   \
        arg5 a5;                                                                                   \
        argret ret;                                                                                \
        if(lua_get_value(L, 1, fmt[1], &a1) && lua_get_value(L, 2, fmt[2], &a2) &&                 \
           lua_get_value(L, 3, fmt[3], &a3) && lua_get_value(L, 4, fmt[4], &a4) &&                 \
           lua_get_value(L, 5, fmt[5], &a5))                                                       \
        {                                                                                          \
            ret = f(a1, a2, a3, a4, a5);                                                           \
            return lua_set_value(L, fmt[0], &ret);                                                 \
        }                                                                                          \
        return 0;                                                                                  \
    }

#endif // LUABINDING_H
