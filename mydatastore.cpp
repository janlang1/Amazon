#include <sstream>
#include <iomanip>
#include "mydatastore.h"
#include "util.h"
#include <map>
#include <string>
#include "product.h"
#include "user.h"
#include "review.h"
#include "msort.h"
#include "heap.h"
#include <cmath>
#include <algorithm>

struct Comp1 {
  bool operator()(Product* a, Product* b)
  {
    return a->getAverageRating() > b->getAverageRating();
  }
}comparator_r;
struct Comp2 {
  bool operator()(Product* a, Product* b)
  {
    return a->getName() < b->getName();
  }
}comparator_n;
struct Comp3 {
  bool operator()(Review* a, Review* b)
  {
    return a->date > b->date;
  }
}comparator_review_date;

MyDataStore::~MyDataStore() {
	//product
	for(std::set<Product*>::iterator it = productList.begin(); 
		it != productList.end(); ++it)
	{
		std::vector<Review*> product_reviews = (*it)->getReviews();
		for (std::vector<Review*>::iterator it1 = product_reviews.begin(); 
		it1 != product_reviews.end(); ++it1)
		{
			delete *it1;
		}
		delete *it;
	}
	//user
	for(std::set<User*>::iterator it = userList.begin(); 
		it != userList.end(); ++it)
	{
		delete *it;
	}
}

/**
 * Adds a product to the data store
 */
void MyDataStore::addProduct(Product* p)
{
	if(productList.find(p) == productList.end())
	{
		//insert product into list
		productList.insert(p); 
		product_name_map.insert(make_pair(p->getName(),p));
		//insert keywords of product into map to products <string,product*>
		std::set<std::string> s = p->keywords();
		for(std::set<std::string>::iterator it = s.begin(); it != s.end(); ++it)
		{
			//std::cout << *it << std::endl;
			if(keywordMap.find(*it) == keywordMap.end())
			{
				std::set<Product*> product_set;
				product_set.insert(p);
				keywordMap.insert(make_pair(*it, product_set));
			}
			else
			{
				std::map<std::string, std::set<Product*> >::iterator it1 = keywordMap.find(*it); 
				(it1->second).insert(p);
			}
			
		}
	}
}

/**
 * Adds a user to the data store
 */
void MyDataStore::addUser(User* u)
{
	//user list
	if(userList.find(u) == userList.end())
	{
		userList.insert(u); 
		//maps the name of user to the user
		std::string str = u->getName();
		if(userMap.find(str) == userMap.end())
		{
			userMap.insert(make_pair(str, u));
		}
	}

	
}

void MyDataStore::addReview(const std::string& prodName,
			   int rating,
			   const std::string& username,
			   const std::string& date,
			   const std::string& review_text)
{
	std::map<std::string, Product*>::iterator it = product_name_map.find(prodName);
	//check if product exists
	if(it != product_name_map.end())
	{
		Review* review = new Review(rating, username, date, review_text);
		it->second->addReview_toProduct(review);
		it->second->computeAverageRating();

		//userReviews
		std::map<std::string, User*>::iterator it = userMap.find(username);

		//insert product name and rating to the data structure in user
		it->second->addToProductRatingMap(prodName, rating);
	}
}

/**
 * Performs a search of products whose keywords match the given "terms"
 *  type 0 = AND search (intersection of results for each term) while
 *  type 1 = OR search (union of results for each term)
 */
