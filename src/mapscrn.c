/*
 * mapscrn.c
 */
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <fcntl.h>
#include <sysexits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/kd.h>

#include "libcommon.h"
#include "kfont.h"

int main(int argc, char *argv[])
{
	int fd, ret;
	struct kfont_context ctx;

	set_progname(argv[0]);
	setuplocale();

	kfont_init(&ctx);

	if (argc == 2 && !strcmp(argv[1], "-V"))
		print_version_and_exit();

	if (argc > 1 && !strcmp(argv[1], "-v")) {
		ctx.verbose++;
		argc--;
		argv++;
	}

	if ((fd = getfd(NULL)) < 0)
		kbd_error(EXIT_FAILURE, 0, _("Couldn't get a file descriptor referring to the console"));

	if (argc >= 3 && !strcmp(argv[1], "-o")) {
		ret = kfont_saveoldmap(&ctx, fd, argv[2]);
		if (ret < 0)
			exit(-ret);
		argc -= 2;
		argv += 2;
		if (argc == 1)
			exit(EXIT_SUCCESS);
	}

	if (argc != 2) {
		fprintf(stderr, _("usage: %s [-V] [-v] [-o map.orig] map-file\n"),
		        get_progname());
		exit(EXIT_FAILURE);
	}
	ret = kfont_loadnewmap(&ctx, fd, argv[1]);
	if (ret < 0)
		exit(-ret);
	exit(EXIT_SUCCESS);
}
