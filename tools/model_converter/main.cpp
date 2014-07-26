#include <iostream>

int main(int argc, char** argv)
{
    if (argc <= 2)
    {
        std::cout << "Usage: model_converter sourceFile destFile\n";
        return 1;
    }

    return 0;
}
