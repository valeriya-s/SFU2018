#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <cmath>
#include <algorithm>

using namespace std;

struct Att_Class
{
	float att_val;
	string class_val;
};

struct rootnode
{
	vector<vector<Att_Class>> Each_Column;
};

struct GainValues
{
	float gain;
	float attribute_val;
	float entropy_less;
	float entropy_more;
};

GainValues Gain_Finder(vector<Att_Class> column, float compare, float EntropyIN)
{
	float lessYES=0;
	float lessNO=0;
	float moreYES=0;
	float moreNO=0;
	double P_less_yes=0;
	double P_less_no=0;
	double LOG_less_yes=0;
	double LOG_less_no=0;
	double ENTROPY_less=0;
	double P_more_yes=0;
	double P_more_no=0;
	double LOG_more_yes=0;
	double LOG_more_no=0;
	double ENTROPY_more=0;
	float gain;
	GainValues final_out;
	string class1;

	class1 = column[0].class_val;
	
	for (int i=0; i<column.size(); i++)		
	{
		//cout << column[i].att_val << " and " << compare << endl;
		if (column[i].att_val < compare)
		{
			if (column[i].class_val == class1)
			{
				lessYES=lessYES+1;
			}
			else
			{
				lessNO=lessNO+1;
			}
		}
		else
		{
			if (column[i].class_val == class1)
			{
				moreYES=moreYES+1;
			}
			else
			{
				moreNO=moreNO+1;
			}
		}
	}
	if ( lessNO == 0 )
	{
		lessNO = 0.00000001;
	}
	else if ( lessYES == 0 )
	{
		lessYES = 0.00000001;
	}
	else if ( moreNO == 0 )
	{
		moreNO = 0.00000001;
	}
	else if ( moreYES == 0 )
	{
		moreYES = 0.00000001;
	}
	P_less_no = lessNO/(lessYES + lessNO);
	P_less_yes = lessYES/(lessYES + lessNO);
	P_more_no = moreNO/(moreYES + moreNO);
	P_more_yes = moreYES/(moreYES + moreNO);
	
	if ( P_less_no == 0 )
	{
		P_less_no = 0.00000001;
	}
	else if ( P_less_yes == 0 )
	{
		P_less_yes = 0.00000001;
	}
	else if ( P_more_no == 0)
	{
		P_more_no = 0.00000001;
	}
	else if ( P_more_yes == 0 )
	{
		P_more_yes = 0.00000001;
	}

	LOG_less_no = log(P_less_no)/log(2);
	LOG_less_yes = log(P_less_yes)/log(2);	
	ENTROPY_less = (-1*P_less_no*LOG_less_no)-(P_less_yes*LOG_less_yes);
	LOG_more_no = log(P_more_no)/log(2);
	LOG_more_yes = log(P_more_yes)/log(2);
	ENTROPY_more = (-1*P_more_no*LOG_more_no)-(P_more_yes*LOG_more_yes);
	

		/*cout << "for less than or equal to " << compare << endl;
		cout << P_less_no << " " << P_less_yes << endl;
		cout << LOG_less_no << " " << LOG_less_yes << endl;
		cout << "for more than " << compare << endl;
		cout << P_more_no << " " << P_more_yes << endl;
		cout << ENTROPY_more << endl;*/
	
	gain = EntropyIN - (((lessYES+lessNO)/column.size())*ENTROPY_less)-(((moreYES+moreNO)/column.size())*ENTROPY_more);

	final_out.gain=gain;	
	final_out.attribute_val=compare;
	final_out.entropy_less=ENTROPY_less;	
	final_out.entropy_more=ENTROPY_more;

	return final_out;
};

vector<Att_Class> Vector_Maker(vector<vector<float>> matrix, int column, vector<string> classes)
{
	Att_Class each_val;
	vector<Att_Class> attributes;
	for (int i=0; i<classes.size(); i++)
	{
		float temp_att;
		temp_att = matrix[i][column];
		each_val.att_val = temp_att;
		each_val.class_val = classes[i];
		attributes.push_back(each_val);
	}

	return attributes;
};

