/*
* Time : 2022/12/13
* Author : YU.J.P
* Project : Virtual Decument System
*
*/

/********** BEGIN **************************************************************************************************/
/********** OVER ***************************************************************************************************/

/********** INCLUDE BIGIN ******************************************************************************************/

#include<iostream>
#include<algorithm>
#include<string.h>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<cmath>
#include<stack>
#include<queue>
#include<stdio.h>
#include<fstream>
#include<iomanip>
using namespace std;

/********** INCLUDE OVER *******************************************************************************************/

/********** DEFINE BEGIN *******************************************************************************************/

vector<string> records;
int reco = 0;

struct user {			// �û�����
	string name;		// �û���
	string password;	// ����
};

struct file {				// �ļ�����
	string name;			// �ļ���
	long bits;				// �ļ���С
	vector<string> content;	// �ļ�����
};

struct dir {					// Ŀ¼����
	string name;				// Ŀ¼��
	dir* pre;					// ��Ŀ¼
	map<string, file*> files;	// ���������ļ�
	map<string, dir*> next;		// ��Ŀ¼
};

user curuser;	// ��ǰ�û�
dir* curdir;	// ��ǰĿ¼
dir* root;		// ��Ŀ¼

/********** DEFINE OVER ********************************************************************************************/

/********** USER FUNCTION DECLARATION BEGIN ************************************************************************/

void initDir();						/* ��ʼ��Ŀ¼ */
void displayPath();					/* չʾ·�� */
dir* reCreate(dir* last);			/* ��ԭ�û��ر�ǰ��״̬ */
void initVds();						/* ��ʼ������ */
void login();						/* ��¼ϵͳ */
bool nameCheak(string name);		/* ��������Ƿ�Ϸ� */
vector<string> split(string str);	/* ����'/'����ַ��� */
dir* pathTrans(string path);		/* ��ַת�� */
void help();						/* ����������� */
void help2(string command);			/* ����������� */
void cls();							/* ���� clear ���� */
void mkdir(string name);			/* ���� mkdir */
void cd(string name);				/* ���� cd */
void ls(string path);				/* ���� ls */
void mkfile(string name);			/* ���� touch �½��ļ� */
void edit(string name);				/* ���� edit �༭�ļ� */
void show(string name);				/* ���� show ��ʾ�ļ����� */
void rename(string temp);			/* ���� rename ������ */
void deletedir(dir* cur);			/* �ݹ�ɾ��Ŀ¼ */
void rm(string temp);				/* ���� rm ɾ�� */
dir* cpDir(dir* temp);				/* �ݹ鸴��Ŀ¼ */
void cp(string temp);				/* ���� cp ���� */
void su();							/* �л��û� */
void reflash();						/* �����û���Ϣ */
void logout();						/* �û�ע�� */
void save(dir* temp);				/* �洢Ŀǰ��Ϣ */
void exit();						/* ������Ϣ �˳�ϵͳ */

/********** USER FUNCTION DECLARATION END **************************************************************************/

/********** USER CODE BEGIN ****************************************************************************************/

/* ��ʼ��Ŀ¼ */
void initDir() {
	curdir = new dir();
	curdir->name = "~";
	curdir->pre = NULL;
	root = curdir;
}

/* չʾ·�� */
void displayPath() {
	dir* temp = curdir;
	vector<string> path;
	while (temp != NULL) {
		path.push_back(temp->name);
		temp = temp->pre;
	}
	cout << "~";
	for (int i = path.size() - 2; i >= 0; i--) {
		cout << "/" << path[i];
	}
	cout << "$ ";
}


