#include<bits/stdc++.h>
using namespace std;
#define pb push_back
const int maxn=100, inf=0x3f3f3f3f;
int CityPos[30][2]=
{{87,7},{91,38},{83,46},{71,44},{64,60},{68,58},{83,69},{87,76},{74,78},{71,71},{58,69},{54,62},{51,67},{37,84},{41,94},{2,99},{7,64},{22,60},{25,62},{18,54},{4,50},{13,40},{18,40},{24,42},{25,38},{41,26},{45,21},{44,35},{58,35},{62,32}};
const int N=30;//��������
int ROU=0.6;//��Ϣ�ػӷ�ϵ�� 
int ALPHA=1, BETA=5;//��Ϣ����Ҫ�̶�,����ʽ������Ҫ�̶�
double Q=500.0;//����ģ����Ϣ��������

struct City{//���� 
	int x,y;
};
class Graph{
public:
	City city[N];
	double dis[N][N];//���м����
	double info[N][N];//��Ϣ��
	
	double dis_2D(int a,int b){//�����������еľ��� 
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

int rnd(int l,int r){//����[l,r]�ڵ��������
    return (int) 1.0*rand()/(RAND_MAX+1.0)*(r-l+1)+l;
}

double rnd(double l,double r){//����(l,r)�ڵ����������
    return l+1.0*rand()/(RAND_MAX+1.0)*(r-l);
}
 
struct Ant{
    vector<int> path;//���������ߵ�·��
    double length;//·���ܳ���
    bool vis[N];//����߹��ĳ���
    int now, cnt;//��ǰ����, ���߳�������
 
    void Init(){//��ʼ��
        for(int i=0;i<N;i++) vis[i]=0;
        length=0.0;
        now=rnd(0,N-1);
        cnt=vis[now]=1;
        path.clear();
        path.pb(now);
    }
 
    int Choose(){//ѡ����һ������
        int select_city=-1;
        double sum=0.0, prob[N];//����ÿ�����б�ѡ�еĸ���
        for(int i=0;i<N;i++){
            if(!vis[i]){
                prob[i]=pow(G.info[now][i],ALPHA)*pow(1.0/G.dis[now][i],BETA);//���Ĺ�ʽ������� 
                sum+=prob[i];
            }
            else prob[i]=0.0;
        }
        if(sum>0.0){//�ܵ���Ϣ�ش���0,��������ѡ��
            double temp=rnd(0.0,sum);
            for(int i=0;i<N;i++)
                if(!vis[i]){
                    temp-=prob[i];
                    if(temp<0.0) {select_city=i; break;}
                }
        }
        else{//��Ϣ��̫�پ�����Ҹ�û�߹��ĳ���
        	int temp=rnd(1,N-cnt);//����N-cnt������û�ߣ�ÿ�����ʾ�Ϊ1/(N-cnt) 
            for(int i=0;i<N;i++)
                if(!vis[i]){
                    temp--;
                    if(temp==0) {select_city=i; break;}
                }
		} 
        return select_city;
    }
 
    void Search(){//���Ͻ���һ�ε���
        Init();
        int nex;
        while(cnt<N){//�����ƶ�N-1�� 
        	nex=Choose();
	        vis[nex]=1;
	        length+=G.dis[now][nex];
	        cnt++;
	        now=nex;
			path.pb(now);
		}
		length+=G.dis[path[N-1]][path[0]];//�ص�ԭ���ĳ���
    }
};
 
struct TSP{
	int ANT_NUM, TMAC;
    Ant ant_best;//����·������
	vector <Ant> ants;//��Ⱥ

	void Init(){
		TMAC=2000;//�������� 
		ANT_NUM=(int) N*1.5;//��Ⱥ����
		ant_best.length=inf;//��ʼ��Ϊ���ֵ
		ants.resize(ANT_NUM);
	}
    
    void UpdateInfo(){//������Ϣ��
        double temp_info[N][N];
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++) temp_info[i][j]=0.0;
        int u,v;
        for(auto it:ants){ //����ÿһֻ����
            for(int i=1;i<N;i++){//����path 
                u=it.path[i-1], v=it.path[i];
                temp_info[u][v]+=Q/it.length;//����ģ�͸�����Ϣ�� 
                temp_info[v][u]=temp_info[u][v];
            }
            //���������кͿ�ʼ����֮�����Ϣ��
            u=it.path[0];//��ʱvΪ���һ������path[N-1] 
            temp_info[u][v]+=Q/it.length;
            temp_info[v][u]=temp_info[u][v];
        }
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
                G.info[i][j]=G.info[i][j]*ROU+temp_info[i][j];//ROUΪ��Ϣ�ػӷ�ϵ�� 
        
    }
    void Search(){
        for(int x=1;x<=TMAC;x++){//����TMAC��
        	int flag=0;
            for(auto &it:ants) it.Search();
            for(auto it:ants) if(ant_best.length>it.length) ant_best=it, flag=1;
            UpdateInfo();
            
            if(flag){
	            printf("\n��%d�ε�������·��������%lf\n", x,ant_best.length);
	            printf("��%d�����·������\n",x);
			    for(int i=0;i<N;i++){
			        printf("%d",ant_best.path[i]);
			        printf(i<N-1? "->":"\n");
			    }	
			}
		    /*printf("��%d�ε�������Ϣ��\n",x);
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
    srand(time(NULL)*10);//�����������
    io();
    tsp.Search();
    printf("\n���·������\n");
    for(int i=0;i<N;i++){
        printf("%d",tsp.ant_best.path[i]);
        printf(i<N-1? "->":"\n");
    }
    printf("���·���ĳ���Ϊ%lf\n",tsp.ant_best.length);
    return 0;
}
