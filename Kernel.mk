#!/bin/bash

pack()
{
./repack-bootimg.pl gzip zImage ramdisk/ boot.img
rm ./OTA/boot.img
cp ./boot.img ./OTA/
cd ./OTA/
zip -r -9 ../Sign/update.zip ./
cd ../Sign/
java -Xmx512m -jar signapk.jar -w testkey.x509.pem testkey.pk8 update.zip ../#$(cat $(pwd)/../../$kernel/.version)-$(stat -c %y $(pwd)/../../$kernel/arch/arm/boot/zImage | cut -b1-10).zip
rm update.zip
echo "Pack boot OTA package done!"
}

PS3="Choose (1-2):"
echo "Build OTA package?"
select bool in No Yes
do
	break
done
echo "You VERSION=$(cat $(pwd)/.version)"
cp $(pwd)/arch/arm/boot/zImage bootimg/
cd bootimg/
rm ./*.img
./repack-bootimg.pl gzip zImage ramdisk/ \#$(cat $(pwd)/../.version)-$(stat -c %y $(pwd)/../arch/arm/boot/zImage | cut -b1-10).img
echo "Pack boot image done!"

if [ $bool = "Yes" ]; then
pack
else
exit 0
fi