/* ��ԭ�û��ر�ǰ��״̬ */
dir* reCreate(dir* last) {
	dir* temp_dir = new dir();  // �½�Ŀ¼
	temp_dir->name = records[reco++];  // Ŀ¼��
	temp_dir->pre = last;  // ���ø�ָ��
	string temp_count;
	temp_count = records[reco++];  // �ļ���
	for (int i = 0; i < stoi(temp_count); i++) {
		file* temp_file = new file();  // �½��ļ�
		temp_file->name = records[reco++];  // �ļ���
		temp_file->bits = stoi(records[reco++]);  // �ļ���С
		while (1) {
			string temp_str;
			temp_str = records[reco++];  // �ļ�����
			if (temp_str != "content") {  // �������� �ؼ��ַָ�
				temp_file->content.push_back(temp_str);
			}
			else {
				break;
			}
		}
		temp_dir->files[temp_file->name] = temp_file;  // ���½����ļ�����Ŀ¼
	}
	temp_count = records[reco++];  // ��Ŀ¼��
	for (int i = 0; i < stoi(temp_count); i++) {
		string name;
		name = records[reco++];  // ��Ŀ¼��
		temp_dir->next[name] = reCreate(temp_dir);  // �ݹ��½���Ŀ¼
	}
	return temp_dir;
}

/* ��ʼ������ */
void initVds() {
	ifstream file_in(curuser.name + ".dat");  // �����û��ļ���Ϣ
	string temp;
	if (!file_in) {
		initDir();  // ���û��³�ʼ��Ŀ¼
	}
	while (getline(file_in, temp)) {
		records.push_back(temp);
	}

	if (records.size() >= 1) {  // ����ϵͳ��Ϣ
		root = curdir = reCreate(NULL);
	}
	else {
		initDir();  // ���û��³�ʼ��Ŀ¼
	}
}

/* ��¼ϵͳ */
void login() {
	map<string, string> users;
	bool flag = 1;  // �û���ƥ���ж����

	system("cls");
	cout << "This is Virtual Decument System v2.0" << endl;
	cout << "Author: YU.J.P" << endl;
	
	ifstream file_in("user.dat");
	if (!file_in) {
		cout << "Error on oponing user.dat��" << endl;
		system("Pause");
		exit(1);
	}

	string temp_name, temp_pawd;
	// ���ش��ڵ��û���Ϣ
	while (file_in >> temp_name >> temp_pawd) {
		users[temp_name] = temp_pawd;
	}
	file_in.close();

	cout << "Login..." << endl;
	cout << "username:";
	cin >> temp_name;
	while (flag) {
		cout << "password:";
		cin >> temp_pawd;
		if (users.find(temp_name) == users.end()) {
			flag = 0;  // û�ҵ�
		}
		else {	// �ҵ�
			if (users[temp_name] == temp_pawd) {
				system("cls");
				curuser.name = temp_name;
				curuser.password = temp_pawd;

				// ������û�
				users[temp_name] = temp_pawd;

				break;  // ƥ��ɹ� �˳�ѭ��
			}
			else {
				cout << "password is incorrect!\n";
			}
		}
	}
	if (!flag) {  // δƥ��ɹ� �����û�
		cout << "This user is not exist.\nDo you want to creat a new user?(y/n):";
		char choice;
		cin >> choice;
		if (choice == 'Y' || choice == 'y') {
			users[temp_name] = temp_pawd;
			curuser.name = temp_name;
			curuser.password = temp_pawd;
			// ����һ�����ļ�
			ofstream outfile(temp_name + ".dat", ios::app);
			if (outfile.is_open()) {
				cout << "File already exists." << endl;
				outfile.close();
			}
			system("cls");
		}
		else {  // ��ͬ�ⴴ�� ��������
			login();  // �ݹ����
		}
	}

	// �洢�û���Ϣ
	ofstream file_out("user.dat");
	for (auto it = users.begin(); it != users.end(); it++) {
		file_out << it->first << " " << it->second << endl;
	}
	getchar();
	return;
}

/* ��������Ƿ�Ϸ� */
bool nameCheak(string name) {
	if (name.find("\\") != name.npos || name.find("/") != name.npos || 
		name.find(":") != name.npos || name.find("*") != name.npos || 
		name.find("?") != name.npos || name.find("<") != name.npos || 
		name.find(">") != name.npos || name.find("|") != name.npos || 
		name.find("\"") != name.npos || name.find("..") != name.npos || 
		name.find(" ") != name.npos) 
		return false;
	return true;
}

