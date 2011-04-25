#include <string.h>
#include <stdlib.h>
#include "lua_hash.h"
#include "lua_hash_crc32.h"


unsigned char *nix_crc32(const unsigned char *string, int raw_output)
{
    LUA_CRC32_CTX context;

    LUA_CRC32Init(&context);
    LUA_CRC32Update(&context, string , strlen(string));

    unsigned char *digest = malloc(4+1);
    LUA_CRC32Final((unsigned char *) digest, &context);

    digest[4] = 0;
    if (raw_output) return ((unsigned char *)digest);

    unsigned char *hex_digest = malloc(8+1);

    lua_hash_bin2hex(hex_digest, (unsigned char *) digest, 4);

    hex_digest[2 * 4] = 0;
    return ((unsigned char *)hex_digest);
}



int main()
{
    const unsigned char *string = "12345";
    printf("%s\n", nix_crc32(string, 1));
    printf("%s\n", nix_crc32(string, 0));
}
