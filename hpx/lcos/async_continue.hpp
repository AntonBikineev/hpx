//  Copyright (c) 2007-2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

///////////////////////////////////////////////////////////////////////////////

#if !defined(HPX_LCOS_ASYNC_CONTINUE_JAN_25_2013_0824AM)
#define HPX_LCOS_ASYNC_CONTINUE_JAN_25_2013_0824AM

#include <hpx/hpx_fwd.hpp>
#include <hpx/traits.hpp>
#include <hpx/traits/promise_remote_result.hpp>
#include <hpx/traits/promise_local_result.hpp>
#include <hpx/runtime/actions/action_support.hpp>
#include <hpx/lcos/packaged_action.hpp>
#include <hpx/lcos/future.hpp>
#include <hpx/lcos/async_fwd.hpp>
#include <hpx/lcos/async_continue_fwd.hpp>
#include <hpx/util/tuple.hpp>

#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

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
            naming::id_type const& gid, Ts&&... vs)
        {
            typedef typename traits::promise_local_result<
                typename util::result_of_async_continue<Action, Ts...>::type
            >::type result_type;

            typedef typename hpx::actions::extract_action<
                Action
            >::result_type continuation_result_type;

            lcos::promise<result_type, RemoteResult> p;
            util::tuple<Ts&&...> args(std::forward<Ts>(vs)...);
            apply<Action>(
                new hpx::actions::typed_continuation<continuation_result_type>(
                    p.get_gid(), util::detail::back<Ts...>::call(std::forward<Ts>(vs)...))
              , gid, util::get<Is>(std::move(args))...);
            return p.get_future();
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Action, typename ...Ts>
    lcos::future<
        typename traits::promise_local_result<
            typename util::result_of_async_continue<Action, Ts...>::type
        >::type>
    async_continue(naming::id_type const& gid, Ts&&... vs)
    {
        typedef typename traits::promise_local_result<
            typename util::result_of_async_continue<Action, Ts...>::type
        >::type result_type;

        return detail::async_continue_r<Action, result_type>(
            typename util::detail::make_index_pack<sizeof...(Ts)-1>::type(),
            gid, std::forward<Ts>(vs)...);
    }

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
      , naming::id_type const& gid, Ts&&... vs)
    {
        return async_continue<Derived>(gid, std::forward<Ts>(vs)...);
    }
}

#endif
