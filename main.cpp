#include <iostream>
#include <functional>
#include <utility>
using namespace std;
using real = double;
using unary_fun = function<real(const real&)>;
#define RESOLUTION (1000)
class curve
{
private:
    real a, b;
    unary_fun f;
// risoluzione dell'intervallo [a, b]
public:
    curve(const real& a_, const real& b_, const unary_fun& f_) : f(f_), a(a_), b(b_) {}
    //curve(const real& c) = default;
    real get_dx() const { return (b - a) / RESOLUTION; }
    pair<real, real> interval() const { return pair<real, real>(a, b); }
    real operator()(const real& x) const { return f(x); }
    curve derivative() const {
        return curve(a,b,[&, dx=get_dx()]( const real &x){
            const real dy= (f(x-dx)-f(x))/dx;
            return dy;
        });
    }
    curve primitive() const {
        return curve(a,b,[&, dx=get_dx()](const real& x){
            const real y= f(x)*dx;
            return y;
        });
    }
    real integral() const {
        const unary_fun& F=primitive();
        return F(b)-F(a);
    }
    class iterator
    {
    private:
        const curve& c;
        real x;
    public:
        iterator(const curve& c_, const real& x_) : c(c_), x(x_) {}
        iterator(const iterator& c) = default;
        pair<real, real> operator*() const {

        }
        iterator operator++() { // il pre-incremento modifica sè stesso e non ritorna una copia
            x+=c.get_dx();
            return *this;
        }
        iterator operator++(int) { // il post-incremento fa una copia, modifica sé stesso e ritorna la copia
            iterator tmp= *(this);
            ++(*this);
            return tmp;
        }
        bool operator!=(const iterator& it) const { /* DA IMPLEMENTARE */ }
    };
    iterator begin() const {
        return iterator(*this, a);
    }
    iterator end() const {
        return iterator(*this,b+get_dx());
    }

};

ostream& operator<<(ostream& os, const curve& c)
{
    os << "dom = [" << c.interval().first << ", " << c.interval().second << "] dx = " << c.get_dx() << ": " << endl;
    for (curve::iterator it = c.begin(); it != c.end(); ++it)
    {
        const pair<real, real>& p = *it;
        const real& x = p.first, & y = p.second;
        os << "\tf(" << x << ") = " << y << endl;
    }
    return os << endl;
}

int main()
{
    curve f(-1., 1., [](const real& x) { return x * x; });

    cout << f << endl
         << f.derivative() << endl
         << f.primitive() << endl
         << f.primitive().derivative() << endl
         << f.derivative().primitive() << endl
            ;
    return 0;
}


