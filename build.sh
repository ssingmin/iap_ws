rm ./hexfile/writetest.txt
make clean
make
#echo (cat < /dev/ttyUSB0) > tmp.txt

tmp.txt < /dev/ttyUSB0


./iap
#ttttest.txt < /dev/ttyUSB0
#hexdump -e '41/1 "%02X " "\n"' ttttest.txt
