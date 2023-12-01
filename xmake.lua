add_requires("glfw", "glm", "tinygltf")

add_rules("mode.debug", "mode.release")

target("reactphysics3d")
    set_kind("static")
    set_languages("cxx11")
    add_includedirs("lib/reactphysics3d/include")
    add_files("lib/reactphysics3d/src/**.cpp")

    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
    elseif is_mode("release") then
        set_symbols("hidden")
        set_fpmodels("fast")
        set_optimize("aggressive")
    end


target("Old3DE")
    set_kind("static")
    set_languages("cxx11")
    add_includedirs(
        "deps",
        "include",
        "lib/reactphysics3d/include"
    )
    add_files(
        "lib/gl/gl.c", "src/**.cpp"
    )
    add_packages("glfw", "glm", "tinygltf")
    add_deps("reactphysics3d")

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

--
target("Old3DEngine")
    set_kind("binary")
    add_files(
        "examples/*.cpp"
    )
    set_languages("cxx11")
    add_includedirs("deps", "include", "lib/reactphysics3d/include")

    add_deps("Old3DE")
    add_packages("glfw", "glm", "tinygltf")

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