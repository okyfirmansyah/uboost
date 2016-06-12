#include <iostream>
//#include <vector>

#include "../../include/uboost.hpp"
#include "../../include/vector.hpp"
#include "../../include/list.hpp"

class int_test
{
    public:
    int_test(int val)
    :test(new int(val))
    {

    };

    //!!! TODO: vector should be able to contains
    //object with no default constructor
    int_test(){};//=delete;

    int_test(const int_test&)=delete;
    int_test(int_test&& t)
    {
        test=t.test;
        t.test= new int(0);
    };

    bool operator==(int val)
    {
        return (val==*test);
    };

    int_test& operator=(const int_test&)=delete;
    int_test& operator=(int_test&& t)
    {
        delete test;
        test=t.test;
        t.test=new int(0);
        return *this;
    }

    ~int_test()
    {
        delete test;
    }

    private:
    int* test;
};

using namespace uboost;
bool vector_assign_test()
{
    bool result=true;
    // assign() and at() test
    declare::vector<uint32_t,100>  tv, tv2;
    tv.push_back(1);
    tv.push_back(2);
    tv.push_back(3);

    tv2=tv;
    auto first=tv.cbegin()+1;
    auto last=tv.cend();
    tv2.assign(first, last);

    result&=(tv2.size()==2);
    result&=(tv2.back()==3);
    tv2.pop_back();
    result&=(tv2.back()==2);
    tv2.pop_back();

    result&=(tv2.empty());

    tv2=tv;
    first=tv.cbegin();
    last=tv.cend()-1;
    tv2.assign(first, last);
    result&=tv2.at(0)==1;
    result&=tv2.at(1)==2;
    result&=(tv2.size()==2);

    tv2.assign(tv.cbegin(), tv.cbegin());
    tv2.push_back(0);
    tv2.assign(first, last);

    result&=tv2.at(0)==1;
    result&=tv2.at(1)==2;
    result&=(tv2.size()==2);



    declare::vector<int_test,100>  itv, itv2;
    typedef vector<int_test>::iterator iter_t;
    itv.emplace_back(1);
    itv.emplace_back(2);
    itv.emplace_back(3);


    //itv2=itv; this shouldnt compile
    auto ifirst=itv.begin()+1;
    auto ilast=itv.end();

    itv2.assign(move_iterator<iter_t>(itv.begin()), move_iterator<iter_t>(itv.begin()+1));
    itv2.assign(move_iterator<iter_t>(ifirst), move_iterator<iter_t>(ilast));

    result&=(itv2.size()==2);
    result&=(itv2.back()==3);
    itv2.pop_back();
    result&=(itv2.back()==2);
    itv2.pop_back();

    result&=(itv2.empty());

    return result;
};

bool vector_emplace_test()
{
    // basic test

    // random emplace test
}

bool vector_push_pop_test()
{
}

bool vector_resize_test()
{
}

bool vector_erase_test()
{
}

bool vector_insert_test()
{
}

bool vector_iterator_test()
{
}

bool vector_test_main()
{
    bool result=true;
    result&=vector_assign_test();
    //result&=vector_stl_test();
    //result&=vector_iterator_test();
    return result;
}
