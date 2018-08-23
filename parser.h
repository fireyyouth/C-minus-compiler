#ifndef PARSER_H
#define PARSER_H
#include <memory>
#include <map>
#include <vector>
#include <string>

typedef enum {
  ASSIGN,
  DEFINE,
  EXPR,
  BOOL_TERM,
  CMP_TERM,
  UNIT,
  ROOT,
  ARITH_OP,
  BOOL_OP,
  CMP_OP,
  NUM,
  ID,
  LP,
  RP,
  COMMA,
  LBRACE,
  RBRACE,
  SEMICOLON,
  EQUAL,
  DEF
} node_t;

extern std::map<node_t, const char *> type_to_name;

struct Token {
  node_t type;
  std::string content;
};

struct Node {
  node_t type;
  std::vector<std::unique_ptr<Node>> kids;
  Node(node_t t) : type(t) {}
};

class Parser {
public:
  Parser(const Token *b, const Token *e);
  std::unique_ptr<Node> parse();
private:
const Token *focus;
  const Token *const end;

  std::map<node_t, std::unique_ptr<Node> (Parser::*)()> type_to_func;


  std::unique_ptr<Node> build_root();
  std::unique_ptr<Node> build_assign();
  std::unique_ptr<Node> build_define();
  std::unique_ptr<Node> build_func();
  std::unique_ptr<Node> build_expr();
  std::unique_ptr<Node> build_bool_term();
  std::unique_ptr<Node> build_cmp_term();
  std::unique_ptr<Node> build_unit();
};
#endif
