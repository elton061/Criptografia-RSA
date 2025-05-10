#include <iostream>
#include <limits>
#include <cstring>

const int tamanhoVetor = 500;

using namespace std;

void converteParaAscii(char textoOriginal[tamanhoVetor], int ascii[tamanhoVetor]);

void converteParaTexto(char textoOriginal[tamanhoVetor], int ascii[tamanhoVetor]);

void criptografar(int ascii[tamanhoVetor], unsigned long long textoCriptografado[tamanhoVetor], int e, int n);

void descriptografar(unsigned long long textoCriptografado[tamanhoVetor], int ascii[tamanhoVetor], int d, int n, char textoOriginal[tamanhoVetor]);

void AdicionarTexto(char textoOriginal[tamanhoVetor]);

void CalculandoChavesPrimarias(int &p, int &q, int &z, int &n);

int main()
{

    int p, q, n, z, d, e;
    char texto[tamanhoVetor]={}, textoManipulado[tamanhoVetor]={};
    int textoAscii[tamanhoVetor] = {};
    unsigned long long textoCriptografado[tamanhoVetor] = {};   //utilizei esse tipo para evitar estouro.
    int opc;
    bool statusCriptografado = false, temTexto = false;     //controladores para o menu.

    //Gera a chave privada e publica
    CalculandoChavesPrimarias(p, q, z, n);

    //Menu para manipular as funcionalidades do programa
    do{
        cout << "\t\t\tMENU PRINCIPAL\n\n"
             << "1) Adicionar um texto.\n"
             << "2) Exibir o texto.\n"
             << "3) Criptografar o texto.\n"
             << "4) Descriptografar o texto.\n"
             << "5) Exibir texto criptografado.\n"
             << "6) Alterar chaves primarias.\n"
             << "9) Sair do programa\n\n";
        cout << "Escolha uma das opcoes acima: ";
        cin >> opc;
        //limpa o buffer.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(opc){
            case 1:
                AdicionarTexto(texto);
                strcpy(textoManipulado, texto);
                temTexto = true;
                cout << "\n\nTexto adicionado com sucesso!\n";
                break;

            case 2:
                if(!temTexto){
                    cout << "\n\nNenhum texto foi adicionado ainda :(\n\n" << endl;
                }else{
                    cout << "\n\nTexto Original = " << texto << "\n\n";
                }
                break;

            case 3:
                converteParaAscii(textoManipulado, textoAscii);

                cout << "\n\nDigite a sua chave publica E: " << endl;
                cin >> e;

                criptografar(textoAscii, textoCriptografado, e, n);

                cout << "\n\nTexto Criptografado = ";
                for(int i = 0; textoCriptografado[i] != 0; i++){
                    cout << textoCriptografado[i] << " ";
                }
                cout << "\n\n";
                statusCriptografado = true;
                break;

            case 4:
                if(!statusCriptografado){
                    cout << "\n\nAinda nao foi feito uma criptografia! ";
                }else{
                cout << "\n\nDigite a sua chave privada D: " << endl;
                cin >> d;
                descriptografar(textoCriptografado, textoAscii, d, n, textoManipulado);

                cout << "\n\nDescriptografado com sucesso!";
                cout << "\n\nTexto Apos descriptografar = " << textoManipulado << "\n\n";
                }
                break;

            case 5:
                if(!statusCriptografado){
                    cout << "\n\nAinda nao foi feito a criptografia :(\n";
                }else{
                    cout << "\n\nTexto Criptografado = ";
                    for(int i = 0; textoCriptografado[i] != 0; i++){
                        cout << textoCriptografado[i] << " ";
                    }
                }
                cout << "\n\n";
                break;

            case 6:
                CalculandoChavesPrimarias(p, q, z, n);
                cout << "\n\nChave privada = " << d << endl;
                cout << "Chave publica = " << e << endl;
                break;

            case 9:
                break;

            default:
                cout << "\n\nOpcao Invalida! \n";
                system("pause");
                system("cls");
                break;
            }
            system("pause");
            system("cls");
    }while(opc != 9);
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
void criptografar(int ascii[tamanhoVetor], unsigned long long textoCriptografado[tamanhoVetor], int e, int n){
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

void descriptografar(unsigned long long textoCriptografado[tamanhoVetor], int ascii[tamanhoVetor], int d, int n, char textoOriginal[tamanhoVetor]){
    // necessario o i esta fora do for para que eu consiga adicionar o caractere nulo no final do vetor do texto original.
    int i;

    for(i = 0; textoCriptografado[i] != 0; i++){
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

    for(i = 0; ascii[i] != 0; i++){
        textoOriginal[i] = static_cast<char>(ascii[i]);
    }

    textoOriginal[i] = '\0';
}

void CalculandoChavesPrimarias(int &p, int &q, int &z, int &n){
    int d, e;

    cout << "Digite um numero primo para representar P: " << endl;
    cin >> p;
    cout << "Informe um outro numero primo para representar Q: " << endl;
    cin >> q;

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
    //apos o while o e recebe o valor candidato que satisfaz a formula.
    e = candidato;

    cout << "\n\nAnote cuidadosamente suas chaves:"
         << "\nChave Privada D = " << d
         << "\nChave publica E = " << e << endl;

         system("pause");
         system("cls");
}

void AdicionarTexto(char textoOriginal[tamanhoVetor]){
    // Obtendo o texto e guardando no vetor de char para conversao.
    cout << "\n\nDigite o texto a ser criptografado: " << endl;
    cin.getline(textoOriginal, tamanhoVetor);
}

