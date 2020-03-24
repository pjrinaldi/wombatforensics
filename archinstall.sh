#!/bin/bash

# INFO BEFORE BEGIN
# You need an arch linux installation with blackarch (for afflib or use AUR), X and a window manager/desktop environment. The test system for this script was a VM with a base install and X, XFCE. Alsa, afflib
# from AUR, firefox, some fonts, and git so i can clone the repository and launch this script. If using afflib from AUR, then you will need to install base-devel prior to running makepkg -si in AUR afflib
# directory.
# No development tools were installed prior other than base-devel for afflib from AUR

# install all the prerequisites (when given the choice of packages, I always select the default=all
sudo pacman -S base-devel qt5 qtav ffmpegthumbnailer imagemagick libewf afflib sleuthkit libtar gst-libav gst-transcoder vlc mplayer gst-plugins-bad gst-plugins-ugly gdb libheif

# optional archive formats
sudo pacman -S zip unzip p7zip

# cp the tsk*.h files into their respective directories
sudo cp ./tskextra/tsk_base_i.h /usr/include/tsk/base/
sudo cp ./tskextra/tsk_fs_i.h /usr/include/tsk/fs/
sudo cp ./tskextra/tsk_img_i.h /usr/include/tsk/img/
sudo cp ./tskextra/tsk_vs_i.h /usr/include/tsk/vs/

# once the installation is done, you should be able to run qmake "wombatforensics.pro" and then "make" to generate the binaries
qmake wombatforensics.pro
make

#assuming compilation finishes successfully, you should be able to run gdb debug/wombatforensics from the project directory
gdb debug/wombatforensics

# at the gdb prompt, type r <Enter> to run the program
(gdb) r

# have fun and report bugs please.
