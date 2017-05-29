#ifndef MODUL3_LAB1_BF_H
#define MODUL3_LAB1_BF_H


#include <iostream>
#include <string>
#include <vector>
#include <math.h>

class boolean_function
{
public:
    using value_type = bool;
    using size_type = size_t;
    using reference = std::vector<value_type>::reference;
    using const_reference = std::vector<value_type>::const_reference;

    using iterator = typename std::vector<value_type>::iterator;
    using const_iterator = typename std::vector<value_type>::const_iterator;

private:
    std::vector<value_type> func;

public:

    // возвращает функцию которая равна xn с размерностью dimension
    // пример 1
    // n = 0
    // dimension = 3
    // вернется функция "01010101"
    // пример 2
    // n = 1
    // dimension = 4
    // вернется функция "0011001100110011"
    static boolean_function var(size_t n, size_t dimension)
    {
        int len = pow(2, dimension);
        int k = pow(2, n);

        std::vector<value_type> res;
        while (res.size() != len)
        {
            for (int j = 0; j != k; j++)
                res.push_back(0);
            for (int j = 0; j != k; j++)
                res.push_back(1);
        }

        boolean_function f(res);
        return f;
    }

    // тождественный ноль "от dimension переменных"
    static boolean_function zero(size_t dimension)
    {
        return boolean_function(0, dimension);
    }

    // тождественная единица "от dimension переменных"
    static boolean_function one(size_t dimension)
    {
        return boolean_function(1, dimension);
    }

    static boolean_function from_anf(std::vector<value_type> v)
    {
        size_type sz = v.size();
        std::vector<value_type> res(sz);

        for (size_type i = 0; i < sz; ++i)
        {
            size_type j = sz - 1 - i;
            res[j] = v[j];
            for (size_type k = j + 1; k < sz; ++k)
            {
                res[k] = res[k - 1] ^ res[k];
            }
        }
        return boolean_function(res);
    }

    boolean_function() = default;

    // задает фунцию тождественного нуля от n переменных
    boolean_function(size_t n)
    {
        int k = pow(2, n);
        for (int i = 0; i < k; i++)
            func.push_back(0);
    }

    // задает фунцию от n переменных. значение равно value
    // пример
    // пусть value = 14, т.е. 0...00001110 в двоичной системе
    // а n = 3
    // тогда АНФ boolean_function будет равна x + y + xy + zx + zy + zyx
    boolean_function(unsigned long long value, size_type n)
    {
        int i = 0;
        while (value > 0)
        {
            func.push_back(value % 2);
            i++;
            value /= 2;
        }
        int len = pow(2, n);
        for (i; i < len; i++)
        {
            func.push_back(0);
        }
    }

    // пусть table = "01110000"
    // тогда АНФ boolean_function будет равна x + y + xy + zx + zy + zyx
    boolean_function(const std::string& table)
    {
        int k = 0;
        int ch = table.size();
        while (ch > 0)
        {
            k += ch % 2;
            ch /= 2;
        }
        if (k != 1) throw std::exception();

        for (int i = 0; i < table.size(); i++)
        {
            if (table[i] == '0')
                func.push_back(0);
            else
                func.push_back(1);
        }
    }

    // пусть table = {true, false, false, true};
    // тогда АНФ boolean_function будет равна x + y + 1
    boolean_function(const std::vector<value_type>& table)
    {
        int k = 0;
        int ch = table.size();
        while (ch > 0)
        {
            k += ch % 2;
            ch /= 2;
        }
        if (k != 1) throw std::exception();

        func = table;
    }
    boolean_function(const std::initializer_list<bool> vars)
    {
        int k = 0;
        int ch = vars.size();
        while (ch > 0)
        {
            k += ch % 2;
            ch /= 2;
        }
        if (k != 1) throw std::exception();

        for (auto &v : vars)
        {
            func.push_back(v);
        }
    }

    boolean_function(const boolean_function& table)
    {
        func = table.func;
    }

    boolean_function& operator=(const boolean_function& rhs)
    {
        func = rhs.func;
        return *this;
    }

    // сложение по модулю 2
    // если разное количество переменных - исключение
    boolean_function& operator += (const boolean_function& rhs)
    {
        if (func.size() != rhs.func.size())
            throw std::exception();


        for (int i = 0; i < size(); i++)
        {
            func[i] = (func[i] + rhs.func[i]) % 2;
        }
        return *this;
    }

