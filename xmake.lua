add_requires("glfw", "glm")

target("reactphysics3d")
    set_kind("static")
    set_languages("cxx11")
    --set_policy("build.optimization.lto", true)
    set_policy("build.merge_archive", true)
    --set_fpmodels("fast")
    --set_optimize("aggressive")
    add_includedirs("lib/reactphysics3d/include")
    add_files(
        "lib/reactphysics3d/src/**.cpp"
    )

target("Old3DE")
    set_kind("static")
    --set_policy("build.optimization.lto", true)
    --set_fpmodels("fast")
    --set_optimize("aggressive")
    add_files(
            "src/*.c", "src/**.cpp"
    )
    add_deps("reactphysics3d")
    set_languages("cxx11")
    add_includedirs("deps", "include", "lib/reactphysics3d/include")
    add_packages("glfw", "glm")
    set_policy("build.merge_archive", true)

target("Old3DEngine")
    set_kind("binary")
    set_strip("all")
    --set_policy("build.optimization.lto", true)
    --set_fpmodels("fast")
    --set_optimize("aggressive")
    add_files(
        "examples/*.cpp"
    )
    set_languages("cxx11")
    add_includedirs("deps", "include", "lib/reactphysics3d/include")
    --set_policy("build.merge_archive", true)
    add_deps("Old3DE")
    add_packages("glfw", "glm")