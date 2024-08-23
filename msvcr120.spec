*asm:
%{m32:--32} %{m64:--64}

*asm_debug:
%{g*:%{%:debug-level-gt(0):}} %{ffile-prefix-map=*:--debug-prefix-map %*} %{fdebug-prefix-map=*:--debug-prefix-map %*}

*asm_debug_option:
%{g*:%{%:debug-level-gt(0):%{%:dwarf-version-gt(4):--gdwarf-5 ;%:dwarf-version-gt(3):--gdwarf-4 ;%:dwarf-version-gt(2):--gdwarf-3 ;:--gdwarf2 }}}

*asm_final:
%{gsplit-dwarf: 
       objcopy --extract-dwo 	 %{c:%{o*:%*}%{!o*:%w%b%O}}%{!c:%U%O} 	 %b.dwo 
       objcopy --strip-dwo 	 %{c:%{o*:%*}%{!o*:%w%b%O}}%{!c:%U%O}     }

*asm_options:
%{-target-help:%:print-asm-header()} %{v} %{w:-W} %{I*} %(asm_debug_option) %{gz*:%e-gz is not supported in this configuration} %a %Y %{c:%W{o*}%{!o*:-o %w%b%O}}%{!c:-o %d%w%u%O}

*invoke_as:
%{!fwpa*:   %{fcompare-debug=*|fdump-final-insns=*:%:compare-debug-dump-opt()}   %{!S:-o %|.s |
 as %(asm_options) %m.s %A }  }

*cpp:
%{posix:-D_POSIX_SOURCE} %{mthreads:-D_MT} %{municode:-DUNICODE} %{!no-pthread:-D_REENTRANT} %{pthread:-U_REENTRANT} %{mcrtdll=crtdll*:-U__MSVCRT__ -D__CRTDLL__} %{mcrtdll=msvcrt10*:-D__MSVCRT_VERSION__=0x100} %{mcrtdll=msvcrt20*:-D__MSVCRT_VERSION__=0x200} %{mcrtdll=msvcrt40*:-D__MSVCRT_VERSION__=0x400} %{mcrtdll=msvcrt-os*:-D__MSVCRT_VERSION__=0x700} %{mcrtdll=msvcr70*:-D__MSVCRT_VERSION__=0x700} %{mcrtdll=msvcr71*:-D__MSVCRT_VERSION__=0x701} %{mcrtdll=msvcr80*:-D__MSVCRT_VERSION__=0x800} %{mcrtdll=msvcr90*:-D__MSVCRT_VERSION__=0x900} %{mcrtdll=msvcr100*:-D__MSVCRT_VERSION__=0xA00} %{mcrtdll=msvcr110*:-D__MSVCRT_VERSION__=0xB00} %{mcrtdll=msvcr120*:-D__MSVCRT_VERSION__=0xC00} %{mcrtdll=ucrt*:-D_UCRT} 

*cpp_options:
%(cpp_unique_options) %1 %{m*} %{std*&ansi&trigraphs} %{W*&pedantic*} %{w} %{f*} %{g*:%{%:debug-level-gt(0):%{g*} %{!fno-working-directory:-fworking-directory}}} %{O*} %{undef} %{save-temps*:-fpch-preprocess}

*cpp_debug_options:
%<dumpdir %<dumpbase %<dumpbase-ext %{d*} %:dumps()

*cpp_unique_options:
%{!Q:-quiet} %{nostdinc*} %{C} %{CC} %{v} %@{I*&F*} %{P} %I %{MD:-MD %{!o:%b.d}%{o*:%.d%*}} %{MMD:-MMD %{!o:%b.d}%{o*:%.d%*}} %{M} %{MM} %{MF*} %{MG} %{MP} %{MQ*} %{MT*} %{Mmodules} %{Mno-modules} %{!E:%{!M:%{!MM:%{!MT:%{!MQ:%{MD|MMD:%{o*:-MQ %*}}}}}}} %{remap} %{%:debug-level-gt(2):-dD} %{!iplugindir*:%{fplugin*:%:find-plugindir()}} %{H} %C %{D*&U*&A*} %{i*} %Z %i %{E|M|MM:%W{o*}} %{fdeps-format=*:%{!fdeps-file=*:-fdeps-file=%:join(%{!o:%b.ddi}%{o*:%.ddi%*})}} %{fdeps-format=*:%{!fdeps-target=*:-fdeps-target=%:join(%{!o:%b.o}%{o*:%.o%*})}}

