# Process this file with autoconf to produce a configure script.

AC_INIT(SoundFeatureExtraction, 1.0)
m4_include([common.ac])
AC_SUBST(INTERFACE_VERSION, [0])
AC_SUBST(REVISION_NUMBER, [$(cd $srcdir && git rev-list HEAD --count)])
AC_SUBST(AGE_NUMBER, [0])

# Check whether to conduct test benchmarks
AC_ARG_ENABLE([benchmarks],
    AS_HELP_STRING([--enable-benchmarks], [execute SIMD speedup benchmarks during tests evaluation])
)
AS_IF([test "x$enable_benchmarks" = "xyes"], [
    CPPFLAGS="$CPPFLAGS -DBENCHMARK"
])

# Check whether to use nice Eina logging
AC_ARG_ENABLE([eina-logging],
    AS_HELP_STRING([--enable-eina-logging], [Use Eina as the logging backend])
)
AS_IF([test "x$enable_eina_logging" = "xyes"], [    
    PKG_CHECK_MODULES([EINA], [eina >= 1.0])
    CPPFLAGS="$CPPFLAGS -DEINA $EINA_CFLAGS"
])

# Check whether to use the built-in Boost
AC_ARG_WITH([built-in-boost],
    AS_HELP_STRING([--with-built-in-boost], [use statically linked embedded Boost parts]), [
    BOOST_DIR="boost"
    LDFLAGS="$LDFLAGS -L\$(abs_top_builddir)/src/boost/stage/lib"
    # Set path to bjam
    AC_ARG_VAR(BJAM, path to bjam Boost builder)
    
    AC_PATH_PROG(BJAM, [bjam], "")
    
    AS_IF([test -z "$BJAM"],
        AC_MSG_ERROR([Building Boost requires a working bjam. You can obtain it via bootstrapping the Boost package.])
    )]
)
AC_SUBST([BOOST_DIR])

# Check whether to use the built-in FFTF
AC_ARG_WITH([built-in-fftf],
    AS_HELP_STRING([--with-built-in-fftf], [use bundled FFTF]), [
    FFTF_LIBS="\$(top_builddir)/fftf/src/libfftf.la"
    FFTF_DIR=fftf
    AC_CONFIG_SUBDIRS([fftf])
    CPPFLAGS="$CPPFLAGS -I\$(top_srcdir)/fftf/inc"
], [
    # Check for installed FFTF
    PKG_CHECK_MODULES([FFTF], [fftf >= 1.0])
])
AC_SUBST([FFTF_DIR])

# Check whether to use the built-in libSimd
AC_ARG_WITH([built-in-simd],
    AS_HELP_STRING([--with-built-in-simd], [use bundled libSimd]), [
    SIMD_LIBS="\$(top_builddir)/simd/src/libSimd.la"
    SIMD_DIR=simd
    mkdir -p simd
    echo "\$(abs_top_builddir)/../fftf" > simd/built-in-fftf
    AC_CONFIG_SUBDIRS([simd])
    AS_IF([test "x$with_built_in_fftf" = "xyes"], [
    	AC_CONFIG_LINKS(["$top_builddir""simd/src/fftf":"$top_srcdir""fftf/inc/fftf"])
    ])
    CPPFLAGS="$CPPFLAGS -I\$(top_srcdir)/simd/inc"    
], [
    # Check for installed SIMD
    PKG_CHECK_MODULES([SIMD], [Simd >= 1.0])
])
AC_SUBST([SIMD_DIR])

AM_CXXFLAGS="$AM_CXXFLAGS -I\$(top_srcdir)/DSPFilters/shared/DSPFilters/include"
AM_CXXFLAGS="$AM_CXXFLAGS -Wno-virtual-move-assign"

# Additional tests
AM_COND_IF([TESTS], [
    AC_CONFIG_FILES(tests/primitives/Makefile
                    tests/transforms/Makefile
                    tests/allocators/Makefile
                    )
])

# Link with Makefile.am in additional source directories
AC_CONFIG_FILES(src/boost/Makefile
                src/boost/user-config.jam
                )

AC_OUTPUT


AC_DEFUN([LIB_SOUND_FEATURE_EXTRACTION_PRINT_STATUS], [
echo
echo -e "${COLOR_WHITE}Sound feature extraction library options:${COLOR_RESET}"
echo -e "  benchmarks.........: $(color_yes_no ${enable_benchmarks:-no})"
echo -e "  eina_logging.......: $(color_yes_no ${enable_eina_logging:-no})"
echo -e "  built_in_simd......: $(color_yes_no ${with_built_in_simd:-no})"
echo -e "  built_in_boost.....: $(color_yes_no ${with_built_in_boost:-no})"
echo -e "  built_in_fftf......: $(color_yes_no ${with_built_in_fftf:-no})"
echo
])

COMMON_PRINT_STATUS
LIB_SOUND_FEATURE_EXTRACTION_PRINT_STATUS
