#ifndef ULIBPP_UPP_GUESS_H_
#define ULIBPP_UPP_GUESS_H_

/*

	_upp_guess.h

	Compiler, OS and HW guessing for source library

	Kuba Feb.2006
	Part of ULIB++

	Taken from ULIB

*/

/* ================================================================================================ *
 *		Guessing OS...																					*
 * ================================================================================================ */

#if !defined(ULIBPP_OS_PLATFORM)

#if defined(__linux__)
#define ULIBPP_OS_PLATFORM				ULIBPP_OS_PLATFORM_LINUX

#elif defined(_WIN64)
#define ULIBPP_OS_PLATFORM				ULIBPP_OS_PLATFORM_WIN_NT64

#elif defined(_WIN32) || defined(__NT__)
/* beware: ICC for itanium defines both WIN32 & WIN64! */
#define ULIBPP_OS_PLATFORM				ULIBPP_OS_PLATFORM_WIN_NT

#elif defined(__MIC__)
#define ULIBPP_OS_PLATFORM				ULIBPP_OS_PLATFORM_K1OM

#elif defined(__sun) || defined(__sun__) || defined(sun)
/* check __SVR4 to determine between solaris & sunos if necessary */
#define ULIBPP_OS_PLATFORM				ULIBPP_OS_PLATFORM_SUNOS
#error Solaris platform is no more supported

#elif defined(__FreeBSD__)
#define ULIBPP_OS_PLATFORM				ULIBPP_OS_PLATFORM_FREEBSD
#error FreeBSD platform is no more supported

#elif defined(__CYGWIN__) || defined(__CYGWIN32__)
#define ULIBPP_OS_PLATFORM				ULIBPP_OS_PLATFORM_CYGWIN
#error Cygwin platform is no more supported

#else
#error "### Cannot determine operating environment."
#endif

#endif /* ULIBPP_OS_PLATFORM */

/* ================================================================================================ *
 *		Guessing HW...																					*
 * ================================================================================================ */

#if !defined(ULIBPP_HW_PLATFORM)

#undef ULIBPP_HW_PLATFORM_BITS

#if defined(__MIC__)

#define ULIBPP_HW_PLATFORM					ULIBPP_HW_PLATFORM_KNC
#define ULIBPP_HW_PLATFORM_BITS				64
#define ULIBPP_ATOMIC_INTEGRAL_LOCK_FREE	2
#define ULIBPP_ATOMIC_ADDRESS_LOCK_FREE		2

#elif defined(__AVX512F__) && defined(__AVX512CD__) &&						\
	defined(__AVX512ER__) && defined(__AVX512PF__)
#define ULIBPP_HW_PLATFORM					ULIBPP_HW_PLATFORM_KNL
#define ULIBPP_HW_PLATFORM_BITS				64
#define ULIBPP_ATOMIC_INTEGRAL_LOCK_FREE	2
#define ULIBPP_ATOMIC_ADDRESS_LOCK_FREE		2

#elif defined(_M_X64) || defined(_M_AMD64) ||						\
	defined(__x86_64) || defined(__x86_64__)
#define ULIBPP_HW_PLATFORM					ULIBPP_HW_PLATFORM_X64
#define ULIBPP_HW_PLATFORM_BITS				64
#define ULIBPP_ATOMIC_INTEGRAL_LOCK_FREE	2
#define ULIBPP_ATOMIC_ADDRESS_LOCK_FREE		2

#elif defined(_M_IA64) ||										\
	defined(__ia64) || defined(__ia64__)
#define ULIBPP_HW_PLATFORM					ULIBPP_HW_PLATFORM_IA64
#define ULIBPP_HW_PLATFORM_BITS				64
#define ULIBPP_ATOMIC_INTEGRAL_LOCK_FREE	2	   // ?
#define ULIBPP_ATOMIC_ADDRESS_LOCK_FREE		2	   // ?
#error IA-64 platform is no more supported

#elif defined(__i386) || defined(__i386__) || defined(__386__) ||		\
	defined(__i686) || defined(__i686__) ||						\
	defined(_X86_) || defined(__X86__) ||						\
	defined(_M_IX86) || defined(_M_I386)
#define ULIBPP_HW_PLATFORM					ULIBPP_HW_PLATFORM_IA32
#define ULIBPP_HW_PLATFORM_BITS				32
#define ULIBPP_ATOMIC_INTEGRAL_LOCK_FREE	2	   // 1? for LL
#define ULIBPP_ATOMIC_ADDRESS_LOCK_FREE		2

#elif defined(__sparcv9) || defined(__sparcv9__)
#define ULIBPP_HW_PLATFORM					ULIBPP_HW_PLATFORM_SPARC64
#define ULIBPP_HW_PLATFORM_BITS				64
#define ULIBPP_ATOMIC_INTEGRAL_LOCK_FREE	1	   // ?
#define ULIBPP_ATOMIC_ADDRESS_LOCK_FREE		1
#error SPARCv9 platform is no more supported

#elif defined(__sparc) || defined(__sparc__)
#define ULIBPP_HW_PLATFORM					ULIBPP_HW_PLATFORM_SPARC
#define ULIBPP_HW_PLATFORM_BITS				32
#define ULIBPP_ATOMIC_INTEGRAL_LOCK_FREE	1	   // ?
#define ULIBPP_ATOMIC_ADDRESS_LOCK_FREE		1
#error SPARCv8 platform is no more supported

#else
#error "### Cannot determine HW platform."
#endif

#endif /* ULIBPP_HW_PLATFORM */

/* ================================================================================================ *
 *		Universal tests...	 																		*
 *      https://en.cppreference.com/w/cpp/feature_test												*
 * ================================================================================================ */

#ifdef __has_include
#if __has_include(<filesystem>)
#define ULIBPP_CCLIB_FS			1
#else
#define ULIBPP_CCLIB_FS			0
#endif
#if __has_include(<regex>)
#define ULIBPP_CCLIB_REGEX			1
#else
#define ULIBPP_CCLIB_REGEX			0
#endif
#endif

// C++11
// attrs
#ifdef __has_cpp_attribute
#if __has_cpp_attribute(carries_dependency)
#define ULIBPP_CC_ATTR_CARRIES_DEPENDENCY	1
#endif
#if __has_cpp_attribute(noreturn)
#define ULIBPP_CC_ATTR_NORETURN				1
#endif
#endif
// language
#if __cpp_alias_templates>=200704
#define ULIBPP_CC_TEMPLATE_ALIASES			1
#endif
#if __cpp_attributes>=200809
#define ULIBPP_CC_ATTRIBUTES				1
#endif
#if __cpp_constexpr>=200704
#define ULIBPP_CC_CONSTEXPR					1
#endif
#if __cpp_decltype>=200707
#define ULIBPP_CC_DECLTYPE					1
#endif
#if __cpp_delegating_constructors>=200604
#define ULIBPP_CC_DELEGATING_CONSTRUCTORS	1
#endif
#if __cpp_inheriting_constructors>=200802
#define ULIBPP_CC_INHERITINING_CONSTRUCTORS	1
#endif
#if __cpp_initializer_lists>=200806
#define ULIBPP_CC_INITIALIZER_LIST			1
#endif
#if __cpp_lambdas>=200907
#define ULIBPP_CC_LAMBDAS					1
#endif
#if __cpp_nsdmi>=200809
#define ULIBPP_CC_NSDMI						1
#endif
#if __cpp_range_based_for>=200907
#define ULIBPP_CC_RANGE_BASED_FOR			1
#endif
#if __cpp_raw_strings>=200710
#define ULIBPP_CC_RAW_STRINGS				1
#endif
#if __cpp_ref_qualifiers>=200710
#define ULIBPP_CC_REF_QUALIFIERS			1
#endif
#if __cpp_rvalue_references>=200610
#define ULIBPP_CC_RVALUE					1
#endif
#if __cpp_static_assert>=200410
#define ULIBPP_CC_STATIC_ASSERT				1
#endif
#if __cpp_threadsafe_static_init>=200806
#define ULIBPP_CC_CONCUR_INIT_DESTR			1
#endif
#if __cpp_unicode_characters>=200704
#define ULIBPP_CC_NEW_CHAR_TYPES			1
#endif
#if __cpp_unicode_literals>=200710
#define ULIBPP_CC_UNICODE_STRINGS			1
#endif
#if __cpp_user_defined_literals>=200809
#define ULIBPP_CC_USERDEF_LITERALS			1
#endif
#if __cpp_variadic_templates>=200704
#define ULIBPP_CC_VARIADIC_TEMPLATES		1
#endif

// C++14
// attrs
#ifdef __has_cpp_attribute
#if __has_cpp_attribute(deprecated)>=201309
#define ULIBPP_CC_DEPRECATED_ATTR			1
#else
#define ULIBPP_CC_DEPRECATED_ATTR			0
#endif
#endif
// language
#if __cpp_aggregate_nsdmi>=201304
#define ULIBPP_CC_AGGR_NSDMI				1
#endif
#if __cpp_binary_literals>=201304
#define ULIBPP_CC_BINARY_LITERALS			1
#endif
#if __cpp_constexpr>=201304
#define ULIBPP_CC_RELAX_CONSTEXPR			1
#endif
#if __cpp_decltype_auto>=201304 || __cpp_return_type_deduction>=201304
#define ULIBPP_CC_RETURN_TYPE_DEDUCTION		1
#endif
#if __cpp_generic_lambdas>=201304
#define ULIBPP_CC_GENERIC_LAMBDAS			1
#endif
#if __cpp_init_captures>=201304
#define ULIBPP_CC_INIT_LAMBDA_CAPTURES		1
#endif
#if __cpp_sized_deallocation>=201309
#define ULIBPP_CC_SIZED_DEALLOC				1
#endif
#if __cpp_variable_templates>=201304
#define ULIBPP_CC_VARIABLE_TEMPLATES		1
#endif
// library
#if defined(__has_include) && __has_include(<chrono>) && __has_include(<string>)
#include <chrono>
#include <string>
#if __cpp_lib_chrono_udls>=201304 && __cpp_lib_string_udls>=201304
#define ULIBPP_CCLIB_UDL_CHRONO_STRING		1
#endif
#endif
#if defined(__has_include) && __has_include(<complex>)
#include <complex>
#if __cpp_lib_complex_udls>=201309
#define ULIBPP_CCLIB_COMPLEX_UDLS			1
#endif
#endif
#if defined(__has_include) && __has_include(<utility>)
#include <utility>
#if __cpp_lib_exchange_function>=201304
#define ULIBPP_CCLIB_EXCHANGE				1
#endif
#if __cpp_lib_integer_sequence>=201304
#define ULIBPP_CCLIB_INT_SEQ				1
#endif
#endif
#if defined(__has_include) && __has_include(<map>)
#include <map>
#if __cpp_lib_generic_associative_lookup>=201304
#define ULIBPP_CCLIB_HETERO_ASSOC_LOOKUP	1
#endif
#endif
#if defined(__has_include) && __has_include(<type_traits>)
#include <type_traits>
#if __cpp_lib_integral_constant_callable>=201304
#define ULIBPP_CCLIB_IMPR_INT_CONST			1
#endif
#if __cpp_lib_is_final>=201402
#define ULIBPP_CCLIB_IS_FINAL				1
#endif
#if __cpp_lib_is_null_pointer>=201309
#define ULIBPP_CCLIB_IS_NULL_PTR			1
#endif
#if __cpp_lib_transformation_trait_aliases>=201304
#define ULIBPP_CCLIB_TRANS_TRAIT_REDUX		1
#endif
#endif
#if defined(__has_include) && __has_include(<iterator>)
#include <iterator>
#if __cpp_lib_make_reverse_iterator>=201402
#define ULIBPP_CCLIB_MAKE_REVERS_ITER		1
#endif
#if __cpp_lib_null_iterators>=201304
#define ULIBPP_CCLIB_NULL_FWD_ITER			1
#endif
#endif
#if defined(__has_include) && __has_include(<memory>)
#include <memory>
#if __cpp_lib_make_unique>=201304
#define ULIBPP_CCLIB_MAKE_UNIQUE			1
#endif
#endif
#if defined(__has_include) && __has_include(<iomanip>)
#include <iomanip>
#if __cpp_lib_quoted_string_io>=201304
#define ULIBPP_CCLIB_QUOTED					1
#endif
#endif
#if defined(__has_include) && __has_include(<functional>)
#include <functional>
#if __cpp_lib_result_of_sfinae>=201210
#define ULIBPP_CCLIB_RESULT_OF				1
#endif
#if __cpp_lib_transparent_operators>=201210
#define ULIBPP_CCLIB_TRANSP_OP_FTORS		1
#endif
#endif
#if defined(__has_include) && __has_include(<algorithm>)
#include <algorithm>
#if __cpp_lib_robust_nonmodifying_seq_ops>=201304
#define ULIBPP_CCLIB_DUAL_RANGE				1
#endif
#endif
#if defined(__has_include) && __has_include(<shared_mutex>)
#include <shared_mutex>
#if __cpp_lib_shared_timed_mutex>=201402
#define ULIBPP_CCLIB_SHR_TIMED_MUTEX		1
#endif
#endif
#if defined(__has_include) && __has_include(<tuple>)
#include <tuple>
#if __cpp_lib_tuple_element_t>=201402
#define ULIBPP_CCLIB_TUPLE_ELEM_T			1
#endif
#if __cpp_lib_tuples_by_type>=201304
#define ULIBPP_CCLIB_GET					1
#endif
#endif

// C++17
// attrs
#ifdef __has_cpp_attribute
#if __has_cpp_attribute(fallthrough)>=201603
#define ULIBPP_CC_FALLTHROUGH_ATTR			1
#endif
#if __has_cpp_attribute(maybe_unused)>=201603
#define ULIBPP_CC_MAYBEUNUSED_ATTR			1
#endif
#if __has_cpp_attribute(nodiscard)>=201603
#define ULIBPP_CC_NODISCARD_ATTR			1
#endif
#endif
// language
#if __cpp_aggregate_bases>=201603
#define ULIBPP_CC_AGGR_BASE_CLASS_INIT		1
#endif
#if __cpp_aligned_new>=201606
#define ULIBPP_CC_ALIGNED_NEW				1
#endif
#if __cpp_capture_star_this>=201603
#define ULIBPP_CC_LAMBDA_CAPTURE_DEREFTHIS	1
#endif
#if __cpp_constexpr>=201603
#define ULIBPP_CC_CONSTEXPR_LAMBDA			1
#endif
#if __cpp_deduction_guides>=201611
#define ULIBPP_CC_TEMPL_DEDUCTION_FOR_CLASS	1
#endif
#if __cpp_namespace_attributes>=201411 && __cpp_enumerator_attributes>=201411
#define ULIBPP_CC_ATTRS_NAMESPACES_ENUMS	1
#endif
#if __cpp_fold_expressions>=201603
#define ULIBPP_CC_FOLD_EXPR					1
#endif
#if __cpp_guaranteed_copy_elision>=201606
#define ULIBPP_CC_GUARANTEED_COPY_ELISION	1
#endif
#if __cpp_hex_float>=201603
#define ULIBPP_CC_HEXA_FLOAT_LITERALS		1
#endif
#if __cpp_if_constexpr>=201606
#define ULIBPP_CC_CONSTEXPR_IF				1
#endif
#if __cpp_inheriting_constructors>=201511
#define ULIBPP_CC_NEW_INHERIT_CONSTR		1
#endif
#if __cpp_inline_variables>=201606
#define ULIBPP_CC_INLINE_VARIABLES			1
#endif
#if __cpp_noexcept_function_type>=201510
#define ULIBPP_CC_EXCSPEC_IN_TYPESYS		1
#endif
#if __cpp_nontype_template_args>=201411
#define ULIBPP_CC_MORE_NONTYPE_TEMPL_ARGS	1
#endif
#if __cpp_nontype_template_parameter_auto>=201606
#define ULIBPP_CC_AUTO_FOR_NOTYPE_TEMPL_ARG	1
#endif
#if __cpp_range_based_for>=201603
#define ULIBPP_CC_DIFF_BEGIN_END_FOR		1
#endif
#if __cpp_static_assert>=201411
#define ULIBPP_CC_TERSE_STATIC_ASSERT		1
#endif
#if __cpp_structured_bindings>=201606
#define ULIBPP_CC_STRUCTURED_BINDING		1
#endif
#if __cpp_template_template_args>=201611
#define ULIBPP_CC_MATCH_TEMPL_TEMPL_ARGS	1
#endif
#if __cpp_variadic_using>=201611
#define ULIBPP_CC_PACK_EXP_IN_USING			1
#endif
// library
#if defined(__has_include) && __has_include(<memory>)
#include <memory>
#if __cpp_lib_addressof_constexpr>=201603
#define ULIBPP_CCLIB_ADDRESSOF_CONSTEXPR	1
#endif
#if __cpp_lib_allocator_traits_is_always_equal>=201411
#define ULIBPP_CCLIB_ALLOC_TR_EQ			1
#endif
#endif
#if defined(__has_include) && __has_include(<any>)
#include <any>
#if __cpp_lib_any>=201606
#define ULIBPP_CCLIB_ANY					1
#endif
#endif
#if defined(__has_include) && __has_include(<tuple>)
#include <tuple>
#if __cpp_lib_apply>=201603
#define ULIBPP_CCLIB_APPLY					1
#endif
#endif
#if defined(__has_include) && __has_include(<array>)
#include <array>
#if __cpp_lib_array_constexpr>=201603
#define ULIBPP_CCLIB_ARRAY_CONSTEXPR		1
#endif
#endif
#if defined(__has_include) && __has_include(<utility>)
#include <utility>
#if __cpp_lib_as_const>=201510
#define ULIBPP_CCLIB_AS_CONST				1
#endif
#endif


#if defined(__has_include) && __has_include(<algorithm>)
#include <algorithm>
#if __cpp_lib_parallel_algorithm>=201603			// not reliable test yet
#define ULIBPP_CCLIB_PARA_ALG					1
#endif
#endif

