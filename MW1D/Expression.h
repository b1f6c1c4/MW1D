#pragma once
#include "stdafx.h"
#include <memory>
#include "boost/flyweight.hpp"

class IExpression
{
public:
    virtual ~IExpression() { }

    virtual std::ostream &ToMathematica(std::ostream &out) const = 0;

protected:
    IExpression() { }
};

inline std::ostream &operator<<(std::ostream &out, const IExpression &exp)
{
    return exp.ToMathematica(out);
}

template<typename T>
class Symbol : public IExpression
{
public:
    explicit Symbol(T v) : m_Value(v) { }

    ~Symbol() { }

    std::ostream &ToMathematica(std::ostream &out) const override
    {
        return out << m_Value;
    }

private:
    T m_Value;
};

class MergedExpression : public IExpression
{
public:
    explicit MergedExpression(std::string head) : m_Head(std::move(head)) { }
    ~MergedExpression() { }

    void push_back(std::shared_ptr<IExpression> content)
    {
        m_Content.push_back(content);
    }

    std::ostream& ToMathematica(std::ostream& out) const override
    {
        out << m_Head;
        out << '[';

        for (size_t i = 0; i < m_Content.size(); i++)
        {
            if (i > 0)
                out << ',';
            out << *m_Content[i];
        }

        out << ']';
        
        return out;
    }
    
private:
    boost::flyweight<std::string> m_Head;

    std::vector<std::shared_ptr<IExpression>> m_Content;
};

class prob : public std::shared_ptr<IExpression>
{
public:
    // ReSharper disable CppNonExplicitConvertingConstructor
    prob() : std::shared_ptr<IExpression>() { }
    prob(int value) : std::shared_ptr<IExpression>(std::make_shared<Symbol<int>>(value)) { }
    prob(size_t value) : std::shared_ptr<IExpression>(std::make_shared<Symbol<size_t>>(value)) { }
    prob(double value) : std::shared_ptr<IExpression>(std::make_shared<Symbol<double>>(value)) { }
    prob(std::shared_ptr<IExpression> ptr) : std::shared_ptr<IExpression>(ptr) { }
    // ReSharper restore CppNonExplicitConvertingConstructor
};

inline prob operator+(prob p1, prob p2)
{
    auto ptr = std::make_shared<MergedExpression>("Plus");
    ptr->push_back(p1);
    ptr->push_back(p2);

    std::shared_ptr<IExpression> ptrE = ptr;
    return ptrE;
}

inline prob operator-(prob p1, prob p2)
{
    auto ptr = std::make_shared<MergedExpression>("Subtract");
    ptr->push_back(p1);
    ptr->push_back(p2);

    std::shared_ptr<IExpression> ptrE = ptr;
    return ptrE;
}

inline prob operator*(prob p1, prob p2)
{
    auto ptr = std::make_shared<MergedExpression>("Times");
    ptr->push_back(p1);
    ptr->push_back(p2);

    std::shared_ptr<IExpression> ptrE = ptr;
    return ptrE;
}

inline prob operator/(prob p1, prob p2)
{
    auto ptr = std::make_shared<MergedExpression>("Divide");
    ptr->push_back(p1);
    ptr->push_back(p2);

    std::shared_ptr<IExpression> ptrE = ptr;
    return ptrE;
}

inline prob &operator+=(prob &p1, prob p2)
{
    return p1 = p1 + p2;
}

inline prob &operator-=(prob &p1, prob p2)
{
    return p1 = p1 - p2;
}

inline prob &operator*=(prob &p1, prob p2)
{
    return p1 = p1 * p2;
}

inline prob &operator/=(prob &p1, prob p2)
{
    return p1 = p1 / p2;
}

inline std::ostream &operator<<(std::ostream &out, const prob &exp)
{
    return exp->ToMathematica(out);
}

inline prob MakeSymbol(std::string &&name)
{
    return std::shared_ptr<IExpression>(std::make_shared<Symbol<std::string>>(name));
}
