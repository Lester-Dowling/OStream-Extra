// Unit-Tests/test-sequence.cpp
#include "pch-unit-tests.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>
#include "OStream-Extra/sequence.hpp"

BOOST_AUTO_TEST_SUITE(test_sequence_suite, *boost::unit_test::enabled())

typedef boost::mpl::list<int16_t, int32_t, float, double> signed_test_types;
typedef boost::mpl::list<uint16_t, uint32_t> unsigned_test_types;
typedef boost::mpl::list<int16_t, uint16_t, int32_t, uint32_t, float, double>
  common_number_test_types;
typedef boost::mpl::list<char, unsigned char, signed char> char_test_types;
typedef boost::mpl::list<float, double> real_test_types;


BOOST_AUTO_TEST_CASE_TEMPLATE(vector_of_reals, T, real_test_types)
{
	using namespace std;
	vector<T> p = { static_cast<T>(3.14), static_cast<T>(7.1) };
	const string expected_answer = "p = 3.14 7.1 ";
	ostringstream oss;
	oss.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);
	const size_t width = 0;
	const int prec = -1;
	const string sep = " ";
	const string eol = "";
	io::write_sequence(oss, p.begin(), p.end(), "p", width, prec, sep, eol);
	BOOST_TEST_MESSAGE(oss.str());
	BOOST_REQUIRE_EQUAL(oss.str(), expected_answer);
}


BOOST_AUTO_TEST_SUITE_END();
