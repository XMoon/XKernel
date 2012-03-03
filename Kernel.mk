#!/bin/bash
PS3="Choose (1-4):"
echo "Choose your ramdisk from the list below."
select name in CyanogenMod7 CyanogenMod9  MIUI
do
	break
done
echo "You chose $name."

PS3="Choose (1-3):"
echo "Choose your ramdisk compression mode from the list below."
select com in GZIP LZMA XZ
do
	break
done

com=`tr '[A-Z]' '[a-z]' <<<"$com"`

pack()
{
./repack-bootimg.pl $com zImage $name/ boot.img
rm ./OTA/boot.img
cp ./boot.img ./OTA/
cd ./OTA/
zip -r -9 ../Sign/update.zip ./
cd ../Sign/
java -Xmx512m -jar signapk.jar -w testkey.x509.pem testkey.pk8 update.zip ../#$(cat $(pwd)/../../$kernel/.version)-$name-$(stat -c %y $(pwd)/../../$kernel/arch/arm/boot/zImage | cut -b1-10).zip
rm update.zip
echo "Pack boot OTA package done!"
}

PS3="Choose (1-2):"
echo "Build OTA package?"
select bool in No Yes
do
	break
done
echo "You chose RAMDISK=$name,VERSION=$(cat $(pwd)/.version)"
cp $(pwd)/arch/arm/boot/zImage bootimg/
cd bootimg/
rm ./*.img
./repack-bootimg.pl $com zImage $name/ \#$(cat $(pwd)/../.version)-$name-$(stat -c %y $(pwd)/../arch/arm/boot/zImage | cut -b1-10).img
echo "Pack boot image done!"

if [ $bool = "Yes" ]; then
pack
else
exit 0
fi
