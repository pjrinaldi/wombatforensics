#!/bin/bash

### INFO BEFORE BEGIN
# You need an arch linux installation with blackarch (for afflib or use AUR), X and a window manager/desktop environment.
# The test system for this script was a VM with a base install and X, XFCE. Alsa, firefox, some fonts, and git;
# so i can clone the repository and launch this script.
# This script will install afflib from the AUR for you.
# No development tools were installed prior other than git to be able to clone this repository

### installing afflib
# install base-devel package set
sudo pacman -S base-devel
# move up a directory from wombatforensics clone
cd ../
# download AUR snapshot
curl -O https://aur.archlinux.org/cgit/aur.git/snapshot/afflib.tar.gz
# extract the afflib PKGBUILD
tar -xzf afflib.tar.gz
# cd into the afflib directory
cd afflib
# run makepkg -si as regular user, install required packages and enter sudo if prompted
makepkg -si

# cd back into the wombatforensics directory
cd ../wombatforensics

# install all the other prerequisites (when given the choice of packages, I always select the default=all)
sudo pacman -S qt5 qtav ffmpegthumbnailer imagemagick libewf libtar gst-libav gst-transcoder vlc mplayer gst-plugins-bad gst-plugins-ugly gdb libzip poppler-qt5 openssl squashfuse

# optional archive and image formats
sudo pacman -S zip unzip p7zip libheif

# move up a directory from wombatforensics clone
cd ../
# download AUR snapshot
curl -O https://aur.archlinux.org/cgit/aur.git/snapshot/liblnk-git.tar.gz
# extract the liblnk-git PKGBUILD
tar -xzf liblnk-git.tar.gz
# cd into the liblnk-git directory
cd liblnk-git
# run makepkg -si as regular user, install required packages and enter sudo if prompted
makepkg -si
# cd back into wombatforensics directory
cd ../wombatforensics

# move up a directory from wombatforensics clone
cd ../
# download libfwnt source
wget https://github.com/libyal/libfwnt/releases/download/20200723/libfwnt-alpha-20200723.tar.gz
# extract the libfwnt archive
tar -xzf libfwnt-alpha-20200723.tar.gz
# cd into the libfwnt directory
cd libfwnt-20200723
# run configure make make install
./configure && make && sudo make install
# cd back into wombat forensics directory
cd ../wombatforensics

# move up a directory from wombatforensics clone
cd ../
# download libregf source
wget https://github.com/libyal/libregf/releases/download/20200805/libregf-alpha-20200805.tar.gz
# extract the libregf archive
tar -xzf libregf-alpha-20200805.tar.gz
# cd into the libregf directory
cd libregf-20200805
# run configure make make install
./configure && make && sudo make install
# cd back into wombat forensics directory
cd ../wombatforensics

# cp the tsk*.h files into their respective directories
#sudo cp ./tskextra/tsk_base_i.h /usr/include/tsk/base/
#sudo cp ./tskextra/tsk_fs_i.h /usr/include/tsk/fs/
#sudo cp ./tskextra/tsk_img_i.h /usr/include/tsk/img/
#sudo cp ./tskextra/tsk_vs_i.h /usr/include/tsk/vs/

# need to add /usr/local/lib to ldconfig library path
echo '/usr/local/lib' > local.conf && sudo mv ./local.conf /etc/ld.so.conf.d/
# update ldconfig cache
sudo ldconfig

# once the installation is done, you should be able to run qmake "wombatforensics.pro" and then "make" to generate the binaries
qmake wombatforensics.pro
make

### wombatforensics should compile successfully.

#assuming compilation finishes successfully, you should be able to run gdb debug/wombatforensics from the project directory
# gdb debug/wombatforensics

# at the gdb prompt, type r <Enter> to run the program
# (gdb) r

# have fun and please report bugs.
