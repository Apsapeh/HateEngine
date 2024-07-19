# Why?
### This engine is developed for a boomer shooter - Arx Infernus. <br /> Unity is a piece of shit, Godot is a very cool engine, but it's use OpenGL 2 and doesn't support Windows XP

<br />

# Supported platforms
## Developed on:
- MacOS (M1 chip)
- Linux (x86_64)
## Tested on (works without or with some small bugs):
- MacOS (M1 chip)
- Linux (x86_64)
- Windows XP (MSVC, AMD Athlon XP 2500+ (without SSE2))
- Windows 10, 11
## Tested on (with major bugs):
- Raspberry Pi 4 (ReactPhysics doesn't work)

<br />

# How to build
### Build only а static lib: 
```sh
$ xmake
```
### Build with ал examples
```sh
$ xmake f -m release --build_examples=true
$ xmake
```

<br />

P.S. Sry for my bad English.