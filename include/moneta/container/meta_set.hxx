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
#include "../detail/keywords.hxx"
#include "../traits/detail/has_member_trait.hxx"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/optional.hpp>

namespace moneta { namespace container {

	namespace detail {
		
		//
		// Constructor hack
		//
		template <class InstanceType, class ArgumentType>
		class sliced_constructor {
			InstanceType& _instance;
			ArgumentType& _argument;
		public:
			sliced_constructor(InstanceType& instance, ArgumentType& argument)
			 : _instance(instance), _argument(argument) {}

			template <class Base>
			void operator()(const Base&) const {
				BOOST_MPL_ASSERT((boost::is_base_and_derived<Base, InstanceType>));
				Base* base_ptr = &_instance;
				new (base_ptr) Base(_argument); // Placement new on sliced entity.
			}
		};

		//
		// For entry to_string'ification.
		//

		DEFINE_HAS_MEMBER_TRAIT(to_string)

		template <class Entity>
		typename boost::enable_if<
			has_member_to_string<Entity>,
			std::string
		>::type
		to_string_or_empty(const Entity& entity) {
			return entity.to_string();
		}

		template <class Entity>
		typename boost::disable_if<
			has_member_to_string<Entity>,
			std::string
		>::type
		to_string_or_empty(const Entity&) {
			return "";
		}

		template <class InstanceType, class OstreamType = std::basic_ostream<char> >
		class sliced_to_string_or_empty_ostreamer {
			const InstanceType& _instance;
			OstreamType& _output;
			std::string _separator;
			const size_t _size;
			size_t _iteration;
		public:
			sliced_to_string_or_empty_ostreamer(
				const InstanceType& instance,
				OstreamType& output,
				const std::string separator = ", ",
				const size_t size = -1
			) : _instance(instance), _output(output), _separator(separator),
			    _size(size), _iteration(0) {}

			template <class Base>
			void operator()(const Base&) {
				BOOST_MPL_ASSERT((boost::is_base_and_derived<Base, InstanceType>));
				const Base& base = _instance;
				_output << to_string_or_empty(base);

				if (_size == size_t(-1) || ++_iteration < _size) {
					_output << _separator;
				}
			}

			const OstreamType& get_ostream() const {
				return _output;
			}
		};

		DEFINE_HAS_NESTED_TYPE(get_index)

		template <class Sequence>
		struct meta_set_impl;

		template <class Sequence>
		struct meta_set_bases : boost::mpl::transform<
			Sequence,
			boost::mpl::apply_wrap1<
				boost::mpl::_1,
				meta_set_impl<Sequence> // Base type
			>
		> {};

		template <class T>
		struct get_entry {
			typedef typename T::entry type;
		};

