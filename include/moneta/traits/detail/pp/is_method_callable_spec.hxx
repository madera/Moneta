// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#pragma once
#include <boost/mpl/bool.hpp>

// TODO: Clean this up, MPLize and PPize it and create fork on GitHub to give back to the community.
// From: https://github.com/jaredhoberock/is_call_possible
// With inspiration from: Roman Perepelitsa

// inspired by Roman Perepelitsa's presentation from comp.lang.c++.moderated
// based on the implementation here: http://www.rsdn.ru/forum/cpp/2759773.1.aspx

namespace is_call_possible_detail {
	template<typename T> struct add_reference     { typedef T& type; };
	template<typename T> struct add_reference<T&> { typedef T& type; };
} // end is_call_possible_detail

#define DEFINE_HAS_MEMBER_FUNCTION(trait_name, member_function_name)                                         \
                                                                                                             \
template<typename T, typename Signature>                                                                     \
class trait_name;                                                                                            \
                                                                                                             \
template<typename T, typename Result>                                                                        \
class trait_name<T, Result(void)>                                                                            \
{                                                                                                            \
   class yes { char m; };                                                                                    \
   class no { yes m[2]; };                                                                                   \
   struct base_mixin                                                                                         \
   {                                                                                                         \
     Result member_function_name();                                                                          \
   };                                                                                                        \
   struct base : public T, public base_mixin {};                                                             \
   template <typename U, U t>  class helper{};                                                               \
   template <typename U>                                                                                     \
   static no deduce(U*, helper<Result (base_mixin::*)(), &U::member_function_name>* = 0);                    \
   static yes deduce(...);                                                                                   \
public:                                                                                                      \
   static const bool value = sizeof(yes) == sizeof(deduce(static_cast<base*>(0)));                           \
   static const boost::mpl::bool_<value> type;                                                               \
};                                                                                                           \
                                                                                                             \
template<typename T, typename Result, typename Arg>                                                          \
class trait_name<T, Result(Arg)>                                                                             \
{                                                                                                            \
   class yes { char m; };                                                                                    \
   class no { yes m[2]; };                                                                                   \
   struct base_mixin                                                                                         \
   {                                                                                                         \
     Result member_function_name(Arg);                                                                       \
   };                                                                                                        \
   struct base : public T, public base_mixin {};                                                             \
   template <typename U, U t>  class helper{};                                                               \
   template <typename U>                                                                                     \
   static no deduce(U*, helper<Result (base_mixin::*)(Arg), &U::member_function_name>* = 0);                 \
   static yes deduce(...);                                                                                   \
public:                                                                                                      \
   static const bool value = sizeof(yes) == sizeof(deduce(static_cast<base*>(0)));                           \
   static const boost::mpl::bool_<value> type;                                                               \
};                                                                                                           \
                                                                                                             \
template<typename T, typename Result, typename Arg1, typename Arg2>                                          \
class trait_name<T, Result(Arg1,Arg2)>                                                                       \
{                                                                                                            \
   class yes { char m; };                                                                                    \
   class no { yes m[2]; };                                                                                   \
   struct base_mixin                                                                                         \
   {                                                                                                         \
     Result member_function_name(Arg1,Arg2);                                                                 \
   };                                                                                                        \
   struct base : public T, public base_mixin {};                                                             \
   template <typename U, U t>  class helper{};                                                               \
   template <typename U>                                                                                     \
   static no deduce(U*, helper<Result (base_mixin::*)(Arg1,Arg2), &U::member_function_name>* = 0);           \
   static yes deduce(...);                                                                                   \
public:                                                                                                      \
   static const bool value = sizeof(yes) == sizeof(deduce(static_cast<base*>(0)));                           \
   static const boost::mpl::bool_<value> type;                                                               \
};                                                                                                           \
                                                                                                             \