std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type, std::string sorting_type)
{
	//initialize empty result vector
	std::vector<Product*> result;
	if(type == 0){
		
		if(terms.size() == 0){
			return result;
		}
		//if one term is entered
		if(terms.size() == 1){
			std::map<std::string, std::set<Product*> >::iterator it1 = keywordMap.find(terms[0]);
			if(it1 !=  keywordMap.end())
			{
				std::set<Product*> product_set = it1->second;
				for(std::set<Product*>::iterator it2 = product_set.begin(); it2 != product_set.end(); ++it2)
				{
					result.push_back(*it2);
				}
				if(sorting_type == "r")
					mergeSort(result, comparator_r);
				if(sorting_type == "n")
					mergeSort(result, comparator_n);
				 //adds product if found
				return result;
			}
		}
		//counter to see if all the terms were hit in the search
		//if more than 1 term is entered
		std::set<Product*> first_set;
		std::set<Product*> second_set;
		bool first_term = true;
		for(size_t i = 0; i < terms.size(); ++i)
		{
			if(keywordMap.find(terms[i]) != keywordMap.end()){
				std::map<std::string,std::set<Product*> >::iterator it1 = keywordMap.find(terms[i]);
				//first term cant union with anything
				if(first_term){
					first_set.insert(it1->second.begin(), it1->second.end());
					first_term = false; 
				}
				else{
					second_set.insert(it1->second.begin(), it1->second.end());
					first_set = setIntersection(first_set, second_set);
				}
			}
			else{
				//one of the terms is not met
				return result;
			}	
		}
		//adds all the products found in the AND search into vector
		for(std::set<Product*>::iterator it1 = first_set.begin(); it1 != first_set.end(); ++it1)
			result.push_back(*it1); //adds product if found
		//all terms hit means that the AND condition was met
	} else if (type == 1) {
		std::set<Product*> product_set;
		for(size_t i = 0; i < terms.size();++i)
		{
			//set iterator to the location of term
			std::map<std::string, std::set<Product*> >::iterator it1 = keywordMap.find(terms[i]);
			if(it1 !=  keywordMap.end())
			{
				product_set = setUnion(it1->second, product_set);
			}
				
		}
		for(std::set<Product*>::iterator it2 = product_set.begin(); it2 != product_set.end(); ++it2)
		{
			result.push_back(*it2); //adds product if found
		}	
	} else {
		return result;
	}
	if(sorting_type == "r")
		mergeSort(result, comparator_r);
	if(sorting_type == "n")
		mergeSort(result, comparator_n);
	return result;
}

/**
 * Reproduce the database file from the current Products and User values
 */
void MyDataStore::dump(std::ostream& ofile) 
{
	//product
	ofile << "<products>" << std::endl;
	for(std::set<Product*>::iterator it = productList.begin(); 
		it != productList.end(); ++it)
	{
		//call specific dump in each specific child class
		(*it)->dump(ofile);
	}
	//user
	ofile << "</products>" << std::endl;
	ofile << "<users>" << std::endl;
	for(std::set<User*>::iterator it = userList.begin(); 
		it != userList.end(); ++it)
	{
		//call specific dump in each specific child class
		(*it)->dump(ofile);
	}
	ofile << "</users>" << std::endl;
	ofile << "<reviews>" << std::endl;
	for(std::set<Product*>::iterator it = productList.begin(); 
		it != productList.end(); ++it)
	{
		
		//call specific dump in each specific child class
		std::vector<Review*> product_reviews = (*it)->getReviews();
		for (std::vector<Review*>::iterator it1 = product_reviews.begin(); 
		it1 != product_reviews.end(); ++it1)
		{
			ofile << (*it)->getName() << std::endl;
			(*it1)->dump(ofile);
		}
	}
	ofile << "</reviews>" << std::endl;
}
bool MyDataStore::isValidUser(std::string username)
{
	std::map<std::string, User*>::iterator it1 = userMap.find(username);
	if(it1 == userMap.end()){
		std::cout << "Invalid user" << std::endl;
		return false;
	}
	return true;
}

void MyDataStore::addAmazon(std::string username, int hitIndex, std::vector<Product*>& hits)
{
	//finding the user in the user map
	std::map<std::string, User*>::iterator it1 = userMap.find(username);
	(it1->second)->addToCart(hits,hitIndex-1);
}

void MyDataStore::add_review_to_product(std::vector<Product*>& hits, int hitIndex,
            int rating, std::string user, std::string date, std::string review_text)
{
	//check if it is a valid 
	if (rating < 1 || rating > 5) {
		std::cout << "Invalid Rating" << std::endl;
		return;
	}
	int sizeOfResults = (hits.size()-1);
    if(hitIndex < 0 || hitIndex > sizeOfResults)
    {
        std::cout << "Invalid request" << std::endl;
        return;
    }
	if(!isValidDate(date))
	{
		std::cout << "Invalid date" << std::endl;
		return;
	}
	Review* review = new Review(rating, user, date, review_text);
	hits[hitIndex]->addReview_toProduct(review);

	//updating for the REC part of hw8
	//userReviews
	std::map<std::string, User*>::iterator it = userMap.find(user);

	//insert product name and rating to the data structure in user
	it->second->addToProductRatingMap(hits[hitIndex]->getName(), rating);
}
				
