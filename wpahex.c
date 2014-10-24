#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <openssl/evp.h>

extern char *__progname;

int
main(int argc, char **argv)
{
	char ssid[32] = {0};
	char passphrase[64] = {0};
	unsigned char digest[32] = {0};
	const EVP_MD *md = NULL;

	if (argc != 3) {
		fprintf(stderr, "usage: %s <ssid> <passphrase>\n",
		    __progname);
		return 1;
	}

	strlcpy(ssid, argv[1], sizeof(ssid));
	strlcpy(passphrase, argv[2], sizeof(passphrase));

	OpenSSL_add_all_digests();
	md = EVP_get_digestbyname("sha1");
	if (!md) {
		printf("Unknown message digest sha1\n");
		return 2;
	}

	int retval = PKCS5_PBKDF2_HMAC(passphrase, strlen(passphrase),
	    (const unsigned char*)ssid, strlen(ssid), 4096, md, sizeof(digest), digest);

	if (retval == 0) {
		printf("pbkdf error!\n");
		return 3;
	}

	for (int i = 0; i < sizeof(digest); i++) {
		printf("%x", digest[i]);
	}

	printf("\n");

	EVP_cleanup();
	return 0;
}
