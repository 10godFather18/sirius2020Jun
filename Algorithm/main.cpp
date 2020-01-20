#include <iostream>
#include <fstream>
#include <strstream>
#include <vector>

using namespace std;

struct substits{string el[2];bool isFinal;};
struct workSch{string word; bool stop;};

string sub(int start, int fin, string str){
    string subStr;
    for(int i = start; i < fin; i++){
        subStr += str[i];
    }
    return subStr;
}

int firstSubsting(string str, string substring){
    if(substring.size() > str.size())
        return -1;
    int start = -1;
    for(int i = 0; i <= str.size()-substring.size(); i++)
    {
        if(sub(i, i+substring.size(), str) == substring)
        {
            start = i;
            break;
        }
    }
    return start;
}

class Schema{
    public:
        bool getIsFinal(){
            return isFinal;
        }
        string getAfterSubstit(){
            return afterSubstit;
        }
        string getBeforeSubstit(){
            return beforeSubstit;
        }
        vector<substits> getSubs(){
            return subs;
        }
        void setIsFinal(){

        }
        void setAfterSubstit(){

        }
        void setBeforeSubstit(){

        }
        void setSubs(vector<substits> preSubs){
            subs = preSubs;
        }
        substits parseStrToSchema(string input){
            substits schema;
            int separ = 0;
            schema.isFinal = false;
            separ = firstSubsting(input, " ");
            schema.el[0] = input.substr(0, separ);
            input = sub(separ+1, input.size(), input);
            if(input[2] == '.')
            {
                schema.isFinal = true;
            }
            separ = firstSubsting(input, " ");
            schema.el[1] = sub(separ+1, input.size(), input);
            return schema;
        }
        vector<string> parseFileToStrings(string strSchema)
        {
            vector<string> vecSchema;
            int newStr = 0;
            int i = 0;
            while(firstSubsting(strSchema, "\n") != -1){
                newStr = firstSubsting(strSchema, "\n");
                vecSchema.push_back(sub(0, newStr, strSchema));
                strSchema=sub(newStr+1, strSchema.size() , strSchema);
                i = newStr;
            }
            vecSchema.push_back(strSchema);
            return vecSchema;
        }
        void parseFileToSchema(string strSchema){
            vector<string> vecSchema;
            vector<substits> preSubs;
            vecSchema = parseFileToStrings(strSchema);
            for(int i = 0; i < vecSchema.size(); i++)
            {
                preSubs.push_back(parseStrToSchema(vecSchema[i]));
            }
            setSubs(preSubs);
        }
    private:
        bool isFinal;
        string afterSubstit;
        string beforeSubstit;
        vector<substits> subs;
};



string applySubstit(string str, string bef, string aft)
{
    int start = firstSubsting(str, bef);
    if(start == 0)
        return aft + str.substr(start + bef.size(), str.size());
    if(start != -1)
        str = str.substr(0, start) + aft + str.substr(start + bef.size(), str.size());
    return str;
}

void TestApplySubsit()
{
    if(applySubstit("", "aa", "b") != "")
        cout << ("Error test 1");
    if(applySubstit("a", "a", "ab") != "ab")
        cout << ("Error test 2");
    if(applySubstit("aaa", "aa", "") != "a")
        cout << ("Error test 3");
    if(applySubstit("aaa", "aa", "b") != "ba")
        cout << ("Error test 4");
    if(applySubstit("a", "ab", "b") != "a")
        cout << ("Error test 5");
    if(applySubstit("aab", "ab", "b") != "ab")
        cout << ("Error test 6");
    if(applySubstit("abcd", "x", "xy") != "abcd")
        cout << ("Error test 7");
    if(applySubstit("cba", "ba", "ab") != "cab")
        cout << ("Error test 8");
}

workSch applySchemaOnce(vector<substits> schema, string word){
    workSch a;
    for(int i = 0; i < schema.size(); i++){
        if(firstSubsting(word, schema[i].el[0]) != -1){
            a.word = applySubstit(word, schema[i].el[0], schema[i].el[1]);
            a.stop = schema[i].isFinal;
            return a;
        }
    }
}

string applySchema(vector<substits> schema, string word){
    workSch a;
    while(true){
        a = applySchemaOnce(schema, word);
        if(a.stop){
            return a.word;
        }
        word = a.word;
    }
}

int main()
{
    string word = "";
    cin >> word;
    string m = "";
    Schema a;
    substits str;
    TestApplySubsit();
    string path = "";
    int i = 1;
    ifstream f;
    f.open("Input.txt");
    while(!f.eof())
    {
        f >> path;
        m += path;
        if(i%3!=0)
            m+= " ";
        else
            m+="\n";
        i++;
    }
    f.close();
    m = m.substr(0, m.size()-1);
    a.parseFileToSchema(m);
    cout << applySchema(a.getSubs(), word);
    return 0;
}
