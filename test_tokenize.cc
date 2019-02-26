#include "tokenize.h"
#include <map>
#include <cassert>
#include <vector>
using namespace std;
int main() {
   map<string, vector<node_t>> testCase = {
        {
            "def x; x=3+2;", 
            {DEF, ID, SEMICOLON, ID, EQUAL, NUM, ADD_OP, NUM, SEMICOLON},
        }, 
        {
            "1+2==a1", 
            {NUM, ADD_OP, NUM, CMP_OP, ID},
        }, 
        {
            "1 * 2 - 1 <=a1", 
            {NUM, MUL_OP, NUM, ADD_OP, NUM, CMP_OP, ID},
        }, 
   }; 
   for (auto & test : testCase) {
        vector<node_t> v;
        Token token;
        const uint8_t * s = (const uint8_t *)test.first.data();
        const uint8_t * e = s + test.first.size();
        while (1) {
            s = tokenize_one(s, e, &token);
            if (token.type == NONE) {
                break; 
            }
            v.push_back(token.type);
        }
        if (v != test.second) {
            for (auto & type : v) {
                printf("%s ", type_to_name[type].data()); 
            } 
            putchar('\n');
            for (auto & type : test.second) {
                printf("%s ", type_to_name[type].data()); 
            } 
            putchar('\n');
        }
   }
   return 0;
}
