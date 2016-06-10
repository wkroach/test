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

#define Size 400005
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
LL n, m, k, mod;
LL ql, qr, pos;

///基本数学常数求解：
LL prime[Size + 10];///素数
LL primePos[Size + 10];///素数的位置
LL euler[Size + 10];///欧拉函数
LL u[Size + 10];///莫比乌斯函数

void mathInit()///线性筛求质数, 欧拉函数, 莫比乌斯函数 (逆元单独用函数来计算，故不放在此函数中)
{
    memset(prime,0,sizeof(prime));
    memset(euler,0,sizeof(euler));
    euler[1] = 1;
    memset(u,0,sizeof(u));
    u[1] = 1; ///u[1] = 1; u[n] = (-1)^k (n = p1p2p3...pk); 其余情况u[n] = 0
    for(LL i=2; i <= Size; ++i)
    {
        if(!prime[i])
        {
            prime[++prime[0]] = i;
            euler[i] = i - 1;///质数的欧拉函数为自己减一
            u[i] = -1;
            primePos[i] = prime[0];///记录素数的位置
        }
        for(LL j = 1; j <= prime[0] && prime[j] <= Size / i; ++j)
        {
            prime[i * prime[j]] = 1;
            if(i % prime[j] == 0)
            {
                euler[i * prime[j]] = euler[i] * prime[j];///如果i的分解中p^k中k>=1，即p|i，
                /// 那么由于euler[p^k] = (p-1)*p^(k-1),所以euler[i*p] = euler[i]*p;
                u[i * prime[j]] = 0; ///i * p中存在p的幂大于1
                break;
            }
            else
            {
                euler[i * prime[j]] = euler[i] * (prime[j] - 1);
                /// 反之，即i与p互质，euler[i * p] = euler[i]*euler[p] = euler[i]*(p-1)
                u[i * prime[j]] = -u[i]; /// 根据奇偶性，直接取反
            }
        }
    }
}



///常用函数：

LL gcd(LL a,LL b)//gcd
{
    return b == 0?a:gcd(b,a%b);
}

LL multi(LL a, LL b, LL mod)//快速乘法取模，防止两个过大的数相乘爆LL, 速度比原生乘法要慢
{
    LL ans = 0;
    a %= mod;
    while(b)//原理与快速幂类似，将一个数装换为二进制，按位相乘再求和
    {
        if(b&1)
        {
            ans = (ans + a) % mod;
        }
        b >>= 1;
        a = (a + a) % mod;
    }
    return ans;
}

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

///乘法逆元：

    ///直接求逆元：
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

    LL inverInit(LL mod)///已有素数表的情况下线性求逆元
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


///组合数取模：

    ///打表法：
//    LL facMod[Size + 10][Size];///Size < 1e4
//    LL facInver[Size + 10][Size];
//
//    void facModInit()///包含了阶乘对小于Size的素数取模与阶乘逆元，复杂度O(nlog(n)log(n))
//    {
//        for(LL i = 1; i <= prime[0]; ++i)
//        {
//            facMod[i][0] = facInver[i][0] = 1;
//            for(LL j = 1; j <= prime[i]; ++j)
//            {
//                facMod[i][j] = (facMod[i][j - 1] * j) % prime[i];
//                facInver[i][j] = modInverPrime(facMod[i][j], prime[i]);
//            }
//        }
//    }
    ///直接求解
    LL com(LL n, LL m, LL mod)///组合数取模，直接计算版本，对于n，m 较小时较有效 复杂度O(m)
    {
        if(m > n)
            return 0;
        LL ans = 1;
        for(LL i = 1; i <= m; ++i)
        {
            LL a = (n + i -m) % mod;
            LL b = i % mod;
            ans = ans * (a * modInverPrime(b, mod) % mod) % mod; ///mod是素数
    //        ans = ans * (a * modInver(b, mod) % mod) % mod; ///mod不是素数
        }
        return ans;
    }

//    LL com_init(LL n, LL m, LL mod)///利用已经打好的表计算版本, n,m要小于mod, 复杂度O(1)
//    {
//        if(m > n)
//            return 0;
//        LL t = primePos[mod];
//        return facMod[t][n] * (facInver[t][n-m] * facInver[t][m] % mod) % mod;
//    }

    ///lucas定理：
    LL lucas(LL n, LL m,LL mod)///基于lucas定理的组合数取模，对于n，m较大时有效，复杂度O(mod*log(m)), 当mod非常大时无效
    {
        if(m == 0)
            return 1;
        return com(n % mod, m % mod, mod) * lucas(n / mod, m / mod, mod) % mod;
    }

