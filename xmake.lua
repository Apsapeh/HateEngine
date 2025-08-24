add_rules("mode.debug", "mode.release")

add_requires("libsdl3 3.2.16")

if is_plat("linux") then
    set_toolset("cc", "gcc")
    set_toolset("ar", "gcc")
    set_toolset("ld", "gcc")
    set_toolset("sh", "gcc")
elseif is_plat("macosx") then
    set_toolset("cc", "clang")
    set_toolset("ar", "clang")
    set_toolset("ld", "clang")
    set_toolset("sh", "clang")
elseif is_plat("mingw") then 
    set_toolset("cc", "i686-w64-mingw32-gcc")
    set_toolset("ar", "i686-w64-mingw32-gcc")
    set_toolset("ld", "i686-w64-mingw32-gcc")
    set_toolset("sh", "i686-w64-mingw32-gcc")
end

target("HateEngineRuntime")
    set_languages("c99")
    set_warnings("pedantic")
    set_kind("binary")
    add_files("src/**.c")
    add_includedirs("src", "deps")
    add_packages("libsdl3")
    -- set_rundir("$(projectdir)")
    if is_mode("debug") then 
        set_symbols("debug")
        set_optimize("none")
    elseif is_mode("release") then
        set_policy("build.optimization.lto", true)
        --set_strip(true)
        set_symbols("hidden")
        set_fpmodels("fast")
        set_optimize("aggressive")
    end 
    add_links("m")
    
    add_defines("HE_FS_SDL3", "HE_WS_SDL3")
    add_defines("HE_MEM_TRACK", "HE_MEM_TRACK_TRACE")

    if is_plat("mingw") then
        add_ldflags("-mcrtdll=msvcrt-os", {force=true})
    end
    --add_ldflags("-Wl,-gc-sections -ffunction-sections")
    
    after_build(function (target)
        p = path.join(target:targetdir(), "assets")
        if not os.exists(p) then
            os.ln("$(projectdir)/examples/assets", path.join(target:targetdir(), "assets"), {force = true, symbolic = true})
        end
        -- local os = import("core.base.os")
        -- os.ln("$(installdir)/bin/myapp", "$(installdir)/bin/myapp-link", {force = true, symbolic = true})
    end)
    
    on_load(function (target)
        -- os.exec("python instrumentor.py src build/instrumented_src")
        print("Hello, World!")
        -- local file = io.open("$(projectdir)/src/tesoeueouteou.c", "w")
        -- if file then
        
        --     -- Write data to file with native lua interface, does not support formatting, no line breaks, does not support built-in variables
        --     file:write("hello xmake\n")
        
        --     -- Write data to file with xmake extended interface, support formatting, no line breaks, no built-in variables
        --     file:writef("hello %s\n", "xmake")
        
        --     -- Use xmake extended formatted parameters to write to one line, with line breaks, and support for built-in variables
        --     file:print("hello %s and $(buildir)", "xmake")
        
        --     -- Write a line using the xmake extended formatted arguments, no line breaks, and support for built-in variables
        --     file:printf("hello %s and $(buildir) \n", "xmake")
        
        --     -- Close the file
        --     file:close()
        -- end
    end)

target("Dev")
    set_languages("c99")
    set_warnings("pedantic")
    set_kind("shared")
    add_files("examples/dev.c")
    add_includedirs("include")

    if is_plat("mingw") then
        add_shflags("-mcrtdll=msvcrt-os", {force=true})
    end
    
    


--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

