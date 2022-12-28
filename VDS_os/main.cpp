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

struct user {			// 用户定义
	string name;		// 用户名
	string password;	// 密码
};

struct file {				// 文件定义
	string name;			// 文件名
	long bits;				// 文件大小
	vector<string> content;	// 文件内容
};

struct dir {					// 目录定义
	string name;				// 目录名
	dir* pre;					// 父目录
	map<string, file*> files;	// 所包含的文件
	map<string, dir*> next;		// 子目录
};

user curuser;	// 当前用户
dir* curdir;	// 当前目录
dir* root;		// 根目录

/********** DEFINE OVER ********************************************************************************************/

/********** USER FUNCTION DECLARATION BEGIN ************************************************************************/

void initDir();						/* 初始化目录 */
void displayPath();					/* 展示路径 */
dir* reCreate(dir* last);			/* 还原用户关闭前的状态 */
void initVds();						/* 初始化函数 */
void login();						/* 登录系统 */
bool nameCheak(string name);		/* 检查名字是否合法 */
vector<string> split(string str);	/* 按照'/'拆分字符串 */
dir* pathTrans(string path);		/* 地址转换 */
void help();						/* 帮助命令界面 */
void help2(string command);			/* 单独命令帮助 */
void cls();							/* 命令 clear 清屏 */
void mkdir(string name);			/* 命令 mkdir */
void cd(string name);				/* 命令 cd */
void ls(string path);				/* 命令 ls */
void mkfile(string name);			/* 命令 touch 新建文件 */
void edit(string name);				/* 命令 edit 编辑文件 */
void show(string name);				/* 命令 show 显示文件内容 */
void rename(string temp);			/* 命令 rename 重命名 */
void deletedir(dir* cur);			/* 递归删除目录 */
void rm(string temp);				/* 命令 rm 删除 */
dir* cpDir(dir* temp);				/* 递归复制目录 */
void cp(string temp);				/* 命令 cp 复制 */
void su();							/* 切换用户 */
void reflash();						/* 更新用户信息 */
void logout();						/* 用户注销 */
void save(dir* temp);				/* 存储目前信息 */
void exit();						/* 保存信息 退出系统 */

/********** USER FUNCTION DECLARATION END **************************************************************************/

/********** USER CODE BEGIN ****************************************************************************************/

/* 初始化目录 */
void initDir() {
	curdir = new dir();
	curdir->name = "~";
	curdir->pre = NULL;
	root = curdir;
}

/* 展示路径 */
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


/* 还原用户关闭前的状态 */
dir* reCreate(dir* last) {
	dir* temp_dir = new dir();  // 新建目录
	temp_dir->name = records[reco++];  // 目录名
	temp_dir->pre = last;  // 设置父指针
	string temp_count;
	temp_count = records[reco++];  // 文件数
	for (int i = 0; i < stoi(temp_count); i++) {
		file* temp_file = new file();  // 新建文件
		temp_file->name = records[reco++];  // 文件名
		temp_file->bits = stoi(records[reco++]);  // 文件大小
		while (1) {
			string temp_str;
			temp_str = records[reco++];  // 文件内容
			if (temp_str != "content") {  // 读入内容 关键字分隔
				temp_file->content.push_back(temp_str);
			}
			else {
				break;
			}
		}
		temp_dir->files[temp_file->name] = temp_file;  // 将新建的文件加入目录
	}
	temp_count = records[reco++];  // 子目录数
	for (int i = 0; i < stoi(temp_count); i++) {
		string name;
		name = records[reco++];  // 子目录名
		temp_dir->next[name] = reCreate(temp_dir);  // 递归新建子目录
	}
	return temp_dir;
}

/* 初始化函数 */
void initVds() {
	ifstream file_in(curuser.name + ".dat");  // 读入用户文件信息
	string temp;
	if (!file_in) {
		initDir();  // 新用户新初始化目录
	}
	while (getline(file_in, temp)) {
		records.push_back(temp);
	}

	if (records.size() >= 1) {  // 读入系统信息
		root = curdir = reCreate(NULL);
	}
	else {
		initDir();  // 新用户新初始化目录
	}
}

