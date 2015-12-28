#include "test.h"

#include <pds.h>
#define PDS_IMPL
#include <pds.h>

int main()
{
	const char *buffer = "test string with lowercase test string, uppercase TEST STRING and mixed-case test sTrIng.";

	struct
	{
		const char *first[2];
		const char *second[2];
		int expected;
	} tv[] = {
		{ { buffer   , buffer+10 }, { buffer+27, buffer+37 }, 1 }, 
		{ { buffer   , buffer+10 }, { buffer+50, buffer+60 }, 1 }, 
		{ { buffer+50, buffer+60 }, { buffer+77, buffer+87 }, 1 },
		{ { buffer   , buffer+25 }, { buffer+27, buffer+42 }, 0 },
		{ { buffer   , buffer+10 }, { buffer+27, buffer+50 }, 0 },
		{ { buffer+27, buffer+47 }, { buffer   , buffer+10 }, 0 },
		{ { 0, 0 } , { 0, 0 }, 0 }
	};
	
	int i;
	for(i=0; 0!=tv[i].first[0]; i++)
	{
		check(tv[i].expected == PDS_string_case_compare(tv[i].first[0], tv[i].first[1], tv[i].second[0], tv[i].second[1]));
	}	
    return EXIT_SUCCESS;
}
