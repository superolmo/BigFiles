#!/bin/bash

rm -rf build
cmake -S . -B build --toolchain cmake/toolchain-cross-mingw32-linux.cmake
cmake --build build

rm build/lib/32bit/libBigFiles.dll.dll.a
zip build/lib/32bit/BigFiles.zip build/lib/32bit/*

sleep 1

VERSION="0.1.4"
SHA256_LINE=`sha256sum build/lib/32bit/BigFiles.zip`
echo ${SHA256_LINE}
echo ${SHA256_LINE::64}
SHA256=${SHA256_LINE::64}

cat << EOF_RC >> build/lib/32bit/entry.json
{
			"folder-name": "BigFiles",
			"display-name": "BigFiles - Open Very Large Files",
			"version": "$VERSION",
			"id": "$SHA256",
			"repository": "https://github.com/superolmo/BigFiles/releases/download/v$VERSION.x32/BigFiles.zip",
			"description": "Reads very large files. It does this by only showing a small buffer of the file and let the user move back and forth in the page.\r\nThis is useful when you want to preview very large text files. Opening is done separately from Notepad++ and there is no option to save.\r\nhttps://github.com/superolmo/BigFiles",
			"author": "Claudio Olmi",
			"homepage": "https://github.com/superolmo/BigFiles"
},
EOF_RC
