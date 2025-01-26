#include "complexObject.h"
int main() {
    complexObject c1("22-1.1i");
    mod(c1);
    cout << c1.getRes() << endl;
}