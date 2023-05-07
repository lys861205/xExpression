#include <stdio.h>
#include "xExpression.h"
#include <thread>
#include <time.h>

using namespace route;

std::string Debug(std::map<std::string, Variant>& values)
{
  int V = values["V"].asConstInt();
  int M = values["P"].asConstInt();
  int A = values["A"].asConstInt();
  std::string testAb = values["E"].asConstString();
  char buffer[128] = {0};
  int n = snprintf(buffer, sizeof(buffer), "V=%d M=%d A=%d AB=%s", V, M, A, testAb.c_str());
  return std::string(buffer, n);
}

int getVersion()
{
  static std::vector<int> vers = {
    1208, 1206, 1209, 1205, 1207
  };
  int i = rand() % vers.size();
  return vers[i];
}

void test(ASTExp* ast)
{
    for (int i=0; i < 100000; ++i) {
      std::map<std::string, Variant> m ;
      m["V"] = Variant(getVersion());
      m["P"] = Variant((int)i%2);
      m["A"] = Variant((int)i%2);
      m["E"] = Variant("abtest");
      if (ast->evaluate(m)) {
        printf("\033[1;92m succ. %s %s\033[0m\n", Debug(m).c_str(), ast->getExp().c_str());
      } else {
        printf("\033[1;91m failed. %s %s\033[0m\n", Debug(m).c_str(), ast->getExp().c_str());
      }
    }
}

int main()
{
    srand(time(NULL));
   std::string exp = "V=(1206,1209] && P={1}  && A={1} && E={abtest}";
    ASTExp* ast = XExpression::compile(exp);
    if (!ast) {
        printf("\033[1;91m compile failed.\033[0m\n");
        return 0;
    } else {
        printf("\033[1;92m compile succ.\n\033[0m");
    }
    std::thread ths[10];
    for (int i=0; i < 10; ++i) {
     ths[i] = std::thread(test, ast);
    }
    for (int i=0; i < 10; ++i) {
      ths[i].join();
    }
    delete ast;
    return 0;
}

