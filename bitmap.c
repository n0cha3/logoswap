#include "bitmap.h"

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
      printf("resolution %dx%d\n Header size:%d\n offset: 0x%x\n", Bitmap->Height, Bitmap->Width, Bitmap->HeadSize, ftell(FilePointer));
    }
   }
  else fputs("Error: Invalid File Pointer\n", stderr);
}

void GetBmpSize(bmp_t *BitmapInfo) {
  uint32_t *Buf = (uint32_t *)(BitmapInfo->Data + 2);
  if (IsLittleEndian()) {  
    BitmapInfo->HeadSize = Buf[0];
    BitmapInfo->Width = Buf[4];
    BitmapInfo->Height = Buf[5];
  }

  else {
    BitmapInfo->HeadSize = BSWAP_32(Buf[0]);
    BitmapInfo->Width = BSWAP_16(Buf[4]);
    BitmapInfo->Height = BSWAP_16(Buf[5]);
  }
}

bool ReadBMP(FILE *Bitmap, bmp_t *BitmapInfo) {
  
  if (Bitmap != NULL && CheckBmp(Bitmap) == true) {

    fseek(Bitmap, 0, SEEK_END);
    
    printf("File size: %u\n", BitmapInfo->RealSize);
    
    fseek(Bitmap, 0, SEEK_SET);
    
    BitmapInfo->Data = calloc(BitmapInfo->RealSize, sizeof(char));
    fread(BitmapInfo->Data, sizeof(char), BitmapInfo->RealSize, Bitmap);
    
     GetBmpSize(BitmapInfo);
     
     printf("Width:\t%d\nHeight:\t%d\nHeader size:\t%d\n", BitmapInfo->Width, BitmapInfo->Height, BitmapInfo->HeadSize);
  return true;

  }

  else return false;
}
