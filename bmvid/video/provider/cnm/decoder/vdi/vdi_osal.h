/*
 * Copyright (c) 2018, Chips&Media
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _VDI_OSAL_H_
#define _VDI_OSAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "vputypes.h"

typedef enum
{
    ERR     = 0,
    WARN    = 1,
    INFO    = 2,
    DEBUG   = 3,
    LOG     = 4,
    TRACE   = 5,
    MAX_LOG_LEVEL
} VpuDecLogLevel;

typedef void (*VpuDecLoggingFunc)(VpuDecLogLevel log_level, char const *file, int const line,
                                    char const *fn, const char *format, ...);

extern VpuDecLogLevel dec_cur_log_level_threshold;
extern VpuDecLoggingFunc dec_cur_logging_fn;

enum
{
    LOG_HAS_DAY_NAME   =    1, /**< Include day name [default: no]           */
    LOG_HAS_YEAR       =    2, /**< Include year digit [no]              */
    LOG_HAS_MONTH      =    4, /**< Include month [no]              */
    LOG_HAS_DAY_OF_MON =    8, /**< Include day of month [no]          */
    LOG_HAS_TIME      =   16, /**< Include time [yes]              */
    LOG_HAS_MICRO_SEC  =   32, /**< Include microseconds [yes]             */
    LOG_HAS_FILE      =   64, /**< Include sender in the log [yes]           */
    LOG_HAS_NEWLINE      =  128, /**< Terminate each call with newline [yes] */
    LOG_HAS_CR      =  256, /**< Include carriage return [no]           */
    LOG_HAS_SPACE      =  512, /**< Include two spaces before log [yes]    */
    LOG_HAS_COLOR      = 1024, /**< Colorize logs [yes on win32]          */
    LOG_HAS_LEVEL_TEXT = 2048 /**< Include level text string [no]          */
};
enum {
    TERM_COLOR_R    = 2,    /**< Red            */
    TERM_COLOR_G    = 4,    /**< Green          */
    TERM_COLOR_B    = 1,    /**< Blue.          */
    TERM_COLOR_BRIGHT = 8    /**< Bright mask.   */
};

#define MAX_PRINT_LENGTH 512

#ifdef ANDROID
#include <utils/Log.h>
#undef LOG_NDEBUG
#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "VPUAPI"
#endif

#define VLOG(level, ...) do{ if (level <= dec_cur_log_level_threshold) { dec_cur_logging_fn(level, __FILE__, __LINE__, __func__, __VA_ARGS__); } } while(0)

#define LOG_ENABLE_FILE    SetLogDecor(GetLogDecor()|LOG_HAS_FILE);

typedef void * osal_file_t;
# ifndef SEEK_SET
# define    SEEK_SET    0
# endif

# ifndef SEEK_CUR
# define    SEEK_CUR    1
# endif

# ifndef SEEK_END
# define    SEEK_END    2
# endif

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WIN32) || defined(__MINGW32__)
#elif defined(linux) || defined(__linux) || defined(ANDROID)
#else

#ifndef stdout
# define    stdout    (void * )1
#endif
#ifndef stderr
# define    stderr    (void * )1
#endif

#endif

typedef void* osal_thread_t;
typedef void* osal_mutex_t;
typedef void* osal_cond_t;

