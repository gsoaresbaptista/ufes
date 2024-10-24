/*
 * util.h
 *
 * Class that separates a string in tokens, given a separator character.
 *
 *  Created on: Jul 10, 2014
 *      Author: Vítor E. Silva Souza (vitorsouza@gmail.com)
 */

#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <functional>
#include <locale>
#include <iomanip>
#include <ctime>

using namespace std;

namespace cpp_util {

	class Tokenizer {
		/* A stream with the contents of the string that is to be separated. */
		stringstream stream;

		/* The separator character. */
		char separator;

	public:
		Tokenizer(const string& str, char separator);
		virtual ~Tokenizer() { }

		/* Indicates if there is another token to read. */
		bool hasNext();

		/* Returns the next token. */
		const string next();

		/* Define uma nova string. */
		void setString(const string&);

		/* Returns all the remaining tokens as a vector of strings. */
		vector<string> remaining();
	};

	// ================= String ======================== //

	string& ltrim(string &s);

	string& rtrim(string &s);

	string& trim(string& s);

	bool stringCompare(string s1, string s2);

	bool isNumber(const string& s);

	// ================= NumPunctPTBR ======================== //

	class NumPunctPTBR: public numpunct<char> {
		protected:
			char do_decimal_point() const;
			string do_grouping() const;
			char do_thousands_sep() const;
	};


	// ================= Number ======================== //

	/* Locale object for Brazilian Portuguese. */
	static const locale LOCALE_PT_BR(locale(), new NumPunctPTBR());

	/* Converts decimal numbers from textual format to double, following a locale configuration. */
	double parseDouble(const string&, const locale&);

	/* Converts decimal numbers from double format to text, following a locale configuration. */
	string formatDouble(const double, const locale&);
	string formatDouble(const double, const int, const locale&);

	/* Converts decimal numbers from double format to currency, following a locale configuration. */
	string formatDoubleCurrency(const double, const locale&);

	const string DATE_FORMAT_PT_BR_SHORT = "%d/%m/%Y";

	// ================= Date ======================== //

	/* Converts dates in human-readable format to machine-processable format for storing in objects. */
	time_t parseDate(const string& str, const string& formato);

	/* Converts dates in machine-processable format to human-readable format for printing. */
	string formatDate(const time_t& t, const string& format);

	/* Verifies if a string contains a valid date according to a format. */
	bool validDate(const string& str, const string& format);

}
#endif