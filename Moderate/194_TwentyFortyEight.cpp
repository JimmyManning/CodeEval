#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;

void ShiftVector(uint16_t (&Grid)[11][11], int8_t UpDown, int8_t RightLeft, int8_t Size, bool Equals);
int main(int argc, char *argv[]) {
    ifstream stream(argv[1]);
    string line;
	uint16_t Grid[11][11];
    while (getline(stream, line)) {
        size_t pos = line.find(";");
        string sDirection = line.substr(0, pos);
        line = line.substr(pos + 1);
        pos = line.find(";");
        string sSize = line.substr(0, pos);
        int8_t Size = atoi(sSize.c_str());
		line = line.substr(pos + 1);
		
		pos = line.find(" ");
		line = line.substr(pos + 1);
		
		
		
		for (uint16_t Index = 0; Index < Size; Index++)
		{		
			for (uint16_t InnerIndex = 0; InnerIndex < Size - 1; InnerIndex++)
			{
				pos = line.find(" ");
		        string sElement = line.substr(0, pos);
		        uint16_t Element = atoi(sElement.c_str());
				line = line.substr(pos + 1);
				Grid[Index][InnerIndex] = Element;
			}
			pos = line.find("|");
	        string sElement = line.substr(0, pos);
	        uint16_t Element = atoi(sElement.c_str());
			line = line.substr(pos + 1);
			Grid[Index][Size - 1] = Element;
		}
		
		if (sDirection.c_str()[0] == 'U')
		{
			ShiftVector(Grid, -1, 0, Size, false);
			ShiftVector(Grid, -1, 0, Size, true);
			ShiftVector(Grid, -1, 0, Size, false);
		}
		else if (sDirection.c_str()[0] == 'D')
		{
			ShiftVector(Grid, 1, 0, Size, false);
			ShiftVector(Grid, 1, 0, Size, true);
			ShiftVector(Grid, 1, 0, Size, false);
		}
		else if (sDirection.c_str()[0] == 'R')
		{
			ShiftVector(Grid, 0, 1, Size, false);
			ShiftVector(Grid, 0, 1, Size, true);
			ShiftVector(Grid, 0, 1, Size, false);
		}
		else if (sDirection.c_str()[0] == 'L')
		{
			ShiftVector(Grid, 0, -1, Size, false);
			ShiftVector(Grid, 0, -1, Size, true);
			ShiftVector(Grid, 0, -1, Size, false);
		}
		else
		{
			// error
		}		
		
		for (uint16_t Index = 0; Index < Size; Index++)
		{
			for (uint16_t InnerIndex = 0; InnerIndex < Size; InnerIndex++)
			{
				if (InnerIndex != 0)
					printf(" ");
				printf("%d", (Grid[Index])[InnerIndex]);
			}
			if(Index < Size - 1)
				printf("|");
		}
		printf("\n");
    }
    return 0;
}


void ShiftVector(uint16_t (&Grid)[11][11], int8_t UpDown, int8_t RightLeft, int8_t Size, bool Equals)
{
	for (int8_t UDIndex = 0; UDIndex < Size; UDIndex++)
	{
		for(int8_t RLIndex = 0; RLIndex < Size; RLIndex++)
		{
			if (((RLIndex + RightLeft) >= 0) && 
				((RLIndex + RightLeft) < Size) &&
				((UDIndex + UpDown) >= 0) && 
				((UDIndex + UpDown) < Size))
			{
				
				if((Grid[UDIndex + UpDown])[RLIndex + RightLeft] == 0 && (Grid[UDIndex])[RLIndex] != 0)
				{
					(Grid[UDIndex + UpDown])[RLIndex + RightLeft] = (Grid[UDIndex])[RLIndex];
					(Grid[UDIndex])[RLIndex] = 0;
					ShiftVector(Grid, UpDown, RightLeft, Size, false);
				}
				else if (Equals && (Grid[UDIndex])[RLIndex] == (Grid[UDIndex + UpDown])[RLIndex + RightLeft] && (Grid[UDIndex])[RLIndex] != 0)
				{
					(Grid[UDIndex + UpDown])[RLIndex + RightLeft] = (Grid[UDIndex + UpDown])[RLIndex + RightLeft] * 2;
					(Grid[UDIndex])[RLIndex] = 0;
					ShiftVector(Grid, UpDown, RightLeft, Size, false);
				}
			}
			
		}
	}
}