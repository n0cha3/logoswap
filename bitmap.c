#include "bitmap.h"

static bool IsLittleEndian(void) {
  uint32_t a = 1;
  if (*(char *)&a == 1) return true;
  else return false;
}

static bool CheckBmp(FILE *Bitmap) {
  if ((fgetc(Bitmap) == 'B') && (fgetc(Bitmap) == 'M')) return true;
  else return false;
}

void ReadMemBmp(bmp_t *Bitmap, FILE *FilePointer, size_t Offset) {
  if (FilePointer != NULL) {
    Bitmap->Data = calloc(64, sizeof(char));
    if (Bitmap->Data != NULL) {
      fread_offset(Bitmap->Data, sizeof(char), 64, FilePointer, Offset);
      GetBmpSize(Bitmap);
      Bitmap->RealSize = Bitmap->HeadSize;
      Bitmap->Data = realloc(Bitmap->Data, Bitmap->HeadSize);
      fread_offset(Bitmap->Data, sizeof(char), Bitmap->RealSize, FilePointer, Offset);
      printf("resolution %dx%d\n Header size:%d\n offset: 0x%x\n", Bitmap->Height, Bitmap->Width, Bitmap->HeadSize, (uint32_t)ftell(FilePointer));
    }
   }
  else fputs("Error: Invalid File Pointer\n", stderr);
}

void GetBmpSize(bmp_t *BitmapInfo) {
  uint32_t *Ptr32 = (uint32_t *)(BitmapInfo->Data + 2);
  if (IsLittleEndian()) {  
    BitmapInfo->HeadSize = Ptr32[0];
    BitmapInfo->Width = (uint16_t)Ptr32[4];
    BitmapInfo->Height = (uint16_t)Ptr32[5];
  }

  else {
    BitmapInfo->HeadSize = BSWAP_32(Ptr32[0]);
    BitmapInfo->Width = BSWAP_16((uint16_t)Ptr32[4]);
    BitmapInfo->Height = BSWAP_16((uint16_t)Ptr32[5]);
  }
}

bool ReadBMP(FILE *Bitmap, bmp_t *BitmapInfo) {
  
  if (Bitmap != NULL && CheckBmp(Bitmap) == true) {

    BitmapInfo->RealSize = GetFileSize(Bitmap);
    
    printf("File size: %u\n", (uint32_t)BitmapInfo->RealSize);
    
    BitmapInfo->Data = calloc(BitmapInfo->RealSize, sizeof(char));
    if (BitmapInfo->Data == NULL) return false;

    fread(BitmapInfo->Data, sizeof(char), BitmapInfo->RealSize, Bitmap);
    
    GetBmpSize(BitmapInfo);
     
    printf("Width:\t%d\nHeight:\t%d\nHeader size:\t%d\n", BitmapInfo->Width, BitmapInfo->Height, BitmapInfo->HeadSize);
    return true;

  }

  else return false;
}
