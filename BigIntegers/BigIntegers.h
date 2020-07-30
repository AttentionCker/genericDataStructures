#ifndef AJ_BIGINT_H
#define AJ_BIGINT_H

#include <string>
#include <vector>
#include <utility>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>

namespace aj {

    using ulong = unsigned long;

    enum g_sign {minus = 0, plus = 1};  //minus = false, plus = true

    class BigInt 
    {    
        private:
            g_sign bSign_;
            std::string strNum_;
            std::vector<int> vecNum_;

        public:

        // constructors:
            BigInt();
            explicit BigInt(std::string s);

        //methods:
            std::string getNum() const;

            // for debugging purpose
            g_sign getSign() const; 
            std::vector<int> getvecNum() const;
            ulong size() const;

        // operations:
            BigInt operator + (const BigInt& rOperand) const;
            BigInt operator * (const BigInt& rOperand) const;
            BigInt operator - (const BigInt& rOperand) const;
            bool operator > (const BigInt& rOperand) const;
            bool operator < (const BigInt& rOperand) const;
            bool operator >= (const BigInt& rOperand) const;
            bool operator <= (const BigInt& rOperand) const;
            bool operator == (const BigInt& rOperand) const; 
            bool operator != (const BigInt& rOperand) const;
            BigInt& operator ++ (); // prefix
            BigInt& operator -- (); // prefix
            BigInt operator ++ (int); // postfix
            BigInt operator -- (int); // postfix

        private:    
        
        // constructor:
            BigInt(std::vector<int>& vec, g_sign sign);
        // methods:
        void setSign(bool sign);

        // static members:
        private:
        
        //values
        static const int D;
        
        // methods:
        static BigInt add(const BigInt& n1, const BigInt& n2);
        static BigInt subtract(const BigInt& n1, const BigInt& n2);
        static BigInt multiply(const BigInt& n1, const BigInt& n2);
        static bool IsNum(const std::string&); 
        static int strToVec(std::vector<int>& vec, const std::string& str);
        static void vecToStr(const std::vector<int>& vec, std::string& str);
        static void removePrecedingZeros(std::vector<int>& vec);
        
        public:
        static BigInt mod(const BigInt& n);
    };

    // additional helping functions:
    constexpr int calculateD();

}

#endif
