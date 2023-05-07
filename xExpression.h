#pragma once

#include "checker.h"
#include "StringUtil.h"
#include "Variant.h"

#include <string.h>
#include <map>
#include <stack>
#include <functional>

namespace route {

#define SAFE_RELEASE(p) \
if (p) { \
    delete p; \
    p = nullptr; \
}

enum Type {INVALID, NUM, AND, OR};

using Creator = std::function<IChecker*(void)>;
using CheckerCreatorMap = std::map<std::string, Creator>;
const CheckerCreatorMap checker_map = {
   {"V",  std::bind(IntChecker::Create)},
   {"P",  std::bind(IntChecker::Create)},
   {"A",  std::bind(IntChecker::Create)},
   {"L",  std::bind(IntChecker::Create)},
   {"E", std::bind(StringChecker::Create)}
};

struct TreeNode {
   std::string name;
   Type type;
   IChecker* p;
   TreeNode* r;
   TreeNode* l;
   TreeNode():type(INVALID), p(nullptr), r(nullptr), l(nullptr) {}
   ~TreeNode() {
        SAFE_RELEASE(p);
   }
   bool build(const std::string& str)
   {
    if (str == "||") {
        name = str;
        type = OR;
        return true;
    }
    else if (str == "&&") {
        name = str;
        type = AND;
        return true;
    } else {
        std::vector<std::string> values;
        if (gsl::StringSplit(str, values, '=') < 2) {
            return false;
        }
        type = NUM;
        name = values[0];
        auto it = checker_map.find(name);
        if (it == checker_map.end()) {
            return false;
        }
        p = it->second();
        int ret = p->Parser(values[1]);
        if (ret) {
            SAFE_RELEASE(p);
        }
        return ret == 0;
    }
   }

   template<class T>
   bool valid(const T& value) 
   {
    return p ? p->IsValid(value) : false;
   }
};

class ASTExp {
public:
    ASTExp(const std::string& exp);
    ASTExp();
    ~ASTExp();
    
    bool parse(std::stack<std::string>& tokens);

    bool evaluate(const std::map<std::string, Variant>& values);

    std::string getExp() const;
private:
    bool match(TreeNode* t, const std::map<std::string, Variant>& values);
    TreeNode* buildTree(std::stack<std::string>& tokens, bool& ret);
    void release(TreeNode* t); 
    // 1 需要是满二叉树，所有叶子节点都是值，其他节点是逻辑操作符
    bool isValid(TreeNode* t);
private:
    TreeNode* _tree;
    std::string _exp;
}; // ASTExp

class XExpression {
public:
    XExpression() = delete;
    ~XExpression() = delete;
public:
    static ASTExp* compile(const std::string& exp);
private:
    static bool lexer(const std::string& exp, std::stack<std::string>& tokens);
}; // end XExpression

} // end namespace route
