/*
*Kieran Boyle
*1265888
*Resources
*http://rosettacode.org/wiki/Banker%27s_algorithm
*/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#define MAX_STR_LENGTH 2000

/*
*This is the process struct that stores infromation about each process
*/
typedef struct process{
	int waiting;
	int *current_allocation;
	int *current_request;
	int *maximum_possible_request;
	
} process;
/*
*Function prototypes
*/
void print_tables(process * processi, int process_count, int resource_count, int * available_res, int* max_available_res);
int bankers(process * processi, int process_count, int resource_count, int * available_res, int* max_available_res, int process_no);
void handler();
/*
* This is the exception handler that breaks out of the function
*/
void handler(){
	printf("\nSimulation has been ended.\n");
	exit(1);
}

/*
*This funtion simply prints out all of the infromation a readable format
*/
void print_tables(process * processi, int process_count, int resource_count, int * available_res, int* max_available_res){

	int value = 0;
	int thang = 0;
	printf("Current snapshot:\n");
	printf("\n");
	printf("	Current	Allocation\n	__________________\n");
	for(value = 0; value < process_count;value++){
		printf("P%d	",value+1);
		for(thang = 0; thang < resource_count; thang++){
			printf("	");
			printf("%d ",processi[value].current_allocation[thang]);
		}
		thang = 0;
		printf("\n");
	}
	value = 0;
	printf("\n\n");
	printf("	Current Request\n	_______________\n");
	for(value = 0; value < process_count;value++){
		printf("P%d	",value+1);
		for(thang = 0; thang < resource_count; thang++){
			printf("	");
			printf("%d ",processi[value].current_request[thang]);
		}
		thang = 0;
		printf("\n");
	}
	value = 0;
	printf("\n\n");
	
	printf("	Currently Available Resources\n	_____________________________\n");
	printf("	");
	for(thang = 0; thang < resource_count; thang++){
		
		printf("	%d ",available_res[thang]);
	}
	thang = 0;
	printf("\n\n");

	printf("	Maximum Possible Request\n	________________________\n");
	for(value = 0; value < process_count;value++){
		printf("P%d	",value+1);
		for(thang = 0; thang < resource_count; thang++){
			printf("	");
			printf("%d ",processi[value].maximum_possible_request[thang]);
		}
		thang = 0;
		printf("\n");
	}
	value = 0;
 	printf("\n\n");
	printf("	Maximum Available Resources\n	___________________________\n");
	printf("	");
	for(thang = 0; thang < resource_count; thang++){
		printf("	%d ",max_available_res[thang]);
	}
	thang = 0;
	printf("\n\n");
	

}