// C++20
#if __cpp_aggregate_paren_init>=201902
#define ULIBPP_CC_DIRECT_AGR_INIT			1
#endif
#if __cpp_char8_t>=201811
#define ULIBPP_CC_CHAR8_T					1
#endif
#if __cpp_conditional_explicit>=201806
#define ULIBPP_CC_COND_EXPLICIT				1
#endif
#if __cpp_coroutines>=201902
#define ULIBPP_CC_COROUTINES				1
#endif
#if __cpp_impl_destroying_delete>=201806
#define ULIBPP_CC_DESTROY_DELETE			1
#endif
#if __cpp_impl_three_way_comparison>=201711
#define ULIBPP_CC_3WAY_COMP					1
#endif
#if __cpp_nontype_template_parameter_class>=201806
#define ULIBPP_CC_CLASS_NONTYPE_TEMPL		1
#endif

#if defined(__has_include) && __has_include(<atomic>)
#include <atomic>
#if __cpp_lib_atomic_ref>=201806
#define ULIBPP_CCLIB_ATOMIC_REF				1
#endif
#endif
#if defined(__has_include) && __has_include(<functional>)
#include <functional>
#if __cpp_lib_bind_front>=201811
#define ULIBPP_CCLIB_BIND_FRONT				1
#endif
#endif
/*
#if defined(__has_include) && __has_include(<bit>)
#include <bit>
#if __cpp_lib_bit_cast>=201806
#define ULIBPP_CCLIB_BIT_CAST					1
#endif
#endif
*/
#if defined(__has_include) && __has_include(<type_traits>)
#include <type_traits>
#if __cpp_lib_bounded_array_traits>=201902
#define ULIBPP_CCLIB_TRAITS_FOR_UNBOUND_ARR		1
#endif
#endif
#if defined(__has_include) && __has_include(<string>)
#include <string>
#if __cpp_lib_char8_t>=201811
#define ULIBPP_CCLIB_CHAR8_T				1
#endif
#endif
#if defined(__has_include) && __has_include(<concepts>)
#if _MSC_FULL_VER>=200000000
#include <concepts>
#if __cpp_lib_concepts>=201806
#define ULIBPP_CCLIB_CONCEPTS				1
#endif
#endif
#endif

// ???
#if defined(__has_include) && __has_include(<array>)
#include <array>
#if __cpp_lib_constexpr_misc>=201811
#define ULIBPP_CCLIB_CONSTEXPR				1
#endif
#endif

// ???
#if defined(__has_include) && __has_include(<algorithm>)
#include <algorithm>
#if __cpp_lib_constexpr_swap_algorithms>=201806
#define ULIBPP_CCLIB_CONSTEXPR_SWAP			1
#endif
#endif

#if defined(__has_include) && __has_include(<new>)
#include <new>
#if __cpp_lib_destroying_delete>=201806
#define ULIBPP_CCLIB_DESTROY_DELETE		1
#endif
#endif

// ???
#if defined(__has_include) && __has_include(<string>)
#include <string>
#if __cpp_lib_erase_if>=201811
#define ULIBPP_CCLIB_UNIFORM_CONT_ERASURE	1
#endif
#endif

// ???
#if defined(__has_include) && __has_include(<execution>)
#include <execution>
#if __cpp_lib_execution>=201902
#define ULIBPP_CCLIB_UNSEQ_EXEC			1
#endif
#endif

// ???
#if defined(__has_include) && __has_include(<unordered_set>)
#include <unordered_set>
#if __cpp_lib_generic_unordered_lookup>=201811
#define ULIBPP_CCLIB_GENER_UNORD_LOOKUP	1
#endif
#if __cpp_lib_generic_unordered_hash_lookup>=201902
#define ULIBPP_CCLIB_GENER_UNORD_HASH_LOOKUP	1
#endif
#endif

#if defined(__has_include) && __has_include(<numeric>)
#include <numeric>
#if __cpp_lib_interpolate>=201902
#define ULIBPP_CCLIB_INTERPOLATE			1
#endif
#endif
#if defined(__has_include) && __has_include(<string>)
#include <string>
#if __cpp_lib_char8_t>=201811
#define ULIBPP_CC_CHAR8_T_LIB				1
#endif
#endif
#if defined(__has_include) && __has_include(<string>)
#include <string>
#if __cpp_lib_char8_t>=201811
#define ULIBPP_CC_CHAR8_T_LIB				1
#endif
#endif
#if defined(__has_include) && __has_include(<string>)
#include <string>
#if __cpp_lib_char8_t>=201811
#define ULIBPP_CC_CHAR8_T_LIB				1
#endif
#endif
#if defined(__has_include) && __has_include(<string>)
#include <string>
#if __cpp_lib_char8_t>=201811
#define ULIBPP_CC_CHAR8_T_LIB				1
#endif
#endif
#if defined(__has_include) && __has_include(<string>)
#include <string>
#if __cpp_lib_char8_t>=201811
#define ULIBPP_CC_CHAR8_T_LIB				1
#endif
#endif
#if defined(__has_include) && __has_include(<string>)
#include <string>
#if __cpp_lib_char8_t>=201811
#define ULIBPP_CC_CHAR8_T_LIB				1
#endif
#endif


/* ================================================================================================ *
 *		Guessing compiler...																		*
 * ================================================================================================ */

#if !defined(ULIBPP_CC_PLATFORM)

#if defined(__INTEL_COMPILER)
#if __INTEL_COMPILER < 700
#error Not tested with Intel CC < 7.0
#endif
#if ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_LINUX || \
	ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_K1OM
#define ULIBPP_CC_PLATFORM				ULIBPP_CC_PLATFORM_LICC
#else
#define ULIBPP_CC_PLATFORM				ULIBPP_CC_PLATFORM_ICC
#endif /* ULIBPP_OS_PLATFORM */

#define ULIBPP_CC_VERSION				__INTEL_COMPILER

#define ULIBPP_CC_STDCPP11				(__GXX_EXPERIMENTAL_CXX0X__!=0)

// C++ 11
// https://software.intel.com/en-us/articles/c0x-features-supported-by-intel-c-compiler
// language
#ifndef ULIBPP_CC_RVALUE
#define ULIBPP_CC_RVALUE					(ULIBPP_CC_VERSION>=1200)
#endif
#ifndef ULIBPP_CC_REF_QUALIFIERS
#define ULIBPP_CC_REF_QUALIFIERS			(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_RVALUE_OBJINIT
#define ULIBPP_CC_RVALUE_OBJINIT			(ULIBPP_CC_VERSION>=1101)
#endif
#ifndef ULIBPP_CC_NSDMI
#define ULIBPP_CC_NSDMI						(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_VARIADIC_TEMPLATES
#define ULIBPP_CC_VARIADIC_TEMPLATES		(ULIBPP_CC_VERSION>=1201)
#endif
#ifndef ULIBPP_CC_VARIADIC_TEMPLATES_TEMPAR
#define ULIBPP_CC_VARIADIC_TEMPLATES_TEMPAR	(ULIBPP_CC_VERSION>=1201)
#endif
#ifndef ULIBPP_CC_INITIALIZER_LIST
#define ULIBPP_CC_INITIALIZER_LIST			(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_STATIC_ASSERT
#define ULIBPP_CC_STATIC_ASSERT				(ULIBPP_CC_VERSION>=1101)
#endif
#ifndef ULIBPP_CC_AUTO
#define ULIBPP_CC_AUTO						(ULIBPP_CC_VERSION>=1200)
#endif
#ifndef ULIBPP_CC_AUTO_MULTIDECL
#define ULIBPP_CC_AUTO_MULTIDECL			(ULIBPP_CC_VERSION>=1200)
#endif
#ifndef ULIBPP_CC_AUTO_REMOVED
#define ULIBPP_CC_AUTO_REMOVED				(ULIBPP_CC_VERSION>=1201)
#endif
#ifndef ULIBPP_CC_NEW_FN_SYNTAX
#define ULIBPP_CC_NEW_FN_SYNTAX				(ULIBPP_CC_VERSION>=1201)
#endif
#ifndef ULIBPP_CC_LAMBDAS
#define ULIBPP_CC_LAMBDAS					(ULIBPP_CC_VERSION>=1200)
#endif
#ifndef ULIBPP_CC_DECLTYPE
#define ULIBPP_CC_DECLTYPE					(ULIBPP_CC_VERSION>=1200)
#endif
#ifndef ULIBPP_CC_DECLTYPE_RETTYPE
#define ULIBPP_CC_DECLTYPE_RETTYPE			(ULIBPP_CC_VERSION>=1200)
#endif
#ifndef ULIBPP_CC_RABRA
#define ULIBPP_CC_RABRA						(ULIBPP_CC_VERSION>=1101)
#endif
#ifndef ULIBPP_CC_DEFAULT_TEMPL_ARG
#define ULIBPP_CC_DEFAULT_TEMPL_ARG			(ULIBPP_CC_VERSION>=1201)
#endif
#ifndef ULIBPP_CC_SFINAE
#define ULIBPP_CC_SFINAE					(ULIBPP_CC_VERSION>=1201)
#endif
#ifndef ULIBPP_CC_TEMPLATE_ALIASES
#define ULIBPP_CC_TEMPLATE_ALIASES			(ULIBPP_CC_VERSION>=1201)
#endif
#ifndef ULIBPP_CC_EXTERN_TEMPLATES
#define ULIBPP_CC_EXTERN_TEMPLATES			(ULIBPP_CC_VERSION>=1101)
#endif
#ifndef ULIBPP_CC_NULLPTR
#define ULIBPP_CC_NULLPTR					(ULIBPP_CC_VERSION>=1201)
#endif
#ifndef ULIBPP_CC_STRONGLYTYPED_ENUMS
#define ULIBPP_CC_STRONGLYTYPED_ENUMS		(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_FORWARD_ENUM_DECL
#define ULIBPP_CC_FORWARD_ENUM_DECL			(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_ATTRIBUTES
#define ULIBPP_CC_ATTRIBUTES				(ULIBPP_CC_VERSION>=1201)
#endif
#ifndef ULIBPP_CC_CONSTEXPR
#define ULIBPP_CC_CONSTEXPR					(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_ALIGNMENT
#define ULIBPP_CC_ALIGNMENT					(ULIBPP_CC_VERSION>=1500)
#endif
#ifndef ULIBPP_CC_COND_SUPPORT
#define ULIBPP_CC_COND_SUPPORT				0
#endif
#ifndef ULIBPP_CC_UNDEF_BEHAVIOR_DIAG_ERR
#define ULIBPP_CC_UNDEF_BEHAVIOR_DIAG_ERR	0
#endif
#ifndef ULIBPP_CC_DELEGATING_CONSTRUCTORS
#define ULIBPP_CC_DELEGATING_CONSTRUCTORS	(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_INHERITINING_CONSTRUCTORS
#define ULIBPP_CC_INHERITINING_CONSTRUCTORS	(ULIBPP_CC_VERSION>=1500)
#endif
#ifndef ULIBPP_CC_EXPLICIT_CONVERSION_OP
#define ULIBPP_CC_EXPLICIT_CONVERSION_OP	(ULIBPP_CC_VERSION>=1300)
#endif
#ifndef ULIBPP_CC_NEW_CHAR_TYPES
#define ULIBPP_CC_NEW_CHAR_TYPES			((ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_WIN_NT || ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_WIN_NT64) && ULIBPP_CC_VERSION>=1400 || ULIBPP_CC_VERSION>=1201)
#endif
#ifndef ULIBPP_CC_UNICODE_STRINGS
#define ULIBPP_CC_UNICODE_STRINGS			(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_RAW_STRINGS
#define ULIBPP_CC_RAW_STRINGS				(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_UNICODE_NAMES_IN_LITERALS
#define ULIBPP_CC_UNICODE_NAMES_IN_LITERALS	(ULIBPP_CC_VERSION>=1201)
#endif
#ifndef ULIBPP_CC_USERDEF_LITERALS
#define ULIBPP_CC_USERDEF_LITERALS			(ULIBPP_CC_VERSION>=1500)
#endif
#ifndef ULIBPP_CC_STD_LAYOUT_TYPES
#define ULIBPP_CC_STD_LAYOUT_TYPES			(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_DEFAULTED_FNCS
#define ULIBPP_CC_DEFAULTED_FNCS			(ULIBPP_CC_VERSION>=1200)
#endif
#ifndef ULIBPP_CC_DELETED_FNCS
#define ULIBPP_CC_DELETED_FNCS				(ULIBPP_CC_VERSION>=1200)
#endif
#ifndef ULIBPP_CC_EXTENDED_FRIENDS
#define ULIBPP_CC_EXTENDED_FRIENDS			(ULIBPP_CC_VERSION>=1200)
#endif
#ifndef ULIBPP_CC_EXTENDED_SIZEOF
#define ULIBPP_CC_EXTENDED_SIZEOF			(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_NAMESPACE_ASSOCIATION
#define ULIBPP_CC_NAMESPACE_ASSOCIATION		(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_UNRESTRICTED_UNIONS
#define ULIBPP_CC_UNRESTRICTED_UNIONS		(ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT && ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT64 && ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_LOCUN_TYPES_AS_TEMPLARGS
#define ULIBPP_CC_LOCUN_TYPES_AS_TEMPLARGS	(ULIBPP_CC_VERSION>=1200)
#endif
#ifndef ULIBPP_CC_RANGE_BASED_FOR
#define ULIBPP_CC_RANGE_BASED_FOR			(ULIBPP_CC_VERSION>=1300)
#endif
#ifndef ULIBPP_CC_OVERRIDE_FINAL
#define ULIBPP_CC_OVERRIDE_FINAL			(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_MIN_SUPPORT_FOR_GC
#define ULIBPP_CC_MIN_SUPPORT_FOR_GC		((ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_WIN_NT || ULIBPP_OS_PLATFORM==ULIBPP_OS_PLATFORM_WIN_NT64) && ULIBPP_CC_VERSION>=1500)
#endif
#ifndef ULIBPP_CC_NOEXCEPT
#define ULIBPP_CC_NOEXCEPT					(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CC_SPEC_MOVE_FNC
#define ULIBPP_CC_SPEC_MOVE_FNC				(ULIBPP_CC_VERSION>=1400)
#endif
// concurrency
#ifndef ULIBPP_CC_SEQ_POINTS
#define ULIBPP_CC_SEQ_POINTS				(ULIBPP_CC_VERSION>=1500)
#endif
#ifndef ULIBPP_CC_ATOMICS
#define ULIBPP_CC_ATOMICS					(ULIBPP_CC_VERSION>=1300)
#endif
#ifndef ULIBPP_CC_STRONG_CAEX
#define ULIBPP_CC_STRONG_CAEX				(ULIBPP_CC_VERSION>=1300)
#endif
#ifndef ULIBPP_CC_BIDIFENCES
#define ULIBPP_CC_BIDIFENCES				(ULIBPP_CC_VERSION>=1300)
#endif
#ifndef ULIBPP_CC_MEM_MODEL
#define ULIBPP_CC_MEM_MODEL					(ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT && ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT64 && ULIBPP_CC_VERSION>=1500)
#endif
#ifndef ULIBPP_CC_DATADEP_ORDER
#define ULIBPP_CC_DATADEP_ORDER				0
#endif
#ifndef ULIBPP_CC_EXCEPTION_PROPAG
#define ULIBPP_CC_EXCEPTION_PROPAG			(ULIBPP_CC_VERSION>=1200)
#endif
#ifndef ULIBPP_CC_ATOMICS_IN_SIGNAL
#define ULIBPP_CC_ATOMICS_IN_SIGNAL			(ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT && ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT64 && ULIBPP_CC_VERSION>=1500)
#endif
#ifndef ULIBPP_CC_TLS
#define ULIBPP_CC_TLS						(ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT && ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT64 && ULIBPP_CC_VERSION>=1500)
#endif
#ifndef ULIBPP_CC_CONCUR_INIT_DESTR
#define ULIBPP_CC_CONCUR_INIT_DESTR			(ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT && ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT64 && ULIBPP_CC_VERSION>=1101)
#endif
// C99
#ifndef ULIBPP_CC_FUNC_PREDEF
#define ULIBPP_CC_FUNC_PREDEF				(ULIBPP_CC_VERSION>=1101)
#endif
#ifndef ULIBPP_CC_C
#define ULIBPP_CC_C99_PP					(ULIBPP_CC_VERSION>=1101)
#endif
#ifndef ULIBPP_CC_LONG_LONG
#define ULIBPP_CC_LONG_LONG					(ULIBPP_CC_VERSION>=1101)
#endif
#ifndef ULIBPP_CC_EXT_INT_TYPES
#define ULIBPP_CC_EXT_INT_TYPES				(ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT && ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT64 && ULIBPP_CC_VERSION>=1500)
#endif
// library
#ifndef ULIBPP_CCLIB_BUILTIN_TYPE_TRAITS
#define ULIBPP_CCLIB_BUILTIN_TYPE_TRAITS		(ULIBPP_CC_VERSION>=1300)
#endif
#ifndef ULIBPP_CCLIB_ADDRESSOF
#define ULIBPP_CCLIB_ADDRESSOF					(ULIBPP_CC_VERSION>=1500)
#endif

// C++ 14
// https://software.intel.com/en-us/articles/c14-features-supported-by-intel-c-compiler
// language
#ifndef ULIBPP_CC_TWEAKED_CONTEXT_CONVERS
#define ULIBPP_CC_TWEAKED_CONTEXT_CONVERS	(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_BINARY_LITERALS
#define ULIBPP_CC_BINARY_LITERALS			(ULIBPP_CC_VERSION>=1101)
#endif
#ifndef ULIBPP_CC_RETURN_TYPE_DEDUCTION
#define ULIBPP_CC_RETURN_TYPE_DEDUCTION		(ULIBPP_CC_VERSION>=1500)
#endif
#ifndef ULIBPP_CC_INIT_LAMBDA_CAPTURES
#define ULIBPP_CC_INIT_LAMBDA_CAPTURES		(ULIBPP_CC_VERSION>=1500)
#endif
#ifndef ULIBPP_CC_GENERIC_LAMBDAS
#define ULIBPP_CC_GENERIC_LAMBDAS			(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_VARIABLE_TEMPLATES
#define ULIBPP_CC_VARIABLE_TEMPLATES		(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_RELAX_CONSTEXPR
#define ULIBPP_CC_RELAX_CONSTEXPR			(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_AGGR_NSDMI
#define ULIBPP_CC_AGGR_NSDMI				(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_CLARIFY_MEMORY_ALLOC
#define ULIBPP_CC_CLARIFY_MEMORY_ALLOC		0
#endif
#ifndef ULIBPP_CC_DEPRECATED_ATTR
#define ULIBPP_CC_DEPRECATED_ATTR			(ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT && ULIBPP_OS_PLATFORM!=ULIBPP_OS_PLATFORM_WIN_NT64 && ULIBPP_CC_VERSION>=1500)
#endif
#ifndef ULIBPP_CC_DIGIT_SEPARATOR
#define ULIBPP_CC_DIGIT_SEPARATOR			(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_SIZED_DEALLOC
#define ULIBPP_CC_SIZED_DEALLOC				(ULIBPP_CC_VERSION>=1700)
#endif
// library
#ifndef ULIBPP_CCLIB_MAKE_UNIQUE
#define ULIBPP_CCLIB_MAKE_UNIQUE			(ULIBPP_CC_PLATFORM==ULIBPP_CC_PLATFORM_ICC && ULIBPP_CC_VERSION>=1500)
#endif