/* ����'/'����ַ��� */
vector<string> split(string str) {
	vector<string> ans;
	string temp = "";
	for (int i = 0; i < str.length(); i++) {
		if (str[i] != '/') {
			temp = temp + str[i];
		}
		else {
			ans.push_back(temp);
			temp = "";
		}
	}
	if (temp != "")ans.push_back(temp);
	return ans;
}

/* ��ַת�� */
dir* pathTrans(string path) {
	// ����·��
	if (path[0] == '~' || path[0] == '/') {
		dir* cur_dir = root;
		if (path[0] == '/') path = "~" + path;
		vector<string> temp_path = split(path);
		for (int i = 1; i < temp_path.size(); i++) {
			if (cur_dir->next.find(temp_path[i]) == cur_dir->next.end()) {
				return NULL;  // û�ҵ�
			}
			cur_dir = cur_dir->next[temp_path[i]];
		}
		return cur_dir;
	}
	// ���·��
	else {
		dir* cur_dir = curdir;
		vector<string> temp_path = split(path);
		for (int i = 0; i < temp_path.size(); i++) {
			if (temp_path[i] == ".") {
				continue;
			}
			else if (temp_path[i] == "..") {
				if (cur_dir == root) {  // ��������������
					return NULL;
				}
				else {
					cur_dir = cur_dir->pre;
				}
			}
			else if (cur_dir->next.find(temp_path[i]) == cur_dir->next.end()) {
				return NULL;  // û�ҵ�
			}
			else if (cur_dir->next.find(temp_path[i]) != cur_dir->next.end()) {
				cur_dir = cur_dir->next[temp_path[i]];
			}
		}
		return cur_dir;
	}
	return NULL;
}

/* ����������� */
void help() {
	cout << "command: \n\
	help   ---  show help menu.\n\
	cd     ---  change the shell working directory.\n\
	clear  ---  clear the screen.\n\
	ls     ---  show all the files and directories in particular directory.\n\
	mkdir  ---  create the directory,if it does not already exist.\n\
	mkfile ---  create a new file.\n\
	edit   ---  read and write something to a particular file.\n\
	show   ---  Read and print content of this file.\n\
	rm     ---  delete a exist file or a directory.\n\
	cp     ---  copy SOURSE to DEST.\n\
	rename ---  rename a exist file or a directory.\n\
	su     ---  switch current user.\n\
	exit   ---  exit this system.\n\
	logout ---  user logout.\n";
}

/* ����������� */
void help2(string command) {
	if (command == "cd?") cout << "cd: cd [dir]\nchange the shell working directory.\n";
	else if (command == "ls?") cout << "ls: ls [dir]\nShow all the files and directories in particular directory.\n";
	else if (command == "mkdir?") cout << "mkdir: mkdir dir-name\nCreate the directory,if it does not already exist.\n";
	else if (command == "mkfile?") cout << "mkfile: mkfile file-name\nCreate a new file,if it does not already exist.\n";
	else if (command == "edit?") cout << "edit: edit file-name\nRead and write something to a particular file.\n";
	else if (command == "show") cout << "cat: cat file-name\nRead and print content of this file.\n";
	else if (command == "rm?") cout << "rm: rm -d|-f file-name|dir\nDelete a exist file or a directory.\noption:\n-d:delete a directory\n-f:delete a file\n";
	else if (command == "cp?") cout << "cp: cp -d|-f|-cd|-cf SOURSE DEST\nCopy SOURSE to DEST\noption:\n-d:copy a directory\n-f:copy a file\n-cd:cut a directory\n-cf:cut a file\n";
	else if (command == "rename?") cout << "rename: rename -d|-f oldname newname\nRename a exist file or a directory.\noption:\n-d:rename a directory\n-f:rename a file\n";
	else if (command == "su?") cout << "Switch current user.\n";
	else if (command == "cls/clear?") cout << "Clear the screen.\n";
	else if (command == "exit?") cout << "Exit this system.\n";
	else if (command == "logout") cout << "User logout.\n";
	//else printf("Wrong Command!\n");
}

