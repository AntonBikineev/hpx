//  Copyright (c) 2007-2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

///////////////////////////////////////////////////////////////////////////////

#if !defined(HPX_LCOS_ASYNC_CONTINUE_FWD_JAN_25_2013_0828AM)
#define HPX_LCOS_ASYNC_CONTINUE_FWD_JAN_25_2013_0828AM

#include <hpx/hpx_fwd.hpp>
#include <hpx/traits.hpp>
#include <hpx/util/move.hpp>
#include <hpx/util/decay.hpp>
#include <hpx/util/result_of.hpp>
#include <hpx/util/detail/pack.hpp>
#include <hpx/lcos/future.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace hpx { namespace util
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename Action, typename ...Ts>
    struct result_of_async_continue
        : actions::detail::remote_action_result<
            typename util::result_of<
                typename util::decay<
                    typename util::detail::back<Ts...>::type
                >::type(naming::id_type,
                    typename hpx::actions::extract_action<
                        Action
                    >::remote_result_type)
            >::type
        >
    {};
}}

namespace hpx
{
    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <
            typename Action, typename RemoteResult,
            std::size_t ...Is, typename ...Ts>
        lcos::future<
            typename traits::promise_local_result<
                typename util::result_of_async_continue<Action, Ts...>::type
            >::type>
        async_continue_r(util::detail::pack_c<std::size_t, Is...>, 
            naming::id_type const& gid, Ts&&... ts);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Action, typename ...Ts>
    lcos::future<
        typename traits::promise_local_result<
            typename util::result_of_async_continue<Action, Ts...>::type
        >::type>
    async_continue(naming::id_type const& gid, Ts&&... ts);

    ///////////////////////////////////////////////////////////////////////////
    template <
        typename Component, typename Result, typename Arguments,
        typename Derived, typename ...Ts>
    lcos::future<
        typename traits::promise_local_result<
            typename util::result_of_async_continue<Derived, Ts...>::type
        >::type>
    async_continue(
        hpx::actions::action<Component, Result, Arguments, Derived> /*act*/
      , naming::id_type const& gid, Ts&&... ts);
}

#endif
