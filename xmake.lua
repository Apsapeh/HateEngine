add_repositories("apsapeh-repo https://github.com/Apsapeh/xmake-repo.git")
add_requires("glfw", "glm", "tinygltf", "reactphysics3d", "nuklear")

add_rules("mode.debug", "mode.release")

option("build_examples")
option_end()

target("Old3DE")
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
    add_packages("glfw", "glm", "tinygltf", "reactphysics3d", "nuklear")
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


target("Old3DEngine")
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

    add_deps("Old3DE")
    add_packages("glfw", "glm", "reactphysics3d", "nuklear")


    if is_plat("mingw") then 
        add_ldflags("--static", {force = true})
    end


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