/* ���� clear ���� */
void cls() {
	system("cls");
}

/* ���� mkdir */
void mkdir(string name) {
	if (name == "") {
		help2("mkdir?");
	}
	else if (curdir->next.find(name) != curdir->next.end()) {
		cout << "There is a directory having the same name.\n";
	}
	else if (!nameCheak(name)) {
		cout << "Directory name cannot contain the following characters:\n"
			 << "'\',' / ',' | ',':','; ','<','>',' ? ',\"..\"" << endl;
	}
	else {
		dir* temp = new dir();//һ��Ҫ���������������ַ�������Ͳ��ܶ�ȡ
		temp->name = name;
		temp->pre = curdir;
		curdir->next[name] = temp;
	}
}

/* ���� cd */
void cd(string name) {
	if (name == "") {
		curdir = root;  // �ص���Ŀ¼
	}
	else {
		dir* temp_path = pathTrans(name);
		if (temp_path == NULL) {
			cout << "This directory does not exist.\n";
		}
		else {
			curdir = temp_path;
		}
	}
}

/* ���� ls */
void ls(string path) {
	dir* temp_dir = curdir;
	if (path != "") {
		temp_dir = pathTrans(path);
		if (temp_dir == NULL) {
			cout << "Wrong Path!\n";
			return;
		}
	}
	for (auto it = temp_dir->next.begin(); it != temp_dir->next.end(); it++) {
		cout << it->first << setw(25 - it->first.length()) << "<DIR>" << endl;
	}
	for (auto it = temp_dir->files.begin(); it != temp_dir->files.end(); it++) {
		cout << it->first << setw(26 - it->first.length()) << "<FILE>" << " size: " << it->second->bits << endl;
	}

}

/* ���� touch �½��ļ� */
void mkfile(string name) {
	//string name;//�ļ���
	//cin >> name;
	if (name == "") {
		help2("mkfile?");
	}
	else if (curdir->files.find(name) != curdir->files.end()) {
		cout << "There is a file having the same name.\n";
	}
	else if (!nameCheak(name)) {
		cout << "File name cannot contain the following characters:\n"
			<< "'\',' / ',' | ',':','; ','<','>',' ? ',\"..\"" << endl;
	}
	else
	{
		file* temp = new file();
		temp->name = name;
		temp->content.push_back("");
		curdir->files[name] = temp;
	}
}

/* ���� edit �༭�ļ� */
void edit(string name) {
	dir* temp_dir = curdir;
	if (name == "") {
		help2("edit?");
		return;
	}
	else if (name.find_last_of('/') != name.npos) {
		// ����·��
		string temp = name;
		if (temp[0] == '/') temp = "~" + temp;
		temp = temp.substr(0, temp.find_last_of('/'));
		// ��ȡ�ļ���
		name = name.substr(name.find_last_of('/') + 1, name.length() - name.find_last_of('/'));
		curdir = pathTrans(temp);
		if (curdir == NULL) {
			curdir = temp_dir;
			printf("Wrong Path!\n");
			return;
		}
	}
	if (curdir->files.find(name) == curdir->files.end()) {
		cout << "No Such File.\n";
	}
	else {
		ofstream file_out(name);
		for (int i = 0; i < curdir->files[name]->content.size(); i++) {
			file_out << curdir->files[name]->content[i] << endl;
		}
		file_out.close();

		// ����notepad�༭�ļ�
		char str[32] = "notepad ";
		strcat_s(str, name.data());
		system(str);

		ifstream file_in(name);
		file_in.seekg(0, ios::end);  // �����ļ�ָ�뵽�ļ�����β��
		streampos bits = file_in.tellg();  // ��ȡ�ļ�ָ���λ��
		curdir->files[name]->bits = bits;  // ��ȡ�ļ���С
		file_in.close();  // �ر��ļ���

		ifstream file_in2(name);
		string temp_str;
		curdir->files[name]->content.clear();  // ���ԭ�������ڵ�����
		while (getline(file_in2, temp_str)) {
			curdir->files[name]->content.push_back(temp_str);
		}
		file_in2.close();  // �ر��ļ���

		char str2[32] = "";
		strcat_s(str2, name.data());
		remove(str2);  // ɾ����ʱ�ļ�
	}
	curdir = temp_dir;
}

