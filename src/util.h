#pragma once

#include <stdio.h>

#define p_err(fmt, ...) fprintf(stderr, "ERROR: " fmt, ##__VA_ARGS__);
#define p_log(fmt, ...) fprintf(stderr, "LOG: " fmt, ##__VA_ARGS__);
#define p_info(fmt, ...) fprintf(stderr, "INFO: " fmt, ##__VA_ARGS__);
#define p_debug(fmt, ...) fprintf(stderr, "DEBUG: " fmt, ##__VA_ARGS__);
#define p_warn(fmt, ...) fprintf(stderr, "WARN: " fmt, ##__VA_ARGS__);
