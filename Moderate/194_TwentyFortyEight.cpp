#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
class Tile;

void ShiftVector(Tile (&Grid)[11][11], int8_t UpDown, int8_t RightLeft, int8_t Size);

class Tile
{
public:
    Tile() 
    {
        Value = 0;
        ActionPreformed = false;
    };
    
    ~Tile()
    {
        
    };
    
    void Set(uint16_t inValue)
    {
        Value = inValue;
        ActionPreformed = false;
    }
    
    bool Combine(Tile & OtherTile)
    {
        bool ReturnValue = false;
        if (OtherTile.Value == 0 && Value != 0)
        {
            OtherTile.Value = Value;
            OtherTile.ActionPreformed = ActionPreformed;
            Value = 0;
            ActionPreformed = false;
            ReturnValue = true;
        }
        else if (OtherTile.Value == Value && !ActionPreformed && !OtherTile.ActionPreformed && Value != 0)
        {
           OtherTile.Value = OtherTile.Value * 2;
           OtherTile.ActionPreformed = true;
           Value = 0;
           ActionPreformed = false;
           ReturnValue = true;
        }
        return ReturnValue;
    }
    uint16_t Value;
    bool ActionPreformed;
    
};


int main(int argc, char *argv[]) {
    ifstream stream(argv[1]);
    string line;
	Tile Grid[11][11];
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
				Grid[Index][InnerIndex].Set(Element);
			}
			pos = line.find("|");
	        string sElement = line.substr(0, pos);
	        uint16_t Element = atoi(sElement.c_str());
			line = line.substr(pos + 1);
			Grid[Index][Size - 1].Set(Element);
		}
		
		if (sDirection.c_str()[0] == 'U')
		{
			ShiftVector(Grid, -1, 0, Size);
		}
		else if (sDirection.c_str()[0] == 'D')
		{
			ShiftVector(Grid, 1, 0, Size);
		}
		else if (sDirection.c_str()[0] == 'R')
		{
			ShiftVector(Grid, 0, 1, Size);
		}
		else if (sDirection.c_str()[0] == 'L')
		{
			ShiftVector(Grid, 0, -1, Size);
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
				printf("%d", (Grid[Index])[InnerIndex].Value);
			}
			if(Index < Size - 1)
				printf("|");
		}
		printf("\n");
    }
    return 0;
};


void ShiftVector(Tile (&Grid)[11][11], int8_t UpDown, int8_t RightLeft, int8_t Size)
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
			    if(Grid[UDIndex][RLIndex].Combine(Grid[UDIndex + UpDown][RLIndex + RightLeft]))
			    {
			        ShiftVector(Grid, UpDown, RightLeft, Size);
			    }
			}
			
		}
	}
}