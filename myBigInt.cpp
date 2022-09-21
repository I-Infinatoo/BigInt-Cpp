#include <iostream>
#include <vector>
#include <string>

// for printing while debuging
#define logg std::cout 

const int baseSize = 9;     //
const int base = 1000000000;       //1000000000

class bigInt {
    private:
    
    std::vector<int> vec;
    int sign;

    public:

    //---------------- constructors -----------------//
    bigInt() : sign(1) {} 
    bigInt(const std::string &);
    
    // template<typename t>
    bigInt(long long &);
    bigInt(const bigInt &obj);


    //---------------- functions ---------------------//
    void readFromString(const std::string &);
    // template <typename t>
    void readFromNum(long long &);
    std::string to_string(std::vector<int> &);
    int reverse_num(int) const;
    bigInt abs(const bigInt &);
    
    //-------------- overloaded operators ---------------//

    // input stream
    friend std::istream& operator >> (std::istream &stream, bigInt &obj);
    // output stream
    friend std::ostream& operator << (std::ostream &stream, bigInt &obj);

    // void operator = (std::string &);
    // void operator = (long long &);

    // declaration of function as a const will not permit the function to made any changes 
    // in the variables of `this`
    bool operator < (const bigInt &) const; 
    bool operator > (const bigInt &) const;
    bool operator <= (const bigInt &) const;
    bool operator >= (const bigInt &) const;
    bool operator == (const bigInt &) const;
    bool operator != (const bigInt &) const;

    bigInt operator + (const bigInt &);
    bigInt operator - (const bigInt &);
    bigInt operator * (const bigInt &);
    bigInt operator / (const bigInt &);
    bigInt operator += (const bigInt &);
    bigInt operator -= (const bigInt &);
    bigInt operator *= (const bigInt &);
    bigInt operator /= (const bigInt &);


    //-------- function overloading ----------//
    // bigInt max(const bigInt &obj1, const bigInt &obj2) const;
    // bigInt min(const bigInt &obj1, const bigInt &obj2) const;


    //------------- logging -----------------//
    void print() {
        std::cout << "\nLogging on the screen...\n";     
        std::cout << "(" << this->sign << ")";
        for(int i = this->vec.size() -1; i >= 0; --i) {
            std::cout <<  this->vec[i] << " "; 
        }
        std::cout << "\nLogging completed!\n";
    }
};

//------------ operator ---------------//
// input stream
std::istream& operator >> (std::istream &stream, bigInt &obj) {
    std::string s;
    stream >> s;

    obj.readFromString(s);
    return stream;
}

// output stream
std::ostream& operator << (std::ostream &stream, bigInt &obj) {

    if(obj.sign == -1) {
        stream << '-';
    }

    // for the case when there is only zeroes or + , - signs in the input
    if(obj.vec.size() == 0) return stream << '0';
    
    for(std::vector<int>::reverse_iterator it = obj.vec.rbegin(); it != obj.vec.rend(); ++it) {
        stream << *it;       
    }

    return stream;
}


bigInt bigInt::operator + (const bigInt &obj) {
    
    bool isThisBig = abs(*this) > abs(obj);   //use abs()
    
    bigInt result;
    result = isThisBig ? *this : obj; //use abs() while checking 
    
    int k = 0;

    // if both have negative sign, then add them having negative sign
    if(this->sign == -1 && obj.sign == -1) {
        result.sign = -1;
    }

    // if obj is -ve
    else if(obj.sign == -1) {
        result = obj;
        result.sign = 1;

        result = *this - result;
        result.sign = isThisBig ? 1 : -1;
        return result;
    }     

    // if this is -ve
    else if(this->sign == -1) {
        // result = isThisBig ? *this : obj;
        bigInt temp = isThisBig ? obj : *this;

        result.sign = temp.sign = 1;
        result = result - temp;
        result.sign = isThisBig ? -1 : 1;

        return result;
    }

    int carry = 0;
    int n = this->vec.size(), i = 0;
    int m = obj.vec.size(), j = 0; 

    while( i < n && j < m) {

        int sum = this->vec[i] + obj.vec[j] + carry;
        result.vec[k] = sum % base;
        carry = sum/base;

        i++, j++, k++;
    }
    while( i < n ) {

        int sum = this->vec[i] + carry;
        result.vec[k] = sum % base;
        carry = sum/base;

        i++, k++;
    }
    while( j < m) {

        int sum = obj.vec[j] + carry;
        result.vec[k] = sum % base;
        carry = sum/base;

        j++, k++;
    }
    if(carry) {
        result.vec.push_back(carry);
    }

    return result;    
}