// C++17
// language
#ifndef ULIBPP_CC_TERSE_STATIC_ASSERT
#define ULIBPP_CC_TERSE_STATIC_ASSERT		(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_REMOVING_TRIGRAPHS
#define ULIBPP_CC_REMOVING_TRIGRAPHS		0
#endif
#ifndef ULIBPP_CC_TYPENAME_IN_TEMPL_TEMPL
#define ULIBPP_CC_TYPENAME_IN_TEMPL_TEMPL	(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_AUTO_BRACED_LIST
#define ULIBPP_CC_AUTO_BRACED_LIST			(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_FOLD_EXPR
#define ULIBPP_CC_FOLD_EXPR					(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_U
#define ULIBPP_CC_U8_LITERALS				(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_NESTED_NAMESPACE_DEF
#define ULIBPP_CC_NESTED_NAMESPACE_DEF		(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_ATTRS_NAMESPACES_ENUMS
#define ULIBPP_CC_ATTRS_NAMESPACES_ENUMS	(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_MORE_NONTYPE_TEMPL_ARGS
#define ULIBPP_CC_MORE_NONTYPE_TEMPL_ARGS	0
#endif
#ifndef ULIBPP_CC_REMOVED_REGISTER_STC
#define ULIBPP_CC_REMOVED_REGISTER_STC		(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_REMOVED_BOOL_INCR
#define ULIBPP_CC_REMOVED_BOOL_INCR			(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_EXCSPEC_IN_TYPESYS
#define ULIBPP_CC_EXCSPEC_IN_TYPESYS		(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_HAS_INCLUDE
#define ULIBPP_CC_HAS_INCLUDE				(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_NEW_INHERIT_CONSTR
#define ULIBPP_CC_NEW_INHERIT_CONSTR		0
#endif
#ifndef ULIBPP_CC_FALLTHROUGH_ATTR
#define ULIBPP_CC_FALLTHROUGH_ATTR			(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_NODISCARD_ATTR
#define ULIBPP_CC_NODISCARD_ATTR			(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_MAYBEUNUSED_ATTR
#define ULIBPP_CC_MAYBEUNUSED_ATTR			(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_AGGR_BASE_CLASS_INIT
#define ULIBPP_CC_AGGR_BASE_CLASS_INIT		0
#endif
#ifndef ULIBPP_CC_CONSTEXPR_LAMBDA
#define ULIBPP_CC_CONSTEXPR_LAMBDA			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_DIFF_BEGIN_END_FOR
#define ULIBPP_CC_DIFF_BEGIN_END_FOR		(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_LAMBDA_CAPTURE_DEREFTHIS
#define ULIBPP_CC_LAMBDA_CAPTURE_DEREFTHIS	(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_DIRECT_LIST_INIT_OF_ENUMS
#define ULIBPP_CC_DIRECT_LIST_INIT_OF_ENUMS	(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_HEXA_FLOAT_LITERALS
#define ULIBPP_CC_HEXA_FLOAT_LITERALS		(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_NS_ATTRS_WO_REPETITION
#define ULIBPP_CC_NS_ATTRS_WO_REPETITION	(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_ALIGNED_NEW
#define ULIBPP_CC_ALIGNED_NEW				0
#endif
#ifndef ULIBPP_CC_TEMPL_DEDUCTION_FOR_CLASS
#define ULIBPP_CC_TEMPL_DEDUCTION_FOR_CLASS	0
#endif
#ifndef ULIBPP_CC_AUTO_FOR_NOTYPE_TEMPL_ARG
#define ULIBPP_CC_AUTO_FOR_NOTYPE_TEMPL_ARG	0
#endif
#ifndef ULIBPP_CC_GUARANTEED_COPY_ELISION
#define ULIBPP_CC_GUARANTEED_COPY_ELISION	0
#endif
#ifndef ULIBPP_CC_STRICTER_EXPR_EVAL_ORDER
#define ULIBPP_CC_STRICTER_EXPR_EVAL_ORDER	0
#endif
#ifndef ULIBPP_CC_IGNORE_UNKNOWN_ATTR
#define ULIBPP_CC_IGNORE_UNKNOWN_ATTR		(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_CONSTEXPR_IF
#define ULIBPP_CC_CONSTEXPR_IF				(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_INLINE_VARIABLES
#define ULIBPP_CC_INLINE_VARIABLES			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_STRUCTURED_BINDING
#define ULIBPP_CC_STRUCTURED_BINDING		(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_INIT_EXPR_FOR_IF_SWITCH
#define ULIBPP_CC_INIT_EXPR_FOR_IF_SWITCH	(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_MATCH_TEMPL_TEMPL_ARGS
#define ULIBPP_CC_MATCH_TEMPL_TEMPL_ARGS	0
#endif
#ifndef ULIBPP_CC_FIX_QUALIF_CONVERS
#define ULIBPP_CC_FIX_QUALIF_CONVERS		0
#endif
#ifndef ULIBPP_CC_REMOVE_EXCEP_SPEC
#define ULIBPP_CC_REMOVE_EXCEP_SPEC			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_PACK_EXP_IN_USING
#define ULIBPP_CC_PACK_EXP_IN_USING			0
#endif
// library
#ifndef ULIBPP_CCLIB_PARA_ALG
#define ULIBPP_CCLIB_PARA_ALG					(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_ALIGNED_ALLOC
#define ULIBPP_CC_ALIGNED_ALLOC				0
#endif

// C++20
// language
#ifndef ULIBPP_CC_DFLT_INIT_BITFLD
#define ULIBPP_CC_DFLT_INIT_BITFLD			0
#endif
#ifndef ULIBPP_CC_CONST_MEMPTR
#define ULIBPP_CC_CONST_MEMPTR			0
#endif
#ifndef ULIBPP_CC_ALLOW_LAMBDA_CAPTURE
#define ULIBPP_CC_ALLOW_LAMBDA_CAPTURE		0
#endif
#ifndef ULIBPP_CC_VAOPT_COMMA_ELISION
#define ULIBPP_CC_VAOPT_COMMA_ELISION		0
#endif
#ifndef ULIBPP_CC_DESIGN_INIT
#define ULIBPP_CC_DESIGN_INIT				0
#endif
#ifndef ULIBPP_CC_TEMPL_PARLIST_GEN_LAMDAS
#define ULIBPP_CC_TEMPL_PARLIST_GEN_LAMDAS		0
#endif
#ifndef ULIBPP_CC_INITL_CLASS_TEMPL_DEDUCT
#define ULIBPP_CC_INITL_CLASS_TEMPL_DEDUCT	0
#endif
#ifndef ULIBPP_CC_CONCEPTS
#define ULIBPP_CC_CONCEPTS					0
#endif
#ifndef ULIBPP_CC_RANGE_FOR_WITH_INIT
#define ULIBPP_CC_RANGE_FOR_WITH_INIT		0
#endif
#ifndef ULIBPP_CC_SIMPLY_IMPL_LAMDA_CAPT
#define ULIBPP_CC_SIMPLY_IMPL_LAMDA_CAPT	0
#endif
#ifndef ULIBPP_CC_ADL_TEMPL_NOT_VISIBLE
#define ULIBPP_CC_ADL_TEMPL_NOT_VISIBLE		0
#endif
#ifndef ULIBPP_CC_CONST_MIS_DFLT_COPYCONST
#define ULIBPP_CC_CONST_MIS_DFLT_COPYCONST	0
#endif
#ifndef ULIBPP_CC_LESS_EAGER_INST_CONSTEXPR
#define ULIBPP_CC_LESS_EAGER_INST_CONSTEXPR	0
#endif
#ifndef ULIBPP_CC_CONSISTENT_COMPARISON
#define ULIBPP_CC_CONSISTENT_COMPARISON		0
#endif
#ifndef ULIBPP_CC_ACC_CHECK_ON_SPEC
#define ULIBPP_CC_ACC_CHECK_ON_SPEC			0
#endif
#ifndef ULIBPP_CC_DFLT_STATELESS_LAMBDA
#define ULIBPP_CC_DFLT_STATELESS_LAMBDA		0
#endif
#ifndef ULIBPP_CC_LAMBDA_UNEVAL_CTX
#define ULIBPP_CC_LAMBDA_UNEVAL_CTX			0
#endif
#ifndef ULIBPP_CC_EMPTY_OBJECTS
#define ULIBPP_CC_EMPTY_OBJECTS				0
#endif
#ifndef ULIBPP_CC_RELAX_RANGE_FOR
#define ULIBPP_CC_RELAX_RANGE_FOR			0
#endif
#ifndef ULIBPP_CC_ALLOW_STRUCT_BIND
#define ULIBPP_CC_ALLOW_STRUCT_BIND			0
#endif
#ifndef ULIBPP_CC_RELAX_STRUCT_BIND
#define ULIBPP_CC_RELAX_STRUCT_BIND			0
#endif
#ifndef ULIBPP_CC_DOWN_WITH_TYPENAME
#define ULIBPP_CC_DOWN_WITH_TYPENAME		0
#endif
#ifndef ULIBPP_CC_ALLOW_PACK_EXP_IN_LAMBDA
#define ULIBPP_CC_ALLOW_PACK_EXP_IN_LAMBDA	0
#endif
#ifndef ULIBPP_CC_LIKELY_UNLIKELY
#define ULIBPP_CC_LIKELY_UNLIKELY			0
#endif

#elif defined(__clang__)

#define ULIBPP_CC_PLATFORM				ULIBPP_CC_PLATFORM_CLANG

#define ULIBPP_CC_VERSION				(__clang_major__*100+__clang_minor__)

#define ULIBPP_CC_STDCPP11				(__GXX_EXPERIMENTAL_CXX0X__!=0)

// C++ 11
// http://clang.llvm.org/cxx_status.html
// language
#ifndef ULIBPP_CC_RVALUE
#define ULIBPP_CC_RVALUE					(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_REF_QUALIFIERS
#define ULIBPP_CC_REF_QUALIFIERS			(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_RVALUE_OBJINIT
#define ULIBPP_CC_RVALUE_OBJINIT			(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_NSDMI
#define ULIBPP_CC_NSDMI						(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_VARIADIC_TEMPLATES
#define ULIBPP_CC_VARIADIC_TEMPLATES		(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_VARIADIC_TEMPLATES_TEMPAR
#define ULIBPP_CC_VARIADIC_TEMPLATES_TEMPAR	(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_INITIALIZER_LIST
#define ULIBPP_CC_INITIALIZER_LIST			(ULIBPP_CC_VERSION>=301)
#endif
#ifndef ULIBPP_CC_STATIC_ASSERT
#define ULIBPP_CC_STATIC_ASSERT				(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_AUTO
#define ULIBPP_CC_AUTO						(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_AUTO_MULTIDECL
#define ULIBPP_CC_AUTO_MULTIDECL			(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_AUTO_REMOVED
#define ULIBPP_CC_AUTO_REMOVED				(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_NEW_FN_SYNTAX
#define ULIBPP_CC_NEW_FN_SYNTAX				(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_LAMBDAS
#define ULIBPP_CC_LAMBDAS					(ULIBPP_CC_VERSION>=301)
#endif
#ifndef ULIBPP_CC_DECLTYPE
#define ULIBPP_CC_DECLTYPE					(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_DECLTYPE_RETTYPE
#define ULIBPP_CC_DECLTYPE_RETTYPE			(ULIBPP_CC_VERSION>=301)
#endif
#ifndef ULIBPP_CC_RABRA
#define ULIBPP_CC_RABRA						(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_DEFAULT_TEMPL_ARG
#define ULIBPP_CC_DEFAULT_TEMPL_ARG			(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_SFINAE
#define ULIBPP_CC_SFINAE					(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_TEMPLATE_ALIASES
#define ULIBPP_CC_TEMPLATE_ALIASES			(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_EXTERN_TEMPLATES
#define ULIBPP_CC_EXTERN_TEMPLATES			(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_NULLPTR
#define ULIBPP_CC_NULLPTR					(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_STRONGLYTYPED_ENUMS
#define ULIBPP_CC_STRONGLYTYPED_ENUMS		(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_FORWARD_ENUM_DECL
#define ULIBPP_CC_FORWARD_ENUM_DECL			(ULIBPP_CC_VERSION>=301)
#endif
#ifndef ULIBPP_CC_ATTRIBUTES
#define ULIBPP_CC_ATTRIBUTES				(ULIBPP_CC_VERSION>=303)
#endif
#ifndef ULIBPP_CC_CONSTEXPR
#define ULIBPP_CC_CONSTEXPR					(ULIBPP_CC_VERSION>=301)
#endif
#ifndef ULIBPP_CC_ALIGNMENT
#define ULIBPP_CC_ALIGNMENT					(ULIBPP_CC_VERSION>=303)
#endif
#ifndef ULIBPP_CC_COND_SUPPORT
#define ULIBPP_CC_COND_SUPPORT				(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_UNDEF_BEHAVIOR_DIAG_ERR
#define ULIBPP_CC_UNDEF_BEHAVIOR_DIAG_ERR	(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_DELEGATING_CONSTRUCTORS
#define ULIBPP_CC_DELEGATING_CONSTRUCTORS	(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_INHERITINING_CONSTRUCTORS
#define ULIBPP_CC_INHERITINING_CONSTRUCTORS	(ULIBPP_CC_VERSION>=303)
#endif
#ifndef ULIBPP_CC_EXPLICIT_CONVERSION_OP
#define ULIBPP_CC_EXPLICIT_CONVERSION_OP	(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_NEW_CHAR_TYPES
#define ULIBPP_CC_NEW_CHAR_TYPES			(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_UNICODE_STRINGS
#define ULIBPP_CC_UNICODE_STRINGS			(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_RAW_STRINGS
#define ULIBPP_CC_RAW_STRINGS				(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_UNICODE_NAMES_IN_LITERALS
#define ULIBPP_CC_UNICODE_NAMES_IN_LITERALS	(ULIBPP_CC_VERSION>=301)
#endif
#ifndef ULIBPP_CC_USERDEF_LITERALS
#define ULIBPP_CC_USERDEF_LITERALS			(ULIBPP_CC_VERSION>=301)
#endif
#ifndef ULIBPP_CC_STD_LAYOUT_TYPES
#define ULIBPP_CC_STD_LAYOUT_TYPES			(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_DEFAULTED_FNCS
#define ULIBPP_CC_DEFAULTED_FNCS			(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_DELETED_FNCS
#define ULIBPP_CC_DELETED_FNCS				(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_EXTENDED_FRIENDS
#define ULIBPP_CC_EXTENDED_FRIENDS			(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_EXTENDED_SIZEOF
#define ULIBPP_CC_EXTENDED_SIZEOF			(ULIBPP_CC_VERSION>=301)
#endif
#ifndef ULIBPP_CC_NAMESPACE_ASSOCIATION
#define ULIBPP_CC_NAMESPACE_ASSOCIATION		(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_UNRESTRICTED_UNIONS
#define ULIBPP_CC_UNRESTRICTED_UNIONS		(ULIBPP_CC_VERSION>=301)
#endif
#ifndef ULIBPP_CC_LOCUN_TYPES_AS_TEMPLARGS
#define ULIBPP_CC_LOCUN_TYPES_AS_TEMPLARGS	(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_RANGE_BASED_FOR
#define ULIBPP_CC_RANGE_BASED_FOR			(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_OVERRIDE_FINAL
#define ULIBPP_CC_OVERRIDE_FINAL			(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_MIN_SUPPORT_FOR_GC
#define ULIBPP_CC_MIN_SUPPORT_FOR_GC		0
#endif
#ifndef ULIBPP_CC_NOEXCEPT
#define ULIBPP_CC_NOEXCEPT					(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_SPEC_MOVE_FNC
#define ULIBPP_CC_SPEC_MOVE_FNC				(ULIBPP_CC_VERSION>=300)
#endif
// concurrency
#ifndef ULIBPP_CC_SEQ_POINTS
#define ULIBPP_CC_SEQ_POINTS				(ULIBPP_CC_VERSION>=303)
#endif
#ifndef ULIBPP_CC_ATOMICS
#define ULIBPP_CC_ATOMICS					(ULIBPP_CC_VERSION>=301)
#endif
#ifndef ULIBPP_CC_STRONG_CAEX
#define ULIBPP_CC_STRONG_CAEX				(ULIBPP_CC_VERSION>=301)
#endif
#ifndef ULIBPP_CC_BIDIFENCES
#define ULIBPP_CC_BIDIFENCES				(ULIBPP_CC_VERSION>=301)
#endif
#ifndef ULIBPP_CC_MEM_MODEL
#define ULIBPP_CC_MEM_MODEL					(ULIBPP_CC_VERSION>=302)
#endif
#ifndef ULIBPP_CC_DATADEP_ORDER
#define ULIBPP_CC_DATADEP_ORDER				(ULIBPP_CC_VERSION>=302)
#endif
#ifndef ULIBPP_CC_EXCEPTION_PROPAG
#define ULIBPP_CC_EXCEPTION_PROPAG			(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_ATOMICS_IN_SIGNAL
#define ULIBPP_CC_ATOMICS_IN_SIGNAL			(ULIBPP_CC_VERSION>=301)
#endif
#ifndef ULIBPP_CC_TLS
#define ULIBPP_CC_TLS						(ULIBPP_CC_VERSION>=303)
#endif
#ifndef ULIBPP_CC_CONCUR_INIT_DESTR
#define ULIBPP_CC_CONCUR_INIT_DESTR			(ULIBPP_CC_VERSION>=209)
#endif
// C99
#ifndef ULIBPP_CC_FUNC_PREDEF
#define ULIBPP_CC_FUNC_PREDEF				(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_C
#define ULIBPP_CC_C99_PP					(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_LONG_LONG
#define ULIBPP_CC_LONG_LONG					(ULIBPP_CC_VERSION>=209)
#endif
#ifndef ULIBPP_CC_EXT_INT_TYPES
#define ULIBPP_CC_EXT_INT_TYPES				0
#endif
// library
#ifndef ULIBPP_CCLIB_BUILTIN_TYPE_TRAITS
#define ULIBPP_CCLIB_BUILTIN_TYPE_TRAITS		(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CCLIB_ADDRESSOF
#define ULIBPP_CCLIB_ADDRESSOF					(ULIBPP_CC_VERSION>=304)
#endif
#ifndef ULIBPP_CCLIB_REGEX
#define ULIBPP_CCLIB_REGEX						(ULIBPP_CC_VERSION>=303)
#endif

