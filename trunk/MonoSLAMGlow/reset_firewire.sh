
#! /bin/bash 
 
# Make right devices for Fedora 4 (so they agree with old RH9 ones)
mkdir -p /dev/video1394
ln -s /dev/video1394-0 /dev/video1394/0
ln -s /dev/raw/raw1394 /dev/raw1394

# 
# Make sure that every man and his dog can grab images. 
# 
chmod -R a+rwx /dev/video1394 
chmod -R a+rwx /dev/raw1394 


 
# 
# Attempt to unload the four firewire related kernel modules. 
# Don't worry if ieee1394 won't unload, as it is sometimes built into 
# the kernel. 
# 
echo Unloading modules 
/sbin/rmmod video1394 
/sbin/rmmod ohci1394 
/sbin/rmmod raw1394 
/sbin/rmmod ieee1394 
 
 
 
# 
# Attempt to reload the four firewire related kernel modules. 
# The argument to ohci1394 tries to force the host adapter to be 
# the bus controller. Without this, the kernel sometimes asks one of 
# the cameras to be bus master, which isn't really very sensible. 
# 
echo Reloading modules 
/sbin/modprobe ieee1394 
/sbin/modprobe raw1394 
/sbin/modprobe ohci1394 attempt_root=1 
/sbin/modprobe video1394 
 
 
 
