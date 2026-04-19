cmake_minimum_required(VERSION 3.0)
project(lua50_builder)

enable_language(C)

add_library(lua50
        lua-5.0.3/src/lapi.c
        lua-5.0.3/src/lcode.c
        lua-5.0.3/src/ldebug.c
        lua-5.0.3/src/ldo.c
        lua-5.0.3/src/ldump.c
        lua-5.0.3/src/lfunc.c
        lua-5.0.3/src/lgc.c
        lua-5.0.3/src/llex.c
        lua-5.0.3/src/lmem.c
        lua-5.0.3/src/lobject.c
        lua-5.0.3/src/lopcodes.c
        lua-5.0.3/src/lparser.c
        lua-5.0.3/src/lstate.c
        lua-5.0.3/src/lstring.c
        lua-5.0.3/src/ltable.c
        lua-5.0.3/src/ltests.c
        lua-5.0.3/src/ltm.c
        lua-5.0.3/src/lundump.c
        lua-5.0.3/src/lvm.c
        lua-5.0.3/src/lzio.c
        lua-5.0.3/src/lib/lauxlib.c
        lua-5.0.3/src/lib/lbaselib.c
        lua-5.0.3/src/lib/ldblib.c
        lua-5.0.3/src/lib/liolib.c
        lua-5.0.3/src/lib/lmathlib.c
        lua-5.0.3/src/lib/loadlib.c
        lua-5.0.3/src/lib/lstrlib.c
        lua-5.0.3/src/lib/ltablib.c
)

target_include_directories(lua50
        PUBLIC
        lua-5.0.3/src
        lua-5.0.3/include
)
