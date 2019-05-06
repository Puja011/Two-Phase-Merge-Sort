#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <map>
using namespace std;
inline bool is_file (const std::string& name) {
	  struct stat buffer;   
	    return (stat (name.c_str(), &buffer) == 0); 
}

int asc,val,i,tuple_size=0,file_size,j,number,record_number,lengths[10005],flag[10005]={0};
int num_in_sub,no_of_records,total_no_of_records,num_of_sub,present_count,new_tuplesize=0;
float mainmemory_size;

ifstream file[1005];

vector<int> order;
string key,value,text;
map<string,int> meta;
vector<string> req_col,given_col,extend_req_col;
vector<string> ::iterator it;
vector< vector<string> > inp;

string change_to_string(vector<string> s)
{
	int i;
	string myst;
	for(i=0;i<req_col.size();i++)
	{
		for(j=0;j<given_col.size();j++)
		{
			if(strcmp(req_col[i].c_str(),given_col[j].c_str())==0)
			{
				myst.append(s[j]);
				myst.append("  ");
			}
		}
	}
	myst.append(s[s.size()-1]);
	return myst;
}
void print_inp()
{
	for(i=0;i<inp.size();i++)
	{
		for(j=0;j<inp[i].size();j++)
		{
			cout<<inp[i][j]<<" ";
		}
		cout<<endl;
	}
}
vector<int> func(vector< vector<string> > inp,int number)
{
	int i,j;
	vector<string> p;
	string s;
	for(i=0;i<inp.size();i++)
	{
		s=change_to_string(inp[i]);
		p.push_back(s);
	}
	sort(p.begin(),p.end());
	vector<int> num;
	for(i=0;i<p.size();i++)
	{
		string a="";
		for(j=new_tuplesize-2;j<p[i].length();j++)
		{
			a=a+p[i][j];
		}
		num.push_back(atoi(a.c_str()));
	}

	return num;
}
void combining(string output)
{
	char texts[100][1000];
	int f,pos[1000]={0},le=0;
	string tex;

	for(i=1;i<=number;i++)
	{
		string filename;
		char s1[10];
		sprintf(s1, "%d", i);
		filename.append(s1);
		filename.append(".txt");
	FILE *fd = fopen(filename.c_str(), "r");
    if (fd == NULL) {
        printf("Failed to open file\n");
        // return -1;
    }
    int end, loop, line=1;
    pos[i]=1;
    char str[512]={'\0'};

    for(end = loop = 0;loop<line;++loop){
        if(0==fgets(str, sizeof(str), fd)){
            end = 1;//can't input (EOF)
            break;
        }
    }
    if(!end)
    {
    	texts[i][0]='\0';
        strcpy(texts[i],str);
    }

    fclose(fd);

	}
	while(1)
	{
		int mini=-1;
		char minst[1000]={'\0'};
		for(i=1;i<=number;i++)
		{
			if(flag[i]==0)
			{
				if(mini==-1)
				{
					mini=i;
					strcpy(minst,texts[i]);
				}
				else
				{
					if(strcmp(texts[i],minst)<0)
					{
						strcpy(minst,texts[i]);
						mini=i;
					}
				}
			}
		}
		if(mini==-1)
			break;
		ofstream myfile;
  		myfile.open(output.c_str(),std::ios_base::app);
  		myfile<<texts[mini];
  		le++;
  		if(le==record_number)
  			break;
  		myfile.close();
		if(pos[mini]==lengths[mini] || flag[mini]==1)
		{
			flag[mini]=1;
			continue;
		}
		string filename;
		char s1[10];
		sprintf(s1, "%d", mini);
		filename.append(s1);
		filename.append(".txt");
	FILE *fd = fopen(filename.c_str(), "r");
    if (fd == NULL) {
        printf("Failed to open file\n");
        // return -1;
    }
    int end, loop, line;
    pos[mini]++;
    line=pos[mini];
    char str[512]={'\0'};

    for(end = loop = 0;loop<line;++loop){
        if(0==fgets(str, sizeof(str), fd)){
            end = 1;//can't input (EOF)
            break;
        }
    }
    if(!end)
    {
    	texts[i][0]='\0';
        strcpy(texts[mini],str);
    }

    fclose(fd);
	}

}


