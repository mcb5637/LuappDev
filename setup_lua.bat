@echo off

:download_lua
setlocal
set link=%1
set out="./LuappDev/%2"
set tmp="./lua.tar.gz"
if exist "%out%/" (
    echo "%2 already exists"
)
else (
    curl -o "%tmp%" -L "%link%"
    mkdir "%out%"
    tar -xf "%tmp%" -C "%out%"
    del "%tmp%"
    move /Y "%out%/include/*" "%out%"
    del "%out%/include/"
)
endlocal
goto :eof

call :download_lua "https://sourceforge.net/projects/luabinaries/files/5.5.0/Windows%%20Libraries/Static/lua-5.5.0_Win64_vc17_lib.zip/download" "lua55"
call :download_lua "https://sourceforge.net/projects/luabinaries/files/5.4.8/Windows%%20Libraries/Static/lua-5.4.8_Win64_vc17_lib.zip/download" "lua54"
call :download_lua "https://sourceforge.net/projects/luabinaries/files/5.3.6/Windows%%20Libraries/Static/lua-5.3.6_Win64_vc17_lib.zip/download" "lua53"
call :download_lua "https://sourceforge.net/projects/luabinaries/files/5.2.4/Windows%%20Libraries/Static/lua-5.2.4_Win64_vc17_lib.zip/download" "lua52"
call :download_lua "https://sourceforge.net/projects/luabinaries/files/5.1.5/Windows%%20Libraries/Static/lua-5.1.5_Win64_vc17_lib.zip/download" "lua51"
call :download_lua "https://sourceforge.net/projects/luabinaries/files/5.0.3/Windows%%20Libraries/lua5_0_3_Win64_vc8_lib.zip/download" "lua50"

if exist "./LuappDev/luajit" (
    echo "luajit already exists"
)
else (
    cd "./luajit_src/src"
    msvcbuild
    cd "../.."
    mkdir "./LuappDev/luajit"
    copy "./luajit_src/src/luajit.lib" "./LuappDev/luajit"
    copy "./luajit_src/src/lua.h" "./LuappDev/luajit"
    copy "./luajit_src/src/lauxlib.h" "./LuappDev/luajit"
    copy "./luajit_src/src/lualib.h" "./LuappDev/luajit"
    copy "./luajit_src/src/luajit.h" "./LuappDev/luajit"
    copy "./luajit_src/src/luaconf.h" "./LuappDev/luajit"
)
