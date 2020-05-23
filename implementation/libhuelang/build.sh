#!/bin/bash

echo "<!> Building libhuelang..."

echo "<-> Cleaning up..."

set -e

rm -rf lib
rm -rf obj
rm -rf headers
find . -name "*.h" -type f -delete

echo "<-> Making headers..."
for file in $(find ./ -name "*.[ch]")
do
  makeheaders $file
  echo "<!> Headers for" $file "made"
done

echo "<-> Adding header guards..."
for file in $(find ./ -name "*.[h]")
do
  fbname=$(basename "$file" .h)
  upper=${fbname^^}
  echo -e "#ifndef" "$upper" "\n""#define" "$upper" "\n" "$(<$file)" "\n""#endif" > $file
  echo "<!> Headers guards for" $file "made"
done

mkdir lib
mkdir obj
mkdir headers

echo "<-> Copying headers to headers folder..."

cp src/*.h headers/

echo "<-> Building source files"

for file in $(find ./ -name "*.[c]")
do
  echo "<!> Building $file ..."
  if [[ $1 == "release" ]]
  then
    gcc -c -Wall -fpic -march=native -O2 -g0 $file
  else
    gcc -c -Wall -fpic -march=native -Og -g $file
  fi
done

mv *.o obj/

echo "<-> Compiling shared object"

if [[ $1 == "release" ]]
then
  gcc -march=native -O2 -g0 -shared -o lib/libhuelang.so obj/*.o
else
  gcc -march=native -Og -g -shared -o lib/libhuelang.so obj/*.o
fi

echo "<-> Done!"