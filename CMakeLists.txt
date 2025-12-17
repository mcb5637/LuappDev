cmake_minimum_required(VERSION 4.0)
project(LuappDev)

enable_language(CXX ASM)

set(CMAKE_CXX_STANDARD 20)

find_package(doctest REQUIRED)

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
