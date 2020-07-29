#include "BigIntegers.h"

namespace aj{

    // constants:
    constexpr auto LOG_2 = 0.301; 
    constexpr long zero_shift = static_cast<double>((sizeof(int) / 2) * 8) * LOG_2;
    const int BigInt::D = calculateD();


    // constructors: 
    // public:
    BigInt::BigInt()
    {
        bSign_ = plus;
        strNum_ = '0';
        dqNum_ = {0};
    }

    BigInt::BigInt(std::string s)
    {
        assert(IsNum(s) && "Entered string is not a number");

        if(isdigit(s[0]))
        {
            bSign_ = plus;
            strNum_ = s;
        }
        else if(s[0] == '-')
        {
            bSign_ = minus;
            strNum_ = s.substr(1);
        }
        else if (s[0] == '+')
        {
            bSign_ = plus;
            strNum_ = s.substr(1);
        }   
        strToDQ(dqNum_, strNum_);           
    }

    // private:
    BigInt::BigInt(const std::deque<int>& dq, g_sign sign)
    {
        dqNum_ = dq;
        DQToStr(dq, strNum_);
        bSign_ = sign;
        // if minus
        if(!sign)
        strNum_ = "-" + strNum_;
    }

// ------------------------------------------------------------------------------
    // public methods:

    std::string BigInt::getNum() const
    {
        return strNum_;
    }

    // for debugging:
    g_sign BigInt::getSign() const
    {
        return bSign_;
    } 

    std::deque<int> BigInt::getDQNum() const
    {
        return dqNum_;
    }

    ulong BigInt::size() const
    {
        return strNum_.size();
    }

    BigInt BigInt::operator + (const BigInt& rOperand)
    {
        return add(*this, rOperand);
    }

// ------------------------------------------------------------------------------
    // private methods: 

    BigInt BigInt::add(const BigInt& n1, const BigInt& n2)
    {
        // check if this works:
        if(n1.bSign_ == n2.bSign_)
        {
            if (n1.size() >= n2.size())
            {
                auto res(n1.getDQNum());
                const auto& l1 = n1.getDQNum(), l2 = n2.getDQNum();
                int i = 0, carry = 0;
                while (i < l2.size())
                {
                    auto temp = res[i] + l2[i] + carry;
                    res[i++] = temp % D;
                    carry = temp / D;
                }
                while (i < res.size())
                {
                    auto temp = res[i] + carry;
                    res[i++] = temp % D;
                    carry = temp / D;
                }
                if(carry)
                    res.push_back(carry);
                
                return BigInt(res, n1.bSign_);
            }
            else
            {
                return add(n2, n1);
            }
            
            
        }
        else    // call subtract
            return BigInt();
    }

// ------------------------------------------------------------------------------
    // static private methods:     

    bool BigInt::IsNum(const std::string& s)
    {
        if (s.size())
        {
            if(isdigit(s[0]))
            {
                for(auto n : s)
                    if(!isdigit(n))
                        return false;
                return true;        
            }
            else if (s.size() > 1 && (s[0] == '+' || s[0] == '-'))
            {
                for(auto n : s.substr(1))
                    if(!isdigit(n))
                        return false;
                return true;
            }
            else
                return false;            
        }
        else
            return false;        
    }

    int BigInt::strToDQ(std::deque<int>& ls, const std::string& str)
    {
        long i = 1;
        long idx = str.size() - zero_shift * i;
        while(idx > -1)         
        {
            ls.push_back(std::stoi(str.substr(idx, zero_shift)));
            idx = str.size() - zero_shift * ++i;
        }
        if(idx + zero_shift)
            ls.push_back(std::stoi(str.substr(0, idx + zero_shift)));

        return 0;
    }

    void BigInt::DQToStr(const std::deque<int>& dq, std::string& str)
    {
        assert(dq.size() != 0);

        str = "";
        std::string smallStr;
        str = std::to_string(*dq.rbegin());

        for(auto itr = dq.rbegin() + 1; itr < dq.rend(); itr++)
        {
            smallStr = std::to_string(*itr);
            while (smallStr.length() < zero_shift)
            {
                smallStr = "0" + smallStr;
            }
            str += smallStr;           
        }
    }
    

    // additional functions:
    constexpr int calculateD()
    {
        int k = 1;
        for(int i = 0; i < zero_shift; i++)
            k *= 10;

        return k;    
    }

}