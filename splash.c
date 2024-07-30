#include "splash.h"

size_t fread_offset(void *Dest, size_t SizeOfEach, size_t Count, FILE *FilePointer, size_t Offset) {
  fseek(FilePointer, (long)Offset, SEEK_SET);
  return fread(Dest, SizeOfEach, Count, FilePointer);
}

size_t GetFileSize(FILE *Filep) {
  if (Filep != NULL) {
    fseek(Filep, 0, SEEK_END);
    size_t FileSize = ftell(Filep);
    rewind(Filep);
    return FileSize;
  }
  else return 0;
}

void GetOffsets(FILE *Logoimg, offset_t *Offsets) {

  if (Logoimg != NULL) {
    size_t LogoSize = GetFileSize(Logoimg);
    Offsets->EndOfFile = LogoSize;
    uint16_t HdrCount = 0;
    char *Buffer = calloc(LogoSize, sizeof(char));

    if (Buffer != NULL) {
      fread(Buffer, sizeof(char), LogoSize, Logoimg);
    }
    else return;

    for (size_t a = 0; a < LogoSize; a++) {
      if ((Buffer[a] == 'L') && (Buffer[a+1] == 'O') && (Buffer[a+2] == 'G') && (Buffer[a+3] == 'O')) {
        HdrCount++;
        Offsets->HasLogoHeader = true;
      }

      
      if ((Buffer[a] == 'B') && (Buffer[a+1] == 'M')) HdrCount++;
      }

    Offsets->Count = HdrCount;
    Offsets->OffsetList = calloc(Offsets->Count, sizeof(size_t));
    HdrCount = 0;

    if (Offsets->OffsetList != NULL) {
     for (size_t a = 0; a < LogoSize; a++) {
       if ((Buffer[a] == 'L') && (Buffer[a+1] == 'O') && (Buffer[a+2] == 'G') && (Buffer[a+3] == 'O')) Offsets->OffsetList[HdrCount] = a;
      
       if ((Buffer[a] == 'B') && (Buffer[a+1] == 'M')) {
	 HdrCount++;
	 Offsets->OffsetList[HdrCount] = a;
       }
      }
    }
    free(Buffer);
    rewind(Logoimg);

   }
}