/* 登录系统 */
void login() {
	map<string, string> users;
	bool flag = 1;  // 用户名匹配判定标记

	system("cls");
	cout << "This is Virtual Decument System v2.0" << endl;
	cout << "Author: YU.J.P" << endl;
	
	ifstream file_in("user.dat");
	if (!file_in) {
		cout << "Error on oponing user.dat！" << endl;
		system("Pause");
		exit(1);
	}

	string temp_name, temp_pawd;
	// 加载存在的用户信息
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
			flag = 0;  // 没找到
		}
		else {	// 找到
			if (users[temp_name] == temp_pawd) {
				system("cls");
				curuser.name = temp_name;
				curuser.password = temp_pawd;

				// 添加新用户
				users[temp_name] = temp_pawd;

				break;  // 匹配成功 退出循环
			}
			else {
				cout << "password is incorrect!\n";
			}
		}
	}
	if (!flag) {  // 未匹配成功 创建用户
		cout << "This user is not exist.\nDo you want to creat a new user?(y/n):";
		char choice;
		cin >> choice;
		if (choice == 'Y' || choice == 'y') {
			users[temp_name] = temp_pawd;
			curuser.name = temp_name;
			curuser.password = temp_pawd;
			// 创建一个空文件
			ofstream outfile(temp_name + ".dat", ios::app);
			if (outfile.is_open()) {
				cout << "File already exists." << endl;
				outfile.close();
			}
			system("cls");
		}
		else {  // 不同意创建 重新输入
			login();  // 递归调用
		}
	}

	// 存储用户信息
	ofstream file_out("user.dat");
	for (auto it = users.begin(); it != users.end(); it++) {
		file_out << it->first << " " << it->second << endl;
	}
	getchar();
	return;
}

/* 检查名字是否合法 */
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

/* 按照'/'拆分字符串 */
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

/* 地址转换 */
dir* pathTrans(string path) {
	// 绝对路径
	if (path[0] == '~' || path[0] == '/') {
		dir* cur_dir = root;
		if (path[0] == '/') path = "~" + path;
		vector<string> temp_path = split(path);
		for (int i = 1; i < temp_path.size(); i++) {
			if (cur_dir->next.find(temp_path[i]) == cur_dir->next.end()) {
				return NULL;  // 没找到
			}
			cur_dir = cur_dir->next[temp_path[i]];
		}
		return cur_dir;
	}
	// 相对路径
	else {
		dir* cur_dir = curdir;
		vector<string> temp_path = split(path);
		for (int i = 0; i < temp_path.size(); i++) {
			if (temp_path[i] == ".") {
				continue;
			}
			else if (temp_path[i] == "..") {
				if (cur_dir == root) {  // 不能再往上走了
					return NULL;
				}
				else {
					cur_dir = cur_dir->pre;
				}
			}
			else if (cur_dir->next.find(temp_path[i]) == cur_dir->next.end()) {
				return NULL;  // 没找到
			}
			else if (cur_dir->next.find(temp_path[i]) != cur_dir->next.end()) {
				cur_dir = cur_dir->next[temp_path[i]];
			}
		}
		return cur_dir;
	}
	return NULL;
}

/* 帮助命令界面 */
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

/* 单独命令帮助 */
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

/* 命令 clear 清屏 */
void cls() {
	system("cls");
}

/* 命令 mkdir */
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
		dir* temp = new dir();//一定要这样创建，否则字符串后面就不能读取
		temp->name = name;
		temp->pre = curdir;
		curdir->next[name] = temp;
	}
}

