#include <iostream>
#include <vector>
#include <string>

#define PREF_FUNC_OUTPUT 1
#define KMP_FUNC_OUTPUT 1
#define NUM 1

using namespace std;


void output(string s, vector<int> p, int i, int pref){
#if PREF_FUNC_OUTPUT == 1
    cout<<"---------------------------------------------------------------------------------\n";
    cout<<"Checking a substring length "<<i+1<<": ";
    for(int k = 0; k < s.length(); k++){
        if(k <= i)
            cout<<char(s[k]-32);
        else
            cout<<s[k];
    }
    cout<<"\nPrevious prefix value: "<<pref<<"\n";
    cout<<"Check if prefix and suffix match with length: "<<pref+1<<". Prefix: ";
    for(int k = 0; k <=pref; k++){
        cout<<char(s[k]-32);
    }
    cout<< " and suffix: ";
    for(int k = i-pref; k <=i; k++){
        cout<<char(s[k]-32);
    }
    cout<<"\nCurrent function prefix values for substrings:\n";
    for(int k = 0; k < s.length(); k++) {
        if(k <= i)
            cout<<'|'<<char(s[k]-32)<<'|';
        else
            cout<<'|'<<s[k]<<'|';
    }
    cout<<endl;
    for(int k = 0; k < s.length(); k++) {
        cout<<"|"<<p[k]<<"|";
    }
    cout<<"\n---------------------------------------------------------------------------------\n";
#endif
}

vector<int> prefix_func(string str) { // префикс функция
    vector<int> p(str.length(), 0);// вектор P, который будем заполнять значениями префикс функции
    for (int i = 1, j = 0; i < str.length();) { // проходим по всем символам исходной строки
        if (str[i] == str[j]) { // если равны, то записываем в p[i] текущую длину префикса + 1 (так как индексация с нуля)
            //проверяем, совпадают ли добавляемый в суффикс и следующий префикса символы
#if PREF_FUNC_OUTPUT == 1
            output(str, p, i, j);
            printf("Prefix of length %d matches substring suffix\n", j+1);
            cout<<"Moves to the next substring.\n";
#endif
            p[i++] = j + 1; // и смотрим следущий символ
            j++;

        }
        else if (j == 0) { //если текущая длина префикса ноль, то записываем ноль
#if PREF_FUNC_OUTPUT == 1
            output(str, p, i, j);
            cout<<"No matching prefix and suffix for this substring!\n";
            cout<<"Moves to the next substring.\n";
#endif
            p[i++] = 0;
        }else{ // иначе, сравниваем s[i] и s[p[j-1]]
#if PREF_FUNC_OUTPUT == 1
            output(str, p, i, j);
            cout<<"Checking the length prefix j = p[j-1] = "<<p[j-1]<<"\n";
#endif
            j = p[j - 1];
        }
    }
#if PREF_FUNC_OUTPUT==1
    cout<<"All substrings checked!\n";
    cout<<"\t\t\t\tLET'S CONTINUE THE KMP ALGORITHM!\n";
#endif
    return p;
}

void compare_out(string s, string t, int s_ind, int t_ind){
#if KMP_FUNC_OUTPUT == 1
    cout<<"--------------------------------------------------------------------------------------------------\n";
    cout<<"Compare characters "<<char(t[t_ind]-32)<<" and "<<char(s[s_ind]-32)<<'\n';
    cout<<"\tTEXT:      ";
    for(int k = 0; k < t.length(); k++){
        if(k == t_ind)
            cout<<char(t[k]-32);
        else
            cout<<char(t[k]);
    }
    cout<<endl;
    cout<<"\tSUBSTRING: ";
    for(int k = 0; k < t_ind-s_ind; k++)
        cout<<'-';
    for(int k = 0; k < s.length(); k++){
        if(k == s_ind)
            cout<<char(s[k]-32);
        else
            cout<<char(s[k]);
    }
    for(int k = (t_ind - s_ind + s.length()); k < t.length(); k++)
        cout<<'-';
    cout<<endl;
#endif
}

vector<int> KMP(string sample, string text) {
    int sample_length = sample.length();
    int text_length = text.length();
    vector<int> entries;//массив индексов вхождений
    int last_prefix;
    vector<int> prefix = prefix_func(sample);
    last_prefix = 0;
    for (int i = 0; i < text_length; i++) {//проходим по всем символам строки
        compare_out(sample, text, last_prefix, i);
        while (last_prefix > 0 && sample[last_prefix] != text[i]){
            last_prefix = prefix[last_prefix - 1];
#if KMP_FUNC_OUTPUT == 1
            cout<<"Characters don't match! Checking the next prefix value.\n";
            compare_out(sample, text, last_prefix, i);
#endif
        }
        if (sample[last_prefix] == text[i]) {
#if KMP_FUNC_OUTPUT == 1
            if(last_prefix != sample_length -1)
                cout<<"Characters match! Continue the comparison\n";
#endif
            last_prefix++;
            if (last_prefix == sample_length) {
                entries.push_back(i + 1 - sample.length());
#if KMP_FUNC_OUTPUT == 1
                cout<<"Found substring with index = "<< i+1-sample_length<< " This is: ";
                for(int k = 0; k < text_length; k++){
                    if((k > i-sample_length) && (k <= i))
                        cout<<char(text[k]-32);
                    else
                        cout<<char(text[k]);
                }
                cout<<" Decrease prefix value to prefix[last_prefix - 1] and continue search!\n";
#endif
#if NUM != 1
                break;
#endif
                last_prefix = prefix[last_prefix - 1];
            }
            continue;
        }
#if KMP_FUNC_OUTPUT == 1
        if((i + sample_length - last_prefix) >= text_length){
            cout<<"Further verification is meaningless\n";
            break;
        }
        cout<<"Characters don't match and prefix value is 0! Checking the next symbol in text.\n";
#endif
    }
#if KMP_FUNC_OUTPUT == 1
    cout<<"--------------------------------------------------------------------------------------------------\n";
    if(!entries.empty())
        cout<<"All entries found!\n";
    else
        cout<<"No entries found!\n";
#endif
    if(entries.empty())
        entries.push_back(-1);
    return entries;
}

int KMP_Cycle(string sample, string text) {
    int res;
#if KMP_FUNC_OUTPUT == 1
    printf("Checking string lengths: sample = %d, text = %d\n", sample.length(), text.length());
#endif
    //проверяем соответствие длин строк
    if(sample.length() != text.length()){
#if KMP_FUNC_OUTPUT == 1
        cout<<"The lengths are different, so the first string is not a cyclic shift of the second. Return -1\n";
#endif
        res = -1;
        return res;
    }
#if KMP_FUNC_OUTPUT == 1
    cout<<"Line lengths are the same. Let's concatenate string '" << text << "' with itself and then search string '"<< sample << "' in '" << text+text << "'\n";
#endif
    //запускаем алгоритм КМП
    text = text+text;
    res = KMP(sample, text)[0];
    return res;
}

int main() {
    string sample;
    string text;
    cin >> sample;
    cin >> text;
#if NUM == 1
    vector<int> ans = KMP(sample, text);
    if (ans.size() == 1)
        cout << -1;
    else {
        for (int i = 0; i < ans.size() - 1; i++) {
            cout << ans[i] << ',';
        }
        cout << ans.back();
    }
#else
    int ans = KMP_Cycle(text, sample);
    cout << ans;
#endif
    return 0;
}
