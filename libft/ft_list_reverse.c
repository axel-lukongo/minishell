#include"libft.h"
#include<stdio.h>
#include<stdlib.h>
void ft_list_reverse(t_list **begin_list)
{
	t_list *ptr1;
	t_list *ptr2;
	t_list *ptr3;
	
	ptr1 = *begin_list;
	ptr2 = ptr1->next;
	ptr3 = ptr2->next;
	ptr1->next = NULL;
	ptr2->next = ptr1;

	while (ptr3)
	{
		ptr1 = ptr2;
		ptr2 = ptr3;
		ptr3 = ptr3->next;
		ptr2->next = ptr1;
	}
	*begin_list=ptr2;
	//print_list(*begin_list);
}

/*int main(int argc, char **argv)
{
	t_list *chaine;
	t_list *tmp;
	chaine = ft_list_push_strs(argc,argv);
	ft_list_reverse(&chaine);
	while (chaine->next)
	{
		tmp = chaine->next;
		free(chaine);
		chaine = tmp;
	}	
	free(chaine);
}*/