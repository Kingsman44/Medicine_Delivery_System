#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_STR_LENGHT 100
#define COST_FACTOR 3.5
#define MAX_COST_FACTOR 5
#define ALOCATE_FACTOR 10

struct details {
    char username[50];
    char password[50];
    char fname[50];
    char lname[50];
    int usertype;
};

struct orders {
    char s[50];
};

typedef struct details USER;
USER *users;
int totalusers=0;
int *userp=&totalusers;

float **graph;
char **city_name;
int vertices=0;
int *nodes=&vertices;


void get_nodes(char filename[],int* nodes) {
    FILE *fp;
    fp=fopen(filename,"r");
    if(fp==NULL) {
        printf("x %s not found\nx Aborting...",filename);
        exit(0);
    }
    char temp[100];
    while(!feof(fp)) {
        fscanf(fp,"%s",temp);
        *nodes=*nodes+1;
    }
    fclose(fp);
}

void get_users(char filename[],int* users) {
    FILE *fp;
    fp=fopen(filename,"r");
    if(fp==NULL) {
        printf("x %s not found\nx Aborting...",filename);
        exit(0);
    }
    char str1[100], str2[100], str3[100], str4[100];
    int num;
    while(!feof(fp)) {
        fscanf(fp,"%s\t%s\t%s\t%s\t%d\n", str1, str2, str3, str4, &num);
        //printf("%s\t%s\t%s\t%s\t%d\n", str1, str2, str3, str4, num);
        *users=*users+1;
    }
    fclose(fp);
}

char **set_cities(char filename[],char **cities,int nodes) {
    FILE *fp;
    fp=fopen(filename,"r");
    if(fp==NULL) {
        printf("x %s not found\nx Aborting...",filename);
        exit(0);
    }
    cities=(char **) malloc(nodes*sizeof(char *));
    for(int i=0;i<nodes;i++) {
        cities[i]=(char *) malloc(MAX_STR_LENGHT*sizeof(char));
        fscanf(fp,"%s",cities[i]);
    }
    fclose(fp);
    return cities;
}

USER* create_user_db(USER *db,int n) {
    db =(USER *) malloc(n*sizeof(USER));
    return db;
}

void fill_userdb(USER *db,int n,char filename[]) {
    FILE *fp;
    fp=fopen(filename,"r");
    if(fp==NULL) {
        printf("x %s not found\nx Aborting...",filename);
        exit(0);
    }
    for(int i=0;i<n;i++) {
        fscanf(fp,"%s\t%s\t%s\t%s\t%d\n", db[i].username,db[i].password,db[i].fname,db[i].lname, &db[i].usertype);
    }
}

float** create_graph(float **graph,int nodes) {
    graph =(float **)malloc(nodes*sizeof(float *));
    int i,j;
    for (i=0;i<nodes;i++) {
        graph[i]=(float *)malloc(nodes*sizeof(float));
    }
    for(i=0;i<nodes;i++) {
        for (j=0;j<nodes;j++) {
            graph[i][j]=0;
        }
    }
    return graph;
}

void print_graph(float **graph,int n) {
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            printf(" %.2f ",graph[i][j]);
        }
        printf("\n");
    }
}

void print_graph_edge(float **graph,char** city,int n) {
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            if(graph[i][j]!=0) {
                printf("%s -> %s = %.2f\n",city[i],city[j],graph[i][j]);
            }
        }
    }
}

void print_cities(char **cities,int n) {
    for(int i=0;i<n;i++) {
        printf("%s\n",cities[i]);
    }
}

void add_graph_data(float **graph,char filename[]) {
    FILE *fp;
    fp=fopen(filename,"r");
    if(fp==NULL) {
        printf("x %s not found\nx Aborting...",filename);
        exit(0);
    }
    int src,des;
    float value;
    while(!feof(fp)) {
        fscanf(fp,"%d %d %f",&src,&des,&value);
        graph[src-1][des-1]=value;
        graph[des-1][src-1]=value;
    }
    fclose(fp);
}

void print_users(USER* db,int n) {
    for(int i=0;i<n;i++) {
        printf("Username:%s\nPassword:%s\nFirstName:%s\nLastName:%s\n\n",db[i].username,db[i].password,db[i].fname,db[i].lname);
    }
}

int find_cost(float kms) {
    int cost=5;
    if(kms>5) {
        cost=5*MAX_COST_FACTOR + (kms-5)*COST_FACTOR;
    } else {
        cost=kms*MAX_COST_FACTOR;
    }
    return cost;
}

int main() {
    get_nodes("cities.txt",nodes);
    get_users("user.txt",userp);
    city_name=set_cities("cities.txt",city_name,vertices);
    graph=create_graph(graph,vertices);
    add_graph_data(graph,"distance.txt");
    users=create_user_db(users,totalusers);
    fill_userdb(users,totalusers,"user.txt");
    print_users(users,totalusers);
    //print_cities(city_name,vertices);
    //print_graph_edge(graph,city_name,vertices);
    //print_graph(graph,vertices);
    //printf("%d",vertices);
    return 0;
}



