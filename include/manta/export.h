
#ifndef MANTA_EXPORT_H
#define MANTA_EXPORT_H

#ifdef MANTA_STATIC
#  define MANTA_EXPORT
#  define MANTA_NO_EXPORT
#else
#  ifndef MANTA_EXPORT
#    ifdef Manta_EXPORTS
        /* We are building this library */
#      define MANTA_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define MANTA_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef MANTA_NO_EXPORT
#    define MANTA_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef MANTA_DEPRECATED
#  define MANTA_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef MANTA_DEPRECATED_EXPORT
#  define MANTA_DEPRECATED_EXPORT MANTA_EXPORT MANTA_DEPRECATED
#endif

#ifndef MANTA_DEPRECATED_NO_EXPORT
#  define MANTA_DEPRECATED_NO_EXPORT MANTA_NO_EXPORT MANTA_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef MANTA_NO_DEPRECATED
#    define MANTA_NO_DEPRECATED
#  endif
#endif

#endif /* MANTA_EXPORT_H */
