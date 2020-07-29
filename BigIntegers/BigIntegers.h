#ifndef AJ_BIGINT_H
#define AJ_BIGINT_H

#include <string>
#include <stdint.h>
#include <vector>
#include <assert.h>
#include <ctype.h>

namespace aj {

    constexpr auto LOG_2 = 0.301; 
    constexpr long zero_shift = static_cast<double>((sizeof(int) / 2) * 8) * LOG_2;
    
    enum g_sign {minus = 0, plus = 1};  //minus = false, plus = true

    class BigInt 
    {    
        private:
            g_sign bSign_;
            std::string strNum_;
            std::vector<int> vecNum_;

        public:
            BigInt();
            explicit BigInt(std::string s);

        //methods:
            std::string getNum();

          // for debugging purpose
            g_sign getSign(); 
            std::vector<int> getVecNum();

        private:    
            BigInt add(BigInt& n1, BigInt& n2);

        // static members:
        private:
        static bool IsNum(const std::string&); 
        static int strToVec(std::vector<int>& vec, const std::string& str);
    };

}


#endif
