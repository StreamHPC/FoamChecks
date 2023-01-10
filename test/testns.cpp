#include "doubleFloat.h"
#include "int.H"

//int min(int, int);

namespace Foam {
    void g();
    void e() {
        min(1.0, 2.0);
    }
}

namespace ASD {
    double min(double, double);
}

using namespace Foam;

void Foam::g() {
    min(1.0, 1.0);
}

int main() {
    double T = 0.0;
    Foam::min(1.0, T);
    ((max))(
        min(1.0, 2.0),
         1.0);
    min(1.0, 1.0);
    ASD::min(1.0, 1.0);
    min(1,1);
}
