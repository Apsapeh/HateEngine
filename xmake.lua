add_requires("glfw", "glm")

target("Old3DEngine")
    set_kind("binary")
    --set_toolset("cxx", "clang++")
    set_optimize("aggressive")
    add_files(
        "src/*.c", "src/*.cpp",
        "src/Render/*.cpp", "src/Utilities/*.cpp",
        "src/Objects/*.cpp", "src/Objects/Light/*.cpp",
        "src/Resources/*.cpp"
    )
    set_languages("cxx11")
    add_includedirs("libs")
    add_packages("glfw", "glm")