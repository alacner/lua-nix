#include "lua_hash.h"
#include "lua_hash_crc32.h"
#include "lua_hash_crc32_tables.h"

void LUA_CRC32Init(LUA_CRC32_CTX *context)
{
	context->state = ~0;
}

void LUA_CRC32Update(LUA_CRC32_CTX *context, const unsigned char *input, crc32_size_t len)
{
	crc32_size_t i;
	
	for (i = 0; i < len; ++i) {
		context->state = (context->state << 8) ^ crc32_table[(context->state >> 24) ^ (input[i] & 0xff)];
	}
}

void LUA_CRC32BUpdate(LUA_CRC32_CTX *context, const unsigned char *input, crc32_size_t len)
{
	crc32_size_t i;
	
	for (i = 0; i < len; ++i) {
		context->state = (context->state >> 8) ^ crc32b_table[(context->state ^ input[i]) & 0xff];
	}
}

void LUA_CRC32Final(unsigned char digest[4], LUA_CRC32_CTX *context)
{
	context->state=~context->state;
	digest[3] = (unsigned char) ((context->state >> 24) & 0xff);
	digest[2] = (unsigned char) ((context->state >> 16) & 0xff);
	digest[1] = (unsigned char) ((context->state >> 8) & 0xff);
	digest[0] = (unsigned char) (context->state & 0xff);
	context->state = 0;
}

void LUA_CRC32BFinal(unsigned char digest[4], LUA_CRC32_CTX *context)
{
	context->state=~context->state;
	digest[0] = (unsigned char) ((context->state >> 24) & 0xff);
	digest[1] = (unsigned char) ((context->state >> 16) & 0xff);
	digest[2] = (unsigned char) ((context->state >> 8) & 0xff);
	digest[3] = (unsigned char) (context->state & 0xff);
	context->state = 0;
}