template<typename T, typename Result, typename Arg1, typename Arg2, typename Arg3>                           \
class trait_name<T, Result(Arg1,Arg2,Arg3)>                                                                  \
{                                                                                                            \
   class yes { char m; };                                                                                    \
   class no { yes m[2]; };                                                                                   \
   struct base_mixin                                                                                         \
   {                                                                                                         \
     Result member_function_name(Arg1,Arg2,Arg3);                                                            \
   };                                                                                                        \
   struct base : public T, public base_mixin {};                                                             \
   template <typename U, U t>  class helper{};                                                               \
   template <typename U>                                                                                     \
   static no deduce(U*, helper<Result (base_mixin::*)(Arg1,Arg2,Arg3), &U::member_function_name>* = 0);      \
   static yes deduce(...);                                                                                   \
public:                                                                                                      \
   static const bool value = sizeof(yes) == sizeof(deduce(static_cast<base*>(0)));                           \
   static const boost::mpl::bool_<value> type;                                                               \
};                                                                                                           \
                                                                                                             \
template<typename T, typename Result, typename Arg1, typename Arg2, typename Arg3, typename Arg4>            \
class trait_name<T, Result(Arg1,Arg2,Arg3,Arg4)>                                                             \
{                                                                                                            \
   class yes { char m; };                                                                                    \
   class no { yes m[2]; };                                                                                   \
   struct base_mixin                                                                                         \
   {                                                                                                         \
     Result member_function_name(Arg1,Arg2,Arg3,Arg4);                                                       \
   };                                                                                                        \
   struct base : public T, public base_mixin {};                                                             \
   template <typename U, U t>  class helper{};                                                               \
   template <typename U>                                                                                     \
   static no deduce(U*, helper<Result (base_mixin::*)(Arg1,Arg2,Arg3,Arg4), &U::member_function_name>* = 0); \
   static yes deduce(...);                                                                                   \
public:                                                                                                      \
   static const bool value = sizeof(yes) == sizeof(deduce(static_cast<base*>(0)));                           \
   static const boost::mpl::bool_<value> type;                                                               \
};                                                                                                           \
                                                                                                             \
template<typename T, typename Result, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5> \
class trait_name<T, Result(Arg1,Arg2,Arg3,Arg4,Arg5)>                                                        \
{                                                                                                            \
   class yes { char m; };                                                                                    \
   class no { yes m[2]; };                                                                                   \
   struct base_mixin                                                                                         \
   {                                                                                                         \
     Result member_function_name(Arg1,Arg2,Arg3,Arg4,Arg5);                                                  \
   };                                                                                                        \
   struct base : public T, public base_mixin {};                                                             \
   template <typename U, U t>  class helper{};                                                               \
   template <typename U>                                                                                     \
   static no deduce(U*, helper<Result (base_mixin::*)(Arg1,Arg2,Arg3,Arg4,Arg5), &U::member_function_name>* = 0); \
   static yes deduce(...);                                                                                   \
public:                                                                                                      \
   static const bool value = sizeof(yes) == sizeof(deduce(static_cast<base*>(0)));                           \
   static const boost::mpl::bool_<value> type;                                                               \
};                                                                                                           \
                                                                                                             \
template<typename T, typename Result, class T0, class T1, class T2, class T3, class T4, class T5>            \
class trait_name<T, Result(T0,T1,T2,T3,T4,T5)>                                                               \
{                                                                                                            \
   class yes { char m; };                                                                                    \
   class no { yes m[2]; };                                                                                   \
   struct base_mixin                                                                                         \
   {                                                                                                         \
     Result member_function_name(T0,T1,T2,T3,T4,T5);                                                         \
   };                                                                                                        \
   struct base : public T, public base_mixin {};                                                             \
   template <typename U, U t>  class helper{};                                                               \
   template <typename U>                                                                                     \
   static no deduce(U*, helper<Result (base_mixin::*)(T0,T1,T2,T3,T4,T5), &U::member_function_name>* = 0);   \
   static yes deduce(...);                                                                                   \
public:                                                                                                      \
   static const bool value = sizeof(yes) == sizeof(deduce(static_cast<base*>(0)));                           \
   static const boost::mpl::bool_<value> type;                                                               \
};                                                                                                           \
                                                                                                             \