/* ���� cat ��ʾ�ļ����� */
void show(string name) {
	dir* temp_dir = curdir;
	if (name == "") {
		help2("cat?");
		return;
	}
	else if (name.find_last_of('/') != name.npos) {
		// ����·��
		string temp = name;
		if (temp[0] == '/') temp = "~" + temp;
		temp = temp.substr(0, temp.find_last_of('/'));
		// ��ȡ�ļ���
		name = name.substr(name.find_last_of('/') + 1, name.length() - name.find_last_of('/'));
		curdir = pathTrans(temp);
		if (curdir == NULL) {
			curdir = temp_dir;
			printf("Wrong Path!\n");
			return;
		}
	}
	if (curdir->files.find(name) == curdir->files.end()) {
		cout << "No Such File.\n";
	}
	else {
		for (int i = 0; i < curdir->files[name]->content.size(); i++) {
			cout << curdir->files[name]->content[i] << endl;
		}
		cout << endl;
	}
	curdir = temp_dir;
}

/* ���� rename ������ */
void rename(string temp) {
	dir* temp_dir = curdir;
	string option = "";
	if (temp.find_first_of('-') != temp.npos && temp.length() >= 2) {
		option = temp.substr(temp.find_first_of('-'), 2);
	}
	else {
		cout << "Wrong Option!\n";
		return;
	}
	string oldName = "", newName = "";
	oldName = temp.substr(temp.find_first_of('-') + 3, temp.find_last_of(' ') - temp.find_first_of('-') - 3);
	newName = temp.substr(temp.find_last_of(' ') + 1, temp.length() - temp.find_last_of(' '));
	if (!nameCheak(newName)) {
		cout << "File name cannot contain the following characters:\n"
			<< "'\',' / ',' | ',':','; ','<','>',' ? ',\"..\"" << endl;
		return;
	}
	if (newName == "" || oldName == "" || newName == oldName) {
		help2("rename?");
		return;
	}
	if (oldName.find_last_of('/') != oldName.npos) {  // ·��ת��
		string temp = oldName;
		if (temp[0] == '/') temp = "~" + temp;
		temp = temp.substr(0, temp.find_last_of('/'));
		oldName = oldName.substr(oldName.find_last_of('/') + 1, oldName.length() - oldName.find_last_of('/'));
		curdir = pathTrans(temp);
		if (curdir == NULL) {
			curdir = temp_dir;
			printf("Wrong Path!\n");
			return;
		}
	}
	if (option == "-d") {  // ��Ŀ¼��
		if (curdir->next.find(oldName) == curdir->next.end()) {
			cout << "No Such Directory.\n";
		}
		else if (curdir->next.find(newName) != curdir->next.end()) {
			cout << "There is a directory having same name.\n";
		}
		else {
			dir* temp = curdir->next[oldName];
			curdir->next.erase(oldName);
			temp->name = newName;
			curdir->next[newName] = temp;
		}
	}
	else if (option == "-f") {  // ���ļ���
		if (curdir->files.find(oldName) == curdir->files.end()) {
			cout << "No Such File.\n";
		}
		else if (curdir->files.find(newName) != curdir->files.end()) {
			cout << "There is a file having same name.\n";
		}
		else {
			file* temp = curdir->files[oldName];
			curdir->files.erase(oldName);
			temp->name = newName;
			curdir->files[newName] = temp;
		}
	}
	else {
		cout << "Wrong Option!\n";
	}
	curdir = temp_dir;
}

