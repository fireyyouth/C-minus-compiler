#include "parser.h"
#include "dot_dump.h"
#include <vector>
using namespace std;
int main() {
  vector<node_t> types_old = {DEF, ID, SEMICOLON, ID, EQUAL, NUM, SEMICOLON};
  vector<node_t> types = {ID,  EQUAL,    NUM, ADD_OP, NUM, CMP_OP,
                          NUM, ADD_OP, NUM, ADD_OP, NUM, SEMICOLON};

  vector<Token> toks;
  for (auto type : types) {
    Token token;
    token.type = type;
    toks.push_back(token);
  }

  Parser p(toks.data(), toks.data() + toks.size());
  auto t = p.parse();

  dump_to_dot(t, "tree.dot");

  return 0;
}