template<typename T, typename Result, class T0, class T1, class T2, class T3, class T4, class T5, class T6>  \
class trait_name<T, Result(T0,T1,T2,T3,T4,T5,T6)>                                                            \
{                                                                                                            \
   class yes { char m; };                                                                                    \
   class no { yes m[2]; };                                                                                   \
   struct base_mixin                                                                                         \
   {                                                                                                         \
     Result member_function_name(T0,T1,T2,T3,T4,T5,T6);                                                      \
   };                                                                                                        \
   struct base : public T, public base_mixin {};                                                             \
   template <typename U, U t>  class helper{};                                                               \
   template <typename U>                                                                                     \
   static no deduce(U*, helper<Result (base_mixin::*)(T0,T1,T2,T3,T4,T5,T6), &U::member_function_name>* = 0);\
   static yes deduce(...);                                                                                   \
public:                                                                                                      \
   static const bool value = sizeof(yes) == sizeof(deduce(static_cast<base*>(0)));                           \
   static const boost::mpl::bool_<value> type;                                                               \
};

namespace is_call_possible_detail { 
   template <typename T>
   class void_exp_result {}; 

   template <typename T, typename U> 
   U const& operator,(U const&, void_exp_result<T>); 

   template <typename T, typename U> 
   U& operator,(U&, void_exp_result<T>); 

   template <typename src_type, typename dest_type> 
   struct clone_constness { typedef dest_type type; }; 

   template <typename src_type, typename dest_type> 
   struct clone_constness<const src_type, dest_type> { 
     typedef const dest_type type; 
   }; 
} 

