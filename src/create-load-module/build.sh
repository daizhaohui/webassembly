                                                                                                                                                                                                    emcc with-glue.c -O3 -s WASM = 1 -s USE_SDL = 2 -o with-glue.html
emcc without-glue.c -Os -s WASM=1 -s SIDE_MODULE=1 -s BINARYEN_ASYNC_COMPILATION=0 -o without-glue.wasm
