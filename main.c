#include "main.h"

static const char Chars[36] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if ((argc > 2) && !strcmp(argv[1], "-p")) {
        
    }

    
    else if ((argc > 2) && !strcmp(argv[1], "-e")) {
        const uint32_t argcc = argc;
        uint32_t ArgCount = argcc - (argcc - 3);
        offset_t Offsets = {0, false, NULL, 0};
        bmp_t Bitmap = {0, 0, 0, NULL, 0};
        
        FILE *Filep = fopen(argv[2], "rb");

        if (Filep != NULL) {

            rewind(Filep);
            GetOffsets(Filep, &Offsets);

            if (Offsets.Count != 0 && Offsets.HasLogoHeader == true) {

              if (argc == 3) {
                  for (uint32_t a = 1; a < Offsets.Count; a++) {
                    ReadMemBmp(&Bitmap, Filep, Offsets.OffsetList[a]);
                    char Filename[14] = "bmp000000.bmp\0";    
                    for (int a = 3; a < 8; a++) Filename[a] = Chars[rand() % 36];

                    FILE *Output = fopen(Filename, "wb");

                    if ((Bitmap.Data != NULL) && (Output != NULL)) {
                      fwrite(Bitmap.Data, sizeof(char), Bitmap.HeadSize, Output);
                      printf("Written to %s\n", Filename);
                      free(Bitmap.Data);
                      fclose(Output);
                    }
              }
              
              if (Offsets.OffsetList != NULL) free(Offsets.OffsetList);

            }

            if (argc > 3) {
                printf("args: %d\n", argc);
                  for (uint32_t a = 1; a < Offsets.Count; a++) {
                    ReadMemBmp(&Bitmap, Filep, Offsets.OffsetList[a]);
                    char Filename[14] = "bmp000000.bmp\0";
                    FILE *Output;
                    for (int t = 3; t < 8; t++) Filename[t] = Chars[rand() % 36];
                    

                    if (ArgCount < argcc) {
                      Output = fopen(argv[ArgCount], "wb");
                    }
                    
                    else {
                      Output = fopen(Filename, "wb");
                    }

                    ArgCount++;
                    
                    if ((Bitmap.Data != NULL) && (Output != NULL)) {
                        fwrite(Bitmap.Data, sizeof(char), Bitmap.HeadSize, Output);

                        if ((ArgCount - 1) < argcc) 
                          printf("Written to %s %d\n", argv[ArgCount - 1], ArgCount);
                        
                        else 
                          printf("Written to %s\n", Filename);

                        free(Bitmap.Data);
                        fclose(Output);
                    }
                }
                   
              }

              if (Offsets.OffsetList != NULL) free(Offsets.OffsetList);

            }
            
      fclose(Filep);
    }
            else {
              fputs("Bad image file\n", stderr);
              return EXIT_FAILURE;
            }

  }


    else if ((argc > 1) && !strcmp(argv[1], "-h")) {
          printf("%s\n%s\n%s\n%s\n%s\n", 
          "Usage:", 
          "-e [logo.img] [BitmapN.bmp] - Extract the bitmaps from image (if file names are not specified it will use default ones)", 
          "-o [logo.img] - Output offsets", 
          "-hdr [logo.img] [Header.bin] - Get partition header (into a file)", 
          "-p [logo.img] [BitmapN.bmp] ... - Pack bitmaps into image");
        }

    else if ((argc > 2) && !strcmp(argv[1], "-o")) {
          offset_t Offsets = {0, false, NULL, 0};

          FILE *Filep = fopen(argv[2], "rb");

          if (Filep != NULL) {

            rewind(Filep);
            GetOffsets(Filep, &Offsets);

            if (Offsets.Count != 0 && Offsets.HasLogoHeader == true) {
              
              printf("Bitmaps: %d\nEOF: %d\n", Offsets.Count - 1, Offsets.EndOfFile);
              for (uint32_t a = 0; a < Offsets.Count; a++) printf("Offset %u : 0x%x\n", a, Offsets.OffsetList[a]);
              if (Offsets.OffsetList != NULL) free(Offsets.OffsetList);

            }

            else {

              fputs("Bad image file\n", stderr);
              return EXIT_FAILURE;

            }

          }

          else {
            fputs("Invalid file name.\n", stderr);
          }
    }

    else if ((argc > 2) && !strcmp(argv[1], "-hdr")) {

          offset_t Offsets = {0, false, NULL, 0};
          FILE *Filep = fopen(argv[2], "rb");

          if (Filep != NULL) {

            rewind(Filep);
            GetOffsets(Filep, &Offsets);

            if (Offsets.Count != 0 && Offsets.HasLogoHeader == true) {

              if (argc < 4) {

                char Filename[14] = "hdr000000.bin\0";    

                for (int a = 3; a < 8; a++) Filename[a] = Chars[rand() % 36];

                FILE *Filepointer = fopen(Filename, "wb");
                char Header[LOGO_HEADER_SIZE]; 

                  if (Filepointer != NULL) {
                    fread_offset(Header, sizeof(char), LOGO_HEADER_SIZE, Filep, Offsets.OffsetList[0]);
                    fwrite(Header, sizeof(char), LOGO_HEADER_SIZE, Filepointer);
                    fclose(Filepointer);
                    printf("Header written to: %s\n", Filename);
                  }

                }

              else {

                FILE *Filepointer = fopen(argv[3], "wb");

                char Header[LOGO_HEADER_SIZE]; 

                  if (Filepointer != NULL) {

                    fread_offset(Header, sizeof(char), LOGO_HEADER_SIZE, Filep, Offsets.OffsetList[0]);
                    fwrite(Header, sizeof(char), LOGO_HEADER_SIZE, Filepointer);
                    fclose(Filepointer);
                    printf("Header written to: %s\n", argv[3]);

                  }

              }

              if (Offsets.OffsetList != NULL) free(Offsets.OffsetList);
              fclose(Filep);
            }

            else { 
              fputs("Bad image file\n", stderr);
              return EXIT_FAILURE;
            }
          }

          else {
            fputs("Invalid file name.\n", stderr);
          }
    }
    

    else fprintf(stderr, "%s\n%s\n", "Invalid arguments, not enough arguments or too many arguments.", "Use flag -h to get help");

  return EXIT_SUCCESS;
}
