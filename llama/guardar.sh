#!/usr/bin/env sh

seed=`./comentarios llama.pnm | grep SEED | sed -r 's/SEED = (.*?)/\1/'`
funcs=`./comentarios llama.pnm | grep FUNCS | sed -r 's/FUNCS = (.*?)/\1/'`
./build.sh -DSEED=$seed -DFUNCS=$funcs -DPOINTS=1e9
./llama $1 $2
n=0
while [ -f img/llama-$seed-$n.png ]; do
  n=$((n+1))
done
pnmscale -w $3 -h $4 llama.pnm | pnmtopng > img/llama-$seed-$n.png
