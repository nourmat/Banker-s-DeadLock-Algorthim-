#include "fun.h"
#include <iostream>
#define endl "\n"
using namespace std;

int *rec ,num_rec;
int **maximum,**allocation,**need,num_proc;
int *available;

void requests()
{
	start:
	int ran = rand() % 100000;
	if (!(ran == 0))
	{
		int *temp_request = generate_request(num_rec,num_proc,rec,need,available);
		if (temp_request == 0){
			cout<<"-------------------------------------------------\n";
			cout<<"Error, process requested more than needed or available\n";
			cout<<"-------------------------------------------------\n";
		}
		else 
		{
			cout<<"-------------------------------------------------\n";
			cout<<"Process "<<temp_request[num_rec]<<" requested the following \n";
			print_vector(temp_request,num_rec,"Requested vector");
			grante_request(temp_request,allocation,available,need,num_rec);
			if (isSafe(num_proc,num_rec,available,allocation,need)){
				cout<<"Request granted --- Still in safe Mode\n\n";
				cout<<"-------------------------------------------------\n";
			}
			else 
			{
				remove_request(temp_request,allocation,available,need,num_rec);
				cout<<"Request denied --- Still in safe Mode\n\n";
				cout<<"-------------------------------------------------\n";
			}
		}
	}
	else
	{
		int *temp_request = free_request(num_rec,num_proc,need,available,allocation);
		//check is all zeros .... drop is they are
		bool zero_cheak = false;
		for (int i = 0; i < num_rec; i++)
			if (temp_request[i] > 0)
				zero_cheak = true;
		if (zero_cheak == false)
			goto start;
		cout<<"-------------------------------------------------\n";
		cout<<"Process "<<temp_request[num_rec]<<" requested to FREE the following \n";
		print_vector(temp_request,num_rec,"Requested vector");
		cout<<"Request granted --- Still in safe Mode\n\n";
		cout<<"-------------------------------------------------\n";
	}
}

int main()
{
	//Entering number of recourse type
	num_rec = enter_rec_types();
	rec = new int [num_rec];
	available = new int[num_rec];

	//Entering total available recourse of each recourse
	enter_available_total_recourses(rec,num_rec);

	//Entering number of Process in the system and setting up the 
	//2-D arrays (max need allocated)
	num_proc = enter_proc_number();
	maximum = new int*[num_proc];
	allocation = new int*[num_proc];
	need = new int*[num_proc];
	for (int i = 0; i < num_proc; i++){
		maximum[i] = new int[num_rec];
		allocation[i] = new int[num_rec];
		need[i] = new int[num_rec];
	}

	set_max_matrix(num_rec,num_proc,rec,maximum);
	print_mat(maximum,num_proc,num_rec,"MAX matrix");

	set_allocation_matrix(num_rec,num_proc,allocation);
	print_mat(allocation,num_proc,num_rec,"Allocated matrix");

	set_need_matrix(num_rec,num_proc,need,allocation,maximum);
	print_mat(need,num_proc,num_rec,"Need matrix");

	set_available(num_proc,num_rec,available,rec,allocation);
	print_vector(available,num_rec,"Available vector");
	
	int option;
	while (1)
	{
		cout<<"Select option\n"<<"1 for requesting \n2 for printing data \n3 for termination\nENTER : ";
		cin>>option;
		switch (option)
		{
			case 1:
				int number_of_requests;
				cout<<"Enter number of requests to do : ";
				cin>>number_of_requests;
				for (int i = 0; i < number_of_requests; i++)
					requests();
				break;
			case 2:
				print_mat(maximum,num_proc,num_rec,"MAX matrix");
				print_mat(allocation,num_proc,num_rec,"Allocated matrix");
				print_mat(need,num_proc,num_rec,"Need matrix");
				print_vector(available,num_rec,"Available vector");
				break;
			case 3:
				return 0;
		}
	}
	return 0;
}