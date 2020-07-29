#include "BigIntegers.h"

namespace aj{


    // constructors: 
    BigInt::BigInt()
    {
        bSign_ = plus;
        strNum_ = '0';
        vecNum_ = {0};
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
        strToVec(vecNum_, strNum_);           
    }


// ------------------------------------------------------------------------------
    // public methods:

    std::string BigInt::getNum()
    {
        return strNum_;
    }

    // for debugging:
    g_sign BigInt::getSign()
    {
        return bSign_;
    } 

    std::vector<int> BigInt::getVecNum()
    {
        return vecNum_;
    }

// ------------------------------------------------------------------------------
    // private methods: 

    BigInt BigInt::add(BigInt& n1, BigInt& n2)
    {
        return *this;
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

    int BigInt::strToVec(std::vector<int>& vec, const std::string& str)
    {
        long i = 1;
        long idx = str.size() - zero_shift * i;
        while(idx > -1)         
        {
            vec.push_back(std::stoi(str.substr(idx, zero_shift)));
            idx = str.size() - zero_shift * ++i;
        }
        if(idx + zero_shift)
            vec.push_back(std::stoi(str.substr(0, idx + zero_shift)));

        return 0;
    }

}