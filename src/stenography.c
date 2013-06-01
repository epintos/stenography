#include <math.h>
#include <mcrypt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/util.h"
#include "../lib/stenography.h"
#include "../lib/recover.h"
#include "../lib/hide.h"

int emb(const char* in, const char * p, const char * out, const char * steg,
		const char * a, const char * m, const char * pass) {
	char* IV = "AAAAAAAAAAAAAAAA";
	char *key = "0123456789abcdef";
	int keysize = 16; /* 128 bits */
	char* msg = parse_message_to_hide(in);
	int size = *((int*) msg);
	printf("Mensaje:%d %s\n", size, msg + 4);
	if (*pass) {
		char* buffer;
		int buffer_len = ceil(size / 16.0) * 16;
		buffer = calloc(1, buffer_len);
		memcpy(buffer, msg, buffer_len);

		encrypt(buffer, buffer_len, IV, key, keysize, DES, "cbc");
		char* encrypted = preappend_size(buffer);
		printf("Encrypted:%d %s\n", *((int*) encrypted), encrypted + 4);
		msg = encrypted;
	}
	hide_message(p, msg, out, steg_from_string(steg));
}

int ext(const char * p, const char * out, const char * steg, const char * a,
		const char * m, const char * pass) {
	char* IV = "AAAAAAAAAAAAAAAA";
	char *key = "0123456789abcdef";
	int keysize = 16; /* 128 bits */
	char* buffer;
	int parsed_steg = steg_from_string(steg);
	char* recovered = recover_msg(p, parsed_steg);
	printf("Recuperado: %d %s\n", *((int*) recovered), recovered + 4);

	char* output;
	if (*pass) {
		// Desencriptar
		int encrypted_size = *((int*) recovered);
		int buffer_len = ceil(encrypted_size / 16.0) * 16;
		buffer = calloc(1, buffer_len);
		memcpy(buffer, recovered+4, buffer_len);
		decrypt(buffer, buffer_len, IV, key, keysize, DES, "cbc");
		output = buffer + 4;
		printf("Decrypted:%s\n", output);
	} else {
		output = recovered + 4;
	}

	FILE* out_file = fopen(out, "w");
	fwrite(output, 1, strlen(output), out_file);
	fclose(out_file);
}
