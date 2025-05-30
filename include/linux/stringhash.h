#pragma once

#include <linux/hash.h>

/* Hash courtesy of the R5 hash in reiserfs modulo sign bits */
#define init_name_hash(salt)		(uintptr_t)(salt)

/*
 * A hash_len is a u64 with the hash of a string in the low
 * half and the length in the high half.
 */
#define hashlen_hash(hashlen) ((u32)(hashlen))
#define hashlen_len(hashlen)  ((u32)((hashlen) >> 32))
#define hashlen_create(hash, len) ((u64)(len)<<32 | (u32)(hash))

/* partial hash update function. Assume roughly 4 bits per character */
static inline uintptr_t partial_name_hash(unsigned long c, uintptr_t prevhash)
{
	return (prevhash + (c << 4) + (c >> 4)) * 11;
}

/*
 * Finally: cut down the number of bits to a int value (and try to avoid
 * losing bits).  This also has the property (wanted by the dcache)
 * that the msbits make a good hash table index.
 */
static inline unsigned int end_name_hash(uintptr_t hash)
{
	return hash_long(hash, 32);
}

extern unsigned int full_name_hash(const void *salt, const char *, unsigned int);
u64 hashlen_string(const void *salt, const char *name);
