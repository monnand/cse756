#include <rose.h>
using namespace std;

extern string prettyPrint(SgProject*);

int main(int argc, char* argv[]) {
  cout << "/* [Main] Start. */" << endl;
  SgProject* project = frontend(argc,argv);
  string result = prettyPrint(project);
  cout << result;
  cout << "/* [Main] Done. */" << endl;
  return 0;
}

