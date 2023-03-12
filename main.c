#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LENGHT 100
#define COST_FACTOR 3.5
#define MAX_COST_FACTOR 5
#define ALOCATE_FACTOR 5

struct details
{
	char username[50];
	char password[50];
	char fname[50];
	char lname[50];
	int wallet;
};

struct deli
{
	int src;
	int des;
	int norder;
	int *arr;
};

struct meditems
{
	char usrname[50];
	int id;
	int number;
	int locid;
};

char CURRENT_USERNAME[50] = { "" };

char CURRENT_FNAME[50] = { "" };

char CURRENT_LNAME[50] = { "" };

int CURRENT_INDEX = -1;

int CURRENT_WALLET = 0;

typedef struct deli DEL;
DEL delivery;

typedef struct meditems MED;

MED * M;
int tmorder = 0;
int *torder = &tmorder;

typedef struct details USER;
USER * users;
int totalusers = 0;
int *userp = &totalusers;

float **graph;
float **floyd;
char **city_name;
int vertices = 0;
int *nodes = &vertices;

int *dj_path;
int tpath = 0;

char **medicines;
int tmed = 0;
int *tmedi = &tmed;

int *order_freq;

void get_nodes(char filename[], int *nodes)
{
	FILE * fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("x %s not found\nx Aborting...", filename);
		exit(0);
	}

	char temp[100];
	while (!feof(fp))
	{
		fscanf(fp, "%s", temp);
		*nodes = *nodes + 1;
	}

	fclose(fp);
}

void get_users(char filename[], int *users)
{
	FILE * fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("x %s not found\nx Aborting...", filename);
		exit(0);
	}

	char str1[100], str2[100], str3[100], str4[100];
	int num;
	while (!feof(fp))
	{
		fscanf(fp, "%s\t%s\t%s\t%s\t%d\n", str1, str2, str3, str4, &num);
		*users = *users + 1;
	}

	fclose(fp);
}

void get_ord(char filename[], int *ord)
{
	FILE * fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("x %s not found\nx Aborting...", filename);
		exit(0);
	}

	char str1[100];
	int num, num1, num2;
	while (!feof(fp))
	{
		fscanf(fp, "%s %d %d %d\n", str1, &num, &num1, &num2);
		*ord = *ord + 1;
	}

	fclose(fp);
}

MED* create_ord(MED *M, int n)
{
	M = (MED*) malloc((n + ALOCATE_FACTOR) *sizeof(MED));
	return M;
}

MED* fill_ord(char filename[], MED *M, int n)
{
	FILE * fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("x %s not found\nx Aborting...", filename);
		exit(0);
	}

	for (int i = 0; i < n; i++)
	{
		fscanf(fp, "%s %d %d %d\n", M[i].usrname, &M[i].id, &M[i].number, &M[i].locid);
		printf("%s\t%d\t%d\t%d\n\n", M[i].usrname, M[i].id, M[i].number, M[i].locid);
	}

	fclose(fp);
	return M;
}

void overwrite_ord(char filename[], MED *M, int n)
{
	FILE * fp;
	fp = fopen(filename, "w");
	if (fp == NULL)
	{
		printf("x %s not found\nx Aborting...", filename);
		exit(0);
	}

	for (int i = 0; i < n; i++)
	{
		fprintf(fp, "%s\t%d\t%d\t%d\n", M[i].usrname, M[i].id, M[i].number, M[i].locid);
	}

	fclose(fp);
}

void print_ord(MED *M, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%s\t%d\t%d\t%d\n\n", M[i].usrname, M[i].id, M[i].number, M[i].locid);
	}
}

void overwrite_users(char filename[], USER *db, int n)
{
	FILE * fp;
	fp = fopen(filename, "w");
	if (fp == NULL)
	{
		printf("x %s not found\nx Aborting...", filename);
		exit(0);
	}

	for (int i = 0; i < n; i++)
	{
		fscanf(fp, "%s\t%s\t%s\t%s\t%d\n", db[i].username, db[i].password, db[i].fname, db[i].lname, db[i].wallet);
	}

	fclose(fp);
}

