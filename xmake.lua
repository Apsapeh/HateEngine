add_repositories("apsapeh-repo https://github.com/Apsapeh/xmake-repo.git")
add_requires(
    "glm 1.0.0", 
    "tinygltf 2.8.13", 
    "recastnavigation 1.6.0",
    "glu",
    "termcolor 5635ae00856eeddffcbf7091d13e2987abde91a2"
)
-- On my pc it doesn't compile for Linux i386 when Wayland enabled
add_requires("glfw 3.4", {configs = {wayland = is_plat("linux") and is_arch("x86_64")}})
add_requires("hateengine-reactphysics3d 0.10.2", {debug = is_mode("debug")})
--add_requires("reactphysics3d 7060cb0fabdcc81ec5929897d3228efe70fae9da", {debug = is_mode("debug")})


if is_arch("i386") then
    add_requires("hateengine-soloud", {configs = {cxflags = {"-DDISABLE_SSE", "-DDISABLE_SIMD"}}})
else 
    add_requires("hateengine-soloud")
end


add_rules("mode.debug", "mode.release")

option("build_examples")
    set_description("Build an examples")
    set_default(false)
option_end()

option("profiler_mode")
    set_description("Enable profiler shortcuts")
    set_default(false)
option_end()

option("__package_mode")
    set_description("Don't use it, it's 'system' option for normaly support a xmake package system")
    set_default(false)
option_end()

set_project("HateEngine")
set_version("1.0.0")


function set_mode_rules(merge)
    merge = merge or false
    if is_mode("debug") then
        set_policy("build.merge_archive", merge)
        set_symbols("debug")
        set_optimize("none")
        add_defines("__HATE_ENGINE_DEBUG")
    elseif is_mode("release") then
        set_policy("build.merge_archive", merge)
        --set_symbols("hidden")
        set_strip("all")
        set_fpmodels("fast")
        set_optimize("aggressive")
    end

    if has_config("profiler_mode") then 
        add_defines("__HATE_ENGINE_PROFILER")
    end

    --add_defines("IS_RP3D_PROFILING_ENABLED")
    if is_plat("windows") then
        add_defines("and=&&", "or=||", "not=!", "_WIN32_WINNT=0x0501")
    elseif is_plat("linux") then
        set_toolchains("clang")
    elseif is_plat("mingw") then 
        add_cxxflags("-specs=msvcr120v2.spec -Wl,-subsystem,windows", {force=true})        
        add_shflags("-specs=msvcr120v2.spec", {force = true})
        add_ldflags("-specs=msvcr120v2.spec", {force = true})
    end
end


function set_HateEngineLib_rules()
    add_packages("glfw", "glm", "tinygltf", "hateengine-reactphysics3d", "glu", "termcolor", "hateengine-soloud", "recastnavigation")
    add_defines("GLM_ENABLE_EXPERIMENTAL", {public = true})

    set_languages("cxx11")
    set_exceptions("no-cxx")
    add_includedirs(
        "deps","include"
    )
    add_files(
        "lib/gl/gl.c",
        "deps/blowfish/blowfish.cpp",
        "src/**.cpp"
    )
end


if has_config("__package_mode") then 
    target("HateEngine")
        set_kind("$(kind)")
        set_HateEngineLib_rules()
        set_mode_rules(true)
else 
    target("HateEngine-static")
        set_kind("static")
        set_HateEngineLib_rules()
        set_mode_rules(true)

    target("HateEngine")
        set_kind("shared")
        set_HateEngineLib_rules()
        set_mode_rules(true)
end




------------------------------------ Examples ------------------------------------
function set_Example_rules(custom_mode)
    custom_mode = custom_mode or "shared"
    set_mode_rules()
    set_enabled(has_config("build_examples") == true)
    set_rundir("$(projectdir)")
    set_kind("binary")
    set_languages("cxx11")
    set_exceptions("no-cxx")
    add_includedirs("include")

    if custom_mode == "shared" then
        add_deps("HateEngine")
    else
        add_deps("HateEngine-static")
    end

    add_packages("glfw", "glm", "hateengine-reactphysics3d", "hateengine-soloud", "recastnavigation", {links = {}})
end

target("Dev")
    set_Example_rules()
    add_files(
        "examples/dev.cpp"
    )


target("Cube")
    set_Example_rules("static")
    add_files(
        "examples/cube.cpp"
    )