/*
 Copyright (c) 2018 Mathieu Laurendeau <mat.lau@laposte.net>
 License: GPLv3
 */

#ifndef GLOG_H_
#define GLOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define GLOG_STR_HELPER(x) #x
#define GLOG_STR(x) GLOG_STR_HELPER(x)

#define TOKENPASTE_HELPER(x, y) x ## y
#define TOKENPASTE(x, y) TOKENPASTE_HELPER(x, y)

typedef enum {
  E_GLOG_LEVEL_NONE,
  E_GLOG_LEVEL_ERROR,
  E_GLOG_LEVEL_INFO,
  E_GLOG_LEVEL_DEBUG,
  E_GLOG_LEVEL_TRACE
} e_glog_level;

#define GLOG_INST(CTX) \
  e_glog_level TOKENPASTE(CTX,_log_level) = E_GLOG_LEVEL_ERROR; \
  void TOKENPASTE(CTX,_set_log_level)(e_glog_level level) { TOKENPASTE(CTX,_log_level) = level; } \
  void TOKENPASTE(CTX,_log_constructor)(void) __attribute__((constructor)); \
  void TOKENPASTE(CTX,_log_constructor)(void) { glog_register(GLOG_STR(CTX), TOKENPASTE(CTX,_set_log_level)); }

#define GLOG_GET(CTX) extern e_glog_level TOKENPASTE(CTX,_log_level);

#define GLOG_LEVEL(CTX, LEVEL) (TOKENPASTE(CTX,_log_level) >= E_GLOG_LEVEL_##LEVEL)

typedef void (* GLOG_CALLBACK)(e_glog_level level);

void glog_register(const char * name, GLOG_CALLBACK callback);

void glog_set_level(const char * name, e_glog_level level);

void glog_set_all_levels(e_glog_level level);

#ifdef __cplusplus
}
#endif

#endif /* GLOG_H_ */
