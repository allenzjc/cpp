
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <deque>
#include <list>

#include <sys/time.h>


// =======================  string   start  ============================
class String
{
private:
    char* str=NULL;//must initialize!
    int len=0;

public:
    //default constructor
    String() {}
    ~String()
    {
        delete[] str;
    }

    //param constructor
    String (const char* str_)
    {
        if (str_)
        {
            len=strlen(str_);
            str=new char [len+1];
            strcpy(str,str_);//str[len]='\0';
        }
    }

    //copy constructor
    String (const String& str_):str(new char [str_.len+1]),len(str_.len)
    {
        strcpy(str,str_.str);
    }

#if 0
    //move copy constructor, NO need ?
    String(String&& str_) noexcept :str(str_.str),len(str.size())
    {
        str_.str=NULL;
        str_.len=0;
    }
#endif

    friend void swap (String &str1,String &str2)
    {
        using std::swap;
        swap(str1.str,str2.str);
        swap(str1.len,str2.len);
    }

#if 1
    //move assign
    String& operator= (String&& str_)
    {
        delete[] str;

        str=str_.str;
        len=str_.len;

        str_.str=NULL;
        return *this;
    }
    //copy assign
    String& operator= (const String& str_)
    {
        if (this!=&str_)
        {
            String tmp(str_);
            swap(*this,tmp);
        }
        return *this;
    }
#else
    //copy assign
    String& operator= (String str_)
    {
        swap(str_);
        return *this;
    }
#endif


    //no need. for inline bool operator== (const String& lhs,const String& rhs)
#if 0
    bool operator== (const String& rhs) const
    {
        if (this==&rhs)
        {
            return true;
        }
        return compare(rhs)==0;
    }
#endif

#if 0
    String(std::initializer_list<String> list)
    {
    }
#endif
    int size (void) const
    {
        return len;
    }
    const char * c_str(void) const
    {
        return str;
    }
    char * c_str(void)
    {
        return str;
    }
    char* data (void)
    {
        return str;
    }

    char operator[] (int pos)
    {
        return str[pos];
    }
    char operator[] (int pos) const
    {
        return str[pos];
    }

    const char* operator() (void)
    {
        return str;//using this func, you can std::string str;printf("%s", str());
    }

    int compare (const String& str_) const
    {
        //const int len = std::min(str_size, rhs.str_size);
        return strcmp(str,str_.str);
    }
    inline bool operator< (String& str_)
    {
        return compare(str_)<0;
    }

    String& append (const String& str_)
    {
        int len=this->len+str_.len;

        char * new_str=new char[len+1];
        strcpy(new_str,str);
        strcpy(new_str+this->len,str_.str);
        new_str[len]=0;

        delete[] str;
        str=new_str;
        this->len=len;
        return *this;
    }

    String& operator+= (const String& str_)
    {
        return append(str_);
    }

};

String operator+ (const String& lhs, const String& rhs)
{
    String str_(lhs);
    str_.append(rhs);
    return str_;
}

inline bool operator!= (const String& lhs,const String& rhs)
{
    //return !(lhs==rhs); //cannot compile, why ?

    if (&lhs==&rhs)
    {
        return false;
    }
    return lhs.compare(rhs);
}

// for unordered_map
inline bool operator== (const String& lhs, const String& rhs)
{
    if (&lhs==&rhs)
    {
        return true;
    }
    return lhs.compare(rhs)==0;
}

//for map
inline bool operator< (const String& lhs,const String& rhs)
{
    return lhs.compare(rhs)<0;
}

//for cout
std::ostream& operator<< (std::ostream& out,const String& str)
{
    return std::__ostream_insert(out, str.c_str(), str.size());
}


namespace std
{
//template<typename _Tp> struct hash;

// for unordered_map
template<>
struct hash<String>
{
    size_t operator() (const String& str) const noexcept
    {
        size_t hash=0;
        for (int i = 0; i < str.size(); i++)
        {
            hash = 31 * hash + str[i];
        }
        return hash;
        //return std::_Hash_impl::hash(str.data(), str.size());
    }
};
}

auto t=[]()->int
{
    String str1="Hello ";
    String str2="world!";
    printf("str=%s\n",(str1+str2)());
    return 0;
}();
// =======================  string   end  ============================



using std::vector;
using std::string;
using std::deque;
using std::list;

const int loop_small= 10;
const int loop_total= 100000000;
const int loop=loop_total/loop_small;
char buffer[loop_small];

void push_back_test (void)
{
    struct timeval tv1;
    struct timeval tv2;

    gettimeofday(&tv1, NULL);

    char* p;
    for(int i=0; i<loop; i++)
    {
        p=new char[loop_small];
        for (int j=0; j<loop_small; j++)
        {
            p[j]=buffer[j]+'c';
        }
        delete[] p;
    }
    gettimeofday(&tv2, NULL);
    printf("array push_back time is %ld ms\n",(tv2.tv_sec-tv1.tv_sec)*1000+(tv2.tv_usec-tv1.tv_usec)/1000);

    gettimeofday(&tv1, NULL);
    for(int i=0; i<loop; i++)
    {
        for (int j=0; j<loop_small; j++)
        {
            string str;
            str.push_back(buffer[j]+'c');
        }
    }
    gettimeofday(&tv2, NULL);
    printf("string push_back time is %ld ms\n",(tv2.tv_sec-tv1.tv_sec)*1000+(tv2.tv_usec-tv1.tv_usec)/1000);

    gettimeofday(&tv1, NULL);
    for(int i=0; i<loop; i++)
    {
        for (int j=0; j<loop_small; j++)
        {
            vector<char> vec;
            vec.push_back(buffer[j]+'c');
        }
    }
    gettimeofday(&tv2, NULL);
    printf("vector push_back time is %ld ms\n",(tv2.tv_sec-tv1.tv_sec)*1000+(tv2.tv_usec-tv1.tv_usec)/1000);

    gettimeofday(&tv1, NULL);
    for(int i=0; i<loop; i++)
    {
        for (int j=0; j<loop_small; j++)
        {
            deque<char> deq;
            deq.push_back(buffer[j]+'c');
        }
    }
    gettimeofday(&tv2, NULL);
    printf("deque push_back time is %ld ms\n",(tv2.tv_sec-tv1.tv_sec)*1000+(tv2.tv_usec-tv1.tv_usec)/1000);

    gettimeofday(&tv1, NULL);
    for(int i=0; i<loop; i++)
    {
        for (int j=0; j<loop_small; j++)
        {
            list<char> lst;
            lst.push_back(buffer[j]+'c');
        }
    }
    gettimeofday(&tv2, NULL);
    printf("list push_back time is %ld ms\n",(tv2.tv_sec-tv1.tv_sec)*1000+(tv2.tv_usec-tv1.tv_usec)/1000);

}

int main (void)
{
    //push_back_test();
}
