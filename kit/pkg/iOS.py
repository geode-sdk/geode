import sys, os
from shutil import copyfile, rmtree
print(sys.argv)
out_dir, dylib = sys.argv[2], sys.argv[1]

os.makedirs(out_dir, exist_ok=True)
os.chdir(out_dir)
os.makedirs("tmp/Library/MobileSubstrate/DynamicLibraries/", exist_ok=True)
os.makedirs("tmp/DEBIAN/", exist_ok=True)

open("tmp/DEBIAN/control", "w").write("""Name: Geode Mod Loader
Architecture: iphoneos-arm
Description: Modding suite for Geometry Dash (test package!!)
Maintainer: camila314
Package: com.camila314.geode-test
Priority: optional
Section: Tweaks
Version: 0.1.0
""")

os.system("ldid -S {}".format(dylib))
copyfile(dylib, "tmp/Library/MobileSubstrate/DynamicLibraries/Geode.dylib")

open("tmp/Library/MobileSubstrate/DynamicLibraries/Geode.plist", "w").write("""{ Filter = { Bundles = ( "com.robtop.geometryjump" ); }; }""")
os.system("dpkg-deb --build tmp geodeloader-test-arm64.deb")
rmtree("tmp")
