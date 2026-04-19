
call :download_lua "https://sourceforge.net/projects/luabinaries/files/5.5.0/Windows%%%%20Libraries/Static/lua-5.5.0_Win64_vc17_lib.zip/download" "lua55"
call :download_lua "https://sourceforge.net/projects/luabinaries/files/5.4.8/Windows%%%%20Libraries/Static/lua-5.4.8_Win64_vc17_lib.zip/download" "lua54"
call :download_lua "https://sourceforge.net/projects/luabinaries/files/5.3.6/Windows%%%%20Libraries/Static/lua-5.3.6_Win64_vc17_lib.zip/download" "lua53"
call :download_lua "https://sourceforge.net/projects/luabinaries/files/5.2.4/Windows%%%%20Libraries/Static/lua-5.2.4_Win64_vc17_lib.zip/download" "lua52"
call :download_lua "https://sourceforge.net/projects/luabinaries/files/5.1.5/Windows%%%%20Libraries/Static/lua-5.1.5_Win64_vc17_lib.zip/download" "lua51"

if exist LuappDev\lua50 (
     echo "lua50 already exists"
) else (
    setlocal
    curl -o "lua.zip" -L "https://www.lua.org/ftp/lua-5.0.3.tar.gz"
    tar -xf "lua.zip" -C "lua50_builder\"
    ren "lua50_builder\lua-5.0.3" "lua50_builder\lua"

    cd "lua50_builder\"
    cmake -S . -B ./build "-DCMAKE_BUILD_TYPE=Debug"
    cmake --build ./build --parallel 4
    cd ..\

    mkdir "LuappDev\lua50"
    del "lua.zip"

    copy /Y "lua50_builder\lua\include\lauxlib.h" "LuappDev\lua50"
    copy /Y "lua50_builder\lua\include\lua.h" "LuappDev\lua50"
    copy /Y "lua50_builder\lua\include\lualib.h" "LuappDev\lua50"
    copy /Y "lua50_builder\build\lua50.lib" "LuappDev\lua50"
    endlocal
)

if exist LuappDev\luajit (
    echo "luajit already exists"
) else (
    cd .\luajit_src\src
    msvcbuild mixed
    cd ..\..
    mkdir LuappDev\luajit
    copy luajit_src\src\lua51.lib LuappDev\luajit\luajit.lib
    copy luajit_src\src\lua.h LuappDev\luajit
    copy luajit_src\src\lauxlib.h LuappDev\luajit
    copy luajit_src\src\lualib.h LuappDev\luajit
    copy luajit_src\src\luajit.h LuappDev\luajit
    copy luajit_src\src\luaconf.h LuappDev\luajit
)

EXIT /B %ERRORLEVEL%

:download_lua
setlocal
set link="%~1"
set out="LuappDev\%~2"
set temparch="lua.zip"
if exist "%out%\" (
    echo %~2 already exists
) else (
    curl -o %temparch% -L %link%
    mkdir %out%
    7z x %temparch% -o%out%
    del %temparch%
    move /Y %out%\include\* %out%
    del %out%\include\
)
endlocal
EXIT /B 0
