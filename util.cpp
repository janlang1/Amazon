#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "util.h"
#include <string>

using namespace std;
std::string convToLower(std::string src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

/** Complete the code to convert a string containing a rawWord
    to a set of words based on the criteria given in the assignment **/
bool isLetter(string s){
	if (s >= "a" && s <= "z"){return true;}
	return false;
}
std::set<std::string> parseStringToWords(string rawWords)
{
    rawWords = convToLower(rawWords);
    std::set<std::string> s;
    string str = "";
    for(size_t i = 0; i < rawWords.length(); ++i)
    {
        std::string segment = rawWords.substr(i,1);
        if(isLetter(segment)){
            str += segment;
        } else if (str.length() >= 2) {
            s.insert(str);
            str = "";
        } else {
            str = ""; 
        }
    }
    //checks to make sure last keyword is inserted
	if(!str.empty()) s.insert(str);

    return s;
}

/**************************************************
 * COMPLETED - You may use the following functions
 **************************************************/

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), 
	    std::find_if(s.begin(), 
			 s.end(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(
	    std::find_if(s.rbegin(), 
			 s.rend(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
	    s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

bool isNumber(string s){
	if (s >= "0" && s <= "9"){return true;}
	return false;
}
bool isValidDate(std::string date)
{
    //YYYY-MM-DD
    //checks length of date input
    if(date.length() != 10) return false;
    //check if dashes are in correct place and inputs are numbers
    for (size_t i = 0; i < date.length(); ++i)
    {
        if (i == 4 && date[i] != '-') return false;
        else if (i == 7 && date[i] != '-') return false;
        else if(!isNumber(date.substr(i,1)) && (i != 4 && i != 7)) return false;
    }
    //extracts the date and day
    string month = date.substr(5,2);
    string day = date.substr(8,2);
    //converts string day into int day
    int day_int = stoi(day);

    if(month == "01" || month == "03" || month =="05" || month == "07" 
        || month =="08"|| month =="10" ||month =="12"){
            if (day_int >= 1 && day_int <= 31){
                return true;
            }
                
        }
    if(month == "04" || month == "06" || month =="09" || month == "11"){
            if (day_int >= 1 && day_int <= 30){
                return true;
            }
                
        }
    if(month == "02"){
            if (day_int >= 1 && day_int <= 28){
                return true;
            }
                
        }
    return false;
}

