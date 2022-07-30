import sys, os
from shutil import copyfile, rmtree
from distutils.dir_util import copy_tree
print(sys.argv)

out_dir = os.getcwd()

os.makedirs(out_dir, exist_ok=True)
os.chdir(out_dir)
os.makedirs("tmp/Library/MobileSubstrate/DynamicLibraries/", exist_ok=True)
os.makedirs("tmp/Applications/", exist_ok=True)
os.makedirs("tmp/DEBIAN/", exist_ok=True)

open("tmp/DEBIAN/control", "w").write("""Name: Geode Launcher
Architecture: iphoneos-arm
Depends: com.cokepokes.libnotifications (>= 0.2-2)
Description: Modding suite for Geometry Dash (test package!!)
Maintainer: camila314
Package: com.camila314.geode-test
Priority: optional
Section: Tweaks
Version: 0.1.0
""")

copy_tree("Geode Helper.app/", "tmp/Applications/Geode Helper.app/")
copyfile("/Users/jakrillis/tmp/geode/loader/loader_ios/build/Geode.dylib", "bin/ios/Geode.dylib")
os.system("ldid -S GeodeLauncher.dylib")
os.system("ldid -S bin/ios/Geode.dylib")
copyfile("GeodeLauncher.dylib", "tmp/Library/MobileSubstrate/DynamicLibraries/GeodeLauncher.dylib")
copyfile("bin/ios/Geode.dylib", "tmp/Library/MobileSubstrate/DynamicLibraries/Geode.dylib")

open("tmp/Library/MobileSubstrate/DynamicLibraries/GeodeLauncher.plist", "w").write("""{ Filter = { Bundles = ( "com.robtop.geometryjump" ); }; }""")
os.system("dpkg-deb --build tmp geodeloader-test-arm64.deb")
rmtree("tmp")
