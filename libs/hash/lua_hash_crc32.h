#ifndef LUA_HASH_CRC32_H
#define LUA_HASH_CRC32_H

typedef struct {
	lua_hash_uint32 state;
} LUA_CRC32_CTX;

void LUA_CRC32Init(LUA_CRC32_CTX *context);
void LUA_CRC32Update(LUA_CRC32_CTX *context, const unsigned char *input, crc32_size_t len);
void LUA_CRC32BUpdate(LUA_CRC32_CTX *context, const unsigned char *input, crc32_size_t len);
void LUA_CRC32Final(unsigned char digest[4], LUA_CRC32_CTX *context);

#endif