void MyDataStore::viewAmazon(std::string username)
{
	std::map<std::string, User*>::iterator it1 = userMap.find(username);
	//it->second is the User*
	std::vector<Product*> userCart = (it1->second)->getCart();
	int itemCounter = 1;
	for(size_t i = 0; i < userCart.size(); ++i)
	{
		//item number
		std::cout << "Item " << itemCounter++ << std::endl;
		std::cout << userCart[i]->displayString();
		std::printf("%.2f", userCart[i]->getAvgRating());
        std::cout << std::endl;
		std::cout << std::endl;
	} 
}

void MyDataStore::viewReview(int hitIndex,  std::vector<Product*>& hits)
{
	int sizeOfResults = (hits.size()-1);
    if(hitIndex < 0 || hitIndex > sizeOfResults)
    {
        std::cout << "Invalid request" << std::endl;
        return;
    }
    else
    {
		std::vector<Review*> review_vector = (hits[hitIndex])->getReviews();
		//sorts vector by date
		mergeSort(review_vector, comparator_review_date);
		if(review_vector.empty())
			return;
		else{
			for(size_t i = 0; i < review_vector.size(); ++i)
				std::cout << review_vector[i]->displayReview() << std::endl;
		}
    }
}

void MyDataStore::buyAmazon(std::string username)
{
	//finding the user in the user map
	std::map<std::string, User*>::iterator it1 = userMap.find(username);
	it1->second->processCart();
}

User* MyDataStore::getUser(std::string username){
		std::map<std::string, User*>::iterator it1 = userMap.find(username);
		return it1->second;

}

std::map<std::string, std::map<std::string, double> > MyDataStore::basicSimilarity(){
	std::map<std::string, std::map<std::string, double> > bs;

	//makes an empty set for every username
	for(std::set<User*>::iterator it1 = userList.begin(); it1 != userList.end(); ++it1)
	{
		std::map<std::string, double> empty;
		bs.insert(make_pair((*it1)->getName(), empty));
	}

	//making the adjacency list with pair of other user and bs score
	for(std::set<User*>::iterator it1 = userList.begin(); it1 != userList.end(); ++it1)
	{
		for(std::set<User*>::iterator it2 = userList.begin(); it2 != userList.end(); ++it2)
		{
			if(it2 != it1)
			{
				std::set<std::string> userSet1 = (*it1)->getProductList();
				std::set<std::string> userSet2 = (*it2)->getProductList();
				std::map<std::string, double> productMapRating1 = (*it1)->getProdRatingMap();
				std::map<std::string, double> productMapRating2 = (*it2)->getProdRatingMap();
				double bsScore = getBSscore(userSet1, userSet2, productMapRating1,productMapRating2);
				//makes User* have a pair with pair of User* and basic similarity rating;
				bs[(*it1)->getName()].insert(std::make_pair((*it2)->getName(), bsScore));
				bs[(*it2)->getName()].insert(std::make_pair((*it1)->getName(), bsScore));
				//bs[(*it2)->getName()] is a map
			}
		}

	}
	return bs;
}

double MyDataStore::getBSscore(std::set<std::string> userSet1,
                     std::set<std::string> userSet2, 
                    std::map<std::string, double> productMapRating1, 
                    std::map<std::string, double> productMapRating2){

	std::set<std::string> sameReviews;
	sameReviews = setIntersection(userSet1, userSet2);
	double total = 0.0; 
	if(sameReviews.empty()) return 1.0;
	else {
		for(std::set<std::string>::iterator it = sameReviews.begin(); it != sameReviews.end(); ++it){
			//find rating for 1st and 2nd user
			total += abs(productMapRating1[*it] - productMapRating2[*it]) / 4.0; 
		}
	}
	return total / sameReviews.size();
}

