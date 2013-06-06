#include <openssl/evp.h>
#include "string.h"
#include <stdio.h>
#include "../lib/util.h"
#include "../lib/crypto.h"

#define ENCRYPT 1
#define DECRYPT 0
#define SUCCESS 1
#define ERROR 0

const EVP_CIPHER * get_evp_algorithm(const char*algorithm, const char * mode);

char * encrypt(char * original, int * encrypted_size, const char* algorithm,
		const char * mode, const char * pass) {

	const EVP_CIPHER *cipher = get_evp_algorithm(algorithm, mode);

	unsigned char *key = malloc(
			sizeof(unsigned char) * EVP_CIPHER_key_length(cipher));
	unsigned char *iv = malloc(
			sizeof(unsigned char) * EVP_CIPHER_iv_length(cipher));

	EVP_BytesToKey(cipher, EVP_md5(), NULL, pass, strlen(pass), 1, key, iv);

	char *out = calloc(*((int*) original) + 16 + 1, sizeof(char));
	int out_partial_size1 = 0;
	int out_partial_size2 = 0;

	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	if (EVP_CipherInit_ex(&ctx, cipher, NULL, key, iv, ENCRYPT) == ERROR) {
		printf("error");
	}
	if (EVP_CipherUpdate(&ctx, out, &out_partial_size1, original,
			*((int*) original)) == ERROR) {
		printf("error");
	}
	if (EVP_CipherFinal_ex(&ctx, out + out_partial_size1,
			&out_partial_size2) == ERROR) {
		printf("error");
	}

	if (EVP_CIPHER_CTX_cleanup(&ctx) == ERROR) {
		printf("error");
	}

	*encrypted_size = out_partial_size1 + out_partial_size2;
	return out;
}

char * decrypt(char * encrypted, int * decrypted_size, const char* algorithm,
		const char * mode, const char * pass) {

	const EVP_CIPHER *cipher = get_evp_algorithm(algorithm, mode);
	unsigned char *key = malloc(
			sizeof(unsigned char) * EVP_CIPHER_key_length(cipher));
	unsigned char *iv = malloc(
			sizeof(unsigned char) * EVP_CIPHER_iv_length(cipher));

	printf("%d", strlen(pass));
	if (EVP_BytesToKey(cipher, EVP_md5(), NULL, pass, strlen(pass), 1, key,
			iv)==ERROR) {
		printf("error");
	}

	unsigned char *out = calloc(*((int*) encrypted) + 16 + 1, sizeof(char));
	int out_partial_size1 = 0;
	int out_partial_size2 = 0;
	int out_size = 0;
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	if (EVP_CipherInit_ex(&ctx, cipher, NULL, key, iv, DECRYPT) == ERROR) {
		printf("error");
	}
	if (EVP_CipherUpdate(&ctx, out, &out_partial_size1, encrypted,
			*((int*) encrypted)) == ERROR) {
		printf("error");
	}
	if (EVP_CipherFinal_ex(&ctx, out + out_partial_size1,
			&out_partial_size2) == ERROR) {
		printf("error");
	}

	if (EVP_CIPHER_CTX_cleanup(&ctx) == ERROR) {
		printf("error");
	}

	*decrypted_size = out_partial_size1 + out_partial_size2;
	return out;
}

const EVP_CIPHER * get_evp_algorithm(const char*algorithm, const char * mode) {
	OpenSSL_add_all_ciphers();

	if (strcmp(algorithm, AES_128) == 0) {
		if (strcmp(mode, ECB) == 0) {
			return EVP_get_cipherbyname("aes-128-ecb");
		} else if (strcmp(mode, CFB) == 0) {
			return EVP_get_cipherbyname("aes-128-cfb");
		} else if (strcmp(mode, OFB) == 0) {
			return EVP_get_cipherbyname("aes-128-ofb");
		} else if (strcmp(mode, CBC) == 0) {
			return EVP_get_cipherbyname("aes-128-cbc");
		}
	} else if (strcmp(algorithm, AES_192) == 0) {
		if (strcmp(mode, ECB) == 0) {
			return EVP_get_cipherbyname("aes-192-ecb");
		} else if (strcmp(mode, CFB) == 0) {
			return EVP_get_cipherbyname("aes-192-cfb");
		} else if (strcmp(mode, OFB) == 0) {
			return EVP_get_cipherbyname("aes-192-ofb");
		} else if (strcmp(mode, CBC) == 0) {
			return EVP_get_cipherbyname("aes-192-cbc");
		}
	} else if (strcmp(algorithm, AES_256) == 0) {
		if (strcmp(mode, ECB) == 0) {
			return EVP_get_cipherbyname("aes-256-ecb");
		} else if (strcmp(mode, CFB) == 0) {
			return EVP_get_cipherbyname("aes-256-cfb");
		} else if (strcmp(mode, OFB) == 0) {
			return EVP_get_cipherbyname("aes-256-ofb");
		} else if (strcmp(mode, CBC)) {
			return EVP_get_cipherbyname("aes-256-cbc");
		}
	} else if (strcmp(algorithm, DES) == 0) {
		if (strcmp(mode, ECB) == 0) {
			return EVP_get_cipherbyname("des-ecb");
		} else if (strcmp(mode, CFB) == 0) {
			return EVP_get_cipherbyname("des-cfb");
		} else if (strcmp(mode, OFB) == 0) {
			return EVP_get_cipherbyname("des-ofb");
		} else if (strcmp(mode, CBC) == 0) {
			return EVP_get_cipherbyname("des-cbc");
		}
	}

	return EVP_get_cipherbyname("aes-128-ccb");
}
