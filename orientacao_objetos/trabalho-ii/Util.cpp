/*
 * Util.cpp
 *
 *  Created on: Jul 10, 2014
 *      Author: Vítor E. Silva Souza (vitorsouza@gmail.com)
 */

#include "Util.hpp"
#define MAX_DATE_STRING_SIZE 100

namespace cpp_util {

	// ================= Tokenizer ======================== //

	Tokenizer::Tokenizer(const string& str, char sep): stream(str), separator(sep) {
	}

	/* Indicates if there is another token to read. */
	bool Tokenizer::hasNext() {
		// If it's not the end of the stream, there's another token to be read.
		return ! stream.eof();
	}

	/* Returns the next token. */
	const string Tokenizer::next() {
		// Reads the token from the stream, stopping at the separator, and returns.
		string token;
		getline(stream, token, separator);
		return token;
	}

	/* Returns all the remaining tokens as a vector of strings. */
	vector<string> Tokenizer::remaining() {
		vector<string> list;

		// While there are more tokens to read, put them in the list. Then returns the list.
		while (hasNext()) list.push_back(next());
		return list;
	}

	string& ltrim(string &s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
	return s;
}

	// ================= String ======================== //

	string& rtrim(string &s) {
		s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
		return s;
	}

	string& trim(string& s) {
		return ltrim(rtrim(s));
	}

	bool stringCompare(string s1, string s2) {
		const collate<char>& col = use_facet<collate<char> >(locale());
		transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
		transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
		const char* pb1 = s1.data();
		const char* pb2 = s2.data();
		return (col.compare(pb1, pb1 + s1.size(), pb2, pb2 + s2.size()) < 0);
	}

	bool isNumber(const string& s) {
		if (s.empty()) return false;
		for (unsigned int i = 0; i < s.size(); i++)
			if (! isdigit(s.at(i))) return false;
		return true;
	}

	// ================= NumPunctPTBR ======================== //

	/* In pt-BR, integer and decimal parts of decimal numbers are separated with a ',': e.g., 3,14. */
	char NumPunctPTBR::do_decimal_point() const {
		return ',';
	}

	/* This locale uses grouping (at every 3 digits). */
	string NumPunctPTBR::do_grouping() const {
		return "\03";
	}

	/* In pt-BR, grouping of thousands are separated with a '.': e.g., 1.000 for a thousand. */
	char NumPunctPTBR::do_thousands_sep() const {
		return '.';
	}

	// ================= Number ======================== //

	/* Converts decimal numbers from textual format to double, following a locale configuration. */
	double parseDouble(const string& str, const locale& loc) {
		double num;

		// Creates a stream with the given string and applies the specified locale to it.
		stringstream stream(str);
		stream.imbue(loc);

		// Uses the >> operator to read the the double value from the stream and returns.
		stream >> num;
		return num;
	}

	/* Converts decimal numbers from double format to text, following a locale configuration. */
	string formatDouble(const double num, const locale& loc) {
		// Creates an empty string stream and applies the specified locale to it.
		stringstream stream;
		stream.imbue(loc);

		// Uses the << operator to "print" the double value in the stream and returns its contents.
		stream << fixed << num;
		return stream.str();
	}

	/* Converts decimal numbers from double format to currency, following a locale configuration. */
	string formatDoubleCurrency(const double num, const locale& loc) {
		// Creates an empty string stream and applies the specified locale to it.
		stringstream stream;
		stream.imbue(loc);

		// Uses the << operator to "print" the double value in the stream and returns its contents.
		stream << fixed << setprecision(2) << num;
		return stream.str();
	}

    string formatDouble(const double num, const int precision, const locale& loc) {
        stringstream stream;
        stream.imbue(loc);

        // Uses the << operator to "print" the double value in the stream and returns its contents.
        stream << fixed << setprecision(precision) << num;
        return stream.str();
    }

	// ================= Date ======================== //

	/* Converts dates in human-readable format to machine-processable format for storing in objects. */
	time_t parseDate(const string& str, const string& format) {
		// Creates an empty calendar.
		struct tm calendar = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		// Fills the calendar fields (year, month, day, etc.) based on the date string following the given format.
		strptime(str.c_str(), format.c_str(), &calendar);

		// Creates a time_t, machine-friendly date value from the calendar and returns.
		time_t t = mktime(&calendar);
		return t;
	}

	/* Converts dates in machine-processable format to human-readable format for printing. */
	string formatDate(const time_t& t, const string& format) {
		// Creates a calendar structure from the machine-friendly time value.
		struct tm *tm = localtime(&t);

		// Writes in a string buffer the date from the calendar following the given format.
		char buffer[MAX_DATE_STRING_SIZE];
		strftime(buffer, MAX_DATE_STRING_SIZE, format.c_str(), tm);

		// Returns the buffer, which gets copied to the receiving string via copy constructor or assignment operator.
		return buffer;
	}

	bool validDate(const string& str, const string& format) {
		struct tm tm;
		return strptime(str.c_str(), format.c_str(), &tm);
	}

}