bigInt bigInt::operator += (const bigInt &obj) {
    bigInt result = *this + obj;
    
    this->sign = result.sign;
    this->vec = result.vec;

    return *this;   
}


// for - operator

// case1 : both are +ve, then big - small with big's sign
// case2 : if one is -ve and other is +ve : 
            // 1. this is -ve : it can be small or big
            // 2. obj is -ve : it can be small or big
// case3 : if both are -ve:
            // 1. this is big, sign of this
            // 2. obj is big, sign of obj
bigInt bigInt::operator - (const bigInt &obj) {

    bool isThisBig = abs(*this) > abs(obj);   //use abs()

    bigInt result;
    bigInt smallNum;
    bigInt result = isThisBig ? *this : obj;
    bigInt smallNum = isThisBig ? obj : *this;
    
    // if both have same  +ve  sign
    if(this->sign == 1 && obj.sign == 1) {
        result.sign = isThisBig ? 1 : -1;
    }


    // if obj is  -ve  
    else if (obj.sign == -1) {

        // eg: -2 - (-3)    || -3 - (-2)    || 2 - (-3)    || 3 - (-2)
        //  => -2 + (+3)    || -3 + (+2)    || 2 + (+3)    || 3 + (+2)

        result = obj;
        result.sign = 1;
        result = *this + result; 
        return result;
    }

    // if this is +ve    
    else if(this->sign == -1) {

        // eg: -2 - (3)         || -3 -(2)
        // =>  2 + 3  = 5       || 3 + 2 = 5
        // =>  (-)5             || (-)5

        result.sign = smallNum.sign =1;
        result = result + smallNum;

        result.sign = -1;
        return result;
    } 

    int i = 0;
    int n = result.vec.size();

    int j = 0;
    int m = smallNum.vec.size();

    int loan = 0;
    while(i<n && j<m) {
        int diff = result.vec[i] - smallNum.vec[j] - loan;

        if(diff < 0) {
            result.vec[i] = diff + base;
            loan = 1;
        } else  {
            result.vec[i] = diff;
            loan = 0;
        }
        i++, j++;
    }
    while(i<n) {
         int diff = result.vec[i] - loan;

        if(diff < 0) {
            result.vec[i] = diff + base;
            loan = 1;
        } else  {
            result.vec[i] = diff;
            loan = 0;
        }
        i++;
    }

    return result;
}  

bool bigInt::operator < (const bigInt &obj) const {
    
    // first check on the basis of sign
    if(this->sign != obj.sign) {
        return this->sign < obj.sign;
    }

    // if signs are same then number with greater size will be bigger
    if(this->vec.size() != obj.vec.size()) {
        return this->vec.size() < obj.vec.size();
    }

    for(int i = obj.vec.size()-1; i>=0; --i) {

        int objNum = obj.vec[i];
        int thisNum = this->vec[i];

        if(thisNum != objNum) {

            // if both the numbers are negative then the number in smaller mangnitude will be greater

            return obj.sign == -1 ? !(thisNum < objNum) : (thisNum < objNum);
        }
    }

    return false;
}


bool bigInt::operator > (const bigInt &obj) const {
    
    // if they are equal.. then no need to check further, it means a !< b
    return !(*this == obj) ? !(*this < obj) : false;
}


bool bigInt::operator == (const bigInt &obj) const {

    // if they have diff sign
    if(this->sign != obj.sign) {
        return false;
    }

    if(this->vec.size() != obj.vec.size()) {
        return false;
    }

    for(int i = obj.vec.size()-1; i>=0; --i) {
        if(this->vec[i] != obj.vec[i]) {
            return false;
        }
    }

    return true;
}

bool bigInt::operator != (const bigInt &obj) const {
    return !(*this == obj);
}


bool bigInt::operator <= (const bigInt &obj) const {
    return (*this < obj) || (*this == obj);
}

bool bigInt::operator >= (const bigInt &obj) const {
    return !(*this < obj) || (*this == obj);
}


// void bigInt::operator = (std::string &str) {
//     // readFromString(str);

//     logg << "before calling the constructor explicitly\n";

//     *this = str;    // 'this' is calling the constructor
// }

// void bigInt::operator = (long long &num) {
//     // readFromNum(num);
//     *this = num;    // 'this' is calling the constructor
// }

//----------- constructor ------------//

bigInt::bigInt(const std::string &str) {
    readFromString(str);
}

// template <typename t>
bigInt::bigInt(long long& num) {
    readFromNum(num);
}

bigInt::bigInt(const bigInt &obj) {
    this->vec = obj.vec;
    this->sign = obj.sign;
}

//-------------- functions -----------------//

bigInt bigInt::abs(const bigInt &obj) {
    bigInt result = obj;
    result.sign = 1;
    return result;
}

