rem emcc html5_core.c -o html5_core.html -s USE_WEBGL2=1 -s WASM=1 -s FULL_ES3=1 -s NO_EXIT_RUNTIME=1 -std=c17 -s ALLOW_MEMORY_GROWTH=1

emcc html5_core.cpp -o html5_core.html -s USE_WEBGL2=1 -s WASM=1 -s FULL_ES3=1 -s NO_EXIT_RUNTIME=1 -std=c++17 -s ALLOW_MEMORY_GROWTH=1 -s FORCE_FILESYSTEM