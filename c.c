#include <stdio.h>

	struct ab{
		int i;
		int j;
	};

void changee(struct ab *ab)
{
	ab->i = 66;
	ab->j = 5;
}
int main()
{
	struct ab a;
	changee(&a);
	printf("%d %d",a.i ,a.j);
}