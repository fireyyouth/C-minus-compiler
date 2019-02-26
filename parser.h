#ifndef PARSER_H
#define PARSER_H
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "node.h"

class Parser {
public:
  Parser(const Token *b, const Token *e);
  std::unique_ptr<Node> parse();
private:
const Token *focus;
  const Token *const end;

  std::map<node_t, std::unique_ptr<Node> (Parser::*)()> type_to_func;


  std::unique_ptr<Node> build_stmt_list();
  std::unique_ptr<Node> build_read();
  std::unique_ptr<Node> build_write();
  std::unique_ptr<Node> build_assign();
  std::unique_ptr<Node> build_define();
  std::unique_ptr<Node> build_func();
  std::unique_ptr<Node> build_expr();
  std::unique_ptr<Node> build_bool_term();
  std::unique_ptr<Node> build_cmp_term();
  std::unique_ptr<Node> build_add_term();
  std::unique_ptr<Node> build_unit();
  std::unique_ptr<Node> build_stmt();
  std::unique_ptr<Node> build_block();
  std::unique_ptr<Node> build_while_stmt();
  std::unique_ptr<Node> build_if_stmt();
};
#endif
