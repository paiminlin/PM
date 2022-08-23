/*
iniparser.h 接口介绍
*/

//获取dictionary对象的section个数
int iniparser_getnsec(dictionary *d);

//获取dictionary对象的第n个section的名字
char * iniparser_getsecname(dictionary *d, int  n);

//保存dictionary对象到file
void iniparser_dump_ini(dictionary * d, FILE * f);

//保存dictionary对象一个section到file
void iniparser_dumpsection_ini(dictionary * d, char * s, FILE * f);

//保存dictionary对象到file
void iniparser_dump(dictionary * d, FILE * f);

//获取dictionary对象某个section下的key个数
int iniparser_getsecnkeys(dictionary * d, char * s);

//获取dictionary对象某个section下全部的key
char ** iniparser_getseckeys(dictionary * d, char * s);

//返回dictionary对象的section:key对应的字串值
char * iniparser_getstring(dictionary * d, const char * key, char * def);

//返回idictionary对象的section:key对应的整形值
int iniparser_getint(dictionary * d, const char * key, int notfound);

//返回dictionary对象的section:key对应的双浮点值
double iniparser_getdouble(dictionary * d, const char * key, double notfound);

//返回dictionary对象的section:key对应的布尔值
int iniparser_getboolean(dictionary * d, const char * key, int notfound);

//设置dictionary对象的某个section:key的值
int iniparser_set(dictionary * ini, const char * entry, const char * val);

//删除dictionary对象中某个section:key
void iniparser_unset(dictionary * ini, const char * entry);

//判断dictionary对象中是否存在某个section:key
int iniparser_find_entry(dictionary * ini, const char * entry);

//解析dictionary对象并返回(分配内存)dictionary对象
dictionary * iniparser_load(const char * ininame);

//释放dictionary对象(内存)
void iniparser_freedict(dictionary * d);

/*
dictionary.h 接口介绍
*/

//计算关键词的hash值
unsigned dictionary_hash(const char * key);

//建立dictionary对象
dictionary * dictionary_new(int size);

//删除dictionary对象
void dictionary_del(dictionary * vd);

//获取dictionary对象的key值
char * dictionary_get(dictionary * d, const char * key, char * def);

//设置dictionary对象的key值
int dictionary_set(dictionary * vd, const char * key, const char * val);

//删除dictionary对象的key值
void dictionary_unset(dictionary * d, const char * key);

//保存dictionary对象
void dictionary_dump(dictionary * d, FILE * out);
