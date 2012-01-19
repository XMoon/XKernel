#!/bin/bash
PS3="Choose (1-4):"
echo "Choose from the list below."
select name in CyanogenMod6 CyanogenMod7 CyanogenMod9 StockEclair
do
	break
done
echo "You chose $name."
cp arch/arm/boot/zImage bootimg/
cd bootimg/
rm ./#*.img
./repack-bootimg.pl zImage $name/ \#$(cat $(pwd)/../.version)-$name-$(stat -c %y $(pwd)/../arch/arm/boot/zImage | cut -b1-10).img
echo "Pack boot image done!"