/* 命令 cd */
void cd(string name) {
	if (name == "") {
		curdir = root;  // 回到根目录
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

/* 命令 ls */
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

/* 命令 touch 新建文件 */
void mkfile(string name) {
	//string name;//文件名
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

/* 命令 edit 编辑文件 */
void edit(string name) {
	dir* temp_dir = curdir;
	if (name == "") {
		help2("edit?");
		return;
	}
	else if (name.find_last_of('/') != name.npos) {
		// 处理路径
		string temp = name;
		if (temp[0] == '/') temp = "~" + temp;
		temp = temp.substr(0, temp.find_last_of('/'));
		// 获取文件名
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

		// 调用notepad编辑文件
		char str[32] = "notepad ";
		strcat_s(str, name.data());
		system(str);

		ifstream file_in(name);
		file_in.seekg(0, ios::end);  // 设置文件指针到文件流的尾部
		streampos bits = file_in.tellg();  // 读取文件指针的位置
		curdir->files[name]->bits = bits;  // 获取文件大小
		file_in.close();  // 关闭文件流

		ifstream file_in2(name);
		string temp_str;
		curdir->files[name]->content.clear();  // 清除原来数组内的内容
		while (getline(file_in2, temp_str)) {
			curdir->files[name]->content.push_back(temp_str);
		}
		file_in2.close();  // 关闭文件流

		char str2[32] = "";
		strcat_s(str2, name.data());
		remove(str2);  // 删除临时文件
	}
	curdir = temp_dir;
}

/* 命令 cat 显示文件内容 */
void show(string name) {
	dir* temp_dir = curdir;
	if (name == "") {
		help2("cat?");
		return;
	}
	else if (name.find_last_of('/') != name.npos) {
		// 处理路径
		string temp = name;
		if (temp[0] == '/') temp = "~" + temp;
		temp = temp.substr(0, temp.find_last_of('/'));
		// 获取文件名
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

/* 命令 rename 重命名 */
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
	if (oldName.find_last_of('/') != oldName.npos) {  // 路径转换
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
	if (option == "-d") {  // 改目录名
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
	else if (option == "-f") {  // 改文件名
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

/* 递归删除目录 */
void deletedir(dir* cur) {
	// 先删文件
	for (auto it = cur->files.begin(); it != cur->files.end(); it++) {
		delete(it->second);
	}
	cur->files.clear();
	// 再删目录
	for (auto it = cur->next.begin(); it != cur->next.end(); it++) {
		deletedir(it->second);
	}
	cur->next.clear();
	delete(cur);
}

/* 命令 rm 删除 */
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
	if (name.find_last_of('/') != name.npos) {  //路径转换
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

/* 递归复制目录 */
dir* cpDir(dir* temp) {
	dir* cp_dir = new dir(*temp);
	// 清除原来的内容
	cp_dir->next.clear();
	cp_dir->files.clear();
	// 把文件重建
	for (auto it = temp->files.begin(); it != temp->files.end(); it++) {
		file* temp_file = new file(*(it->second));
		cp_dir->files[it->first] = temp_file;
	}
	// 重建目录
	for (auto it = temp->next.begin(); it != temp->next.end(); it++) {
		dir* temp_dir = cpDir(it->second);
		temp_dir->pre = cp_dir;
		cp_dir->next[it->first] = temp_dir;
	}
	return cp_dir;
}

/* 命令 cp 复制 */ 
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
	else {  // 路径转换
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
	else if (option == "-f") {  // 复制文件
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
				string front = "", later = "";  // 文件格式
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
	else if (option == "-d") {  // 复制目录
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
	else if (option == "-cf") {  // 剪切复制文件
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
				string front = "", later = "";  // 文件格式
				front = name.substr(0, name.find_last_of('.'));
				later = name.substr(name.find_last_of('.') + 1, name.length() - name.find_last_of('.'));
				den->files[front + "_copy." + later] = sou->files[name];
				sou->files.erase(name);  // 删除
			}
		}
		else {
			den->files[name] = sou->files[name];
			sou->files.erase(name);  // 删除
		}
	}
	else if (option == "-cd") {  // 剪切复制文件目录
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
				sou->next.erase(name);  // 删除
			}
		}
		else {
			den->next[name] = sou->next[name];
			sou->next.erase(name);  // 删除
		}
	}
	else {
		cout << "Wrong Option!\n";
	}
}

/* 切换用户 */
void su() {
	cout << "Input password:";
	string temp_pawd;
	cin >> temp_pawd;
	if (temp_pawd == curuser.password) {
		exit();
		login();
		initVds();  // 系统初始化
	}
	else {
		cout << "password is incorrect!" << endl;
	}
}

/* 更新用户信息 */
void reflash() {
	map<string, string> users;

	ifstream file_in("user.dat");
	string temp_name, temp_pawd;
	// 加载存在的用户信息
	while (file_in >> temp_name >> temp_pawd) {
		users[temp_name] = temp_pawd;
	}
	file_in.close();

	//for (auto it = users.begin(); it != users.end(); it++) {
	//	cout << it->first << " " << it->second << endl;
	//}

	users.erase(curuser.name);  // 删除用户

	ofstream file_out("user.dat");
	for (auto it = users.begin(); it != users.end(); it++) {
		file_out << it->first << " " << it->second << endl;
	}
	file_out.close();
}

/* 用户注销 */
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
		reflash();  // 刷新用户信息
		getchar();
		login();  // 重新登陆
		initVds();  // 系统初始化
	}
	else {
		cout << "password is incorrect!" << endl;
	}
}

/* 存储目前情况 */
void save(dir* temp) {
	records.push_back(temp->name);  // 目录名
	records.push_back(to_string(temp->files.size()));  // 文件数
	for (auto it = temp->files.begin(); it != temp->files.end(); it++) {
		records.push_back(it->second->name);  // 文件名
		records.push_back(to_string(it->second->bits));  // 文件大小
		for (int i = 0; i < it->second->content.size(); i++) {
			records.push_back(it->second->content[i]);  // 文件内容
		}
		records.push_back("content");  // 文件内容结束符
	}
	records.push_back(to_string(temp->next.size()));  // 子目录数
	for (auto it = temp->next.begin(); it != temp->next.end(); it++) {
		records.push_back(it->second->name);  // 子目录名
		save(it->second);  // 递归子目录
	}
}

/* 退出系统 */
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
	
	login();  // 用户登录 或者 创建新用户
	initVds();  // 系统初始化

	string command;
	while (1) {
		cout << curuser.name + "@" + curuser.name << "-vds:";
		displayPath();  // 展示目录

		getline(cin, command);  // 获取命令
		string temp = command;	// 命令处理
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