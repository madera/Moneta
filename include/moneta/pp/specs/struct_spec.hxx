#ifndef BOOST_PP_IS_ITERATING
#	ifndef STRUCT_SPEC_HEADER
#		define STRUCT_SPEC_HEADER
#		include "../moneta_pp.hxx"
#		include <boost/optional.hpp>
using boost::optional;
#		define BOOST_PP_ITERATION_LIMITS (0, 0)
#		define BOOST_PP_FILENAME_1 "moneta/pp/specs/struct_spec.hxx"
#		include BOOST_PP_ITERATE()
#	endif // STRUCT_SPEC_HEADER
#else

//
// BOOST_PP_IS_ITERATING
//

//
// ENTITY_DEFINITION: The whole entity definition.
// CLASS_MEMBER_DATATYPES: (int)(std::string)
// CLASS_MEMBER_NAMES: (ID)(Name)
// TODO: Complete this list...
//

MONETA_PP_NAMESPACE_BEGIN(CLASS_NAMESPACE)

	struct CLASS_NAME {
		typedef CLASS_NAME this_type;

		MONETA_PP_MEMBER_DECLARATIONS(CLASS_MEMBER_DATATYPES, CLASS_MEMBER_NAMES)

		MONETA_PP_DEFAULT_CONSTRUCTOR(CLASS_NAME)
	};

MONETA_PP_NAMESPACE_END(CLASS_NAMESPACE)

#undef STRUCT_SPEC_HEADER
#endif // BOOST_PP_IS_ITERATING
