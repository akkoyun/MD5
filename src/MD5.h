#ifndef _MD5_
#define _MD5_

#ifndef Arduino_h
	#include <Arduino.h>
#endif

#include <stdint.h>
#include <string.h>
#include <stddef.h>

#if defined(__GNUC__) || defined(__clang__)
	#define MD5_FORCE_INLINE __attribute__((always_inline)) inline
#else
	#define MD5_FORCE_INLINE inline
#endif

#define MD5_F(x, y, z)      ((z) ^ ((x) & ((y) ^ (z))))
#define MD5_G(x, y, z)      ((y) ^ ((z) & ((x) ^ (y))))
#define MD5_H(x, y, z)      ((x) ^ (y) ^ (z))
#define MD5_I(x, y, z)      ((y) ^ ((x) | ~(z)))

#define MD5_STEP(f, a, b, c, d, x, t, s) do { \
	(a) += f((b), (c), (d)) + (x) + (uint32_t)(t); \
	(a) = (((a) << (s)) | ((a) >> (32U - (s)))); \
	(a) += (b); \
} while(0)

#define MD5_READ_LE(ptr, n) \
	(((uint32_t)(ptr)[(n) * 4]) | \
	((uint32_t)(ptr)[(n) * 4 + 1] << 8) | \
	((uint32_t)(ptr)[(n) * 4 + 2] << 16) | \
	((uint32_t)(ptr)[(n) * 4 + 3] << 24))

// Default low-RAM mode: do not store 16-word block, recompute words on demand.
#define MD5_SET(n) MD5_READ_LE(ptr, (n))
#define MD5_GET(n) MD5_READ_LE(ptr, (n))

class MD5 {

	private:

		uint64_t _Lo;
		uint32_t _StateA;
		uint32_t _StateB;
		uint32_t _StateC;
		uint32_t _StateD;

		uint8_t _Buffer[64];

