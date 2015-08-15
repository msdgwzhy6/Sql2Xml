#pragma once

#include "tinyxml.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

//���ݿ� ��־
const string DBFlag = "use";
//���ݱ� ��־
const string TABLEFlag = "create table";

//���ݿ��зָ� ����
const string semicolon = ";";
const string space = " ";
const string comma = ",";

//�ֶ���Ϣ������ָ�����Ϊ��Щע�����ж���
const string fieldIsolate = "@@"; 

//�����ؼ���
const string note = "--";   //����ע��
const string quotes1 = "`";	//�ֶ�������
const string quotes2 = "'";	//�ַ�������
const string equalSign = "=";	//�Ⱥ�
const string bracketsL = "(";//������
const string bracketsR = ")";//������
const string primary = "primary";	//����
const string desc = "comment";	//����
const string sql = ".sql";	//���ݿ��ʽ
const string xml = ".xml";	//xml��ʽ

//sql�������͹ؼ���
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

	//��sql�е���������ȡ�ڴ��У����浽vector��
	bool GetStatementBySql(string path="");

	//��sql���ת�����ļ�
	void CreateXml();

	//��ȡsql���Ĺؼ��� �� ����
	void DatabaseName(string &name, TiXmlElement* database);	//��ȡ������Ϣ��name��
	void TableInfo(string &name,string &comment,string &primary,TiXmlElement* table);//��ȡ����Ϣ��name��comment��primary��
	void FieldInfo(string &info, string &name, string &type, string &comment, TiXmlElement* field);//���ݱ���ÿ���ֶ���Ϣ(name,type,comment)

	//��Ĺؼ��� ������
	void TableName(string &name);		//���ݱ� ����
	void TableComment(string &comment);	//���ݱ� ����
	void TablePrimary(string &key);		//���ݱ� ����
	
	//�ַ���������
	void ToLowerCase(string& str);		//ת����Сд
	void ToUpperCase(string& str);		//ת���ɴ�д
	bool StartsWith(const string& str, const string& pattern);	//�Ƿ���ĳ���ַ�����ʼ
	bool EndsWith(const string& str, const string& pattern);	//�Ƿ���ĳ���ַ�������
	vector<string> Split(const string& str,const string &separateStr);//ͨ��ĳ���ؼ��ַָ��ַ���
	bool Find(const string &str,const string &substr);			//�ַ������Ƿ���ĳ���Ӵ�
	void Trim(string &str, const string &pattern);				//ȥ���ַ����Ŀ�ʼ�ͽ�βĳ��������ַ���

	//�ַ�����
	void Utf8ToGb2312(string &str);

	//���
	void clear();

	//����
	void DisplayStatement();
	
private:
	string m_sqlPath;				//sql·��
	vector<string> m_vecStatement;	//Ҫ�����sql���
};