/* �ݹ�ɾ��Ŀ¼ */
void deletedir(dir* cur) {
	// ��ɾ�ļ�
	for (auto it = cur->files.begin(); it != cur->files.end(); it++) {
		delete(it->second);
	}
	cur->files.clear();
	// ��ɾĿ¼
	for (auto it = cur->next.begin(); it != cur->next.end(); it++) {
		deletedir(it->second);
	}
	cur->next.clear();
	delete(cur);
}

/* ���� rm ɾ�� */
void rm(string temp) {
	string option = "";
	dir* temp_dir = curdir;
	if (temp.find_first_of('-') != temp.npos && temp.length() >= 2) {
		option = temp.substr(temp.find_first_of('-'), 2);
	}
	else {
		cout << "Wrong Option!\n";
		return;
	}
	string name = "";
	name = temp.substr(temp.find_first_of('-') + 3, temp.length() - temp.find_first_of('-') - 3);
	if (name.find_last_of('/') != name.npos) {  //·��ת��
		string temp = name;
		if (temp[0] == '/') temp = "~" + temp;
		temp = temp.substr(0, temp.find_last_of('/'));
		name = name.substr(name.find_last_of('/') + 1, name.length() - name.find_last_of('/'));
		curdir = pathTrans(temp);
		if (curdir == NULL) {
			curdir = temp_dir;
			printf("Wrong Path!\n");
			return;
		}
	}
	if (name == "") {
		help2("rm?");
	}
	else if (option == "-d") {
		if (curdir->next.find(name) == curdir->next.end()) {
			cout << "No Such Directory.\n";
		}
		else {
			deletedir(curdir->next[name]);
			curdir->next.erase(name);
		}
	}
	else if (option == "-f") {
		if (curdir->files.find(name) == curdir->files.end()) {
			cout << "No Such File.\n";
		}
		else {
			delete(curdir->files[name]);
			curdir->files.erase(name);
		}
	}
	else {
		cout << "Wrong Option!\n";
	}
}

/* �ݹ鸴��Ŀ¼ */
dir* cpDir(dir* temp) {
	dir* cp_dir = new dir(*temp);
	// ���ԭ��������
	cp_dir->next.clear();
	cp_dir->files.clear();
	// ���ļ��ؽ�
	for (auto it = temp->files.begin(); it != temp->files.end(); it++) {
		file* temp_file = new file(*(it->second));
		cp_dir->files[it->first] = temp_file;
	}
	// �ؽ�Ŀ¼
	for (auto it = temp->next.begin(); it != temp->next.end(); it++) {
		dir* temp_dir = cpDir(it->second);
		temp_dir->pre = cp_dir;
		cp_dir->next[it->first] = temp_dir;
	}
	return cp_dir;
}

