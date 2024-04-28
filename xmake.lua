add_repositories("apsapeh-repo https://github.com/Apsapeh/xmake-repo.git")
add_requires(
    "glfw 3.3.9",
    "glm 1.0.0", 
    "tinygltf 2.8.13", 
    "reactphysics3d 0.9.0", 
    "nuklear", 
    "glu",
    "ncvm 68c940cec9b119268c0ce9a8ee061d91c922dd5b"
)

add_rules("mode.debug", "mode.release")

option("build_examples")
option_end()

set_project("HateEngine")

target("HateEngine")
    if is_plat("windows") then
        add_defines("and=&&", "or=||", "not=!")
    end
    set_kind("$(kind)")
    set_languages("cxx11")
    add_includedirs(
        "deps","include"
    )
    add_files(
        "lib/gl/gl.c", "src/**.cpp"
    )
    add_packages("glfw", "glm", "tinygltf", "reactphysics3d", "nuklear", "glu")
    add_defines("GLM_ENABLE_EXPERIMENTAL")
    --add_ldflags("--static", {force = true})
    
    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
        add_defines("_OLD3D_DEBUG")
    elseif is_mode("release") then
        set_policy("build.merge_archive", true)
        set_symbols("hidden")
        set_fpmodels("fast")
        set_optimize("aggressive")
    end


target("Example_1")
    if is_plat("windows") then
        add_defines("and=&&", "or=||", "not=!")
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
    add_includedirs("deps", "include")

    add_deps("HateEngine")
    add_packages("glfw", "glm", "reactphysics3d", "nuklear", "ncvm")
    add_defines("GLM_ENABLE_EXPERIMENTAL")

    if is_plat("mingw") then 
        add_ldflags("--static", {force = true})
    end

    set_rundir("$(projectdir)")


    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
        add_defines("_OLD3D_DEBUG")
    elseif is_mode("release") then
        --set_policy("build.merge_archive", true)
        set_symbols("hidden")
        set_strip("all")
        set_fpmodels("fast")
        set_optimize("aggressive")
    end
