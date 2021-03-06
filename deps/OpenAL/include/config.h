/* API declaration export attribute */
//#define AL_API  __declspec(dllexport)
//#define ALC_API __declspec(dllexport)

/* Define to the library version */
#define ALSOFT_VERSION "1.14"

/* Define if we have the OSS backend */
/* #undef HAVE_OSS */

/* Define if we have the Solaris backend */
/* #undef HAVE_SOLARIS */

/* Define if we have the SndIO backend */
/* #undef HAVE_SNDIO */

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
/* Define if we have the MMDevApi backend */
#define HAVE_MMDEVAPI

/* Define if we have the DSound backend */
//#define HAVE_DSOUND

/* Define if we have the Windows Multimedia backend */
#define HAVE_WINMM
#endif

#if defined(__LINUX) || defined(__LINUX__) || defined(LINUX)
/* Define if we have the ALSA backend */
#define HAVE_ALSA

/* Define if we have the PortAudio backend */
/* #undef HAVE_PORTAUDIO */

/* Define if we have the PulseAudio backend */
#define HAVE_PULSEAUDIO
#endif

#if defined(__APPLE__) || defined(MACOSX)
/* Define if we have the CoreAudio backend */
#define HAVE_COREAUDIO
#endif

#if defined(__native_client__)
/* Define if we have the PPAPI backend */
#define HAVE_PPAPI
#endif

/* Define if we have the OpenSL backend */
/* #undef HAVE_OPENSL */

/* Define if we have the Wave Writer backend */
//#define HAVE_WAVE

/* Define if we have dlfcn.h */
/* #undef HAVE_DLFCN_H */

/* Define if we have the stat function */
#define HAVE_STAT

/* Define if we have the powf function */
#define HAVE_POWF

/* Define if we have the sqrtf function */
#define HAVE_SQRTF

/* Define if we have the cosf function */
#define HAVE_COSF

/* Define if we have the sinf function */
#define HAVE_SINF

/* Define if we have the acosf function */
#define HAVE_ACOSF

/* Define if we have the asinf function */
#define HAVE_ASINF

/* Define if we have the atanf function */
#define HAVE_ATANF

/* Define if we have the atan2f function */
#define HAVE_ATAN2F

/* Define if we have the fabsf function */
#define HAVE_FABSF

/* Define if we have the log10f function */
#define HAVE_LOG10F

/* Define if we have the floorf function */
#define HAVE_FLOORF

/* Define if we have the strtof function */
/* #undef HAVE_STRTOF */

/* Define if we have stdint.h */
#define HAVE_STDINT_H

/* Define if we have the __int64 type */
/* #undef HAVE___INT64 */

/* Define to the size of a long int type */
#define SIZEOF_LONG 4

/* Define to the size of a long long int type */
#define SIZEOF_LONG_LONG 8

/* Define if we have GCC's destructor attribute */
#if defined(__native_client__)
/* #undef HAVE_GCC_DESTRUCTOR */
#define HAVE_GCC_DESTRUCTOR
#endif

/* Define if we have GCC's format attribute */
/* #undef HAVE_GCC_FORMAT */

/* Define if we have pthread_np.h */
/* #undef HAVE_PTHREAD_NP_H */

/* Define if we have arm_neon.h */
/* #undef HAVE_ARM_NEON_H */

/* Define if we have guiddef.h */
#if !defined(__native_client__)
#define HAVE_GUIDDEF_H
#endif

/* Define if we have guiddef.h */
/* #undef HAVE_INITGUID_H */

/* Define if we have ieeefp.h */
/* #undef HAVE_IEEEFP_H */

/* Define if we have float.h */
#define HAVE_FLOAT_H
 
/* Define if we have fpu_control.h */
/* #undef HAVE_FPU_CONTROL_H */

/* Define if we have fenv.h */
/* #undef HAVE_FENV_H */

/* Define if we have fesetround() */
/* #undef HAVE_FESETROUND */

/* Define if we have _controlfp() */
#if !defined(__native_client__)
#define HAVE__CONTROLFP
#endif

/* Define if we have pthread_setschedparam() */
/* #undef HAVE_PTHREAD_SETSCHEDPARAM */

/* Define if we have the restrict keyword */
/* #undef HAVE_RESTRICT */

/* Define if we have the __restrict keyword */
#define HAVE___RESTRICT
