cmake_minimum_required(VERSION 4.0)
project(LuappDev)

enable_language(CXX ASM)

set(CMAKE_CXX_STANDARD 20)

find_package(doctest REQUIRED)

set(MSVC_WARNINGS
        /W4 # Baseline reasonable warnings
        /w14242 # 'identifier': conversion from 'type1' to 'type2', possible loss of data
        /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
        /w14263 # 'function': member function does not override any base class virtual member function
        /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not
        # be destructed correctly
        /w14287 # 'operator': unsigned/negative constant mismatch
        /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside
        # the for-loop scope
        /w14296 # 'operator': expression is always 'boolean_value'
        /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
        /w14545 # expression before comma evaluates to a function which is missing an argument list
        /w14546 # function call before comma missing argument list
        /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
        /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
        /w14555 # expression has no effect; expected expression with side- effect
        /w14619 # pragma warning: there is no warning number 'number'
        /w14640 # Enable warning on thread un-safe static member initialization
        /w14826 # Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior.
        /w14905 # wide string literal cast to 'LPSTR'
        /w14906 # string literal cast to 'LPWSTR'
        /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
        /permissive- # standards conformance mode for MSVC compiler.
)
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
set(GCC_WARNINGS
        ${CLANG_WARNINGS}
        -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
        -Wduplicated-cond # warn if if / else chain has duplicated conditions
        -Wduplicated-branches # warn if if / else branches have duplicated code
        -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
        -Wuseless-cast # warn if you perform a cast to the same type
        -Wsuggest-override # warn if an overridden member function is not marked 'override' or 'final'
        -Wno-changes-meaning
)
list(APPEND CLANG_WARNINGS -Werror)
list(APPEND GCC_WARNINGS -Werror)
list(APPEND MSVC_WARNINGS /WX)
if(MSVC)
    set(PROJECT_WARNINGS_CXX ${MSVC_WARNINGS})
elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(PROJECT_WARNINGS_CXX ${CLANG_WARNINGS})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(PROJECT_WARNINGS_CXX ${GCC_WARNINGS})
else()
    message(AUTHOR_WARNING "No compiler warnings set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
    # TODO support Intel compiler
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(LIB_EXTENSION ".lib")
    set(LIB_PREFIX "")
else()
    set(LIB_EXTENSION ".a")
    set(LIB_PREFIX "lib")
endif()

add_compile_options(${PROJECT_WARNINGS_CXX})
enable_testing()

function(make_lua_version name compileopt pp_compile link include)
    add_executable(${name}
            LuappDev/CreateDelete.cpp
            LuappDev/StackAccess.cpp
            LuappDev/BaseDatatypes.cpp
            LuappDev/Functions.cpp
            LuappDev/Tables.cpp
            LuappDev/Operators.cpp
            LuappDev/RunLua.cpp
            LuappDev/UserClass.cpp
            LuappDev/Iterate.cpp
            LuappDev/LocalAccess.cpp
            LuappDev/Hook.cpp
            LuappDev/Extension.cpp
            LuappDev/AutoCleanStack.cpp
            LuappDev/Typename.cpp
            LuappDev/Serialization.cpp
            ${pp_compile}
    )

    target_compile_definitions(${name} PUBLIC ${compileopt})

    add_test(NAME ${name} COMMAND ${name})

    target_include_directories(${name}
            PRIVATE
            LuappDev
            LuappDev/luapp/include
            ${include}
    )

    target_link_libraries(${name} PRIVATE doctest::doctest ${link})
endfunction()

make_lua_version("LuappDev55" "-DLUA55" "LuappDev/luapp/src55/luapp55.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/LuappDev/lua55/${LIB_PREFIX}lua55${LIB_EXTENSION}" "LuappDev/lua55")
make_lua_version("LuappDev54" "-DLUA54" "LuappDev/luapp/src54/luapp54.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/LuappDev/lua54/${LIB_PREFIX}lua54${LIB_EXTENSION}" "LuappDev/lua54")
make_lua_version("LuappDev53" "-DLUA53" "LuappDev/luapp/src53/luapp53.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/LuappDev/lua53/${LIB_PREFIX}lua53${LIB_EXTENSION}" "LuappDev/lua53")
make_lua_version("LuappDev52" "-DLUA52" "LuappDev/luapp/src52/luapp52.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/LuappDev/lua52/${LIB_PREFIX}lua52${LIB_EXTENSION}" "LuappDev/lua52")
make_lua_version("LuappDev51" "-DLUA51" "LuappDev/luapp/src51/luapp51.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/LuappDev/lua51/${LIB_PREFIX}lua5.1${LIB_EXTENSION}" "LuappDev/lua51")
make_lua_version("LuappDev50" "-DLUA50" "LuappDev/luapp/src50/luapp50.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/LuappDev/lua50/${LIB_PREFIX}lua50${LIB_EXTENSION}" "LuappDev/lua50")
make_lua_version("LuappDevJIT" "-DLUAJIT" "LuappDev/luapp/srcjit/luappjit.cpp;LuappDev/luapp/src51/luapp51.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/LuappDev/luajit/${LIB_PREFIX}luajit${LIB_EXTENSION}" "LuappDev/luajit")
