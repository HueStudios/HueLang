#!/bin/bash

echo "<!> Building huemodule.std..."

echo "<-> Cleaning up..."

set -e

rm -rf lib
rm -rf obj
rm -rf headers
rm -rf module
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

echo "<-> Copying headers to headers folder..."

mkdir headers
cp src/*.h headers/

echo "<-> Bringin dependencies in..."

mkdir lib
cp ../libhuelang/lib/* ./lib
cp ../libhuelang/headers/* src/

echo "<-> Building source files"

mkdir module
mkdir obj

for file in $(find ./ -name "*.[c]")
do
  echo "<!> Building $file ..."
  if [[ $1 == "release" ]]
  then
    gcc -c -Wall -fpic -march=native -L./lib -lhuelang -lm -O2 -g0 $file
  else
    gcc -c -Wall -fpic -march=native -L./lib -lhuelang -lm -Og -g $file
  fi
done

mv *.o obj/

echo "<-> Compiling shared object"

if [[ $1 == "release" ]]
then
  gcc -march=native -O2 -L./lib -lhuelang -lm -g0 -shared -o module/huemodule.std.so obj/*.o
else
  gcc -march=native -Og -L./lib -lhuelang -lm -g -shared -o module/huemodule.std.so obj/*.o
fi

echo "<-> Done!"
