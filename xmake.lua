add_repositories("apsapeh-repo https://github.com/Apsapeh/xmake-repo.git")
add_requires("glfw", "glm", "tinygltf", "reactphysics3d")
add_requireconfs("reactphysics3d", {configs={fast_math=true, opt_level = "a"}})

add_rules("mode.debug", "mode.release")

target("Old3DE")
    set_kind("static")
    set_languages("cxx11")
    add_includedirs(
        "deps","include"
    )
    add_files(
        "lib/gl/gl.c", "src/**.cpp"
    )
    add_packages("glfw", "glm", "tinygltf", "reactphysics3d")
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
    set_kind("binary")
    add_files(
        "examples/*.cpp"
    )
    set_languages("cxx11")
    add_includedirs("deps", "include")

    add_deps("Old3DE")
    add_packages("glfw", "glm", "reactphysics3d")


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