// C++ 14
// language
#ifndef ULIBPP_CC_TWEAKED_CONTEXT_CONVERS
#define ULIBPP_CC_TWEAKED_CONTEXT_CONVERS	(ULIBPP_CC_VERSION>=304)
#endif
#ifndef ULIBPP_CC_BINARY_LITERALS
#define ULIBPP_CC_BINARY_LITERALS			(ULIBPP_CC_VERSION>=303)
#endif
#ifndef ULIBPP_CC_RETURN_TYPE_DEDUCTION
#define ULIBPP_CC_RETURN_TYPE_DEDUCTION		(ULIBPP_CC_VERSION>=304)
#endif
#ifndef ULIBPP_CC_INIT_LAMBDA_CAPTURES
#define ULIBPP_CC_INIT_LAMBDA_CAPTURES		(ULIBPP_CC_VERSION>=304)
#endif
#ifndef ULIBPP_CC_GENERIC_LAMBDAS
#define ULIBPP_CC_GENERIC_LAMBDAS			(ULIBPP_CC_VERSION>=304)
#endif
#ifndef ULIBPP_CC_VARIABLE_TEMPLATES
#define ULIBPP_CC_VARIABLE_TEMPLATES		(ULIBPP_CC_VERSION>=304)
#endif
#ifndef ULIBPP_CC_RELAX_CONSTEXPR
#define ULIBPP_CC_RELAX_CONSTEXPR			(ULIBPP_CC_VERSION>=304)
#endif
#ifndef ULIBPP_CC_AGGR_NSDMI
#define ULIBPP_CC_AGGR_NSDMI				(ULIBPP_CC_VERSION>=303)
#endif
#ifndef ULIBPP_CC_CLARIFY_MEMORY_ALLOC
#define ULIBPP_CC_CLARIFY_MEMORY_ALLOC		(ULIBPP_CC_VERSION>=304)
#endif
#ifndef ULIBPP_CC_DEPRECATED_ATTR
#define ULIBPP_CC_DEPRECATED_ATTR			(ULIBPP_CC_VERSION>=304)
#endif
#ifndef ULIBPP_CC_DIGIT_SEPARATOR
#define ULIBPP_CC_DIGIT_SEPARATOR			(ULIBPP_CC_VERSION>=304)
#endif
#ifndef ULIBPP_CC_SIZED_DEALLOC
#define ULIBPP_CC_SIZED_DEALLOC				(ULIBPP_CC_VERSION>=304)
#endif
// library
#ifndef ULIBPP_CCLIB_MAKE_UNIQUE
#define ULIBPP_CCLIB_MAKE_UNIQUE			(ULIBPP_CC_VERSION>=304)
#endif

// C++17
// language
#ifndef ULIBPP_CC_TERSE_STATIC_ASSERT
#define ULIBPP_CC_TERSE_STATIC_ASSERT		(ULIBPP_CC_VERSION>=305)
#endif
#ifndef ULIBPP_CC_REMOVING_TRIGRAPHS
#define ULIBPP_CC_REMOVING_TRIGRAPHS		(ULIBPP_CC_VERSION>=305)
#endif
#ifndef ULIBPP_CC_TYPENAME_IN_TEMPL_TEMPL
#define ULIBPP_CC_TYPENAME_IN_TEMPL_TEMPL	(ULIBPP_CC_VERSION>=305)
#endif
#ifndef ULIBPP_CC_AUTO_BRACED_LIST
#define ULIBPP_CC_AUTO_BRACED_LIST			(ULIBPP_CC_VERSION>=308)
#endif
#ifndef ULIBPP_CC_FOLD_EXPR
#define ULIBPP_CC_FOLD_EXPR					(ULIBPP_CC_VERSION>=309)
#endif
#ifndef ULIBPP_CC_U
#define ULIBPP_CC_U8_LITERALS				(ULIBPP_CC_VERSION>=306)
#endif
#ifndef ULIBPP_CC_NESTED_NAMESPACE_DEF
#define ULIBPP_CC_NESTED_NAMESPACE_DEF		(ULIBPP_CC_VERSION>=306)
#endif
#ifndef ULIBPP_CC_ATTRS_NAMESPACES_ENUMS
#define ULIBPP_CC_ATTRS_NAMESPACES_ENUMS	(ULIBPP_CC_VERSION>=306)
#endif
#ifndef ULIBPP_CC_MORE_NONTYPE_TEMPL_ARGS
#define ULIBPP_CC_MORE_NONTYPE_TEMPL_ARGS	(ULIBPP_CC_VERSION>=306)
#endif
#ifndef ULIBPP_CC_REMOVED_REGISTER_STC
#define ULIBPP_CC_REMOVED_REGISTER_STC		(ULIBPP_CC_VERSION>=308)
#endif
#ifndef ULIBPP_CC_REMOVED_BOOL_INCR
#define ULIBPP_CC_REMOVED_BOOL_INCR			(ULIBPP_CC_VERSION>=308)
#endif
#ifndef ULIBPP_CC_EXCSPEC_IN_TYPESYS
#define ULIBPP_CC_EXCSPEC_IN_TYPESYS		(ULIBPP_CC_VERSION>=400)
#endif
#ifndef ULIBPP_CC_HAS_INCLUDE
#define ULIBPP_CC_HAS_INCLUDE				(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_NEW_INHERIT_CONSTR
#define ULIBPP_CC_NEW_INHERIT_CONSTR		(ULIBPP_CC_VERSION>=309)
#endif
#ifndef ULIBPP_CC_FALLTHROUGH_ATTR
#define ULIBPP_CC_FALLTHROUGH_ATTR			(ULIBPP_CC_VERSION>=309)
#endif
#ifndef ULIBPP_CC_NODISCARD_ATTR
#define ULIBPP_CC_NODISCARD_ATTR			(ULIBPP_CC_VERSION>=309)
#endif
#ifndef ULIBPP_CC_MAYBEUNUSED_ATTR
#define ULIBPP_CC_MAYBEUNUSED_ATTR			(ULIBPP_CC_VERSION>=309)
#endif
#ifndef ULIBPP_CC_AGGR_BASE_CLASS_INIT
#define ULIBPP_CC_AGGR_BASE_CLASS_INIT		(ULIBPP_CC_VERSION>=309)
#endif
#ifndef ULIBPP_CC_CONSTEXPR_LAMBDA
#define ULIBPP_CC_CONSTEXPR_LAMBDA			(ULIBPP_CC_VERSION>=500)
#endif
#ifndef ULIBPP_CC_DIFF_BEGIN_END_FOR
#define ULIBPP_CC_DIFF_BEGIN_END_FOR		(ULIBPP_CC_VERSION>=309)
#endif
#ifndef ULIBPP_CC_LAMBDA_CAPTURE_DEREFTHIS
#define ULIBPP_CC_LAMBDA_CAPTURE_DEREFTHIS	(ULIBPP_CC_VERSION>=309)
#endif
#ifndef ULIBPP_CC_DIRECT_LIST_INIT_OF_ENUMS
#define ULIBPP_CC_DIRECT_LIST_INIT_OF_ENUMS	(ULIBPP_CC_VERSION>=309)
#endif
#ifndef ULIBPP_CC_HEXA_FLOAT_LITERALS
#define ULIBPP_CC_HEXA_FLOAT_LITERALS		(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_NS_ATTRS_WO_REPETITION
#define ULIBPP_CC_NS_ATTRS_WO_REPETITION	(ULIBPP_CC_VERSION>=309)
#endif
#ifndef ULIBPP_CC_ALIGNED_NEW
#define ULIBPP_CC_ALIGNED_NEW				(ULIBPP_CC_VERSION>=400)
#endif
#ifndef ULIBPP_CC_TEMPL_DEDUCTION_FOR_CLASS
#define ULIBPP_CC_TEMPL_DEDUCTION_FOR_CLASS	(ULIBPP_CC_VERSION>=500)
#endif
#ifndef ULIBPP_CC_AUTO_FOR_NOTYPE_TEMPL_ARG
#define ULIBPP_CC_AUTO_FOR_NOTYPE_TEMPL_ARG	(ULIBPP_CC_VERSION>=400)
#endif
#ifndef ULIBPP_CC_GUARANTEED_COPY_ELISION
#define ULIBPP_CC_GUARANTEED_COPY_ELISION	(ULIBPP_CC_VERSION>=400)
#endif
#ifndef ULIBPP_CC_STRICTER_EXPR_EVAL_ORDER
#define ULIBPP_CC_STRICTER_EXPR_EVAL_ORDER	(ULIBPP_CC_VERSION>=400)
#endif
#ifndef ULIBPP_CC_IGNORE_UNKNOWN_ATTR
#define ULIBPP_CC_IGNORE_UNKNOWN_ATTR		(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_CONSTEXPR_IF
#define ULIBPP_CC_CONSTEXPR_IF				(ULIBPP_CC_VERSION>=309)
#endif
#ifndef ULIBPP_CC_INLINE_VARIABLES
#define ULIBPP_CC_INLINE_VARIABLES			(ULIBPP_CC_VERSION>=309)
#endif
#ifndef ULIBPP_CC_STRUCTURED_BINDING
#define ULIBPP_CC_STRUCTURED_BINDING		(ULIBPP_CC_VERSION>=400)
#endif
#ifndef ULIBPP_CC_INIT_EXPR_FOR_IF_SWITCH
#define ULIBPP_CC_INIT_EXPR_FOR_IF_SWITCH	(ULIBPP_CC_VERSION>=309)
#endif
#ifndef ULIBPP_CC_MATCH_TEMPL_TEMPL_ARGS
#define ULIBPP_CC_MATCH_TEMPL_TEMPL_ARGS	0
#endif
#ifndef ULIBPP_CC_FIX_QUALIF_CONVERS
#define ULIBPP_CC_FIX_QUALIF_CONVERS		(ULIBPP_CC_VERSION>=500)
#endif
#ifndef ULIBPP_CC_REMOVE_EXCEP_SPEC
#define ULIBPP_CC_REMOVE_EXCEP_SPEC			(ULIBPP_CC_VERSION>=400)
#endif
#ifndef ULIBPP_CC_PACK_EXP_IN_USING
#define ULIBPP_CC_PACK_EXP_IN_USING			(ULIBPP_CC_VERSION>=400)
#endif
// library
#ifndef ULIBPP_CCLIB_PARA_ALG
#define ULIBPP_CCLIB_PARA_ALG					0
#endif
#ifndef ULIBPP_CC_ALIGNED_ALLOC
#define ULIBPP_CC_ALIGNED_ALLOC				0
#endif

// C++20
// language
#ifndef ULIBPP_CC_DFLT_INIT_BITFLD
#define ULIBPP_CC_DFLT_INIT_BITFLD			(ULIBPP_CC_VERSION>=600)
#endif
#ifndef ULIBPP_CC_CONST_MEMPTR
#define ULIBPP_CC_CONST_MEMPTR			(ULIBPP_CC_VERSION>=600)
#endif
#ifndef ULIBPP_CC_ALLOW_LAMBDA_CAPTURE
#define ULIBPP_CC_ALLOW_LAMBDA_CAPTURE		(ULIBPP_CC_VERSION>=600)
#endif
#ifndef ULIBPP_CC_VAOPT_COMMA_ELISION
#define ULIBPP_CC_VAOPT_COMMA_ELISION		(ULIBPP_CC_VERSION>=600)
#endif
#ifndef ULIBPP_CC_DESIGN_INIT
#define ULIBPP_CC_DESIGN_INIT				0
#endif
#ifndef ULIBPP_CC_TEMPL_PARLIST_GEN_LAMDAS
#define ULIBPP_CC_TEMPL_PARLIST_GEN_LAMDAS		0
#endif
#ifndef ULIBPP_CC_INITL_CLASS_TEMPL_DEDUCT
#define ULIBPP_CC_INITL_CLASS_TEMPL_DEDUCT	(ULIBPP_CC_VERSION>=600)
#endif
#ifndef ULIBPP_CC_CONCEPTS
#define ULIBPP_CC_CONCEPTS					0
#endif
#ifndef ULIBPP_CC_RANGE_FOR_WITH_INIT
#define ULIBPP_CC_RANGE_FOR_WITH_INIT		0
#endif
#ifndef ULIBPP_CC_SIMPLY_IMPL_LAMDA_CAPT
#define ULIBPP_CC_SIMPLY_IMPL_LAMDA_CAPT	0
#endif
#ifndef ULIBPP_CC_ADL_TEMPL_NOT_VISIBLE
#define ULIBPP_CC_ADL_TEMPL_NOT_VISIBLE		0
#endif
#ifndef ULIBPP_CC_CONST_MIS_DFLT_COPYCONST
#define ULIBPP_CC_CONST_MIS_DFLT_COPYCONST	0
#endif
#ifndef ULIBPP_CC_LESS_EAGER_INST_CONSTEXPR
#define ULIBPP_CC_LESS_EAGER_INST_CONSTEXPR	0
#endif
#ifndef ULIBPP_CC_CONSISTENT_COMPARISON
#define ULIBPP_CC_CONSISTENT_COMPARISON		0
#endif
#ifndef ULIBPP_CC_ACC_CHECK_ON_SPEC
#define ULIBPP_CC_ACC_CHECK_ON_SPEC			0
#endif
#ifndef ULIBPP_CC_DFLT_STATELESS_LAMBDA
#define ULIBPP_CC_DFLT_STATELESS_LAMBDA		0
#endif
#ifndef ULIBPP_CC_LAMBDA_UNEVAL_CTX
#define ULIBPP_CC_LAMBDA_UNEVAL_CTX			0
#endif
#ifndef ULIBPP_CC_EMPTY_OBJECTS
#define ULIBPP_CC_EMPTY_OBJECTS				0
#endif
#ifndef ULIBPP_CC_RELAX_RANGE_FOR
#define ULIBPP_CC_RELAX_RANGE_FOR			0
#endif
#ifndef ULIBPP_CC_ALLOW_STRUCT_BIND
#define ULIBPP_CC_ALLOW_STRUCT_BIND			0
#endif
#ifndef ULIBPP_CC_RELAX_STRUCT_BIND
#define ULIBPP_CC_RELAX_STRUCT_BIND			0
#endif
#ifndef ULIBPP_CC_DOWN_WITH_TYPENAME
#define ULIBPP_CC_DOWN_WITH_TYPENAME		0
#endif
#ifndef ULIBPP_CC_ALLOW_PACK_EXP_IN_LAMBDA
#define ULIBPP_CC_ALLOW_PACK_EXP_IN_LAMBDA	0
#endif
#ifndef ULIBPP_CC_LIKELY_UNLIKELY
#define ULIBPP_CC_LIKELY_UNLIKELY			0
#endif

#elif defined(__GNUC__)
#if ( __GNUC__ < 2 ) || ( __GNUC__ == 2 && __GNUC_MINOR__ < 96)
#error Not tested with GNU C/C++ < 2.96
#endif
#define ULIBPP_CC_PLATFORM				ULIBPP_CC_PLATFORM_GCC

#define ULIBPP_CC_VERSION				(__GNUC__*100+__GNUC_MINOR__)

#define ULIBPP_CC_STDCPP11				(__GXX_EXPERIMENTAL_CXX0X__!=0)

