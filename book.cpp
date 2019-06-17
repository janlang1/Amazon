#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>
#include "book.h"
#include "util.h"

Book::Book(const std::string category, const std::string name, double price, int qty, 
                    std::string author, std::string isbn)
			:Product(category, name, price,qty),
	author_(author),
	isbn_(isbn)
{
	//puts name author and isbn into set of keywords
	std::set<std::string> s1 = parseStringToWords(name_);
	std::set<std::string> s2 = parseStringToWords(author_);
	keys = setUnion(s1,s2);
	keys.insert(isbn_);
}

Book::~Book()
{
}
/**
 * Returns the appropriate keywords that this product should be associated with
 */
std::set<std::string> Book::keywords() const
{
	return keys;
}

/**
 * Allows for a more detailed search beyond simple keywords
bool Book::isMatch(std::vector<std::string>& searchTerms) const
{
} */

/**
 * Returns a string to display the product info for hits of the search
 */
std::string Book::displayString() const
{
	std::string str = name_ + "\n" + 
				"Author: " + author_ + " " + "ISBN: " + isbn_ + "\n"
				+ std::to_string(price_) + " " + std::to_string(qty_) + 
				" left." + "\n" + "Rating: " ;
	return str;
}

/**
 * Outputs the product info in the database format
 */
void Book::dump(std::ostream& os) const{
	os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << isbn_
	<< "\n"  << author_ << std::endl;
}

/**
 * Accessors and mutators
 */
std::string Book::getAuthor() const{return author_;}
std::string Book::getISBN() const{return isbn_;}