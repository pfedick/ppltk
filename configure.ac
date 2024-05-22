###############################################################################
# This file is part of "Patrick's Programming Library Toolkit", ppltk.
# Web: https://github.com/pfedick/ppltk
#
###############################################################################
# Copyright (c) 2024, Patrick Fedick <patrick@pfp.de>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER AND CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
###############################################################################


dnl Process this file with autoconf to produce a configure script.
AC_PREREQ([2.71])
AC_INIT([ppltk],[1.0.0],[patrick@pfp.de],[ppltk-1.0.0.tar.bz2])
AC_CONFIG_AUX_DIR(autoconf)

AC_CONFIG_SRCDIR([include/ppltk.h])
AC_CONFIG_SRCDIR([include/ppltk_config.h.in])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADERS([include/ppltk_config.h])


###############################################################################
# Includes
###############################################################################
#sinclude(autoconf/ax_cxx_compile_stdcxx.m4)
sinclude(autoconf/sdl2.m4)
sinclude(autoconf/ppl7.m4)
#sinclude(autoconf/lib-link.m4)
#sinclude(autoconf/checkfuncs.m4)
#sinclude(autoconf/ax_gcc_x86_cpuid.m4)
#sinclude(autoconf/ax_check_compile_flag.m4)
#sinclude(autoconf/ax_check_compiler_flags.m4)
#sinclude(autoconf/ax_gcc_archflag.m4)


report_have_sdl2="no"


AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE

OLD_CFLAGS=$CFLAGS
# Checks for programs.
AC_PROG_CC
CFLAGS=$OLD_CFLAGS
AC_PROG_CXX

AX_CXX_COMPILE_STDCXX_17
dnl AX_CXX_COMPILE_STDCXX(11)


dnl AC_LANG(C++)

dnl AX_GCC_ARCHFLAG
#AX_CC_MAXOPT

AX_CHECK_COMPILE_FLAG([-no-pie],
    LDFLAGS="-no-pie $LDFLAGS"
	CFLAGS="$CFLAGS -fPIE "
    NOPIE="-no-pie",
    NOPIE=""
)
AC_SUBST(NOPIE)



case "$host_os" in
	freebsd*)
		CFLAGS="-I/usr/local/include $CFLAGS"
		LDFLAGS="-L/usr/local/lib $LDFLAGS"
  		;;
	*)
		;;
esac



PKG_PROG_PKG_CONFIG()

#--------------------------------------------------------------------
#       Check for various typedefs and provide substitutes if
#       they don't exist.
#--------------------------------------------------------------------

AC_HEADER_STDBOOL
AC_CHECK_HEADERS_ONCE([sys/time.h])
AC_TYPE_MODE_T

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_STRUCT_TM

AC_PATH_XTRA

AC_CHECK_HEADERS([stdlib.h stdint.h inttypes.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([stdarg.h])
AC_CHECK_HEADERS([stddef.h])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([math.h])
AC_CHECK_HEADERS([sys/types.h])
AC_CHECK_HEADERS([strings.h])
AC_CHECK_HEADERS([errno.h])
AC_CHECK_HEADERS([sys/stat.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([ctype.h])
AC_CHECK_HEADERS([wctype.h])
AC_CHECK_HEADERS([locale.h])
AC_CHECK_HEADERS([wchar.h])
AC_CHECK_HEADERS([widec.h])

AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINTPTR_T

AC_CHECK_TYPE(size_t, unsigned)
AC_CHECK_TYPE(ssize_t, signed)
AC_CHECK_TYPE(wchar_t, unsigned)

# Checks for library functions.



dnl Check for SDL
report_have_sdl2="no"
AM_PATH_SDL2([2.0.0],
            AC_DEFINE(HAVE_SDL2, 1, [ Define if you have libsdl2. ])
            report_have_sdl2="yes"
			AC_SUBST(SDL2_CFLAGS)
			AC_SUBST(SDL2_LIBS)
            ,
			AC_MSG_ERROR([libsdl2 is required])
			:)
AC_SUBST(report_have_sdl2)

report_have_ppl7="no"
AX_PATH_LIB_PPL7([7.0.0],
            AC_DEFINE(HAVE_PPL7, 1, [ Define if you have libppl7. ])
            report_have_ppl7="yes"
			AC_SUBST(LIBPPL7_CFLAGS)
			AC_SUBST(LIBPPL7_LIBS)
			AC_SUBST(LIBPPL7)
			AX_PPL7_FEATURE(zlib,,AC_MSG_ERROR([ppl7 feature "zlib" is required]))
			AX_PPL7_FEATURE(png,,AC_MSG_ERROR([ppl7 feature "png" is required]))
			#AX_PPL7_FEATURE(blubb,,AC_MSG_ERROR([ppl7 feature "blubb" is required]))
            ,
			AC_MSG_ERROR([ppl7 is required])
			:)
AC_SUBST(report_have_ppl7)


###############################################################################
# Output
###############################################################################

AC_OUTPUT


###############################################################################
# Report
###############################################################################

dnl report configuration
AC_MSG_RESULT([
******************************************************************************
** Configuration summary for $PACKAGE_NAME Version $PACKAGE_VERSION
** Target: $target_os        CPU: $target_cpu
******************************************************************************
**
**   SDL2 library:         $report_have_sdl2
**   PPL7 library:         $report_have_sdl2
**
******************************************************************************
])