/*
*Running bankers algorithm
*/
int bankers(process * processi, int process_count, int resource_count, int * available_res, int* max_available_res, int process_no){
	int available_resources[resource_count];
	int safe_proc[process_count];
	int safe = 0;
	int count = process_count;
	//running = (int*)calloc(resource_count,sizeof(int)*resource_count);
	for (int thang = 0; thang < resource_count; thang++){
		available_resources[thang] = available_res[thang];
		if(processi[process_no].current_request[thang] > available_res[thang]){
			
			return 0;
		}
	}
	
	while(count != 0){
		count = 0;

		for(int value = 0;value < process_count; value++ ){
			safe = 1;
			if(safe_proc[value] == 0){
				
				for(int thang = 0; thang < resource_count; thang++){
						if ((processi[value].maximum_possible_request[thang] - processi[value].current_allocation[thang]) > available_resources[thang] ){
							safe = 0;
							break;
						}
				
				}
			
				if(safe == 1){
					safe_proc[value] = 1;
					count = 1;
					
					for(int thang = 0; thang < resource_count; thang++){
						available_resources[thang] = available_resources[thang] + processi[value].current_allocation[thang];
					}
				}
			}
		}
		
		if(safe == 0){
			return 0;
		}else if(safe == 1){
			return 1;
		}
		
		

	}
	
	return 1;
}
int main(){
	/*
	*Variable declarations
	*/
	process * processes;
	(void) signal(SIGINT,handler);
	//srand(2);
	int no_resource_types;
	int no_processes;
	int * temp;
	int * temp_availability;
	int * request_array;
	int * orig_alloc;
	int * first_go;
	//int * temp_request; 
	int sim_time = 1;
	int * available;
	int * maximum_available;
	int i=0;	
	int res;
	int redo = 0;
	int choice; 
	int request;
	//int first_go;
	int release_request;
	int sum;
	int dont_fulfill = 0;
	//int process_count = 0;
	int safe = 0;

	/*
	*  User Input infromation to run the simulation
	*/
	printf("Number of different resource types: ");
	scanf("%d",&no_resource_types);
	available = (int*)calloc(no_resource_types,sizeof(int)*no_resource_types);
	maximum_available = (int*)calloc(no_resource_types,sizeof(int)*no_resource_types);
	temp = (int*)calloc(no_resource_types,sizeof(int)*no_resource_types);
	temp_availability = (int*)calloc(no_resource_types,sizeof(int)*no_resource_types);
	request_array = (int*)calloc(no_resource_types,sizeof(int)*no_resource_types);
	orig_alloc = (int*)calloc(no_resource_types,sizeof(int)*no_resource_types);
	//temp_request = (int*)calloc(no_resource_types,sizeof(int)*no_resource_types);
	printf("Number of instances of each resource type: ");
	
	while (i < no_resource_types) {
		scanf("%d", &res);
		available[i] = res;
		maximum_available[i] = res;
		i++;	
	}
	

	printf("Number of processes: ");
	scanf("%d",&no_processes);
	processes = (process*)malloc(sizeof(process)*no_processes);
	first_go = (int*)calloc(no_processes,sizeof(int)*no_processes);
	for(int value = 0; value < no_processes; value++){
		printf("Details of P%d ", value+1);
		i = 0;
		processes[value].waiting = 0;
		processes[value].current_allocation = (int*)calloc(no_resource_types,sizeof(int)*no_resource_types);
		processes[value].current_request = (int*)calloc(no_resource_types,sizeof(int)*no_resource_types);
		processes[value].maximum_possible_request = (int*)calloc(no_resource_types,sizeof(int)*no_resource_types);
		while (i < no_resource_types) {
			scanf("%d", &res);
			processes[value].maximum_possible_request[i] = res;
			i++;
		
		}
		

	}

	//print_tables(processes, no_processes, no_resource_types, available, maximum_available);

	/*
	*actually running the simlation
	*/
	while(1){
		if ((sim_time%5) == 0) {
			//printf("in sim\n");
			/*
			*Iterating through each process
			*/
			for(int process_count = 0; process_count < no_processes; process_count++){
				/*
				* deciding whether I should request, release, or continue
				*/
				//first_go = 1;
				choice = rand()%3;
				if(choice  == 0){
					if (processes[process_count].waiting == 0 ){
						/*
						*Processing the intial rewuest
						*/
						printf("Request (");
						for(int resource= 0; resource < no_resource_types ;resource++){
								request = rand()%(processes[process_count].maximum_possible_request[resource]-
								processes[process_count].current_allocation[resource]+1);
								orig_alloc[resource] = processes[process_count].current_allocation[resource];
								processes[process_count].current_request[resource] = request;
								printf("%d",request);
								if (resource < (no_resource_types - 1)){
									printf(", ");
								}
						}
						printf(") came from P%d\n", process_count+1);
						/*
						* Running Bankers algorithem to see if the process is in a safe state
						*/
						safe = bankers(processes, no_processes, no_resource_types, available, maximum_available,process_count);

						if(safe == 0){
							printf("Request (");
							for(int resource= 0; resource < no_resource_types ;resource++){
								//available[resource] = available[resource] - processes[process_count].current_request[resource];
								processes[process_count].current_allocation[resource] = orig_alloc[resource];
								printf("%d",processes[process_count].current_request[resource]);
								if (resource < (no_resource_types - 1)){
									printf(", ");
								}
								
							}
							printf(") from P%d cannot be satisfied, P%d is in waiting state\n",process_count+1,process_count+1);
							processes[process_count].waiting = 1;
							print_tables(processes, no_processes, no_resource_types, available, maximum_available); 
						}else if(safe != 0){
							printf("Request (");
							for(int resource= 0; resource < no_resource_types ;resource++){
								available[resource] = available[resource] - processes[process_count].current_request[resource];
								processes[process_count].current_allocation[resource] = processes[process_count].current_allocation[resource] + 
								processes[process_count].current_request[resource];
								printf("%d",processes[process_count].current_request[resource] );
								if (resource < (no_resource_types - 1)){
									printf(", ");
								}
							}
							printf(") from P%d has been granted\n",process_count+1);
							print_tables(processes, no_processes, no_resource_types, available, maximum_available);
							first_go[process_count] = 1;
							
						}
					}else if(processes[process_count].waiting == 1 ) {
						/*
						* If the process is in a waiting state then run banker and see if it can get req resources
						*/
						safe = bankers(processes, no_processes, no_resource_types, available, maximum_available,process_count);
						if(safe != 0){

							printf("Request (");
							for(int resource= 0; resource < no_resource_types ;resource++){
								available[resource] = available[resource] - processes[process_count].current_request[resource];
								processes[process_count].current_allocation[resource] = processes[process_count].current_allocation[resource] + 
								processes[process_count].current_request[resource];
								printf("%d",processes[process_count].current_request[resource]);
								if (resource < (no_resource_types - 1)){
									printf(", ");
								}
							}
							printf(") from P%d has been satisfied\n",process_count+1);
							processes[process_count].waiting = 0;
							first_go[process_count] = 1;
							//print_tables(processes, no_processes, no_resource_types, available, maximum_available);
							
						}
						
						
					}
					sleep(2);

				}else if (choice == 1){
					/*
					* Releasing resources
					*/
					if(processes[process_count].waiting == 0){
						
						sum = 0;
						/*
						* making  sure that there is at least one allocated resource to the process
						*/
						
						
						redo = 1;
						while(redo == 1 ){
							
							redo = 0;
							//printf("in here");
							for(int resource = 0; resource < no_resource_types ;resource++){
								release_request = rand()%(processes[process_count].current_allocation[resource] + 1);
								//printf("release_request %d\n",release_request);
								request_array[resource] = release_request;
								temp[resource] = processes[process_count].current_allocation[resource];
								sum = sum + (processes[process_count].current_allocation[resource] - release_request);
								processes[process_count].current_allocation[resource] = processes[process_count].current_allocation[resource] - release_request;
								available[resource] = available[resource] + release_request;
								
			
							}
							//printf("%s\n"sum);
							/*
							for(int - processi[value].current_request[thang])resource = 0; resource < no_resource_types ;resource++){
								printf("request_array contents %d",request_array[resource]);
							}
							printf("\n");
							*/
							if(first_go[process_count] == 0 && sum == 0) {
								dont_fulfill = 1;
								break;
							}
							if(sum == 0){
								//keep_1 = rand()
								//printf("\n");
								/*
								*Resetting the array if it is zero
								*/
								//redo = 1;
								for (int resource = 0; resource < no_resource_types ;resource++){
									processes[process_count].current_allocation[resource] = temp[resource];
									available[resource] = temp_availability[resource]; 
								}
								dont_fulfill = 1;
								break;
							} 

							
						}

						if (dont_fulfill == 0){
							printf("P%d has released (",process_count+1);
							for (int resource = 0; resource < no_resource_types ;resource++){
								printf("%d",request_array[resource]);
								if (resource < (no_resource_types - 1)){
									printf(", ");
								}
							}
							//release resouce
							printf(") resources\n");
						}
						dont_fulfill = 0;

						
					}
					redo = 0;					
				}else if (choice == 2){
					//printf("do absolutely nothing like nothing at all\n");
					//continue
					//do absolutely nothing like nothing at all 
					//sleep(2);
				}
				
			}
			
		}
		
		//process_count = 0;
		sim_time++;
		//sleep(5);
		//print_tables(processes, no_processes, no_resource_types, available, maximum_available);

	}
	
	/*
	*Freeing all the memeory I allocated
	*/
	for(int thang = 0; thang < no_processes; thang++){
		free(processes[thang].current_allocation);
		free(processes[thang].current_request);
		free(processes[thang].maximum_possible_request);

	}
	
	free(processes);
	free(temp);
	free(available);
	free(maximum_available);
	return 0;
}
