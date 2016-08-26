#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct MemoryStruct 
{
	char* memory;
	size_t size;
};

size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp);