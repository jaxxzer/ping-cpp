#include <cstdio>

class Base
{
public:
    void call() {
        handle();
    }

    virtual void handle() {
        printf("base\n\r");
    }
};

class Derived : public Base
{
    void handle() override {
        printf("derived\n\r");
        Base::handle();
    }
};

int main()
{
    Derived d;
    d.call();
    return 0;
}
