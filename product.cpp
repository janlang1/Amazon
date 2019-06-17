#include <sstream>
#include <iomanip>
#include "product.h"

using namespace std;

Product::Product(const std::string category, const std::string name, double price, int qty) :
    name_(name),
    price_(price),
    qty_(qty),
    category_(category)
{

}

Product::~Product()
{

}
double Product::getAvgRating() const
{
    return average_rating;
}

double Product::getPrice() const
{
    return price_;
}

std::string Product::getName() const
{
    return name_;
}

void Product::subtractQty(int num)
{
    qty_ -= num;
}

int Product::getQty() const
{
    return qty_;
}
vector<Review*> Product::getReviews()
{
    return reviews;
}

int Product::getAverageRating() const
{
    if(reviews.empty()) return 0;
    else return average_rating;
}

void Product::addReview_toProduct(Review* r)
{
    reviews.push_back(r);
}
void Product::computeAverageRating()
{
    double total_rating = 0;
    double total_number = reviews.size();
    if(total_number == 0) 
    {
        average_rating = 0;
    }
    else
    {
        for(size_t i = 0; i < reviews.size(); ++i)
        {
            total_rating += reviews[i]->rating;
        }
        average_rating = total_rating / total_number;
    }
    
}

/**
 * default implementation...can be overriden in a future
 * assignment
 */
bool Product::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}

void Product::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << endl;
}
