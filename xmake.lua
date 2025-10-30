-- stylua: ignore start

add_rules("mode.debug", "mode.release")

-- easy-args
package("easy-args")
    set_urls("https://github.com/gouwsxander/easy-args.git")
    add_versions("0.0.1", "1b776957e13200a8d0d192dc909c46672baeb065")

    on_install(function(package)
	    os.cp("includes/*.h", package:installdir("include"))
    end)
package_end()


add_requires("libsdl3 3.2.16")
add_requires("easy-args 0.0.1")


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


function set_rules(is_shared)
	if is_mode("debug") then
		set_symbols("debug")
		set_optimize("none")
		add_defines("HE_DEBUG")
	elseif is_mode("release") then
		set_policy("build.optimization.lto", true)
		--set_strip(true)
		is_shared = false
        if not is_shared then
			set_symbols("hidden")
		end
		set_fpmodels("fast")
		set_optimize("aggressive")
	end
end

target("HateEngineRuntime")
    set_languages("c99")
    set_warnings("pedantic")
    set_kind("binary")
    add_files("src/**.c")
    add_includedirs("src", "deps")
    add_packages("libsdl3", "easy-args")
    -- set_rundir("$(projectdir)")
    set_rules(false)
--print(config.mode)
    set_runargs("--game-lib", "example-Dev")

    add_links("m")

    add_defines("HE_FS_SDL3", "HE_WS_SDL3")
    add_defines("HE_MEM_TRACK", "HE_MEM_TRACK_TRACE")

    if is_plat("mingw") then
	    add_ldflags("-mcrtdll=msvcrt-os", { force = true })
    end
--add_ldflags("-Wl,-gc-sections -ffunction-sections")

    after_build(function(target)
        p = path.join(target:targetdir(), "assets")
        if not os.exists(p) then
            os.ln(
                "$(projectdir)/examples/assets",
                path.join(target:targetdir(), "assets"),
                { force = true, symbolic = true }
            )
        end
        -- local os = import("core.base.os")
    end)

    on_load(function(target)
        -- os.exec("python instrumentor.py src build/instrumented_src")
        -- print("Hello, World!")
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

function declare_exmaple(name, ...)
	target("example-" .. name)
        set_languages("c99")
        set_warnings("pedantic")
        set_kind("shared")
        add_includedirs("include")
        set_rules(true)
        set_prefixname("")

        if is_plat("mingw") then
            add_shflags("-mcrtdll=msvcrt-os", { force = true })
        end

        add_files(...)

    -- Normal run command: "xmake run Dev"
    target(name)
        set_kind("phony")
        set_default(false)
        set_rundir("$(projectdir)/build/linux/x86_64/release")
        add_deps("HateEngineRuntime")
        on_run(function(target)
            import("core.project.project")
            local runtime = project.target("HateEngineRuntime")
            local olddir = os.cd(path.directory(runtime:targetfile()))
            os.execv(runtime:targetfile(), {"--game-lib", "example-" .. name})
            os.cd(olddir)
        end)
end




declare_exmaple("Dev", "examples/dev.c")
declare_exmaple("Signal", "examples/signal.c")
