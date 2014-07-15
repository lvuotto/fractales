#!/usr/bin/env sh

seed=`./comentarios llama.pnm | grep SEED | sed -r 's/SEED = (.*?)/\1/'`
./build.sh -DSEED=$seed $@
