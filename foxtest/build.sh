#!/bin/bash

g++ -O3 -o wombatregistry wombatregistry.cpp icons.cpp managetags.cpp aboutbox.cpp viewer.cpp -lX11 -lXext -lXft -lfontconfig -lfreetype -lXcursor -lXrender -lXrandr -lXfixes -lXi -lGLU -lGL -ldl -lpthread -ljpeg -lrt -lpng -ltiff -lz -lbz2 libFOX-1.7.a libregf.a