// C++ 11
// https://gcc.gnu.org/projects/cxx-status.html#cxx11
// language
#ifndef ULIBPP_CC_RVALUE
#define ULIBPP_CC_RVALUE					(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CC_REF_QUALIFIERS
#define ULIBPP_CC_REF_QUALIFIERS			(ULIBPP_CC_VERSION==408 && __GNUC_PATCHLEVEL__>=1 || ULIBPP_CC_VERSION>408)
#endif
#ifndef ULIBPP_CC_RVALUE_OBJINIT
#define ULIBPP_CC_RVALUE_OBJINIT			(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CC_NSDMI
#define ULIBPP_CC_NSDMI						(ULIBPP_CC_VERSION>=407)
#endif
#ifndef ULIBPP_CC_VARIADIC_TEMPLATES
#define ULIBPP_CC_VARIADIC_TEMPLATES		(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CC_VARIADIC_TEMPLATES_TEMPAR
#define ULIBPP_CC_VARIADIC_TEMPLATES_TEMPAR	(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_INITIALIZER_LIST
#define ULIBPP_CC_INITIALIZER_LIST			(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_STATIC_ASSERT
#define ULIBPP_CC_STATIC_ASSERT				(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CC_AUTO
#define ULIBPP_CC_AUTO						(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_AUTO_MULTIDECL
#define ULIBPP_CC_AUTO_MULTIDECL			(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_AUTO_REMOVED
#define ULIBPP_CC_AUTO_REMOVED				(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_NEW_FN_SYNTAX
#define ULIBPP_CC_NEW_FN_SYNTAX				(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_LAMBDAS
#define ULIBPP_CC_LAMBDAS					(ULIBPP_CC_VERSION>=405)
#endif
#ifndef ULIBPP_CC_DECLTYPE
#define ULIBPP_CC_DECLTYPE					(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CC_DECLTYPE_RETTYPE
#define ULIBPP_CC_DECLTYPE_RETTYPE			(ULIBPP_CC_VERSION>=ULIBPP_CC_VERSION==408 && __GNUC_PATCHLEVEL__>=1 || ULIBPP_CC_VERSION>408)
#endif
#ifndef ULIBPP_CC_RABRA
#define ULIBPP_CC_RABRA						(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CC_DEFAULT_TEMPL_ARG
#define ULIBPP_CC_DEFAULT_TEMPL_ARG			(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CC_SFINAE
#define ULIBPP_CC_SFINAE					(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_TEMPLATE_ALIASES
#define ULIBPP_CC_TEMPLATE_ALIASES			(ULIBPP_CC_VERSION>=407)
#endif
#ifndef ULIBPP_CC_EXTERN_TEMPLATES
#define ULIBPP_CC_EXTERN_TEMPLATES			(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CC_NULLPTR
#define ULIBPP_CC_NULLPTR					(ULIBPP_CC_VERSION>=406)
#endif
#ifndef ULIBPP_CC_STRONGLYTYPED_ENUMS
#define ULIBPP_CC_STRONGLYTYPED_ENUMS		(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_FORWARD_ENUM_DECL
#define ULIBPP_CC_FORWARD_ENUM_DECL			(ULIBPP_CC_VERSION>=406)
#endif
#ifndef ULIBPP_CC_ATTRIBUTES
#define ULIBPP_CC_ATTRIBUTES				(ULIBPP_CC_VERSION>=408)
#endif
#ifndef ULIBPP_CC_CONSTEXPR
#define ULIBPP_CC_CONSTEXPR					(ULIBPP_CC_VERSION>=406)
#endif
#ifndef ULIBPP_CC_ALIGNMENT
#define ULIBPP_CC_ALIGNMENT					(ULIBPP_CC_VERSION>=408)
#endif
#ifndef ULIBPP_CC_COND_SUPPORT
#define ULIBPP_CC_COND_SUPPORT				0
#endif
#ifndef ULIBPP_CC_UNDEF_BEHAVIOR_DIAG_ERR
#define ULIBPP_CC_UNDEF_BEHAVIOR_DIAG_ERR	0
#endif
#ifndef ULIBPP_CC_DELEGATING_CONSTRUCTORS
#define ULIBPP_CC_DELEGATING_CONSTRUCTORS	(ULIBPP_CC_VERSION>=407)
#endif
#ifndef ULIBPP_CC_INHERITINING_CONSTRUCTORS
#define ULIBPP_CC_INHERITINING_CONSTRUCTORS	(ULIBPP_CC_VERSION>=408)
#endif
#ifndef ULIBPP_CC_EXPLICIT_CONVERSION_OP
#define ULIBPP_CC_EXPLICIT_CONVERSION_OP	(ULIBPP_CC_VERSION>=405)
#endif
#ifndef ULIBPP_CC_NEW_CHAR_TYPES
#define ULIBPP_CC_NEW_CHAR_TYPES			(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_UNICODE_STRINGS
#define ULIBPP_CC_UNICODE_STRINGS			(ULIBPP_CC_VERSION>=405)
#endif
#ifndef ULIBPP_CC_RAW_STRINGS
#define ULIBPP_CC_RAW_STRINGS				(ULIBPP_CC_VERSION>=405)
#endif
#ifndef ULIBPP_CC_UNICODE_NAMES_IN_LITERALS
#define ULIBPP_CC_UNICODE_NAMES_IN_LITERALS	(ULIBPP_CC_VERSION>=405)
#endif
#ifndef ULIBPP_CC_USERDEF_LITERALS
#define ULIBPP_CC_USERDEF_LITERALS			(ULIBPP_CC_VERSION>=407)
#endif
#ifndef ULIBPP_CC_STD_LAYOUT_TYPES
#define ULIBPP_CC_STD_LAYOUT_TYPES			(ULIBPP_CC_VERSION>=405)
#endif
#ifndef ULIBPP_CC_DEFAULTED_FNCS
#define ULIBPP_CC_DEFAULTED_FNCS			(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_DELETED_FNCS
#define ULIBPP_CC_DELETED_FNCS				(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_EXTENDED_FRIENDS
#define ULIBPP_CC_EXTENDED_FRIENDS			(ULIBPP_CC_VERSION>=407)
#endif
#ifndef ULIBPP_CC_EXTENDED_SIZEOF
#define ULIBPP_CC_EXTENDED_SIZEOF			(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_NAMESPACE_ASSOCIATION
#define ULIBPP_CC_NAMESPACE_ASSOCIATION		(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_UNRESTRICTED_UNIONS
#define ULIBPP_CC_UNRESTRICTED_UNIONS		(ULIBPP_CC_VERSION>=406)
#endif
#ifndef ULIBPP_CC_LOCUN_TYPES_AS_TEMPLARGS
#define ULIBPP_CC_LOCUN_TYPES_AS_TEMPLARGS	(ULIBPP_CC_VERSION>=405)
#endif
#ifndef ULIBPP_CC_RANGE_BASED_FOR
#define ULIBPP_CC_RANGE_BASED_FOR			(ULIBPP_CC_VERSION>=406)
#endif
#ifndef ULIBPP_CC_OVERRIDE_FINAL
#define ULIBPP_CC_OVERRIDE_FINAL			(ULIBPP_CC_VERSION>=407)
#endif
#ifndef ULIBPP_CC_MIN_SUPPORT_FOR_GC
#define ULIBPP_CC_MIN_SUPPORT_FOR_GC		0
#endif
#ifndef ULIBPP_CC_NOEXCEPT
#define ULIBPP_CC_NOEXCEPT					(ULIBPP_CC_VERSION>=406)
#endif
#ifndef ULIBPP_CC_SPEC_MOVE_FNC
#define ULIBPP_CC_SPEC_MOVE_FNC				(ULIBPP_CC_VERSION>=406)
#endif
// concurrency
#ifndef ULIBPP_CC_SEQ_POINTS
#define ULIBPP_CC_SEQ_POINTS				(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CC_ATOMICS
#define ULIBPP_CC_ATOMICS					(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_STRONG_CAEX
#define ULIBPP_CC_STRONG_CAEX				(ULIBPP_CC_VERSION>=405)
#endif
#ifndef ULIBPP_CC_BIDIFENCES
#define ULIBPP_CC_BIDIFENCES				(ULIBPP_CC_VERSION>=408)
#endif
#ifndef ULIBPP_CC_MEM_MODEL
#define ULIBPP_CC_MEM_MODEL					(ULIBPP_CC_VERSION>=408)
#endif
#ifndef ULIBPP_CC_DATADEP_ORDER
#define ULIBPP_CC_DATADEP_ORDER				(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_EXCEPTION_PROPAG
#define ULIBPP_CC_EXCEPTION_PROPAG			(ULIBPP_CC_VERSION>=404)
#endif
#ifndef ULIBPP_CC_ATOMICS_IN_SIGNAL
#define ULIBPP_CC_ATOMICS_IN_SIGNAL			(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CC_TLS
#define ULIBPP_CC_TLS						(ULIBPP_CC_VERSION>=408)
#endif
#ifndef ULIBPP_CC_CONCUR_INIT_DESTR
#define ULIBPP_CC_CONCUR_INIT_DESTR			(ULIBPP_CC_VERSION>=403)
#endif
// C99
#ifndef ULIBPP_CC_FUNC_PREDEF
#define ULIBPP_CC_FUNC_PREDEF				(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CC_C
#define ULIBPP_CC_C99_PP					(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CC_LONG_LONG
#define ULIBPP_CC_LONG_LONG					(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CC_EXT_INT_TYPES
#define ULIBPP_CC_EXT_INT_TYPES				(ULIBPP_CC_VERSION>=403)
#endif
// library
#ifndef ULIBPP_CCLIB_BUILTIN_TYPE_TRAITS
#define ULIBPP_CCLIB_BUILTIN_TYPE_TRAITS		(ULIBPP_CC_VERSION>=403)
#endif
#ifndef ULIBPP_CCLIB_ADDRESSOF
#define ULIBPP_CCLIB_ADDRESSOF					(ULIBPP_CC_VERSION>=409)
#endif
#ifndef ULIBPP_CCLIB_REGEX
#define ULIBPP_CCLIB_REGEX						(ULIBPP_CC_VERSION>=409)
#endif

// C++ 14
// https://gcc.gnu.org/projects/cxx-status.html#cxx14
// language
#ifndef ULIBPP_CC_TWEAKED_CONTEXT_CONVERS
#define ULIBPP_CC_TWEAKED_CONTEXT_CONVERS	(ULIBPP_CC_VERSION>=409)
#endif
#ifndef ULIBPP_CC_BINARY_LITERALS
#define ULIBPP_CC_BINARY_LITERALS			(ULIBPP_CC_VERSION>=409)
#endif
#ifndef ULIBPP_CC_RETURN_TYPE_DEDUCTION
#define ULIBPP_CC_RETURN_TYPE_DEDUCTION		(ULIBPP_CC_VERSION>=409)
#endif
#ifndef ULIBPP_CC_INIT_LAMBDA_CAPTURES
#define ULIBPP_CC_INIT_LAMBDA_CAPTURES		(ULIBPP_CC_VERSION>=409)
#endif
#ifndef ULIBPP_CC_GENERIC_LAMBDAS
#define ULIBPP_CC_GENERIC_LAMBDAS			(ULIBPP_CC_VERSION>=409)
#endif
#ifndef ULIBPP_CC_VARIABLE_TEMPLATES
#define ULIBPP_CC_VARIABLE_TEMPLATES		(ULIBPP_CC_VERSION>=500)
#endif
#ifndef ULIBPP_CC_RELAX_CONSTEXPR
#define ULIBPP_CC_RELAX_CONSTEXPR			(ULIBPP_CC_VERSION>=501)
#endif
#ifndef ULIBPP_CC_AGGR_NSDMI
#define ULIBPP_CC_AGGR_NSDMI				(ULIBPP_CC_VERSION>=501)
#endif
#ifndef ULIBPP_CC_CLARIFY_MEMORY_ALLOC
#define ULIBPP_CC_CLARIFY_MEMORY_ALLOC		(ULIBPP_CC_VERSION>=409)
#endif
#ifndef ULIBPP_CC_DEPRECATED_ATTR
#define ULIBPP_CC_DEPRECATED_ATTR			(ULIBPP_CC_VERSION>=409)
#endif
#ifndef ULIBPP_CC_DIGIT_SEPARATOR
#define ULIBPP_CC_DIGIT_SEPARATOR			(ULIBPP_CC_VERSION>=409)
#endif
#ifndef ULIBPP_CC_SIZED_DEALLOC
#define ULIBPP_CC_SIZED_DEALLOC				(ULIBPP_CC_VERSION>=501)
#endif
// library
#ifndef ULIBPP_CCLIB_MAKE_UNIQUE
#define ULIBPP_CCLIB_MAKE_UNIQUE			(ULIBPP_CC_VERSION>=409)
#endif

// C++17
// https://gcc.gnu.org/projects/cxx-status.html#cxx1z
// language
#ifndef ULIBPP_CC_TERSE_STATIC_ASSERT
#define ULIBPP_CC_TERSE_STATIC_ASSERT		(ULIBPP_CC_VERSION>=600)
#endif
#ifndef ULIBPP_CC_REMOVING_TRIGRAPHS
#define ULIBPP_CC_REMOVING_TRIGRAPHS		(ULIBPP_CC_VERSION>=501)
#endif
#ifndef ULIBPP_CC_TYPENAME_IN_TEMPL_TEMPL
#define ULIBPP_CC_TYPENAME_IN_TEMPL_TEMPL	(ULIBPP_CC_VERSION>=501)
#endif
#ifndef ULIBPP_CC_AUTO_BRACED_LIST
#define ULIBPP_CC_AUTO_BRACED_LIST			(ULIBPP_CC_VERSION>=501)
#endif
#ifndef ULIBPP_CC_FOLD_EXPR
#define ULIBPP_CC_FOLD_EXPR					(ULIBPP_CC_VERSION>=600)
#endif
#ifndef ULIBPP_CC_U
#define ULIBPP_CC_U8_LITERALS				(ULIBPP_CC_VERSION>=600)
#endif
#ifndef ULIBPP_CC_NESTED_NAMESPACE_DEF
#define ULIBPP_CC_NESTED_NAMESPACE_DEF		(ULIBPP_CC_VERSION>=600)
#endif
#ifndef ULIBPP_CC_ATTRS_NAMESPACES_ENUMS
#define ULIBPP_CC_ATTRS_NAMESPACES_ENUMS	(ULIBPP_CC_VERSION>=600)
#endif
#ifndef ULIBPP_CC_MORE_NONTYPE_TEMPL_ARGS
#define ULIBPP_CC_MORE_NONTYPE_TEMPL_ARGS	(ULIBPP_CC_VERSION>=600)
#endif
#ifndef ULIBPP_CC_REMOVED_REGISTER_STC
#define ULIBPP_CC_REMOVED_REGISTER_STC		(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_REMOVED_BOOL_INCR
#define ULIBPP_CC_REMOVED_BOOL_INCR			(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_EXCSPEC_IN_TYPESYS
#define ULIBPP_CC_EXCSPEC_IN_TYPESYS		(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_HAS_INCLUDE
#define ULIBPP_CC_HAS_INCLUDE				(ULIBPP_CC_VERSION>=500)
#endif
#ifndef ULIBPP_CC_NEW_INHERIT_CONSTR
#define ULIBPP_CC_NEW_INHERIT_CONSTR		(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_FALLTHROUGH_ATTR
#define ULIBPP_CC_FALLTHROUGH_ATTR			(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_NODISCARD_ATTR
#define ULIBPP_CC_NODISCARD_ATTR			(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_MAYBEUNUSED_ATTR
#define ULIBPP_CC_MAYBEUNUSED_ATTR			(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_AGGR_BASE_CLASS_INIT
#define ULIBPP_CC_AGGR_BASE_CLASS_INIT		(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_CONSTEXPR_LAMBDA
#define ULIBPP_CC_CONSTEXPR_LAMBDA			(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_DIFF_BEGIN_END_FOR
#define ULIBPP_CC_DIFF_BEGIN_END_FOR		(ULIBPP_CC_VERSION>=600)
#endif
#ifndef ULIBPP_CC_LAMBDA_CAPTURE_DEREFTHIS
#define ULIBPP_CC_LAMBDA_CAPTURE_DEREFTHIS	(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_DIRECT_LIST_INIT_OF_ENUMS
#define ULIBPP_CC_DIRECT_LIST_INIT_OF_ENUMS	(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_HEXA_FLOAT_LITERALS
#define ULIBPP_CC_HEXA_FLOAT_LITERALS		(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_NS_ATTRS_WO_REPETITION
#define ULIBPP_CC_NS_ATTRS_WO_REPETITION	(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_ALIGNED_NEW
#define ULIBPP_CC_ALIGNED_NEW				(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_TEMPL_DEDUCTION_FOR_CLASS
#define ULIBPP_CC_TEMPL_DEDUCTION_FOR_CLASS	(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_AUTO_FOR_NOTYPE_TEMPL_ARG
#define ULIBPP_CC_AUTO_FOR_NOTYPE_TEMPL_ARG	(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_GUARANTEED_COPY_ELISION
#define ULIBPP_CC_GUARANTEED_COPY_ELISION	(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_STRICTER_EXPR_EVAL_ORDER
#define ULIBPP_CC_STRICTER_EXPR_EVAL_ORDER	(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_IGNORE_UNKNOWN_ATTR
#define ULIBPP_CC_IGNORE_UNKNOWN_ATTR		(ULIBPP_CC_VERSION>=300)
#endif
#ifndef ULIBPP_CC_CONSTEXPR_IF
#define ULIBPP_CC_CONSTEXPR_IF				(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_INLINE_VARIABLES
#define ULIBPP_CC_INLINE_VARIABLES			(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_STRUCTURED_BINDING
#define ULIBPP_CC_STRUCTURED_BINDING		(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_INIT_EXPR_FOR_IF_SWITCH
#define ULIBPP_CC_INIT_EXPR_FOR_IF_SWITCH	(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_MATCH_TEMPL_TEMPL_ARGS
#define ULIBPP_CC_MATCH_TEMPL_TEMPL_ARGS	(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_FIX_QUALIF_CONVERS
#define ULIBPP_CC_FIX_QUALIF_CONVERS		0
#endif
#ifndef ULIBPP_CC_REMOVE_EXCEP_SPEC
#define ULIBPP_CC_REMOVE_EXCEP_SPEC			(ULIBPP_CC_VERSION>=700)
#endif
#ifndef ULIBPP_CC_PACK_EXP_IN_USING
#define ULIBPP_CC_PACK_EXP_IN_USING			(ULIBPP_CC_VERSION>=700)
#endif
// library
#ifndef ULIBPP_CCLIB_PARA_ALG
#define ULIBPP_CCLIB_PARA_ALG					0
#endif
#ifndef ULIBPP_CC_ALIGNED_ALLOC
#define ULIBPP_CC_ALIGNED_ALLOC				(ULIBPP_CC_VERSION>=800)
#endif

