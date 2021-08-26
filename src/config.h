/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you are compiling for PowerPC. */
/* #undef ARCH_PPC */

/* Define to 1 if you are compiling for PowerPC64. */
/* #undef ARCH_PPC64 */

/* Define to 1 if you are compiling for ix86. */
#define ARCH_X86 1

/* Define to 1 if you are compiling for amd64. */
#define ARCH_X86_64 1

/* The URL to open in order to report bugs */
#define BUG_REPORT_URL "https://gitlab.gnome.org/GNOME/gimp/issues/new"

/* Define to 1 if automatic update check is enabled */
#define CHECK_UPDATE 1

/* The path to default color profiles for this system */
#define COLOR_PROFILE_DIRECTORY "/usr/share/color/icc"

/* Define to 1 if SSE2 intrinsics are available. */
#define COMPILE_SSE2_INTRINISICS 1

/* Define to 1 if SSE4.1 intrinsics are available. */
#define COMPILE_SSE4_1_INTRINISICS 1

/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
   */
/* #undef CRAY_STACKSEG_END */

/* Define to 1 if using `alloca.c'. */
/* #undef C_ALLOCA */

/* Define to 1 to enable support for multiple processors. */
#define ENABLE_MP 1

/* always defined to indicate that i18n is enabled */
#define ENABLE_NLS 1

/* Define to 1 if resources are considered bundled under the same prefix */
/* #undef ENABLE_RELOCATABLE_RESOURCES */

/* Define to 1 if GIMP should open a debug console at start */
/* #undef ENABLE_WIN32_DEBUG_CONSOLE */

/* The prefix for our gettext translation domains. */
#define GETTEXT_PACKAGE "gimp30"

/* The string defining the build */
#define GIMP_BUILD_ID "unknown"

/* The OS this was built for */
#define GIMP_BUILD_PLATFORM "linux-gnu"

/* The OS family this was built for */
#define GIMP_BUILD_PLATFORM_FAMILY "linux"

/* libgimp pkg-config version */
#define GIMP_PKGCONFIG_VERSION "3.0"

/* GIMP tools version */
#define GIMP_TOOL_VERSION "2.99"

/* Define to 1 if this is an unstable version of GIMP */
#define GIMP_UNSTABLE 1

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
   */
#define HAVE_ALLOCA_H 1

/* Define to 1 if alsa is available */
#define HAVE_ALSA 1

/* Define to 1 if you have the <altivec.h> header file. */
/* #undef HAVE_ALTIVEC_H */

/* Define to 1 if the altivec runtime test should use a sysctl. */
/* #undef HAVE_ALTIVEC_SYSCTL */

/* Define to 1 if you have the `backtrace' function. */
#define HAVE_BACKTRACE 1

/* Define to 1 if you have the `bind_textdomain_codeset' function. */
#define HAVE_BIND_TEXTDOMAIN_CODESET 1

/* Define to 1 if you have the Mac OS X function CFLocaleCopyCurrent in the
   CoreFoundation framework. */
/* #undef HAVE_CFLOCALECOPYCURRENT */

/* Define to 1 if you have the Mac OS X function CFPreferencesCopyAppValue in
   the CoreFoundation framework. */
/* #undef HAVE_CFPREFERENCESCOPYAPPVALUE */

/* define if the compiler supports basic C++14 syntax */
#define HAVE_CXX14 1

/* Define to 1 if you have the `dcgettext' function. */
#define HAVE_DCGETTEXT 1

/* Define to 1 if you have the `difftime' function. */
#define HAVE_DIFFTIME 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef HAVE_DOPRNT */

/* Define to 1 if you have DirectX DirectInput headers and libraries */
/* #undef HAVE_DX_DINPUT */

/* Define to 1 when Dr. MingW is available */
/* #undef HAVE_EXCHNDL */

/* Define to 1 if you have the <execinfo.h> header file. */
#define HAVE_EXECINFO_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `fsync' function. */
#define HAVE_FSYNC 1

/* Define to 1 if the system has the `destructor' function attribute */
#define HAVE_FUNC_ATTRIBUTE_DESTRUCTOR 1

/* Define to 1 if you have the `getaddrinfo' function. */
#define HAVE_GETADDRINFO 1

/* Define to 1 if you have the `getnameinfo' function. */
#define HAVE_GETNAMEINFO 1

/* Define if the GNU gettext() function is already present or preinstalled. */
#define HAVE_GETTEXT 1

