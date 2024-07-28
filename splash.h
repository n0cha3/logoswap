#pragma once

#include "main.h"

#define LOGO_HEADER_SIZE 64

typedef struct _offset {
  uint32_t Count;
  bool HasLogoHeader;
  size_t *OffsetList;
  size_t EndOfFile;
} offset_t;

void GetOffsets(FILE *Logoimg, offset_t *Offsets);
size_t fread_offset(void *Dest, size_t SizeOfEach, size_t Count, FILE *FilePointer, size_t Offset);
size_t GetFileSize(FILE *Filep);
