#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <cmath>
#include <set>
#include <map>
#include <complex>
#include <cstdlib>
#include <climits>
#include <utility>
#include <sstream>

#define REP(i, n) for(int i=0; i<(int)n; i++)
#define ALL(c) (c).begin(),(c).end()

using namespace std;

const static int LIMIT = 10000;
const static int LANGE = 4;

#define LESS -1
#define EQUAL 0
#define GREATER 1
#define FRONT "%d"
#define FORMAT "%04d"

class BigInteger {
public:
  BigInteger(void){
    negative = false;
    value.resize(1);
    value[0] = 0;
  }
  BigInteger(vector<int> &in){
    negative = false;
    value = in;
  }
  BigInteger(char sign, vector<int> &in){
    if(sign == '+')
      negative = false;
    else if(sign == '-')
      negative = true;
    value = in;
  }
  BigInteger(vector<int> &in, bool sign){
    negative = sign;
    value = in;
  }
  BigInteger(string &in){
    negative = false;
    if(in[0] == '-'){
      negative = true;
      in.erase(in.begin());
    }    
    while (in.size()%LANGE != 0 )
      in.insert(in.begin(),1,'0');
    for(int i=in.size()-LANGE; i>=0; i-=LANGE)
      value.push_back(atoi(in.substr(i, LANGE).c_str()));
    regulation();
  }
  BigInteger(const char* c){
    int l = strlen(c);
    string in;
    int i;
    for(i=0; i<l; i++)
      in += c[i];
    negative = false;
    if(in[0] == '-'){
      negative = true;
      i++;
    }    
    while (in.size()%LANGE != 0 )
      in.insert(in.begin(),1,'0');
    for(int i=in.size()-LANGE; i>=0; i-=LANGE)
      value.push_back(atoi(in.substr(i, LANGE).c_str()));
    regulation();
  }
  BigInteger(char sign, string &in){
  }
  BigInteger(string &in, bool sign){
  }
  BigInteger operator + (BigInteger &in){
    if(sign() == in.sign())
      return add(*this, in, sign());
    else if(compare(in, true) == LESS)
      return sub(in, *this, in.sign());
    else
      return sub(*this, in, sign());
  }
  BigInteger operator * (BigInteger &in){
    bool s = (sign() == in.sign())? false : true;
    return mul(*this, in, s);
  }
  BigInteger operator - (BigInteger &in){
    if(sign() != in.sign())
      return add(*this, in, sign());
    else if(compare(in, true) == LESS){
      return sub(*this, in, sign());
    }
    else{
      return sub(in, *this, !in.sign());
    }
  }
  BigInteger operator / (BigInteger &in){
    bool s = (sign() == in.sign())? false : true;
    return div(*this, in, s);
  }
  bool operator == (BigInteger &in){
    if(compare(in, false) == EQUAL)
      return true;
    else
      return false;
  }
  bool operator < (BigInteger &in){
    if(compare(in, false) == LESS)
      return true;
    else
      return false;
  }
  bool operator > (BigInteger &in){
    if(compare(in, false) == GREATER)
      return true;
    else
      return false;
  }
  bool operator >= (BigInteger &in){
    return (*this > in) || (*this == in);
  }
  bool operator <= (BigInteger &in){
    return (*this < in) || (*this == in);
  }
  int &operator [] (int n) {
    return value[n];
  }
  string toString(void){
    char buffer[LANGE+1];
    string result;
    
    if (negative == true) result = "-";
    sprintf(buffer, FRONT, value.back()), result +=buffer;
    
    for(int i=value.size()-2; i>=0; i--)
      sprintf(buffer, FORMAT, value[i]), result += buffer;
    return result;
  }
  int size(void){
    return value.size();
  }
  void resize(int n){
    value.resize(n);
  }
  bool sign(void){
    return negative;
  }
  void test(void){
    if(negative)
      cout << '-';
    cout << value.back();
    for(int i=value.size()-1-1; i>=0; i--)
      printf("%04d",value[i]);
  }
private:
  bool negative;
  vector<int> value;

  void regulation(void) {
    while(value.back() == 0 && value.size() > 1)
      value.pop_back();
    if(value.size() == 1 && value[0] == 0)
      negative = false;
  }
  int compare(BigInteger &in, bool abs){
    if(sign() != in.sign() && !abs)
      return (sign())? GREATER : LESS;
    if(size() != in.size())
      return (size() > in.size())? GREATER : LESS;
    for(int i=in.size()-1; i>=0; i--){
      if(value[i] == in.value[i])continue;
      return (value[i] > in.value[i])? GREATER : LESS;
    }
    return EQUAL;
  }
  BigInteger add(BigInteger &a, BigInteger &b, bool sign){
    BigInteger result = BigInteger(a.value, sign);
    result.value.resize( max( a.size(), b.size() )+1, 0 );
    for(int i=0; i<result.size()-1; i++){
      if(i < b.size() )
	result[i] += b[i];
      if(result[i]/LIMIT != 0){
	result[i+1]++;
	result[i] %= LIMIT;
      }
    }
    result.regulation();
    return result;
  }
  BigInteger mul(BigInteger &a, BigInteger &b, bool sign){
    BigInteger result;
    result.value.resize(a.size() + b.size() + 1, 0);
    for(int i=0; i<a.size(); i++){
      for(int j=0; j<b.size(); j++){
	int carry = a[i] * b[j];
	for(int k=i+j; ; k++ ){
	  result[k] += carry;
	  if(result[k] < LANGE)
	    break;
	  else{
	    carry = result[k] / LIMIT;
	    result[k] %= LIMIT;
	  }
	}
      }
    }
    result.negative = sign;
    result.regulation();
    return result;
  }
  BigInteger sub(BigInteger &a, BigInteger &b, bool sign){
    BigInteger result = BigInteger(a.value, sign);
    result.value.resize(a.size(), 0 );
    int borrow = 0;
    //cout << ": " << a.toString() << endl;
    //cout << ": " << b.toString() << endl;
    for(int i=0; i<result.size(); i++){
      if(i < b.size())result[i] -= b[i] + borrow;
      else result[i] -= borrow;
      if(result[i] >= 0)borrow = 0;
      else borrow = 1,result[i] += LIMIT;
    }
    result.regulation();
    return result;
  }
  BigInteger div(BigInteger &a, BigInteger &b, bool sign){
    BigInteger result = BigInteger(a.value, a.sign());
    result.value.resize(a.size(), 0 );
    
    return result;
  }
};

const int N = 4000 + 1;

BigInteger num[N];

int main(void) 
{     
  num[0] = num[1] = BigInteger("1");
  REP(i, N-2)
    num[i + 2] = num[i] + num[i + 1];
  for(int n; ; ){
    cin >> n;
    if(n == 0)break;
    cout << num[n * 2 - 1].toString() << endl;
  }
  return 0;
}
