/**
 * @file        OStream-Extra/pair.hpp
 * @date        Started Mon Mar  5 11:11:49 EST 2007
 * @author      Lester J. Dowling
 * @par         Copyright &copy; Lester J. Dowling.
 *
 * @brief Input and output operators for @c pair.
 */
#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <locale>
namespace {

	/**
	 * A local input utility function that skips across C-style comments in an input
	 * stream and ignores them.
	 */
	inline void _ignore_comments_within_pair(std::istream& s__)
	{
		char c = 0, d = 0;
		if (s__ >> c) {
			if (c == '/') {
				if (s__.get(d)) {
					if (d == '*') {
						d = 0;
						while (!(c == '*' && d == '/')) {
							c = d;
							if (s__.get(d))
								continue;
							else {
								// Comment left open:
								s__.clear(std::ios_base::failbit);
								return;
							}
						}
					} else { // d != '*' ==> Not the start of a comment:
						s__.putback(d);
						s__.putback(c);
						return;
					}
				} else {
					// Unexpected eof reading d:
					s__.putback(c);
					return;
				}
			} else if (c) { // c != '/' ==> not a comment:
				s__.putback(c);
				return;
			}
		}
	}

	inline std::istream& _read_opening_bracket(
	  std::istream& s__,
	  char& matching_closing_bracket)
	{
		char c = 0;
		if (!(s__ >> c))
			return s__;
		// Do we have an opening bracket:
		switch (c) {
			case '(':
				matching_closing_bracket = ')';
				break;
			case '{':
				matching_closing_bracket = '}';
				break;
			case '[':
				matching_closing_bracket = ']';
				break;
			default: // Missing bracket:
				matching_closing_bracket = 0;
				if (c)
					s__.putback(c);
				s__.clear(std::ios_base::failbit);
		}
		return s__;
	}

	inline std::istream& _read_string_within_pair(
	  std::istream& s__,
	  std::string& result,
	  const char closing_delimiter)
	{
		const char kComma = ',';
		const char kDQuote = '"';
		const char kSQuote = '\'';
		const char kSlash = '\\';
		_ignore_comments_within_pair(s__);
		char quote = 0;
		char slash = 0;
		char c = 0;
		result.clear(); // Prepare to append to result.
		if (!(s__ >> c)) {
			// Expecting a string.
			s__.clear(std::ios_base::badbit);
			return s__;
		}
		do {
			if (quote) { // Quoted String ---------------------------------
				if (c == quote && !slash) {
					// End of quoted string:
					quote = 0;
					_ignore_comments_within_pair(s__);
					// Expecting to read the closing_delimiter:
					if (s__ >> c) {
						if (c != closing_delimiter)
							s__.clear(std::ios_base::badbit);
					}
					return s__;
				} else if (c == quote && slash) { // Interior escaped quote:
					result += quote;
					slash = 0;
					continue;
				} else if (slash) { // Non-escaping slashes are appended:
					result += slash;
					slash = 0;
					// Fall through ...
				}
				if (c == kSlash) {
					slash = c;
					continue;
				} else {
					result += c;
					continue;
				}
			}	  // end if (quote)
			else { // Unquoted String ------------------------------------
				if (c == closing_delimiter)
					return s__;
				else if ((c == kDQuote || c == kSQuote) && !slash) {
					// Start of a quoted string:
					quote = c;
					continue;
				} else if ((c == kDQuote || c == kSQuote) && slash) {
					// Escaped quote:
					result += c;
					slash = 0;
					continue;
				} else if (slash) { // Non-escaping slashes are appended:
					result += slash;
					slash = 0;
					// Fall through ...
				}
				if (c == kSlash) {
					slash = c;
					continue;
				} else if (std::isspace(c, std::locale("C"))) {
					// Whitespace marks the end of an unquoted string,
					// so ignore everything till the closing delimiter:
					_ignore_comments_within_pair(s__);
					// Expecting to read the closing_delimiter:
					if (s__ >> c) {
						if (c != closing_delimiter)
							s__.clear(std::ios_base::badbit);
					}
					return s__;
				} else { // Continue appending to string:
					result += c;
					continue;
				}
			}
		} while (s__.get(c));
		// The closing_delimiter is missing:
		s__.clear(std::ios_base::badbit);
		return s__;
	}

} // namespace

namespace std {
	namespace whitespace_delimited {
		/**
		 * Put-to operator for @c pair.  The output format for a @c pair is the
		 * @c first and @c second separated by whitespace.
		 *
		 * For example:
		 *
		 * <code><pre>
		 *     3  4
		 * </pre></code>
		 */
		template<class U, class V>
		ostream& operator<<(ostream& o, const pair<U, V>& p)
		{
			return o << p.first << "    " << p.second;
		}
	} // end namespace whitespace_delimited
}

