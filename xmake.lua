add_repositories("apsapeh-repo https://github.com/Apsapeh/xmake-repo.git")
add_requires(
    --"glfw 3.3.9",
    "glm 1.0.0", 
    "tinygltf 2.8.13", 
    "reactphysics3d 0.9.0", 
    --"nuklear",
    "glu",
    "termcolor 5635ae00856eeddffcbf7091d13e2987abde91a2"
)

add_requires("glfw 3.4", {configs = {wayland = is_plat("linux")}})

add_rules("mode.debug", "mode.release")

option("build_examples")
option_end()

set_project("HateEngine")

target("HateEngine")
    if is_plat("windows") then
        add_defines("and=&&", "or=||", "not=!", "_WIN32_WINNT=0x0501")
    end

    set_kind("$(kind)")
    set_languages("cxx11")
    add_includedirs(
        "deps","include"
    )
    add_files(
        "lib/gl/gl.c",
        "deps/blowfish/blowfish.cpp",
        "src/**.cpp"
    )
    add_packages("glfw", "glm", "tinygltf", "reactphysics3d", "glu", "termcolor")
    add_defines("GLM_ENABLE_EXPERIMENTAL")
    
    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
        add_defines("_OLD3D_DEBUG")
        set_warnings("everything")
    elseif is_mode("release") then
        set_policy("build.merge_archive", true)
        set_symbols("hidden")
        set_fpmodels("fast")
        set_optimize("aggressive")
    end

    -- after_link(function (target)
    --     print(target:info()) -- тут дальше можно вычленить linkdirs чтоб объединить glfw и reactphysics в hateengine
    --     import("utils.archive.merge_staticlib")
    --     merge_staticlib(target, "libout.a", {"libfoo.a", "libbar.a"})
    -- end)


target("Example_1")
    if is_plat("windows") then
        add_defines("and=&&", "or=||", "not=!", "_WIN32_WINNT=0x0501")
    end

    if is_plat("linux") then
        add_ldflags("-static-libstdc++ -static-libgcc")
    end
    set_enabled(has_config("build_examples") == true)
    set_kind("binary")
    add_files(
        "examples/*.cpp"
    )
    set_languages("cxx11")
    add_includedirs("include")

    add_deps("HateEngine")
    add_packages("glfw", "glm", "reactphysics3d", "ncvm")
    add_defines("GLM_ENABLE_EXPERIMENTAL")

    if is_plat("mingw") then 
        add_cxxflags("-specs=msvcr120.spec", {force=true})        
        add_ldflags("-static-libstdc++ -static-libgcc -specs=msvcr120.spec", {force = true})
    end

    set_rundir("$(projectdir)")


    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
        add_defines("_OLD3D_DEBUG")
        set_warnings("everything")
    elseif is_mode("release") then
        --set_policy("build.merge_archive", true)
        set_symbols("hidden")
        set_strip("all")
        set_fpmodels("fast")
        set_optimize("aggressive")
    end