char **set_cities(char filename[], char **cities, int nodes)
{
	FILE * fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("x %s not found\nx Aborting...", filename);
		exit(0);
	}

	cities = (char **) malloc(nodes* sizeof(char*));
	for (int i = 0; i < nodes; i++)
	{
		cities[i] = (char*) malloc(MAX_STR_LENGHT* sizeof(char));
		fscanf(fp, "%s", cities[i]);
	}

	fclose(fp);
	return cities;
}

USER* create_user_db(USER *db, int n)
{
	db = (USER*) malloc(n* sizeof(USER));
	return db;
}

void fill_userdb(USER *db, int n, char filename[])
{
	FILE * fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("x %s not found\nx Aborting...", filename);
		exit(0);
	}

	for (int i = 0; i < n; i++)
	{
		fscanf(fp, "%s\t%s\t%s\t%s\t%d\n", db[i].username, db[i].password, db[i].fname, db[i].lname, &db[i].wallet);
	}
}

float **create_graph(float **graph, int nodes)
{
	graph = (float **) malloc(nodes* sizeof(float*));
	int i, j;
	for (i = 0; i < nodes; i++)
	{
		graph[i] = (float*) malloc(nodes* sizeof(float));
	}

	for (i = 0; i < nodes; i++)
	{
		for (j = 0; j < nodes; j++)
		{
			graph[i][j] = 0;
		}
	}

	return graph;
}

void print_graph(float **graph, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf(" %.2f ", graph[i][j]);
		}

		printf("\n");
	}
}

void print_graph_edge(float **graph, char **city, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (graph[i][j] != 0)
			{
				printf("%s -> %s = %.2f\n", city[i], city[j], graph[i][j]);
			}
		}
	}
}

void print_cities(char **cities, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d)\t%s\n", i + 1, cities[i]);
	}
}

void add_graph_data(float **graph, char filename[])
{
	FILE * fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("x %s not found\nx Aborting...", filename);
		exit(0);
	}

	int src, des;
	float value;
	while (!feof(fp))
	{
		fscanf(fp, "%d %d %f", &src, &des, &value);
		graph[src - 1][des - 1] = value;
		graph[des - 1][src - 1] = value;
	}

	fclose(fp);
}

void print_users(USER *db, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("Username:%s\nPassword:%s\nFirstName:%s\nLastName:%s\nWallet Balance:%d\n\n", db[i].username, db[i].password, db[i].fname, db[i].lname, db[i].wallet);
	}
}

int find_cost(float kms)
{
	int cost = 5;
	if (kms > 5)
	{
		cost = 5 *MAX_COST_FACTOR + (kms - 5) *COST_FACTOR;
	}
	else
	{
		cost = kms * MAX_COST_FACTOR;
	}

	return cost;
}

int check_signin(USER *db, int n, char username[], char password[])
{
	for (int i = 0; i < n; i++)
	{
		if (strcasecmp(username, db[i].username) == 0 && strcasecmp(username, db[i].username) == 0)
		{
			strcpy(CURRENT_USERNAME, username);
			strcpy(CURRENT_FNAME, db[i].fname);
			strcpy(CURRENT_LNAME, db[i].lname);
			CURRENT_INDEX = i;
			CURRENT_WALLET = db[i].wallet;
			return 1;
		}
	}

	return 0;
}

int userpresent(USER *db, int n, char usr[])
{
	for (int i = 0; i < n; i++)
	{
		//printf("%s - %s\n",usr,db[i].username);
		if (strcasecmp(usr, db[i].username) == 0)
		{
			return 1;
		}
	}

	return 0;
}

int isempty(char str[])
{
	if (strcasecmp(str, "") == 0)
		return 1;
	return 0;
}

int isvalid_pass(char str[])
{
	if (strlen(str) < 5)
		return 0;
	int v = 0;
	for (int i = 0; i < strlen(str); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			v = 1;
		}
	}

	if (v == 1)
	{
		return 1;
	}

	return 0;
}

USER* create_user(USER *db, int *users, char filename[])
{
	db = (USER*) realloc(db, (*users + ALOCATE_FACTOR) *sizeof(USER));
	char usr[50];
	char pwd[50];
	char fn[50], ln[50];
	printf("Enter Username: ");
	scanf("%s", db[*users].username);
	while (userpresent(db, *users, db[*users].username))
	{
		printf(" ! Username already used, Please enter different one.\n");
		printf("Enter Username: ");
		scanf("%s", db[*users].username);
	}

	printf("Password conditions:\n");
	printf("length must be greater 5\n");
	printf("Contains atleast one Capital Letter\n");
	printf("Enter Password: ");
	scanf("%s", db[*users].password);
	while (!isvalid_pass(db[*users].password))
	{
		printf(" ! Password not statisfy the condition\n");
		printf("Enter Username: ");
		scanf("%s", db[*users].password);
	}

	printf("Enter Name (First Name) (Last Name):");
	scanf("%s%s", db[*users].fname, db[*users].lname);
	while (isempty(db[*users].fname) || isempty(db[*users].lname))
	{
		printf("! First or Last Name cannot be empty\n");
		printf("Enter Name (First Name) (Last Name):");
		scanf("%s%s", db[*users].fname, db[*users].lname);
	}

	db[*users].wallet = 100;
	FILE * fp;
	fp = fopen(filename, "a");
	fprintf(fp, "%s\t%s\t%s\t%s\t%d\n", db[*users].username, db[*users].password, db[*users].fname, db[*users].lname, db[*users].wallet);
	fclose(fp);
	*users = *users + 1;
	check_signin(db, *users, db[*users].username, db[*users].password);
	return db;
}

int is_signed()
{
	if (strcasecmp("", CURRENT_USERNAME) == 0)
		return 0;
	return 1;
}

void logout()
{
	strcpy(CURRENT_USERNAME, "");
	strcpy(CURRENT_FNAME, "");
	strcpy(CURRENT_LNAME, "");
	CURRENT_INDEX = -1;
	CURRENT_WALLET = 0;
}

void print_path_dj(int path[], int des, int src, char **city)
{
	if (des == src)
	{
		dj_path[tpath] = des;
		tpath++;
		//printf(" %s",city[des]);
		return;
	}
	else
	{
		dj_path[tpath] = des;
		tpath++;
		//printf("%s<-",city[des]);
		print_path_dj(path, path[des], src, city);
	}
}

int min_distance(float *dis, int *temp, int v)
{
	int i, min = 10000, index;
	for (i = 0; i < v; i++)
	{
		if (temp[i] == 0 && dis[i] <= min)
		{
			min = dis[i];
			index = i;
		}
	}

	return index;
}

int dj(float **a, int v, int src, int des, char **city)
{
	float dis[v];
	int flag = 0, i;
	int path[v];
	int temp[v];
	for (i = 0; i < v; i++)

	{
		dis[i] = 10000;
		temp[i] = 0;
		path[i] = src;
	}

	dis[src] = 0;
	for (i = 0; i < v - 1; i++)
	{
		int u = min_distance(dis, temp, v);
		temp[u] = 1;
		int j;
		for (j = 0; j < v; j++)
		{
			if (temp[j] == 0 && a[u][j] && dis[u] != INT_MAX && dis[u] + a[u][j] <= dis[j])
			{
				dis[j] = dis[u] + a[u][j];
				path[j] = u;
			}
		}
	}

	//printf("\n");
	tpath = 0;
	print_path_dj(path, des, src, city);
	return dis[des];
}

void flloyd_warshall(float **graph, int n)
{
	for (int k = 0; k < n; k++)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				int via_prev_path = graph[i][k] + graph[k][j];
				if (via_prev_path < graph[i][j])
				{
					graph[i][j] = via_prev_path;
				}
			}
		}
	}
}

void printf_city_opt(char **city_name, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d\t%s", i + 1, city_name);
	}
}

int get_option(char **city_name, int n, int s)
{
	if (s == 1)
	{
		printf_city_opt(city_name, n);
	}

	int ch;
	printf("Enter Choice: ");
	scanf("%d", &ch);
	return ch;
}

int get_index(int *arr, int n, int find)
{
	for (int i = 0; i < n; i++)
	{
		//printf("%d %d\n",arr[i],find);
		if (arr[i] == find)
			return i;
	}

	//printf("\n");
	return -1;
}

int min_distance_path(float *dis, DEL dev, int *temp)
{
	//printf("Min dest\n\n");
	int i, min = 10000, index;
	for (i = 0; i < dev.norder; i++)
	{
		if (temp[i] == 0 && dis[dev.arr[i]] <= min && dis[dev.arr[i]] > 0)
		{
			min = dis[dev.arr[i]];
			index = dev.arr[i];
		}
	}

	//printf("Fin\n\n");
	return index;
}

void print_dj(char **city, int n)
{
	for (int i = tpath - 1; i > n; i--)
	{
		printf("%s --> ", city[dj_path[i]]);
	}

	printf("%s", city[dj_path[n]]);
}

int travel_path(float **graph, float **floyd, DEL dev, int n, char **city, int show)
{
	int cost = 0;
	int cost1;
	int traveled[dev.norder];
	for (int i = 0; i < dev.norder; i++)
	{
		traveled[i] = 0;
	}

	int v = dev.src;
	if (show) printf("Path: Without Delivery:\n\n");
	cost1 = dj(graph, n, dev.src, dev.des, city);
	if (show) print_dj(city, 0);
	if (show) printf("\nPath: after delivery:\n");
	int u = min_distance_path(floyd[v], dev, traveled);
	int gindex = get_index(dev.arr, dev.norder, u);
	traveled[gindex] = 1;
	//printf("Src: %s\t\tDest %s\n",city_name[v],city_name[u]);
	for (int i = 0; i < dev.norder - 1; i++)
	{
		cost += dj(graph, n, v, u, city);
		if (show)
		{
			print_dj(city, 1);
			printf(" --> ");
		}

		v = u;
		u = min_distance_path(floyd[v], dev, traveled);
		gindex = get_index(dev.arr, dev.norder, u);
		//printf("\n\nIndex: %d\n\n",gindex);
		traveled[gindex] = 1;
		//printf("\nSrc: %s\t\tDest %s\n",city_name[v],city_name[u]);
	}

	cost += dj(graph, n, v, u, city);
	if (show) print_dj(city, 0);
	//printf("\nSrc: %s\t\tDest %s\n",city_name[u],city_name[dev.des]);
	//cost+=dj(graph,n,u,dev.des,city);
	if (show) printf("\nCOST Without Delivery: %d\nCOST With Delivery: %d\n\n", cost1, cost);
	if (dev.des == u)
	{
		return 1;
	}

	return 0;
}

void add_to_file(char filename[], int n, int q)
{
	FILE * fp;
	fp = fopen(filename, "a");
	if (fp == NULL)
	{
		printf("x %s not found\nx Aborting...", filename);
		exit(0);
	}

	for (int i = 0; i < n; i++)
	{
		fprintf(fp, "%s %d %d %d\n", CURRENT_USERNAME, M[q + i].id, M[q + i].number, M[q + i].locid);
	}

	fclose(fp);
}

void request_delivery(char **city, int tcity, char **medicines, int tmed)
{
	int i;
	int med_id;
	int quantity;
	int loc;
	char choice = 'y';
	char med;
	printf("|=============|=========================|\n");
	printf("|Medicine ID. | Medicine Name           |\n");
	printf("|=============|=========================|\n");
	for (i = 0; i < tmed; i++)
	{
		printf(" %d\t\t", i + 1);
		printf("%s\n", medicines[i]);
	}

	printf("=============|==========================|\n");

	int j = 0;
	int old = *torder;
	while (choice == 'y' || choice == 'Y')
	{
		printf("Enter Medicine Id: ");
		scanf("%d", &med_id);
		printf("You want to order %s ?(y/n)", medicines[med_id - 1]);
		scanf("%*c%c", &med);
		if (med == 'n')
		{
			continue;
		}

		printf("Number of Quantity: ");
		scanf("%d", &quantity);
		M[*torder].id = med_id;
		M[*torder].number = quantity;
		strcpy(M[*torder].usrname, CURRENT_USERNAME);
		*torder = *torder + 1;
		M = (MED*) realloc(M, (ALOCATE_FACTOR + *torder) *sizeof(MED));
		j++;
		printf("Do you want to order more items?(y/n): ");
		scanf("%*c%c", &choice);
	}

	printf("Enter location to be delivered: ");
	print_cities(city, tcity);
	printf("\nEnter location: ");
	scanf("%d", &loc);
	for (int i = 0; i < j; i++)
	{
		M[old + i].locid = loc;
	}

	print_ord(M, *torder);
	add_to_file("delivery_item.txt", j, old);
}

void deliver_item(float **graph, float **floyd, char **city, int n)
{
	int src, des;
	delivery.norder = 0;
	printf("\n========================\n");
	print_cities(city, n);
	printf("\n========================\n");
	printf("Enter Source: ");
	scanf("%d", &src);
	delivery.src = src - 1;
	printf("Enter Destination: ");
	scanf("%d", &des);
	delivery.des = des - 1;
	delivery.arr[delivery.norder] = des - 1;
	delivery.norder++;
	dj(graph, n, delivery.src, delivery.des, city);
	if (tpath > 2)
	{
		printf("Recommended Cities to Deliver\n");
		for (int i = 1; i < tpath - 1; i++)
		{
			printf("%d)\t%s\n", dj_path[i] + 1, city[dj_path[i]]);
		}

		printf("\n\n");
	}

	printf("Write city id to 0 to stop\n\n");
	while (delivery.norder < n)
	{
		printf("Enter City id to deliver: ");
		scanf("%d", &src);
		if (src == 0) break;
		delivery.arr[delivery.norder] = src - 1;
		delivery.norder++;
		if (travel_path(graph, floyd, delivery, n, city, 0) == 0)
		{
			printf("\nDeliver to these location will make you travel more after reaching destination.\n");
			printf("Do you still want to deliver to %s\n", city[src - 1]);
			printf("   YES - 1\n   NO  - 0\n");
			printf("\nEnter Choice: ");
			scanf("%d", &des);
			if (des == 0)
			{
				delivery.norder--;
				continue;
			}
		}

		delivery.arr = (int*) realloc(delivery.arr, sizeof(ALOCATE_FACTOR + delivery.norder) *sizeof(int));
	}

	printf("Estimated calculations: \n");
	travel_path(graph, floyd, delivery, n, city, 1);
}

void check_intercity(float **graph, float **floyd, char **city, int n)
{
	delivery.norder = 0;
	int src, des;
	printf("\n========================\n");
	print_cities(city, n);
	printf("\n========================\n");
	printf("Enter Source: ");
	scanf("%d", &src);
	delivery.src = src - 1;
	printf("Enter Destination: ");
	scanf("%d", &des);
	delivery.des = des - 1;
	delivery.arr[delivery.norder] = des - 1;
	delivery.norder++;
	printf("Cities that may come in between %s to %s path\n\n", city[src - 1], city[des - 1]);
	for (int i = 0; i < n; i++)
	{
		if (i != src - 1 && i != des - 1)
		{
			delivery.arr[delivery.norder] = i;
			delivery.norder++;
			if (travel_path(graph, floyd, delivery, n, city, 0))
			{
				printf("%d)\t%s\n", i + 1, city[i]);
			}

			delivery.norder--;
		}
	}
}

int *update_order_freq(int *frq, int n, MED *M, int m)
{
	for (int i = 0; i < n; i++)
	{
		frq[i] = 0;
	}

	for (int i = 0; i < m; i++)
	{
		frq[M[i].locid]++;
	}

	return frq;
}

void no_of_orders(int *frq, int n, char **city)
{
	for (int i = 0; i < n; i++)
	{
		if (frq[i] > 0)
			printf("%d)\t%s\t%d\n", i + 1, city[i], frq[i]);
	}
}

void menu(USER *db, int *users, char **city, int n, char **medicines, int tmed, float **floyd)
{
	int sign = is_signed();
	printf("\n===============================================\n");
	if (sign)
	{
		printf(" Welcome %s\n", CURRENT_FNAME);
		printf(" Wallet Balance - %d\n", CURRENT_WALLET);
		printf("===============================================\n");
		printf(" 1. Request Medicines Delivery\n");
		printf(" 2. Deliver Medicines\n");
		printf(" 3. Check Recommended Cities to deliver\n");
		printf(" 4. Check Available Request in each location\n");
		printf(" 5. Logout\n");
		printf(" 6. Exit\n");
		printf("===============================================\n");
		printf("Enter Choice: ");
		int ch;
		char usr[50], psw[50];
		scanf("%d", &ch);
		switch (ch)
		{
			case 1:
				//TO DO
				request_delivery(city, n, medicines, tmed);
				menu(db, users, city, n, medicines, tmed, floyd);
				break;
			case 2:
				// TO DO
				deliver_item(graph, floyd, city, n);
				menu(db, users, city, n, medicines, tmed, floyd);
				break;
			case 3:
				// TO DO
				check_intercity(graph, floyd, city, n);
				menu(db, users, city, n, medicines, tmed, floyd);
				break;
			case 4:
				no_of_orders(order_freq, n, city);
				menu(db, users, city, n, medicines, tmed, floyd);
				break;
			case 5:
				logout();
				menu(db, users, city, n, medicines, tmed, floyd);
				break;
			case 6:
				exit(0);
				break;
			default:
				printf(" ! Invalid Choice\n");
				menu(db, users, city, n, medicines, tmed, floyd);
		}
	}
	else
	{
		printf(" Welcome User\n");
		printf("===============================================\n");
		printf(" 1. Log-in\n");
		printf(" 2. Create Account\n");
		printf(" 3. Exit\n");
		printf("===============================================\n");
		printf("Enter Choice: ");
		int ch;
		char usr[50], psw[50];
		scanf("%d", &ch);
		switch (ch)
		{
			case 1:
				printf("Enter Username:");
				scanf("%s", usr);
				printf("Enter Password:");
				scanf("%s", psw);
				if (check_signin(db, *users, usr, psw))
				{
					printf("- Logged-in successfully\n");
				}
				else
				{
					printf("! Invalid username or Password\n");
				}

				menu(db, users, city, n, medicines, tmed, floyd);
				break;
			case 2:
				db = create_user(db, users, "user.txt");
				print_users(db, *users);
				menu(db, users, city, n, medicines, tmed, floyd);
				break;
			case 3:
				exit(0);
				break;
			default:
				printf(" ! Invalid Choice\n");
				menu(db, users, city, n, medicines, tmed, floyd);
		}
	}
}

void cpy_graph(float **src, float **des, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			des[i][j] = src[i][j];
		}
	}
}

void get_medicines(char filename[], int *med)
{
	FILE * fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("x %s not found\nx Aborting...", filename);
		exit(0);
	}

	char temp[100];
	while (!feof(fp))
	{
		fscanf(fp, "%s", temp);
		*med = *med + 1;
	}

	fclose(fp);
}

int main()
{
	get_nodes("cities.txt", nodes);
	get_users("user.txt", userp);
	get_medicines("medicines.txt", tmedi);
	get_ord("delivery_item.txt", torder);
	city_name = set_cities("cities.txt", city_name, vertices);
	medicines = set_cities("medicines.txt", medicines, tmed);
	M = create_ord(M, tmorder);
	graph = create_graph(graph, vertices);
	floyd = create_graph(floyd, vertices);
	add_graph_data(graph, "distance.txt");
	cpy_graph(graph, floyd, vertices);
	flloyd_warshall(floyd, vertices);
	users = create_user_db(users, totalusers);
	M = fill_ord("delivery_item.txt", M, tmorder);
	print_ord(M, tmorder);
	fill_userdb(users, totalusers, "user.txt");
	print_users(users, totalusers);
	dj_path = (int*) malloc(vertices* sizeof(int));
	delivery.arr = (int*) malloc((ALOCATE_FACTOR) *sizeof(int));
	delivery.norder = 0;
	order_freq = (int*) malloc(vertices* sizeof(int));
	order_freq = update_order_freq(order_freq, vertices, M, tmorder);
	//no_of_orders(order_freq,vertices,city_name);
	//printf("\n%d\n",travel_path(graph,floyd,delivery,vertices,city_name,1));
	//print_graph_edge(floyd,city_name,vertices);
	//printf("KMS: %d",dj(graph,vertices,102,103,city_name));
	//print_graph(graph,vertices);
	menu(users, userp, city_name, vertices, medicines, tmed, floyd);
	//print_cities(city_name,vertices);
	//print_graph(graph,vertices);
	//printf("%d",vertices);
	return 0;
}
