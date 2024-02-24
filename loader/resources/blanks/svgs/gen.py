import subprocess

# you must run this in the same directory as the script!
# and also you must have rsvg-convert
# on ubuntu you can get this with `apt install librsvg2-bin`

sizes = [
    "Large",
    "Big",
    "BigAlt",
    "Medium",
    "MediumAlt",
    "Small",
    "SmallAlt",
    "Tiny",
]

# the green gradient colors we are replacing in the base svgs
color_from = ["#D6FF5A", "#66CC11", "#90FF44", "#449906"]

# each pair of colors represents a gradient,
# first pair is for the top part of the button,
# second pair is for the bottom.
colors = {
    "Green": ["#D6FF5A", "#66CC11", "#90FF44", "#449906"],
    "Pink": ["#FACCFC", "#FF71FF", "#FBB1FF", "#FB58FF"],
    "Cyan": ["#58FDFA", "#13D5EA", "#44F9F6", "#0B9FBE"],
    "Blue": ["#1AF1F8", "#0AB4FF", "#23DCFA", "#0077FA"],
    "Gray": ["#DEDEE0", "#979997", "#CACCCA", "#747472"],
    # "DarkPurple": ["#41384b", "#2f2937", "#393142", "#221e28"],
}

for size in sizes:
    with open(f"baseCircle_{size}.svg", "r") as file:
        svg_base = file.read()
    for name, cols in colors.items():
        svg = svg_base
        out = f"../baseCircle_{size}_{name.title()}.png"
        print(f"Generating {out}")
        for color_orig, color_to in zip(color_from, cols):
            svg = svg.replace(color_orig, color_to)
        subprocess.run(["rsvg-convert", "-o", out], input=svg.encode())
