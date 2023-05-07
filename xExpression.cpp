#include "xExpression.h"

namespace route {

ASTExp::ASTExp(const std::string& exp):
_tree(nullptr),
_exp(exp)
{
}

ASTExp::ASTExp() 
{
}

ASTExp::~ASTExp()
{
    release(_tree);
}

void ASTExp::release(TreeNode* t)
{
    if (t) {
        release(t->l);
        release(t->r);
        SAFE_RELEASE(t);
    }
}

bool ASTExp::parse(std::stack<std::string>& tokens)
{
    bool ret = true;
    _tree = buildTree(tokens, ret);
    if (ret) {
      ret = isValid(_tree);
    } 
    return ret;
}

bool ASTExp::isValid(TreeNode* t)
{
    if (!t) {
      return true;
    }
    if (t->type == AND || t->type == OR) {
      if (t->l == nullptr || t->r == nullptr) {
        return false;
      }
    }
    else {
      if (t->l != nullptr || t->r != nullptr) {
        return false;
      }
    }
    return isValid(t->l) && isValid(t->r);
}

TreeNode* ASTExp::buildTree(std::stack<std::string>& tokens, bool& ret)
{
    if (tokens.size() <= 0) {
        return nullptr;
    }
    std::string token = tokens.top();
    token.erase(std::remove_if(token.begin(), token.end(), [](unsigned char ch){ return std::isspace(ch);}), token.end());
    tokens.pop();
    TreeNode* root = new TreeNode();
    if (!root->build(token)) {
        fprintf(stderr, "error token format: %s\n", token.c_str());
        ret = false;
        SAFE_RELEASE(root);
        return root;
    }
    if (root->type == AND || root->type == OR) {
        root->l = buildTree(tokens, ret);
    }
    else {
       TreeNode* ptr = buildTree(tokens, ret);
       if (ptr) {
        ptr->r = root;
        root = ptr;
       }
    }
    return root;
}

bool ASTExp::evaluate(const std::map<std::string, Variant>& values)
{
    return match(_tree, values);
}

std::string ASTExp::getExp() const
{
    return _exp;
}

bool ASTExp::match(TreeNode* t, const std::map<std::string, Variant>& values)
{
    if (!t) {
        return true;
    }
    if (t->type == NUM) {
        auto it = values.find(t->name);
        if (it == values.end()) {
            return false;
        }
        const Variant& data = it->second;
        if (data.isInt()) {
            return t->valid<int32_t>(data.asConstInt());
        } else if (data.isUInt()) {
            return t->valid<uint32_t>(data.asConstUInt());
        } else if (data.isString()) {
            return t->valid<std::string>(data.asConstString());
        } else if (data.isDouble()) {
            return t->valid<double>(data.asConstDouble());
        } else if (data.isFloat()) {
            return t->valid<float>(data.asConstFloat());
        } else {
            fprintf(stderr, "Not support type\n");
            return false;
        }
    }
    else if (t->type == AND) {
        return match(t->l, values) && match(t->r, values);
    }
    else if (t->type == OR) {
        return match(t->r, values) || match(t->l, values);
    }
    return false;
}

ASTExp* XExpression::compile(const std::string& exp)
{
    std::stack<std::string> tokens;
    if (!lexer(exp, tokens)) {
        return nullptr;
    }
    ASTExp* pExp = new ASTExp(exp);
    if (!pExp->parse(tokens)) {
        SAFE_RELEASE(pExp);
    }
    return pExp;
}

bool XExpression::lexer(const std::string& exp, std::stack<std::string>& tokens)
{
    char prec;
    char curc;
    std::string token;
    bool can_push = false;
    for (size_t i=0; i < exp.size(); ++i) {
        curc = exp[i];
        if (curc == ' ' && (prec == '&' || prec == '|')) {
            fprintf(stderr, "format error : %s\n", exp.c_str());
            return false;
        }
        if (curc == ' ') {
          continue;
        }
        if (curc == '|' && prec == '|') {
            can_push = true;
        }
        else if (curc == '&' && prec == '&') {
            can_push = true;
        }
        else if (curc == '&' && prec == '|') {
            fprintf(stderr, "format error : %s\n", exp.c_str());
            return false;
        }
        else if (curc == '|' && prec == '&') {
            fprintf(stderr, "format error : %s\n", exp.c_str());
            return false;
        }
        else if (curc == '|' || curc == '&') {
            if (token.size() <= 0) {
              fprintf(stderr, "format error : %s\n", exp.c_str());
              return false;
            }
            tokens.push(token);
            token.clear();
        }
        else if (prec == '|' || prec == '&') {
            fprintf(stderr, "format error : %s\n", exp.c_str());
            return false;
        }
        token += curc;
        if (can_push) {
            tokens.push(token);
            token.clear();
            can_push = false;
            curc = ' ';
        }
        prec = curc;
    }
    if (token.size()) {
        tokens.push(token);
    }
    return true;
}

} //end namespace route


