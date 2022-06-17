/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 11:47:42 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/30 17:23:34 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<string.h>
#include<stdio.h>
#include"libft.h"

/*int main() {

    char array [] = { "salut"};
    size_t size = sizeof( char ) * 8;
    int length;

    // Display the initial values 
    for( length=0; length<5; length++) {
        printf( "%c ", array[ length ] );
    }
    printf( "\n" );

    // Reset the memory bloc 
    ft_memset( array, 2, size );

    //Display the new values 
    for( length=0; length<9; length++) {
        printf( "%d ", array[ length ] );
    }
    printf( "\n" );
    
    return 0;
}*/
/*int main()
{
  int i = 0;
  char **tab;
  int x = 0;
  tab = ft_strsplit("sa---lu",'-');
  if(tab == NULL)
    return(0);
  while (i < 3)
    {
      printf("string %d : %s\n", i, tab[i]);
      i++;
    }
  
  while(x < 3)
    {
      free(tab[x++]);
    }
  free(tab);
  return (0);
}*/
/*int main() //-----ft_strtrim-----
{
	char	*str;

	str = ft_strtrim("  lorem ipsum dolor sit amet  ,  "," ");
	printf("%s\n", str);
	free(str);
}*/
/*int main() //------ft_strdel----
{
	printf("[ft_strdel]\n");
	char **tofree2;
	tofree2 = ft_memalloc(1);
	tofree2[0] = ft_strnew(2);
	tofree2[0][0] = 'y';
	tofree2[0][1] = 'o';
	tofree2[0][2] = '\0';
	tofree2[1] = NULL;
	printf("%s\n", tofree2[0]);
//	printf("%d\n", (int)tofree2[0]);
	printf("----------\n");
	ft_strdel(tofree2);//FT_STRDEL
	printf("%s\n", *tofree2[0]);
//	printf("%d\n", (int)tofree2[0]);
	printf("\n");
	printf("[expected]\n");
	printf("yo\n(some adress)\n----------\n(null)\n0\n");
}*/
/*int main() //------ft_itoa----
{
	char *str;
	str = ft_itoa(214748364);
	printf("%s\n",str);
}*/
/*int main() //-----ft_putendl_fd-----
{
	ft_putendl_fd("salut",1);
}*/
/*int main() //-----ft_putnbr_fd-----
{
	ft_putnbr_fd(25,1);
}*/
/*int main()
{
	t_list *ptr;
	ptr = ft_lstnew("21");
	print_list(ptr);
}*/
/*int main() //------ft_lstnew-----
{
	t_list *ptr1;
	t_list *ptr2;
	ptr1 = ft_lstnew("2");
	ptr2 = ft_lstnew("5");
	ft_lstadd_front(&ptr1,ptr2);
	print_list(ptr1);
}*/
/*void *f(void *str) // fonction for -----ft_lstmap----
{
	int a = 4;
	str = &a;
	return(str);
}*/
/*int main(void)//-----ft_lstmap----
{
	t_list *lst;
	t_list *list2;
	int a = 3;
	lst = (t_list*)malloc(sizeof(t_list));
	lst->content = &a;
	lst->next = (t_list*)malloc(sizeof(t_list));
	lst->next->content = &a;
	lst->next->next = (t_list*)malloc(sizeof(t_list));
	lst->next->next->content = &a;
	lst->next->next->next = (t_list*)malloc(sizeof(t_list));
	lst->next->next->next->content = &a;
	lst->next->next->next->next = (t_list*)malloc(sizeof(t_list));
	lst->next->next->next->next->content = &a;
	lst->next->next->next->next->next =  0;
	print_list(lst);
	list2 = ft_lstmap(lst,f);
	print_list(list2);
	while (list2)
	{
		free(lst);
		lst = list2;
		list2 = list2->next;
	}
	free(lst);
	return (0);
}*/
/*int main() //-----ft_atoi---
{
	int	c;

	c = ft_atoi("+563");
	printf("%d\n", c);
	printf("vrai atoi %d\n", atoi("+563"));
}*/
/*int main() //-----ft_bzero-----
{
    char array [] = { "salut"};
    size_t size = sizeof( char ) * 8;
    int length;

    // Display the initial values 
    for( length=0; length<5; length++) {
        printf( "%c ", array[ length ] );
    }
    printf( "\n" );

    // Reset the memory bloc 
    ft_bzero( array,size);

    //Display the new values 
    for( length=0; length<9; length++) {
        printf( "%d ", array[ length ] );
    }
    printf( "\n" );
    
    return 0;
}*/
/*int main() //------memset-----
{
    char array [] = { "salut"};
    size_t size = sizeof( char ) * 8;
    int length;

    // Display the initial values 
    for( length=0; length<5; length++) {
        printf( "%c ", array[ length ] );
    }
    printf( "\n" );

    // Reset the memory bloc 
    memset( array, 2, size );

    //Display the new values 
    for( length=0; length<9; length++) {
        printf( "%d ", array[ length ] );
    }
    printf( "\n" );
    
    return 0;
}
*/
int	main()//-----ft_Split----
{
	/*char	**str;
	int		i;

	i = 0;
	str = ft_split("444lorem444ipsum4dolor44444sit4amet,4consectetur444adipiscing4elit.4Sed4non4risus.4Suspendisse444", '4');
	while (str[i])
	{
		printf("%s\n", str[i++]);
	}*/
	//char *s = "      split       this for   me  !       ";
	int i = 0;
	char	**result = ft_split("", ' ');

	if (!result)
	{
		printf("not string");
		return (0);
	}
	while (result[i])
	{
		printf("%s\n", result[i++]);
	}
	return (0);
}

/*int main() //------ft_strnstr----
{
	printf("[ft_strnstr]\n");
	char s22[50], s23[50];
	strcpy(s22, "tritricolor");
	strcpy(s23, "tric");
	char *p7;
	p7 = ft_strnstr(s22, s23, 7);//FT_STRNSTR
	printf("%s\n", p7);
	char s220[50], s230[50];
	strcpy(s220, "ozarabozaraboze123");
	strcpy(s230, "ozaraboze");
	char *p70;
	p70 = strnstr(s220, s230, 15);
	printf("%s\n", p70);
	printf("\n");
	printf("[strnstr]\n");
	char s24[50], s25[50];
	strcpy(s24, "tritricolor");
	strcpy(s25, "tric");
	char *p8;
	p8 = strnstr(s24, s25, 7);
	printf("%s\n", p8);
	char s240[50], s250[50];
	strcpy(s240, "ozarabozaraboze123");
	strcpy(s250, "ozaraboze");
	char *p80;
	p80 = strnstr(s240, s250, 15);
	printf("%s\n", p80);
}*/
/*int main()//------ft_memmove------
{
	char *dest;

	dest = ft_memmove(dest, "bonjours", 5);
	printf("%s\n", dest);
}*/
/*int	main(void)
{
	printf("%s\n", ft_itoa(0));
}*/
