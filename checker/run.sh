#!/bin/sh
gcc ./src/main.c -lz -o ./build/main &&
./build/main > ./scripts/distances.csv &&

{
  pushd ./scripts/
  . .venv/bin/activate
  
  python plot.py
  setsid nsxiv plot.png
  
  deactivate
  popd
}
