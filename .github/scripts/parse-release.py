import sys
import json
import os

platform_info = {
	"win": None,
	"android": None,
	"mac": None,
	"ios": None,
}

in_parse_section = False
with open("CMakeLists.txt", "r") as f:
	for line in f:
		if line == "# end GEODE_GD_VERSION\n":
			in_parse_section = False
			break

		if in_parse_section:
			[platform, version] = [x.strip() for x in line.removeprefix("#").split(":")]
			if platform in platform_info:
				platform_info[platform] = version
			else:
				print(f"WARNING: unknown platform {platform} with version {version}")

		if line == "# begin GEODE_GD_VERSION\n":
			in_parse_section = True

for k, v in platform_info.items():
	if not v:
		print(f"WARNING: missing version for platform {k}")
	else:
		print(f"{k}: {v}")

env_file = os.getenv("GITHUB_OUTPUT")
with open(env_file, "a") as gh_env:
	data = json.dumps(platform_info)
	gh_env.write(f"PLATFORM_INFO<<EOF\n{data}\nEOF\n")

print("Success!")