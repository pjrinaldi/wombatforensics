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

# install all the other prerequisites (when given the choice of packages, I always select the default=all)
sudo pacman -S qt5 qtav ffmpegthumbnailer imagemagick libewf sleuthkit libtar gst-libav gst-transcoder vlc mplayer gst-plugins-bad gst-plugins-ugly gdb

# optional archive and image formats
sudo pacman -S zip unzip p7zip libheif

# cd back into wombatforensics directory
cd ../wombatforensics-0.2
# cp the tsk*.h files into their respective directories
sudo cp ./tskextra/tsk_base_i.h /usr/include/tsk/base/
sudo cp ./tskextra/tsk_fs_i.h /usr/include/tsk/fs/
sudo cp ./tskextra/tsk_img_i.h /usr/include/tsk/img/
sudo cp ./tskextra/tsk_vs_i.h /usr/include/tsk/vs/

# once the installation is done, you should be able to run qmake "wombatforensics.pro" and then "make" to generate the binaries
qmake wombatforensics.pro
make

### wombatforensics should compile successfully.

#assuming compilation finishes successfully, you should be able to run gdb debug/wombatforensics from the project directory
# gdb debug/wombatforensics

# at the gdb prompt, type r <Enter> to run the program
# (gdb) r

# have fun and please report bugs.