    // конъюнкция
    // если разное количество переменных - исключение
    boolean_function& operator *= (const boolean_function& rhs)
    {
        if (func.size() != rhs.func.size())
            throw std::exception();


        for (int i = 0; i < size(); i++)
        {
            func[i] = func[i] * rhs.func[i];
        }
        return *this;
    }

    // дизъюнкция
    // если разное количество переменных - исключение
    boolean_function& operator |= (const boolean_function& rhs)
    {
        if (func.size() != rhs.func.size())
            throw std::exception();


        for (int i = 0; i < size(); i++)
        {
            if (func[i] == 0 && rhs.func[i] == 0)
                func[i] = 0;
            else
                func[i] = 1;
        }
        return *this;
    }

    // побитовое отрицание
    boolean_function operator ~ ()
    {
        for (int i = 0; i < size(); i++)
        {
            if (func[i] == 0)
                func[i] = 1;
            else
                func[i] = 0;
        }
        return *this;
    }

    // true если функции равны
    //  иначе false
    bool operator == (const boolean_function& rhs) const
    {
        size_type s1 = size();
        size_type s2 = rhs.size();

        if(s1 != s2)
            return false;

        for (size_type i = 0; i < s1; i++)
        {
            if (func[i] != rhs.func[i])
                return false;
        }

        return true;
    }

    // true если одна функция сравнима и больше или равна rhs
    // false если сравнима и меньше rhs
    // logic_error если фунции не сравнимы
    bool operator >= (const boolean_function& rhs) const
    {
        if (size() != rhs.size())
            throw std::exception();

        int fl1 = 0;
        int fl2 = 0;
        for (int i = 0; i < size(); i++)
        {
            if (func[i] > rhs.func[i])
                fl1 = 1;
            if (func[i] < rhs.func[i])
                fl2 = 2;
        }

        if (fl1 != 0 && fl2 != 0)
            throw std::exception();
        else
            return fl1 != 0 && fl2 == 0 || fl1 == 0 && fl2 == 0;
    }

    reference operator[](size_type ind)
    {
        if (ind < 0 || ind >= size() || size() == 0)
            throw std::exception();

        return func[ind];
    }
    const_reference operator[](size_type ind) const
    {
        if (ind < 0 || ind >= size() || size() == 0)
            throw std::exception();

        return func[ind];
    }

    reference at(size_type ind)
    {
        if (ind < 0 || ind >= size() || size() == 0)
            throw std::exception();

        return func[ind];
    }
    const_reference at(size_type ind) const
    {
        if (ind < 0 || ind >= size() || size() == 0)
            throw std::exception();

        return func[ind];
    }

    iterator begin()
    {
        return func.begin();
    }
    iterator end()
    {
        return func.end();
    }

    const_iterator cbegin() const
    {
        return func.cbegin();

    }
    const_iterator cend() const
    {
        return func.cend();
    }


    // длина столбца функции
    size_type size() const throw()
    {
        return func.size();
    }

    // количество переменных
    size_type dimension() const throw()
    {
        return log2(func.size());
    }

    // возвращает значение функции
    // пусть boolean_function задает функцию f(x, y, z)
    // тогда функция вернет f(vars[0], vars[1], vars[2])
    bool operator()(const std::vector<bool>& vars) const
    {
        if (vars.size() != dimension())
            throw std::exception();

        size_type index = 0;
        size_type i = 0;
        for (bool v : vars)
        {
            index += pow(2, i) * v;
            ++i;
        }
        return func[index];
    }
    bool operator()(const std::initializer_list<bool> vars) const
    {
        if (vars.size() != dimension())
            throw std::exception();

        size_type index = 0;
        size_type i = 0;
        for (bool v : vars)
        {
            index += pow(2, i) * v;
            ++i;
        }
        return func[index];
    }


