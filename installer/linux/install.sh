#!/usr/bin/env bash

TEMP_DIR=$(mktemp -d)

# Colors
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

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

    if ! [ -x "$(command -v curl)" ]; then
        echo -e "${RED}Error${NC}: curl is not installed." >&2
        exit 1
    fi

    if ! [ -x "$(command -v jq)" ]; then
        # if we don't have jq, check if we can use python instead
        if [ -x "$(command -v python)" ]; then
            py_cmd=python
        elif [ -x "$(command -v python3)" ]; then
            py_cmd=python3
        else
            # we have neither jq nor python, error
            echo -e "${RED}Error${NC}: neither jq nor python are installed" >&2
            exit 1
        fi
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

    for GD_IDX in "$DATA_HOME/Steam" "$HOME/Steam" "$HOME/.var/app/com.valvesoftware.Steam/data/Steam"; do
        local PATH_TEST="$GD_IDX/steamapps/common/Geometry Dash"
        verbose_log "- Testing path ${YELLOW}$PATH_TEST${NC}"

        if is_valid_gd_path "$PATH_TEST"; then
            # Found it!
            GD_PATH="$PATH_TEST"
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
}

install() {
    if [ ! -d "$GD_PATH" ]; then
        echo -e "${RED}Error:${NC} Geometry Dash path is not set." >&2
        exit 1
    fi

    if [ -z "$TAG" ]; then
        echo -e "${RED}Error:${NC} Geode tag is not set." >&2
        exit 1
    fi

    if [ ! -d "$TEMP_DIR" ]; then
        echo -e "${RED}Error:${NC} Download directory $TEMP_DIR doesn't exist." >&2
        exit 1
    fi

    echo ""
    echo "Downloading Geode $TAG..."
    if ! curl -L -o "$TEMP_DIR/geode.zip" "https://github.com/geode-sdk/geode/releases/download/$TAG/geode-$TAG-win.zip"; then
        echo -e "${RED}Error:${NC} Failed to download Geode." >&2
        exit 1
    fi

    echo "Unzipping..."

    if ! unzip -qq "$TEMP_DIR/geode.zip" -d "$TEMP_DIR/geode"; then
        echo -e "${RED}Error:${NC} Failed to unzip Geode." >&2
        exit 1
    fi

    echo "Installing..."
    mv $TEMP_DIR/geode/* "$GD_PATH"
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

# Get latest tag from the Index
VERSION_JSON="$(curl -s 'https://api.geode-sdk.org/v1/loader/versions/latest')"
if [ -x "$(command -v jq)" ]; then
    TAG="$(echo "$VERSION_JSON" | jq -r .payload.tag)"
else
    TAG="$(echo "$VERSION_JSON" | $py_cmd -c 'import json,sys;print(json.load(sys.stdin)["payload"]["tag"])' 2>/dev/null)"
fi

if [ -z "$TAG" ]; then
    echo "Failed to get latest version from the Geode index."
    exit 1
fi

echo -e "Detected latest ${YELLOW}Geode${NC} version as ${BLUE}$TAG${NC}."

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
