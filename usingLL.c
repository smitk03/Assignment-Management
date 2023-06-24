#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LEN 50

typedef enum{FAILURE,SUCCESS} status_code;

typedef struct student_record
{
	int student_id;
	char name[MAX_LEN];
	char group_partner[MAX_LEN];
	int group_id;
	char assignment_name_id[MAX_LEN];
	char deadline[MAX_LEN];
	char status[MAX_LEN];
	float offline_marks;
	float viva_marks;
	
	struct student_record* next;
	
}Node1;


typedef struct assignment_record
{
	int assignment_id;
	char topic_name[MAX_LEN];
	char status[MAX_LEN];
	struct StudentAssignmentRecord* student_group;
	
	struct assignment_record* next;
}Node2;


typedef struct StudentAssignmentRecord 
{
    int student_group_id;
    char status[MAX_LEN];
    float marks_given;
    
    struct StudentAssignmentRecord* next;
}Node3;


Node1* student = NULL;
Node2* assignment = NULL;


Node1* create_student(int student_id,char* name,char* group_partner,int group_id,char* assignment_name_id,char* deadline,char* status,float offline_marks,float viva_marks)
{
	Node1* nptr = (Node1*)malloc(sizeof(Node1));
	
	nptr->student_id = student_id;
	strcpy(nptr->name , name);
	strcpy(nptr->group_partner , group_partner);
	nptr->group_id = group_id;
	strcpy(nptr->assignment_name_id , assignment_name_id);
	strcpy(nptr->deadline , deadline);
	strcpy(nptr->status , status);
	nptr->offline_marks = offline_marks;
	nptr->viva_marks = viva_marks;
	nptr->next = NULL;
	
	return nptr;
}


Node2* create_assignment(int assignment_id , char* topic_name)
{
	Node2* nptr = (Node2*)malloc(sizeof(Node2));
	
	nptr->assignment_id = assignment_id;
	strcpy(nptr->topic_name , topic_name);
	strcpy(nptr->status , "Declared");
	nptr->student_group = NULL;
	nptr->next = NULL;
	
	return nptr;
}

float max(float a , float b)
{
	if(a>=b)
	{
		return a;
	}
	return b;
}

Node3* create_student_group(int student_group_id , char* status , float offline_marks1 , float viva_marks1 , float offline_marks2 , float viva_marks2)
{
	Node3* nptr = (Node3*) malloc(sizeof(Node3));
	
	nptr->student_group_id = student_group_id;
	strcpy(nptr->status, status);
	nptr->marks_given = max(offline_marks1+viva_marks1 , offline_marks2+viva_marks2);
	nptr->next = NULL;
	
	return nptr;
}




/////Question1 ------------------------>>>

void insert_student(Node1** student,int student_id1,int student_id2,char* name,char* group_partner,int group_id,char* assignment_name_id,char* deadline,char* status,float offline_marks1,float viva_marks1,float offline_marks2,float viva_marks2,int assignment_id)
{
	Node1* ptr = *student;
	Node1* nptr1 = create_student(student_id1,name,group_partner,group_id,assignment_name_id,deadline,status,offline_marks1,viva_marks1);
	Node1* nptr2 = create_student(student_id2,group_partner,name,group_id,assignment_name_id,deadline,status,offline_marks2,viva_marks2);
	nptr1->next = nptr2 ;
	nptr2->next = ptr;
	
	*student = nptr1;
	
	
	
	//Checking in asg record
	int found = 0;
	
	Node2* nptr = assignment ;
	while(nptr!=NULL)
	{
		if(strcmp(nptr->topic_name , assignment_name_id) == 0)
		{
			found =1;
			break;
		}
		nptr = nptr->next;
	}
	
	if(found == 0)
	{
		Node2* nptr = create_assignment(assignment_id , assignment_name_id );
		nptr->next = assignment;
		assignment = nptr;
	}
	
	
	// Adding in student group
	Node3* nptr3 = create_student_group(group_id ,status ,offline_marks1 ,viva_marks1 ,offline_marks2 ,viva_marks2);
	nptr = assignment;
	
	while(nptr!=NULL)
	{
		if(strcmp(nptr->topic_name , assignment_name_id) == 0)
		{
			nptr3->next = nptr->student_group;
			nptr->student_group = nptr3;
			
			break;
		}
		nptr = nptr->next;
	}
	
	
	
}



