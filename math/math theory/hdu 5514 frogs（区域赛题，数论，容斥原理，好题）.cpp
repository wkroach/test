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

#define Size 100005
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
///注意此题中全部使用了long long 而没有使用int
using namespace std;
LL n,m,k;
LL ql,qr,pos;

vector<LL>factors;
LL vis[Size];
LL num[Size];

LL gcd(LL a,LL b)
{
    return b == 0?a:gcd(b,a%b);
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
    cin>>t;
    while(t--)
//    while(scanf("%d",&n)==1)
    {
        factors.clear();
        memset(vis,0,sizeof(vis));
        memset(num,0,sizeof(num));

        scanf("%lld%lld",&n,&m);
        for(LL i=1; i * i <= m; ++i)//要统计青蛙走过的石头，只需统计m的因子中哪些被gcd(x,m)整除，并求出这些因子的倍数的数的所有和即可，记得去掉m本身
        {
            if(!(m%i))
            {
                factors.push_back(i);
                if(i * i != m)
                {
                    factors.push_back((m/i));
                }
            }
        }
        sort(factors.begin(),factors.end());
        factors.pop_back();

        LL ans = 0;
        for(LL i=0;i<n;++i)//由于因子可能被重复统计，要利用容斥的思想，记录每个要被统计的次数以及实际
        {
            LL x;
            scanf("%lld",&x);
            LL tmp = gcd(x,m);
            for(LL j = 0;j<factors.size();++j)
            {
                if(factors[j] % tmp == 0)///重点！！！！任意个约数的公约数一定也是m的约数，有唯一分解定理可证
                {
                    vis[j] = 1;
                }
            }
        }
        for(LL j=0;j < factors.size();++j)///因此，这道题中，小于m的所有和m互素的数，要么不被走到，要么整个m全被走到，因此只需要考虑小于m且与m不互质的数
                                        ///与普通容斥不同，此题中，由于任意约数的lcm（交集）也为约数，因此m的约数集合本身就是约数的幂集（即容斥的集合，通常为2^n)
        {
                if(vis[j] == num[j])
                    continue;
                LL t = (m-1)/factors[j];
                ans += (t*(t+1) / 2 * factors[j] * (vis[j] - num[j]));///统计这个因子在小于m的范围内的倍数之和，并统计数组计算所要计算的次数进行计算
                LL tmp2 = vis[j] - num[j];
                for(LL k = j; k < factors.size(); ++k)
                {
                    if(factors[k] % factors[j] == 0)
                        num[k] += tmp2;//更新重复记录数组，表示当前此数又被计算了多少次
                }
        }
        printf("Case #%lld: %lld\n",++Case,ans);
    }
    return 0;
}
