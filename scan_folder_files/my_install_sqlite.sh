
BUILD_PATH=`pwd`

echo "==Unpackage sqlite ========================================================"
tar -zxvf sqlite-autoconf-3180000.tar.gz
sleep 1

#Conifguration 
echo "==Conifguration sqlite========================================================"
cd $BUILD_PATH/sqlite-autoconf-3180000
./configure --prefix=`pwd`/../scan_folder_files/build/
sleep 1


echo "==Compile sqlite========================================================"
make

sleep 1
echo "==Install sqlite========================================================"
make install

sleep 1
ls $BUILD_PATH/../scan_folder_files/build/

sleep 1
echo "==Clean sqlite ========================================================"
cd $BUILD_PATH
rm -rf build sqlite-autoconf-3180000
ls