namespace std {

	/**
	 * Put-to operator for @c pair.  The output format for a @c pair is the @c
	 * first and @c second separated by a comma and wrapped in parentheses.
	 *
	 * For example:
	 *
	 * <code><pre>
	 *     (3, 4)
	 * </pre></code>
	 */
	template<class U, class V>
	ostream& operator<<(ostream& o, const pair<U, V>& p)
	{
		return o << '(' << p.first << ", " << p.second << ')';
	}

	/**
	 * Specialization of the put-to operator for @c pair where the @c first
	 * is a @c std::string.
	 *
	 * The output of the @c std::string is modified in the following way: firstly, the whole
	 * string
	 * is wrapped in double quotes, '"'; secondly, any double quotes within the string are
	 * escaped with a slash, '\'.
	 *
	 * For example, given:
	 *
	 * <code><pre>
	 *     pair<string,int> p( "George's \"Age\"", 8 );
	 * </pre></code>
	 *
	 * then, the output from this put-to operator is:
	 *
	 * <code><pre>
	 *     ("George's \"Age\"", 8)
	 * </pre></code>
	 */
	template<class V>
	ostream& operator<<(ostream& o, const pair<string, V>& p)
	{
		o << '(' << '"';

		size_t dquote_pos = p.first.find_first_of('"');
		if (dquote_pos == string::npos) {
			o << p.first;
		} else {
			string dup(p.first);
			while (dquote_pos != string::npos) {
				dup.insert(dquote_pos, 1, '\\');
				dquote_pos = dup.find_first_of('"', dquote_pos + 2);
			}
			o << dup;
		}
		return o << '"' << ", " << p.second << ')';
	}

	/**
	 * Specialization of the put-to operator for @c pair where the @c second is a @c
	 * std::string.
	 *
	 * The output of the string is modified in the following way: first, the whole string is
	 * wrapped in double quotes, '"'; second, any double quotes within the string are
	 * escaped
	 * with a slash, '\'.
	 *
	 * For example, given:
	 *
	 * <code><pre>
	 *     pair<int,string> p( 8, "George" );
	 * </pre></code>
	 *
	 * then, the output from this put-to operator is:
	 *
	 * <code><pre>
	 *     (8, "George")
	 * </pre></code>
	 */
	template<class U>
	ostream& operator<<(ostream& o, const pair<U, string>& p)
	{
		o << '(' << p.first << ", \"";
		size_t dquote_pos = p.second.find_first_of('"');
		if (dquote_pos == string::npos) {
			o << p.second;
		} else {
			string dup(p.second);
			while (dquote_pos != string::npos) {
				dup.insert(dquote_pos, 1, '\\');
				dquote_pos = dup.find_first_of('"', dquote_pos + 2);
			}
			o << dup;
		}
		return o << "\")";
	}

	/**
	 * Specialization of the put-to operator for @c pair where both the @c first and @c
	 * second are @c std::strings.
	 *
	 * The output of the string is modified in the following way: first, the whole string is
	 * wrapped in double quotes, '"'; second, any double quotes within the string are
	 * escaped
	 * with a slash, '\'.
	 *
	 * For example, given:
	 *
	 * <code><pre>
	 *     pair<int,string> p( "George" , "Mildred" );
	 * </pre></code>
	 *
	 * then, the output from this put-to operator is:
	 *
	 * <code><pre>
	 *     ("George", "Mildred")
	 * </pre></code>
	 */
	inline ostream& operator<<(ostream& o, const pair<string, string>& p)
	{
		size_t dquote_pos;
		o << '(' << '"';
		dquote_pos = p.first.find_first_of('"');
		if (dquote_pos == string::npos) {
			o << p.first;
		} else {
			string dup(p.first);
			while (dquote_pos != string::npos) {
				dup.insert(dquote_pos, 1, '\\');
				dquote_pos = dup.find_first_of('"', dquote_pos + 2);
			}
			o << dup;
		}

		o << "\", \"";

		dquote_pos = p.second.find_first_of('"');
		if (dquote_pos == string::npos) {
			o << p.second;
		} else {
			string dup(p.second);
			while (dquote_pos != string::npos) {
				dup.insert(dquote_pos, 1, '\\');
				dquote_pos = dup.find_first_of('"', dquote_pos + 2);
			}
			o << dup;
		}
		return o << "\")";
	}

} // end namespace std
