#ifndef LUA_HASH_H
#define LUA_HASH_H
typedef unsigned int lua_hash_uint32;
typedef   unsigned   long   crc32_size_t;

static inline void lua_hash_bin2hex(char *out, const unsigned char *in, int in_len)
{
	static const char hexits[17] = "0123456789abcdef";
	int i;

	for(i = 0; i < in_len; i++) {
		out[i * 2]       = hexits[in[i] >> 4];
		out[(i * 2) + 1] = hexits[in[i] &  0x0F];
	}
}
#endif 