#define DEFINE_IS_CALL_POSSIBLE(trait_name, member_function_name)                                                       \
namespace moneta { namespace traits { namespace detail {                                                                \
namespace trait_name##_detail                                                                                           \
{                                                                                                                       \
DEFINE_HAS_MEMBER_FUNCTION(has_member, member_function_name)                                                            \
}                                                                                                                       \
                                                                                                                        \
template <typename T, typename Signature>                                                                               \
struct trait_name                                                                                                       \
{                                                                                                                       \
  private:                                                                                                              \
   class yes {};                                                                                                        \
   class no { yes m[2]; };                                                                                              \
   struct derived : public T                                                                                            \
   {                                                                                                                    \
     using T::member_function_name;                                                                                     \
     no member_function_name(...) const;                                                                                \
   };                                                                                                                   \
                                                                                                                        \
   typedef typename is_call_possible_detail::clone_constness<T, derived>::type derived_type;                            \
                                                                                                                        \
   template <typename U, typename Result>                                                                               \
   struct return_value_check                                                                                            \
   {                                                                                                                    \
     static yes deduce(Result);                                                                                         \
     static no deduce(...);                                                                                             \
     static no deduce(no);                                                                                              \
     static no deduce(is_call_possible_detail::void_exp_result<T>);                                                     \
   };                                                                                                                   \
                                                                                                                        \
   template <typename U>                                                                                                \
   struct return_value_check<U, void>                                                                                   \
   {                                                                                                                    \
     static yes deduce(...);                                                                                            \
     static no deduce(no);                                                                                              \
   };                                                                                                                   \
                                                                                                                        \
   template <bool has_the_member_of_interest, typename F>                                                               \
   struct impl                                                                                                          \
   {                                                                                                                    \
     static const bool value = false;                                                                                   \
     typedef boost::mpl::bool_<value> type;                                                                             \
   };                                                                                                                   \
                                                                                                                        \
   template <typename Result, typename Arg>                                                                             \
   struct impl<true, Result(Arg)>                                                                                       \
   {                                                                                                                    \
     static typename is_call_possible_detail::add_reference<derived_type>::type test_me;                                \
     static typename is_call_possible_detail::add_reference<Arg>::type          arg;                                    \
                                                                                                                        \
     static const bool value =                                                                                          \
       sizeof(                                                                                                          \
            return_value_check<T, Result>::deduce(                                                                      \
             (test_me.member_function_name(arg), is_call_possible_detail::void_exp_result<T>())                         \
                         )                                                                                              \
            ) == sizeof(yes);                                                                                           \
     typedef boost::mpl::bool_<value> type;                                                                        \
   };                                                                                                                   \
                                                                                                                        \
   template <typename Result, typename Arg1, typename Arg2>                                                             \
   struct impl<true, Result(Arg1,Arg2)>                                                                                 \
   {                                                                                                                    \
     static typename is_call_possible_detail::add_reference<derived_type>::type test_me;                                \
     static typename is_call_possible_detail::add_reference<Arg1>::type         arg1;                                   \
     static typename is_call_possible_detail::add_reference<Arg2>::type         arg2;                                   \
                                                                                                                        \
     static const bool value =                                                                                          \
       sizeof(                                                                                                          \
            return_value_check<T, Result>::deduce(                                                                      \
             (test_me.member_function_name(arg1,arg2), is_call_possible_detail::void_exp_result<T>())                   \
                         )                                                                                              \
            ) == sizeof(yes);                                                                                           \
     typedef boost::mpl::bool_<value> type;                                                                             \
   };                                                                                                                   \
                                                                                                                        \
   template <typename Result, typename Arg1, typename Arg2, typename Arg3>                                              \
   struct impl<true, Result(Arg1,Arg2,Arg3)>                                                                            \
   {                                                                                                                    \
     static typename is_call_possible_detail::add_reference<derived_type>::type test_me;                                \
     static typename is_call_possible_detail::add_reference<Arg1>::type         arg1;                                   \
     static typename is_call_possible_detail::add_reference<Arg2>::type         arg2;                                   \
     static typename is_call_possible_detail::add_reference<Arg3>::type         arg3;                                   \
                                                                                                                        \
     static const bool value =                                                                                          \
       sizeof(                                                                                                          \
            return_value_check<T, Result>::deduce(                                                                      \
             (test_me.member_function_name(arg1,arg2,arg3), is_call_possible_detail::void_exp_result<T>())              \
                         )                                                                                              \
            ) == sizeof(yes);                                                                                           \
     typedef boost::mpl::bool_<value> type;                                                                             \
   };                                                                                                                   \
                                                                                                                        \
   template <typename Result, typename Arg1, typename Arg2, typename Arg3, typename Arg4>                               \
   struct impl<true, Result(Arg1,Arg2,Arg3,Arg4)>                                                                       \
   {                                                                                                                    \
     static typename is_call_possible_detail::add_reference<derived_type>::type test_me;                                \
     static typename is_call_possible_detail::add_reference<Arg1>::type         arg1;                                   \
     static typename is_call_possible_detail::add_reference<Arg2>::type         arg2;                                   \
     static typename is_call_possible_detail::add_reference<Arg3>::type         arg3;                                   \
     static typename is_call_possible_detail::add_reference<Arg4>::type         arg4;                                   \
                                                                                                                        \
     static const bool value =                                                                                          \
       sizeof(                                                                                                          \
            return_value_check<T, Result>::deduce(                                                                      \
             (test_me.member_function_name(arg1,arg2,arg3,arg4), is_call_possible_detail::void_exp_result<T>())         \
                         )                                                                                              \
            ) == sizeof(yes);                                                                                           \
     typedef boost::mpl::bool_<value> type;                                                                             \
   };                                                                                                                   \
                                                                                                                        \
   template <typename Result, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>                \
   struct impl<true, Result(Arg1,Arg2,Arg3,Arg4,Arg5)>                                                                  \
   {                                                                                                                    \
     static typename is_call_possible_detail::add_reference<derived_type>::type test_me;                                \
     static typename is_call_possible_detail::add_reference<Arg1>::type         arg1;                                   \
     static typename is_call_possible_detail::add_reference<Arg2>::type         arg2;                                   \
     static typename is_call_possible_detail::add_reference<Arg3>::type         arg3;                                   \
     static typename is_call_possible_detail::add_reference<Arg4>::type         arg4;                                   \
     static typename is_call_possible_detail::add_reference<Arg5>::type         arg5;                                   \
                                                                                                                        \
     static const bool value =                                                                                          \
       sizeof(                                                                                                          \
            return_value_check<T, Result>::deduce(                                                                      \
             (test_me.member_function_name(arg1,arg2,arg3,arg4,arg5), is_call_possible_detail::void_exp_result<T>())    \
                         )                                                                                              \
            ) == sizeof(yes);                                                                                           \
     typedef boost::mpl::bool_<value> type;                                                                             \
   };                                                                                                                   \
                                                                                                                        \
   template <typename Result, class T0, class T1, class T2, class T3, class T4, class T5>                               \
   struct impl<true, Result(T0,T1,T2,T3,T4,T5)>                                                                         \
   {                                                                                                                    \
     static typename is_call_possible_detail::add_reference<derived_type>::type test_me;                                \
     static typename is_call_possible_detail::add_reference<T0>::type         arg1;                                     \
     static typename is_call_possible_detail::add_reference<T1>::type         arg2;                                     \
     static typename is_call_possible_detail::add_reference<T2>::type         arg3;                                     \
     static typename is_call_possible_detail::add_reference<T3>::type         arg4;                                     \
     static typename is_call_possible_detail::add_reference<T4>::type         arg5;                                     \
     static typename is_call_possible_detail::add_reference<T5>::type         arg6;                                     \
                                                                                                                        \
     static const bool value =                                                                                          \
       sizeof(                                                                                                          \
            return_value_check<T, Result>::deduce(                                                                      \
             (test_me.member_function_name(arg1,arg2,arg3,arg4,arg5,arg6), is_call_possible_detail::void_exp_result<T>()) \
                         )                                                                                              \
            ) == sizeof(yes);                                                                                           \
     typedef boost::mpl::bool_<value> type;                                                                             \
   };                                                                                                                   \
                                                                                                                        \
   template <typename Result, class T0, class T1, class T2, class T3, class T4, class T5, class T6>                     \
   struct impl<true, Result(T0,T1,T2,T3,T4,T5,T6)>                                                                      \
   {                                                                                                                    \
     static typename is_call_possible_detail::add_reference<derived_type>::type test_me;                                \
     static typename is_call_possible_detail::add_reference<T0>::type         arg1;                                     \
     static typename is_call_possible_detail::add_reference<T1>::type         arg2;                                     \
     static typename is_call_possible_detail::add_reference<T2>::type         arg3;                                     \
     static typename is_call_possible_detail::add_reference<T3>::type         arg4;                                     \
     static typename is_call_possible_detail::add_reference<T4>::type         arg5;                                     \
     static typename is_call_possible_detail::add_reference<T5>::type         arg6;                                     \
     static typename is_call_possible_detail::add_reference<T5>::type         arg7;                                     \
                                                                                                                        \
     static const bool value =                                                                                          \
       sizeof(                                                                                                          \
            return_value_check<T, Result>::deduce(                                                                      \
             (test_me.member_function_name(arg1,arg2,arg3,arg4,arg5,arg6,arg7), is_call_possible_detail::void_exp_result<T>()) \
                         )                                                                                              \
            ) == sizeof(yes);                                                                                           \
     typedef boost::mpl::bool_<value> type;                                                                             \
   };                                                                                                                   \
                                                                                                                        \
  public:                                                                                                               \
    static const bool value = impl<trait_name##_detail::has_member<T,Signature>::value, Signature>::value;              \
    typedef boost::mpl::bool_<value> type;                                                                              \
};                                                                                                                      \
}}}
