
BOOST_AUTO_TEST_CASE_TEMPLATE(test_IO_of_char, T, char_test_types)
{
	using namespace std;
	typedef pair<T, T> Pair;
	istringstream iss("(a,b) ( A , B ) (c,d) (e,f) (g,h)");
	iss.exceptions(ios_base::badbit | ios_base::failbit);
	vector<Pair> v;

	// Read iss into v:
	try {
		v.insert(v.end(), std::istream_iterator<Pair>(iss), std::istream_iterator<Pair>());
	}
	catch (const std::ios_base::failure&) {
		if (iss.fail()) {
			// BOOST_TEST_MESSAGE("iss has failbit set");
		}
		if (iss.bad()) {
			// BOOST_TEST_MESSAGE("iss has badbit set");
			BOOST_FAIL("Failed to parse pairs in istringstream");
		}
		if (iss.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("iss has eofbit set");
			BOOST_TEST(v.size() == 5);
		}
		else
			BOOST_FAIL("Failed to parse pairs in istringstream");
	}

	ostringstream oss;
	oss.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);
	io::write_sequence(oss, v, "v", 0, -1, " ", "");
	BOOST_REQUIRE_EQUAL(oss.str(), "v = (a, b) (A, B) (c, d) (e, f) (g, h) ");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_IO_of_real_numbers, T, real_test_types)
{
	using namespace std;
	typedef pair<T, T> Pair;
	istringstream iss("   ( 3.55 , 373.23)    ( 2.4 , 1.5)    ( 5.9 , -1e10 )    ");
	iss.exceptions(ios_base::badbit | ios_base::failbit);
	vector<Pair> v;

	// Read iss into v:
	try {
		v.insert(v.end(), std::istream_iterator<Pair>(iss), std::istream_iterator<Pair>());
		// ljd::io::write_sequence( cout, v, "v", 0 );
	}
	catch (const std::ios_base::failure&) {
		if (iss.fail()) {
			// BOOST_TEST_MESSAGE("iss has failbit set");
		}
		if (iss.bad()) {
			// BOOST_TEST_MESSAGE("iss has badbit set");
			BOOST_FAIL("Failed to parse pairs in istringstream");
		}
		if (iss.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("iss has eofbit set");
			BOOST_TEST(v.size() == 3);
		}
		else
			BOOST_FAIL("Failed to parse pairs in istringstream");
	}

	ostringstream oss;
	oss.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);
	io::write_sequence(oss, v, "v", /*width*/ 0, /*prec*/ -1, " ", "");
	BOOST_REQUIRE_EQUAL(oss.str(), "v = (3.55, 373.23) (2.4, 1.5) (5.9, -1e+10) ");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
  test_IO_of_one_word_string_and_real_numbers,
  T,
  real_test_types)
{
	using namespace std;
	typedef pair<string, T> Pair;
	istringstream iss("   ( abc , 373.23)    ( def , 1.5)    ( ghi , -1e10 )    ");
	iss.exceptions(ios_base::badbit | ios_base::failbit);
	vector<Pair> v;

	// Read iss into v:
	try {
		v.insert(v.end(), std::istream_iterator<Pair>(iss), std::istream_iterator<Pair>());
	}
	catch (const std::ios_base::failure&) {
		if (iss.fail()) {
			// BOOST_TEST_MESSAGE("iss has failbit set");
		}
		if (iss.bad()) {
			// BOOST_TEST_MESSAGE("iss has badbit set");
			BOOST_FAIL("Failed to parse pairs in istringstream");
		}
		if (iss.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("iss has eofbit set");
			BOOST_TEST(v.size() == 3);
		}
		else
			BOOST_FAIL("Failed to parse pairs in istringstream");
	}

	ostringstream oss;
	oss.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);
	io::write_sequence(oss, v, "v", /*width*/ 0, /*prec*/ -1, " ", "");
	BOOST_REQUIRE_EQUAL(
	  oss.str(), "v = (\"abc\", 373.23) (\"def\", 1.5) (\"ghi\", -1e+10) ");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
  test_IO_of_multi_word_string_and_real_numbers,
  T,
  real_test_types)
{
	using namespace std;
	typedef pair<string, T> Pair;
	istringstream iss(
	  "   ( 'abc \"def' , 373.23)    ( \\\"def\\\" , 1.5)    ( ghi , -1e10 )    ");
	iss.exceptions(ios_base::badbit | ios_base::failbit);
	vector<Pair> v;

	// Read iss into v:
	try {
		v.insert(v.end(), std::istream_iterator<Pair>(iss), std::istream_iterator<Pair>());
	}
	catch (const std::ios_base::failure&) {
		if (iss.fail()) {
			// BOOST_TEST_MESSAGE("iss has failbit set");
		}
		if (iss.bad()) {
			// BOOST_TEST_MESSAGE("iss has badbit set");
			BOOST_FAIL("Failed to parse pairs in istringstream");
		}
		if (iss.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("iss has eofbit set");
			BOOST_TEST(v.size() == 3);
		}
		else
			BOOST_FAIL("Failed to parse pairs in istringstream");
	}

	ostringstream oss;
	oss.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);
	io::write_sequence(oss, v, "v", /*width*/ 0, /*prec*/ -1, " ", "");
	BOOST_REQUIRE_EQUAL(
	  oss.str(), "v = (\"abc \\\"def\", 373.23) (\"\\\"def\\\"\", 1.5) (\"ghi\", -1e+10) ");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
  test_IO_of_real_numbers_and_multi_word_string,
  T,
  real_test_types)
{
	using namespace std;
	typedef pair<T, string> Pair;
	istringstream iss(
	  "   ( 373.23 , 'abc \"123' )    ( 1.5 , \\\"def\\\"  )    ( -1e10 , ghi )   ");
	iss.exceptions(ios_base::badbit | ios_base::failbit);
	vector<Pair> v;

	// Read iss into v:
	try {
		v.insert(v.end(), std::istream_iterator<Pair>(iss), std::istream_iterator<Pair>());
	}
	catch (const std::ios_base::failure&) {
		if (iss.fail()) {
			// BOOST_TEST_MESSAGE("iss has failbit set");
		}
		if (iss.bad()) {
			// BOOST_TEST_MESSAGE("iss has badbit set");
			BOOST_FAIL("Failed to parse pairs in istringstream");
		}
		if (iss.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("iss has eofbit set");
			BOOST_TEST(v.size() == 3);
		}
		else
			BOOST_FAIL("Failed to parse pairs in istringstream");
	}

	ostringstream oss;
	oss.exceptions(ios_base::badbit | ios_base::failbit);
	io::write_sequence(oss, v, "v", /*width*/ 0, /*prec*/ -1, " ", "");
	BOOST_REQUIRE_EQUAL(
	  oss.str(), "v = (373.23, \"abc \\\"123\") (1.5, \"\\\"def\\\"\") (-1e+10, \"ghi\") ");
}

