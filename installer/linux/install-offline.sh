#!/usr/bin/env bash

set -euo pipefail

# Colors
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

SELF="${BASH_SOURCE[0]:-}"

if [ -z "$SELF" ] || [ ! -f "$SELF" ]; then
    echo -e "${RED}Error${NC}: script is being piped to bash" >&2
    echo "This breaks self-extraction, download the script first and then run it" >&2
    exit 1
fi

TEMP_DIR=$(mktemp -d)
VERBOSE=""

# This only has verbose for now
while getopts v OPTS; do
    case ${OPTS} in
        v) VERBOSE=1 ;;
    esac
done

verbose_log() {
    if [ ! -z "$1" ] && [ ! -z "$VERBOSE" ]; then
        echo -e "[VERBOSE]: $1"
    fi
}

check_dependencies() {
    if ! [ -x "$(command -v unzip)" ]; then
        echo -e "${RED}Error${NC}: unzip is not installed." >&2
        exit 1
    fi
}

is_valid_gd_path() {
    if [ -z "$1" ]; then
        LAST_VALID_GD_PATH_ERR="No path specified."
        return 1
    fi

    if [ ! -d "$1" ]; then
        LAST_VALID_GD_PATH_ERR="Path is not a directory."
        return 1
    fi

    # Check if cocos2d is there, for good measure
    if [ ! -f "$1/libcocos2d.dll" ]; then
        LAST_VALID_GD_PATH_ERR="Path doesn't contain Geometry Dash."
        return 1
    fi

    return 0
}

# Echoes the GD path if found
find_gd_installation() {
    verbose_log "Searching for Geometry Dash..."
    local DATA_HOME="${XDG_DATA_HOME:-$HOME/.local/share}"

    for GD_IDX in "$DATA_HOME/Steam" "$HOME/Steam" "$HOME/.steam/debian-installation" "$HOME/.var/app/com.valvesoftware.Steam/data/Steam" "$HOME/snap/steam/common/.steam/steam" "/steam"; do
        local PATH_TEST="$GD_IDX/steamapps/common/Geometry Dash"
        verbose_log "- Testing path ${YELLOW}$PATH_TEST${NC}"

        if is_valid_gd_path "$PATH_TEST"; then
            # Found it!
            GD_PATH="$PATH_TEST"

            case "$PATH_TEST" in
              */snap/steam/common/*)
                echo "${YELLOW}Detected Steam installed with Snap. This is not a supported platform, and you may run into issues.${NC}"
                echo "${YELLOW}To ensure you have the best experience, install Steam using your distribution's package manager, or through Flatpak.${NC}"
                ;;
            esac

            verbose_log "* Found at ${YELLOW}$GD_PATH${NC}"
            return 0
        fi
    done

    # Check some other random paths, maybe we find something
    if is_valid_gd_path "$HOME/Games/Geometry Dash"; then
        GD_PATH="$HOME/Games/Geometry Dash"
        return 0
    fi

    return 1
}

# Defaults to yes
confirm() {
    while read -n1 -r -p "$(echo -e $1) [Y/n]: " < /dev/tty; do
        case $REPLY in
            y) return 0 ;;
            n) echo ""; return 1 ;;
            *) return 0;;
        esac
    done
}

ask_gd_path() {
    local POTENTIAL_PATH=""
    while read -p "Enter the path where GeometryDash.exe is located: " POTENTIAL_PATH < /dev/tty; do
        local POTENTIAL_PATH=${POTENTIAL_PATH%"/GeometryDash.exe"}

        if is_valid_gd_path "$POTENTIAL_PATH"; then
            if confirm "Do you want to install ${YELLOW}Geode${NC} to ${YELLOW}$POTENTIAL_PATH${NC}?"; then
                GD_PATH="$POTENTIAL_PATH"
                break
            fi
        else
            echo -e "* Invalid path: ${RED}$LAST_VALID_GD_PATH_ERR${NC}"
        fi
    done
    # incase of ctrl+d
    if [ -z "$POTENTIAL_PATH" ]; then
        echo ""
        exit 1
    fi
}

install() {
    if [ ! -d "$GD_PATH" ]; then
        echo -e "${RED}Error:${NC} Geometry Dash path is not set." >&2
        exit 1
    fi

    if [ ! -d "$TEMP_DIR" ]; then
        echo -e "${RED}Error:${NC} Download directory $TEMP_DIR doesn't exist." >&2
        exit 1
    fi

    echo -e "\nUnzipping..."

    # self extraction
    if ! unzip -q "$SELF" -d "$TEMP_DIR/geode"; then
        echo -e "${RED}Error:${NC} Failed to unzip Geode." >&2
        exit 1
    fi

    echo "Installing..."
    mv $TEMP_DIR/geode/* "$GD_PATH"

    if [ -f "$GD_PATH/XINPUT1_4.dll" ]; then
        echo "Deleting conflicting XINPUT1_4.dll..."
        rm "$GD_PATH/XINPUT1_4.dll"
    fi

    # clean up temp dir
    rm -r "$TEMP_DIR"
}

check_dependencies
verbose_log "${YELLOW}Running in verbose mode.${NC}"
verbose_log "Using $TEMP_DIR as temporary directory for installation."
cat << "EOF"
      ...@@@@@@@@@@@@...      
    ..@@@@@@@@..@@@@@@@@..    
   ..@@@@@@...@@@@@@@@@@@..   
 ..@@@@@@...@@.@@@@@@@@@@@@.. 
 .@@@@...@@@......@@@@@@@@@@. 
..@@@..@@@..@@@@@@..@@@..@@@..
..@@@..@@.@@@@@@@@@..@@..@@@..
..@@@@..@@@@@@@@@@@.@@..@@@@..
..@@@@..@@@@@@@@@@..@@.@@@@@..
 .@@@@@..@@........@@..@@@@@. 
 ..@@@@@.@@@@@@@@@@@..@@@@@.. 
   ..@@@..............@@@..   
    ..@@@@@@@@@@@@@@@@@@..    
      ...@@@@@@@@@@@@...      
EOF
echo "Installing Geode..."

if ! find_gd_installation; then
    echo -e "Didn't find ${YELLOW}Geometry Dash${NC} in any of the commonly used paths."

    ask_gd_path
else
    echo -e "Found Geometry Dash at ${YELLOW}$GD_PATH${NC}."

    if ! confirm "Do you want to install ${YELLOW}Geode${NC} to ${YELLOW}$GD_PATH${NC}?"; then
        ask_gd_path
    fi
fi

install

echo -e "Geode has been installed successfully at ${YELLOW}$GD_PATH${NC}!"
echo -e "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓"
echo -e "┃ Make sure to set \"${YELLOW}WINEDLLOVERRIDES=\"xinput1_4=n,b\" %command%${NC}\" as your ${YELLOW}launch options${NC} for Geometry Dash inside ${BLUE}Steam${NC}. ┃"
echo -e "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛"
echo "Have fun modding!"
echo ""

exit 0
# binary data below
