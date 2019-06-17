#ifndef USER_H
#define USER_H
#include <iostream>
#include <string>
#include "product.h"
#include <vector>
#include <map>
/**
 * Implements User functionality and information storage
 *  You should not need to derive anything from User at this time
 */
class User {
public:
    User();
    User(std::string name, double balance, int type, unsigned long long password);
    virtual ~User();

    double getBalance() const;
    unsigned long long getPassword() const;
    std::string getName() const;
    void deductAmount(double amt);
    virtual void dump(std::ostream& os);

    //cart functions
    void processCart();
    std::vector<Product*> getCart(){return cart;}
    void addToCart(std::vector<Product*>& hits, int hitIndex);
    void removeFromCart(int index);
    void addToProductRatingMap(std::string productName, double rating);
    std::set<std::string> getProductList();
    std::map<std::string, double>  getProdRatingMap(){return prodRatingMap;};
    
private:
    //user now has a container for products
    std::vector<Product*> cart;
    std::string name_;
    double balance_;
    int type_;
    unsigned long long password_; 

    //user now has a map of product names to rating
    std::map<std::string, double>  prodRatingMap; 

};
#endif