int bigInt::reverse_num(int num) const{
    int revNum = 0;
    // int tempNum = num;
    // while(tempNum) {
    while(num) {
        // revNum = revNum * 10 + (tempNum%10);
        revNum = revNum * 10 + (num%10);
        // tempNum/=10; 
        num/=10; 
    }
    return revNum;
}

std::string bigInt::to_string(std::vector<int> &vec) {  // need to change the logic
    std::string str;
    
    for(std::vector<int>::reverse_iterator it = vec.rbegin(); it != vec.rend(); ++it) {
        
        int tempNum = *it;

        while(tempNum) {
            str += (tempNum%10);
            tempNum /= 10;
        }
    }

    return str;
}

// template <typename t>
void bigInt::readFromNum(long long &num) {
    
    this->sign = 1;
    if(num < 0) { 
        this->sign = -1;
        num = -num;  
    }

    // int baseCount = baseSize;
    // int tempNum = 0;
    
    while(num){
       this->vec.emplace_back((int)num%base);
       num/=base;
    }
}

void bigInt::readFromString(const std::string &str) {
    
    int strSize = (int) str.size();

    this->sign = 1;
    this->vec.clear();
    int pos = 0;

    // to check for the continues signs in the string and for the prefix zeroes
    while(pos < strSize && (str[pos] == '-' || str[pos] == '+' || str[pos] == '0') ) {
        if(str[pos] == '-') 
            this->sign = -(this->sign);
        ++pos;
    }

    /*

    //consider the string "+2345689" and the baseSize is 2
    // +2 34 56 89 will be stored in the vec as:  98 65 43 2
    
    // for(int i = n-1; i>=pos; i-=baseSize) {
        
    //     int tempNum = 0;
                
    //     for(int j = std::max(pos, i-baseSize+1), tempI = i; tempI >= j; tempI--) {
    //         tempNum = tempNum * 10 + (str[tempI] - '0');
    //     }

    //     this->vec.emplace_back(tempNum); 
    // }

    */



    //  consider the string "+23456890" and the baseSize is 2
    // +23 45 68 90 will be stored in the vec as:  90 68 45 23

    /*

    
    // for(int i = n-1; i>=pos; i-=baseSize) {
    //     int tempNum = 0;

    //     for(int j = std::max(pos, i-baseSize+1); j<= i; j++) {
    //         tempNum = tempNum*10 + (str[i] -'0');
    //     }

    //     this->vec.emplace_back(tempNum);
    // }

    */

    // strSize -= pos;

    // logg << "pos: " << pos << "\tstrSize: " << strSize << "\n";

    while (strSize > pos) {
        
        if (strSize <= 0) break;
        // if ( this->sign == -1 && strSize <= 1) break;

        int length = 0;
        int tempNum = 0;
        int prefix = 1;

        for (int i = (strSize - 1); (i >= pos && i >= strSize - baseSize) ; --i) {
            
            // invalid character
            if (str[i] < '0' || str[i] > '9') break;

            tempNum += (str[i] - '0') * prefix;
            prefix *= 10;
            ++length;
        }

        // logg << tempNum << "\n"; 

        this->vec.emplace_back(tempNum);
        strSize -= length;
    }
}

// bigInt max(const bigInt &obj1, const bigInt &obj2) {
//     bigInt result;

//     return result;
// }


void fn() {
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);
    #endif
}

int main () {
    fn();
    
    // std::string num;
    // long long num;
    // std::cin >> num;

    // bigInt num2(num);
    bigInt num1;
    std::cin >> num1;
    std::cout << "num1: " << num1 << "\n";
    
    bigInt num2;
    std::cin >> num2;
    std::cout << "num2: " << num2 << "\n";

    // std::cin >> num;
    // bigInt num2 = num;

    // std::cout << "(num1 < num2): " << (num1 < num2) << "\n";
    // std::cout << "(num1 > num2): " << (num1 > num2) << "\n";
    // std::cout << "(num1 <= num2): " << (num1 <= num2) << "\n";
    // std::cout << "(num1 >= num2): " << (num1 >= num2) << "\n";
    // std::cout << "(num1 != num2): " << (num1 != num2) << "\n";
    // std::cout << "(num1 == num2): " << (num1 == num2) << "\n";

    bigInt num3 = (num1 + num2);
    std::cout << "(num1 + num2): " << num3 << "\n";

    // bigInt  a( (long long)1 );           ---> not working
    // num1+=num2;
    // std::cout << "num1: " << num1 << "\n"; 


    // bigInt num1 = -2345689;
    // std::cout << num1;


    // std::cout << "max(num1, num2): " << max(num1, num2) << "\n";

    return 0;
}