//    LL lucas_init(LL n, LL m, LL mod)///利用已经打好的表计算lucas，复杂度O(log(m))
//    {
//        if(m == 0)
//            return 1;
//        return com_init(n % mod, m % mod, mod) * lucas_init(n / mod, m / mod, mod) % mod;
//    }

    ///质数分解法：
    LL facFactor(LL n, LL p)///求n的阶乘的中因子p的个数，利用计数的思想求，O(log(n))
    {
        LL ans = 0;
        while(n)
        {
            ans += n / p;///每次统计n的范围内p的倍数，即有p^i作为因子的数的个数
            n /= p;///每次缩小n的规模（等价于i++，统计p的更高次幂的倍数）
        }
        return ans;
    }

    LL com_nprime(LL n, LL m, LL mod)///对于mod不是素数时的组合数求模的解法O(n),对于n<1e8有效，因为要打n范围的素数表
    {
        LL ans = 1;
        for(LL i = 1; i <= prime[0] && prime[i] <= n; ++i)
        {
            LL x = facFactor(n, prime[i]);
            LL y = facFactor(n - m, prime[i]);
            LL z = facFactor(m, prime[i]);
            x -= (y + z);
            ans *= quickMod(prime[i], x, mod);
            ans %= mod;
        }
        return ans;
    }

///计算素因子：
    //直接计算：
    vector<LL> factors;
    void getFactors(LL n)///利用sqrt(n)范围内的素数表即可求出n的素因子
    {
        factors.clear();
        LL tmp = n;
        for(LL i = 1; i <= prime[0] && prime[i] <= n / prime[i]; ++i)
        {
            if(tmp % prime[i] == 0)
            {
                factors.push_back(prime[i]);
            }
            while(tmp % prime[i] == 0)///由于大于sqrt(n)后还可能有唯一的较大大素因子，因此前面的素因子要除干净
            {
                tmp /= prime[i];
            }
        }
        if(tmp != 1)///最后的结果若不为1，说明还有素因子，要保存
        {
            factors.push_back(tmp);
        }
    }

///连续和公式：
    /// 1^k + 2^k + 3^k +.....n^k 的和对mod求模公式及函数
    /// 推导见此题题解cxlove的博客

    LL sumPow2(LL n, LL mod)///计算1^2 + 2^2 + 3^2 +.......n^2 的和对mod求模
    ///公式：n*(n+1)*(2*n+1)/6;
    {
        LL ans = n;
        ans = ans * (n + 1) % mod;
        ans = ans * (2 * n + 1) % mod;
        return ans * modInverPrime(6, mod) % mod;
    }

    LL sumPow3(LL n, LL mod)///计算1^3 + 2^3 + 3^3 +.......n^3 的和对mod求模
    ///公式：n^2*(n+1)^2/4
    {
        LL ans = 1;
        ans = n * n % mod;
        ans = (ans * (n + 1) % mod) * (n + 1) % mod;
        return ans * modInverPrime(4, mod) % mod;
    }

    LL sumPow4(LL n, LL mod)///计算1^4 + 2^4 + 3^4 +.......n^4 的和对mod求模
    ///公式：(n*(n+1)*(2*n+1)*(3*n^2+3*n-1))/30
    {
        LL ans = 1;
        ans = n * (n + 1) % mod;
        ans = ans * (2 * n + 1) % mod;
        ans = ans * (3 * quickMod(n, 2, mod) + 3 * n % mod - 1) % mod;
        return ans * modInverPrime(30, mod) % mod;
    }

///题目代码：

LL solve(LL n)
{
    LL ans = 0;
    LL tmp3 = 0;
    for(LL i = 1; i < (1 << factors.size()); ++i)
    {
        LL cnt = 0;
        LL tmp2 = 1;
        for(LL j = 0; j < factors.size(); ++j)
        {
            LL tmp = 1 << j;
            if(tmp & i)
            {
                cnt++;
                tmp2 *= factors[j];///统计当前状态下素因子的乘积
            }
        }
        if(cnt & 1)
        {
            tmp3 = n / tmp2;///表示n的范围内有多少素数乘积的倍数，即要进行容斥的那些数
            ans += tmp3;
        }
        else
        {
            tmp3 = n / tmp2;
            ans -= tmp3;
        }
    }
    return n - ans;
}

