#include "user.h"
#include <cmath>
using namespace std;

User::User() : name_("unknown"), balance_(0.0), type_(1)
{

}
User::User(std::string name, double balance, int type, unsigned long long password) :
    name_(name), 
    balance_(balance), 
    type_(type), 
    password_(password)
{

}

User::~User()
{

}


std::string User::getName() const
{
    return name_;
}

double User::getBalance() const
{
    return balance_;
}

unsigned long long User::getPassword() const
{
    return password_;
}

void User::deductAmount(double amt)
{
    balance_ -= amt;
}

void User::dump(std::ostream& os)
{
    os << name_ << " "  << balance_ << " " << type_ << endl;
}

void User::addToCart(std::vector<Product*> &hits, int hitIndex)
{
    int sizeOfResults = (hits.size()-1);
    if(hitIndex < 0 || hitIndex > sizeOfResults)
    {
        cout << "Invalid request" << endl;
        return;
    }
    else
    {
        cart.push_back(hits[hitIndex]);
    }
}

void User::addToProductRatingMap(std::string productName, double rating){
    prodRatingMap.insert(make_pair(productName, rating));
}

void User::removeFromCart(int i)
{
    //erase product in the cart at index i
    cart.erase(cart.begin() + i);
}

void User::processCart()
{
    size_t i = 0;
    while(i < cart.size())
    {
        
        Product* p = cart[i];
        //price of first item compared with the current balance
        //and checks the stock of the product
        if(p->getPrice() < balance_ && p->getQty() > 0)
        {
            //subtract price from user balance
            deductAmount(p->getPrice());
            //subtract qty of product by 1
            p->subtractQty(1);
            //remove product from the cart at index
            removeFromCart(i);
        }
        else ++i;
    } 
}

std::set<std::string> User::getProductList(){
    set<std::string> productList;
    for(std::map<std::string, double>::iterator it = prodRatingMap.begin(); 
                                            it != prodRatingMap.end(); it++)
    {
        if(productList.find(it->first) == productList.end())
            productList.insert(it->first);
    }
    return productList;
}
