#include <cstdio>
#include <memory>
#include "node.h"
using namespace std;
void dump_edges(unique_ptr<Node> &node, FILE *fp) {
  for (auto &kid : node->kids) {
      if (kid->content.empty()) {
        fprintf(fp, "\t\"%s/%x\" -> \"%s/%x\";\n", type_to_name[node->type].data(), node.get(), type_to_name[kid->type].data(), kid.get());
      } else {
        fprintf(fp, "\t\"%s/%x\" -> \"%s/%x\";\n", type_to_name[node->type].data(), node.get(), kid->content.data(), kid.get());
      }
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

