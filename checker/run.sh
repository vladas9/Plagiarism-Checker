#!/bin/sh
gcc ./src/buff.c ./src/parser.c ./src/main.c -lz -lclang -o ./build/main
./build/main 

#{
#  pushd ./scripts/
#  . .venv/bin/activate
#
#  python plot.py
#  setsid nsxiv plot.png
#
#  deactivate
#  popd
#}
