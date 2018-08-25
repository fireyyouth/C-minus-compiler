#include "parser.h"
#include <vector>
#include <map>
#include <cassert>

#define DEBUG
#include "debug.h"


using namespace std;

extern map<node_t, string> type_to_name;

string get_label() {
    static size_t i = 0;
    return "label" + to_string(i++);
}

static map<string, string> op_to_jump = {
    {"<", "jl"},
    {"==", "je"},
    {">", "jg"},
    {"&&", "jz"},
    {"||", "jnz"}
};

static vector<string> op_to_instrctions(string op, string left, string right) {
    vector<string> v;
    if (op == "+") {
        v.push_back("add " + left + "," + right);
    }else if(op == "-") {
        v.push_back("sub " + left + "," + right);
    } else if (op == "<" || op == "==" || op == ">") {
        string l1 = get_label();
        string l2 = get_label();
        v.push_back("cmp qword " + left + "," + right);
        v.push_back(op_to_jump[op] + " " + l1);
        v.push_back("mov qword " + left + "," + "0");
        v.push_back("jmp " + l2);
        v.push_back(l1 + ":");
        v.push_back("mov qword " + left + "," + "1");
        v.push_back(l2 + ":");
    } else if (op == "||" || op == "&&") {
        string l1 = get_label();
        string l2 = get_label();
        v.push_back("cmp qword " + left + "," + "0");
        v.push_back(op_to_jump[op] + " " + l1);
        v.push_back("cmp qword " + right + "," + "0");
        v.push_back(op_to_jump[op] + " " + l1);
        v.push_back("mov qword" + left + "," + "1");
        v.push_back("jmp " + l2);
        v.push_back(l1 + ":");
        v.push_back("mov qword" + left + "," + "0");
        v.push_back(l2 + ":");
    } else {
        throw runtime_error("unsupported op " + op);
    }
    return v;
}

size_t lookup_symbol(const vector< map<string, size_t> > & symbol_stack, string name) {
     for (auto scope = symbol_stack.rbegin(); scope != symbol_stack.rend(); ++scope) {
         auto it = scope->find(name);
        if (it != scope->end()) {
            return it->second;
        }
     }
     throw runtime_error("lookup failed");
}

static void gen_expr(unique_ptr<Node> & node, const vector< map<string, size_t> > & symbol_stack) {
    switch (node->type) {
    case NUM:
        printf("push qword %s\n", node->content.data());
        break;
    case ID:
        {
            size_t offset = lookup_symbol(symbol_stack, node->content);
            printf("push qword [rbp - %u * 8]\n", offset);
        }
        break;
    case UNIT:
        if (node->kids.size() == 1) {
            gen_expr(node->kids[0], symbol_stack); 
        } else {
            gen_expr(node->kids[1], symbol_stack); 
        }
        break;
    default:
        {
            gen_expr(node->kids[0], symbol_stack);
            string op;
            for (size_t i = 1; i < node->kids.size(); ++i) {
                if (i % 2 == 1) {
                    op = node->kids[i]->content;
                } else {
                    gen_expr(node->kids[i], symbol_stack);
                    printf("pop rax\n");
                    for (const auto & instruction : op_to_instrctions(op, "[rsp]", "rax")) {
                        printf("%s\n", instruction.data()); 
                    }
                }
            }
        }
    }
}

void gen_stmt(unique_ptr<Node> & stmt, vector< map<string, size_t> > & symbol_stack, size_t & total) {
    switch (stmt->type) {
    case DEFINE:
        {
            string name = stmt->kids[1]->content;
            if (symbol_stack.back().find(name) != symbol_stack.back().end()) {
                throw runtime_error("redefinition in the same scope"); 
            }
            symbol_stack.back()[name] = total++;
            if (stmt->kids.size() == 3) {
                printf("push qword 0\n");
            } else {
                gen_expr(stmt->kids[3], symbol_stack);
            }
        }
        break;
    case ASSIGN:
        {
             string name = stmt->kids[0]->content;
             size_t offset = lookup_symbol(symbol_stack, name);
            gen_expr(stmt->kids[2], symbol_stack);
            printf("pop qword [rbp - %u * 8]\n", offset);
        }
        break;
    case READ:
        {
             string name = stmt->kids[1]->content;
             size_t offset = lookup_symbol(symbol_stack, name);
            printf("push rbx\n");
            printf("lea  rdi, [rel read_format]\n");
            printf("lea  rsi, [rbp - %u * 8]\n", offset);
            printf("xor eax, eax\n");
            printf("call scanf\n");
            printf("pop rbx\n");
        }
        break;
    case WRITE:
        gen_expr(stmt->kids[1], symbol_stack);

        printf("push rbx\n");
        printf("lea  rdi, [rel write_format]\n");
        printf("mov  rsi, [rsp + 8]\n");
        printf("xor eax, eax\n");
        printf("call printf\n");
        printf("pop rbx\n");

        printf("sub rsp, 8\n");
        break;
    
    case BLOCK:
        {
            symbol_stack.push_back(map<string, size_t>());
            for (auto & sub_stmt: stmt->kids[1]->kids) {
                gen_stmt(sub_stmt, symbol_stack, total);
            }
            size_t n = symbol_stack.back().size();
            if (n > 0) {
                printf("sub rsp, %u * 8\n", n); 
                total -= n; 
            }
            symbol_stack.pop_back();
        }
        break;
    case IF_STMT:
        {
            string label = get_label();
            gen_expr(stmt->kids[2], symbol_stack);
            printf("cmp qword [rsp], 0\n");
            printf("jz %s\n", label.data());
            gen_stmt(stmt->kids[4], symbol_stack, total);
            printf("%s:\n", label.data());
            printf("sub rsp, 8\n");
        }
        break;
    case WHILE_STMT:
        {
            string l1 = get_label();
            string l2 = get_label();
            printf("%s:\n", l1.data());
            gen_expr(stmt->kids[2], symbol_stack);
            printf("cmp qword [rsp], 0\n");
            printf("jz %s\n", l2.data());
            gen_stmt(stmt->kids[4], symbol_stack, total);
            printf("jmp %s\n", l1.data());
            printf("%s:\n", l2.data());
            printf("sub rsp, 8\n");
        }
        break;
    default:
        throw runtime_error("unsupported stmt type " + type_to_name[stmt->type]);
    }

}

void gen_code(unique_ptr<Node> & stmt_list) {
    assert(stmt_list->type == STMT_LIST);

    printf("global main\n");
    printf("extern printf\n");
    printf("extern scanf\n");

    printf("section .data\n");
    printf("write_format: db \"%%llu\", 10, 0\n");
    printf("read_format: db \"%%llu\", 0\n");

    printf("section .text\n");
    printf("main:\n");

    printf("push rbp\n");
    printf("lea rbp, [rsp - 8]\n");

    vector< map<string, size_t> > symbol_stack;
    symbol_stack.push_back(map<string, size_t>());
    size_t total = 0;

    for (auto & stmt : stmt_list->kids) {
        gen_stmt(stmt, symbol_stack, total);         
    }

    size_t n = symbol_stack.back().size();
    if (n > 0) {
        printf("sub rsp, %u * 8\n", n); 
        total -= n;
    }
    symbol_stack.pop_back();
    assert(symbol_stack.empty());

    printf("pop rbp\n");
    printf("ret\n");

}