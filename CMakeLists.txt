cmake_minimum_required(VERSION 4.0)
project(LuappDev)

enable_language(CXX ASM)

set(CMAKE_CXX_STANDARD 20)

find_package(doctest REQUIRED)

set(CLANG_WARNINGS
        -Wall
        -Wextra # reasonable and standard
        -Wshadow # warn the user if a variable declaration shadows one from a parent context
        -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps
        # catch hard to track down memory errors
        -Wold-style-cast # warn for c-style casts
        -Wcast-align # warn for potential performance problem casts
        -Wunused # warn on anything being unused
        -Woverloaded-virtual # warn if you overload (not override) a virtual function
        -Wpedantic # warn if non-standard C++ is used
        -Wconversion # warn on type conversions that may lose data
        -Wsign-conversion # warn on sign conversions
        -Wnull-dereference # warn if a null dereference is detected
        -Wdouble-promotion # warn if float is implicit promoted to double
        -Wformat=2 # warn on security issues around functions that format output (ie printf)
        -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
)

add_compile_options(${CLANG_WARNINGS} -Werror)

function(make_lua_version name compileopt pp_compile link)
    add_executable(${name}
            LuappDev/LuappDev.cpp
            ${pp_compile}
    )

    target_compile_options(${name} PUBLIC ${compileopt})

    add_test(NAME ${name} COMMAND ${name})

    target_include_directories(${name} PRIVATE LuappDev)

    target_link_libraries(${name} PRIVATE doctest::doctest ${link})
endfunction()

make_lua_version("LuappDev54" "-DLUA54" "LuappDev/luapp/luapp54.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/LuappDev/lua54/liblua54.a")
make_lua_version("LuappDev53" "-DLUA53" "LuappDev/luapp/luapp53.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/LuappDev/lua53/liblua53.a")
make_lua_version("LuappDev52" "-DLUA52" "LuappDev/luapp/luapp52.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/LuappDev/lua52/liblua52.a")
make_lua_version("LuappDev51" "-DLUA51" "LuappDev/luapp/luapp51.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/LuappDev/lua51/liblua51.a")
make_lua_version("LuappDev50" "-DLUA50" "LuappDev/luapp/luapp50.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/LuappDev/lua50/liblua50.a")
make_lua_version("LuappDevJIT" "-DLUAJIT" "LuappDev/luapp/luappjit.cpp;LuappDev/luapp/luapp51.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/LuappDev/luajit/libluajit.a")
