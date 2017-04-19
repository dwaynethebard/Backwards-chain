#include <vector>       // std::vector
#include <string.h>		//std::string
#include <iostream>		//std::cout
#include <fstream>		// opening files
#include <stdio.h> 		//for strtok
#include <string>

bool solve(std::vector<std::vector<char>> rules,std::vector<char> &goals,std::vector<char> repeat);

int append(int i,std::vector<std::vector<char>> rules, std::vector<char> &goals, char current,std::vector<char> &repeat);

bool isAtom(char cur,std::vector<std::vector<char>> rules);

bool donebefore(char cur,std::vector<char> repeat);

int main(int argc, char* argv[])
{
	//although the nodes are number 1..n the index for the color and other list are 0..n-1. So the acess the color of node 4, look in spot 3

	typedef std::vector<char> rule_list_inside;//the list of nodes adjacent to the current node
	//adjacency list for the nodes 
  	std::vector<rule_list_inside> Rules;
  	std::vector<char> atoms; //list of atoms found
  	std::vector<char> goals; //queue of the goals
  	std::vector<char> stoprepeate; //keep track of what atoms we have put on goals list yet


	std::string line; //stores the line read in
	std::ifstream myfile (argv[1]);	//open the file sent 
	char query[1];
	if (myfile.is_open()) //if it opened properly
	{
		getline (myfile,line);
		char str[line.size()]; //create an array to parse later
		std::size_t length = line.copy(str,line.size(),0); //move line into char so i can convert properly
		char * pch;
		str[length]='\0';//put somthing in the last spot
		rule_list_inside temp;
		pch = strtok (str," ,.-");
		
		strcpy(query,pch); //put the querey we want in a list
		while ( getline (myfile,line) ) //while not at the end of the file
		{		
			char str[line.size()]; //create an array to parse later
			std::size_t length = line.copy(str,line.size(),0); //move line into char so i can convert properly
			char * pch;
			str[length]='\0';//put somthing in the last spot
			rule_list_inside temp;
			pch = strtok (str," ,.-");
				while (pch != NULL)
	  			{
	  				//Rules.push_back(pch);//put on the list
				    if(pch != NULL)
				    {
				    	char moo[1];
				    	strcpy(moo,pch);
				    	temp.push_back(moo[0]);//put on the list
				    }
				    pch = strtok (NULL, " ,.-");
	 			}
	 			Rules.push_back(temp);
		}
		myfile.close(); //close the file
	}

	std::cout<<"your query is: "<<query[0]<<std::endl;
	goals.push_back(query[0]);//put query on the goal list.

	
	if(solve(Rules,goals,stoprepeate))
	{
		std::cout<<"a solution exist\n";
		std::cout<<query[0]<<": is true\n";
	}
	else
	{
		std::cout<<"a solution does not exist\n";
		std::cout<<query[0]<<": is false\n";
	}
	
	return 0;
}


bool solve(std::vector<std::vector<char>> rules,std::vector<char> &goals,std::vector<char> repeat)
{
	if (goals.size()==0) //if goals queue is empty then we are done
	{
		return true;
	}

	char current_goal; //currently look for rules with head current_goal

	int added=0; //if we add any new atoms to the goal list
	current_goal=goals.back(); //take the last element of the goals, ie the atom of the rule we just looked at
	goals.pop_back();//take the last atom off list

	int length=rules.size();//how long the list it
std::cout<<"looking for rules with head :"<<current_goal<<std::endl;
	if (isAtom(current_goal,rules))//if the current_goal is an atom just move on
	{
		std::cout<<current_goal<<" is known to be true\n";
		if(solve(rules,goals,repeat))
		{
			return true;
		}
	}
	else
	{
		

		for (int i = 0; i < length; ++i)//go though the list of rules
		{
		    if (rules[i][0]==current_goal) //found a rule with head current
			{
				
				added=append(i,rules,goals,current_goal,repeat); //add to the queue list
				//if (added>0)
				//{
					if(solve(rules,goals,repeat))
					{
						std::cout<<current_goal<<" is true\n";
						return true;
					}
				//}

			}	
		}
		std::cout<<"This line of reasoning failed "<<current_goal<<std::endl;

	}
	return false;
}

int append(int i,std::vector<std::vector<char>> rules, std::vector<char> &goals, char current,std::vector<char> &repeat)
{
	//add all the atoms in the ith rule
	int length=rules[i].size(); //how many atoms are part of the rule
	int added=0;
	for (int j = 1; j < length; ++j) //don't want the head because thats what atom we were at
	{
		//if (donebefore(rules[i][j],repeat)==false)//if we havn't added the atom to the goals before
		//{
			std::cout<<"determined new atom :"<<rules[i][j]<<" from rule "<<current<<std::endl;
			goals.push_back(rules[i][j]);//add to goals
			repeat.push_back(rules[i][j]);//add to visited before list
			added=added+1;
		/*}
		else //if we have added the atom before
		{
			if (isAtom(rules[i][j],rules)) //it was an atom
			{
				std::cout<<rules[i][j]<<" is known to be true\n";
				goals.push_back(rules[i][j]);//add to goals
				added=added+1;
			}
			else
			{
				std::cout<<"All ready found and checked "<<rules[i][j]<<std::endl;
			}
			
		}*/
	}
	return added;
}

bool isAtom(char cur,std::vector<std::vector<char>> rules) //check if the rule is a fact.
{
	int length=rules.size(); //how many rules we have

	for (int i = 0; i < length; ++i) //go though the list of the rules
	{
		if (rules[i][0]==cur)//if the head of a rule is the same as the atom we are checking
		{
			if (rules[i].size()==1) //if the length is 1, then i=0 then it is a fact
			{
				return true; //the atom is a fact
			}
		}
	}
	return false; //the atom is not a fact
}

bool donebefore(char cur,std::vector<char> repeat) //keep track of what atoms we have done before
{
	int length=repeat.size();//how many rules we have

	for (int i = 0; i < length; ++i) //go though the list of the rules
	{
		if (repeat[i]==cur) //if the atom is on the list we have derivied the atom before
		{
			return true; //we have visited the atom before
		}
	}
	return false; //we have not visited the atom before
}