		MD5_FORCE_INLINE const void* Process(const void* data, size_t length) {
			const uint8_t* ptr = (const uint8_t*)data;

			uint32_t a = this->_StateA;
			uint32_t b = this->_StateB;
			uint32_t c = this->_StateC;
			uint32_t d = this->_StateD;

			do {
				uint32_t saved_a = a;
				uint32_t saved_b = b;
				uint32_t saved_c = c;
				uint32_t saved_d = d;

				/* Round 1 */
				MD5_STEP(MD5_F, a, b, c, d, MD5_SET(0), 0xd76aa478, 7);
				MD5_STEP(MD5_F, d, a, b, c, MD5_SET(1), 0xe8c7b756, 12);
				MD5_STEP(MD5_F, c, d, a, b, MD5_SET(2), 0x242070db, 17);
				MD5_STEP(MD5_F, b, c, d, a, MD5_SET(3), 0xc1bdceee, 22);
				MD5_STEP(MD5_F, a, b, c, d, MD5_SET(4), 0xf57c0faf, 7);
				MD5_STEP(MD5_F, d, a, b, c, MD5_SET(5), 0x4787c62a, 12);
				MD5_STEP(MD5_F, c, d, a, b, MD5_SET(6), 0xa8304613, 17);
				MD5_STEP(MD5_F, b, c, d, a, MD5_SET(7), 0xfd469501, 22);
				MD5_STEP(MD5_F, a, b, c, d, MD5_SET(8), 0x698098d8, 7);
				MD5_STEP(MD5_F, d, a, b, c, MD5_SET(9), 0x8b44f7af, 12);
				MD5_STEP(MD5_F, c, d, a, b, MD5_SET(10), 0xffff5bb1, 17);
				MD5_STEP(MD5_F, b, c, d, a, MD5_SET(11), 0x895cd7be, 22);
				MD5_STEP(MD5_F, a, b, c, d, MD5_SET(12), 0x6b901122, 7);
				MD5_STEP(MD5_F, d, a, b, c, MD5_SET(13), 0xfd987193, 12);
				MD5_STEP(MD5_F, c, d, a, b, MD5_SET(14), 0xa679438e, 17);
				MD5_STEP(MD5_F, b, c, d, a, MD5_SET(15), 0x49b40821, 22);

				/* Round 2 */
				MD5_STEP(MD5_G, a, b, c, d, MD5_GET(1), 0xf61e2562, 5);
				MD5_STEP(MD5_G, d, a, b, c, MD5_GET(6), 0xc040b340, 9);
				MD5_STEP(MD5_G, c, d, a, b, MD5_GET(11), 0x265e5a51, 14);
				MD5_STEP(MD5_G, b, c, d, a, MD5_GET(0), 0xe9b6c7aa, 20);
				MD5_STEP(MD5_G, a, b, c, d, MD5_GET(5), 0xd62f105d, 5);
				MD5_STEP(MD5_G, d, a, b, c, MD5_GET(10), 0x02441453, 9);
				MD5_STEP(MD5_G, c, d, a, b, MD5_GET(15), 0xd8a1e681, 14);
				MD5_STEP(MD5_G, b, c, d, a, MD5_GET(4), 0xe7d3fbc8, 20);
				MD5_STEP(MD5_G, a, b, c, d, MD5_GET(9), 0x21e1cde6, 5);
				MD5_STEP(MD5_G, d, a, b, c, MD5_GET(14), 0xc33707d6, 9);
				MD5_STEP(MD5_G, c, d, a, b, MD5_GET(3), 0xf4d50d87, 14);
				MD5_STEP(MD5_G, b, c, d, a, MD5_GET(8), 0x455a14ed, 20);
				MD5_STEP(MD5_G, a, b, c, d, MD5_GET(13), 0xa9e3e905, 5);
				MD5_STEP(MD5_G, d, a, b, c, MD5_GET(2), 0xfcefa3f8, 9);
				MD5_STEP(MD5_G, c, d, a, b, MD5_GET(7), 0x676f02d9, 14);
				MD5_STEP(MD5_G, b, c, d, a, MD5_GET(12), 0x8d2a4c8a, 20);

				/* Round 3 */
				MD5_STEP(MD5_H, a, b, c, d, MD5_GET(5), 0xfffa3942, 4);
				MD5_STEP(MD5_H, d, a, b, c, MD5_GET(8), 0x8771f681, 11);
				MD5_STEP(MD5_H, c, d, a, b, MD5_GET(11), 0x6d9d6122, 16);
				MD5_STEP(MD5_H, b, c, d, a, MD5_GET(14), 0xfde5380c, 23);
				MD5_STEP(MD5_H, a, b, c, d, MD5_GET(1), 0xa4beea44, 4);
				MD5_STEP(MD5_H, d, a, b, c, MD5_GET(4), 0x4bdecfa9, 11);
				MD5_STEP(MD5_H, c, d, a, b, MD5_GET(7), 0xf6bb4b60, 16);
				MD5_STEP(MD5_H, b, c, d, a, MD5_GET(10), 0xbebfbc70, 23);
				MD5_STEP(MD5_H, a, b, c, d, MD5_GET(13), 0x289b7ec6, 4);
				MD5_STEP(MD5_H, d, a, b, c, MD5_GET(0), 0xeaa127fa, 11);
				MD5_STEP(MD5_H, c, d, a, b, MD5_GET(3), 0xd4ef3085, 16);
				MD5_STEP(MD5_H, b, c, d, a, MD5_GET(6), 0x04881d05, 23);
				MD5_STEP(MD5_H, a, b, c, d, MD5_GET(9), 0xd9d4d039, 4);
				MD5_STEP(MD5_H, d, a, b, c, MD5_GET(12), 0xe6db99e5, 11);
				MD5_STEP(MD5_H, c, d, a, b, MD5_GET(15), 0x1fa27cf8, 16);
				MD5_STEP(MD5_H, b, c, d, a, MD5_GET(2), 0xc4ac5665, 23);

				/* Round 4 */
				MD5_STEP(MD5_I, a, b, c, d, MD5_GET(0), 0xf4292244, 6);
				MD5_STEP(MD5_I, d, a, b, c, MD5_GET(7), 0x432aff97, 10);
				MD5_STEP(MD5_I, c, d, a, b, MD5_GET(14), 0xab9423a7, 15);
				MD5_STEP(MD5_I, b, c, d, a, MD5_GET(5), 0xfc93a039, 21);
				MD5_STEP(MD5_I, a, b, c, d, MD5_GET(12), 0x655b59c3, 6);
				MD5_STEP(MD5_I, d, a, b, c, MD5_GET(3), 0x8f0ccc92, 10);
				MD5_STEP(MD5_I, c, d, a, b, MD5_GET(10), 0xffeff47d, 15);
				MD5_STEP(MD5_I, b, c, d, a, MD5_GET(1), 0x85845dd1, 21);
				MD5_STEP(MD5_I, a, b, c, d, MD5_GET(8), 0x6fa87e4f, 6);
				MD5_STEP(MD5_I, d, a, b, c, MD5_GET(15), 0xfe2ce6e0, 10);
				MD5_STEP(MD5_I, c, d, a, b, MD5_GET(6), 0xa3014314, 15);
				MD5_STEP(MD5_I, b, c, d, a, MD5_GET(13), 0x4e0811a1, 21);
				MD5_STEP(MD5_I, a, b, c, d, MD5_GET(4), 0xf7537e82, 6);
				MD5_STEP(MD5_I, d, a, b, c, MD5_GET(11), 0xbd3af235, 10);
				MD5_STEP(MD5_I, c, d, a, b, MD5_GET(2), 0x2ad7d2bb, 15);
				MD5_STEP(MD5_I, b, c, d, a, MD5_GET(9), 0xeb86d391, 21);

				a += saved_a;
				b += saved_b;
				c += saved_c;
				d += saved_d;

				ptr += 64;
			} while ((length -= 64) != 0);

			this->_StateA = a;
			this->_StateB = b;
			this->_StateC = c;
			this->_StateD = d;

			return ptr;
		}

