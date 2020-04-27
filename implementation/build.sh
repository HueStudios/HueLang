echo "<-> Cleaning up..."

rm ./bin/Debug/*
rm ./obj/Debug/*
find . -name "*.h" -type f -delete
rm HueLang.make
rm Makefile

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

rm -rf headers

mkdir headers
mkdir headers/libhuelang
cp src/libhuelang/*.h headers/libhuelang

echo "<-> Running premake..."
premake5 gmake2
echo "<-> Running make..."
if [[ $1 == "" ]]
then
  make
else
  make config=$1  
fi
echo "<-> Done!"
