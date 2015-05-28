//////////////////////////////////////////////////////////////////////////////
// Minimal Embedded C++ support, no exception handling, no RTTI
// Date of the Last Update:  Sep 10, 2011
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) 2007-2011 Quantum Leaps, LLC. All rights reserved.
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// e-mail:                  info@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>                   // for prototypes of malloc() and free()

//............................................................................
void *operator new(size_t size) throw()
{
    (void)size;
    return malloc(size);
}
//............................................................................
void operator delete(void *p) throw()
{
    (void)p;
    free(p);
}

extern "C" {
    //............................................................................
    void __cxa_atexit(void (*arg1)(void *), void *arg2, void *arg3)
    {
        (void)arg1; (void)arg2; (void)arg3;
    }
    //............................................................................
    void __cxa_guard_acquire()
    {
    }
    //............................................................................
    void __cxa_guard_release()
    {
    }
    //............................................................................

}                                                                // extern "C"

/**
 * This is an error handler that is invoked by the C++ runtime when a pure virtual function is called.
 * If anywhere in the runtime of your program an object is created with a virtual function pointer not
 * filled in, and when the corresponding function is called, you will be calling a 'pure virtual function'.
 * The handler you describe should be defined in the default libraries that come with your development environment.
 */
extern "C" void __cxa_pure_virtual() {
        while (1) ;
}
