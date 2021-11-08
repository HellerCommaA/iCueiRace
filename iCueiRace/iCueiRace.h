#pragma once

#include <cstdio>



#define IRACE_DEBUG 1
#define LOGD(fmt, ...) do { if (IRACE_DEBUG) LOG(stdout, fmt, __VA_ARGS__); } while (0)

#define IRACE_ERR 1
#define LOGE(fmt, ...) do { if (IRACE_ERR) LOG(stderr, fmt, __VA_ARGS__); } while (0)


#define LOG(OUT, fmt, ...) \
	do { \
		fprintf(OUT, "%s: " fmt "\n", __func__, __VA_ARGS__); \
		fflush(OUT); \
	} while (0)

