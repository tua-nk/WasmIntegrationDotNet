emcc html5_mouse.c -o html5_mouse.html -s USE_WEBGL2=1 -s WASM=1 -s FULL_ES3=1 -s NO_EXIT_RUNTIME=1 -std=gnu17 -s ALLOW_MEMORY_GROWTH=1