#if defined (__cplusplus)
extern "C" {
#endif

    void SetLoggingFunc(VpuDecLoggingFunc logging_fn);
    VpuDecLogLevel GetLoggingFunc(void);

    int InitLog(void);
    void DeInitLog(void);

    void SetMaxLogLevel(int level);
    int GetMaxLogLevel(void);

    void SetLogColor(int level, int color);
    int GetLogColor(int level);

    void SetLogDecor(int decor);
    int GetLogDecor(void);

    //log print
    void LogMsg(int level, const char *format, ...);

    //timer
    void timer_start(void);
    void timer_stop(void);
    double timer_elapsed_us(void);
    double timer_elapsed_ms(void);
    int timer_is_valid(void);
    double timer_frequency(void);

    //math
    //int math_div(int number, int denom);
    int math_modulo(int number, int denom);

    //terminal
    void osal_init_keyboard(void);
    void osal_close_keyboard(void);

    //memory
    void * osal_memcpy (void *dst, const void * src, int count);
    void * osal_memset (void *dst, int val,int count);
    int    osal_memcmp(const void* src, const void* dst, int size);
    void * osal_malloc(int size);
    void * osal_realloc(void* ptr, int size);
    void osal_free(void *p);

    osal_file_t osal_fopen(const char * osal_file_tname, const char * mode);
    size_t osal_fwrite(const void * p, int size, int count, osal_file_t fp);
    size_t osal_fread(void *p, int size, int count, osal_file_t fp);
    long osal_ftell(osal_file_t fp);
    int osal_fseek(osal_file_t fp, long offset, int origin);
    int osal_fclose(osal_file_t fp);
    int osal_fflush(osal_file_t fp);
    int osal_fprintf(osal_file_t fp, const char * _Format, ...);
    int osal_fscanf(osal_file_t fp, const char * _Format, ...);
    int osal_kbhit(void);
    int osal_getch(void);
    int osal_flush_ch(void);
    int osal_feof(osal_file_t fp);

    char osal_fgetc(osal_file_t fp);
    void osal_srand(int seed);
    int osal_rand(void);
    int osal_toupper(int c);
    size_t osal_fputs(const char *s, osal_file_t fp);

    size_t osal_strlen(const char * str);
    int osal_strcmp(const char * str1, const char * str2);
    int osal_sprintf(char * str, const char * format, ...);
    int osal_atoi(const char * str);
    char * osal_strtok(char * str, const char * delimiters);
    char * osal_strcpy(char * destination, const char * source);
    double osal_atof(const char* str);
    char * osal_fgets(char * str, int num, osal_file_t stream);
    int osal_strcasecmp(const char *s1, const char *s2);
    int osal_sscanf(const char * s, const char * format, ...);
    int osal_strncmp(const char * str1, const char * str2, size_t num);
    char * osal_strstr(char * str1, const char * str2);
    int osal_fputc(int c, osal_file_t fp);
    char * osal_strdup(const char * str);

    void osal_msleep(Uint32 millisecond);

/********************************************************************************
 * THREAD                                                                       *
 ********************************************************************************/
osal_thread_t osal_thread_create(void(*start_routine)(void*), void*arg);
/* @return  0 - success
            2 - failure
 */
Int32 osal_thread_join(osal_thread_t thread, void** retval);
/* @return  0 - success
            1 - timed out
            2 - failure
 */
Int32 osal_thread_timedjoin(osal_thread_t thread, void** retval, Uint32 second);

BOOL osal_thread_cancel(osal_thread_t thread);

/********************************************************************************
 * MUTEX                                                                        *
 ********************************************************************************/
#ifdef __linux__
osal_mutex_t osal_mutex_create(void);
void osal_mutex_destroy(osal_mutex_t mutex);
BOOL osal_mutex_lock(osal_mutex_t mutex);
BOOL osal_mutex_unlock(osal_mutex_t mutex);
#elif _WIN32
osal_mutex_t *osal_mutex_create(void);
void osal_mutex_destroy(osal_mutex_t *mutex);
BOOL osal_mutex_lock(osal_mutex_t *mutex);
BOOL osal_mutex_unlock(osal_mutex_t *mutex);
#endif
/************************************************************************/
/* COND                                                                */
/************************************************************************/
osal_cond_t osal_cond_create(void);
void osal_cond_destroy(osal_cond_t handle);
BOOL osal_cond_wait(osal_cond_t handle);

BOOL osal_cond_signal(osal_cond_t handle);

BOOL osal_cond_lock(osal_cond_t handle);
BOOL osal_cond_unlock(osal_cond_t handle);

/********************************************************************************
 * SYSTEM TIME                                                                  *
 ********************************************************************************/
/* @brief   It returns system time in millisecond.
 */
Uint64 osal_gettime(void);
#if defined (__cplusplus)
}
#endif

#endif //#ifndef _VDI_OSAL_H_

