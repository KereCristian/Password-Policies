#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

class Policy{
protected:
    bool isChecked;
public:
    virtual void check(const string & parola)= 0;
    bool getCheck()const{
        return isChecked;
    }
};

class LengthPolicy:public Policy{
protected:
    uint16_t minLength;
    uint16_t maxLength;
public:
    LengthPolicy(uint16_t min){
        minLength = min;
        maxLength = 255;
    }
    LengthPolicy(uint16_t min, uint16_t max){
        minLength = min;
        maxLength = max;
    }
    void check(const string & parola){
        if(parola.size() >= minLength && parola.size() <= maxLength) isChecked = true;
        else isChecked = false;
    }
};

class ClassPolicy:public Policy{
private:
    uint16_t minClassCount;
public:
    ClassPolicy(uint16_t min){
        minClassCount = min;
    }
    void check(const string & parola){
        char s[256];
        strcpy(s,parola.c_str());
        int okA=0,oka=0,ok_=0,ok1=0;
        for(int i = 0; i< strlen(s); i++){
            if(isupper(s[i])){
                okA=1;
            }
            if(islower(s[i])){
                oka=1;
            }
            if(isdigit(s[i])){
                ok1=1;
            }
            if(isupper(s[i]) == 0 && islower(s[i]) == 0 && isdigit(s[i]) == 0){
                ok_=1;
            }
        }
        if((ok1+okA+oka+ok_) >= minClassCount) isChecked = true;
        else isChecked = false;
    }
};

class IncludePolicy:public Policy{
private:
    char characterType;
public:
    IncludePolicy(char tip){
        characterType = tip;
    }
    void check(const string & parola){
        isChecked = false;
//        char s[256];
//        strcpy(s,parola.c_str());
        if(characterType == 'A'){
            for (int i = 0; i < parola.size(); i++) {
                if (isupper(parola[i]))
                    isChecked = true;
            }
        }
        if(characterType == 'a'){
            for(int i = 0; i< parola.size(); i++){
                if(islower(parola[i]))
                    isChecked = true;
            }
        }
        if(characterType == '0'){
            for(int i = 0; i< parola.size(); i++){
                if(isdigit(parola[i]))
                    isChecked = true;
            }
        }
        if(characterType == '$'){
            for(int i = 0; i<parola.size(); i++){
                if(!isalnum(parola[i]))
                    isChecked = true;
            }
        }
    }
};

class NotIncludePolicy:public Policy{
private:
    char characterType;
public:
    NotIncludePolicy(char tip){
        characterType = tip;
    }
    void check(const string & parola){
        isChecked = true;
//        char s[256];
//        strcpy(s,parola.c_str());
        if(characterType == 'A'){
            for (int i = 0; i < parola.size(); i++) {
                if (isupper(parola[i]))
                    isChecked = false;
            }
        }
        if(characterType == 'a'){
            for(int i = 0; i< parola.size(); i++){
                if(islower(parola[i]))
                    isChecked = false;
            }
        }
        if(characterType == '0'){
            for(int i = 0; i< parola.size(); i++){
                if(isdigit(parola[i]))
                    isChecked = false;
            }
        }
        if(characterType == '$'){
            for(int i = 0; i<parola.size(); i++){
                if(!isalnum(parola[i]))
                    isChecked = false;
            }
        }
    }
};

class RepetitionPolicy:public Policy{
private:
    uint16_t maxCount;
public:
    RepetitionPolicy(uint16_t max){
        maxCount = max;
    }
    void check(const string & parola){
        int ok=1, max=0;
        for(int i=1; i<parola.size(); i++){
            if(parola[i]==parola[i-1]) {
                ok++;
                if(ok>max) max=ok;
            }
            else {
                ok=1;
            }
        }
        if(maxCount>=max)
            isChecked=true;
        else isChecked=false;
//        cout << parola << " " << max<<endl;
    }
};

class ConsecutivePolicy:public Policy{
private:
    uint16_t maxCount;
public:
    ConsecutivePolicy(uint16_t max){
        maxCount = max;
    }
    void check(const string & parola) {
        int k = 1;
        int max = 0;
        for (int i = 1; i < parola.size(); i++) {
            if (parola[i] - 1 == parola[i - 1]) {
                k++;
                if (k > max) max = k;
            } else k = 1;
        }
        if(max <= maxCount) isChecked = true;
        else isChecked = false;
//        cout << parola << " " << max<<endl;
    }
};

string checkPassword(string parola, vector<Policy*> v){
    int k = 0;
    for(int i = 0; i<v.size(); i++){
        v[i]->check(parola);
        if(v[i]->getCheck() == true) k++;
    }
    if(k == v.size()) return "OK";
    else return "NOK";
}

int main(){
    vector <Policy*> v;
    int n,min_class_count, max_count,minim,max;
    char clasa;
    string regula,parola;
    cin >> n;
    for(int i = 0; i<n; i++){
        cin >> regula;
        if(regula == "class"){
            cin >> min_class_count;
            v.push_back(new ClassPolicy(min_class_count));
        }
        if(regula == "include"){
            cin >> clasa;
            v.push_back(new IncludePolicy(clasa));
        }
        if(regula == "ninclude"){
            cin >> clasa;
            v.push_back(new NotIncludePolicy(clasa));
        }
        if(regula == "repetition"){
            cin >> max_count;
            v.push_back(new RepetitionPolicy(max_count));
        }
        if(regula == "consecutive"){
            cin >> max_count;
            v.push_back(new ConsecutivePolicy(max_count));
        }
        if(regula == "length"){
            cin >> minim;
            if(getchar() == ' '){
                cin >> max;
                v.push_back(new LengthPolicy(minim,max));
            }
            else v.push_back(new LengthPolicy(minim));
        }
    }
    while(cin >> parola){
        cout << checkPassword(parola,v)<<endl;
    }

    return 0;
}