GainValues Gain_Compare(vector<GainValues> allGains)
{
	GainValues check_att;
	GainValues largest_gain=GainValues();
	check_att.gain=0;
	//cout << "size: " << allGains.size() << endl;
	for (int i=0; i<allGains.size(); i++)
	{
		//cout << allGains[i].gain << endl;
		if (allGains[i].gain>check_att.gain)
		{
			largest_gain = allGains[i];
			check_att.gain=allGains[i].gain;
		}
	}

	return largest_gain;
};

//check if its the final leaf node:
bool LeafNode (vector<Att_Class> split_col)
{
	Att_Class classes;
	string checker;
	checker = split_col[0].class_val;
	bool done=0;

	for (int i=0; i<split_col.size(); i++)
	{
		if ( split_col[i].class_val != checker )
		{
			done=1;
		}
	}

	return done;
}

void nodes_makerLESS(vector<vector<Att_Class>> att_list, int column_to_split, vector<GainValues> largest_gain);

void nodes_makerMORE(vector<vector<Att_Class>> att_list, int column_to_split, vector<GainValues> largest_gain);

int main(int argc, char** argv)
{
	ifstream inputFile;
	rootnode firstnode;
	vector<string> classification;
	int counter_att=0;
	int width[10000]={'\0'};
	int counter_width=0;
	vector<vector<float>> attributes;
	char fuck;
	float Entropy_Class=0;
	float class1=0;
	float class2=0;
	float pofi1= 0;
	float pofi2= 0;
	float logval1= 0;
	float logval2= 0;
	vector<GainValues> attribute_gains;
	float checktemp=0;
	vector<GainValues> largest_gain;
	vector<vector<Att_Class> >attribute_list;
	int NUM_OF_ATTRIBUTES;
	int NUM_OF_CLASS;
	int column_to_split;
	GainValues largest_gain_temp;
	float cutoff;
	vector<int> less_rownum;
	vector<int> more_rownum;
	bool CheckLeaf=0;
	vector<vector<Att_Class>> less_att_list;
	vector<vector<Att_Class>> more_att_list;
	string trash;
	string file_name;
	bool impossible=1;
	vector<float> plsagain;
	
	file_name = argv[1];

	inputFile.open(file_name);

	if (inputFile.fail())
	{
		cerr << "ERROR: input file not open correctly";
		return 1;
	}

	while (!inputFile.eof())
	{
		if(inputFile.peek() == '/')
		{
			getline(inputFile,trash);
		}
		else
		{
		if (isdigit(inputFile.peek()))
		{
			float pls;
			inputFile >> pls;
			plsagain.push_back(pls);
			width[counter_width]=width[counter_width]+1;
			counter_att++;
		}
		else
		{
			inputFile >> fuck;
			//cout << fuck;
			if (isdigit(inputFile.peek()))
			{
				//inputFile.unget();
			}
			else
			{
				string temp;
				float pls;
				inputFile >> temp;
				classification.push_back(temp);
				counter_att=0;
				counter_width++;
				attributes.push_back(plsagain);
				plsagain.clear();
				inputFile >> pls;
				plsagain.push_back(pls);
				width[counter_width]=width[counter_width]+1;
				counter_att++;
			}
		}
		}
	}

	NUM_OF_ATTRIBUTES = width[0];
	NUM_OF_CLASS = classification.size();

	//find entropy for classification:
	
	for (int i=0; i<NUM_OF_CLASS; i++)
	{
		if (classification[i] == classification[0])
		{
			class1=class1+1;
		}
		else
		{
			class2=class2+1;
		}
	}
	
	pofi1 = (class1/(class1+class2));
	pofi2 = (class2/(class1+class2));
	logval1 = log(pofi1)/log(2);
	logval2 = log(pofi2)/log(2);
	
	Entropy_Class=-1*pofi1*logval1-pofi2*logval2;

	for (int i=0; i<NUM_OF_ATTRIBUTES; i++)
	{
		vector<Att_Class> tempvector;
		tempvector = Vector_Maker(attributes, i, classification);
		attribute_list.push_back(tempvector);
		//cout << "col : " << i << endl;
		for (int j=0; j<NUM_OF_CLASS; j++)
		{
			GainValues temp;
			temp = Gain_Finder(attribute_list[i],attribute_list[i][j].att_val, Entropy_Class);
			attribute_gains.push_back(temp);
			//cout << temp.gain << " ";
		}
		largest_gain_temp = Gain_Compare(attribute_gains);
		largest_gain.push_back(largest_gain_temp);
		largest_gain_temp=GainValues();
		attribute_gains.clear();
	}

	// this will create my first node :-) :
	firstnode.Each_Column = attribute_list;

	

	for (int i=0; i<NUM_OF_ATTRIBUTES; i++)
	{ 
		//cout << largest_gain[i].gain << endl;
		if ( largest_gain[i].gain>checktemp )
		{
			column_to_split = i;
			checktemp=largest_gain[i].gain;
		}
		if (largest_gain[i].gain != 0)
		{
			impossible=0;
		}
	}

	if (impossible)
	{
		cout << "Can't be split into anything else" << endl << endl;
		cout << "First Node:" << endl;
		for (int i=0; i<NUM_OF_CLASS; i++)
		{
			for (int j=0; j<NUM_OF_ATTRIBUTES; j++)
			{
				cout << firstnode.Each_Column[j][i].att_val << "	";
				if ( j == NUM_OF_ATTRIBUTES-1)
				{
					cout << firstnode.Each_Column[j][i].class_val << endl;
				}
			}
		}
		return 2;
	}

	cutoff = largest_gain[column_to_split].attribute_val;

	for (int i=0; i<NUM_OF_CLASS; i++)
	{
		if (attribute_list[column_to_split][i].att_val < cutoff )
		{
			less_rownum.push_back(i);
		}
		else
		{
			more_rownum.push_back(i);
		}
	}
	
	for (int i=0; i<NUM_OF_ATTRIBUTES; i++)
	{
		vector<Att_Class> temp_less_vector;
		vector<Att_Class> temp_more_vector;
		for (int j=0; j<less_rownum.size(); j++)
		{
			temp_less_vector.push_back(attribute_list[i][less_rownum[j]]);
		}
		for (int k=0; k<more_rownum.size(); k++)
		{
			temp_more_vector.push_back(attribute_list[i][more_rownum[k]]);
		}
		less_att_list.push_back(temp_less_vector);
		more_att_list.push_back(temp_more_vector);
		temp_less_vector.clear();
		temp_more_vector.clear();
	}		

	cout << "First Node:" << endl;
	for (int i=0; i<NUM_OF_CLASS; i++)
	{
		for (int j=0; j<NUM_OF_ATTRIBUTES; j++)
		{
			cout << firstnode.Each_Column[j][i].att_val << "	";
			if ( j == NUM_OF_ATTRIBUTES-1)
			{
				cout << firstnode.Each_Column[j][i].class_val << endl;
			}
		}
	}

	cout << endl << "Split on column #" << column_to_split+1 << endl << "Node with values bigger than or equal to cutoff value (" << cutoff << ") :" << endl;
	for (int i=0; i<more_att_list[0].size(); i++)
	{
		for (int j=0; j<NUM_OF_ATTRIBUTES; j++)
		{
			cout << more_att_list[j][i].att_val << "	";
			if ( j == NUM_OF_ATTRIBUTES-1)
			{
				cout << more_att_list[j][i].class_val << endl;
			}
		}
	}
	cout << endl << "Node with values smaller than cutoff value (" << cutoff << ") :" << endl;
	for (int i=0; i<less_att_list[0].size(); i++)
	{
		for (int j=0; j<NUM_OF_ATTRIBUTES; j++)
		{
			cout << less_att_list[j][i].att_val << "	";
			if ( j == NUM_OF_ATTRIBUTES-1)
			{
				cout << less_att_list[j][i].class_val << endl;
			}
		}
	}

	CheckLeaf = LeafNode(less_att_list[column_to_split]);
	if (CheckLeaf)
	{
		nodes_makerLESS(less_att_list,column_to_split,largest_gain);
	}
	CheckLeaf = 0;
	CheckLeaf = LeafNode(more_att_list[column_to_split]);
	if (CheckLeaf)
	{
		cout << endl << "Going back to first split node" << endl;
		nodes_makerMORE(more_att_list,column_to_split,largest_gain);
	}

	return 0;
}


