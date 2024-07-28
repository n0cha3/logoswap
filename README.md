
# LogoSwap ![icon](icon.png)

A simple utility to help you extract/change logo graphics on various Xiaomi devices.

 # Usage :
    logoswap -e [logo.img] [BitmapN.bmp] ... - Extract the bitmaps from image (if file names are not specified it will use randomized ones)
    P.S bitmaps must be of Windows BMP format
    logoswap -o [logo.img] - Output offsets
    logoswap -hdr [logo.img] [Header.bin] - Extract the header (into a file)", 
    logoswap -p [logo.img] [BitmapN.bmp] ... - Pack bitmaps into image (If some bitmaps weren't provided it will reuse ones found in the provided logo.img)