// C++20
// language
#ifndef ULIBPP_CC_DFLT_INIT_BITFLD
#define ULIBPP_CC_DFLT_INIT_BITFLD			(ULIBPP_CC_VERSION>=800)
#endif
#ifndef ULIBPP_CC_CONST_MEMPTR
#define ULIBPP_CC_CONST_MEMPTR			(ULIBPP_CC_VERSION>=800)
#endif
#ifndef ULIBPP_CC_ALLOW_LAMBDA_CAPTURE
#define ULIBPP_CC_ALLOW_LAMBDA_CAPTURE		(ULIBPP_CC_VERSION>=800)
#endif
#ifndef ULIBPP_CC_VAOPT_COMMA_ELISION
#define ULIBPP_CC_VAOPT_COMMA_ELISION		0
#endif
#ifndef ULIBPP_CC_DESIGN_INIT
#define ULIBPP_CC_DESIGN_INIT				(ULIBPP_CC_VERSION>=800)
#endif
#ifndef ULIBPP_CC_TEMPL_PARLIST_GEN_LAMDAS
#define ULIBPP_CC_TEMPL_PARLIST_GEN_LAMDAS		(ULIBPP_CC_VERSION>=800)
#endif
#ifndef ULIBPP_CC_INITL_CLASS_TEMPL_DEDUCT
#define ULIBPP_CC_INITL_CLASS_TEMPL_DEDUCT	(ULIBPP_CC_VERSION>=800)
#endif
#ifndef ULIBPP_CC_CONCEPTS
#define ULIBPP_CC_CONCEPTS					0
#endif
#ifndef ULIBPP_CC_RANGE_FOR_WITH_INIT
#define ULIBPP_CC_RANGE_FOR_WITH_INIT		0
#endif
#ifndef ULIBPP_CC_SIMPLY_IMPL_LAMDA_CAPT
#define ULIBPP_CC_SIMPLY_IMPL_LAMDA_CAPT	(ULIBPP_CC_VERSION>=800)
#endif
#ifndef ULIBPP_CC_ADL_TEMPL_NOT_VISIBLE
#define ULIBPP_CC_ADL_TEMPL_NOT_VISIBLE		0
#endif
#ifndef ULIBPP_CC_CONST_MIS_DFLT_COPYCONST
#define ULIBPP_CC_CONST_MIS_DFLT_COPYCONST	0
#endif
#ifndef ULIBPP_CC_LESS_EAGER_INST_CONSTEXPR
#define ULIBPP_CC_LESS_EAGER_INST_CONSTEXPR	0
#endif
#ifndef ULIBPP_CC_CONSISTENT_COMPARISON
#define ULIBPP_CC_CONSISTENT_COMPARISON		0
#endif
#ifndef ULIBPP_CC_ACC_CHECK_ON_SPEC
#define ULIBPP_CC_ACC_CHECK_ON_SPEC			0
#endif
#ifndef ULIBPP_CC_DFLT_STATELESS_LAMBDA
#define ULIBPP_CC_DFLT_STATELESS_LAMBDA		0
#endif
#ifndef ULIBPP_CC_LAMBDA_UNEVAL_CTX
#define ULIBPP_CC_LAMBDA_UNEVAL_CTX			0
#endif
#ifndef ULIBPP_CC_EMPTY_OBJECTS
#define ULIBPP_CC_EMPTY_OBJECTS				0
#endif
#ifndef ULIBPP_CC_RELAX_RANGE_FOR
#define ULIBPP_CC_RELAX_RANGE_FOR			(ULIBPP_CC_VERSION>=800)
#endif
#ifndef ULIBPP_CC_ALLOW_STRUCT_BIND
#define ULIBPP_CC_ALLOW_STRUCT_BIND			0
#endif
#ifndef ULIBPP_CC_RELAX_STRUCT_BIND
#define ULIBPP_CC_RELAX_STRUCT_BIND			0
#endif
#ifndef ULIBPP_CC_DOWN_WITH_TYPENAME
#define ULIBPP_CC_DOWN_WITH_TYPENAME		0
#endif
#ifndef ULIBPP_CC_ALLOW_PACK_EXP_IN_LAMBDA
#define ULIBPP_CC_ALLOW_PACK_EXP_IN_LAMBDA	0
#endif
#ifndef ULIBPP_CC_LIKELY_UNLIKELY
#define ULIBPP_CC_LIKELY_UNLIKELY			0
#endif

#elif defined(_MSC_VER) /* beware -- many compilers emulates MSVC by defining _MSC_VER too */
#if _MSC_VER < 1310						/* 1310 <-> MSVC 7.1 */
#error Not tested with MSVC < 7.1 (.NET 2003)
#else
#define ULIBPP_CC_PLATFORM				ULIBPP_CC_PLATFORM_MSVC
#endif

#define ULIBPP_CC_VERSION				_MSC_VER

#define ULIBPP_CC_STDCPP11				(ULIBPP_CC_VERSION>=1700)

// C++ 11
// http://blogs.msdn.com/b/vcblog/archive/2014/08/21/c-11-14-features-in-visual-studio-14-ctp3.aspx
// https://blogs.msdn.microsoft.com/vcblog/2015/06/19/c111417-features-in-vs-2015-rtm/
// https://blogs.msdn.microsoft.com/vcblog/2017/12/19/c17-progress-in-vs-2017-15-5-and-15-6/
// language
#ifndef ULIBPP_CC_RVALUE
#define ULIBPP_CC_RVALUE					(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_REF_QUALIFIERS
#define ULIBPP_CC_REF_QUALIFIERS			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_RVALUE_OBJINIT
#define ULIBPP_CC_RVALUE_OBJINIT			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_NSDMI
#define ULIBPP_CC_NSDMI						(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_VARIADIC_TEMPLATES
#define ULIBPP_CC_VARIADIC_TEMPLATES		(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_VARIADIC_TEMPLATES_TEMPAR
#define ULIBPP_CC_VARIADIC_TEMPLATES_TEMPAR	(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_INITIALIZER_LIST
#define ULIBPP_CC_INITIALIZER_LIST			(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_STATIC_ASSERT
#define ULIBPP_CC_STATIC_ASSERT				(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_AUTO
#define ULIBPP_CC_AUTO						(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_AUTO_MULTIDECL
#define ULIBPP_CC_AUTO_MULTIDECL			(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_AUTO_REMOVED
#define ULIBPP_CC_AUTO_REMOVED				(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_NEW_FN_SYNTAX
#define ULIBPP_CC_NEW_FN_SYNTAX				(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_LAMBDAS
#define ULIBPP_CC_LAMBDAS					(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_DECLTYPE
#define ULIBPP_CC_DECLTYPE					(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_DECLTYPE_RETTYPE
#define ULIBPP_CC_DECLTYPE_RETTYPE			(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_RABRA
#define ULIBPP_CC_RABRA						(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_DEFAULT_TEMPL_ARG
#define ULIBPP_CC_DEFAULT_TEMPL_ARG			(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_SFINAE
#define ULIBPP_CC_SFINAE					(ULIBPP_CC_VERSION>=1914)
#endif
#ifndef ULIBPP_CC_TEMPLATE_ALIASES
#define ULIBPP_CC_TEMPLATE_ALIASES			(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_EXTERN_TEMPLATES
#define ULIBPP_CC_EXTERN_TEMPLATES			(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_NULLPTR
#define ULIBPP_CC_NULLPTR					(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_STRONGLYTYPED_ENUMS
#define ULIBPP_CC_STRONGLYTYPED_ENUMS		(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_FORWARD_ENUM_DECL
#define ULIBPP_CC_FORWARD_ENUM_DECL			(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_ATTRIBUTES
#define ULIBPP_CC_ATTRIBUTES				(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_CONSTEXPR
#define ULIBPP_CC_CONSTEXPR					(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_ALIGNMENT
#define ULIBPP_CC_ALIGNMENT					(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_COND_SUPPORT
#define ULIBPP_CC_COND_SUPPORT				0
#endif
#ifndef ULIBPP_CC_UNDEF_BEHAVIOR_DIAG_ERR
#define ULIBPP_CC_UNDEF_BEHAVIOR_DIAG_ERR	0
#endif
#ifndef ULIBPP_CC_DELEGATING_CONSTRUCTORS
#define ULIBPP_CC_DELEGATING_CONSTRUCTORS	(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_INHERITINING_CONSTRUCTORS
#define ULIBPP_CC_INHERITINING_CONSTRUCTORS	(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_EXPLICIT_CONVERSION_OP
#define ULIBPP_CC_EXPLICIT_CONVERSION_OP	(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_NEW_CHAR_TYPES
#define ULIBPP_CC_NEW_CHAR_TYPES			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_UNICODE_STRINGS
#define ULIBPP_CC_UNICODE_STRINGS			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_RAW_STRINGS
#define ULIBPP_CC_RAW_STRINGS				(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_UNICODE_NAMES_IN_LITERALS
#define ULIBPP_CC_UNICODE_NAMES_IN_LITERALS	(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_USERDEF_LITERALS
#define ULIBPP_CC_USERDEF_LITERALS			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_STD_LAYOUT_TYPES
#define ULIBPP_CC_STD_LAYOUT_TYPES			(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_DEFAULTED_FNCS
#define ULIBPP_CC_DEFAULTED_FNCS			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_DELETED_FNCS
#define ULIBPP_CC_DELETED_FNCS				(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_EXTENDED_FRIENDS
#define ULIBPP_CC_EXTENDED_FRIENDS			(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_EXTENDED_SIZEOF
#define ULIBPP_CC_EXTENDED_SIZEOF			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_NAMESPACE_ASSOCIATION
#define ULIBPP_CC_NAMESPACE_ASSOCIATION		(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_UNRESTRICTED_UNIONS
#define ULIBPP_CC_UNRESTRICTED_UNIONS		(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_LOCUN_TYPES_AS_TEMPLARGS
#define ULIBPP_CC_LOCUN_TYPES_AS_TEMPLARGS	(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_RANGE_BASED_FOR
#define ULIBPP_CC_RANGE_BASED_FOR			(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_OVERRIDE_FINAL
#define ULIBPP_CC_OVERRIDE_FINAL			(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_MIN_SUPPORT_FOR_GC
#define ULIBPP_CC_MIN_SUPPORT_FOR_GC		(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_NOEXCEPT
#define ULIBPP_CC_NOEXCEPT					(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_SPEC_MOVE_FNC
#define ULIBPP_CC_SPEC_MOVE_FNC				(ULIBPP_CC_VERSION>=1900)
#endif
// concurrency
#ifndef ULIBPP_CC_SEQ_POINTS
#define ULIBPP_CC_SEQ_POINTS				(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_ATOMICS
#define ULIBPP_CC_ATOMICS					(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_STRONG_CAEX
#define ULIBPP_CC_STRONG_CAEX				(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_BIDIFENCES
#define ULIBPP_CC_BIDIFENCES				(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_MEM_MODEL
#define ULIBPP_CC_MEM_MODEL					(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_DATADEP_ORDER
#define ULIBPP_CC_DATADEP_ORDER				(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CC_EXCEPTION_PROPAG
#define ULIBPP_CC_EXCEPTION_PROPAG			(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_ATOMICS_IN_SIGNAL
#define ULIBPP_CC_ATOMICS_IN_SIGNAL			(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_TLS
#define ULIBPP_CC_TLS						(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_CONCUR_INIT_DESTR
#define ULIBPP_CC_CONCUR_INIT_DESTR			(ULIBPP_CC_VERSION>=1900)
#endif
// C99
#ifndef ULIBPP_CC_FUNC_PREDEF
#define ULIBPP_CC_FUNC_PREDEF				(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_C
#define ULIBPP_CC_C99_PP					0
#endif
#ifndef ULIBPP_CC_LONG_LONG
#define ULIBPP_CC_LONG_LONG					(ULIBPP_CC_VERSION>=1600)
#endif
#ifndef ULIBPP_CC_EXT_INT_TYPES
#define ULIBPP_CC_EXT_INT_TYPES				0
#endif
// library
#ifndef ULIBPP_CCLIB_BUILTIN_TYPE_TRAITS
#define ULIBPP_CCLIB_BUILTIN_TYPE_TRAITS		(ULIBPP_CC_VERSION>=1400)
#endif
#ifndef ULIBPP_CCLIB_ADDRESSOF
#define ULIBPP_CCLIB_ADDRESSOF					(ULIBPP_CC_VERSION>=1700)
#endif
#ifndef ULIBPP_CCLIB_REGEX
#define ULIBPP_CCLIB_REGEX						(ULIBPP_CC_VERSION>=1700)
#endif

// C++ 14
// language
#ifndef ULIBPP_CC_TWEAKED_CONTEXT_CONVERS
#define ULIBPP_CC_TWEAKED_CONTEXT_CONVERS	(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_BINARY_LITERALS
#define ULIBPP_CC_BINARY_LITERALS			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_RETURN_TYPE_DEDUCTION
#define ULIBPP_CC_RETURN_TYPE_DEDUCTION		(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_INIT_LAMBDA_CAPTURES
#define ULIBPP_CC_INIT_LAMBDA_CAPTURES		(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_GENERIC_LAMBDAS
#define ULIBPP_CC_GENERIC_LAMBDAS			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_VARIABLE_TEMPLATES
#define ULIBPP_CC_VARIABLE_TEMPLATES		(ULIBPP_CC_VERSION>=1900 && _MSC_FULL_VER>=190023918)
#endif
#ifndef ULIBPP_CC_RELAX_CONSTEXPR
#define ULIBPP_CC_RELAX_CONSTEXPR			(ULIBPP_CC_VERSION>=1910)
#endif
#ifndef ULIBPP_CC_AGGR_NSDMI
#define ULIBPP_CC_AGGR_NSDMI				(ULIBPP_CC_VERSION>=1910)
#endif
#ifndef ULIBPP_CC_CLARIFY_MEMORY_ALLOC
#define ULIBPP_CC_CLARIFY_MEMORY_ALLOC		0
#endif
#ifndef ULIBPP_CC_DEPRECATED_ATTR
#define ULIBPP_CC_DEPRECATED_ATTR			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_DIGIT_SEPARATOR
#define ULIBPP_CC_DIGIT_SEPARATOR			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_SIZED_DEALLOC
#define ULIBPP_CC_SIZED_DEALLOC				(ULIBPP_CC_VERSION>=1900)
#endif
// library
#ifndef ULIBPP_CCLIB_MAKE_UNIQUE
#define ULIBPP_CCLIB_MAKE_UNIQUE			(ULIBPP_CC_VERSION>=1800)
#endif

// C++17
// language
#ifndef ULIBPP_CC_TERSE_STATIC_ASSERT
#define ULIBPP_CC_TERSE_STATIC_ASSERT		(ULIBPP_CC_VERSION>=1910)
#endif
#ifndef ULIBPP_CC_REMOVING_TRIGRAPHS
#define ULIBPP_CC_REMOVING_TRIGRAPHS		(ULIBPP_CC_VERSION>=1800)
#endif
#ifndef ULIBPP_CC_TYPENAME_IN_TEMPL_TEMPL
#define ULIBPP_CC_TYPENAME_IN_TEMPL_TEMPL	(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_AUTO_BRACED_LIST
#define ULIBPP_CC_AUTO_BRACED_LIST			(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_FOLD_EXPR
#define ULIBPP_CC_FOLD_EXPR					(ULIBPP_CC_VERSION>=1912)
#endif
#ifndef ULIBPP_CC_U
#define ULIBPP_CC_U8_LITERALS				(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_NESTED_NAMESPACE_DEF
#define ULIBPP_CC_NESTED_NAMESPACE_DEF		(ULIBPP_CC_VERSION>=1900 && _MSC_FULL_VER>=190024210)
#endif
#ifndef ULIBPP_CC_ATTRS_NAMESPACES_ENUMS
#define ULIBPP_CC_ATTRS_NAMESPACES_ENUMS	(ULIBPP_CC_VERSION>=1900)
#endif
#ifndef ULIBPP_CC_MORE_NONTYPE_TEMPL_ARGS
#define ULIBPP_CC_MORE_NONTYPE_TEMPL_ARGS	(ULIBPP_CC_VERSION>=1912)
#endif
#ifndef ULIBPP_CC_REMOVED_REGISTER_STC
#define ULIBPP_CC_REMOVED_REGISTER_STC		(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_REMOVED_BOOL_INCR
#define ULIBPP_CC_REMOVED_BOOL_INCR			(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_EXCSPEC_IN_TYPESYS
#define ULIBPP_CC_EXCSPEC_IN_TYPESYS		(ULIBPP_CC_VERSION>=1912)
#endif
#ifndef ULIBPP_CC_HAS_INCLUDE
#define ULIBPP_CC_HAS_INCLUDE				(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_NEW_INHERIT_CONSTR
#define ULIBPP_CC_NEW_INHERIT_CONSTR		(ULIBPP_CC_VERSION>=1914)
#endif
#ifndef ULIBPP_CC_FALLTHROUGH_ATTR
#define ULIBPP_CC_FALLTHROUGH_ATTR			(ULIBPP_CC_VERSION>=1910)
#endif
#ifndef ULIBPP_CC_NODISCARD_ATTR
#define ULIBPP_CC_NODISCARD_ATTR			(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_MAYBEUNUSED_ATTR
#define ULIBPP_CC_MAYBEUNUSED_ATTR			(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_AGGR_BASE_CLASS_INIT
#define ULIBPP_CC_AGGR_BASE_CLASS_INIT		(ULIBPP_CC_VERSION>=1914)
#endif
#ifndef ULIBPP_CC_CONSTEXPR_LAMBDA
#define ULIBPP_CC_CONSTEXPR_LAMBDA			(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_DIFF_BEGIN_END_FOR
#define ULIBPP_CC_DIFF_BEGIN_END_FOR		(ULIBPP_CC_VERSION>=1910)
#endif
#ifndef ULIBPP_CC_LAMBDA_CAPTURE_DEREFTHIS
#define ULIBPP_CC_LAMBDA_CAPTURE_DEREFTHIS	(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_DIRECT_LIST_INIT_OF_ENUMS
#define ULIBPP_CC_DIRECT_LIST_INIT_OF_ENUMS	(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_HEXA_FLOAT_LITERALS
#define ULIBPP_CC_HEXA_FLOAT_LITERALS		(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_NS_ATTRS_WO_REPETITION
#define ULIBPP_CC_NS_ATTRS_WO_REPETITION	(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_ALIGNED_NEW
#define ULIBPP_CC_ALIGNED_NEW				(ULIBPP_CC_VERSION>=1912)
#endif
#ifndef ULIBPP_CC_TEMPL_DEDUCTION_FOR_CLASS
#define ULIBPP_CC_TEMPL_DEDUCTION_FOR_CLASS	(ULIBPP_CC_VERSION>=1914)
#endif
#ifndef ULIBPP_CC_AUTO_FOR_NOTYPE_TEMPL_ARG
#define ULIBPP_CC_AUTO_FOR_NOTYPE_TEMPL_ARG	(ULIBPP_CC_VERSION>=1914)
#endif
#ifndef ULIBPP_CC_GUARANTEED_COPY_ELISION
#define ULIBPP_CC_GUARANTEED_COPY_ELISION	(ULIBPP_CC_VERSION>=1913)
#endif
#ifndef ULIBPP_CC_STRICTER_EXPR_EVAL_ORDER
#define ULIBPP_CC_STRICTER_EXPR_EVAL_ORDER	(ULIBPP_CC_VERSION>=1914)
#endif
#ifndef ULIBPP_CC_IGNORE_UNKNOWN_ATTR
#define ULIBPP_CC_IGNORE_UNKNOWN_ATTR		(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_CONSTEXPR_IF
#define ULIBPP_CC_CONSTEXPR_IF				(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_INLINE_VARIABLES
#define ULIBPP_CC_INLINE_VARIABLES			(ULIBPP_CC_VERSION>=1912)
#endif
#ifndef ULIBPP_CC_STRUCTURED_BINDING
#define ULIBPP_CC_STRUCTURED_BINDING		(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_INIT_EXPR_FOR_IF_SWITCH
#define ULIBPP_CC_INIT_EXPR_FOR_IF_SWITCH	(ULIBPP_CC_VERSION>=1911)
#endif
#ifndef ULIBPP_CC_MATCH_TEMPL_TEMPL_ARGS
#define ULIBPP_CC_MATCH_TEMPL_TEMPL_ARGS	(ULIBPP_CC_VERSION>=1912)
#endif
#ifndef ULIBPP_CC_FIX_QUALIF_CONVERS
#define ULIBPP_CC_FIX_QUALIF_CONVERS		(ULIBPP_CC_VERSION>=1914)
#endif
#ifndef ULIBPP_CC_REMOVE_EXCEP_SPEC
#define ULIBPP_CC_REMOVE_EXCEP_SPEC			(ULIBPP_CC_VERSION>=1912)
#endif
#ifndef ULIBPP_CC_PACK_EXP_IN_USING
#define ULIBPP_CC_PACK_EXP_IN_USING			(ULIBPP_CC_VERSION>=1914)
#endif
// library
#ifndef ULIBPP_CCLIB_PARA_ALG
#define ULIBPP_CCLIB_PARA_ALG					0
#endif
#ifndef ULIBPP_CC_ALIGNED_ALLOC
#define ULIBPP_CC_ALIGNED_ALLOC				0
#endif

