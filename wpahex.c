#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

extern char *__progname;
static const char DIGEST_NAME[] = "sha1";

int
main(int argc, char **argv)
{
	char ssid[32] = {0};
	size_t ssid_length = 0;
	char pass[64] = {0};
	size_t pass_length = 0;
	unsigned char output[32] = {0};
	const EVP_MD *md = NULL;

	if (argc != 3) {
		fprintf(stderr, "usage: %s <ssid> <pass>\n",
		    __progname);
		return 1;
	}

	ssid_length = strlcpy(ssid, argv[1], sizeof(ssid));
	pass_length = strlcpy(pass, argv[2], sizeof(pass));

	OpenSSL_add_all_digests();
	md = EVP_get_digestbyname(DIGEST_NAME);
	if (!md) {
		fprintf(stderr, "Unknown message digest '%s'\n",
		    DIGEST_NAME);
		return 2;
	}

	int retval = PKCS5_PBKDF2_HMAC(pass, pass_length,
	    (const unsigned char*)ssid, ssid_length, 4096, md, sizeof(output), output);

	if (retval == 0) {
		fprintf(stderr, "pbkdf error %d\n", retval);
		return 3;
	}

	for (int i = 0; i < sizeof(output); i++) {
		printf("%x", output[i]);
	}

	printf("\n");

	EVP_cleanup();
	return 0;
}
