#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <deque>
using namespace std;

class MemoryMap;
class Op;
class OpFactory;
void CreateObjects(string & Code, MemoryMap & Memory, vector<Op*> & OutMyOps, OpFactory & Factory);

class Op
{
public:
	Op(MemoryMap & Memory)
    : Memory(Memory)
	{
		
	}
	virtual ~Op() {};
	
	virtual void Execute() = 0;	
	virtual void PrintName() = 0;
	
protected:
	MemoryMap & Memory;	
};


class MemoryMap
{
public:	
	MemoryMap()
	: Address(0) 
	{
		Heap.push_back(0);
	};
	
	~MemoryMap() 
	{
		
	};
	
	void Increment()
	{
		Heap[Address]++;
	}
	
	void Decrement()
	{
		Heap[Address]--;
	}
	
	void Next()
	{
		Address++;
		if (Heap.size() == Address)
			Heap.push_back(0);
	}
	
	void Previous()
	{
		if (Address == 0)
		{
			Heap.push_front(0);
		}
		else
		{
			Address--;
		}
	}
	
	char GetValue()
	{
		return Heap[Address];
	}
	
	void Print()
	{
		printf("%c", Heap[Address]);
	}

protected:
	uint16_t Address;
	deque<char> Heap;
	
	
};


class Loop : public Op
{
public:	
	Loop(string & Code, MemoryMap & Memory, OpFactory & Factory)
		: Op(Memory)
	{
		CreateObjects(Code, Memory, MyOps, Factory);
	}
	
	void Execute()
	{
		if (Memory.GetValue() != 0)
		{
			do
			{
				for (uint16_t Index = 0; Index < MyOps.size(); Index++)
				{
					MyOps[Index]->Execute();
				}
			} while(Memory.GetValue() != 0);
		}
	}
	
	void PrintName()
	{
		printf("Loop\n");
		for (uint16_t Index = 0; Index < MyOps.size(); Index++)
		{
			printf(" ");
			MyOps[Index]->PrintName();
		}
	}
	
	virtual ~Loop() 
	{
		// for (uint16_t Index = 0; Index < MyOps.size(); Index++)
		// {
		// 	delete MyOps[Index];
		// }
	};
	
	
protected:
	vector<Op*> MyOps;
};


class Increment : public Op
{
public: 
	Increment(MemoryMap & Memory) 
		: Op(Memory)
	{
		
	}
	
	void Execute()
	{
		Memory.Increment();
	}
	
	void PrintName()
	{
		printf("Increment\n");
	}
};


class Decrement : public Op
{
public: 
	Decrement(MemoryMap & Memory) 
		: Op(Memory)
	{
		
	}
	
	void Execute()
	{
		Memory.Decrement();
	}
	
	void PrintName()
	{
		printf("Decrement\n");
	}
};


class Next : public Op
{
public: 
	Next(MemoryMap & Memory) 
		: Op(Memory)
	{
		
	}
	
	void Execute()
	{
		Memory.Next();
	}
	
	void PrintName()
	{
		printf("Next\n");
	}
};


class Previous : public Op
{
public: 
	Previous(MemoryMap & Memory) 
		: Op(Memory)
	{
		
	}
	
	void Execute()
	{
		Memory.Previous();
	}
	
	void PrintName()
	{
		printf("Previous\n");
	}
};


class Print : public Op
{
public: 
	Print(MemoryMap & Memory) 
		: Op(Memory)
	{
		
	}
	
	void Execute()
	{
		Memory.Print();
	}
	
	void PrintName()
	{
		printf("Print\n");
	}
};


class Input : public Op
{
public: 
	Input(MemoryMap & Memory) 
		: Op(Memory)
	{
		
	}
	
	void Execute()
	{
		// Don't know what to do here
	}
	
	void PrintName()
	{
		printf("Input\n");
	}
};

class OpFactory
{
public:	
	OpFactory(MemoryMap & Memory) 
		: Memory(Memory)
		, mNext(Memory)
		, mPrevious(Memory)
		, mIncrement(Memory)
		, mDecrement(Memory)
		, mPrint(Memory)
	{};
	~OpFactory() 
	{
		for(uint16_t Index = 0; Index < OpsToDelete.size(); Index++)
		{
			delete OpsToDelete[Index];
		}
	};
	
	Next * GetNext() {return &mNext;};
	Previous * GetPrevious() {return &mPrevious;};
	Increment * GetIncrement() {return &mIncrement;};
	Decrement * GetDecrement() {return &mDecrement;};
	Print * GetPrint() {return &mPrint;};
	Op * GetLoop(string & Code) 
	{
		OpsToDelete.push_back(new Loop(Code, Memory, *this));
		return OpsToDelete.back();
	};
	
protected: 
	MemoryMap & Memory;
	
	Next mNext;
	Previous mPrevious;
	Increment mIncrement;
	Decrement mDecrement;
	Print mPrint;
	
	vector<Op *> OpsToDelete;
	
	
};



void CreateObjects(string & Code, MemoryMap & Memory, vector<Op*> & OutMyOps, OpFactory & Factory)
{
	while (Code.length() > 0)
	{
		char CurrentOp = Code.c_str()[0];
		Code = Code.substr(1);
		
		switch (CurrentOp)
		{
			case '>':
				OutMyOps.push_back(Factory.GetNext());
			break;
			case '<':
				OutMyOps.push_back(Factory.GetPrevious());
			break;
			case '+':
				OutMyOps.push_back(Factory.GetIncrement());
			break;
			case '-':
				OutMyOps.push_back(Factory.GetDecrement());
			break;
			case '.':
				OutMyOps.push_back(Factory.GetPrint());
			break;
			case ',':
				//OutMyOps.push_back(Factory.Get Input(Memory));
			break;
			case '[':
				OutMyOps.push_back(Factory.GetLoop(Code));
			break;
			case ']':
				return;
			break;
		}
	}
	
}


int main(int argc, char *argv[]) {
    ifstream stream(argv[1]);
    string line;

    while (getline(stream, line)) 
	{	
		MemoryMap Memory;
		OpFactory Factory(Memory);
		vector<Op*> MyOps;
		CreateObjects(line, Memory, MyOps, Factory);
		
		for (uint16_t Index = 0; Index < MyOps.size(); Index++)
			MyOps[Index]->Execute();
		
		//for (uint16_t Index = 0; Index < MyOps.size(); Index++)
		//	delete MyOps[Index];
		printf("\n");
    }
    return 0;
};