/////Question 2 ----------------------->>>

void insert_assignment(Node1** student , Node2** assignment , char* topic_name , int assignment_id , int* student_id , char** name , int* group_id , char* deadline , char** status , float* offline_marks , float* viva_marks , int num_stud )
{
	Node2* nptr = create_assignment(assignment_id , topic_name );
	nptr->next = *assignment;
	*assignment = nptr;
	int i;
	for(i=0;i<num_stud;i+=2)
	{
		Node3* nptr3 = create_student_group(group_id[i] ,status[i] ,offline_marks[i] ,viva_marks[i] ,offline_marks[i+1] ,viva_marks[i+1]);
		nptr = *assignment;
		
		nptr3->next = nptr->student_group;
		nptr->student_group = nptr3;
	}
	
	for(i=0;i<num_stud;i+=2)
	{
		Node1* ptr = *student;
		Node1* nptr1 = create_student(student_id[i],name[i],name[i+1],group_id[i],topic_name,deadline,status[i],offline_marks[i],viva_marks[i]);
		Node1* nptr2 = create_student(student_id[i+1],name[i+1],name[i],group_id[i],topic_name,deadline,status[i],offline_marks[i+1],viva_marks[i+1]);
		nptr1->next = nptr2 ;
		nptr2->next = ptr;
		
		*student = nptr1;
	}
	
}




/////Question 3 ------------------------>>>

void change_status(Node1** student , Node2** assignment , int group_id , char* status)
{
	Node1* ptr = *student;
	while(ptr!=NULL)
	{
		if(ptr->group_id == group_id)
		{
			strcpy(ptr->status , status);
			strcpy(ptr->next->status , status);
			break;
		}
		ptr = ptr->next;
	}
	
	
	
	Node2* ptr2 = *assignment;
	while(ptr2!=NULL)
	{
		if(strcmp(ptr2->topic_name , ptr->assignment_name_id) == 0)
		{
			Node3* ptr3 = ptr2->student_group;
			while(ptr3!=NULL)
			{
				if(ptr3->student_group_id == group_id)
				{
					strcpy(ptr3->status , status);
					break;
				}
				ptr3 = ptr3->next ;
			}
			break;
		}
		ptr2 = ptr2->next;
	}
	
	
	
	
	ptr2 = *assignment;
	int flag ;
	while(ptr2!=NULL)
	{
		flag=1;
		Node3* ptr3 = ptr2->student_group;
		while(ptr3!=NULL)
		{
			if(ptr3->status != "Evaluated")
			{
				flag = 0;
				break;
			}
			ptr3 = ptr3->next;
		}
		if(flag == 1)
		{
			strcpy(ptr2->status , "Evaluated");
		}
		ptr2 = ptr2->next;
	}
	
}




/////Question 4 ------------------------>>>

void print_declared_not_eval(Node2** assignment)
{
	Node2* ptr = *assignment;
	while(ptr!=NULL)
	{
		if(ptr->status != "Evaluated" && ptr->status != "DueDateOver")
		{
			printf("Assignment id : %d\n",ptr->assignment_id);
			printf("Topic Name : %s\n",ptr->topic_name);
			printf("Status : %s\n",ptr->status);
			printf("-----------\n");
		}
		ptr = ptr->next;
	}
}





/////Question 5 ------------------------->>>

void print_stud_not_submitted(Node2** assignment , Node1** student)
{
	Node2* ptr = *assignment;
	while(ptr!=NULL)
	{
		int found = 0;
		if(ptr->status == "DueDateOver")
		{
			printf("Assignment id : %d\n",ptr->assignment_id);
			printf("Topic Name : %s\n",ptr->topic_name);
			printf("Status : %s\n\n",ptr->status);
			
			Node3* ptr2 = ptr->student_group;
			while(ptr2!=NULL)
			{
				if(ptr2->status == "NotSubmitted")
				{
					Node1* ptr3 = *student;
					while(ptr3!=NULL)
					{
						if(ptr3->group_id == ptr2->student_group_id)
						{
							printf("		Group id : %d\n",ptr2->student_group_id);
							printf("		Member 1 : %s\n",ptr3->name);
							printf("		Member 2 : %s\n\n",ptr3->group_partner);
							found = 1;
							break;
						}
						ptr3 = ptr3->next;
					}	
				}
				ptr2 = ptr2->next;
			}
			if(found == 0)
			{
				printf("		No One\n\n");
			}
			printf("------------\n\n");
		}
		ptr = ptr->next;
	}
}





