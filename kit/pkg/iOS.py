import sys, os
from shutil import copyfile, rmtree
print(sys.argv)
out_dir, dylib, name, control_file, out_file = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5]

os.makedirs(out_dir, exist_ok=True)
os.chdir(out_dir)
os.makedirs("tmp/Library/MobileSubstrate/DynamicLibraries/", exist_ok=True)
os.makedirs("tmp/DEBIAN/", exist_ok=True)
copyfile(control_file, "tmp/DEBIAN/control")

os.system("ldid -S {}".format(dylib))
copyfile(dylib, "tmp/Library/MobileSubstrate/DynamicLibraries/{}.dylib".format(name))

open("tmp/Library/MobileSubstrate/DynamicLibraries/{}.plist".format(name), "w").write("""{ Filter = { Bundles = ( "com.robtop.geometryjump" ); }; }""")
os.system("dpkg-deb --build tmp {}".format(out_file))
rmtree("tmp")
