
#include "UniVar.h"
#include "StandardFuncs.h"
#include <stdio.h>

/*
	start = 0
	end = 0
	while start == 0 or input[end - 1] != 0
		if input[end] == ' ' or input[end] == 0
			for j = start, k = end - 1; j < k; j++, k--
				c = input[j]
				input[j] = input[k]
				input[k] = c
		start = end + 1

*/

#ifndef __unix__
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
	// for debug purposes
	CUniVar input = "test string";

	CUniVar start = 0;
	CUniVar end = 0;	
	while ((start == 0) || (input[end - 1] != 0))
	{
		if ((input[end] == ' ') || (input[end] == 0))
		{
			for (CUniVar j = start, k = end - 1; j < k; j++, k--)
			{
				CUniVar c = input[j];
				input[j] = input[k];
				input[k] = c;
			}
			start = end + 1;			
		}
		end++;
	}

	print(input);
	input += "\n";
	print(input);	

	return 0;
}
