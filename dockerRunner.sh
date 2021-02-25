#!/bin/sh

echo "Updating hostname for UI->Server communication..."
sed -i "s/localhost/$(hostname -i)/" /wordbrush/frontend/ui/src/main.js
hostname -i

echo "Booting..."
cd /wordbrush/frontend/ui && npm run production &
ui=$!
cd /wordbrush/frontend/server && npm run production &
server=$!

echo "Booted."
wait $ui $server
echo "Terminated."
