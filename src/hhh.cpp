#include <vector>
#include <string>
#include <iostream>

int main(){
    std::vector<std::vector<std::string>> hhh;
    for(int i=0;i<10;i++){
        hhh.push_back({"2313123", "HFIOSHO_DF", "FDAAFL"});
    }
    for(int i=0;i<10;i++){
        for(int j=0;j<3;j++){
            std::cout << hhh[i][j] << std::endl;
        }
        std::cout << std::endl;
    }
}