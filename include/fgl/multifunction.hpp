#ifndef FGL_MULTIFUNCTION_HPP
#define FGL_MULTIFUNCTION_HPP

#include <memory>

namespace fgl
{

namespace multifunction_detail
{
    template<class... Ts>
    struct icallable_base;

    template<class R, class... Args, class... Ts>
    struct icallable_base<R(Args...), Ts...>: icallable_base<Ts...>
    {
        using icallable_base<Ts...>::operator(); //don't hide other overloads
        virtual ~icallable_base(){}
        virtual R operator()(Args...) = 0;
    };

    template<class R, class... Args>
    struct icallable_base<R(Args...)>
    {
        virtual ~icallable_base(){}
        virtual R operator()(Args...) = 0;
    };

    template<class... Ts>
    struct icallable: icallable_base<Ts...>
    {
        virtual ~icallable(){}
        virtual icallable* clone() const = 0;
    };



    template<class IBase, class F, class... Ts>
    class callable_base;

    template<class IBase, class F, class R, class... Args, class... Ts>
    class callable_base<IBase, F, R(Args...), Ts...>: public callable_base<IBase, F, Ts...>
    {
        public:
            callable_base(const F& f):
                callable_base<IBase, F, Ts...>(f)
            {
            }

            R operator()(Args... args)
            {
                return this->f_(std::forward<Args>(args)...);
            }
    };

    template<class IBase, class F>
    class callable_base<IBase, F>: public IBase
    {
        public:
            callable_base(const F& f):
                f_(f)
            {
            }

        protected:
            F f_;
    };

    template<class F, class... Ts>
    class callable: public callable_base<icallable<Ts...>, F, Ts...>
    {
        public:
            callable(const F& f):
                callable_base<icallable<Ts...>, F, Ts...>(f)
            {
            }

            icallable<Ts...>* clone() const
            {
                return new callable{this->f_};
            }
    };
}

template<class... Ts>
class multifunction
{
    private:
        std::unique_ptr<multifunction_detail::icallable<Ts...>> callable_;

    public:
        multifunction()
        {
        }

        template<class F>
        multifunction(const F& f):
            callable_(new multifunction_detail::callable<F, Ts...>{f})
        {
        }

        multifunction(const multifunction& rhs):
            callable_(rhs.callable_->clone())
        {
        }

        multifunction& operator=(const multifunction& rhs)
        {
            callable_.reset(rhs.callable_->clone());
        }

        template<class... Args>
        auto operator()(Args&&... args) -> decltype((*callable_)(std::forward<Args>(args)...))
        {
            return (*callable_)(std::forward<Args>(args)...);
        }
};

} //namespace fgl

#endif