		template <class Sequence>
		struct meta_set_impl : boost::mpl::inherit_linearly<
			typename meta_set_bases<Sequence>::type,
			boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2>
		>::type {
			typedef meta_set_impl this_type;
			typedef typename meta_set_bases<Sequence>::type bases_type;

			typedef typename boost::mpl::transform<
				bases_type,
				get_entry<boost::mpl::_1>
			>::type entries_type;

			struct entry : boost::mpl::inherit_linearly<
				entries_type,
				boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2>
			>::type {
				entry() {}

				template <class Entity>
				entry(Entity& entity) {
					boost::mpl::for_each<entries_type>(
						sliced_constructor<entry, Entity>(*this, entity)
					);
				}

				std::string to_string() const {
					std::ostringstream oss;
					boost::mpl::for_each<entries_type>(
						sliced_to_string_or_empty_ostreamer<entry>(
							*this, oss, " || ",
							boost::mpl::size<entries_type>::value
						)
					);

					return oss.str();
				}

			};

			// WARNING: Used for slow O(n) searches in first implementation.
			//          If this proves useful in others scenarios, remove this comment.
			struct is_equal {
				const entry& _model;

				is_equal(const entry& model)
				 : _model(model) {}

				bool operator()(const entry& other) {
					return false;//!(_model < other) && !(other < _model);
				}
			};

			template <typename T>
			struct get_index : boost::mpl::apply<
				typename T::get_index,
				entry
			> {};

			typedef typename boost::mpl::copy_if<
				bases_type,
				has_nested_type_get_index<boost::mpl::_1>,
				boost::mpl::inserter<
					boost::mpl::vector<>,
					boost::mpl::push_back<
						boost::mpl::_1,
						get_index<boost::mpl::_2>
					>
				>
			>::type index_vector;

			struct sequenced_index_tag;

			typedef boost::multi_index::multi_index_container<
				entry,
				typename boost::mpl::push_front<
					index_vector,
					// Default index 0 is sequential index.
					boost::multi_index::random_access<
						boost::multi_index::tag<sequenced_index_tag>
					>
				>::type
			> container_type;

			//
			// External component's access.
			//
			// protected:
			//
			template <class This>
			static this_type& get(This* _this) {
				return *static_cast<this_type*>(_this);
			}

			template <class This>
			static const this_type& get(const This* _this) {
				return *static_cast<const this_type*>(_this);
			}

			template <class This>
			static container_type& container(This* _this) {
				return get(_this)._container;
			}

			template <class This>
			static const container_type& container(const This* _this) {
				return get(_this)._container;
			}

			//
			// public:
			//

			//
			// Insert into default sequential index.
			//
			void insert(const entry& entry) {
				_container.MONETA_INTRA_TEMPLATE_KEYWORD get<sequenced_index_tag>().push_back(entry);
				std::cerr << "Inserted: " << entry.to_string() << std::endl;
			}

			template <class Entity>
			void insert(const Entity& x) {
				insert(entry(x));
			}

			//
			// Replace. TODO: Document.
			//
			void replace(const entry& entry_) {
				// FIXME: SLOW: XXX
				// TODO: Reach out to hashed O(1) impl if available.

				auto b = _container.begin();
				auto e = _container.end();

				entry x;
				entry y;

				std::find_if(b, e, is_equal(entry_));
				//auto itr = std::find(_container.begin(), _container.end(), entry);
				//if (itr != c.end()) {
				//	c.replace(itr, entry);
				//}

				std::cerr << "Replaced: " << entry_.to_string() << std::endl;
			}

			template <class Entity>
			void replace(const Entity& x) {
				replace(entry(x));
			}

			// Erase from default sequential index.
			//void erase(entry& entry) {
			//	auto& index = _container.get<sequenced_index_tag>();
			//	auto itr = index.find(entry);
			//	if (itr != index.end()) {
			//		index.erase(itr);
			//		std::cerr << "Erased: " << entry.to_string() << std::endl;
			//	}
			//}

			size_t size() const {
				return _container.size();
			}

			const entry& at(const size_t index) {
				return _container.MONETA_INTRA_TEMPLATE_KEYWORD get<sequenced_index_tag>().at(index);
			}

			const entry& operator[](const size_t index) {
				return at(index);
			}

			// TODO: Surround by debug ifdefs.
			// TODO: Convert to ostream operator.
			template <class Ostream>
			Ostream& debug_dump(Ostream& output) {
				for (size_t i=0; i<size(); ++i) {
					output << at(i).to_string() << '\n';
				}

				return output;
			}

		private:
			container_type _container;
		};
	
	} // detail

	template <
		class T0,
		class T1 = boost::mpl::na, class T2 = boost::mpl::na, class T3 = boost::mpl::na,
		class T4 = boost::mpl::na, class T5 = boost::mpl::na, class T6 = boost::mpl::na,
		class T7 = boost::mpl::na, class T8 = boost::mpl::na, class T9 = boost::mpl::na
	>
	struct meta_set : detail::meta_set_impl<
		boost::mpl::vector<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>
	> {};
}}
