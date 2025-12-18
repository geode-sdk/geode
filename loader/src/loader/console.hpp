#pragma once

#include <string>
#include <Geode/loader/Types.hpp>

namespace geode::console {
    // intended for setting up an already attached console
    // for example, if the game was launched with a debugger, it'd already have a console attached
    // so we can setup that console regardless of the setting
    void setup();
    // if the setting is on, we call tryOpenIfClosed, and if there's no console attached yet
    // (e.g. from a debugger, see above), this function should create a new console
    // and attach it (perhaps, by calling setup again, see windows impl for an example)
    void openIfClosed();

    void log(ZStringView msg, Severity severity);
    void messageBox(ZStringView title, ZStringView info, Severity severity = Severity::Error);
}
