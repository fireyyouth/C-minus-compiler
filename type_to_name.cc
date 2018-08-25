#include <map>
#include <string>
#include "node.h"
std::map<node_t, std::string> type_to_name = {{ASSIGN, "ASSIGN"},
                                          {DEFINE, "DEFINE"},
                                          {WHILE_STMT, "WHILE_STMT"},
                                          {WHILE, "WHILE"},
                                          {IF_STMT, "IF_STMT"},
                                          {IF, "IF"},
                                          {STMT, "STMT"},
                                          {BLOCK, "BLOCK"},
                                          {WRITE, "WRITE"},
                                          {READ, "READ"},
                                          {RD, "RD"},
                                          {WR, "WR"},
                                          {EXPR, "EXPR"},
                                          {BOOL_TERM, "BOOL_TERM"},
                                          {CMP_TERM, "CMP_TERM"},
                                          {UNIT, "UNIT"},
                                          {STMT_LIST, "STMT_LIST"},
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


