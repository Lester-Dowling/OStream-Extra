// Unit-Tests/test-pair.cpp
#include "pch-unit-tests.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>
#include "OStream-Extra/pair.hpp"
// include "OStream-Extra/sequence.hpp"

BOOST_AUTO_TEST_SUITE(test_pair_output_suite, *boost::unit_test::enabled())

typedef boost::mpl::list<int16_t, int32_t, float, double> signed_test_types;
typedef boost::mpl::list<uint16_t, uint32_t> unsigned_test_types;
typedef boost::mpl::list<int16_t, uint16_t, int32_t, uint32_t, float, double>
  common_number_test_types;
typedef boost::mpl::list<char, unsigned char, signed char> char_test_types;
typedef boost::mpl::list<float, double> real_test_types;


BOOST_AUTO_TEST_CASE_TEMPLATE(output_of_string_in_pair, T, common_number_test_types)
{
	using namespace std;
	pair<string, T> p{ "George's \"Age\"", static_cast<T>(8) };
	const string expected_answer = "(\"George's \\\"Age\\\"\", 8)";
	ostringstream oss;
	oss.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);
	oss << p;
	BOOST_REQUIRE_EQUAL(oss.str(), expected_answer);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(output_of_numbers_in_pair, T, common_number_test_types)
{
	using namespace std;
	pair<T, T> p{ static_cast<T>(0), static_cast<T>(1) };
	const string expected_answer = "(0, 1)";
	ostringstream oss;
	oss.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);
	oss << p;
	BOOST_REQUIRE_EQUAL(oss.str(), expected_answer);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(output_of_reals_in_pair, T, real_test_types)
{
	using namespace std;
	pair<T, T> p{ static_cast<T>(3.14), static_cast<T>(7.1) };
	const string expected_answer = "(3.14, 7.1)";
	ostringstream oss;
	oss.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);
	oss << p;
	BOOST_REQUIRE_EQUAL(oss.str(), expected_answer);
}


BOOST_AUTO_TEST_SUITE_END();