// C++20
// language
#ifndef ULIBPP_CC_DFLT_INIT_BITFLD
#define ULIBPP_CC_DFLT_INIT_BITFLD			0
#endif
#ifndef ULIBPP_CC_CONST_MEMPTR
#define ULIBPP_CC_CONST_MEMPTR			0
#endif
#ifndef ULIBPP_CC_ALLOW_LAMBDA_CAPTURE
#define ULIBPP_CC_ALLOW_LAMBDA_CAPTURE		0
#endif
#ifndef ULIBPP_CC_VAOPT_COMMA_ELISION
#define ULIBPP_CC_VAOPT_COMMA_ELISION		0
#endif
#ifndef ULIBPP_CC_DESIGN_INIT
#define ULIBPP_CC_DESIGN_INIT				0
#endif
#ifndef ULIBPP_CC_TEMPL_PARLIST_GEN_LAMDAS
#define ULIBPP_CC_TEMPL_PARLIST_GEN_LAMDAS		0
#endif
#ifndef ULIBPP_CC_INITL_CLASS_TEMPL_DEDUCT
#define ULIBPP_CC_INITL_CLASS_TEMPL_DEDUCT	0
#endif
#ifndef ULIBPP_CC_CONCEPTS
#define ULIBPP_CC_CONCEPTS					0
#endif
#ifndef ULIBPP_CC_RANGE_FOR_WITH_INIT
#define ULIBPP_CC_RANGE_FOR_WITH_INIT		0
#endif
#ifndef ULIBPP_CC_SIMPLY_IMPL_LAMDA_CAPT
#define ULIBPP_CC_SIMPLY_IMPL_LAMDA_CAPT	0
#endif
#ifndef ULIBPP_CC_ADL_TEMPL_NOT_VISIBLE
#define ULIBPP_CC_ADL_TEMPL_NOT_VISIBLE		0
#endif
#ifndef ULIBPP_CC_CONST_MIS_DFLT_COPYCONST
#define ULIBPP_CC_CONST_MIS_DFLT_COPYCONST	0
#endif
#ifndef ULIBPP_CC_LESS_EAGER_INST_CONSTEXPR
#define ULIBPP_CC_LESS_EAGER_INST_CONSTEXPR	0
#endif
#ifndef ULIBPP_CC_CONSISTENT_COMPARISON
#define ULIBPP_CC_CONSISTENT_COMPARISON		0
#endif
#ifndef ULIBPP_CC_ACC_CHECK_ON_SPEC
#define ULIBPP_CC_ACC_CHECK_ON_SPEC			0
#endif
#ifndef ULIBPP_CC_DFLT_STATELESS_LAMBDA
#define ULIBPP_CC_DFLT_STATELESS_LAMBDA		0
#endif
#ifndef ULIBPP_CC_LAMBDA_UNEVAL_CTX
#define ULIBPP_CC_LAMBDA_UNEVAL_CTX			0
#endif
#ifndef ULIBPP_CC_EMPTY_OBJECTS
#define ULIBPP_CC_EMPTY_OBJECTS				0
#endif
#ifndef ULIBPP_CC_RELAX_RANGE_FOR
#define ULIBPP_CC_RELAX_RANGE_FOR			0
#endif
#ifndef ULIBPP_CC_ALLOW_STRUCT_BIND
#define ULIBPP_CC_ALLOW_STRUCT_BIND			0
#endif
#ifndef ULIBPP_CC_RELAX_STRUCT_BIND
#define ULIBPP_CC_RELAX_STRUCT_BIND			0
#endif
#ifndef ULIBPP_CC_DOWN_WITH_TYPENAME
#define ULIBPP_CC_DOWN_WITH_TYPENAME		0
#endif
#ifndef ULIBPP_CC_ALLOW_PACK_EXP_IN_LAMBDA
#define ULIBPP_CC_ALLOW_PACK_EXP_IN_LAMBDA	0
#endif
#ifndef ULIBPP_CC_LIKELY_UNLIKELY
#define ULIBPP_CC_LIKELY_UNLIKELY			0
#endif

#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)

#error Sun compiler is no more supported

#define ULIBPP_CC_PLATFORM				ULIBPP_CC_PLATFORM_SUNCC

#define ULIBPP_CC_VERSION				((__SUNPRO_C&0xff)+((__SUNPRO_C>>8)&0xff)*100)

#else
#error "### Cannot determine compiler breed."
#endif

#endif /* ULIBPP_CC_PLATFORM */

/* ================================================================================================ *
 *		Deriving remaining symbols																	*
 * ================================================================================================ */

#ifndef ULIBPP_LITTLEENDIAN
#if ULIBPP_HW_PLATFORM == ULIBPP_HW_PLATFORM_SPARC ||		\
	ULIBPP_HW_PLATFORM == ULIBPP_HW_PLATFORM_SPARC64
#define ULIBPP_LITTLEENDIAN					0
#else
#define ULIBPP_LITTLEENDIAN					1
#endif
#endif /* ULIBPP_LITTLEENDIAN */

#ifndef ULIBPP_USE_POSIX_WHENEVER_POSSIBLE
#if ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_LINUX ||		\
	ULIBPP_OS_PLATFORM == ULIBPP_OS_PLATFORM_K1OM
#define ULIBPP_USE_POSIX_WHENEVER_POSSIBLE	1
#else
#define ULIBPP_USE_POSIX_WHENEVER_POSSIBLE	0
#endif
#endif /* ULIBPP_USE_POSIX_WHENEVER_POSSIBLE */

#include <cstdint>

namespace ulibpp {

/* ================================================================================================ *
 *		Setting up ulib++ types																		*
 * ================================================================================================ */

}
#endif /* ULIBPP_UPP_GUESS_H_ */