/////Question 6 ------------------------->>>

void print_stud_notEval_sub(Node1** student)
{
	Node1* ptr = *student;
	while(ptr!=NULL)
	{
		if(ptr->status == "Submitted")
		{
			printf("Group id : %d\n",ptr->group_id);
			printf("Assignment Topic : %s\n",ptr->assignment_name_id);
			printf("Member 1 : %s\n",ptr->name);
			printf("Member 2 : %s\n",ptr->group_partner);
			printf("Submitted but not Evaluated\n\n\n");
		}
		if(ptr->status == "Evaluated")
		{
			if(ptr->viva_marks == -1)
			{
				printf("Group id : %d\n",ptr->group_id);
				printf("Assignment Topic : %s\n",ptr->assignment_name_id);
				printf("Member 1 : %s\n",ptr->name);
				printf("Member 2 : %s\n",ptr->group_partner);
				printf("Evaluated but Viva is Not Yet Completed\n\n\n");
			}
		}
		ptr = ptr->next->next;
	}
}





/////Question 7 ------------------------->>>


void BubbleSort(Node3* lptr)
{
	Node3* ptr = lptr;
	Node3* ptr1 = NULL;
	int sorted = 0;
	while(sorted == 0)
	{
		ptr = lptr;
		sorted = 1;
		while(ptr->next!=ptr1)
		{
			if(ptr->marks_given < ptr->next->marks_given)
			{
				int a = ptr->marks_given;
				ptr->marks_given = ptr->next->marks_given;
				ptr->next->marks_given = a;
				sorted = 0;
			}
			ptr = ptr->next;
		}
		ptr1 = ptr;
	}
}


void print_dec_order(Node2** assignment , int assignment_id)
{
	Node2* ptr = *assignment;
	while(ptr!=NULL)
	{
		if(ptr->assignment_id == assignment_id)
		{
			Node3* ptr1 = ptr->student_group;
			BubbleSort(ptr1);
			
			while(ptr1!=NULL)
			{
				printf("Group id : %d\n",ptr1->student_group_id);
				printf("Marks Given : %f\n\n\n",ptr1->marks_given);
				ptr1 = ptr1->next;
			}
			break;
		}
		ptr = ptr->next;
	}
}





/////Question 8 -------------------------->>>

void print_top_stud(Node2** assignment)
{
	Node2* ptr = *assignment;
	while(ptr!=NULL)
	{
		printf("Assignment id : %d\n",ptr->assignment_id);
		printf("Topic Name : %s\n",ptr->topic_name);
		Node3* ptr1 = ptr->student_group;
		BubbleSort(ptr1);
		printf("		Group id : %d\n",ptr1->student_group_id);
		printf("		Top Marks Given : %f\n\n\n",ptr1->marks_given);
		
		ptr = ptr->next;
	}
}




////Display Marks of a student ----------------->>>

void Display_Marks(char* name)
{
	Node1* ptr = student;
	while(ptr!=NULL)
	{
		if(strcmp(ptr->name , name) == 0)
		{
			printf("Printing your Marks : \n");
			printf("Group id : %d\n",ptr->group_id);
			printf("Assignment Topic : %s\n",ptr->assignment_name_id);
			printf("Name : %s\n",ptr->name);
			if(ptr->status == "NotSubmitted")
			{
				printf("Assignment Not Submitted yet\n\n");
			}
			else if(ptr->status == "Submitted")
			{
				printf("Assignment Submitted but not evaluated yet\n\n");
			}
			else
			{
				printf("Offline Marks : %f\n",ptr->offline_marks);
				printf("Viva Marks : %f\n\n",ptr->viva_marks);
			}
		}
	}
}






/////Display Group Details ----------------------->>>

