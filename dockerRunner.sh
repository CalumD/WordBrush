#!/bin/sh

echo "Container ip:$(hostname -i), Backend Address to use:${BACKEND_ADDRESS}"
echo "Updating address for UI->Server communication..."
sed -i "s/localhost/$BACKEND_ADDRESS/" /wordbrush/frontend/ui/src/main.js


echo "Booting..."
cd /wordbrush/frontend/ui && npm run production &
ui=$!
cd /wordbrush/frontend/server && npm run production &
server=$!

echo "Booted."
wait $ui $server
echo "Terminated."
