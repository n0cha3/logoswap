#include "main.h"

const char Chars[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static bool ExtractHeader(int argc, char *argv[]) {
          offset_t Offsets = {0, false, NULL, 0};
          FILE *Filep = fopen(argv[2], "rb");

          if (Filep != NULL) {
            rewind(Filep);
            GetOffsets(Filep, &Offsets);

            if (Offsets.Count != 0 && Offsets.HasLogoHeader == true) {

              if (argc < 4) {

                char Filename[14] = "hdr000000.bin";    

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

          else fputs("Invalid file name.\n", stderr);
          return EXIT_SUCCESS;
}

static bool ExtractBMP(int argc, char *argv[]) {
        const uint32_t argcc = argc;
        uint32_t ArgCount = argc - (argc - 3);
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
                    char Filename[14] = "bmp000000.bmp";    
                    for (uint32_t i = 3; i < 8; i++) Filename[i] = Chars[rand() % 36];

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

                    for (uint32_t c = argcc - 3; c < argcc; c++) {
                      if (!strcmp(argv[c], argv[c - 1])) {
                        fputs("Matching filenames", stderr);
                        fclose(Filep);
                        free(Offsets.OffsetList);
                        return EXIT_FAILURE;
                      }
                    }
                    
                  for (uint32_t a = 1; a < Offsets.Count; a++) {
                    ReadMemBmp(&Bitmap, Filep, Offsets.OffsetList[a]);
                    char Filename[14] = "bmp000000.bmp";
                    FILE *Output = NULL;

                    for (uint32_t t = 3; t < 8; t++) Filename[t] = Chars[rand() % 36];

                    if (ArgCount < argcc) Output = fopen(argv[ArgCount], "wb");
                    
                    else Output = fopen(Filename, "wb");

                    ArgCount++;
                    
                    if ((Bitmap.Data != NULL) && (Output != NULL)) {
                        fwrite(Bitmap.Data, sizeof(char), Bitmap.HeadSize, Output);

                        if ((ArgCount - 1) < argcc) printf("Written to %s %d\n", argv[ArgCount - 1], ArgCount);
                        
                        else printf("Written to %s\n", Filename);

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

  return EXIT_SUCCESS;
}

static bool ExtractOut(char *argv[]) {
          offset_t Offsets = {0, false, NULL, 0};

          FILE *Filep = fopen(argv[2], "rb");

          if (Filep != NULL) {

            rewind(Filep);
            GetOffsets(Filep, &Offsets);

            if (Offsets.Count != 0 && Offsets.HasLogoHeader == true) {
              
              printf("Bitmaps: %u\nEOF: %u\n", Offsets.Count - 1, (uint32_t)Offsets.EndOfFile);
              for (uint32_t a = 0; a < Offsets.Count; a++) printf("Offset %u : 0x%x\n", a, (uint32_t)Offsets.OffsetList[a]);

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
            return EXIT_FAILURE;
          }
  return EXIT_SUCCESS;
}

static bool PackBMP(int argc, char *argv[]) {
        bmp_t *LogoImgBmp = NULL, *UserBmp = NULL;
        FILE *LogoImg = fopen(argv[2], "rb"),
        *OutputFile = fopen(argv[3], "wb");
        offset_t Offsets = {0, false, NULL, 0};
        char *Output = NULL;

        if (LogoImg != NULL && OutputFile != NULL) {
          GetOffsets(LogoImg, &Offsets);
          rewind(LogoImg);

          if (Offsets.Count != 0 && Offsets.HasLogoHeader) {
            LogoImgBmp = calloc(Offsets.Count - 1, sizeof(bmp_t));
            UserBmp = calloc(argc - 4, sizeof(bmp_t));

              printf("\n%s %s:\n", argv[2], "bitmaps");
              for (uint32_t c = 0; c < Offsets.Count - 1; c++) {
                    printf("\n%u:\n", c + 1);
                    if (ReadMemBmp(&LogoImgBmp[c], LogoImg, Offsets.OffsetList[c + 1])) {
                    }
                    
                    else {
                      fputs("Bad image file\n", stderr);
                      return EXIT_FAILURE;
                    }
                }
                puts("\nUser bitmaps:\n");

                for (int32_t c = argc - (argc - 4); c < argc; c++) {
                    FILE *UserBm = fopen(argv[c], "rb");
                    if (UserBm != NULL) {
                      printf("%d: %s\n", (c - 4) + 1, argv[c]);
                      if (ReadBMP((UserBm), &UserBmp[c - 4])) {
                        fclose(UserBm);
                      }
                    }
                    else {
                      fputs("Bad image file\n", stderr);
                      return EXIT_FAILURE;
                    }
                }
          }

      }
      char *Header = calloc(LOGO_HEADER_SIZE, sizeof(char));

      if (Header != NULL) {
       fread_offset(Header, sizeof(char), LOGO_HEADER_SIZE, LogoImg, Offsets.OffsetList[0]);
       rewind(LogoImg);
      }

      Output = calloc(Offsets.EndOfFile, sizeof(char));

      if (Output != NULL) {
        memset(Output, 0, Offsets.EndOfFile);
        memcpy(Output + Offsets.OffsetList[0], Header, LOGO_HEADER_SIZE);
        free(Header);
        for (uint32_t c = 1; c < Offsets.Count; c++) {
          printf("%d %d\n", (4 + c) , argc);
          if ((4 + c) < (uint32_t) argc) {
            memcpy(Output + Offsets.OffsetList[c], UserBmp[c - 1].Data, UserBmp[c - 1].HeadSize);
          }
          else {
            memcpy(Output + Offsets.OffsetList[c], LogoImgBmp[c - 1].Data, LogoImgBmp[c - 1].HeadSize);
          }
        }
      }

      fwrite(Output, sizeof(char), Offsets.EndOfFile, OutputFile);
      free(Output);
      

  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    srand((uint32_t)time(NULL));
    if ((argc > 3) && !strcmp(argv[1], "-p")) {
      return PackBMP(argc, argv);
    }

    else if ((argc > 2) && !strcmp(argv[1], "-e")) {
       return ExtractBMP(argc, argv);
  }


    else if ((argc > 1) && !strcmp(argv[1], "-h")) {
          printf("%s\n%s\n%s\n%s\n%s\n", 
          "Usage:", 
          "-e [logo.img] [BitmapN.bmp] - Extract the bitmaps from image (if file names are not specified it will use default ones)", 
          "-o [logo.img] - Output offsets", 
          "-hdr [logo.img] [Header.bin] - Get partition header (into a file)", 
          "-p [logo.img] [Output.img] [BitmapN.bmp] ... - Pack bitmaps into image");
        }

    else if ((argc > 2) && !strcmp(argv[1], "-o")) {
      return ExtractOut(argv);
    }

    else if ((argc > 2) && !strcmp(argv[1], "-hdr")) {
      return ExtractHeader(argc, argv);
    }
    

    else fprintf(stderr, "%s\n%s\n", "Invalid arguments, not enough arguments or too many arguments.", "Use flag -h to get help");

  return EXIT_SUCCESS;
}
