DEFAULT_NOTEPAD_DIR=~/Downloads/npp.8.6.5.portable
BUILD_DIR=build/lib/32bit

# Generate 10 MB text file
tr -dc "A-Za-z 0-9" </dev/urandom | fold -w100 | head -n 100000 >bigfile.txt

# Delete current Notepad BigFiles plugin folder
rm -rf ${DEFAULT_NOTEPAD_DIR}/plugins/BigFiles

# Recreate folder and copy BigFiles plugin to local Notepad++ plugin folder
mkdir ${DEFAULT_NOTEPAD_DIR}/plugins/BigFiles/ && cp ${BUILD_DIR}/BigFiles.dll ${DEFAULT_NOTEPAD_DIR}/plugins/BigFiles/

wine ${DEFAULT_NOTEPAD_DIR}/notepad++.exe
