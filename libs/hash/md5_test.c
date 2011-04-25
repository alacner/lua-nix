#include <string.h>
#include <stdlib.h>
#include "lua_hash.h"
#include "lua_hash_md.h"


unsigned char *nix_md5(const unsigned char *string, int raw_output)
{
    LUA_MD5_CTX context;

    LUA_MD5Init(&context);
    LUA_MD5Update(&context, string , strlen(string));

    unsigned char *digest = malloc(16+1);
    LUA_MD5Final((unsigned char *) digest, &context);

    digest[16] = 0;
    if (raw_output) return ((unsigned char *)digest);

    unsigned char *hex_digest = malloc(32+1);

    lua_hash_bin2hex(hex_digest, (unsigned char *) digest, 16);

    hex_digest[2 * 16] = 0;
    return ((unsigned char *)hex_digest);
}



int main()
{
    const unsigned char *string = "12345";
    printf("%s\n", nix_md5(string, 1));
    printf("%s\n", nix_md5(string, 0));
}
