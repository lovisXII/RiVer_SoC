//unsigned int good;
unsigned int good=12;
int main () 
	{
	int a=14;
	int b=21;

	while (a != b)
		{
		if (a > b) a-= b;
		else b -= a;
		}
	good = a;
	return 0;
	}
