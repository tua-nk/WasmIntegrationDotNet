emcc canvas.c -s WASM=1 -s EXPORTED_FUNCTIONS="['_main','_getCircles']" -o canvas.js