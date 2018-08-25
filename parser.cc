#include <cstdio>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "parser.h"

#define DEBUG
#include "debug.h"

using namespace std;

struct ParseError {
  ParseError(int line) { printf("error at %d\n", line); }
};


#define consume_token(_type)                                                   \
  do {                                                                         \
    if (this->focus == end || this->focus->type != _type) {                                \
      throw ParseError(__LINE__);                                              \
    } else {                                                                   \
      node->kids.push_back(make_unique<Node>(_type));                          \
      node->kids.back()->content = this->focus->content;         \
      debug("%s\n", type_to_name[this->focus->type].data());                               \
      ++this->focus;                                                                 \
    }                                                                          \
  } while (0)

#define consume_node(type)                                                     \
  do {                                                                         \
    node->kids.push_back((this->*(this->type_to_func[type]))());                     \
  } while (0)

Parser::Parser(const Token *b, const Token *e) : focus(b), end(e) {
  type_to_func[READ] = &Parser::build_read;
  type_to_func[WRITE] = &Parser::build_write;
  type_to_func[DEFINE] = &Parser::build_define;
  type_to_func[ASSIGN] = &Parser::build_assign;
  type_to_func[EXPR] = &Parser::build_expr;
  type_to_func[BOOL_TERM] = &Parser::build_bool_term;
  type_to_func[CMP_TERM] = &Parser::build_cmp_term;
  type_to_func[UNIT] = &Parser::build_unit;
  type_to_func[BLOCK] = &Parser::build_block;
  type_to_func[IF_STMT] = &Parser::build_if_stmt;
  type_to_func[WHILE_STMT] = &Parser::build_while_stmt;
  type_to_func[STMT_LIST] = &Parser::build_stmt_list;
}

unique_ptr<Node> Parser::parse() { return build_stmt_list(); }

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
  if (focus->type == SEMICOLON) {
      consume_token(SEMICOLON);
  } else {
      consume_token(EQUAL); 
      consume_node(EXPR);
      consume_token(SEMICOLON);
  }
  return node;
}

unique_ptr<Node> Parser::build_stmt_list() {
  auto node = make_unique<Node>(STMT_LIST);
  while (this->focus < end) {
    switch (this->focus->type) {
        case DEF:
          consume_node(DEFINE);
          break;
        case ID:
          consume_node(ASSIGN);
          break;
        case RD:
          consume_node(READ);
          break;
        case WR:
          consume_node(WRITE);
          break;
        case LBRACE:
          consume_node(BLOCK);
          break;
        case IF:
          consume_node(IF_STMT);
          break;
        case WHILE:
          consume_node(WHILE_STMT);
          break;
        default:
          return node;
      }
  }
  return node;
}

unique_ptr<Node> Parser::build_block() {
  auto node = make_unique<Node>(BLOCK);
  consume_token(LBRACE);
  while (this->focus < end && this->focus->type != RBRACE) {
    consume_node(STMT_LIST);       
  }
  consume_token(RBRACE);
  return node;
}

unique_ptr<Node> Parser::build_while_stmt() {
  auto node = make_unique<Node>(WHILE_STMT);
  consume_token(WHILE);
  consume_token(LP);
  consume_node(EXPR);
  consume_token(RP);
  switch (this->focus->type) {
    case ID:
      consume_node(ASSIGN);
      break;
    case RD:
      consume_node(READ);
      break;
    case WR:
      consume_node(WRITE);
      break;
    case LBRACE:
      consume_node(BLOCK);
      break;
    case IF:
      consume_node(IF_STMT);
      break;
    case WHILE:
      consume_node(WHILE_STMT);
      break;
    default:
      throw ParseError(__LINE__);
  }
  return node;
}

unique_ptr<Node> Parser::build_if_stmt() {
  auto node = make_unique<Node>(IF_STMT);
  consume_token(IF);
  consume_token(LP);
  consume_node(EXPR);
  consume_token(RP);
  switch (this->focus->type) {
    case ID:
      consume_node(ASSIGN);
      break;
    case RD:
      consume_node(READ);
      break;
    case WR:
      consume_node(WRITE);
      break;
    case LBRACE:
      consume_node(BLOCK);
      break;
    case IF:
      consume_node(IF_STMT);
      break;
    case WHILE:
      consume_node(WHILE_STMT);
      break;
    default:
      throw ParseError(__LINE__);
  }
  return node;
}

unique_ptr<Node> Parser::build_read() {
  auto node = make_unique<Node>(READ);
  consume_token(RD);
  consume_token(ID);
  consume_token(SEMICOLON);
  return node;
}

unique_ptr<Node> Parser::build_write() {
  auto node = make_unique<Node>(WRITE);
  consume_token(WR);
  consume_node(EXPR);
  consume_token(SEMICOLON);
  return node;
}

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
