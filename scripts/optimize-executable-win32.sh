#!/bin/sh
strip -s ./build/msys/bin/tsgui.exe
./third/tools/upx.exe -9 ./build/msys/bin/tsgui.exe
