#include<cstdio>
#include<cstdlib>
#include<time.h>
#include<cmath>
#include<cstring>
#include<string>
#include<iostream>
#include<queue>
#include<map>
#include<set>
#include<stack>
#include<vector>
#include<algorithm>

//#include<bits/c++std.h>

#define Size 1000005
#define inf 2e9
#define INF 2e18
#define LL long long int
#define i64 __int64
#define ULL unsigned long long
#define Mod 1000000007
#define pi 4*atan(1)
#define eps 1e-8
#define lson now*2,l,l+(r-l)/2
#define rson now*2+1,l+(r-l)/2+1,r
using namespace std;
LL n,m,k;
///普通求逆元

LL quickMod(LL a, LL b, LL mod)//基于快速乘法取模的快速幂取模
///重点！！！！！如果能够改成int且满足题目要求一定要改，速度比LL快两倍
{
    if(b == 0)
        return 1%mod;
    LL c = a%mod;
    LL ans = 1;
    while(b)
    {
        if(b&1)
        {
//            ans = multi(ans,c,mod);//根据题目要求选择乘法方式
            ans = ans * c % mod;
        }
//        c = multi(c,c,mod);
        c = c * c % mod;
        b >>= 1;
    }
    return ans;
}

LL exGcd(LL a, LL b, LL &x, LL &y)///扩展欧几里得，返回a b的gcd，x，y分别为a b的系数
{
    if(a==0&&b==0) return -1;
    if(!b)
    {
        x=1,y=0;
        return a;
    }
    else
    {
       LL tmp = exGcd(b,a%b,y,x);
       y-=x*(a/b);
       return tmp;
    }
}

LL modInverPrime(LL a, LL mod)///乘法逆元 快速幂法，对于mod是质数时有效
{
    return quickMod(a,mod-2,mod);
}

LL modInver(LL a, LL mod)///乘法逆元 扩展欧几里得法，对于mod不是质数但与a 互质使有效
{
    LL x,y,g;
    LL gcd = exGcd(a,mod,x,y);
    if(gcd == 1)
        return (x%mod+mod)%mod;
    return -1;
}

///筛法求1~Size之间所有逆元
LL inv[Size];

LL inverInitPrime(LL mod)///线性筛逆元，在同时筛素数的基础上筛出逆元
{
    memset(prime,0,sizeof(prime));
    inv[1] = 1 % mod;
    for(LL i=2; i <= Size; ++i)
    {
        if(!prime[i])
        {
            prime[++prime[0]] = i;
            inv[i] = modInverPrime(i, mod);
        }
        for(LL j = 1; j <= prime[0] && prime[j] <= Size / i; ++j)
        {
            prime[i * prime[j]] = 1;
            inv[i * prime[j]] = inv[i] * inv[prime[j]] % mod;
            if(i % prime[j] == 0)
            {
                break;
            }
        }
    }
}

LL inverInit(LL mod)///已有素数表
{
    LL cnt = 0;///cnt表示当前数之前有多少个素数
    inv[1] = 1;
    for(LL i = 2; i <= Size; ++i)
    {
        if(primePos[i])///利用素数的位置判断是否为素数，同时更新cnt
        {
            cnt = primePos[i];
            inv[i] = modInverPrime(i, mod);
        }
        for(LL j = 1; j <= cnt && prime[j] <= Size / i; ++j)
        {
            inv[i * prime[j]] = inv[i] * inv[prime[j]] % mod;
            if(i % prime[j] == 0)
            {
                break;
            }
        }
    }
}

///最强公式法，线性筛1~mod中所有对mod的逆元，p为奇质数
LL inverInit_ex(LL mod)
{
    inv[1] = 1;
    for(LL i = 2; i <= mod; ++i)
    {
        inv[i] = (mod - mod / i) * inv[mod % i] % mod;
    }
}