	public:

		static const size_t HASH_SIZE = 16;
		static const size_t DIGEST_SIZE = 32;
		static const size_t BLOCK_SIZE = 64;

		explicit MD5() {
			this->Reset();
		}

		void Reset(void) {
			this->_StateA = 0x67452301;
			this->_StateB = 0xefcdab89;
			this->_StateC = 0x98badcfe;
			this->_StateD = 0x10325476;

			this->_Lo = 0;
		}

		void Update(const void* data, uint32_t size) {
			if (data == NULL || size == 0) {
				return;
			}

			uint64_t saved_lo = this->_Lo;
			uint32_t used;
			uint32_t free_space;

			if ((saved_lo + size) < saved_lo) {
				return;
			}
			this->_Lo = saved_lo + size;

			used = (uint32_t)(saved_lo & 0x3f);

			if (used) {
				free_space = 64U - used;

				if (size < free_space) {
					memcpy(&this->_Buffer[used], data, size);
					return;
				}

				memcpy(&this->_Buffer[used], data, free_space);
				data = (const uint8_t*)data + free_space;
				size -= free_space;

				this->Process(this->_Buffer, 64);
			}

			if (size >= 64) {
				data = this->Process(data, size & ~(size_t)0x3f);
				size &= 0x3f;
			}

			if (size > 0) {
				memcpy(this->_Buffer, data, size);
			}
		}

		void Finalize(uint8_t* result) {
			if (result == NULL) {
				return;
			}

			uint32_t used = (uint32_t)(this->_Lo & 0x3f);
			uint32_t free_space;

			this->_Buffer[used++] = 0x80;
			free_space = 64 - used;

			if (free_space < 8) {
				memset(&this->_Buffer[used], 0, free_space);
				this->Process(this->_Buffer, 64);
				used = 0;
				free_space = 64;
			}

			memset(&this->_Buffer[used], 0, free_space - 8);

			uint64_t bits = this->_Lo << 3;
			this->_Buffer[56] = (uint8_t)(bits);
			this->_Buffer[57] = (uint8_t)(bits >> 8);
			this->_Buffer[58] = (uint8_t)(bits >> 16);
			this->_Buffer[59] = (uint8_t)(bits >> 24);
			this->_Buffer[60] = (uint8_t)(bits >> 32);
			this->_Buffer[61] = (uint8_t)(bits >> 40);
			this->_Buffer[62] = (uint8_t)(bits >> 48);
			this->_Buffer[63] = (uint8_t)(bits >> 56);

			this->Process(this->_Buffer, 64);

			result[0] = (uint8_t)this->_StateA;
			result[1] = (uint8_t)(this->_StateA >> 8);
			result[2] = (uint8_t)(this->_StateA >> 16);
			result[3] = (uint8_t)(this->_StateA >> 24);
			result[4] = (uint8_t)this->_StateB;
			result[5] = (uint8_t)(this->_StateB >> 8);
			result[6] = (uint8_t)(this->_StateB >> 16);
			result[7] = (uint8_t)(this->_StateB >> 24);
			result[8] = (uint8_t)this->_StateC;
			result[9] = (uint8_t)(this->_StateC >> 8);
			result[10] = (uint8_t)(this->_StateC >> 16);
			result[11] = (uint8_t)(this->_StateC >> 24);
			result[12] = (uint8_t)this->_StateD;
			result[13] = (uint8_t)(this->_StateD >> 8);
			result[14] = (uint8_t)(this->_StateD >> 16);
			result[15] = (uint8_t)(this->_StateD >> 24);
		}

		static void Hash(const char* str, unsigned char* hash) {
			if (str == NULL || hash == NULL) {
				return;
			}

			MD5 md5;
			size_t len = strlen(str);
			if (len > 0) {
				md5.Update(str, (uint32_t)len);
			}
			md5.Finalize(hash);
		}

		static void Hash(const void* buffer, size_t size, unsigned char* hash) {
			if (hash == NULL) {
				return;
			}

			MD5 md5;
			if (buffer != NULL && size > 0) {
				md5.Update(buffer, (uint32_t)size);
			}
			md5.Finalize(hash);
		}

		static void Digest(const unsigned char* hash, char* digest) {
			if (hash == NULL || digest == NULL) {
				return;
			}

			static const char digits[17] = "0123456789abcdef";

			for (int i = 0; i < 16; ++i) {
				digest[i * 2] = digits[hash[i] >> 4];
				digest[(i * 2) + 1] = digits[hash[i] & 0x0F];
			}

			digest[32] = 0;
		}
};

#endif