void nodes_makerMORE(vector<vector<Att_Class>> att_list, int column_to_split, vector<GainValues> largest_gain)
{
	vector<GainValues> more_gains;
	GainValues largest_gain_temp;
	vector<GainValues> largest_gain_list;
	float checktemp = 0;
	int col2split;
	float cutoff;
	vector<int> less_rownum;
	vector<int> more_rownum;
	vector<vector<Att_Class>> less_att_listMORE;
	vector<vector<Att_Class>> more_att_listMORE;
	bool CheckLeaf=0;

	for (int i=0; i < att_list.size(); i++)
	{
		for (int j=0; j < att_list[i].size(); j++)
		{
			GainValues temp;
			temp = Gain_Finder(att_list[i],att_list[i][j].att_val, largest_gain[column_to_split].entropy_more);
			more_gains.push_back(temp);
		}
		largest_gain_temp = Gain_Compare(more_gains);
		largest_gain_list.push_back(largest_gain_temp);
		more_gains.clear();
		largest_gain_temp = GainValues();
	}	

	checktemp=0;
	for (int i=0; i<att_list.size(); i++)
	{ 
		if ( largest_gain_list[i].gain>checktemp )
		{
			col2split = i;
			checktemp=largest_gain_list[i].gain;
		}
	}

	cutoff = largest_gain_list[col2split].attribute_val;

	for (int i=0; i<att_list[0].size(); i++)
	{
		if (att_list[col2split][i].att_val < cutoff )
		{
			less_rownum.push_back(i);
		}
		else
		{
			more_rownum.push_back(i);
		}
	}

	for (int i=0; i<att_list.size(); i++)
	{
		vector<Att_Class> temp_less_vector;
		vector<Att_Class> temp_more_vector;
		for (int j=0; j<less_rownum.size(); j++)
		{
			temp_less_vector.push_back(att_list[i][less_rownum[j]]);
		}
		for (int k=0; k<more_rownum.size(); k++)
		{
			temp_more_vector.push_back(att_list[i][more_rownum[k]]);
		}
		less_att_listMORE.push_back(temp_less_vector);
		more_att_listMORE.push_back(temp_more_vector);
		temp_less_vector.clear();
		temp_more_vector.clear();
	}

	cout << endl << "Split from node with values bigger than previous cutoff:" << endl;
	cout << endl << "Split on column #" << col2split+1 << endl << "Node with values bigger than or equal to cutoff value (" << cutoff << ") :" << endl;
	for (int i=0; i<more_att_listMORE[0].size(); i++)
	{
		for (int j=0; j<more_att_listMORE.size(); j++)
		{
			cout << more_att_listMORE[j][i].att_val << "	";
			if ( j == more_att_listMORE.size()-1)
			{
				cout << more_att_listMORE[j][i].class_val << endl;
			}
		}
	}
	cout << endl << "Node with values smaller than cutoff value (" << cutoff << ") :" << endl;
	for (int i=0; i<less_att_listMORE[0].size(); i++)
	{
		for (int j=0; j<less_att_listMORE.size(); j++)
		{
			cout << less_att_listMORE[j][i].att_val << "	";
			if ( j == less_att_listMORE.size()-1)
			{
				cout << less_att_listMORE[j][i].class_val << endl;
			}
		}
	}

	CheckLeaf = LeafNode(less_att_listMORE[col2split]);
	if (CheckLeaf)
	{
		nodes_makerLESS(less_att_listMORE,col2split,largest_gain_list);
	}
	CheckLeaf = 0;
	CheckLeaf = LeafNode(more_att_listMORE[col2split]);
	if (CheckLeaf)
	{
		nodes_makerMORE(more_att_listMORE,col2split,largest_gain_list);
	}

};