/* ���� cp ���� */ 
void cp(string temp) {
	string option = "";
	if (temp.find_first_of('-') != temp.npos && temp.length() >= 3) {
		option = temp.substr(temp.find_first_of('-'), temp.find_first_of(' ', temp.find_first_of('-'))
			- temp.find_first_of('-'));
	}
	else {
		cout << "Wrong Option!\n";
		return;
	}
	string source = "", dest = "", name = "";
	source = temp.substr(temp.find_first_of(' ', temp.find_first_of('-')) + 1, temp.find_last_of(' ') 
		- temp.find_first_of(' ', temp.find_first_of('-')) - 1);
	dest = temp.substr(temp.find_last_of(' ') + 1, temp.length() - temp.find_last_of(' '));
	
	dir* sou = NULL, * den = NULL;
	if (source.find_first_of("/") == source.npos) {
		sou = curdir;
		name = source;
	}
	else {  // ·��ת��
		if (source[0] == '/') source = "~" + source;
		name = source.substr(source.find_last_of('/') + 1, source.length() - source.find_last_of('/'));
		source = source.substr(0, source.find_last_of('/'));
		sou = pathTrans(source);
		if (sou == NULL) {
			cout << "No Such File." << endl;
			return;
		}
	}
	den = pathTrans(dest);
	if (den == NULL) {
		cout << "Wrong Path!" << endl;
	}
	else if (option == "-f") {  // �����ļ�
		if (sou->files.find(name) == sou->files.end()) {
			cout << "No Such File." << endl;
			return;
		}
		else if (den->files.find(name) != den->files.end()) {
			cout << "Destination has a same file.\n";
			cout << "Whether to continue copying?(y/n):";
			char sign;
			cin >> sign;
			if (sign == 'Y' || sign == 'y') {
				file* temp_file = new file(*(sou->files[name]));
				string front = "", later = "";  // �ļ���ʽ
				front = name.substr(0, name.find_last_of('.'));
				later = name.substr(name.find_last_of('.') + 1, name.length() - name.find_last_of('.'));
				temp_file->name = front + "_copy." + later;
				den->files[front + "_copy." + later] = temp_file;
			}
		}
		else {
			file* temp_file = new file(*(sou->files[name]));
			den->files[name] = temp_file;
		}
	}
	else if (option == "-d") {  // ����Ŀ¼
		if (sou->next.find(name) == sou->next.end()) {
			cout << "No Such Directory." << endl;
			return;
		}
		else if (den->next.find(name) != den->next.end()) {
			cout << "Destination has a same directory.\n";
			cout << "Whether to continue copying?(y/n):";
			char sign;
			cin >> sign;
			if (sign == 'Y' || sign == 'y') {
				dir* temp_dir = cpDir(sou->next[name]);
				temp_dir->name += "_copy";
				temp_dir->pre = den;
				den->next[name + "_copy"] = temp_dir;
			}
		}
 		else {
			dir* temp_dir = cpDir(sou->next[name]);
			temp_dir->pre = den;
			den->next[name] = temp_dir;
		}
	}
	else if (option == "-cf") {  // ���и����ļ�
		if (sou->files.find(name) == sou->files.end()) {
			cout << "No Such File." << endl;
			return;
		}
		else if (den->files.find(name) != den->files.end()) {
			cout << "Destination has a same file.\n";
			cout << "Whether to continue copying?(y/n):";
			char sign;
			cin >> sign;
			if (sign == 'Y' || sign == 'y') {
				string front = "", later = "";  // �ļ���ʽ
				front = name.substr(0, name.find_last_of('.'));
				later = name.substr(name.find_last_of('.') + 1, name.length() - name.find_last_of('.'));
				den->files[front + "_copy." + later] = sou->files[name];
				sou->files.erase(name);  // ɾ��
			}
		}
		else {
			den->files[name] = sou->files[name];
			sou->files.erase(name);  // ɾ��
		}
	}
	else if (option == "-cd") {  // ���и����ļ�Ŀ¼
		if (sou->next.find(name) == sou->next.end()) {
			cout << "No Such Directory." << endl;
			return;
		}
		else if (den->next.find(name) != den->next.end()) {
			cout << "Destination has a same directory.\n";
			cout << "Whether to continue copying?(y/n):";
			char sign;
			cin >> sign;
			if (sign == 'Y' || sign == 'y') {
				den->next[name + "_copy"] = sou->next[name];
				sou->next.erase(name);  // ɾ��
			}
		}
		else {
			den->next[name] = sou->next[name];
			sou->next.erase(name);  // ɾ��
		}
	}
	else {
		cout << "Wrong Option!\n";
	}
}

/* �л��û� */
void su() {
	cout << "Input password:";
	string temp_pawd;
	cin >> temp_pawd;
	if (temp_pawd == curuser.password) {
		exit();
		login();
		initVds();  // ϵͳ��ʼ��
	}
	else {
		cout << "password is incorrect!" << endl;
	}
}