void Display_grp_details(char* name)
{
	Node1* ptr = student;
	while(ptr!=NULL)
	{
		if(strcmp(ptr->name , name) == 0)
		{
			printf("Printing Group Details : \n");
			printf("Group id : %d\n",ptr->group_id);
			printf("Assignment Topic : %s\n",ptr->assignment_name_id);
			printf("Member 1 : %s\n",ptr->name);
			printf("Member 2 : %s\n",ptr->group_partner);
			printf("Status : %s\n\n",ptr->status);
		}
	}
}






////Print student records ------------------------>>>>

void print_stud_rec(Node1* student)
{
	Node1* ptr = student;
	while(ptr!=NULL)
	{
		printf("%d\n",ptr->student_id);
		printf("%s\n",ptr->name);
		printf("%s\n",ptr->group_partner);
		printf("%d\n",ptr->group_id);
		printf("%s\n",ptr->assignment_name_id);
		printf("%s\n",ptr->deadline);
		printf("%s\n",ptr->status);
		printf("%f\n",ptr->offline_marks);
		printf("%f\n",ptr->viva_marks);
		printf("---------------\n");
		
		ptr = ptr->next;
	}
}






/////Main Function

int main()
{
	int a;
	printf("Choose the correct choice :\n");
	printf("1. Student\n");
	printf("2. Teacher\n");
	printf("Enter your choice : ");
	scanf("%d",&a);
	
	switch(a)
	{
		char name[MAX_LEN] ;
		int password ,found;
		case 1 :
			found = 0;
			printf("Enter your Name : ");
			scanf("%s",name);
			printf("Enter your Password (Password is same as of your student_id) : ");
			scanf("%d",&password);
			Node1* ptr = student;
			while(ptr!=NULL)
			{
				if(ptr->student_id == password && strcmp(ptr->name,name)==0)
				{
					printf("Validated\n");
					found = 1;
					do
					{
						int b;
						printf("Choose the correct choice :\n");
						printf("1. Display your Marks\n");
						printf("2. Display your Group Details\n");
						printf("3. Exit this block\n\n");
						printf("Enter your choice : ");
						scanf("%d",&b);
						if(b==3)
						{
							printf("Exiting this block\n\n");
							break;
						}
						switch(b)
						{
							case 1 :
								Display_Marks(name);
								break;
							case 2 :
								Display_grp_details(name);
								break;
							default : 
								printf("Invalid Choice\n");	
								break;
						}
					}while(1);
					break;
				}
				ptr = ptr->next;
				if(found == 0)
				{
					printf("Password is Wrong / User not Found\n\n");
				}
			}
		char name2[MAX_LEN] ;
		char password1[MAX_LEN] ;
		case 2 :
			printf("Enter your Name : ");
			scanf("%s",name2);
			printf("Enter your Password (all small letters) : ");
			scanf("%s",password1);
			printf("Hi");
			if(strcmp(name2, "Keskar") == 0 && strcmp(password1, "keskar") == 0 )
			{
				printf("Validated\n");
				do
				{
					int b;
					printf("Choose the correct choice :\n");
					printf("1. Insert Student Record\n");
					printf("2. Insert Assignment\n");
					printf("3. Change Status\n");
					printf("4. View Details of all students\n");
					printf("5. View Details of All Assignment\n");
					printf("6. View all students assigned to a particular assignment\n");
					printf("7. Exit this block\n\n");
					printf("Enter your choice : ");
					scanf("%d",&b);
					if(b==7)
					{
						printf("Exiting this block\n\n");
						break;
					}
					switch(b)
					{
						int c,d,h,o;
						char e[MAX_LEN],f[MAX_LEN],g[MAX_LEN],i[MAX_LEN],j[MAX_LEN];
						float k,l,m,n;
						case 1 :
							printf("Inserting student record\n");
							printf("Enter the both student ids and names : ");
							scanf("%d %d %s %s",&c,&d,e,f);
							printf("\nEnter group id : ");
							scanf("%d",&o);
							printf("\nEnter asg name and id");
							scanf("%s %d",g,&h);
							printf("\nEnter deadline and status : ");
							scanf("%s %s",i,j);
    						printf("\nEnter offline and viva marks of both students : ");
							scanf("%f %f %f %f",&k,&l,&m,&n);
							
							insert_student(&student,c,d,e,f,o,g,i,j,k,l,m,n,h);
							print_stud_rec(student);
					}
				}while(1);
			}
			else
			{
				printf("Incorrect Password\n");
			}
						
	}
}