int main(int argc,char *argv[])
{


	std::ios::sync_with_stdio(false);

	if(argc-1<5)
	{
		cout<<"ERROR : improper input\n";
		return 0;
	}
	else if(!is_file(argv[1]))
	{
		cout<<"ERROR : input.txt is missing\n";
		return 0;
	}
	else if(!is_file("metadata.txt"))
	{
		cout<<"ERROR : metadata.txt is missing\n";
		return 0;
	}
	else if(strcmp(argv[4],"asc")==0)
		asc=1;
	else if(strcmp(argv[4],"desc")==0)
		asc=0;
	else if(strcmp(argv[4],"asc")!=0 && strcmp(argv[4],"desc")!=0)
	{
		cout<<"ERROR : need to write asc or desc only\n";
		return 0;
	}

	ifstream file;
	file.open("metadata.txt",ios::in);

	while(file>>text)
	{
		stringstream ss(text);
		getline(ss,key,',');
		getline(ss,value,',');
		value = value.c_str();
		stringstream geek(value);
		geek>>val;
		tuple_size+=val+2;
		given_col.push_back(key);
		meta.insert(make_pair(key,val));
	}
	tuple_size-=2;
	file.close();

	for(i=5;i<argc;i++)
	{
		if(meta.find(argv[i])==meta.end())
		{
			cout<<"Column :"<<argv[i]<<" Doesn't exist\n";
			return 0;
		}
	}

	for(i=5;i<argc;i++)
	{
		req_col.push_back(argv[i]);
		new_tuplesize+=meta[argv[i]]+2;
	}
	new_tuplesize+=2;

	struct stat filestatus;
	stat( argv[1], &filestatus );
	file_size=filestatus.st_size;
	mainmemory_size=atof(argv[3])*1024*1024;

	no_of_records=(int)(float(mainmemory_size)/float(tuple_size));

	total_no_of_records=0;
	file.open(argv[1],ios::in);
	while(getline(file,text))
	{
		total_no_of_records++;
	}
	file.close();

	if((int)mainmemory_size >= (tuple_size+1)*total_no_of_records)
	{
		cout<<"Two way merge sort not required\n";
		return 0;
	}
	
	else if((int)(float)file_size/(float)mainmemory_size * tuple_size > (int)mainmemory_size)
	{
		cout<<"Two way merge sort not possible\n";
		return 0;
	}

	num_of_sub= file_size / mainmemory_size+1;
	num_in_sub= mainmemory_size/tuple_size;

	file.open(argv[1],ios::in);
	char *token=NULL;
	string input;
	
	present_count=0;
	number=0;
	record_number=0;
	string delimiter="  ";
	while(getline(file,text))
	{
		int variable=0;
		string s;
		vector<string> v;
		present_count++;
		record_number++;
		text=text.c_str();
		for(i=0;i<given_col.size();i++)
		{
			s="";
			val=meta[given_col[i]];
			for(j=0;j<val;j++)
			{
				s=s+text[variable];
				variable++;
			}
			v.push_back(s);
			variable+=2;
		}
		char sr[10];
		sprintf(sr, "%d", record_number);
		v.push_back(sr);
		inp.push_back(v);
		if(present_count==num_in_sub)
		{
			number++;
			order=func(inp,number);
			lengths[number]=num_in_sub;
			string filename;
			char s1[10];
			sprintf(s1, "%d", number);
			filename.append(s1);
			filename.append(".txt");

			ofstream fp;
			fp.open(filename.c_str(),ios::out);
			int l;
			l=(number-1)*num_in_sub+1;
			for(j=0;j<order.size();j++)
			{
				string myst="";
				for(i=0;i<inp[order[j]-l].size();i++)
				{
					if(i!=inp[order[j]-l].size()-1)
						myst=myst+inp[order[j]-l][i]+" ";
					else
						myst=myst+inp[order[j]-l][i];
				}
				fp<<myst<<endl;
			}
			fp.close();
			order.clear();
			inp.clear();
			present_count=0;
		}
		v.clear();
	}
	if(present_count>0)
	{
		number++;
		lengths[number]=present_count;
		order=func(inp,number);
		char s1[10];
		string filename;
		sprintf(s1, "%d", number);
		filename.append(s1);
		filename.append(".txt");

		ofstream fp;
		fp.open(filename.c_str(),ios::out);
		int l;
		l=(number-1)*num_in_sub+1;
		for(j=0;j<order.size();j++)
		{
			string myst="";
			for(i=0;i<inp[order[j]-l].size();i++)
			{
				if(i!=inp[order[j]-l].size()-1)
					myst=myst+inp[order[j]-l][i]+" ";
				else
					myst=myst+inp[order[j]-l][i];
			}
			fp<<myst<<endl;
		}
		fp.close();
	}
	file.close();
	combining(argv[2]);
	for(i=1;i<=number;i++)
	{
		string filename;
		char s1[10]={'\0'};
		sprintf(s1, "%d", i);
		filename.append(s1);
		filename.append(".txt");
		remove(filename.c_str());
	}
	return 0;
}

