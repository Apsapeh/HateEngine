# Custom compiled *libstdc++-6.dll* and *libgcc_s_dw2-1.dll*
### <span style="color: red"> If your Windows target is above, then Windows XP or you use the MSVC compiler, you doesn't need this libraries </span>

<!-- ###  -->
<br>

### Русский
Перекомпилированные *libstdc++-6.dll* и *libgcc_s_dw2-1.dll*
**Компилятор**: i686-w64-mingw32-g++ (GCC) 14.2.0
**Версия компилируемого gcc**: 14.2.0
**Флаги конфигуратора**: 
```
../configure --prefix=/home/ghost/Downloads/mingw64-i686-libstdc++-msvcr120/out/ --host=i686-w64-mingw32 --target=i686-w64-mingw32 --disable-multilib --enable-languages=c,c++ --without-isl --disable-nls LDFLAGS="-specs=/home/ghost/Downloads/mingw64-i686-libstdc++-msvcr120/gcc-releases-gcc-14.2.0/msvcr120v2.spec" CFLAGS="-specs=/home/ghost/Downloads/mingw64-i686-libstdc++-msvcr120/gcc-releases-gcc-14.2.0/msvcr120v2.spec" CXXFLAGS="-specs=/home/ghost/Downloads/mingw64-i686-libstdc++-msvcr120/gcc-releases-gcc-14.2.0/msvcr120v2.spec"
```

Во время компиляции возникали ошибки из-за отсутствия функции **getpid()**. Просто во всех файлах (штук 5), где были ошибки, заменил вызов функции на **0**. Надеюсь никак не повлияет на работоспособность *libstdc++-6.dll* и *libgcc_s_dw2-1.dll*.

Ещё, в файлах ниже, заменил **-lmsvcrt** на **-lmsvcr120**. Иначе *libgcc_s_dw2-1.dll* компилировалась с UCRT, а оно на WinXP работает не очень хорошо.

```
gcc-releases-gcc-14.2.0/gcc/config/i386/mingw-w64.h:49:   -lmingwex %{!mcrtdll=*:-lmsvcrt} %{mcrtdll=*:-l%*} \
gcc-releases-gcc-14.2.0/gcc/config/i386/mingw32.h:203:   -lmingwex %{!mcrtdll=*:-lmsvcrt} %{mcrtdll=*:-l%*} \
gcc-releases-gcc-14.2.0/libgcc/config/i386/t-mingw32:2:SHLIB_LC = -lmingwthrd -lmingw32 -lmingwex -lmoldname -lmsvcrt -ladvapi32 -lshell32 -luser32 -lkernel32
```

Т.к. стандартные прекомпилированные библиотеки, поставляемые вместе с MinGW зависят от UCRT, то при динамческой линковке оно не запускалось на WinXP, можно [обходными путями](https://github.com/abbodi1406/vcredist/releases/tag/v0.35.0) поставить Visual C++ 2019, но это хрень какая-то. Можно линковать *libstdc++-6.dll* и *libgcc_s_dw2-1.dll* статически, но они под LGPL. Самое адекватное решение это перекомпилировать эти библиотеки.

### English
In future may be, I'm very lazy, just use a translator from Russian