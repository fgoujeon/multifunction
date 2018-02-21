# multifunction
multifunction is a C++ utility class template. It is a type-erasing container for a single functor that defines a set of parenthesis operator overloads. In other words, it is a generalization of std::function for any number of signatures.

For example this instance of multifunction:

    multifunction
    <
        void(int),
        void(const char*),
        bool(std::unique_ptr<double>&&)
    >
    
is able to hold a functor of the following forms:

    struct overloaded_functor
    {
        void operator()(int);
        void operator()(const char*);
        bool operator()(std::unique_ptr<double>&&);
        //...
    };

    struct generic_functor
    {
        template<typename Any> auto operator()(Any&&);
        //...
    };

    struct mixed_functor
    {
        template<typename Any> void operator()(const Any&);
        bool operator()(std::unique_ptr<double>&&);
        //...
    };

