#include <rose.h>
using namespace std;

int main(int argc, char* argv[])
{
  cout << "[ROSE] Starting frontend\n";
  SgProject* project = frontend(argc, argv);
  cout << "[ROSE] Done with frontend; starting backend\n";
  int status = backend(project);
  cout << "[ROSE] Done with backend (status " << status << ")\n";
  return status;
}
