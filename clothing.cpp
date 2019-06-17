#include <sstream>
#include <iomanip>
#include <iostream>
#include "clothing.h"
#include "util.h"

Clothing::Clothing(const std::string category, const std::string name, double price, int qty, 
std::string brand, std::string size)
	:Product(category, name, price,qty),
	brand_(brand),
	sizing_(size)
{
	//puts name and brand into set of keywords
	std::set<std::string> s1 = parseStringToWords(name_);
	std::set<std::string> s2 = parseStringToWords(brand_);
	this->keys = setUnion(s1,s2);
}
Clothing::~Clothing()
{
}
/**
     * Returns the appropriate keywords that this product should be associated with
     */
std::set<std::string> Clothing::keywords() const
{
	//accessor for keywords
	return keys;
}
/**
 * Allows for a more detailed search beyond simple keywords
     
bool Clothing::isMatch(std::vector<std::string>& searchTerms) const
{
}*/

/**
 * Returns a string to display the product info for hits of the search
 */
std::string Clothing::displayString() const
{
	
	std::string str = name_ + "\n" + 
				"Size: " + sizing_ + " " + "Brand: " + brand_ +  "\n"
				+ std::to_string(price_) + " " + std::to_string(qty_) +
				 " left." + "\n" + "Rating: ";
	return str;
}

    /**
     * Outputs the product info in the database format
     */
void Clothing::dump(std::ostream& os) const
{
	os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << 
		sizing_ << "\n"  << brand_ << std::endl;
}

/**
 * Accessors and mutators
 */
std::string Clothing::getSizing()const{return sizing_;}
std::string Clothing::getBrand()const{return brand_;}
