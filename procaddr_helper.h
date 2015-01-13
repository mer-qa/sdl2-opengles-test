
/**
 *
 * SDL 2.0 OpenGL ES Test Application
 *
 * Copyright (C) 2015 Jolla Ltd.
 * Contact: Thomas Perl <thomas.perl@jolla.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 **/

#ifndef PROCADDR_HELPER_H
#define PROCADDR_HELPER_H

#include <stdio.h>
#include <dlfcn.h>

#include <EGL/egl.h>

typedef void (*PGLCLEARCOLOR)(GLclampf, GLclampf, GLclampf, GLclampf);
typedef const GLubyte *(*PGLGETSTRING)(GLenum);

#define LOOKUP_FUNCTION(T, N) \
    T linked_ ## N = (T)(&(N)); printf("&%s(): %p\n", #N, linked_ ## N); \
    T dlsym_ ## N = (T)dlsym(libGL, #N); printf("dlsym(..., \"%s\"): %p\n", #N, dlsym_ ## N); \
    T egl_ ## N = (T)eglGetProcAddress(#N); printf("eglGetProcAddress(\"%s\"): %p\n", #N, egl_ ## N); \

#define CALL_FUNCTION(N, FMT, ...) \
    printf("Calling %s(%s): " FMT "\n", #N, #__VA_ARGS__, linked_ ## N(__VA_ARGS__)); \
    printf("Calling dlsym() %s(%s): " FMT "\n", #N, #__VA_ARGS__, dlsym_ ## N(__VA_ARGS__)); \
    printf("Calling eglGetProcAddress() %s(%s): " FMT "\n", #N, #__VA_ARGS__, egl_ ## N(__VA_ARGS__)); \

#define CALL_FUNCTION_NORESULT(N, ...) \
    printf("Calling %s(%s)\n", #N, #__VA_ARGS__); linked_ ## N(__VA_ARGS__); \
    printf("Calling dlsym() %s(%s)\n", #N, #__VA_ARGS__); dlsym_ ## N(__VA_ARGS__); \
    printf("Calling eglGetProcAddress() %s(%s)\n", #N, #__VA_ARGS__); egl_ ## N(__VA_ARGS__); \

inline void testProcAddr(const char *libname)
{
    void *libGL = dlopen(libname, RTLD_LAZY);

    LOOKUP_FUNCTION(PGLGETSTRING, glGetString);
    LOOKUP_FUNCTION(PGLCLEARCOLOR, glClearColor);

    CALL_FUNCTION(glGetString, "'%s'", GL_EXTENSIONS);
    CALL_FUNCTION_NORESULT(glClearColor, 0.3f, 0.3f, 0.6f, 1.0f);
};

#endif /* PROCADDR_HELPER_H */
