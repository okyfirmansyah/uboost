# uboost
C++11 library intended to bring C++11 feature as much as possible like lambda, coroutine, etc into embedded software environment(currently tested for AVR GCC)

## static vector container
Uboost static vector container is designed based on standard C++11 std::vector container but using full static allocation. As consequence to use thic container we should first detemine maximum item size that can be contained. For declaration and forwarding purpose, vector container class is separated as uboost::vector and uboost::declare::vector. The former is used to pass reference of vector around while the later is used as declaration point(where the size allocation happens).

Like C++11's std::vector, uboost::vector can contains primitive, basic class or non-trivial class that should follow const correctness and its copy-able and move-able properties.

## static list container
Uboost static list container is designed based on standard C++11 std::list container but using full static allocation. Unlike std::list container, uboost::container should be linked to a uboost::listpool where the space in which all list item are contained. Multiple uboost::container can share same uboost::listpool instance, this give advantage when we want to move around list item between two container sharing same listpool instance. The advantage is no moving operation of contained item actually take a place. This add more freedom that is we can add non-moveable non-copyable class instance to the container.


## generalized functor container
(explained soon)

## coroutines
(soon)
