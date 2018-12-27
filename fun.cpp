#include "fun.h"
#include <iostream>

#define endl "\n"

using namespace std;

int enter_rec_types()
{
	int temp;
	cout<<"Enter Number of recourses Types available : ";
	cin>>temp;
	return temp;
}

int enter_proc_number()
{
	int temp;
	cout<<endl<<"Enter Number of Proccess in the System : ";
	cin>>temp;
	return temp;
}

void enter_available_total_recourses(int rec[] ,int num)
{
	cout<<endl<<"Enter Number of each available resource : ";
	for (int i = 0; i < num; i++) cin>>rec[i];
}

void print_mat(int **pointer,int y,int x,char str[])
{
	cout<<str<<"  --------------------------------------\n";
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
			cout<<pointer[i][j]<<" ";
		cout<<endl;
	}
	cout<<"--------------------------------------\n";
}

void print_vector(int vec[],int size,char str[])
{
	cout<<str<<" ----------------------------------------\n";
	for (int i = 0; i < size; i++)
		cout<<vec[i]<<" ";
	cout<<"\n----------------------------------------\n";

}

void set_max_matrix(int num_rec,int num_proc,int rec[],int **max)
{
	for (int pro = 0; pro < num_proc; pro++)
		for (int res = 0; res < num_rec; res++)
			max[pro][res] = rand() % (rec[res]+1);
}

void set_allocation_matrix (int num_rec,int num_proc,int **allocation)
{
	for (int res = 0; res < num_rec; res++)
		for (int pro = 0; pro < num_proc; pro++)
			allocation[pro][res] = 0;

}

void set_need_matrix(int num_rec,int num_proc,int **need,int **allocation,int **max)
{
	for (int pro = 0; pro < num_proc; pro++)
		for (int res = 0; res < num_rec; res++)
			need[pro][res] = max[pro][res] - allocation[pro][res];
}

void set_available(int num_proc,int num_rec,int available[],int rec[],int **allocation)
{
	for (int res = 0; res < num_rec;res++)
	{
		int sum = 0;
		for (int pro = 0 ; pro < num_proc;pro++)
			sum += allocation[pro][res];
		available[res] = rec[res] - sum;
	}
}

//Safty algorithm
bool need_is_less_than_work(int **need,int work[],int num_rec,int pro)
{
	for (int i = 0 ; i < num_rec;i++)
	{
		if (need[pro][i] > work[i])
			return false;
	}
	return true;
}

void update_work(int work[],int **allocation,int num_rec,int pro)
{
	for (int i = 0; i < num_rec; i++)
		work[i] += allocation[pro][i];
}

bool isSafe (int num_proc,int num_rec,int available[],int **allocation,int **need)
{
	int *work = new int[num_rec];
	bool *finish = new bool[num_proc];
	//work = available
	for (int i = 0; i < num_rec; i++)
		work[i] = available[i];
	//finish = false
	for (int i = 0; i < num_proc; i++)
		finish[i] = false;

	int last_modified = num_proc-1;
	int true_counter = 0;
	for (int pro = 0; pro < num_proc; pro++)
	{
		if (finish[pro] == false && need_is_less_than_work(need,work,num_rec,pro))
		{
			update_work(work,allocation,num_rec,pro);
			finish[pro] = true;
			last_modified = pro == 0? num_proc-1:pro - 1;
			true_counter++;
		}
		if (true_counter == num_proc)
			return true;
		if (pro == last_modified)
			return false;
		pro = pro == num_proc-1? -1:pro;
	}
	return false;
}

//generate request
bool valid_request(int generate_request[],int num_rec,int **need,int generated_request_processes,int available[])
{
	for (int i = 0; i < num_rec; i++)
		if (generate_request[i] > need[generated_request_processes][i] || generate_request[i] > available[i])
			return false;
	return true;
}

int* generate_request (int num_rec,int num_proc,int rec[],int **need,int available[])
{
	int *generated_request = new int[num_rec+1];
	int generated_request_processes = rand() % (num_proc);
	//carry data in last index slot
	generated_request[num_rec] = generated_request_processes;
	for (int i = 0; i < num_rec;i++)
		generated_request[i] = rand() % (rec[i]+1);
	if (valid_request(generated_request,num_rec,need,generated_request_processes,available))
		return generated_request;
	else
		return 0;
}
//----
void grante_request(int generate_request[], int **allocation,int available[],int **need,int num_rec)
{
	int pro = generate_request[num_rec];
	for (int i = 0;i < num_rec;i++)
	{
		available[i] -= generate_request[i];
		allocation[pro][i] += generate_request[i];
		need[pro][i] -= generate_request[i];
	}
}

void remove_request(int generate_request[], int **allocation,int available[],int **need,int num_rec)
{
	int pro = generate_request[num_rec];
	for (int i = 0;i < num_rec;i++)
	{
		available[i] += generate_request[i];
		allocation[pro][i] -= generate_request[i];
		need[pro][i] += generate_request[i];
	}
}

int* free_request(int num_rec,int num_proc,int **need,int available[],int **allocation)
{
	int *generated_request = new int[num_rec+1];
	int generated_request_processes = rand() % (num_proc);
	//carry data in last index slot
	generated_request[num_rec] = generated_request_processes;
	for (int i = 0;i < num_rec ;i++){
		generated_request[i] = rand() % (allocation[generated_request_processes][i]+1);
	}
	
	for (int i = 0;i < num_rec;i++)
	{
		available[i] += generated_request[i];
		allocation[generated_request_processes][i] -= generated_request[i];
		need[generated_request_processes][i] += generated_request[i];
	}
	return generated_request;
}