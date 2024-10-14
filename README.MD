<!-- ![logo](readme_res/logo.png =100x) -->
<img src="readme_res/logo.png" alt="drawing" width="400" style="display: block; margin: 0 auto"/>

# Why?
### This engine is developed for a boomer shooter - Arx Infernus. <br /> Unity is a piece of shit, Godot is a very cool engine, but it's use OpenGL 2 and doesn't support Windows XP

# Some Examples
## [YouTube Playlist](https://www.youtube.com/playlist?list=PLcj730HQ_3uWg2s6zLx-olq6rEzHnq53T)

<br />

# Supported platforms
## Developed on:
- MacOS (M1 chip)
- Linux (x86_64)
## Tested on (works without or with some small bugs):
- MacOS (M1 chip)
- Linux (x86_64)
- Windows XP (MSVC 2022 (v140), MinGW*, AMD Athlon XP 2500+ (without SSE2))
- Windows 10, 11
## Tested on (with major bugs):
- Raspberry Pi 4 (ReactPhysics doesn't work)

\* - MinGW <=7.0.0 (standalone) or newer, but with additional libwinpthread-1.dll

<br />

# How to build
### Build only а static lib: 
```sh
$ xmake
```
### Build with аn examples
```sh
$ xmake f -m release --build_examples=true
$ xmake
```

<br />

P.S. Sry for my bad English.