*trad_capable_cpp:
cc1 -E %{traditional|traditional-cpp:-traditional-cpp}

*cc1:
%(cc1_cpu) 

*cc1_options:
%{pg:%{fomit-frame-pointer:%e-pg and -fomit-frame-pointer are incompatible}} %{!iplugindir*:%{fplugin*:%:find-plugindir()}} %1 %{!Q:-quiet} %(cpp_debug_options) %{m*} %{aux-info*} %{g*} %{O*} %{W*&pedantic*} %{w} %{std*&ansi&trigraphs} %{v:-version} %{pg:-p} %{p} %{f*} %{undef} %{Qn:-fno-ident} %{Qy:} %{-help:--help} %{-target-help:--target-help} %{-version:--version} %{-help=*:--help=%*} %{!fsyntax-only:%{S:%W{o*}%{!o*:-o %w%b.s}}} %{fsyntax-only:-o %j} %{-param*} %{coverage:-fprofile-arcs -ftest-coverage} %{fprofile-arcs|fcondition-coverage|fprofile-generate*|coverage:   %{!fprofile-update=single:     %{pthread:-fprofile-update=prefer-atomic}}}

*cc1plus:


*link_gcc_c_sequence:
%G %{!nolibc:%L %G}

*link_ssp:
%{fstack-protector|fstack-protector-all|fstack-protector-strong|fstack-protector-explicit:-lssp_nonshared -lssp}

*endfile:
%{mdaz-ftz:crtfastmath.o%s;Ofast|ffast-math|funsafe-math-optimizations:%{!shared:%{!mno-daz-ftz:crtfastmath.o%s}}}    %{!shared:%:if-exists(default-manifest.o%s)}   %{fvtable-verify=none:%s;     fvtable-verify=preinit:vtv_end.o%s;     fvtable-verify=std:vtv_end.o%s}   crtend.o%s

*link:
%{m64:-m i386pep} %{!m64:-m i386pe} %{mwindows:--subsystem windows}   %{mconsole:--subsystem console}   %{shared: %{mdll: %eshared and mdll are not compatible}}   %{shared: --shared} %{mdll:--dll}   %{static:-Bstatic} %{!static:-Bdynamic}   %{shared|mdll: %{m64:-e DllMainCRTStartup} %{!m64:-e _DllMainCRTStartup@12} --enable-auto-image-base}     %{!shared:%{!mdll:%{no-pie:--disable-dynamicbase}}}  %(shared_libgcc_undefs)

*lib:
%{pg:-lgmon} %{!no-pthread:-lpthread} %{pthread: } %{mwindows:-lgdi32 -lcomdlg32} %{fvtable-verify=preinit:-lvtv -lpsapi;         fvtable-verify=std:-lvtv -lpsapi} -ladvapi32 -lshell32 -luser32 -lkernel32

*link_gomp:


*libgcc:
%{mthreads:-lmingwthrd} -lmingw32      %{static|static-libgcc:-lgcc -lgcc_eh}  %{!static:    %{!static-libgcc:      %{!shared:        %{!shared-libgcc:-lgcc -lgcc_eh}        %{shared-libgcc:-lgcc_s -lgcc}       }      %{shared:-lgcc_s -lgcc}     }   }     -lmoldname -lmingwex -lmsvcr120

*startfile:
%{shared|mdll:dllcrt2%O%s}   %{!shared:%{!mdll:%{!municode:crt2%O%s}}}   %{!shared:%{!mdll:%{municode:crt2u%O%s}}}   %{pg:gcrt2%O%s}   crtbegin.o%s   %{fvtable-verify=none:%s;     fvtable-verify=preinit:vtv_start.o%s;     fvtable-verify=std:vtv_start.o%s}

*cross_compile:
1

*version:
14.1.1

*multilib:
. !m64 !m32;.:../lib64 m64 !m32;.:../lib !m64 m32;

*multilib_defaults:
m32

*multilib_extra:


*multilib_matches:
m64 m64;m32 m32;

*multilib_exclusions:


*multilib_options:
m64/m32

*multilib_reuse:


*linker:
collect2

*linker_plugin_file:


*lto_wrapper:


