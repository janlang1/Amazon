#include <sstream>
#include <iomanip>
#include <iostream>
#include "movie.h"
#include "util.h"

Movie::Movie(const std::string category, const std::string name, double price, int qty,
        const std::string genre, const std::string rating)
    : Product(category, name, price,qty),
    genre_(genre),
    rating_(rating)
{
    //puts name and genre into set of keywords
	keys = parseStringToWords(name_);
    keys.insert(convToLower(genre_));
}
Movie::~Movie()
{

}
/**
     * Returns the appropriate keywords that this product should be associated with
     */
std::set<std::string> Movie::keywords() const
{
	return keys;
}
/**
 * Allows for a more detailed search beyond simple keywords   
bool Movie::isMatch(std::vector<std::string>& searchTerms) const
{
}*/

/**
 * Returns a string to display the product info for hits of the search
 */
std::string Movie::displayString() const
{
	std::string str = name_ + "\n" + 
				"Genre: " + genre_ + " " + "Rating: " + rating_ + "\n" 
				+ std::to_string(price_) + " " + std::to_string(qty_) +
                 " left." + "\n" + "Rating: ";
	return str;
}

    /**
     * Outputs the product info in the database format
     */
void Movie::dump(std::ostream& os) const
{
	os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << genre_
	<< "\n"  << rating_ << std::endl;
}
/**
 * Accessors and mutators
 */
std::string Movie::getGenre() const{return genre_;}
std::string Movie::getRating() const{return rating_;}