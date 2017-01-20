#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct slice{
		struct slice *L;
		char *descr;
		char dir;
		int num;
		struct slice *R;
		}slice;

slice* curr = NULL, *begin = NULL;
char currdir = 'R';

slice *mkslice(char *descr, char dir, int num)
{
	slice *sp = malloc(sizeof(slice));
	if (sp)
	{
		sp->R = sp;
		sp->descr = descr;
		sp->dir = dir;
		sp->num = num;
		sp->L = sp;
	}
	return sp;
}

slice *add(char *descr, char dir, int num)
{
	slice *toBeAdded, *next;
	printf("A");
	if(!curr)
	{
		begin = curr = mkslice(descr, dir, num);
		printf(" %s %s %s\n", curr->descr, curr->descr, curr->descr);
		return curr;
	}
	toBeAdded = mkslice(descr, dir, num);
	if (currdir == 'R')
	{
		next = curr->R;
		curr->R = toBeAdded;
		toBeAdded->L = curr;
		toBeAdded->R = next;
		next->L = toBeAdded;
	}
	else if(currdir == 'L')
	{
		next = curr->L;
		curr->L = toBeAdded;
		toBeAdded->R = curr;
		toBeAdded->L = next;
		next->R = toBeAdded;
	}
	printf(" %s", toBeAdded->L->descr);
	printf(" %s", toBeAdded->descr);
	printf(" %s\n", toBeAdded->R->descr);
	return toBeAdded;
}

slice *delete()
{
	slice *newcurr, *toBeDeleted;
	toBeDeleted = curr;
	printf("D");
	printf(" %s", toBeDeleted->L->descr);
	printf(" %s", toBeDeleted->descr);
	printf(" %s\n", toBeDeleted->R->descr);
	if(currdir == 'R')
	{
		newcurr = toBeDeleted->L;
		newcurr->R = toBeDeleted->R;
		toBeDeleted->R->L = newcurr;
	}
	else if(currdir == 'L')
	{
		newcurr = toBeDeleted->R;
		newcurr->L = toBeDeleted->L;
		toBeDeleted->L->R = newcurr;
	}
	free(toBeDeleted);
	return newcurr;
}

slice *run()
{
	int i , steps = curr->num;
	printf("R");
	currdir = curr->dir;
	if(curr->dir == 'R')
	{
		for (i = 0; i < steps; i++)
		{
			curr = curr->R;
			printf(" %s", curr->descr);
		}
	}
	else if(curr->dir == 'L')
	{
		for (i = 0; i < steps; i++)
		{
			curr = curr->L;
			printf(" %s", curr->descr);
		}
	}
	else if (curr->dir == 'N') printf(" DRILL");
	printf("\n");
	return curr;
}

void test(int slice_lim)
{
	int i, j, flag = 0, steps;
	slice *current = begin;
	steps = current->num;
	printf("T");
	if(!strcmp(current->descr,"DRILL"))
	{
		printf(" DRILL\n"); return;
	}
	else
	{
		for (j = 0; j < slice_lim; j++)
		{
			if(current->dir == 'R')
			{
				for (i = 0; i < steps; i++)
				{
					printf(" %s", current->descr);
					if (!strcmp(current->descr, "DRILL"))
					{
						flag = 1;
						break;
					}
					else current = current->R;
				}
			}
			else if(current->dir == 'L')
			{
				for (i = 0; i < steps; i++)
				{
					printf(" %s", current->descr);
					if(!strcmp(current->descr, "DRILL")) 
					{
						flag = 1;
						break;
					}
					else current = current->L;
				}
			}
			if(flag) 
			{
				printf("\n"); break;
			}
			steps = current->num;
			if(!strcmp(current->descr, "DRILL"))
			{
				flag = 1;
				printf(" DRILL\n");
				break;
			}
		}
		if(!flag)
		{
			printf(" %s", current->descr);
			printf(" DRILL_NOT_FOUND\n");
		}
	}
}

void control()
{
	slice *current = begin;
	printf("C");
	if(current->R == begin)
	{
		printf(" %s %s", current->descr, current->descr);
	}
	else
	{
		while(current->R != begin)
		{
			printf(" %s", current->descr);
			current = current->R;
		}
		printf(" %s", current->descr);
		while(current->L != begin->L)
		{
			printf(" %s", current->L->descr);
			current = current->L;
		}
	}
	printf("\n");
}

int main()
{
	char *command = malloc(8), *descr, dir;
	int i, len, num, slice_lim;
	while(1)
	{
		scanf(" %s", command);
		if (!strcmp(command, "ADD"))
		{
			scanf(" %d", &len);
			descr = malloc(len+1);
			for(i = 0; i < len; i++) scanf(" %c", &descr[i]);
			descr[len] = 0;
			if(strcmp(descr, "DRILL"))
			{
				scanf(" %c %d", &dir, &num);
				curr = add(descr, dir, num);
			}
			else curr = add(descr, 'N', -1);
		}
		if(!strcmp(command, "DELETE")) curr = delete();
		if(!strcmp(command, "RUN"))
		{
			if(!strcmp(curr->descr, "DRILL"))
			{
				printf("R DRILL\n"); break;
			}
			else curr = run();
		} 
		if(!strcmp(command, "CONTROL")) control();		
		if(!strcmp(command, "TEST")) 
		{
			scanf(" %d", &slice_lim);
			test(slice_lim);
			break;
		}
	}
	free(command);
	return 0;
}