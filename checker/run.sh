#!/bin/sh

if [ "$(uname)" = "Linux" ]; then
  gcc ./src/buff.c ./src/parser.c ./src/main.c -lz -lclang -o ./build/main
elif [ "$(uname)" = "Darwin" ]; then
  clang -I./include ./src/buff.c ./src/parser.c ./src/main.c -o ./build/main -lz -L/opt/homebrew/opt/llvm/lib -lclang
else
exit 1
fi


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
