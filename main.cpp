#include <iostream>
#include <limits>
#include <cstring>

const int tamanhoVetor = 500;

using namespace std;

unsigned long long mulmod(unsigned long long a, unsigned long long b, unsigned long long mod);

void converteParaAscii(char textoOriginal[tamanhoVetor], int ascii[tamanhoVetor], int &tamanhoTexto);

void converteParaTexto(char textoOriginal[tamanhoVetor], int ascii[tamanhoVetor]);

void criptografar(int ascii[tamanhoVetor], unsigned long long textoCriptografado[tamanhoVetor], unsigned long long  e, unsigned long long  n, int tamanhoTexto);

void descriptografar(unsigned long long textoCriptografado[tamanhoVetor], int ascii[tamanhoVetor], unsigned long long  d, unsigned long long  n, char textoOriginal[tamanhoVetor], int tamanhoTexto);

void AdicionarTexto(char textoOriginal[tamanhoVetor]);

void CalculandoChavesPrimarias(unsigned long long &p, unsigned long long  &q, unsigned long long  &z, unsigned long long  &n);

int main()
{

    unsigned long long p, q, n, z, d, e;
    char texto[tamanhoVetor]={}, textoManipulado[tamanhoVetor]={};
    int textoAscii[tamanhoVetor] = {}, tamanhoTexto=0;
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
                converteParaAscii(textoManipulado, textoAscii, tamanhoTexto);

                cout << "\n\nDigite a sua chave publica E: " << endl;
                cin >> e;

                criptografar(textoAscii, textoCriptografado, e, n, tamanhoTexto);

                cout << "\n\nTexto Criptografado = ";
                for(int i = 0; i < tamanhoTexto; i++){
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
                descriptografar(textoCriptografado, textoAscii, d, n, textoManipulado, tamanhoTexto);

                cout << "\n\nDescriptografado com sucesso!";
                cout << "\n\nTexto Apos descriptografar = " << textoManipulado << "\n\n";
                }
                break;

            case 5:
                if(!statusCriptografado){
                    cout << "\n\nAinda nao foi feito a criptografia :(\n";
                }else{
                    cout << "\n\nTexto Criptografado = ";
                    for(int i = 0; i < tamanhoTexto; i++){
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

void converteParaAscii(char textoOriginal[tamanhoVetor], int ascii[tamanhoVetor], int &tamanhoTexto){
    //convertendo o texto para a base ascii
    for(int i = 0; textoOriginal[i] != '\0'; i++){
        ascii[i] = static_cast<int>(textoOriginal[i]); // converte tipo char para int.
        tamanhoTexto++;     //Conta os caracteres para servir como ponto de parada para as funcoes de criptografia e descriptografia.
    }
}

void converteParaTexto(char textoOriginal[tamanhoVetor], int ascii[tamanhoVetor]){
    //convertendo o texto no formato ASCII para formato original novamente
    for (int i = 0; ascii[i] != 0; i++){
        textoOriginal[i] = static_cast<char>(ascii[i]); //convertendo tipo int para char.
    }
}

//E preciso utilizar exponenciacao rapida modular para otimizar o tempo para calcular e evitar estouros, quebrando o calculo grande da potencia em pequenas partes.
void criptografar(int ascii[tamanhoVetor], unsigned long long textoCriptografado[tamanhoVetor], unsigned long long  e, unsigned long long  n, int tamanhoTexto){
    for(int i = 0; i < tamanhoTexto; i++){
        unsigned long long resultado = 1;
        unsigned long long base = ascii[i] % n;
        unsigned long long  expoente = e;
        while(expoente > 0){
            if(expoente % 2 == 1){
                resultado = mulmod(resultado, base, n);
            }
            base = mulmod(base, base, n);
            expoente = expoente / 2;
        }
        textoCriptografado[i] = resultado;
    }
}

void descriptografar(unsigned long long textoCriptografado[tamanhoVetor], int ascii[tamanhoVetor], unsigned long long  d, unsigned long long  n, char textoOriginal[tamanhoVetor], int tamanhoTexto){
    // necessario o i esta fora do for para que eu consiga adicionar o caractere nulo no final do vetor do texto original.
    int i;

    for(i = 0; i < tamanhoTexto; i++){
        unsigned long long resultado = 1;
        unsigned long long base = textoCriptografado[i] % n;
        unsigned long long expoente = d;
        while(expoente > 0){
            if(expoente % 2 == 1){
                resultado = mulmod(resultado, base, n);
            }
            base = mulmod(base, base, n);
            expoente = expoente / 2;
        }
        ascii[i] = resultado;
    }

    for(i = 0; i < tamanhoTexto; i++){
        textoOriginal[i] = static_cast<char>(ascii[i]);
    }

    textoOriginal[i] = '\0';
}

void CalculandoChavesPrimarias(unsigned long long &p, unsigned long long &q, unsigned long long &z, unsigned long long &n) {
    unsigned long long d;
    unsigned long long e = 65537;  // valor fixo de E, bastante utilizado em RSA

    while (true) {
        cout << "Digite um numero primo para representar P: " << endl;
        cin >> p;
        cout << "Informe um outro numero primo para representar Q: " << endl;
        cin >> q;

        n = p * q;
        z = (p - 1) * (q - 1);

        // Calcular MDC entre E e Z
        unsigned long long a = e;
        unsigned long long b = z;
        unsigned long long resto;
        while (b != 0) {
            resto = a % b;
            a = b;
            b = resto;
        }
        unsigned long long mdc = a;

        if (mdc == 1) {
            // Se MDC == 1, podemos calcular d (inverso modular)
            break; // sai do while principal
        } else {
            cout << "\nO valor de e = 65537 nao eh coprimo com z = " << z << ". Escolha outros primos p e q.\n\n";
        }
    }

    // Encontrar D tal que (d * e) % z == 1
    d = 2;
    while ((d * e) % z != 1) {
        d++;
        // Para segurança, se d ficar muito grande (por ex, > z), interrompe
        if (d > z) {
            cout << "Nao foi possivel encontrar um d valido. Tente outros primos.\n";
            return;
        }
    }

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

//Funcao utilizada para calcular ((a * b) % mod) sem causar overflow
unsigned long long mulmod(unsigned long long a, unsigned long long b, unsigned long long mod){
    unsigned long long resultado = 0;
    a = a % mod;

    while (b > 0) {
        if (b & 1) {  // se o bit menos significativo de b for 1
            resultado = (resultado + a) % mod;
        }
        a = (a << 1) % mod; // multiplica a por 2, mod mod
        b >>= 1; // divide b por 2, descartando o bit menos significativo
    }
    return resultado;
}

