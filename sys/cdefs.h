#ifndef __sys_cdefs_h
#define __sys_cdefs_h

#ifdef __cplusplus
# define    __BEGIN_DECLS   extern "C" {
# define    __END_DECLS     }
#else
# define    __BEGIN_DECLS
# define    __END_DECLS
#endif

#define _UNUSED                 __attribute__((unused))
#define _ENSURE_USED(_u, _v)    ((void)(_u), (_v))

#define _FLASH                  const __flash
#define _MACRO                  static __inline

#define lengthof(x) (sizeof(x)/sizeof(*(x)))

#endif