void nodes_makerLESS(vector<vector<Att_Class>> att_list, int column_to_split, vector<GainValues> largest_gain)
{
	vector<GainValues> more_gains;
	GainValues largest_gain_temp;
	vector<GainValues> largest_gain_list;
	float checktemp = 0;
	int col2split;
	float cutoff;
	vector<int> less_rownum;
	vector<int> more_rownum;
	vector<vector<Att_Class>> less_att_listLESS;
	vector<vector<Att_Class>> more_att_listLESS;
	bool CheckLeaf=0;

	for (int i=0; i < att_list.size(); i++)
	{
		for (int j=0; j < att_list[i].size(); j++)
		{
			GainValues temp;
			temp = Gain_Finder(att_list[i],att_list[i][j].att_val, largest_gain[column_to_split].entropy_less);
			more_gains.push_back(temp);
		}
		largest_gain_temp = Gain_Compare(more_gains);
		largest_gain_list.push_back(largest_gain_temp);
		more_gains.clear();
		largest_gain_temp = GainValues();
	}	

	checktemp=0;
	for (int i=0; i<att_list.size(); i++)
	{ 
		if ( largest_gain_list[i].gain>checktemp )
		{
			col2split = i;
			checktemp=largest_gain_list[i].gain;
		}
	}

	cutoff = largest_gain_list[col2split].attribute_val;
	
	for (int i=0; i<att_list[0].size(); i++)
	{
		if (att_list[col2split][i].att_val < cutoff )
		{
			less_rownum.push_back(i);
		}
		else
		{
			more_rownum.push_back(i);
		}
	}

	for (int i=0; i<att_list.size(); i++)
	{
		vector<Att_Class> temp_less_vector;
		vector<Att_Class> temp_more_vector;
		for (int j=0; j<less_rownum.size(); j++)
		{
			temp_less_vector.push_back(att_list[i][less_rownum[j]]);
		}
		for (int k=0; k<more_rownum.size(); k++)
		{
			temp_more_vector.push_back(att_list[i][more_rownum[k]]);
		}
		less_att_listLESS.push_back(temp_less_vector);
		more_att_listLESS.push_back(temp_more_vector);
		temp_less_vector.clear();
		temp_more_vector.clear();
	}
	
	cout << endl << "Split from node with values smaller than previous cutoff:" << endl;
	cout << endl << "Split on column #" << col2split+1 << endl << "Node with values bigger than or equal to cutoff value (" << cutoff << ") :" << endl;
	for (int i=0; i<more_att_listLESS[0].size(); i++)
	{
		for (int j=0; j<more_att_listLESS.size(); j++)
		{
			cout << more_att_listLESS[j][i].att_val << "	";
			if ( j == more_att_listLESS.size()-1)
			{
				cout << more_att_listLESS[j][i].class_val << endl;
			}
		}
	}

	cout << endl << "Node with values smaller than cutoff value (" << cutoff << ") :" << endl;
	for (int i=0; i<less_att_listLESS[0].size(); i++)
	{
		for (int j=0; j<less_att_listLESS.size(); j++)
		{
			cout << less_att_listLESS[j][i].att_val << "	";
			if ( j == less_att_listLESS.size()-1)
			{
				cout << less_att_listLESS[j][i].class_val << endl;
			}
		}
	}
	
	CheckLeaf = LeafNode(less_att_listLESS[col2split]);
	if (CheckLeaf)
	{
		nodes_makerLESS(less_att_listLESS,col2split,largest_gain_list);
	}
	CheckLeaf = 0;
	CheckLeaf = LeafNode(more_att_listLESS[col2split]);
	if (CheckLeaf)
	{
		nodes_makerMORE(more_att_listLESS,col2split,largest_gain_list);
	}
};

