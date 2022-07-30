#!/bin/sh
cd `dirname $0`
cp /Users/jakrillis/tmp/geode/loader/loader_ios/build/Geode.dylib ./bin/ios/Geode.dylib
rm -rf Geode\ Helper.app
cp -r /Users/jakrillis/Library/Developer/Xcode/DerivedData/Geode_Helper-eojylhlhpfgucjaupttaqyhqcbmp/Build/Products/Debug-iphoneos/Geode\ Helper.app .
g++ main.cpp -arch arm64 -std=c++17 -isysroot `xcrun --show-sdk-path --sdk iphoneos` -dynamiclib -o GeodeLauncher.dylib

python3 pkg.py

scp "./geodeloader-test-arm64.deb" root@three.local:/var/mobile/Documents
ssh root@three.local dpkg -i "/var/mobile/Documents/geodeloader-test-arm64.deb" "&&" killall GeometryJump #";" uicache