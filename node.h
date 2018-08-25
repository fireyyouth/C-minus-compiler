#ifndef NODE_H
#define NODE_H
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cstdint>
typedef enum {
  NONE,
  STMT,
  IF_STMT,
  WHILE_STMT,
  BLOCK,
  ASSIGN,
  DEFINE,
  READ,
  WRITE,
  EXPR,
  BOOL_TERM,
  CMP_TERM,
  UNIT,
  STMT_LIST,
  ARITH_OP,
  BOOL_OP,
  NUM,
  LP,
  RP,
  COMMA,
  LBRACE,
  RBRACE,
  SEMICOLON,
  CMP_OP,
  EQUAL,
  ID,
  DEF,
  RD,
  WR,
  IF,
  WHILE
} node_t;

extern std::map<node_t, std::string> type_to_name;

struct Token {
  node_t type;
  std::string content;
};

struct Node {
  node_t type;
  std::vector<std::unique_ptr<Node>> kids;
  std::string content;
  Node(node_t t) : type(t) {}
};
#endif
