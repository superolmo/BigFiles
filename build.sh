#!/bin/bash

rm -rf build
cmake -S . -B build
cmake --build build
rm build/lib/64bit/libBigFiles.dll.dll.a
zip build/lib/64bit/BigFiles.zip build/lib/64bit/*

VERSION="0.1.4"
SHA256_LINE=`sha256sum build/lib/64bit/BigFiles.zip`
SHA256=${SHA256_LINE::64}

cat << EOF_RC >> build/lib/64bit/entry.json
{
			"folder-name": "BigFiles",
			"display-name": "BigFiles - Open Very Large Files",
			"version": "$VERSION",
			"id": "$SHA256",
			"repository": "https://github.com/superolmo/BigFiles/releases/download/v$VERSION.x64/BigFiles.zip",
			"description": "Reads very large files. It does this by only showing a small buffer of the file and let the user move back and forth in the page.\r\nThis is useful when you want to preview very large text files. Opening is done separately from Notepad++ and there is no option to save.\r\nhttps://github.com/superolmo/BigFiles",
			"author": "Claudio Olmi",
			"homepage": "https://github.com/superolmo/BigFiles"
},
EOF_RC
