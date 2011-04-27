//  Copyright (c) 2007-2011 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_UTIL_ITT_NOTIFY_AUG_17_2010_1237PM)
#define HPX_UTIL_ITT_NOTIFY_AUG_17_2010_1237PM

#include <hpx/config.hpp>

struct ___itt_caller;
struct __itt_frame_t;

///////////////////////////////////////////////////////////////////////////////
// decide whether to use the ITT notify API if it's available

#if HPX_USE_ITT == 1
extern bool use_ittnotify_api;

///////////////////////////////////////////////////////////////////////////////
HPX_EXPORT void itt_sync_create(void* addr, const char* objtype, const char* objname);
HPX_EXPORT void itt_sync_rename(void* addr, const char* name);
HPX_EXPORT void itt_sync_prepare(void* addr);
HPX_EXPORT void itt_sync_acquired(void* addr);
HPX_EXPORT void itt_sync_cancel(void* addr);
HPX_EXPORT void itt_sync_releasing(void* addr);
HPX_EXPORT void itt_sync_released(void* addr);
HPX_EXPORT void itt_sync_destroy(void* addr);

HPX_EXPORT ___itt_caller* itt_stack_create();
HPX_EXPORT void itt_stack_enter(___itt_caller* ctx);
HPX_EXPORT void itt_stack_leave(___itt_caller* ctx);
HPX_EXPORT void itt_stack_destroy(___itt_caller* ctx);

HPX_EXPORT __itt_frame_t* itt_frame_create(char const*);
HPX_EXPORT void itt_frame_begin(__itt_frame_t* frame);
HPX_EXPORT void itt_frame_end(__itt_frame_t* frame);
HPX_EXPORT void itt_frame_destroy(__itt_frame_t* frame);

HPX_EXPORT int itt_mark_create(char const*);
HPX_EXPORT void itt_mark_off(int mark);
HPX_EXPORT void itt_mark(int mark, char const*);

#else

inline void itt_sync_create(void* addr, const char* objtype, const char* objname) {}
inline void itt_sync_rename(void* addr, const char* name) {}
inline void itt_sync_prepare(void* addr) {}
inline void itt_sync_acquired(void* addr) {}
inline void itt_sync_cancel(void* addr) {}
inline void itt_sync_releasing(void* addr) {}
inline void itt_sync_released(void* addr) {}
inline void itt_sync_destroy(void* addr) {}

inline ___itt_caller* itt_stack_create() { return (___itt_caller*)0; }
inline void itt_stack_enter(___itt_caller* ctx) {}
inline void itt_stack_leave(___itt_caller* ctx) {}
inline void itt_stack_destroy(___itt_caller* ctx) {}

inline __itt_frame_t* itt_frame_create(char const*) { return (__itt_frame_t*)0; }
inline void itt_frame_begin(__itt_frame_t* frame) {}
inline void itt_frame_end(__itt_frame_t* frame) {}
inline void itt_frame_destroy(__itt_frame_t* ctx) {}

inline int itt_mark_create(char const*) { return 0; }
inline void itt_mark_off(int mark) {}
inline void itt_mark(int mark, char const*) {}

#endif // HPX_USE_ITT

///////////////////////////////////////////////////////////////////////////////
#define HPX_ITT_SYNC_CREATE(obj, type, name)  itt_sync_create(obj, type, name)
#define HPX_ITT_SYNC_RENAME(obj, name)        itt_sync_rename(obj, name)
#define HPX_ITT_SYNC_PREPARE(obj)             itt_sync_prepare(obj)
#define HPX_ITT_SYNC_CANCEL(obj)              itt_sync_cancel(obj)
#define HPX_ITT_SYNC_ACQUIRED(obj)            itt_sync_acquired(obj)
#define HPX_ITT_SYNC_RELEASING(obj)           itt_sync_releasing(obj)
#define HPX_ITT_SYNC_RELEASED(obj)            itt_sync_released(obj)
#define HPX_ITT_SYNC_DESTROY(obj)             itt_sync_destroy(obj)

#define HPX_ITT_STACK_CREATE(ctx)             ctx = itt_stack_create()
#define HPX_ITT_STACK_CALLEE_ENTER(ctx)       itt_stack_enter(ctx)
#define HPX_ITT_STACK_CALLEE_LEAVE(ctx)       itt_stack_leave(ctx)
#define HPX_ITT_STACK_DESTROY(ctx)            itt_stack_destroy(ctx)

#define HPX_ITT_FRAME_CREATE(frame, name)     frame = itt_frame_create(name)
#define HPX_ITT_FRAME_BEGIN(frame)            itt_frame_begin(frame)
#define HPX_ITT_FRAME_END(frame)              itt_frame_end(frame)
#define HPX_ITT_FRAME_DESTROY(frame)          itt_frame_destroy(frame)

#define HPX_ITT_MARK_CREATE(mark, name)       mark = itt_mark_create(name)
#define HPX_ITT_MARK_OFF(mark)                itt_mark_off(mark)
#define HPX_ITT_MARK(mark, parameter)         itt_mark(mark, parameter)

#endif
