#include <cstdio>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "parser.h"

using namespace std;

struct ParseError {
  ParseError(int line) { printf("error at %d\n", line); }
};
map<node_t, const char *> type_to_name = {{ASSIGN, "ASSIGN"},
                                          {DEFINE, "DEFINE"},
                                          {EXPR, "EXPR"},
                                          {BOOL_TERM, "BOOL_TERM"},
                                          {CMP_TERM, "CMP_TERM"},
                                          {UNIT, "UNIT"},
                                          {ROOT, "ROOT"},
                                          {ARITH_OP, "ARITH_OP"},
                                          {BOOL_OP, "BOOL_OP"},
                                          {CMP_OP, "CMP_OP"},
                                          {NUM, "NUM"},
                                          {ID, "ID"},
                                          {LP, "LP"},
                                          {RP, "RP"},
                                          {COMMA, "COMMA"},
                                          {LBRACE, "LBRACE"},
                                          {RBRACE, "RBRACE"},
                                          {SEMICOLON, "SEMICOLON"},
                                          {EQUAL, "EQUAL"},
                                          {DEF, "DEF"}};



#define consume_token(token)                                                   \
  do {                                                                         \
    if (this->focus == end || this->focus->type != token) {                                \
      throw ParseError(__LINE__);                                              \
    } else {                                                                   \
      node->kids.push_back(make_unique<Node>(token));                          \
      printf("%s\n", type_to_name[this->focus->type]);                               \
      ++this->focus;                                                                 \
    }                                                                          \
  } while (0)

#define consume_node(type)                                                     \
  do {                                                                         \
    node->kids.push_back((this->*(this->type_to_func[type]))());                     \
  } while (0)

Parser::Parser(const Token *b, const Token *e) : focus(b), end(e) {
  type_to_func[DEFINE] = &Parser::build_define;
  type_to_func[ASSIGN] = &Parser::build_assign;
  type_to_func[EXPR] = &Parser::build_expr;
  type_to_func[BOOL_TERM] = &Parser::build_bool_term;
  type_to_func[CMP_TERM] = &Parser::build_cmp_term;
  type_to_func[UNIT] = &Parser::build_unit;
}

unique_ptr<Node> Parser::parse() { return build_root(); }

unique_ptr<Node> Parser::build_root() {
  auto node = make_unique<Node>(ROOT);
  while (this->focus < end) {
    if (this->focus->type == DEF) {
      consume_node(DEFINE);
    } else {
      consume_node(ASSIGN);
    }
  }
  return node;
}

unique_ptr<Node> Parser::build_assign() {
  auto node = make_unique<Node>(ASSIGN);
  consume_token(ID);
  consume_token(EQUAL);
  consume_node(EXPR);
  consume_token(SEMICOLON);
  return node;
}

unique_ptr<Node> Parser::build_define() {
  auto node = make_unique<Node>(DEFINE);
  consume_token(DEF);
  consume_token(ID);
  consume_token(SEMICOLON);
  return node;
}

/*
unique_ptr<Node> Parser::build_func() {
    auto node = make_unique<Node>(FUNC);
    consume_token(ID);
    consume_token(LP);
    if (this->focus < end && this->focus->type == RP) {
        consume_token(RP);
    } else {
        while (1) {
            consume_node(EXPR);
            if (this->focus < end && this->focus->type == RP) {
                consume_token(RP);
                break;
            }
            consume_token(COMMA);
        }
    }
    return node;
}
*/

unique_ptr<Node> Parser::build_expr() {
  auto node = make_unique<Node>(EXPR);
  while (1) {
    consume_node(BOOL_TERM);
    if (this->focus < end && this->focus->type == BOOL_OP) {
      consume_token(BOOL_OP);
    } else {
      break;
    }
  }
  return node;
}

unique_ptr<Node> Parser::build_bool_term() {
  auto node = make_unique<Node>(BOOL_TERM);
  while (1) {
    consume_node(CMP_TERM);
    if (this->focus < end && this->focus->type == CMP_OP) {
      consume_token(CMP_OP);
    } else {
      break;
    }
  }
  return node;
}

unique_ptr<Node> Parser::build_cmp_term() {
  auto node = make_unique<Node>(CMP_TERM);
  while (1) {
    consume_node(UNIT);
    if (this->focus < end && this->focus->type == ARITH_OP) {
      consume_token(ARITH_OP);
    } else {
      break;
    }
  }
  return node;
}

unique_ptr<Node> Parser::build_unit() {
  auto node = make_unique<Node>(UNIT);
  if (this->focus >= end) {
    throw ParseError(__LINE__);
  }
  switch (this->focus->type) {
  case NUM:
    consume_token(NUM);
    break;
  case ID:
    consume_token(ID);
    break;
  case LP:
    consume_token(LP);
    consume_node(EXPR);
    consume_token(RP);
    break;
  default:
    throw ParseError(__LINE__);
  }
  return node;
}
