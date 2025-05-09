#include <iostream>
#include <cmath>

const int tamanhoVetor = 500;

using namespace std;

void converteParaAscii(char textoOriginal[tamanhoVetor], int ascii[tamanhoVetor]);

void converteParaTexto(char textoOriginal[tamanhoVetor], int ascii[tamanhoVetor]);

void criptografando(int ascii[tamanhoVetor], unsigned long long textoCriptografado[tamanhoVetor], int e, int n);

void descriptografando(unsigned long long textoCriptografado[tamanhoVetor], int ascii[tamanhoVetor], int d, int n);

int main()
{

    int p, q, n, z, d, e;
    char texto[tamanhoVetor];
    int textoAscii[tamanhoVetor] = {};
    unsigned long long textoCriptografado[tamanhoVetor] = {};   //utilizei esse tipo para evitar estouro.
    char opc;

    cout << "Digite um numero primo para representar P: " << endl;
    cin >> p;
    cout << "Informe um outro numero primo para representar Q: " << endl;
    cin >> q;

    // Obtendo o texto e guardando no vetor de char para conversao.
    cout << "\n\nDigite o texto a ser criptografado: " << endl;
    cin.ignore();
    cin.getline(texto, tamanhoVetor);

    //convertendo o texto para base numerica ascii
    converteParaAscii(texto, textoAscii);

//CALCULANDO VALORES NECESSARIOS **********
    //Calculando N e Z
    n = p * q;
    z = (p - 1) * (q - 1);

    //calculando D (coprimo de Z) utilizando MDC.
    int x;
    int candidato = 2;
    int resto;
    int y;
    do{
        x = z;
        y = candidato;
        //Calcula MDC de x e y
        do{
            resto = x%y;
            if(resto == 0)
                break;
            x = y;
            y = resto;}while(resto != 0);
        candidato++;
    //caso o valor encontrado como mdc nao for igual a 1 ele repete o processo incrementando o candidato.
    }while(y != 1);

    //D recebe o numero candidato a coprimo apos a verificacao menos 1 para ignorar a ultima linha do looping que incrementa o candidato.
    d = candidato - 1;

    //Encontrando um numero que satisfaca (E * D) mod Z = 1.
    candidato = 2;
    while((candidato * d) % z != 1)
        candidato++;
    //apos o while o e recebe o valor candidato que satisfez a formula.
    e = candidato;

    system("cls");

    //Menu para manipular as funcionalidades do programa
    do{
        char opcValida;

        do{
            opcValida=true;

            cout << "\t\t\tMENU PRINCIPAL\n\n";
            cout << "1) Adicionar um texto.\n"
                 << "2) Exibir o texto.\n"
                 << "3) Criptografar o texto.\n"
                 << "4) Descriptografar o texto.\n"
                 << "9) Sair do programa\n\n";
            cout << "Escolha uma das opcoes acima: ";
            cin >> opc;

            if(opc != '1' && opc != '2' && opc != '3' && opc != '4' && opc !='9'){
                opcValida = false;
                cout << "\n\nOpcao invalida!";
                system("pause");
                system("cls");
            }
        }while(opcValida != true);
    }while(opc != '9');
}

void converteParaAscii(char textoOriginal[tamanhoVetor], int ascii[tamanhoVetor]){
    //convertendo o texto para a base ascii
    for(int i = 0; textoOriginal[i] != '\0'; i++){
        ascii[i] = static_cast<int>(textoOriginal[i]); // converte tipo char para int.
    }
}

void converteParaTexto(char textoOriginal[tamanhoVetor], int ascii[tamanhoVetor]){
    //convertendo o texto no formato ASCII para formato original novamente
    for (int i = 0; ascii[i] != 0; i++){
        textoOriginal[i] = static_cast<char>(ascii[i]); //convertendo tipo int para char.
    }
}

//E preciso utilizar exponenciacao rapida modular para otimizar o tempo para calcular e evitar estouros, quebrando o calculo grande da potencia em pequenas partes.
void criptografando(int ascii[tamanhoVetor], unsigned long long textoCriptografado[tamanhoVetor], int e, int n){
    for(int i = 0; ascii[i] != 0; i++){
        unsigned long long resultado = 1;
        unsigned long long base = ascii[i] % n;
        int expoente = e;
        while(expoente > 0){
            if(expoente % 2 == 1){
                resultado = (resultado * base) % n;
            }
            base = (base * base) % n;
            expoente = expoente / 2;
        }
        textoCriptografado[i] = resultado;
    }
}

void descriptografando(unsigned long long textoCriptografado[tamanhoVetor], int ascii[tamanhoVetor], int d, int n){
    for(int i = 0; textoCriptografado[i] != 0; i++){
        unsigned long long resultado = 1;
        unsigned long long base = textoCriptografado[i] % n;
        int expoente = d;
        while(expoente > 0){
            if(expoente % 2 == 1){
                resultado = (resultado * base) % n;
            }
            base = (base * base) % n;
            expoente = expoente / 2;
        }
        ascii[i] = resultado;
    }
}
