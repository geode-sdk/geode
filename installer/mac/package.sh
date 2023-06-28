#!/bin/bash

pkgbuild --root "$1" --identifier com.geode-sdk.geode --scripts "installer/mac/Scripts" --install-location "/tmp/geode-install" $2
