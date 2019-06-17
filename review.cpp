#include "review.h"
#include <string>
#include <iostream>


std::string Review::displayReview(){
	std::string str = date + " " + std::to_string(rating) + " " + username + " " + reviewText;
	return str;
}
void Review::dump(std::ostream& os) const
{
	//3 adal 2019-12-31 Is this required?
    os << std::to_string(rating) << " " << username << " " << date << " " << reviewText << std::endl;
}