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
int n,m,k;
int ql,qr,pos;
int dp[250005];
vector<int> arr;

void zero_one_pack(int n, int v, int c, int w)///01背包模板
{
    for(int j = v; j >= c; j--)
    {
        dp[j] = max(dp[j], dp[j - c] + w);
    }
}

int solve(int n, int sum)
{
    for(int i = 0;i<n;++i)
    {
        zero_one_pack(n,sum,arr[i],arr[i]);
    }
    return dp[sum];
}


int main()
{
    #ifndef ONLINE_JUDGE
//        freopen("input.txt","r",stdin);
//        freopen("output.txt","w",stdout);
    #endif // ONLINE_JUDGE
    int t;
    int x,y,z;
    int Case=0;
//    cin>>t;
//    while(t--)
    while(scanf("%d",&n)==1)
    {
        if(n < 0) return 0;
        memset(dp,0,sizeof(dp));
        arr.clear();
        int tmp;
        int cnt;
        int sum = 0;
        for(int i=0;i<n;++i)
        {
            scanf("%d",&tmp);
            arr.push_back(tmp);
            scanf("%d",&cnt);
            sum += cnt*tmp;
            for(int j = 1; j < cnt; ++j)
            {
                arr.push_back(tmp);
            }
        }
        int ans = solve(arr.size(), sum/2);///由于小的院系所获得的值一定小于等于总数的一半，那么在一半中01背包取最大值即可
        cout<<max(ans,sum-ans)<<" "<<min(ans,sum-ans)<<endl;
    }
    return 0;
}

