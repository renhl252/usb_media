
cp -f  ./11-media-by-label-auto-mount.rules  /etc/udev/rules.d/

#/etc/init.d/networking stop

/etc/init.d/udev stop

udevadm control --reload-rules

/etc/init.d/udev start

#/etc/init.d/networking start

rm -rf /tmp/test.txt

