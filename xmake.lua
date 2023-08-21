add_requires("glfw", "glm")

target("Old3DE")
    set_kind("static")
    set_optimize("aggressive")
    add_files(
            "src/*.c", "src/*.cpp",
            "src/Render/*.cpp", "src/Utilities/*.cpp",
            "src/Objects/*.cpp", "src/Objects/Light/*.cpp", "src/Objects/Physics/*.cpp",
            "src/Resources/*.cpp"
    )
    set_languages("cxx11")
    add_includedirs("deps", "include")
    add_packages("glfw", "glm")

target("Old3DEngine")
    set_kind("binary")
    --set_toolset("cxx", "clang++")
    set_optimize("aggressive")
    add_files(
        "examples/*.cpp"
    )
    set_languages("cxx11")
    add_includedirs("deps", "include")
    add_deps("Old3DE")
    add_packages("glfw", "glm")