#include <lua.h>
#include <lauxlib.h>
#include <unistd.h>

/* Compatibility between Lua 5.1+ and Lua 5.0 */
#ifndef LUA_VERSION_NUM
#define LUA_VERSION_NUM 0
#endif
#if LUA_VERSION_NUM < 501
#define luaL_register(a, b, c) luaL_openlib((a), (b), (c), 0)
#endif

static int Lpwd (lua_State *L) {
    char pwd[1024];
    getcwd(pwd, sizeof(pwd));
    if (pwd) {
        lua_pushstring(L, pwd);
        return 1;
    } else {
        lua_pushnil(L);
        return 1;
    }
}

static int Lgetpid (lua_State *L) {
    int pid;
    pid = getpid();
    if (pid) {
        lua_pushnumber(L, pid);
        return 1;
    } else {
        lua_pushnil(L);
        return 1;
    }
}

static int Lsleep (lua_State *L) {
    lua_Number s = luaL_checknumber(L, 1);
    sleep(s);
    lua_pushnumber(L, s);
    return 1;
}

static const luaL_reg mylib[] = {
    { "getpid",   Lgetpid },
    { "sleep",   Lsleep },
    { "pwd",   Lpwd },
    { NULL, NULL }
};

int luaopen_mylib(lua_State *L) {
    luaL_register(L, "mylib", mylib);
    return 0;
}
