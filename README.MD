<!-- ![logo](readme_res/logo.png =100x) -->
<!-- <img src="readme_res/logo.png" alt="drawing" width="400" style="display: block; margin: 0 auto"/> -->
<p align="center">
    <img src="readme_res/logo.png" alt="drawing" width="400" style="display: block; margin: 0 auto"/>
</p>

# :question: Why

### This engine is developed for a boomer shooter - Arx Infernus. <br /> Unity is a piece of shit, Godot is a very cool engine, but it's use OpenGL 2 and doesn't support Windows XP


# :eyes: Some Examples

## [YouTube Playlist](https://www.youtube.com/playlist?list=PLcj730HQ_3uWg2s6zLx-olq6rEzHnq53T)


# :gear: Supported platforms

- Linux 
- MacOS (Apple Silicon and Intel 64-bit, MacOS 10.7 and above)
- Windows (XP and above, MinGW\*, MSVC\*\*)

## Developed on:
- MacOS (M1 chip)
- Linux (x86_64)

## Tested on (works without or with some small bugs):
- MacOS (M1 chip, i5-2th gen)
- Linux (x86_64, armv6 (RPI4))
- Windows XP (MinGW*, AMD Athlon XP 2500+ (without SSE2))
- Windows 10, 11 (MinGW*)


\* - MinGW <=7.0.0 (standalone) or newer, but with additional libwinpthread-1.dll
\*\* - MSVC is long untested, I don't know, it still works or not


# :hammer: How to build
### Autoconfigure and build library for all supported platforms on your host:
```sh
$ ./build.py all
```

### Build only а library (static and dynamic): 
```sh
$ xmake
```

### Build with аn examples
```sh
$ xmake f -m release --build_examples=true
$ xmake
```


# :warning: УВАГА! *Code quality is very poor* :warning: