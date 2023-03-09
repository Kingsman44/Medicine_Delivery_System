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
};

struct orders
{
	char s[50];
};

char CURRENT_USERNAME[50] = { "" };

char CURRENT_FNAME[50] = { "" };

char CURRENT_LNAME[50] = { "" };

typedef struct details USER;
USER * users;
int totalusers = 0;
int *userp = &totalusers;

float **graph;
char **city_name;
int vertices = 0;
int *nodes = &vertices;

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
	while (!feof(fp))
	{
		fscanf(fp, "%s\t%s\t%s\t%s\n", str1, str2, str3, str4);
		//printf("%s\t%s\t%s\t%s\t%d\n", str1, str2, str3, str4, num);
		*users = *users + 1;
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
		fscanf(fp, "%s\t%s\t%s\t%s\n", db[i].username, db[i].password, db[i].fname, db[i].lname);
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
		printf("%s\n", cities[i]);
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
		printf("Username:%s\nPassword:%s\nFirstName:%s\nLastName:%s\n\n", db[i].username, db[i].password, db[i].fname, db[i].lname);
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

void create_user(USER *db, int *users, char filename[])
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

	FILE * fp;
	fp = fopen(filename, "a");
	fprintf(fp, "%s\t%s\t%s\t%s\n", db[*users].username, db[*users].password, db[*users].fname, db[*users].lname);
	fclose(fp);
	*users = *users + 1;
	check_signin(db, *users, db[*users].username, db[*users].password);
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
}

void menu(USER *db, int *users)
{
	int sign = is_signed();
	printf("======================\n");
	if (sign)
	{
		printf(" Welcome %s\n", CURRENT_FNAME);
		printf("======================\n");
		printf(" 1. Request Delivery\n");
		printf(" 2. Deliver Item\n");
		printf(" 3. Check orders\n");
		printf(" 4. Logout\n");
		printf(" 5. Exit\n");
		printf("======================\n");
		printf("Enter Choice: ");
		int ch;
		char usr[50], psw[50];
		scanf("%d", &ch);
		switch (ch)
		{
			case 1:
				//TO DO
				menu(db, users);
				break;
			case 2:
				// TO DO
				menu(db, users);
				break;
			case 3:
				// TO DO
				menu(db, users);
				break;
			case 4:
				logout();
				menu(db, users);
				break;
			case 5:
				exit(0);
				break;
			default:
				printf(" ! Invalid Choice\n");
				menu(db, users);
		}
	}
	else
	{
		printf(" Welcome User\n");
		printf("======================\n");
		printf(" 1. Log-in\n");
		printf(" 2. Create Account\n");
		printf(" 3. Exit\n");
		printf("======================\n");
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

				menu(db, users);
				break;
			case 2:
				create_user(db, users, "user.txt");
				print_users(db, *users);
				menu(db, users);
				break;
			case 3:
				exit(0);
				break;
			default:
				printf(" ! Invalid Choice\n");
				menu(db, users);
		}
	}
}

int main()
{
	get_nodes("cities.txt", nodes);
	get_users("user.txt", userp);
	city_name = set_cities("cities.txt", city_name, vertices);
	graph = create_graph(graph, vertices);
	add_graph_data(graph, "distance.txt");
	users = create_user_db(users, totalusers);
	fill_userdb(users, totalusers, "user.txt");
	print_users(users, totalusers);
    //print_graph(graph,vertices);
	menu(users, userp);
	//print_cities(city_name,vertices);
	//print_graph(graph,vertices);
	//printf("%d",vertices);
	return 0;
}
