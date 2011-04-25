#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include <lua.h>
#include <lauxlib.h>

#include "libs/hash/lua_hash.h"
#include "libs/hash/lua_hash_md.h"
#include "libs/hash/lua_hash_crc32.h"

/* Compatibility between Lua 5.1+ and Lua 5.0 */
#ifndef LUA_VERSION_NUM
#define LUA_VERSION_NUM 0
#endif
#if LUA_VERSION_NUM < 501
#define luaL_register(a, b, c) luaL_openlib((a), (b), (c), 0)
#endif
#define MICRO_IN_SEC 1000000.00

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

static int Lpathinfo (lua_State *L) {
    char *dn, *bn, *ext;
    const char *path = luaL_checkstring(L, 1);

    if (path) {
        dn = dirname(strdup(path)); 
        bn = basename(strdup(path)); 
        ext = strrchr(bn, '.'); 
        ext++;

        lua_pushstring(L, dn);
        lua_pushstring(L, bn);

        if (ext) {
            lua_pushstring(L, ext);
        } else {
            lua_pushnil(L);
        }
        return 3;
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

static int Lmicrotime (lua_State *L) {
    struct timeval tp = {0};
    if (gettimeofday(&tp, NULL)) {
	lua_pushboolean(L, 0);
    }
    lua_pushnumber(L, (double)(tp.tv_sec + tp.tv_usec / MICRO_IN_SEC));
    return 1;
}

static int luaM_md5 (lua_State *L) {
    unsigned const char *string = (unsigned const char *)luaL_optstring(L, 1, NULL);
    int raw_output = luaL_optnumber(L, 2, 0);

    LUA_MD5_CTX context;

    LUA_MD5Init(&context);
    LUA_MD5Update(&context, string , strlen(string));

    unsigned char *digest = malloc(16+1);
    LUA_MD5Final((unsigned char *) digest, &context);

    digest[16] = 0;

    if (raw_output) {
        lua_pushstring(L, (unsigned char *)digest);
    }
    else
    {
        unsigned char *hex_digest = malloc(32+1);
        lua_hash_bin2hex(hex_digest, (unsigned char *) digest, 16);
        hex_digest[2 * 16] = 0;
        lua_pushstring(L, (unsigned char *)hex_digest);
    }

    return 1;
}

static int luaM_crc32 (lua_State *L) {
    unsigned const char *string = (unsigned const char *)luaL_optstring(L, 1, NULL);
    int raw_output = luaL_optnumber(L, 2, 0);

    LUA_CRC32_CTX context;

    LUA_CRC32Init(&context);
    LUA_CRC32Update(&context, string , strlen(string));

    unsigned char *digest = malloc(4+1);
    LUA_CRC32Final((unsigned char *) digest, &context);

    if (raw_output) {
        lua_pushstring(L, (unsigned char *)digest);
    }
    else
    {
        unsigned char *hex_digest = malloc(8+1);
        lua_hash_bin2hex(hex_digest, (unsigned char *) digest, 4);
        hex_digest[2 * 4] = 0;
        lua_pushstring(L, (unsigned char *)hex_digest);
    }

    return 1;
}

static const luaL_reg nix[] = {
    { "getpid",   Lgetpid },
    { "sleep",   Lsleep },
    { "pwd",   Lpwd },
    { "pathinfo",   Lpathinfo },
    { "microtime",   Lmicrotime },
    { "md5", luaM_md5 },
    { "crc32", luaM_crc32 },
    //{ "crc32b", luaM_crc32b },
    { NULL, NULL }
};

int luaopen_nix(lua_State *L) {
    luaL_register(L, "nix", nix);
    return 0;
}
