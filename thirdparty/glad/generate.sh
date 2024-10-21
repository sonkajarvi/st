#!/usr/bin/env bash

# Script for generating GL files

# Requires glad 2, which can be installed with:
#   apt install python3-glad

OUT_PATH=$(dirname $(realpath $0))
echo "$(basename $0): Output directory: '$OUT_PATH'"

API+="gl:core=4.6"
API+=",glx=1.4"

EXT+="GLX_ARB_create_context,GLX_ARB_create_context_profile,GLX_EXT_swap_control"

glad --out-path $OUT_PATH --api $API --extensions $EXT c --loader
