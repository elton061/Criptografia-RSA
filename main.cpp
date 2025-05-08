#include <iostream>

using namespace std;

int main()
{

    int p, q, n, z, d;
    int candidato = 2;

    cout << "Digite um numero primo para representar P: ";
    cin >> p;
    cout << "Informe um outro numero primo para representar Q: ";
    cin >> q;

    //Calculando N e Z
    n = p * q;
    z = (p - 1) * (q - 1);

    //calculando D (coprimo de Z) utilizando MDC.
    int x = z;
    int y = candidato;
    int resto;
    do{
        do{
            resto = x%y;
            if(resto == 0)
                break;
            x = y;
            y = resto;}while(resto != 0);
        candidato++;
    }while(y != 1);

    //D recebe o numero candidato a coprimo apos a verificacao menos 1 para ignorar a ultima linha do looping que incrementa o candidato.
    d = candidato - 1;

    return 0;
}
