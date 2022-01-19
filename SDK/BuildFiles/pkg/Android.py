import sys, os, shutil

out_dir, build_dir, name, apk_file, out_file = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5]

os.makedirs(out_dir, exist_ok=True)

os.chdir(os.path.dirname(apk_file))

if not os.path.isdir("GeometryDash"):
	os.system(f"apktool d '{apk_file}'")

with open(os.path.join(os.path.dirname(__file__), "GeometryJump.smali"), "r") as f:
	data = f.read();

data = data.replace("CacTemplate", name)

with open("GeometryDash/smali/com/robtopx/geometryjump/GeometryJump.smali", "w") as f:
	f.write(data)

binary_dir = "GeometryDash/lib/armeabi-v7a"

# if os.path.isfile(os.path.join(binary_dir, f"libCacao.so")):
# 	os.remove(os.path.join(binary_dir, f"libCacao.so"))
if os.path.isfile(os.path.join(binary_dir, f"lib{name}.so")):
	os.remove(os.path.join(binary_dir, f"lib{name}.so"))

shutil.copy(os.path.join(build_dir, f"lib{name}.so"), os.path.join(binary_dir, f"lib{name}.so"))
# shutil.copy(os.path.join(build_dir, f"Cacao/libCacao.so"), os.path.join(binary_dir, f"libCacao.so"))

os.system(f'apktool b GeometryDash -o "{os.path.join(out_dir, out_file)}"')
os.system(f'java -jar "{os.path.join(os.path.dirname(__file__), "uber-apk-signer.jar")}" --apks "{os.path.join(out_dir, out_file)}"')
