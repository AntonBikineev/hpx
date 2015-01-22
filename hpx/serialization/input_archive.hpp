//  Copyright (c) 2014 Thomas Heller
//  Copyright (c) 2015 Anton Bikineev
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HPX_SERIALIZATION_INPUT_ARCHIVE_HPP
#define HPX_SERIALIZATION_INPUT_ARCHIVE_HPP

#include <hpx/config.hpp>

#include <hpx/serialization/archive.hpp>
#include <hpx/serialization/input_container.hpp>
#include <hpx/serialization/polymorphic_nonintrusive_factory.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>

namespace hpx { namespace serialization {

    struct HPX_ALWAYS_EXPORT input_archive
      : archive<input_archive>
    {
        typedef archive<input_archive> base_type;

        typedef
            std::map<std::size_t, HPX_STD_UNIQUE_PTR<detail::ptr_helper> >
            pointer_tracker;

        template <typename Container>
        static HPX_STD_UNIQUE_PTR<container> make_container(Container & buffer)
        {
            return HPX_STD_UNIQUE_PTR<container>(new input_container<Container>(buffer, 0));
        }

        template <typename Container>
        input_archive(Container & buffer)
          : base_type(0, make_container(buffer))
        {}

        template <typename T>
        void invoke_impl(T & t)
        {
            load(t);
        }

        template <typename T>
        typename boost::disable_if<
            boost::is_integral<T>
        >::type
        load(T & t)
        {
            load_bitwise(t,
                typename hpx::traits::is_bitwise_serializable<T>::type());
        }

        template <typename T>
        void load_bitwise(T & t, boost::mpl::false_)
        {
            load_nonintrusively_polymorphic(t,
                hpx::traits::is_nonintrusive_polymorphic<T>());
        }

        template <typename T>
        void load_bitwise(T & t, boost::mpl::true_)
        {
            BOOST_STATIC_ASSERT_MSG(!boost::is_abstract<T>::value,
                "Can not bitwise serialize a class that is abstract");
            if(disable_array_optimization())
            {
                serialize(*this, t, 0);
            }
            else
            {
                load_binary(&t, sizeof(t));
            }
        }

        template <class T>
        void load_nonintrusively_polymorphic(T& t, boost::mpl::false_)
        {
            serialize(*this, t, 0);
        }

        template <class T>
        void load_nonintrusively_polymorphic(T& t, boost::mpl::true_)
        {
            polymorphic_nonintrusive_factory::instance().load(*this, t);
        }

        template <typename T>
        typename boost::enable_if<
            boost::is_integral<T>
        >::type
        load(T & t)
        {
            load_integral(t,
                typename boost::is_unsigned<T>::type());
        }

        template <typename T>
        void load_integral(T & val, boost::mpl::false_)
        {
            boost::int64_t l;
            load_impl(l);
            val = static_cast<T>(l);
        }

        template <typename T>
        void load_integral(T & val, boost::mpl::true_)
        {
            boost::uint64_t ul;
            load_impl(ul);
            val = static_cast<T>(ul);
        }

        void load(float & f)
        {
            load_binary(&f, sizeof(float));
        }

        void load(double & d)
        {
            load_binary(&d, sizeof(double));
        }

        void load(char & c)
        {
            load_binary(&c, sizeof(char));
        }

        void load(bool & b)
        {
            HPX_ASSERT(0 == static_cast<int>(b) || 1 == static_cast<int>(b));
            load_binary(&b, sizeof(bool));
        }

        void load_impl(boost::int64_t & l);
        void load_impl(boost::uint64_t & ul);
        void load_binary(void * address, std::size_t count);

        void register_pointer(std::size_t pos, HPX_STD_UNIQUE_PTR<detail::ptr_helper> helper)
        {
            HPX_ASSERT(pointer_tracker_.find(pos) == pointer_tracker_.end());

            pointer_tracker_.insert(std::make_pair(pos, std::move(helper)));
        }

        template <typename Helper>
        Helper & tracked_pointer(std::size_t pos)
        {
            pointer_tracker::iterator it = pointer_tracker_.find(pos);
            HPX_ASSERT(it != pointer_tracker_.end());

            return static_cast<Helper &>(*it->second);
        }

        pointer_tracker pointer_tracker_;
    };

    void register_pointer(input_archive & ar, std::size_t pos, HPX_STD_UNIQUE_PTR<detail::ptr_helper> helper);

    template <typename Helper>
    Helper & tracked_pointer(input_archive & ar, std::size_t pos)
    {
        return ar.tracked_pointer<Helper>(pos);
    }
}}

#endif