#include "parser.h"
#include <cstdio>
using namespace std;
void dump_edges(unique_ptr<Node> &node, FILE *fp) {
  for (auto &kid : node->kids) {
    fprintf(fp, "\t%s_%x -> %s_%x;\n", type_to_name[node->type], node.get(),
            type_to_name[kid->type], kid.get());
    dump_edges(kid, fp);
  }
}

void dump_to_dot(unique_ptr<Node> &node, const char *path) {
  FILE *fp = fopen(path, "w");
  fprintf(fp, "digraph parse_tree {\n");
  dump_edges(node, fp);
  fprintf(fp, "}\n");
  fclose(fp);
}
int main() {
  vector<node_t> types_old = {DEF, ID, SEMICOLON, ID, EQUAL, NUM, SEMICOLON};
  vector<node_t> types = {ID,  EQUAL,    NUM, ARITH_OP, NUM, CMP_OP,
                          NUM, ARITH_OP, NUM, ARITH_OP, NUM, SEMICOLON};

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