BOOST_AUTO_TEST_CASE(test_IO_of_strings_and_comments)
{
	using namespace std;
	typedef pair<string, string> Pair;
	istringstream iss("   ( \"xyz\" , 'abc \"123' )    ( lmn , def )    ( /* in a comment "
					  "*/ qrs , ghi )    ");
	iss.exceptions(ios_base::badbit | ios_base::failbit);
	vector<Pair> v;
	try {
		v.insert(v.end(), std::istream_iterator<Pair>(iss), std::istream_iterator<Pair>());
	}
	catch (const std::ios_base::failure&) {
		if (iss.fail()) {
			// BOOST_TEST_MESSAGE("iss has failbit set");
		}
		if (iss.bad()) {
			// BOOST_TEST_MESSAGE("iss has badbit set");
			BOOST_FAIL("Failed to parse pairs in istringstream");
		}
		if (iss.eof()) {
			// Okay.  This is expected: istream_iterator ends correctly by
			// setting the eofbit.
			// BOOST_TEST_MESSAGE("iss has eofbit set");
			BOOST_TEST(v.size() == 3);
		}
		else
			BOOST_FAIL("Failed to parse pairs in istringstream");
	}

	ostringstream oss;
	oss.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);
	io::write_sequence(oss, v, "v", /*width*/ 0, /*prec*/ -1, " ", "");
	BOOST_REQUIRE_EQUAL(
	  oss.str(), "v = (\"xyz\", \"abc \\\"123\") (\"lmn\", \"def\") (\"qrs\", \"ghi\") ");
}



namespace std {
	template<typename T>
	ostream& operator<<(ostream& o, const std::stack<T>& s)
	{
		std::stack<T> y{ s };
		bool first_e = true;
		while (!y.empty()) {
			const auto e = y.top();
			y.pop();
			if (first_e) {
				o << e;
				first_e = false;
			}
			else
				o << ',' << ' ' << e;
		}
		return o;
	}

	template<typename T>
	ostream& operator<<(ostream& o, const std::deque<T>& s)
	{
		bool first_e = true;
		for (const auto& e : s) {
			if (first_e) {
				o << e;
				first_e = false;
			}
			else
				o << ',' << ' ' << e;
		}
		return o;
	}

	template<typename T>
	ostream& operator<<(ostream& o, const std::vector<T>& s)
	{
		bool first_e = true;
		for (const auto& e : s) {
			if (first_e) {
				o << e;
				first_e = false;
			}
			else
				o << ',' << ' ' << e;
		}
		return o;
	}
} // namespace std
