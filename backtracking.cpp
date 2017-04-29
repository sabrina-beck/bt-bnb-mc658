
#include<vector>

using namespace std;

int weights[] = {2, 3, 1}, values[] = {6, 15, 7};

int solution = 0, n = 3;

std::vector<int> vsol;
std::vector<int> temp;

bool issol;

// achamos que value no caso do lab é preço p
// subtrai de B o número de ds para o numero de classes usadas na solução
// adicionar timeout
void Knapsack (int i, int max, int value) {
  for (int k = i; k < n; k++) {
    // verificar tamanho t de vetor secundário com as classes q estao na mochila
    // max - t tem que ser maior q zero
    if ( max > 0 ) {

        if (weights[k] <= max) {
          temp.push_back(k);

          if (value + values[k] >= solution) {
            // verificar tamanho t de vetor secundário com as classes q estao na mochila
            // max - t tem que ser maior q zero
            solution = value + values[k];
            issol = true;
          }
        }

        if ( (k+1) < n) {

          Knapsack (k+1, max - weights[k], value + values[k]);
        
        } else {
          if (issol == true) {
            if (!vsol.empty()) {
                vsol.clear();
            }

            std::move(temp.begin(), temp.end(), std::back_inserter(vsol));
            temp.clear();
            issol = false;
          } else { 
            temp.clear();
          }

          return;
        }
    }
    else {
        if (issol == true) {
            if (! vsol.empty()) vsol.clear();
            std::move(temp.begin(), temp.end(), std::back_inserter(vsol));
            temp.clear();
            issol = false;
        } else { 
          temp.clear();
        }

        return;
    }
  }
}
