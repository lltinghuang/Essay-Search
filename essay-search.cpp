#define FILE_EXTENSION ".txt"
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;
const int ALPHABET_SIZE = 26;
// Utility Func

// trie node
struct TrieNode
{
	struct TrieNode *children[ALPHABET_SIZE];
	// isEndOfWord is true if the node represents end of a word
	bool isEndOfWord;
};
// returns new trie node
struct TrieNode *getNode()
{
	struct TrieNode *pNode = new TrieNode;
	pNode->isEndOfWord = false;
	for (int i = 0; i < ALPHABET_SIZE; i++)
		pNode->children[i] = NULL;
	return pNode;
};
void insert(struct TrieNode *root, string key)
{
	struct TrieNode *pCrawl = root;
	for (int i = 0; i < key.length(); i++)
	{
		int index = tolower(key[i]) - 'a';
		if (!pCrawl->children[index])
			pCrawl->children[index] = getNode();
		pCrawl = pCrawl->children[index];
	}
	// mark last node as leaf
	pCrawl->isEndOfWord = true;
}
void insert_r(struct TrieNode *root, string key)
{
	struct TrieNode *p = root;
	for (int i = key.length() - 1; i >= 0; i--)
	{
		int index = tolower(key[i]) - 'a';
		if (!p->children[index])
			p->children[index] = getNode();
		p = p->children[index];
	}
	p->isEndOfWord = true;
}
bool exact(struct TrieNode *root, string key)
{
	struct TrieNode *p = root;
	for (int i = 0; i < key.length(); i++)
	{
		int index = tolower(key[i]) - 'a';
		if (!p->children[index])
			return false;

		p = p->children[index];
	}
	return (p->isEndOfWord);
}
bool prefix(struct TrieNode *root, string key)
{
	struct TrieNode *p = root;
	for (int i = 0; i < key.length(); i++)
	{
		int index = tolower(key[i]) - 'a';
		if (!p->children[index])
			return false;

		p = p->children[index];
	}
	return true;
}

bool search(struct TrieNode *root, struct TrieNode *suf, string key)
{
	if (key[0] == '*')
	{ // suffix
		string new_str;
		for (auto &ch : key)
		{
			if (isalpha(ch))
				new_str.push_back(ch);
		}
		reverse(new_str.begin(), new_str.end());
		return prefix(suf, new_str);
	}
	else if (key[0] == '"')
	{ // exact
		string new_str;
		for (auto &ch : key)
		{
			if (isalpha(ch))
				new_str.push_back(ch);
		}
		return exact(root, new_str);
	}
	else
	{ // prefix
		return prefix(root, key);
	}
}
// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string)
{
	vector<string> parse_string;
	for (auto &word : tmp_string)
	{
		string new_str;
		for (auto &ch : word)
		{
			if (isalpha(ch))
				new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string &str, const string &delim)
{
	vector<string> res;
	if ("" == str)
		return res;
	//先將要切割的字串從string型別轉換為char*型別
	char *strs = new char[str.length() + 1]; //不要忘了
	strcpy(strs, str.c_str());

	char *d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while (p)
	{
		string s = p;			//分割得到的字串轉換為string型別
		res.push_back(s); //存入結果陣列
		p = strtok(NULL, d);
	}

	return res;
}

int num = 0;
int main(int argc, char *argv[])
{

	// INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route
	clock_t start, end;
	start = clock();
	string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);

	// Read File & Parser Example

	string file, title_name, tmp;
	fstream fi, fj;
	vector<string> tmp_string;
	vector<string> title_table;
	vector<TrieNode *> root, suf_root;
	// from data_dir get file ....
	// eg : use 0.txt in data directory
	int i = 0;
	int filenumber = 0;
	string filename;
	while (i >= 0)
	{
		filename = data_dir + to_string(i) + ".txt";
		// cout << filename << '\n';
		fi.open(filename, ios::in);
		if (!fi.is_open())
		{
			filenumber = i;
			// cout << "i is " << i << '\n';
			break;
		}
		// GET TITLENAME
		getline(fi, title_name);
		title_table.push_back(title_name);
		// GET TITLENAME WORD ARRAY
		tmp_string = split(title_name, " ");
		vector<string> title = word_parse(tmp_string);
		struct TrieNode *p = getNode();
		struct TrieNode *r = getNode();
		for (auto &word : title)
		{
			insert(p, word);
			insert_r(r, word);
			// cout << word << endl;
		}
		// GET CONTENT LINE BY LINE
		while (getline(fi, tmp))
		{
			// GET CONTENT WORD VECTOR
			tmp_string = split(tmp, " ");

			// PARSE CONTENT
			vector<string> content = word_parse(tmp_string);

			for (auto &word : content)
			{
				insert(p, word);
				insert_r(r, word);
				// cout << word << endl;
			}
			//......
		}
		// CLOSE FILE
		fi.close();
		i++;
		root.push_back(p);
		suf_root.push_back(r);
	}

	// cout << "root has " << root.size() << '\n';
	fi.open(query, ios::in);
	fj.open(output, ios::out);
	while (getline(fi, tmp))
	{
		tmp_string = split(tmp, " ");
		vector<int> output_set;
		bool ans;
		for (int j = 0; j < filenumber; j++)
		{
			int flag = 0;
			for (auto &word : tmp_string)
			{
				if (word[0] == '+')
					flag = 1;
				else if (word[0] == '/')
					flag = 2;
				else
				{
					if (flag == 0)
					{
						ans = search(root[j], suf_root[j], word);
					}
					else if (flag == 1)
					{ // and
						// cout << "word = " << word << '\n';
						ans = ans && search(root[j], suf_root[j], word);
					}
					else // or
						ans = ans || search(root[j], suf_root[j], word);
				}
			}
			if (ans)
			{
				output_set.push_back(j);
				// cout << j << '\n';
			}
		}
		for (auto &word : output_set)
		{
			fj << title_table[word] << "\n";
			// cout << title_table[word] << "\n";
			num++;
		}
		if (output_set.empty())
			fj << "Not Found!\n";
		output_set.clear();
		// cout << "------------------\n";
		// fj << "------------------\n";
	}
	fj.close();
	fi.close();

	end = clock();
	cout << (double)((double)(end-start)/CLOCKS_PER_SEC) << '\n';
}

// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