*lto_gcc:


*post_link:


*link_libgcc:
%D

*md_exec_prefix:


*md_startfile_prefix:


*md_startfile_prefix_1:


*startfile_prefix_spec:


*sysroot_spec:
--sysroot=%R

*sysroot_suffix_spec:


*sysroot_hdrs_suffix_spec:


*self_spec:


*cc1_cpu:
%{march=native:%>march=native %:local_cpu_detect(arch %{m64|mx32:64;:32})   %{!mtune=*:%>mtune=native %:local_cpu_detect(tune %{m64|mx32:64;:32})}} %{mtune=native:%>mtune=native %:local_cpu_detect(tune %{m64|mx32:64;:32})}

*shared_libgcc_undefs:
%{shared-libgcc: -u ___register_frame_info -u ___deregister_frame_info}

*link_command:
%{!fsyntax-only:%{!c:%{!M:%{!MM:%{!E:%{!S:    %(linker) %{!fno-use-linker-plugin:%{!fno-lto:     -plugin %(linker_plugin_file)     -plugin-opt=%(lto_wrapper)     -plugin-opt=-fresolution=%u.res         %{flinker-output=*:-plugin-opt=-linker-output-known}     %{!nostdlib:%{!nodefaultlibs:%:pass-through-libs(%(link_gcc_c_sequence))}}     }}%{flto|flto=*:%<fcompare-debug*}     %{flto} %{fno-lto} %{flto=*} %l %{static|shared|r:;pie:-pie} %{fuse-ld=*:-fuse-ld=%*}  %{gz*:%e-gz is not supported in this configuration} %X %{o*} %{e*} %{N} %{n} %{r}    %{s} %{t} %{u*} %{z} %{Z} %{!nostdlib:%{!r:%{!nostartfiles:%S}}}     %{static|no-pie|static-pie:} %@{L*} %(link_libgcc) %{fvtable-verify=none:} %{fvtable-verify=std:   %e-fvtable-verify=std is not supported in this configuration} %{fvtable-verify=preinit:   %e-fvtable-verify=preinit is not supported in this configuration} %{!nostdlib:%{!r:%{!nodefaultlibs:%{%:sanitize(address):}     %{%:sanitize(hwaddress):}     %{%:sanitize(thread):}     %{%:sanitize(leak):}}}} %o      %{fopenacc|fopenmp|%:gt(%{ftree-parallelize-loops=*:%*} 1):	%:include(libgomp.spec)%(link_gomp)}    %{fgnu-tm:%:include(libitm.spec)%(link_itm)}     %{fsplit-stack: --wrap=pthread_create}    %{fprofile-arcs|fcondition-coverage|fprofile-generate*|coverage:-lgcov} %{!nostdlib:%{!r:%{!nodefaultlibs:%{%:sanitize(address):%{static-libasan:-Bstatic} -lasan %{static-libasan:-Bdynamic} %{static-libasan|static:%:include(libsanitizer.spec)%(link_libasan)}    %{static:%ecannot specify -static with -fsanitize=address}}    %{%:sanitize(hwaddress):%{static-libhwasan:-Bstatic} -lhwasan %{static-libhwasan:-Bdynamic} %{static-libhwasan|static:%:include(libsanitizer.spec)%(link_libhwasan)}	%{static:%ecannot specify -static with -fsanitize=hwaddress}}    %{%:sanitize(thread):%{static-libtsan:-Bstatic} -ltsan %{static-libtsan:-Bdynamic} %{static-libtsan|static:%:include(libsanitizer.spec)%(link_libtsan)}    %{static:%ecannot specify -static with -fsanitize=thread}}    %{%:sanitize(undefined):%{static-libubsan:-Bstatic} -lubsan %{static-libubsan:-Bdynamic} %{static-libubsan|static:%:include(libsanitizer.spec)%(link_libubsan)}}    %{%:sanitize(leak):%{static-liblsan:-Bstatic} -llsan %{static-liblsan:-Bdynamic} %{static-liblsan|static:%:include(libsanitizer.spec)%(link_liblsan)}}}}}     %{!nostdlib:%{!r:%{!nodefaultlibs:%(link_ssp) %(link_gcc_c_sequence)}}}    %{!nostdlib:%{!r:%{!nostartfiles:%E}}} %{T*}  
%(post_link) }}}}}}

