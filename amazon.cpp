#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "mydatastore.h"
#include "user.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product*>& hits);
unsigned int hashPassword(string password);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;

    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;
    ReviewSectionParser* reviewSectionParser = new ReviewSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);
    parser.addSectionParser("reviews", reviewSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }
    cout << " " <<endl;

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  LOGIN username password            " << endl;
    cout << "  LOGOUT                             " << endl;
    cout << "  AND r/n term term ...              " << endl;
    cout << "  OR r/n term term ...               " << endl;
    cout << "  ADD search_hit_number              " << endl;
    cout << "  VIEWCART                           " << endl;
    cout << "  BUYCART                            " << endl;
    cout << "  ADDREV seach_hit_number rating date review_text" << endl;
    cout << "  VIEWREV seach_hit_number           " << endl;
    cout << "  REC                                " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "=====================================" << endl;

    vector<Product*> hits;
    bool done = false;
    bool logged_in = false;
    string user = "";
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "AND") {
                string term;
                //r or n
                string sorting_type; 
                ss >> sorting_type;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0, sorting_type);
                displayProducts(hits);
            }
            else if ( cmd == "OR" ) {
                string term;
                vector<string> terms;
                //r or n
                string sorting_type; 
                ss >> sorting_type;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1, sorting_type);
                displayProducts(hits);
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
            //user login
            else if ( cmd == "LOGIN"){
                string password;
                if(ss >> user)
                {
                    if(ss >> password)
                    {
                        if(ds.isValidUser(user))
                        {
                            User* loginUser = ds.getUser(user);
                        
                            if(loginUser->getPassword() == hashPassword(password)){
                                logged_in = true;
                            }       
                        }
                    }
                }       
            }
            else if ( cmd == "LOGOUT"){
                logged_in = false;
            }
            //user features
            else if ( cmd == "ADD") {
                if(!logged_in)
                {
                    cout << "No current user" << endl;
                    continue;
                }
                int hitIndex;
                ss >> hitIndex; 
                ds.addAmazon(user, hitIndex, hits);
                
            }
            else if ( cmd == "VIEWCART") {
                if(!logged_in)
                {
                    cout << "No current user" << endl;
                    continue;
                }
                ds.viewAmazon(user);
            }
            else if ( cmd == "BUYCART") {
                if(!logged_in)
                {
                    cout << "No current user" << endl;
                    continue;
                }
                ds.buyAmazon(user);
            }
            else if ( cmd == "ADDREV") {
                if(!logged_in)
                {
                    cout << "No current user" << endl;
                    continue;
                }
                int hitIndex;
                int rating; 
                string date;
                string review_text; 
                ss >> hitIndex;
                ss >> rating;
                ss >> date;
                getline(ss,review_text);
                review_text = trim(review_text);
                //cout << review_text << endl;
                ds.add_review_to_product(hits ,hitIndex - 1, rating, user, date, review_text);

            }
            else if ( cmd == "VIEWREV") {
                int hitIndex = 0;
                ss >> hitIndex;
                ds.viewReview(hitIndex-1, hits);
            }

            else if ( cmd == "REC") {
                if(!logged_in)
                {
                    cout << "No current user" << endl;
                    continue;
                }
                ofstream ofile("rec.txt");
                //ds.makeSuggestion(user);
                ds.printSuggestions(ds.makeSuggestion(user), user, ofile);
                ofile.close();
                
            }

            else {
                cout << "Unknown command" << endl;
            }
        }

    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    //std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString();
        (*it)->computeAverageRating();
        std::printf("%.2f", (*it)->getAvgRating());
        cout << endl;
        cout << endl;
        resultNo++;
    }
}

unsigned int hashPassword(string password){
    unsigned long long hashCode = 0;
    
    unsigned int encryptedPassword = 0;

    for(size_t i = 0; i < password.length(); ++i){
        hashCode *= 128;
        hashCode += (int)password[i];
    }

    unsigned int encrypt[4];
    for(int i = 0; i < 4; i++){
        encrypt[i] = hashCode % 65521; 
        hashCode /= 65521;
    }
    //(45912 * w1 + 35511 * w2 + 65169 * w3 + 4625 * w4) % 65521
    encryptedPassword = (45912 * encrypt[3] + 35511 * encrypt[2] + 
                        65169 * encrypt[1] + 4625 * encrypt[0]) % 65521;
    
    return encryptedPassword;
}