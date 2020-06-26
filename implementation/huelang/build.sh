#!/bin/bash

echo "<!> Building HueLang..."


echo "<-> Cleaning up..."

set -e

rm -rf bin
rm -rf lib
rm -rf obj
find src -name "*.h" -type f -delete

echo "<-> Building dependencies..."

cd ../libhuelang
./build.sh
cd ../huemodule.std
./build.sh
cd ../huelang

echo "<-> Making headers..."
for file in $(find src -name "*.[ch]")
do
  makeheaders $file
  echo "<!> Headers for" $file "made"
done

echo "<-> Adding header guards..."
for file in $(find src -name "*.[h]")
do
  fbname=$(basename "$file" .h)
  upper=${fbname^^}
  echo -e "#ifndef" "$upper" "\n""#define" "$upper" "\n" "$(<$file)" "\n""#endif" > $file
  echo "<!> Headers guards for" $file "made"
done

echo "<-> Bringin dependencies in..."

mkdir lib
cp ../libhuelang/lib/* ./lib
cp ../huemodule.std/module/* ./lib
cp ../libhuelang/headers/* src/

echo "<-> Building source files"

mkdir bin
mkdir obj

for file in $(find src -name "*.[c]")
do
  echo "<!> Building $file ..."
  if [[ $1 == "release" ]]
  then
    gcc -c -Wall -march=native -L./lib -ldl -lhuelang -lreadline -O2 -g0 $file
  else
    gcc -c -Wall -march=native -L./lib -ldl -lhuelang -lreadline -O0 -g $file
  fi
done

mv *.o obj/

echo "<-> Compiling executable"

if [[ $1 == "release" ]]
then
  gcc -march=native -O2 -L./lib -ldl -lhuelang -lreadline -g0 -o bin/huelang obj/*.o
else
  gcc -march=native -O0 -L./lib -ldl -lhuelang -lreadline -g -o bin/huelang obj/*.o
fi

echo "<-> Done!"

export LD_LIBRARY_PATH=./lib/
chmod +x bin/huelang
./bin/huelang
