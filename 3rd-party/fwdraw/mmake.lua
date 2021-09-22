------ Not Implemented ------------
function hasparam(name) return false end


------ Variables ------------------
local DEFINES = {}
if hasparam("debug") then
    table.insert(Defines, "DEBUG")
end

------------------------------------
project "lib"
    lang "c++"
    kind "static library"
    defines (DEFINES)
    includes { "include" }
    sources { "src/**.cpp", "src/gl3w.c" }
    -- options { "-arch x86_64" }
    compiler "g++-10"
    standard "c++17"
    build_dir "build/"
    bin_dir "lib/"
    bin "libfwdraw.a"
------------------------------------
project "deps"
    lang "c & c++"
    kind "nolink"
    includes { "include" }
    sources { "src/gl3w.c" }
    compiler "gcc-10"
    build_dir "build/"
------------------------------------

local FRAMEWORKS = { "IOKit", "Cocoa", "OpenGL" }
for i, v in ipairs(FRAMEWORKS) do FRAMEWORKS[i] = "-framework " .. v end

project "example"
    lang "c++"
    kind "executable"
    libraries { "glfw3" }
    libdirs { "lib" }
    includes { "include" }
    -- options {  } TODO: linker options
    sources { "example/**.cpp" }
    objects { "lib/libfwdraw.a", table.unpack(FRAMEWORKS) }
    standard "c++17"
    compiler "g++-10"
    build_dir "example/build/"
    bin_dir "."
    bin "exmpl"
------------------------------------
