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
    "DarkPurple": ["#41384b", "#2f2937", "#393142", "#221e28"],
    "DarkAqua": ["#2a4559", "#1f3441", "#253d4e", "#17272f"],
}

editor_sizes = [
    "Normal",
]

editor_color_from = ["#C6F249", "#7ADE2D", "#49851B"]

editor_colors = {
    "Cyan": ["#0effff", "#00d2f6", "#007d94"],
    "Blue": ["#82d6ff", "#80a1ff", "#4d60d2"],
    "Teal": ["#3cfcbc", "#3cfcbc", "#0f7a68"],
    "Magenta": ["#ffafff", "#f67fff", "#944cca"],
    "Pink": ["#ff93fd", "#ff6fab", "#d44266"],
    "Green": ["#a2f30e", "#58d000", "#347c00"],
    "BrightGreen": ["#62ff2a", "#23dc23", "#148414"],
    "DimGreen": ["#ace83f", "#6ac227", "#3f7417"],
    "Orange": ["#ffd387", "#ffa83f", "#b35d1e"],
    "LightBlue": ["#52e8ff", "#4fb1ff", "#2f6ac7"],
    "Gray": ["#c8c7c9", "#9b9a9b", "#5c5c5d"],
    "DarkGray": ["#9a9a9a", "#717171", "#3a3a3a"],
    "Salmon": ["#ffbbb9", "#ff9260", "#e15032"],
}

for size in sizes:
    with open(f"baseCircle_{size}.svg", "r") as file:
        svg_base = file.read()
    for name, cols in colors.items():
        svg = svg_base
        out = f"../baseCircle_{size}_{name}.png"
        print(f"Generating {out}")
        for color_orig, color_to in zip(color_from, cols):
            svg = svg.replace(color_orig, color_to)
        subprocess.run(["rsvg-convert", "-o", out], input=svg.encode())

for size in editor_sizes:
    with open(f"baseEditor_{size}.svg", "r") as file:
        svg_base = file.read()
    for name, cols in editor_colors.items():
        svg = svg_base
        out = f"../baseEditor_{size}_{name}.png"
        print(f"Generating {out}")
        for color_orig, color_to in zip(editor_color_from, cols):
            svg = svg.replace(color_orig, color_to)
        subprocess.run(["rsvg-convert", "-o", out], input=svg.encode())
