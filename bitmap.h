#pragma once

#include "main.h"

typedef struct _bmp {
  uint16_t Width;
  uint16_t Height;
  uint32_t HeadSize;
  char *Data;
  uint32_t RealSize;
} bmp_t;

void ReadMemBmp(bmp_t *Bitmap, FILE *FilePointer, size_t Offset);
void GetBmpSize(bmp_t *BitmapInfo);
bool ReadBMP(FILE *Bitmap, bmp_t *BitmapInfo);