/* Define to 1 if you have the <ieeefp.h> header file. */
/* #undef HAVE_IEEEFP_H */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <sys/ipc.h> header file. */
#define HAVE_IPC_H 1

/* Define to 1 if the iso-codes package is available */
#define HAVE_ISO_CODES 1

/* Define if your <locale.h> file defines LC_MESSAGES. */
#define HAVE_LC_MESSAGES 1

/* Define to 1 if you have the `asound' library (-lasound). */
#define HAVE_LIBASOUND 1

/* Define to 1 if libbacktrace is available */
/* #undef HAVE_LIBBACKTRACE */

/* Define to 1 if you have the `execinfo' library (-lexecinfo). */
/* #undef HAVE_LIBEXECINFO */

/* Define to 1 if libgudev is available */
#define HAVE_LIBGUDEV 1

/* Define to 1 if libheif >= 1.4.0 is available */
#define HAVE_LIBHEIF_1_4_0 1

/* Define to 1 if you have the `nsl' library (-lnsl). */
/* #undef HAVE_LIBNSL */

/* Define to 1 if you have the `socket' library (-lsocket). */
/* #undef HAVE_LIBSOCKET */

/* Define to 1 if libunwind is available */
#define HAVE_LIBUNWIND 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the <math.h> header file. */
#define HAVE_MATH_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <sys/mman.h> header file. */
/* #undef HAVE_MMAN_H */

/* Define to 1 if you have the `mmap' function. */
#define HAVE_MMAP 1

/* Define to 1 if you have the rint function. */
#define HAVE_RINT 1

/* Define to 1 if you have the <sys/shm.h> header file. */
#define HAVE_SHM_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/prctl.h> header file. */
#define HAVE_SYS_PRCTL_H 1

/* Define to 1 if you have the <sys/select.h> header. */
/* #undef HAVE_SYS_SELECT_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/thr.h> header file. */
/* #undef HAVE_SYS_THR_H */

/* Define to 1 if you have the <sys/times.h> header file. */
#define HAVE_SYS_TIMES_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/wait.h> header file. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the `thr_self' function. */
/* #undef HAVE_THR_SELF */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `vfork' function. */
#define HAVE_VFORK 1

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* Define to 1 if webkit is available */
/* #undef HAVE_WEBKIT */

/* Define to 1 if you have the <X11/extensions/shape.h> header file. */
#define HAVE_X11_EXTENSIONS_SHAPE_H 1

/* Define to 1 if you have the <X11/Xmu/WinUtil.h> header file. */
#define HAVE_X11_XMU_WINUTIL_H 1

/* Define to 1 to if the XFIXES X extension is available */
#define HAVE_XFIXES 1

/* Define to 1 if xvfb-run is available */
/* #undef HAVE_XVFB_RUN */

/* Define to 1 if _NL_MEASUREMENT_MEASUREMENT is available */
#define HAVE__NL_MEASUREMENT_MEASUREMENT 1

/* Define to 1 if shared memory segments are released deferred. */
#define IPC_RMID_DEFERRED_RELEASE 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Define to 1 if you don't have the fd_set typedef. */
/* #undef NO_FD_SET */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "https://gitlab.gnome.org/GNOME/gimp/issues/new"

/* Define to the full name of this package. */
#define PACKAGE_NAME "GIMP"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "GIMP 2.99.7"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "gimp"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.99.7"

/* define to 1 if compiling for Mac OS X */
/* #undef PLATFORM_OSX */

/* The MTA used by the mail plug-in. */
/* #undef SENDMAIL */

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at runtime.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define to 1 if AltiVec support is available. */
/* #undef USE_ALTIVEC */

/* Define to 1 if MMX assembly is available. */
#define USE_MMX 1

/* Define to 1 to use POSIX shared memory */
/* #undef USE_POSIX_SHM */

/* Define to 1 if SSE assembly is available. */
#define USE_SSE 1

/* Define to 1 to use SYSV shared memory */
#define USE_SYSV_SHM 1

/* Define if 32-bit versions of libraries are packaged in an alternative
   folder */
/* #undef WIN32_32BIT_DLL_FOLDER */

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */

/* Enable large inode numbers on Mac OS X 10.5.  */
#ifndef _DARWIN_USE_64_BIT_INODE
# define _DARWIN_USE_64_BIT_INODE 1
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */
