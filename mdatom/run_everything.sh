#!/usr/bin/env bash

#this script runs everything
rm *.xyz *.final output.txtout
cd source_code/build
cmake ..
make
cd ../..
cp source_code/build/mdatom mdatom
for j in 1.57
do
  ./mdatom input/input.inp input/start.xyz $j > output.txtout
  mv output.txtout output$j.txtout
  #rm movement.xyz *.xyz *.final coords.traj
done
rm mdatom
echo "finish"