std::map<std::string, double> MyDataStore::refineDijkstra(std::string startUser){
	//to get graph
	std::map<std::string, std::map<std::string, double> > adjList = basicSimilarity();
	//store distances
	std::map<std::string, double> refinedSimilarities;
	Heap<std::string> pq;
	//max distance
	double maxDistance = adjList.size();
	pq.push(0.0, startUser);
	for(std::map<std::string, std::map<std::string, double> >::iterator it = adjList.begin();
																it != adjList.end(); ++it){
		if(it->first != startUser)
		{
			pq.push(maxDistance, it->first);
			refinedSimilarities.insert(make_pair(it->first, maxDistance));
		}
	}
	while(!pq.empty())
	{
		std::string temp = pq.top(); //v
		pq.pop();
		//change iterator to the type
		for(std::map<std::string, double>::iterator it1 = adjList[temp].begin();  
								it1 != adjList[temp].end(); ++it1)
		{
			std::string neighbor = it1->first;
			double edgeWeight = adjList[temp][neighbor];//map's map's double
			if(refinedSimilarities[temp] + edgeWeight < refinedSimilarities[neighbor])
			{
				refinedSimilarities[neighbor] = refinedSimilarities[temp] + edgeWeight;
				pq.decreaseKey(refinedSimilarities[neighbor], neighbor);
			}
		}
	}

	return refinedSimilarities;
}


std::vector<std::pair<std::string, double> > MyDataStore::makeSuggestion(std::string currentUser){
	//DEBUG SIMILARITY
	/*
	std::map<std::string, std::map<std::string, double> > debug = basicSimilarity();
	std::map<std::string, double> debug1 = debug[currentUser];
	for(std::map<std::string, double>::iterator it=debug1.begin(); it != debug1.end(); ++it)
	{
		std::cout << it->first << " " << it->second << std::endl;
	}
	std::map<std::string, double> debug2 = refineDijkstra(currentUser);
	for(std::map<std::string, double>::iterator it = debug2.begin(); it != debug2.end(); ++it)
		std::cout << it->first << " " << it->second << std::endl;
	*/


	std::vector<std::pair<std::string, double> > suggest;

	User* currentU = userMap.find(currentUser)->second;
	//user's product list
	std::set<std::string> userProducts = currentU->getProductList();

	std::set<Product*> notFound;

	for(std::map<std::string,Product*>::iterator it = product_name_map.begin(); it != product_name_map.end(); ++it)
	{
		if(userProducts.find(it->first) == userProducts.end())
		{
			Product* p = it->second;
			if(!(p->getReviews().empty()))
			{
				notFound.insert(p);
			}
		}
	}
	//all products were reviewed
	if(notFound.empty()) return suggest;
		
	std::map<std::string, double> refinedS = refineDijkstra(currentUser);

	for(std::set<Product*>::iterator it = notFound.begin(); it != notFound.end(); ++it)
	{

		//get ref similarities
		//make map of user to their product rating
		std::vector<Review*> productReviews = (*it)->getReviews();
		std::map<std::string, double> userWhoReviewed;
		for(size_t i = 0; i < productReviews.size(); ++i)
		{
			userWhoReviewed.insert(make_pair(productReviews[i]->username, productReviews[i]->rating));
		}

		double rp = 0;
		double w = 0;

		for(std::map<std::string, double>::iterator it1 = userWhoReviewed.begin(); it1 != userWhoReviewed.end(); ++it1)
		{
			rp += it1->second * (1 - refinedS[it1->first]);
			w += (1 - refinedS[it1->first]);
		}
		if(w == 0) suggest.push_back(make_pair((*it)->getName(), 0.0));
		else suggest.push_back(make_pair((*it)->getName(), (rp/w)));
	}
	std::stable_sort(suggest.begin(), suggest.end());

	return suggest;

}

void MyDataStore::printSuggestions(std::vector<std::pair<std::string, double> > suggest, std::string user, std::ostream& os){

	if(suggest.empty()){
		std::cout << "No recommendations available" << std::endl;
		os << "No recommendations available" << std::endl;
		return;
		
	}
	std::cout << user << std::endl;
	os << user << std::endl;
	for(size_t i = 0; i < suggest.size(); ++i)
	{
		std::printf("%.2f", suggest[i].second);
		std::cout << " " << suggest[i].first << std::endl;
		os << suggest[i].second << " " << suggest[i].first << std::endl;
	}
}

