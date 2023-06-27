
#ifndef STATPHYSQC_EXPORT_H
#define STATPHYSQC_EXPORT_H

#ifdef STATPHYSQC_STATIC_DEFINE
#  define STATPHYSQC_EXPORT
#  define STATPHYSQC_NO_EXPORT
#else
#  ifndef STATPHYSQC_EXPORT
#    ifdef StatPhysQC_EXPORTS
        /* We are building this library */
#      define STATPHYSQC_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define STATPHYSQC_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef STATPHYSQC_NO_EXPORT
#    define STATPHYSQC_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef STATPHYSQC_DEPRECATED
#  define STATPHYSQC_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef STATPHYSQC_DEPRECATED_EXPORT
#  define STATPHYSQC_DEPRECATED_EXPORT STATPHYSQC_EXPORT STATPHYSQC_DEPRECATED
#endif

#ifndef STATPHYSQC_DEPRECATED_NO_EXPORT
#  define STATPHYSQC_DEPRECATED_NO_EXPORT STATPHYSQC_NO_EXPORT STATPHYSQC_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef STATPHYSQC_NO_DEPRECATED
#    define STATPHYSQC_NO_DEPRECATED
#  endif
#endif

#endif
