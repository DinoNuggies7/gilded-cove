#!/bin/sh

CC="gcc"
CCACHE="ccache"
CFLAGS="-Wall -I./raylib/src/"
LDFLAGS="-lm -lcjson -L./raylib/src/ -lraylib"
OUT="gilded-cove"

if [ "$1" == "win" ]
then
	CC="x86_64-w64-mingw32-gcc"
	LDFLAGS+=" -mwindows"
elif [ "$1" == "zip" ]
then
	zip -r $OUT.zip $OUT.exe *.dll res/
	exit
elif [ "$1" == "clean" ]
then
	rm -rf build/ $OUT $OUT.exe $OUT.zip
	cd raylib/src
	make clean
	cd ../..
	exit
fi

SRC=$(ls src | grep ".*\.c" | sed "s/\.c//")

mkdir -p build
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP -j$(nproc)
cd ../..

for file in $SRC
do
  $CCACHE $CC $CFLAGS -c src/$file.c -o build/$file.o
done

$CC build/*.o $LDFLAGS -o $OUT
