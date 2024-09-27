#!/usr/bin/env bash

# Script for generating glad files

# Requires glad, can be installed with:
# apt install python3-glad

# Options
GL_API="gl:core=4.6"
GL_EXTS=""

GLX_API="glx=1.4"
GLX_EXTS=" \
    GLX_ARB_create_context, \
    GLX_ARB_create_context_profile, \
    GLX_EXT_swap_control"

GLAD_BIN=glad
OUT_DIR=$(dirname $(realpath "$0"))/thirdparty/glad

# Generate files for:
# GL
$GLAD_BIN --out-path=$OUT_DIR --api=$GL_API --extensions="$GL_EXTS" c

# GLX
$GLAD_BIN --out-path=$OUT_DIR --api=$GLX_API --extensions="$GLX_EXTS" c