/* �����û���Ϣ */
void reflash() {
	map<string, string> users;

	ifstream file_in("user.dat");
	string temp_name, temp_pawd;
	// ���ش��ڵ��û���Ϣ
	while (file_in >> temp_name >> temp_pawd) {
		users[temp_name] = temp_pawd;
	}
	file_in.close();

	//for (auto it = users.begin(); it != users.end(); it++) {
	//	cout << it->first << " " << it->second << endl;
	//}

	users.erase(curuser.name);  // ɾ���û�

	ofstream file_out("user.dat");
	for (auto it = users.begin(); it != users.end(); it++) {
		file_out << it->first << " " << it->second << endl;
	}
	file_out.close();
}

/* �û�ע�� */
void logout() {
	cout << "Input password:";
	string temp_pawd;
	cin >> temp_pawd;
	if (temp_pawd == curuser.password) {
		char str[32] = "";
		strcat_s(str, curuser.name.data());
		strcat_s(str, ".dat");
		if (remove(str) == 0) {
			cout << "User logout Successfully..." << endl;
		}
		else {
			cout << "Fall to logout..." << endl;
		}
		reflash();  // ˢ���û���Ϣ
		getchar();
		login();  // ���µ�½
		initVds();  // ϵͳ��ʼ��
	}
	else {
		cout << "password is incorrect!" << endl;
	}
}

/* �洢Ŀǰ��� */
void save(dir* temp) {
	records.push_back(temp->name);  // Ŀ¼��
	records.push_back(to_string(temp->files.size()));  // �ļ���
	for (auto it = temp->files.begin(); it != temp->files.end(); it++) {
		records.push_back(it->second->name);  // �ļ���
		records.push_back(to_string(it->second->bits));  // �ļ���С
		for (int i = 0; i < it->second->content.size(); i++) {
			records.push_back(it->second->content[i]);  // �ļ�����
		}
		records.push_back("content");  // �ļ����ݽ�����
	}
	records.push_back(to_string(temp->next.size()));  // ��Ŀ¼��
	for (auto it = temp->next.begin(); it != temp->next.end(); it++) {
		records.push_back(it->second->name);  // ��Ŀ¼��
		save(it->second);  // �ݹ���Ŀ¼
	}
}

/* �˳�ϵͳ */
void exit() {
	records.clear();
	save(root);
	ofstream file_out(curuser.name + ".dat");
	for (int i = 0; i < records.size(); i++) {
		file_out << records[i] << endl;
	}
	file_out.close();
}

/********** USER CODE OVER *****************************************************************************************/

/********** MAIN BEGIN *********************************************************************************************/

int main() {
	
	login();  // �û���¼ ���� �������û�
	initVds();  // ϵͳ��ʼ��

	string command;
	while (1) {
		cout << curuser.name + "@" + curuser.name << "-vds:";
		displayPath();  // չʾĿ¼

		getline(cin, command);  // ��ȡ����
		string temp = command;	// �����
		if (command.find_first_of(' ') != command.npos) {
			command = command.substr(0, command.find_first_of(' '));
			temp = temp.substr(temp.find_first_of(' ') + 1, temp.length() - temp.find_first_of(' '));
		}
		else temp = "";

		if (command == "help") help();
		else if (command == "mkdir") mkdir(temp);
		else if (command == "cd") cd(temp);
		else if (command == "mkfile") mkfile(temp);
		else if (command == "ls") ls(temp);
		else if (command == "edit") edit(temp);
		else if (command == "rename") rename(temp);
		else if (command == "rm") rm(temp);
		else if (command == "cp") cp(temp);
		else if (command == "cls" || command == "clear") cls();
		else if (command == "su") su();
		else if (command == "show") show(temp);
		else if (command == "logout") logout();

		else if (command == "exit") {
			exit();
			return 0;
		}
		else help2(command);
	}
	system("pause");
}

/********** MAIN OVER **********************************************************************************************/