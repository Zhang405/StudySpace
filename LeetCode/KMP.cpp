#include<string>
#include<iostream>
int* buildNext(std::string p){
    int m = p.size();
    int* next = new int[m];
    int j =-1;
    next[0] = -1;
    int i = 0;
    while(i<m-1){
        if(j<0||p[i] == p[j]){
            next[++i] = ++j;
        }
        else{
            j = next[j];
        }
    }
    return next;
}

int kmpMatch(std::string text,std::string parttern){
    int* next = buildNext(parttern);
    for(int i = 0; i<parttern.size();i++){
        std::cout<<next[i]<<' ';
    }
    std::cout<<std::endl;
    int m = text.size();
    int n = parttern.size();
    int i = 0;
    int j = 0;
    while(i<m&&j<n){
        if(j<0||text[i] == parttern[j]){
            i++;
            j++;
        }
        else{
            j = next[j];
        }
    }
    delete[] next;
    return j;
}

int main(){
    std::string text;
    std::string parttern;
    std::cin>>text;
    std::cout<<"The Text String is: "<<text<<std::endl;
    std::cin>>parttern;
    std::cout<<"The Parttern String is: "<<parttern<<std::endl;
    int error = kmpMatch(text,parttern);
    std::cout<<"The Match Result is: "<<error<<std::endl;
    return 0;
}







