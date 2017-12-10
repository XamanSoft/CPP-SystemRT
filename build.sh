#!/bin/bash
gyp build.gyp -DOS=linux --depth=. -f make --generator-output=./build/makefiles
make -C ./build/makefiles/
