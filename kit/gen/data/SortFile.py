import re

for file in ("GeometryDash.mm", "Cocos2d.mm"):

    with open(file, "r") as f:
        data = f.read()

    with open("Backup/"+file, "w") as f:
        f.write(data)

    parsed = sorted(re.findall(r"class .+{[^ı]*?\n}\n", data))

    if len(parsed) != len(re.findall(r"class ", data)):
        print(file)
        raise Exception("Couldn't parse all classes.")

    with open(file, "w") as f:
        f.write('\n'.join(parsed))
