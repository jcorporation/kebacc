#!/bin/sh
#
# SPDX-License-Identifier: GPL-3.0-or-later
# (c) 2025 Juergen Mang <mail@jcgames.de>
# https://github.com/jcorporation/kebacc

echo "Compiling Kebacc"
cmake -B build \
    -DCMAKE_INSTALL_PREFIX:PATH=/usr \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    .
make -j4 -C build
echo "Linking compilation database"
sed -e 's/\\t/ /g' -e 's/-Wformat-truncation//g' -e 's/-Wformat-overflow=2//g' -e 's/-fsanitize=bounds-strict//g' \
    -e 's/-Wno-stringop-overread//g' -e 's/-fstack-clash-protection//g' \
    build/compile_commands.json > src/compile_commands.json
