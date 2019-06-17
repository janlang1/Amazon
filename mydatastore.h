#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <string>
#include <set>
#include <vector>
#include "product.h"
#include "user.h"
#include "review.h"
#include "datastore.h"
#include <map>
#include "heap.h"
#include <algorithm>


class MyDataStore : public DataStore{
public:
    ~MyDataStore();

    /**
     * Adds a product to the data store
     */
    void addProduct(Product* p);

    /**
     * Adds a user to the data store
     */
    void addUser(User* u);

    /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
    std::vector<Product*> search(std::vector<std::string>& terms, int type, std::string sorting_type);

    /**
     * Reproduce the database file from the current Products and User values
     */
    void dump(std::ostream& ofile) ;

    void addReview(const std::string& prodName,
			   int rating,
			   const std::string& username,
			   const std::string& date,
			   const std::string& review_text);

    //Functions of the amazon.cpp
    void addAmazon(std::string username, int hitIndex,  std::vector<Product*>& hits);
    void viewAmazon(std::string username);
    void buyAmazon(std::string username);
    bool isValidUser(std::string username);
    User* getUser(std::string username);

    //hw5
    void viewReview(int hitIndex,  std::vector<Product*>& hits);
    void add_review_to_product(std::vector<Product*>& hits, int hitIndex,
            int rating, std::string user, std::string date, std::string review_text);

    //hw8
    //map of username to username with similarity score
    std::map<std::string, std::map<std::string, double> > basicSimilarity();
    //return the BS score between two users
    double getBSscore(std::set<std::string> userSet1,
                     std::set<std::string> userSet2, 
                    std::map<std::string, double> productMapRating1, 
                    std::map<std::string, double> productMapRating2);

    std::map<std::string, double> refineDijkstra(std::string start);

    std::vector<std::pair<std::string, double> > makeSuggestion(std::string currentUser);
    void printSuggestions(std::vector<std::pair<std::string, double> > suggest, std::string user, std::ostream& os);

private:	
	std::set<Product*> productList;
	std::set<User*> userList;

    //product name mapped to product
    std::map<std::string,Product*> product_name_map;
    //maps keywords to their products
	std::map<std::string,std::set<Product*> > keywordMap;
    //maps usernames to user*
    std::map<std::string,User*> userMap;

};

#endif
