#pragma once

#include "tinyxml.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

//数据库 标志
const string DBFlag = "use";
//数据表 标志
const string TABLEFlag = "create table";

//数据库中分隔 符号
const string semicolon = ";";
const string space = " ";
const string comma = ",";

//字段信息以这个分隔，因为有些注释中有逗号
const string fieldIsolate = "@@"; 

//其他关键字
const string note = "--";   //单行注释
const string quotes1 = "`";	//字段名引号
const string quotes2 = "'";	//字符串引号
const string equalSign = "=";	//等号
const string bracketsL = "(";//左括号
const string bracketsR = ")";//右括号
const string primary = "primary";	//主键
const string desc = "comment";	//评论
const string sql = ".sql";	//数据库格式
const string xml = ".xml";	//xml格式

//sql数据类型关键字
////////////string/////////////
const string tText = "text";
const string tTinytext = "tinytext";
const string tMediumtext = "mediumtext";
const string tLongtext = "longtext";
const string tVarchar = "varchar";
const string tDate = "date";
const string tDatetime = "datetime";
const string tTimestamp = "timestamp";
const string tTime = "time";
const string tYear = "year";
const string tChar = "char";

/////////////double////////////
const string tDouble = "double";
const string tDecimal = "decimal";
const string tReal = "real";

/////////////float/////////////
const string tFloat = "float";

///////////int64////////////////
const string tBigint = "bigint";

////////////int32/////////////
const string tInt = "int";
const string tBoolean = "boolean";

/////////////unsigned//////////
const string tUnsigned = "unsigned";

class SqlToXml
{
public:
	SqlToXml();
	~SqlToXml();

	void SetSqlPath(string path){ m_sqlPath = path; }
	string GetSqlPath(){ return m_sqlPath; }

	//把sql中的有用语句读取内存中，保存到vector中
	bool GetStatementBySql(string path="");

	//把sql语句转换成文件
	void CreateXml();

	//获取sql语句的关键字 并 保存
	void DatabaseName(string &name, TiXmlElement* database);	//获取数据信息（name）
	void TableInfo(string &name,string &comment,string &primary,TiXmlElement* table);//获取表信息（name，comment，primary）
	void FieldInfo(string &info, string &name, string &type, string &comment, TiXmlElement* field);//数据表中每个字段信息(name,type,comment)

	//表的关键字 处理函数
	void TableName(string &name);		//数据表 名称
	void TableComment(string &comment);	//数据表 描述
	void TablePrimary(string &key);		//数据表 主键
	
	//字符串处理函数
	void ToLowerCase(string& str);		//转换成小写
	void ToUpperCase(string& str);		//转换成大写
	bool StartsWith(const string& str, const string& pattern);	//是否以某个字符串开始
	bool EndsWith(const string& str, const string& pattern);	//是否以某个字符串结束
	vector<string> Split(const string& str,const string &separateStr);//通过某个关键字分隔字符串
	bool Find(const string &str,const string &substr);			//字符串中是否有某个子串
	void Trim(string &str, const string &pattern);				//去掉字符串的开始和结尾某个特殊的字符串

	//字符编码
	void Utf8ToGb2312(string &str);

	//清空
	void clear();

	//测试
	void DisplayStatement();
	
private:
	string m_sqlPath;				//sql路径
	vector<string> m_vecStatement;	//要处理的sql语句
};

