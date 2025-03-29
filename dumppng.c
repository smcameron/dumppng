#include <ctype.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "png_utils.h"

static void usage(void)
{
	fprintf(stderr, "usage: dumppng file1.png [ ... ]\n");
	exit(1);
}

struct png_file_bytes {
	int w, h, a;
	char *bytes;
};

static int define_struct(void)
{
	printf("struct png_file_bytes {\n");
	printf("	int w, h, a;\n");
	printf("	unsigned char bytes[];\n");
	printf("};\n");
	printf("\n");
}

static int process_file(char *filename)
{
	char whynot[256];
	struct png_file_bytes pfb;
	char varname[256];
	int i, j;

	/* copy filename to fname, cutting off trailing ".png" */
	char *fname = strdup(filename);
	int l = strlen(fname);
	if (l > 4) {
		if (strcasecmp(&fname[l - 4], ".png") == 0)
			fname[l - 4] = '\0';
	}
	char *bfname = basename(fname);

	/* Construct a variable name from the filename */
	j = 0;
	for (i = 0; i < 256 && bfname[i] != '\0'; i++) {
		if (bfname[i] >= 'A' && bfname[i] <= 'Z')
			varname[j++] = bfname[i];
		else if (bfname[i] >= 'a' && bfname[i] <= 'z')
			varname[j++] = bfname[i];
		else if (bfname[i] >= '0' && bfname[i] <= '9')
			varname[j++] = bfname[i];
		else if (bfname[i] == '-')
			varname[j++] = '_';
		else
			continue;
	}
	free(fname);

	char *bytes = png_utils_read_png_image(filename, 0, 0, 1, &pfb.w, &pfb.h, &pfb.a, whynot, sizeof(whynot));
	if (!bytes) {
		fprintf(stderr, "Failed to read png file '%s': %s\n", filename, whynot);
		return -1;
	}

	printf("static struct png_file_bytes %s = {\n", varname);
	printf("	.w = %d,\n", pfb.w);
	printf("	.h = %d,\n", pfb.h);
	printf("	.a = %d,\n", pfb.a);
	printf("	.bytes = {\n");


	int bytes_per_row = (3 + pfb.a) * pfb.w;

	/* align to 4 byte boundary */
	if ((bytes_per_row % 4) != 0)
		bytes_per_row = (bytes_per_row + 4) & ~0x03;

	int bytes_per_line = 15;

	for (i = 0; i < bytes_per_row * pfb.h; i++) {
		int beginning = ((i % bytes_per_line) == 0);
		int endofline = ((i % bytes_per_line) == (bytes_per_line - 1));
		printf("%s0x%02hhx,%s", beginning ? "\t\t" : "", bytes[i], endofline ? "\n" : " ");
	}

	printf("\n	},\n");
	printf("};\n\n");

	return 0;
}

int main(int argc, char *argv[])
{
	int rc = 0;

	if (argc < 2)
		usage();

	define_struct();

	/* Suppress warnings about initializing flexible array members */
	printf("#pragma GCC diagnostic push\n");
	printf("#pragma GCC diagnostic ignored \"-Wpedantic\"\n");
	for (int i = 1; i < argc; i++) {
		rc += process_file(argv[i]);
	}
	printf("#pragma GCC diagnostic pop\n");
	return rc != 0;
}