LL solveBetween(LL a, LL b, LL n)///求解a,b(闭区间）之间与n互质的数的和
{
    LL ans1 = 0, ans2 = 0;
    LL tmp3 = 0;
    for(LL i = 1; i < (1 << factors.size()); ++i)
    {
        LL cnt = 0;
        LL tmp2 = 1;
        LL tmp3 = 0;
        for(LL j = 0; j < factors.size(); ++j)
        {
            LL tmp = 1 << j;
            if(tmp & i)
            {
                cnt++;
                tmp2 *= factors[j];///统计当前状态下素因子的乘积
            }
        }
        if(cnt & 1)
        {
            tmp3 = (a - 1) / tmp2;
            ans1 += tmp3 * (tmp3 + 1) / 2 * tmp2;
            tmp3 = b / tmp2;
            ans2 += tmp3 * (tmp3 + 1) / 2 * tmp2;
        }
        else
        {
            tmp3 = (a - 1) / tmp2;
            ans1 -= tmp3 * (tmp3 + 1) / 2 * tmp2;
            tmp3 = b / tmp2;
            ans2 -= tmp3 * (tmp3 + 1) / 2 * tmp2;
        }
    }
    ans1 = (a - 1) * a / 2 - ans1;
    ans2 = b * (b + 1) / 2 - ans2;
    return ans2 - ans1;
}

LL binarySolve(LL s, LL e, LL n, LL x)///二分答案，由于可能在一个区间内的素数个数均相同，要求出目标区间，再从后往前找到与n互质的数即答案
{
    LL pre = s;
    LL mid = 0;
    if(solve(e) < x)
    {
        return 0;
    }
    while(s < e)
    {
        mid = (s + e) / 2;
        LL tmp = solve(mid);
        if(tmp < x)
        {
            pre = s;
            s = mid + 1;
        }
        else if(tmp >= x)
        {
            e = mid;
        }
    }
    for(LL i = s; i >= pre; --i)
    {
        if(gcd(i, n) == 1)
        {
            return i;
        }
    }
    return 0;
}

///题目代码：

struct Rec///矩形类，基本
{
    LL x1, x2, y1, y2;
    Rec():x1(0), x2(0), y1(0), y2(0) {}

    LL area()///面积
    {
        if(x1 >= x2 || y1 >= y2)
        {
            return 0;
        }
        return (y2 - y1) * (x2 - x1);
    }

    Rec interRec(const Rec& b)const///返回两个矩形相交矩形
    {
        Rec ans;
        if(x2 <= b.x1 || y2 <= b.y1 || b.x2 <= x1 || b.y2 <= y1)
        {
            return ans;
        }
        ans.x1 = max(x1, b.x1);
        ans.y1 = max(y1, b.y1);
        ans.x2 = min(x2, b.x2);
        ans.y2 = min(y2, b.y2);
        return ans;
    }
};

LL arr[25];
Rec arr2[25];

LL dfsRec(LL& ans, LL cur, Rec inter, LL flag)
{
    Rec tmp = arr2[arr[cur]].interRec(inter);
    if(tmp.area() == 0)///注意，要求出多个矩形公共面积，如果某一公共面积为0，则不用再搜索
        return 0;
    if(flag & 1)
    {
        ans += tmp.area();
    }
    else
    {
        ans -= tmp.area();
    }
    for(LL i = cur + 1; i <= k; ++i)
    {
        dfsRec(ans, i, tmp, flag + 1);
    }
}

int main()
{
    #ifndef ONLINE_JUDGE
//        freopen("input.txt","r",stdin);
//        freopen("output.txt","w",stdout);
    #endif // ONLINE_JUDGE
    LL t;
    LL x,y,z;
    LL Case=0;
//    mathInit();
//    cin>>t;
//    while(t--)
    while(scanf("%I64d%I64d",&n, &m) == 2 && (m + n))
    {
        printf("Case %I64d:\n", ++Case);
        LL ans = 0;
//        scanf("%I64d%I64d",&n, &m);
        Rec s;
        s.x1 = s.y1 = 0;
        s.x2 = s.y2 = 1005;
        for(LL i = 1; i <= 20; ++i)
        {
            arr2[i] = Rec();
        }
        for(LL i = 1; i <= n; ++i)
        {
            scanf("%I64d%I64d%I64d%I64d",&arr2[i].x1, &arr2[i].y1, &arr2[i].x2, &arr2[i].y2);
        }
        for(LL i = 1; i <= m; ++i)///由于矩形数目较少，暴力深搜容斥，方便剪枝，位运算无法剪枝
        {
            memset(arr,0,sizeof(arr));
            scanf("%I64d",&k);
            for(LL j = 1; j <= k; ++j)
            {
                scanf("%I64d",&arr[j]);
            }
            LL ans = 0;
            for(LL j = 1; j <= k; ++j)
            {
                dfsRec(ans, j, s, 1);
            }
            printf("Query %I64d: %I64d\n", i, ans);
        }
        printf("\n");
//        printf("Case #%I64d: ", ++Case);
//        printf("%I64d\n",ans);

    }
    return 0;
}
