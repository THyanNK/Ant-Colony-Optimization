#include<bits/stdc++.h>
using namespace std;
#define pb push_back
const int maxn=100, inf=0x3f3f3f3f;
int CityPos[30][2]=
{{87,7},{91,38},{83,46},{71,44},{64,60},{68,58},{83,69},{87,76},{74,78},{71,71},{58,69},{54,62},{51,67},{37,84},{41,94},{2,99},{7,64},{22,60},{25,62},{18,54},{4,50},{13,40},{18,40},{24,42},{25,38},{41,26},{45,21},{44,35},{58,35},{62,32}};
const int N=30;//城市数量
int ROU=0.6;//信息素挥发系数 
int ALPHA=1, BETA=5;//信息素重要程度,启发式因子重要程度
double Q=500.0;//蚁周模型信息素增加量

struct City{//城市 
	int x,y;
};
class Graph{
public:
	City city[N];
	double dis[N][N];//城市间距离
	double info[N][N];//信息素
	
	double dis_2D(int a,int b){//计算两座城市的距离 
		double x=city[a].x-city[b].x, y=city[a].y-city[b].y;
		return sqrt(x*x+y*y);
	}
	
	void Init(){
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++){
				info[i][j]=0.0;
                dis[i][j]=dis_2D(i,j);
            }
    }
	 
} G;

int rnd(int l,int r){//返回[l,r]内的随机整数
    return (int) 1.0*rand()/(RAND_MAX+1.0)*(r-l+1)+l;
}

double rnd(double l,double r){//返回(l,r)内的随机浮点数
    return l+1.0*rand()/(RAND_MAX+1.0)*(r-l);
}
 
struct Ant{
    vector<int> path;//保存蚂蚁走的路径
    double length;//路径总长度
    bool vis[N];//标记走过的城市
    int now, cnt;//当前城市, 已走城市数量
 
    void Init(){//初始化
        for(int i=0;i<N;i++) vis[i]=0;
        length=0.0;
        now=rnd(0,N-1);
        cnt=vis[now]=1;
        path.clear();
        path.pb(now);
    }
 
    int Choose(){//选择下一个城市
        int select_city=-1;
        double sum=0.0, prob[N];//保存每个城市被选中的概率
        for(int i=0;i<N;i++){
            if(!vis[i]){
                prob[i]=pow(G.info[now][i],ALPHA)*pow(1.0/G.dis[now][i],BETA);//核心公式计算概率 
                sum+=prob[i];
            }
            else prob[i]=0.0;
        }
        if(sum>0.0){//总的信息素大于0,进行轮盘选择
            double temp=rnd(0.0,sum);
            for(int i=0;i<N;i++)
                if(!vis[i]){
                    temp-=prob[i];
                    if(temp<0.0) {select_city=i; break;}
                }
        }
        else{//信息素太少就随便找个没走过的城市
        	int temp=rnd(1,N-cnt);//还有N-cnt个城市没走，每个概率均为1/(N-cnt) 
            for(int i=0;i<N;i++)
                if(!vis[i]){
                    temp--;
                    if(temp==0) {select_city=i; break;}
                }
		} 
        return select_city;
    }
 
    void Search(){//蚂蚁进行一次迭代
        Init();
        int nex;
        while(cnt<N){//蚂蚁移动N-1次 
        	nex=Choose();
	        vis[nex]=1;
	        length+=G.dis[now][nex];
	        cnt++;
	        now=nex;
			path.pb(now);
		}
		length+=G.dis[path[N-1]][path[0]];//回到原来的城市
    }
};
 
struct TSP{
	int ANT_NUM, TMAC;
    Ant ant_best;//最优路径蚂蚁
	vector <Ant> ants;//蚁群

	void Init(){
		TMAC=2000;//迭代次数 
		ANT_NUM=(int) N*1.5;//蚁群数量
		ant_best.length=inf;//初始化为最大值
		ants.resize(ANT_NUM);
	}
    
    void UpdateInfo(){//更新信息素
        double temp_info[N][N];
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++) temp_info[i][j]=0.0;
        int u,v;
        for(auto it:ants){ //遍历每一只蚂蚁
            for(int i=1;i<N;i++){//遍历path 
                u=it.path[i-1], v=it.path[i];
                temp_info[u][v]+=Q/it.length;//蚁周模型更新信息素 
                temp_info[v][u]=temp_info[u][v];
            }
            //更新最后城市和开始城市之间的信息素
            u=it.path[0];//此时v为最后一个城市path[N-1] 
            temp_info[u][v]+=Q/it.length;
            temp_info[v][u]=temp_info[u][v];
        }
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
                G.info[i][j]=G.info[i][j]*ROU+temp_info[i][j];//ROU为信息素挥发系数 
        
    }
    void Search(){
        for(int x=1;x<=TMAC;x++){//迭代TMAC次
        	int flag=0;
            for(auto &it:ants) it.Search();
            for(auto it:ants) if(ant_best.length>it.length) ant_best=it, flag=1;
            UpdateInfo();
            
            if(flag){
	            printf("\n第%d次迭代最优路径长度是%lf\n", x,ant_best.length);
	            printf("第%d次最短路径如下\n",x);
			    for(int i=0;i<N;i++){
			        printf("%d",ant_best.path[i]);
			        printf(i<N-1? "->":"\n");
			    }	
			}
		    /*printf("第%d次迭代后信息素\n",x);
	        for(int k=0;k<N;k++,cout<<endl)
	            for(int j=0;j<N;j++) printf("%.2f ",G.info[k][j]);
            printf("\n");*/
            
        }
    }
} tsp;

void io(){
//	cin>>N;
//	ANT_NUM=(int)N*1.5;
//	for(int i=0;i<N;i++) cin>>city[i].x>>city[i].y;
	for(int i=0;i<N;i++) G.city[i]=(City) {CityPos[i][0],CityPos[i][1]}; 
	tsp.Init(), G.Init();
} 

int main(){
    srand(time(NULL)*10);//产生随机种子
    io();
    tsp.Search();
    printf("\n最短路径如下\n");
    for(int i=0;i<N;i++){
        printf("%d",tsp.ant_best.path[i]);
        printf(i<N-1? "->":"\n");
    }
    printf("最短路径的长度为%lf\n",tsp.ant_best.length);
    return 0;
}
