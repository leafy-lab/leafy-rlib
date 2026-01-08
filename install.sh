#!/bin/sh

set -e

INSTALL_DIR="/usr/local/include"
HEADER="include/leafy-raylib.h"

echo "Installing leafy-raylib..."

if [ ! -f "$HEADER" ]; then
  echo "Error: $HEADER not found"
  exit 1
fi

sudo cp "$HEADER" "$INSTALL_DIR/leafy-raylib.h"

echo "Installed to $INSTALL_DIR/leafy-raylib.h"
echo "You can now use: #include <leafy-raylib.h>"
