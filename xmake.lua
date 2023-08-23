add_requires("glfw", "glm")

target("reactphysics3d")
    set_kind("static")
    --set_policy("build.optimization.lto", true)
    set_fpmodels("fast")
    set_optimize("aggressive")
    add_files(
        "lib/reactphysics3d/src/**.cpp"
    )
    set_languages("cxx11")
    add_includedirs("lib/reactphysics3d/include")

target("Old3DE")
    set_kind("static")
    set_policy("build.merge_archive", true)
    --set_policy("build.optimization.lto", true)
    set_fpmodels("fast")
    set_optimize("aggressive")
    add_files(
            "src/*.c", "src/*.cpp",
            "src/Render/*.cpp", "src/Utilities/*.cpp",
            "src/Objects/*.cpp", "src/Objects/Light/*.cpp", "src/Objects/Physics/*.cpp",
            "src/Resources/*.cpp"
    )
    --add_linkdirs(".")
    --add_links("libreactphysics3d.a", {static = true})
    add_deps("reactphysics3d")
    set_languages("cxx11")
    add_includedirs("deps", "include", "lib/reactphysics3d/include")
    add_packages("glfw", "glm")

target("Old3DEngine")
    set_kind("binary")
    --set_toolset("cxx", "clang++")
    set_strip("all")
    --set_policy("build.optimization.lto", true)
    set_fpmodels("fast")
    set_optimize("aggressive")
    add_files(
        "examples/*.cpp"
    )
    set_languages("cxx11")
    add_includedirs("deps", "include")
    --set_policy("build.merge_archive", true)
    add_deps("Old3DE")
    add_packages("glfw", "glm")