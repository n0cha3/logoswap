#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "splash.h"
#include "bitmap.h"

#define BSWAP_32(x) ((((x) & 0xFF000000) >> 24) | (((x) & 0x00FF0000) >>  8) | (((x) & 0x0000FF00) <<  8) | (((x) & 0x000000FF) << 24))
#define BSWAP_16(x) ((((x)  & 0xFF) << 8) | ((x) >> 8))