    // T(x1, x2, ..., xN) - текущая функция
    // operator вернет новую функцию, которая равна композиции G = T(fs[0], fs[1], ..., fs[N-1])
    boolean_function operator()(const std::vector<boolean_function>& fs) const
    {
        if (fs.size() != dimension())
            throw std::exception();

        if (fs.size() == 0)
            return boolean_function();


        size_type d = 0;
        for (boolean_function const& f : fs)
        {
            if (f.dimension() > d)
                d = f.dimension();
        }

        boolean_function result(d);
        for (size_type i = 0; i < result.func.size(); ++i)
        {
            std::vector<bool> vars;
            for (boolean_function const& f : fs)
            {
                vars.push_back(f.func[i % f.size()]);
            }
            result.func[i] = (*this)(vars);
        }

        return result;
    }
    boolean_function operator()(const std::initializer_list<boolean_function> vars) const
    {
        if (vars.size() != dimension())
            throw std::exception();

        if (vars.size() == 0)
            return boolean_function();


        size_type d = 0;
        for (boolean_function const& v : vars)
        {
            if (v.dimension() > d)
                d = v.dimension();
        }

        boolean_function result(d);
        for (size_type i = 0; i < result.func.size(); ++i)
        {
            std::vector<bool> var;
            for (boolean_function const& v : vars)
            {
                var.push_back(v.func[i % v.size()]);
            }
            result.func[i] = (*this)(var);
        }

        return result;
    }

    bool is_monotone() const
    {
        for (size_type i = 0; i < func.size(); ++i)
        {
            for (size_type j = 0; j < dimension(); ++j)
            {
                if ((i & size_type(pow(2, j))) == 0)
                {
                    if (func[i + size_type(pow(2, j))] < func[i])
                        return false;
                }
            }
        }

        return true;
    }
    bool is_symmetric() const
    {
        std::vector<value_type> res;
        for (unsigned long i = func.size() - 1; i > -1; i--)
        {
            if (func[i] == 1)
                res.push_back(0);
            else
                res.push_back(1);
        }

        return !(res != func);
    }
    bool is_linear() const
    {
        std::vector<value_type> f = anf();
        for (size_type i = dimension() + 1; i < f.size(); ++i)
        {
            if (f[i])
                return false;
        }
        return true;
    }
    bool is_T1() const
    {
        return (func[func.size()-1] == 1);
    }
    bool is_T0() const
    {
        return (func[0] == 0);
    }
    bool is_balanced() const //равновесная ли
    {
        return weight() == (size() / 2);
    }
    size_t weight() const
    {
        size_t res = 0;
        for (int i = 0; i < size(); i++)
        {
            if (func[i])
                res++;
        }
        return res;
    }

    bool is_functionally_complete_system() const
    {
        return !is_T0() && !is_T1() && !is_linear() && !is_symmetric() && !is_monotone();
    }

    // Возвращает АНФ функции
    std::vector<value_type> anf() const
    {
        std::vector<value_type> res;
        std::vector<value_type> f = func;
        for (size_t i = 0; i < func.size(); ++i)
        {
            res.push_back(f[0]);
            for (size_t k = 0; k < f.size() - 1; ++k)
            {
                if(f[k] == f[k+1])
                    f[k] = 0;
                else
                    f[k] = 1;
            }
            f.pop_back();
        }
        return res;
    }
};

// пусть boolean_function представляет из себя функцию "01110000"
// тогда get_polynom вернет строку "x0 + x1 + x0x1 + x0x2 + x1x2 + x0x1x2"
std::string get_polynom(const boolean_function& rhs)
{
    boolean_function c = rhs.anf();
    std::string res;
    for (int i = 0; i < c.size(); i++)
    {
        if (c[i] != 0)
        {
            if (i == 0)
            {
                res += "1";
                res += " + ";
                continue;
            }

            int ch = i;
            std::vector<int> num;
            while (ch > 0)
            {
                num.push_back(ch % 2);
                ch /= 2;
            }

            for (int j = 0; j < num.size(); j++)
            {
                if (num[j] == 1)
                {
                    res += "x";
                    res += std::to_string(j+1);
                }
            }

            if (i + 1 != c.size())
                res += " + ";
        }
    }

    return res;
}

boolean_function operator + (const boolean_function& a, const boolean_function& b)
{
    boolean_function f(a);
    f += b;
    return f;
}
boolean_function operator * (const boolean_function& a, const boolean_function& b)
{
    boolean_function f(a);
    f *= b;
    return f;
}
boolean_function operator | (const boolean_function& a, const boolean_function& b)
{
    boolean_function f(a);
    f |= b;
    return f;
}
bool operator != (const boolean_function& a, const boolean_function& b)
{
    return !(a == b);
}







#endif //MODUL3_LAB1_BF_H