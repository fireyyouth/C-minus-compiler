#include "gen_code.h"
#include "parser.h"
#include "tokenize.h"
#include "dot_dump.h"
#include <cstdio>
#include <cassert>
#include <cstdint>

#include "debug.h"

using namespace std;

#define INPUT_SIZE 10000

uint8_t input[INPUT_SIZE];

int main(int argc, char ** argv) {
    assert(argc == 2);
    FILE * fp = fopen(argv[1], "rb");
    assert(fp);
    size_t n = fread(input, 1, INPUT_SIZE, fp);

    debug("input = %.*s\n", n, input);

    const uint8_t *s = input;
    vector<Token> toks;
    Token token;
    while (1) {
        s = tokenize_one(s, input + n, &token);
        if (token.type == NONE) {
            break; 
        }
        toks.push_back(token);
    }

  Parser p(toks.data(), toks.data() + toks.size());
  auto t = p.parse();
  dump_to_dot(t, "tree.dot");
  gen_code(t);

   return 0; 

}
