#include <iostream>
#include <stdlib.h>

bool vector_test_main();
int main()
{
    if(!vector_test_main())
        std::cout<<"testing vector library failed!\r\n";

    std::cout<<"done!\r\n";
    return 0;
};
