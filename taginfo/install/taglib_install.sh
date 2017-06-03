
BUILD_PATH=`pwd`

echo "==Unpackage taglib ========================================================"
tar -zxvf taglib-1.11.tar.gz
sleep 1

#Conifguration 
echo "==Conifguration taglib========================================================"
mkdir build
mkdir $BUILD_PATH/../taglib
cd build
cmake -DCMAKE_INSTALL_PREFIX=$BUILD_PATH/../taglib -DCMAKE_BUILD_TYPE=Release $BUILD_PATH/taglib-1.11

echo "==Compile sqlite========================================================"
make

echo "==Install sqlite========================================================"
make install

cd $BUILD_PATH
#rm -rf taglib-1.11 build


