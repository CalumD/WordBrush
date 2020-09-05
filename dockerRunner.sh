#!/bin/sh

echo "COMPILING APPLICATION..."
cd compute

mkdir bin
make build

echo "TESTING APPLICATION..."
make test


echo "BOOTING FRONTEND..."
cd ../frontend
node src/hello-world.js