/*

ordering from https://en.cppreference.com/w/cpp/compiler_support
ordered by paper ID

ULIBPP_CC_STDCPP11					- has TR1 incorporated as STD

// C++ 11
// language
ULIBPP_CC_C99_PP					- C99 preprocessor N1653
ULIBPP_CC_STATIC_ASSERT				- static_assert N1720 *
ULIBPP_CC_RABRA						- right angle brackets N1757
ULIBPP_CC_EXTENDED_FRIENDS			- extended friend declarations N1791
ULIBPP_CC_LONG_LONG					- long long N1811
ULIBPP_CC_BUILTIN_TYPE_TRAITS		- built-in type traits N1836
ULIBPP_CC_AUTO						- auto-typed variables N1984
ULIBPP_CC_DELEGATING_CONSTRUCTORS	- delegating constructors N1986 *
ULIBPP_CC_EXTERN_TEMPLATES			- extern templates N1987
ULIBPP_CC_RVALUE					- rvalue references N2118, std::move N3053 *
ULIBPP_CC_CONSTEXPR					- constexpr N2235 *
ULIBPP_CC_VARIADIC_TEMPLATES		- variadic templates N2242 *
ULIBPP_CC_TEMPLATE_ALIASES			- template aliases N2258 *
ULIBPP_CC_NEW_CHAR_TYPES			- new character types N2249 *
ULIBPP_CC_ALIGNMENT					- alignment support N2341
ULIBPP_CC_DECLTYPE					- decltype N2343 *
ULIBPP_CC_DELETED_FNCS				- deleted functions N2346
ULIBPP_CC_STRONGLYTYPED_ENUMS		- strongly-typed enums N2347
ULIBPP_CC_ATOMICS					- atomic operations N2427
ULIBPP_CC_NULLPTR					- nullptr N2431
ULIBPP_CC_EXPLICIT_CONVERSION_OP	- explicit conversion operators N2437
ULIBPP_CC_REF_QUALIFIERS			- rvalue references for *this N2439 *
ULIBPP_CC_UNICODE_STRINGS			- unicode string literals N2442 *
ULIBPP_CC_RAW_STRINGS				- raw string literals N2442 *
ULIBPP_CC_NAMESPACE_ASSOCIATION		- inline namespaces N2535
ULIBPP_CC_INHERITINING_CONSTRUCTORS	- inheriting constructors N2540 *
ULIBPP_CC_NEW_FN_SYNTAX				- new function declarator syntax N2541
ULIBPP_CC_UNRESTRICTED_UNIONS		- unrestricted unions N2544
ULIBPP_CC_VARIADIC_TEMPLATES_TEMPAR	- extending variadic template template parameters N2555
ULIBPP_CC_SFINAE					- solving the SFINAE problem for expressions N2634, DR339
ULIBPP_CC_LOCUN_TYPES_AS_TEMPLARGS	- local and unnamed types as template arguments N2657
ULIBPP_CC_TLS						- thread-local storage N2659
ULIBPP_CC_CONCUR_INIT_DESTR			- Dynamic initialization and destruction with concurrency N2660 *
ULIBPP_CC_MIN_SUPPORT_FOR_GC		- minimal support for garbage collection and reachability-based leak detection N2670
ULIBPP_CC_INITIALIZER_LIST			- initializer lists N2672 *
ULIBPP_CC_NSDMI						- non-static data members initializers N2756 *
ULIBPP_CC_ATTRIBUTES				- attributes N2761 *
ULIBPP_CC_COND_SUPPORT				- conditionally-support behavior N1627
ULIBPP_CC_USERDEF_LITERALS			- user-defined literals N2765 *
ULIBPP_CC_LAMBDAS					- lambda expressions and closures N2927 *
ULIBPP_CC_RANGE_BASED_FOR			- range-based for-loop N2930 *
ULIBPP_CC_NOEXCEPT					- allowing move constructors to throw N3050
ULIBPP_CC_OVERRIDE_FINAL			- explicit virtual overrides N2928, N3206, N3272
ULIBPP_CC_DECLTYPE_RETTYPE			- incomplete return types N3276
// library
ULIBPP_CCLIB_TYPE_TRAITS			- Type traits N1836
ULIBPP_CCLIB_MIN_SUPPORT_FOR_GC		- Garbage Collection and Reachability-Based Leak Detection (library support) N2670
ULIBPP_CCLIB_IOMANIP				- Money, Time, and hexfloat I/O manipulators N2071, N2072
// remnants
ULIBPP_CCLIB_ADDRESSOF				- std::addressof *
ULIBPP_CCLIB_REGEX					- working <regex> implementation

// C++ 14
// language
ULIBPP_CC_TWEAKED_CONTEXT_CONVERS	- Tweak to certain C++ contextual conversions N3323
ULIBPP_CC_BINARY_LITERALS			- Binary literals N3472 *
ULIBPP_CC_RETURN_TYPE_DEDUCTION		- Return type deduction for normal functions N3638 *
ULIBPP_CC_INIT_LAMBDA_CAPTURES		- Initialized lambda captures N3648 *
ULIBPP_CC_GENERIC_LAMBDAS			- Generic lambdas N3649 *
ULIBPP_CC_VARIABLE_TEMPLATES		- Variable templates N3651 *
ULIBPP_CC_RELAX_CONSTEXPR			- Relaxing requirements on constexpr functions N3652 *
ULIBPP_CC_AGGR_NSDMI				- Member initializers and aggregates N3653 *
ULIBPP_CC_CLARIFY_MEMORY_ALLOC		- Clarifying memory allocation N3664
ULIBPP_CC_DEPRECATED_ATTR			- [[deprecated]] attribute N3760
ULIBPP_CC_SIZED_DEALLOC				- C++ Sized Deallocation N3778 *
ULIBPP_CC_DIGIT_SEPARATOR			- Single quotation mark as digit separator N3781 ?
// library
ULIBPP_CCLIB_CONSTEXPR_COMPLEX		- constexpr for <complex> N3302
ULIBPP_CCLIB_RESULT_OF				- std::result_of and SFINAE N3462 *
ULIBPP_CCLIB_CONSTEXPR_CHRONO		- constexpr for <chrono> N3469
ULIBPP_CCLIB_CONSTEXPR_ARRAY		- constexpr for <array> N3470
ULIBPP_CCLIB_CONSTEXPR_UTIL			- constexpr for <initializer_list>, <utility> and <tuple> N3471
ULIBPP_CCLIB_IMPR_INT_CONST			- Improved std::integral_constant N3545 *
ULIBPP_CCLIB_UDL_CHRONO_STRING		- User-defined literals for <chrono> and <string> N3642 *
ULIBPP_CCLIB_NULL_FWD_ITER			- Null forward iterators N3644 *
ULIBPP_CCLIB_QUOTED					- std::quoted N3654 *
ULIBPP_CCLIB_HETERO_ASSOC_LOOKUP	- Heterogeneous associative lookup N3657 *
ULIBPP_CCLIB_INT_SEQ				- std::integer_sequence N3658 *
ULIBPP_CCLIB_SHR_TIMED_MUTEX		- std::shared_timed_mutex N3659 *
ULIBPP_CCLIB_EXCHANGE				- std::exchange N3668 *
ULIBPP_CCLIB_FIX_CONSTEXPR_FNCS		- fixing constexpr member functions without const N3669
ULIBPP_CCLIB_GET					- std::get<T>() N3670 *
ULIBPP_CCLIB_DUAL_RANGE				- Dual-Range std::equal, std::is_permutation, std::mismatch N3671 *
// remnants
ULIBPP_CCLIB_MAKE_UNIQUE			- std::make_unique N3656 ?
ULIBPP_CCLIB_COMPLEX_UDLS			- User-defined Literals for std::complex !
ULIBPP_CCLIB_IS_FINAL				- std::is_final !
ULIBPP_CCLIB_IS_NULL_PTR			- std::is_null_pointer !
ULIBPP_CCLIB_MAKE_REVERS_ITER		- std::make_reverse_iterator !
ULIBPP_CCLIB_TRANS_TRAIT_REDUX		- ransformationTraits Redux !
ULIBPP_CCLIB_TRANSP_OP_FTORS		- Transparent operator functors (std::less<> et al) !
ULIBPP_CCLIB_TUPLE_ELEM_T			- std::tuple_element_t !

// C++ 17
//language
ULIBPP_CC_AUTO_BRACED_LIST			- New rules for auto with braced-init-lists N3922
ULIBPP_CC_TERSE_STATIC_ASSERT		- Terse static_assert N3928 *
ULIBPP_CC_TYPENAME_IN_TEMPL_TEMPL	- typename in template template-parameters N4051
ULIBPP_CC_REMOVING_TRIGRAPHS		- Removing trigraphs N4086
ULIBPP_CC_NESTED_NAMESPACE_DEF		- Nested namespace definitions N4230
ULIBPP_CC_ATTRS_NAMESPACES_ENUMS	- Attributes for namespaces and enumerators N4266 *
ULIBPP_CC_U8_LITERALS				- u8 character literals N4267
ULIBPP_CC_MORE_NONTYPE_TEMPL_ARGS	- Allowing more non-type template args N4268 *
ULIBPP_CC_FOLD_EXPR					- Fold expressions N4295 *
ULIBPP_CC_UNARY_FOLD_EXPR			- Unary fold expressions and empty parameter packs P0036R0
ULIBPP_CC_REMOVED_REGISTER_STC		- Remove deprecated register storage class P0001R1
ULIBPP_CC_REMOVED_BOOL_INCR			- Remove deprecated bool increment P0002R1
ULIBPP_CC_EXCSPEC_IN_TYPESYS		- Make exception specifications part of the type system P0012R1 *
ULIBPP_CC_AGGR_BASE_CLASS_INIT		- Aggregate initialization of classes with base classes P0017R1 *
ULIBPP_CC_HAS_INCLUDE				- __has_include in preprocessor conditionals P0061R1
ULIBPP_CC_NEW_INHERIT_CONSTR		- New specification for inheriting constructors P0136R1 *
ULIBPP_CC_LAMBDA_CAPTURE_DEREFTHIS	- Lambda capture of *this P0018R3 *
ULIBPP_CC_DIRECT_LIST_INIT_OF_ENUMS	- Direct-list-initialization of enums P0138R2
ULIBPP_CC_CONSTEXPR_LAMBDA			- constexpr lambda expressions P0170R1 *
ULIBPP_CC_DIFF_BEGIN_END_FOR		- Differing begin and end types in range-based for P0184R0 *
ULIBPP_CC_FALLTHROUGH_ATTR			- [[fallthrough]] attribute P0188R1 *
ULIBPP_CC_NODISCARD_ATTR			- [[nodiscard]] attribute P0189R1 *
ULIBPP_CC_MAYBEUNUSED_ATTR			- [[maybe_unused]] attribute P0212R1 *
ULIBPP_CC_HEXA_FLOAT_LITERALS		- Hexadecimal floating-point literals P0245R1 *
ULIBPP_CC_NS_ATTRS_WO_REPETITION	- Using attribute namespaces without repetition P0028R4
ULIBPP_CC_ALIGNED_NEW				- Dynamic memory allocation for over-aligned data P0035R4 *
ULIBPP_CC_TEMPL_DEDUCTION_FOR_CLASS	- Template argument deduction for class templates P0091R3, P0512R0 *
ULIBPP_CC_AUTO_FOR_NOTYPE_TEMPL_ARG	- Non-type template parameters with auto type P0127R2 *
ULIBPP_CC_GUARANTEED_COPY_ELISION	- Guaranteed copy elision P0135R1 *
ULIBPP_CC_STRICTER_EXPR_EVAL_ORDER	- Stricter expression evaluation order (chapter 8 alternative) P0145R3, P0400R0
ULIBPP_CC_STRUCTURED_BINDING		- Structured bindings P0217R3 *
ULIBPP_CC_IGNORE_UNKNOWN_ATTR		- Requirement to ignore unknown attributes P0283R2
ULIBPP_CC_CONSTEXPR_IF				- constexpr if-statements P0292R2 *
ULIBPP_CC_INIT_EXPR_FOR_IF_SWITCH	- Separate variable and condition for if and switch P0305R1
ULIBPP_CC_INLINE_VARIABLES			- Inline variables P0386R2 *
ULIBPP_CC_REMOVE_EXCEP_SPEC			- Removing dynamic-exception-specifications P0003R5
ULIBPP_CC_PACK_EXP_IN_USING			- Pack expansions in using-declarations P0195R2 *
ULIBPP_CC_MATCH_TEMPL_TEMPL_ARGS	- Matching template template-parameters to compatible arguments P0522R0 *
//library
ULIBPP_CCLIB_VOID_T					- std::void_t N3911
ULIBPP_CCLIB_UNCAUGHT_EXCEP			- std::uncaught_exceptions() N4259
ULIBPP_CCLIB_STANDALONE_CONT_FNCS	- std::size(), std::empty() and std::data() N4280
ULIBPP_CCLIB_IMPR_PAIR_TUPLE		- Improving std::pair and std::tuple N4387
ULIBPP_CCLIB_BOOL_CONST				- std::bool_constant N4389
ULIBPP_CCLIB_SHARED_MUTEX			- std::shared_mutex (untimed) N4508
ULIBPP_CCLIB_TYPE_TRAITS_VAR_TEMPL	- Type traits variable templates P0006R0
ULIBPP_CCLIB_LOGIC_OP_TYPE_TRAITS	- Logical operator type traits P0013R1
ULIBPP_CCLIB_PARA_ALG				- Parallel Algorithms P0024R2
ULIBPP_CCLIB_CLAMP					- std::clamp() P0025R0
ULIBPP_CCLIB_HW_INTERFERENCE_SIZE	- Hardware interference size P0154R1
ULIBPP_CCLIB_SWAPPABLE_TRAITS		- (nothrow-)swappable traits P0185R1
ULIBPP_CCLIB_FS						- File system library P0218R1
ULIBPP_CCLIB_STRING_VIEW			- std::string_view N3921, P0220R1
ULIBPP_CCLIB_ANY					- std::any P0220R1 *
ULIBPP_CCLIB_OPTIONAL				- std::optional P0220R1
ULIBPP_CCLIB_PMR					- Polymorphic memory resources P0220R1
ULIBPP_CCLIB_MATH_SPEC_FNCS			- Mathematical special functions P0226R1
ULIBPP_CCLIB_C11					- C++17 should refer to C11 instead of C99 P0063R3
ULIBPP_CCLIB_SPLICE_MAP_SET			- Splicing Maps and Sets P0083R3
ULIBPP_CCLIB_VARIANT				- std::variant P0088R3
ULIBPP_CCLIB_MAKE_FROM_TUPLE		- std::make_from_tuple() P0209R2
ULIBPP_CCLIB_UNIQ_OBJ_REPRE			- std::has_unique_object_representations P0258R2
ULIBPP_CCLIB_GCD_LCM				- std::gcd() and std::lcm() P0295R0
ULIBPP_CCLIB_NOT_FN					- std::not_fn P0005R4 P0358R1
ULIBPP_CCLIB_ELEM_STR_CONV			- Elementary string conversions P0067R5
ULIBPP_CCLIB_PTRS_WITH_ARRAY		- std::shared_ptr and std::weak_ptr with array support P0414R2
ULIBPP_CCLIB_SCOPED_LOCK			- std::scoped_lock P0156R2
ULIBPP_CCLIB_BYTE					- std::byte P0298R3
ULIBPP_CCLIB_IS_AGGREGATE			- std::is_aggregate LWG2911
// remnants
ULIBPP_CCLIB_ADDRESSOF_CONSTEXPR	- constexpr std::addressof *
ULIBPP_CCLIB_ALLOC_TR_EQ			- std::allocator_traits::is_always_equal *
ULIBPP_CCLIB_APPLY					- std::apply *
ULIBPP_CCLIB_ARRAY_CONSTEXPR		- Adding constexpr modifiers to std::reverse_iterator, std::move_iterator, std::array and range access *
ULIBPP_CCLIB_AS_CONST				- std::as_const


// C++20 (in the order from https://en.cppreference.com/w/cpp/compiler_support)
ULIBPP_CC_ALLOW_LAMBDA_CAPTURE		- Allow lambda capture [=, this] P0409R2
ULIBPP_CC_VAOPT_COMMA_ELISION		- __VA_OPT__ for preprocessor comma elision P0306R4, P1042R1
ULIBPP_CC_DESIGN_INIT				- Designated initializers P0329R4
ULIBPP_CC_TEMPL_PARLIST_GEN_LAMDAS	- Familiar template syntax for generic lambdas P0428R2
ULIBPP_CC_DFLT_INIT_BITFLD			- Default member initializers for bit-fields P0683R1
ULIBPP_CC_INITL_CLASS_TEMPL_DEDUCT	- Fixing class template argument deduction for initializer-list ctors P0702R1
ULIBPP_CC_CONST_MEMPTR				- Fixing const-qualified pointers to members P0704R1
ULIBPP_CC_CONCEPTS					- Concepts P0734R0
ULIBPP_CC_LAMBDA_UNEVAL_CTX			- Lambdas in unevaluated contexts P0315R4
ULIBPP_CC_3WAY_COMPARISON			- Consistent comparison (operator<=>) P0515R3
ULIBPP_CC_SIMPLY_IMPL_LAMDA_CAPT	- Simplifying implicit lambda capture P0588R1
ULIBPP_CC_RANGE_FOR_WITH_INIT		- Range-based for statements with initializer P0614R1
ULIBPP_CC_DFLT_STATELESS_LAMBDA		- Default constructible and assignable stateless lambdas P0624R2
ULIBPP_CC_CONST_MIS_DFLT_COPYCONST	- const mismatch with defaulted copy constructor P0641R2
ULIBPP_CC_ACC_CHECK_ON_SPEC			- Access checking on specializations P0692R1
ULIBPP_CC_ADL_TEMPL_NOT_VISIBLE		- ADL and function templates that are not visible P0846R0
ULIBPP_CC_LESS_EAGER_INST_CONSTEXPR	- Less eager instantiation of constexpr functions P0859R0
ULIBPP_CC_LIKELY_UNLIKELY			- Proposed wording for likely and unlikely attributes P0479R5
ULIBPP_CC_TYPENAME_MORE_OPTIONAL	- Down with typename! P0634R3
ULIBPP_CC_ALLOW_PACK_EXP_IN_LAMBDA	- Allow pack expansion in lambda init-capture P0780R2
ULIBPP_CC_NO_UNIQUE_ADDRESS			- Language support for empty objects P0840R2
ULIBPP_CC_RELAX_STRUCT_BINDING		- DR: Relaxing the structured bindings customization point finding rules P0961R1
ULIBPP_CC_RELAX_RANGE_FOR			- DR: Relaxing the range-for loop customization point finding rules P0962R1
ULIBPP_CC_ALLOW_STRUCT_BINDING		- DR: Allow structured bindings to accessible members P0969R0
ULIBPP_CC_DESTROY_DELETE			- Destroying operator delete P0722R3
ULIBPP_CC_CLASS_IN_NONTYPE_PARM		- Class types in non-type template parameters P0732R2
ULIBPP_CC_DEPRECATE_IMPL_CAPTURE	- Deprecate implicit capture of this via [=] P0806R2
ULIBPP_CC_EXPLICIT_BOOL				- explicit(bool) P0892R2
ULIBPP_CC_FEAT_TEST_MACROS			- Integrating feature-test macros P0941R2
ULIBPP_CC_PROHIBIT_AGGR_USER_CONSTR	- Prohibit aggregates with user-declared constructors P1008R1
ULIBPP_CC_CONSTEXPR_VIRTFNC			- constexpr virtual function P1064R0
ULIBPP_CC_COMPAR_CONSISTENCY		- Consistency improvements for comparisons P1120R0
ULIBPP_CC_CHAR8_T					- char8_t P0482R6
ULIBPP_CC_IS_CONST_EVAL				- std::is_constant_evaluated() P0595R2
ULIBPP_CC_CONSTEXPR_TRY_CATCH		- constexpr try-catch blocks P1002R1
ULIBPP_CC_IMMED_FNC					- Immediate functions P1073R3
ULIBPP_CC_NESTED_INL_NAMESPACE		- Nested inline namespaces P1094R2
ULIBPP_CC_CONSTRAIN_DECL			- Yet another approach for constrained declarations P1141R2
ULIBPP_CC_SINT_2COMPL				- Signed integers are two's complement P1236R1
ULIBPP_CC_CONSTEXPR_DYNCAST			- constexpr dynamic_cast and polymorphic typeid P1327R1
ULIBPP_CC_CONSTEXPR_UNION			- Changing the active member of a union inside constexpr P1330R0
ULIBPP_CC_COROUTINES				- Coroutines P0912R5
ULIBPP_CC_PARENT_AGGGR_INIT			- Parenthesized initialization of aggregates P0960R3
ULIBPP_CC_ARR_SIZE_DEDUC_IN_NEW		- DR: Array size deduction in new-expressions P1009R2
ULIBPP_CC_MODULES					- Modules P1103R3
ULIBPP_CC_STRONG_UNICODE_REQ		- Stronger Unicode requirements P1041R4, P1139R2
ULIBPP_CC_3WAY_NE_EQ				- <=> != == P1185R2
ULIBPP_CC_EXPL_DFLT_FNC_EXCEPT		- DR: Explicitly defaulted functions with different exception specifications P1286R2
ULIBPP_CC_STRUCT_BIND_EXT			- Structured binding extensions P1091R3, P1381R1
ULIBPP_CC_CONSTEXPR_CONTAINER		- More constexpr containers P0784R7
ULIBPP_CC_DEPREC_COMMA_IN_SUBSCRIPT	- Deprecate comma operator in subscripts P1161R3
ULIBPP_CC_NODISCARD_WITH_MSG		- [[nodiscard]] with message P1301R4
// not in list yet
ULIBPP_CC_CONSTINIT					- constinit keyword P1143R1

// library
ULIBPP_CCLIB_ENDIAN					- std::endian P0463R1
ULIBPP_CCLIB_REMOVE_CVREF			- std::remove_cvref P0550R2
ULIBPP_CCLIB_EXT_MAKE_SHARED_ARRAY	- Extending std::make_shared to support arrays P0674R1
ULIBPP_CCLIB_FP_ATOMICS				- Floating point atomic P0020R6
ULIBPP_CCLIB_SYNC_OSTREAM			- Synchronized buffered ostream P0053R7
ULIBPP_CCLIB_CONSTEXRP_ALGO_UTIL	- constexpr for <algorithm> and <utility> P0202R3
ULIBPP_CCLIB_CONSTEXPR_COMPLEX		- More constexpr for <complex> 	P0415R1
ULIBPP_CCLIB_STRING_PREFIX_SUFFIX	- String prefix and suffix checking P0457R2
ULIBPP_CCLIB_SUP_3WAY_COMP			- Library support for operator<=> <compare> P0515R3
ULIBPP_CCLIB_NODISCARD				- [[nodiscard]] in the standard library P0600R1
ULIBPP_CCLIB_CVRT_PTR2RAWPTR		- Utility to convert a pointer to a raw pointer P0653R2
ULIBPP_CCLIB_ATOMIC_SHARED_PTR		- Atomic shared_ptr and weak_ptr P0718R2
ULIBPP_CCLIB_SPAN					- std::span P0122R7
ULIBPP_CCLIB_CALENDAR_TZ			- Calendar and timezone P0355R7
ULIBPP_CCLIB_VERSION				- <version> P0754R2
ULIBPP_CCLIB_COMP_UNORDERED_CONT	- Comparing unordered containers P0809R0
ULIBPP_CCLIB_CONSTEXPRITERATOR_REQ	- ConstexprIterator requirements P0858R0
ULIBPP_CCLIB_STRING_RESERVE			- std::basic_string::reserve() should not shrink P0966R1
ULIBPP_CCLIB_ATOMIC_REF				- std::atomic_ref P0019R8
ULIBPP_CCLIB_CONTAINS_FNC_ASSOC		- contains() member function of associative containers P0458R2
ULIBPP_CCLIB_GUARANT_COPY_ELISION	- DR: Guaranteed copy elision for piecewise construction P0475R1
ULIBPP_CCLIB_BIT_CAST				- std::bit_cast() P0476R2
ULIBPP_CCLIB_INT_POW2				- Integral power-of-2 operations P0556R3
ULIBPP_CCLIB_IMPR_RV_FOR_ERASE		- Improving the return value of erase-like algorithms P0646R1
ULIBPP_CCLIB_DESTROY_DELETE			- std::destroying_delete P0722R3
ULIBPP_CCLIB_IS_NOTHROW_CONVERT		- std::is_nothrow_convertible P0758R1
ULIBPP_CCLIB_SHIFT					- Add shift to <algorithm> P0769R2
ULIBPP_CCLIB_TYPE_IDENTITY			- std::type_identity P0887R1
ULIBPP_CCLIB_CONCEPTS				- Concepts library P0898R3
ULIBPP_CCLIB_CONSTEXPR_COMP_ARRAY	- constexpr comparison operators for std::array P1023R0
ULIBPP_CCLIB_UNWRAP_REF				- std::unwrap_ref_decay and std::unwrap_reference P0318R1
ULIBPP_CCLIB_BIND_FRONT				- std::bind_front() P0356R5
ULIBPP_CCLIB_REF_WRAP_INCOMPL_TYPES	- std::reference_wrapper for incomplete types P0357R3
ULIBPP_CCLIB_FIX_OP_RSHIFT			- Fixing operator>>(basic_istream&, CharT*) P0487R1
ULIBPP_CCLIB_CHAR8_T				- Library support for char8_t P0482R6
ULIBPP_CCLIB_UTIL_FNCS_FOR_ALLOC	- Utility functions to implement uses-allocator construction P0591R4
ULIBPP_CCLIB_VAR_OPT_PROPAG_CMTRIV	- DR: std::variant and std::optional should propagate copy/move triviality P0602R4
ULIBPP_CCLIB_VARIANT_CONV_CONSTR	- A sane std::variant converting constructor P0608R3
ULIBPP_CCLIB_FNC_MV_CONSTR_NOEXCEPT	- std::function's move constructor should be noexcept P0771R1
ULIBPP_CCLIB_RANGES					- The One Ranges Proposal P0896R4
ULIBPP_CCLIB_CHRONO_FNCS_NOEXCEPT	- <chrono> zero(), min(), and max() should be noexcept P0972R0
ULIBPP_CCLIB_CE_POINTER_TRAITS		- constexpr in std::pointer_traits P1006R1
ULIBPP_CCLIB_ASSUME_ALIGNED			- std::assume_aligned() P1007R3
ULIBPP_CCLIB_REMOVE_COMP_OF_SPAN	- Remove comparison operators of std::span P1085R2
ULIBPP_CCLIB_STRING_ALLOC_APPEND	- Make stateful allocator propagation more consistent for operator+(basic_string) P1165R1
ULIBPP_CCLIB_CONSIST_CONT_ERASE		- Consistent container erasure P1209R0
ULIBPP_CCLIB_INTERPOLATE			- std::lerp() and std::midpoint() P0811R3
ULIBPP_CCLIB_ENH_SPAN				- Usability enhancements for std::span P1024R3
ULIBPP_CCLIB_CREATE_DIR_INTUITIVE	- DR: Make create_directory() Intuitive P1164R1
ULIBPP_CCLIB_SSIZE_FOR_SPAN			- std::ssize() and unsigned extent for std::span P1227R2
ULIBPP_CCLIB_TRAITS_FOR_UNBOUND_ARR	- Traits for (un)bounded arrays P1357R0
ULIBPP_CCLIB_BIT_OPERATIONS			- Bit operations P0553R4

*/

/*

	Notes:
	Detection of a VS version
	_MSC_FULL_VER, _MSC_BUILD
	VS2005 SP1							140050727
	VS2008 FP (without FP?)				150021022
	VS2008 RTM/SP1						150030729
	VS2010								160030319
	VS2012								170050727
	VS2012 CTP Nov 2012					170051025
	VS2012 U1							170051106 - does not have features of VS2012 CTP Nov 2012
	VS2012 U2+U3						170060315 - does not have features of VS2012 CTP Nov 2012
	VS2013								180021005
	VS2015 RC							190022816
	VS2015								190023026
	VS2015 U1							190023506
	VS2015 U2							190023918
	VS2015 U3							190024210
	VS2017								191025017
	VS2017 15.3							191125506
	VS2017 15.4							191125542
	VS2017 15.5							191225830
	VS2017 15.6							191326128
	VS2017 15.7							191426428
	VS2017 15.8							191526726
	VS2017 15.9							191627024
	VS2019 16.0							192027508
	VS2019 16.1							192127702
	VS2019 16.2							192227905
	VS2019 16.4							192428315
	VS2022 17.0							193030705
*/
