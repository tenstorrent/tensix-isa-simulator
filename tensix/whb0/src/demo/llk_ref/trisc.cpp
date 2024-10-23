
#include <functional>

#include "core/tensix.hpp"
#include "core/thread.hpp"

#include "api/ckernel_core.h"

#include "trisc.hpp"

namespace ronin {
namespace iss {
namespace whb0 {
namespace demo {
namespace llk_ref {

using core::Tensix;
using core::ThreadType;
using core::ThreadSync;
using core::Thread;

using llk::api::ThreadContext;
using llk::api::ThreadBase;

//
//    Trisc
//

Trisc::Trisc(
        ThreadType thread_type, 
        Tensix *tensix, 
        ThreadSync *thread_sync,
        ThreadContext *thread_context,
        std::function<void ()> main):
            ThreadBase(thread_type, tensix, thread_sync, thread_context),
            m_main(main) { }

Trisc::~Trisc() { }

void Trisc::run() {
    m_main();
}

//
//    Unpack
//

Unpack::Unpack(
        Tensix *tensix, 
        ThreadSync *thread_sync,
        ThreadContext *thread_context,
        std::function<void ()> main):
            Trisc(ThreadType::Unpack, tensix, thread_sync, thread_context, main) { }

Unpack::~Unpack() { }

//
//    Math
//

Math::Math(
        Tensix *tensix, 
        ThreadSync *thread_sync,
        ThreadContext *thread_context,
        std::function<void ()> main):
            Trisc(ThreadType::Math, tensix, thread_sync, thread_context, main) { }

Math::~Math() { }

//
//    Pack
//

Pack::Pack(
        Tensix *tensix, 
        ThreadSync *thread_sync,
        ThreadContext *thread_context,
        std::function<void ()> main):
            Trisc(ThreadType::Pack, tensix, thread_sync, thread_context, main) { }

Pack::~Pack() { }

} // namespace llk_ref
} // namespace demo
} // namespace whb0
} // namespace iss
} // namespace ronin

