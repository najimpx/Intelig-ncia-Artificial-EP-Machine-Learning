#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <time.h>

using namespace std;

//usado para normalizar .dat
string normaliza(string divide, string* fim){

    string temp = divide;
    istringstream ss(divide);
    vector<string> somaTexto;
    string normal;
    double soma, num;

	while(getline(ss, divide, ',')) {
		somaTexto.push_back(divide);
		soma += stod(divide);
	}
	soma -= stod(divide);
    istringstream st(temp);

    while(getline(st, temp, ',')) {
		somaTexto.push_back(temp);
        num = stod(temp)/soma;
		normal = normal + to_string(num) + ",";
	}
	//tira o �ltimo n�mero e v�rgula
	fim[0] = temp;
	normal = normal.substr(0, normal.size()-2-sizeof(num));
    //guarda no arquivo
    return normal;
}

//cria pesos aleatorios e os normaliza
double** peso(int nNeuroniosCamadaOculta,int tEntrada, string nomearq, int seed){
    double rng;
    double** guarda = new double*[tEntrada];
    ofstream arqPeso(nomearq);
    //usa rand() para criar cada peso
    srand ( seed);
    for(int i = 0; i < tEntrada; i++){
        guarda[i] = new double[nNeuroniosCamadaOculta];
        for(int j = 0; j < nNeuroniosCamadaOculta; j++){
            //rand cria double de 0 a 1, multiplicando a 2 fica 0 a 2, subtraindo por 1 -1 a 1.
            rng = (int)rand()/100;
            guarda[i][j] = rng;
        }
        //cada linha � o peso para um input na entrada
    }

    for(int i = 0; i < tEntrada; i++){
        double aux = 0;

        for(int j = 0; j < nNeuroniosCamadaOculta; j++){
            aux += guarda[i][j];
        }
        for(int j = 0; j < nNeuroniosCamadaOculta; j++){
            if(rng == (int)rand()/101 >= 50 ){
                guarda[i][j] = guarda[i][j]/aux;
            }
            else{
                guarda[i][j] = -guarda[i][j]/aux;
            }

            //cout<<guarda[i][j]<<endl;
            if(i < tEntrada && j < nNeuroniosCamadaOculta){
                arqPeso<<to_string(guarda[i][j])+",";
            }
            //nao cria virgula no ultimo elemento
            else{
                arqPeso<<to_string(guarda[i][j]);
            }
        }

        if(i < tEntrada-1){
            arqPeso<<"\n";
        }
    }
    arqPeso.close();
    return guarda;
}

void mistura(){
    string nomearqdat = "optdigits.dat";

    fstream fileobjectdat (nomearqdat);
    string file,normalizado,fim,ultimo;

    ofstream MyFile("optdigits.nor");

    while (fileobjectdat >> file ) {
        normalizado += normaliza(file, &fim);
        ultimo += fim;
        ultimo += "\n";
        normalizado += "\n";
    }
    MyFile << normalizado;
    string nomearqult = "ultimo.txt";
    fstream fileobjectult (nomearqult);
    ofstream ultFile(nomearqult);
    ultFile << ultimo;

    fileobjectdat.close();
    fileobjectult.close();
    MyFile.close();
}

void junta(){
    string nomearqtes = "optdigits.tes";
    string nomearqtrae = "optdigits.trae";

    fstream fileobjecttes (nomearqtes);
    fstream fileobjecttrae (nomearqtrae);
    ofstream MyFile("optdigits.dat");

    string file;

    while (fileobjecttes >> file ) {
        MyFile << file;
        MyFile << "\n";
    }
    while (fileobjecttrae >> file ) {
        MyFile << file;
        MyFile << "\n";
    }
    fileobjecttes.close();
    fileobjecttrae.close();
    MyFile.close();
}

//transforma as saidas desejadas( numero que representa a imagem) para binario
vector<int>* bina(int saidas){

    fstream fileobject ("ultimo.txt");
    string file;
    vector<int>* converte = new vector<int>[saidas];

    int j = 0;
    while ( fileobject >> file ) {
        for(int i = 0; i < 10; i++){
            converte[j].push_back(0);
        }
        converte[j].at(stoi(file)) = 1;
        j++;
    }
    return converte;
}

vector<int>* Resposta(vector<int>*binario,int tamanho,int comeco){
    vector<int>* RespostaBinario = new vector<int>[tamanho];
    for(int i = 0; i < tamanho; i++){
        for(int j = 0;j < 10; j++){
            RespostaBinario[i].push_back(binario[i+comeco].at(j));
        }
    }
    return RespostaBinario;
}

void imprimeVetor(vector<int>* binario, int tam){
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < 10; j++){
            cout<<binario[i].at(j)<<" ";
        }
        cout<<i<<endl;
    }
}

void escreve(vector<int>* binario){

    ofstream arqSaida("saidas.txt");

    for(int i = 0; i < 5620; i++){
        for(int j = 0; j < 10; j++){
            arqSaida << to_string(binario[i].at(j)) + " ";
        }
        arqSaida<< "\n";
    }
}

double sigmoide(double entra){
        double sig = (1/(1 + exp(-entra)));
        return sig;
}

//double TermoErro()
double ErroQuad(vector<double>saida, vector<double> esperado,double soma){
    for(int i = 0;i < saida.size();i++){
        soma +=(esperado.at(i) - saida.at(i)*(esperado.at(i) - saida.at(i)));
    }
    return soma;
 }


 vector<double> TermoErroSaida(vector<double> saida,vector<int> esperado){
    vector<double> tErros;
    bool maior = true;
    for(int i = 0;i < saida.size();i++){
        tErros.push_back(saida.at(i)*(1 - saida.at(i))*(esperado.at(i) - saida.at(i)));
    }
    return tErros;
 }

int acertado(int acertos,vector<double> saida,vector<int> esperado){
    bool maior = true;
    for(int i = 0;i < saida.size();i++){
        //cout<<saida.at(i)<<" "<<i<<" ";
        if(esperado.at(i) == 1){
            //cout<<"valor esperado "<<i;
            for(int j = 0; j < saida.size();j++){
                if(saida.at(i) < saida.at(j)){
                    maior = false;
                }
            }
        }
        //cout<<endl;
    }
    if(maior == true){
        acertos++;
    }
    return acertos;
}

vector<double> aplicaPeso(int qntNeuroniosEntrada, int tamHiddenLayer, string arqNeuroniosEntrada, double** pesos, double bias) {

     vector<double>HiddenPosPesos;
     vector<double>HiddenSaida;
     double neuroniosEntrada[qntNeuroniosEntrada][tamHiddenLayer + 1];
     string stringDeNeuroniosEntrada;
     string pesosDaRede;
     stringstream iss(arqNeuroniosEntrada);
     int iteradorEntrada = 0;
     int iteradorPeso = 1;

     //guardando valores dos neuronios de entrada na primeira posição da matriz
     while(getline(iss, stringDeNeuroniosEntrada, ',' )){
            neuroniosEntrada[iteradorEntrada][0] = stod(stringDeNeuroniosEntrada);
            iteradorEntrada++;
     }

     iteradorEntrada = 0;

     //guardando valor dos pesos a serem aplicados na matriz
    for(int i = 0; i < qntNeuroniosEntrada; i++){
        for(int j = 0; j < tamHiddenLayer; j++){
            neuroniosEntrada[iteradorEntrada][iteradorPeso] = pesos[i][j];
            iteradorPeso++;
            if(iteradorPeso == tamHiddenLayer + 1)
            {
                iteradorEntrada++;
                iteradorPeso = 1;
            }
        }
    }

     iteradorEntrada = 0;
     iteradorPeso = 1;

     //agora que ja temos a matriz preenchida, aplicaremos os pesos
     double aux = 0;
    for(int i=1;i <tamHiddenLayer+1; i++ ){
        for(int j=0;j<qntNeuroniosEntrada;j++){
            aux = aux + (neuroniosEntrada[j][0] * neuroniosEntrada[j][i]);
        }
        aux = aux + bias;
        HiddenPosPesos.push_back(aux);
        aux = 0;
     }

     for(int i=0; i < HiddenPosPesos.size(); i++){
        HiddenSaida.push_back(sigmoide(HiddenPosPesos.at(i)));
        //cout<<HiddenSaida.at(i)<<endl;
     }
     //cout<<endl;
     HiddenPosPesos.clear();
     return HiddenSaida;
 }

vector<double> aplicaPesoSaida(int qntNeuroniosEntrada, int tamHiddenLayer, vector<double>HiddenLayerS, double** pesos2, double bias, vector<int> binario, int iterador) {

     vector<double>HiddenPosPesos;
     vector<double>HiddenSaida;
     double neuroniosEntrada[qntNeuroniosEntrada][tamHiddenLayer + 1];
     string stringDeNeuroniosEntrada, pesosDaRede;
     int iteradorEntrada = 0;
     int iteradorPeso = 1;
     //guardando valores dos neuronios de entrada na primeira posição da matriz
     for(int i = 0; i<HiddenLayerS.size();i++){
            neuroniosEntrada[i][0] = HiddenLayerS.at(i);
     }

     iteradorEntrada = 0;
     iteradorPeso = 1;

     //guardando valor dos pesos a serem aplicados na matriz

    for(int i = 0; i < qntNeuroniosEntrada; i++){
        for(int j = 0; j < tamHiddenLayer; j++){
            neuroniosEntrada[iteradorEntrada][iteradorPeso] = pesos2[i][j];
            iteradorPeso++;
            if(iteradorPeso == tamHiddenLayer + 1)
            {
                iteradorEntrada++;
                iteradorPeso = 1;
            }
        }
    }

     //agora que ja temos a matriz preenchida, aplicaremos os pesos
     double aux = 0;
     for(int i=1;i <tamHiddenLayer+1; i++ ){
        for(int j=0;j<qntNeuroniosEntrada;j++){
            aux = aux + (neuroniosEntrada[j][0] * neuroniosEntrada[j][i]);
        }
        aux = aux + bias;
        HiddenPosPesos.push_back(aux);
        aux = 0;
     }

     for(int i=0; i < HiddenPosPesos.size(); i++){
        HiddenSaida.push_back(sigmoide(HiddenPosPesos.at(i)));
        //cout<<HiddenSaida.at(i)<<endl;
     }
        //cout<<endl;
        //ErroQuad(binario,HiddenSaida);
    HiddenPosPesos.clear();
     return HiddenSaida;
 }

vector<double> TErroHidden(vector<double>tErros,vector<double> hiddenOutput, double** pesos,int qntNeuroniosEntrada, int tamHiddenLayer){
    vector<double> tErrosHidden;
    string linhaPeso;
    string temp;

    for(int i = 0; i < qntNeuroniosEntrada;i++){
        int iterador = 0;
        double guarda = 0;
        for(int j = 0; j < tamHiddenLayer; j++){
            guarda += pesos[i][j]*tErros.at(iterador);
            iterador++;
        }
        guarda = guarda * (hiddenOutput.at(iterador) * (1- hiddenOutput.at(iterador)));
        tErrosHidden.push_back(guarda);
    }
    return tErrosHidden;
 }

double**  mudaPesoEntrada1(vector<double> entradas, vector<double> erroEntrada, int qtdNeuronioEntrada, int qtdHiddenLayers, double** pesos, double n){
    vector<double> primeiroTriangulo;
    vector<double>temp1;
    double Entradas;
    int auxIterador = 0;
    int aux = 0;
    int y = 0;

    for(int i = 0; i < entradas.size();i++){
        for(int j =0; j < erroEntrada.size();j++){
            primeiroTriangulo.push_back(entradas.at(i)*erroEntrada.at(j)*n);
            double temp =entradas.at(i)*erroEntrada.at(j)* n;
            y++;
        }
    }
    for(int i = 0; i < qtdNeuronioEntrada; i++){
        for(int j = 0; j < qtdHiddenLayers; j++){
            temp1.push_back(pesos[i][j]);
        }
    }

    int z = 0;
        //o for apenas sabe onde coloca \n
    for(int i = 0; i < qtdNeuronioEntrada; i++){
        for(int j = 0; j < qtdHiddenLayers; j++){
            pesos[i][j] = (temp1.at(z)+primeiroTriangulo.at(z));
            z++;
        }
    }
    primeiroTriangulo.clear();
    temp1.clear();
    return pesos;
}

double**  mudaPesoEntrada2(vector<double> erroEntrada, vector<double> erroSaida,vector<double> hiddenLayers,int qtdNeuronioEntrada, int qtdHiddenLayers, double** pesos2, double n){
    vector<double> segundoTriangulo;
    double Entradas;
    double pesoCorrigido;
    int auxIterador = 0;
    int aux = 0;
    vector<double>temp2;

     for(int i = 0; i < hiddenLayers.size();i++){
        for(int j =0; j < erroSaida.size();j++){
            segundoTriangulo.push_back(hiddenLayers.at(i)*erroSaida.at(j)*n);
            double temp = hiddenLayers.at(i)*erroSaida.at(j)*n;
            //cout<<temp<<"  "<<segundoTriangulo.at(y)<<" "<<i<<endl;
        }
    }

    for(int i = 0; i < qtdHiddenLayers; i++){
        for(int j = 0; j < 10; j++){
            temp2.push_back(pesos2[i][j]);
        }
    }

    int z = 0;

    for(int i = 0; i < qtdHiddenLayers; i++){
        for(int j = 0; j < 10; j++){
            pesos2[i][j] = (temp2.at(z) + segundoTriangulo.at(z));
            z++;
        }
    }
    segundoTriangulo.clear();
    temp2.clear();
    return pesos2;
}

//usado nos Sort para mudar as ordens das colunas aleatoriamente
double** Randomize2DArray(double** matriz,int tam1, int tam2, vector<int>* binario){
//randomiza e divide em tres o arquivo
    //randomiza arquivo normalizado e divide em tres

//randomiza arquivo e vetor binario(numeros de saida) e divide os dois em tres arquivos e vetores

    srand (time(NULL));
    int rng;
    for (int i =  0; i < tam1 ; i++) {
        int n = 0;
        rng = (int)rand()%tam1;
        int m = rng;
        vector<int> temp;
        for(int z = 0; z<10; z++){
            temp.push_back(binario[i].at(z));
            binario[i].at(z) = binario[m].at(z);
            binario[m].at(z) = temp.at(z);
        }
        temp.clear();
        for (int j = 0; j <  tam2; j++) {
            double temp = matriz[i][j];
            matriz[i][j] = matriz[m][n];
            matriz[m][n] = temp;
            n++;
        }
    }
    return matriz;
}
void SortNor(int tam1, int tam2, string NomeTxtNor, vector<int>* binario,bool randomiza){

    int iteradorNorColuna = 0;
    int iteradorNorLinha = 0;
    string str;
    string input;
    //stringstream strstream;
    fstream arquivoNor(NomeTxtNor);

    if(!arquivoNor.is_open()){
        cout << "Erro ao abrir arquivo Nor" << endl;
     };
    double** matriz = new double*[tam1];
    while(arquivoNor >> str){
        stringstream strstream(str);

        matriz[iteradorNorLinha] = new double[tam2];
        while(getline(strstream, input, ',')){
            matriz[iteradorNorLinha][iteradorNorColuna] = stod(input);
            //cout << "add at " << iteradorNorLinha << " " << iteradorNorColuna << " the value:  " << input << endl;
            iteradorNorColuna++;
        }
        iteradorNorColuna = 0;
        iteradorNorLinha++;
    }
    arquivoNor.close();

    for(int i = 0; i<3;i++){
        if(randomiza == true){
            matriz = Randomize2DArray(matriz,tam1,tam2,binario);
        }
    }
       ofstream gravandoPrimeiraParte("treino.txt");
       for(int indexx = 0; indexx < 1874;indexx++) {
           for(int x=0;x < tam2;x++){
                if(x == tam2 -1){
                    gravandoPrimeiraParte<<to_string(matriz[indexx][x])+"\n";
                }
                else{
                    gravandoPrimeiraParte<<to_string(matriz[indexx][x])+",";
                }
            }
        }
        gravandoPrimeiraParte.close();

        ofstream gravandoSegundaParte ("teste.txt");
        for(int indexx = 1874; indexx < 3747;indexx++) {
            for(int x=0;x < tam2;x++){
                if(x == tam2 -1){
                    gravandoSegundaParte<<to_string(matriz[indexx][x])+"\n";
                }
                else{
                    gravandoSegundaParte<<to_string(matriz[indexx][x])+",";
                }
           }
        }
        gravandoSegundaParte.close();

        ofstream gravandoTerceiraParte ("validacao.txt");
        for(int indexx = 3747; indexx < 5620;indexx++) {
            for(int x=0;x < tam2;x++){
                if(x == tam2 -1){
                    gravandoTerceiraParte<<to_string(matriz[indexx][x])+"\n";
                }
                else{
                    gravandoTerceiraParte<<to_string(matriz[indexx][x])+",";
                }
           }
        }
    gravandoTerceiraParte.close();

}

//randomiza um arquivo
void SortIndividual(int tam1, int tam2, string NomeTxt, vector<int>* binario){

    int iteradorNorColuna = 0;
    int iteradorNorLinha = 0;
    string str;
    string input;
    //stringstream strstream;
    fstream arquivoNor(NomeTxt);

    if(!arquivoNor.is_open()){
        cout << "Erro ao abrir arquivo" << endl;
     };
    double** matriz = new double*[tam1];
    while(arquivoNor >> str){
        stringstream strstream(str);

        matriz[iteradorNorLinha] = new double[tam2];
        while(getline(strstream, input, ',')){
            matriz[iteradorNorLinha][iteradorNorColuna] = stod(input);
            //cout << "add at " << iteradorNorLinha << " " << iteradorNorColuna << " the value:  " << input << endl;
            iteradorNorColuna++;
        }
        iteradorNorColuna = 0;
        iteradorNorLinha++;
    }
    arquivoNor.close();

    for(int i = 0; i<3;i++){
        matriz = Randomize2DArray(matriz,tam1,tam2,binario);
    }
       ofstream gravandoPrimeiraParte(NomeTxt);
       for(int indexx = 0; indexx < tam1;indexx++) {
           for(int x=0;x < tam2;x++){
                if(x == tam2 -1){
                    gravandoPrimeiraParte<<to_string(matriz[indexx][x])+"\n";
                }
                else{
                    gravandoPrimeiraParte<<to_string(matriz[indexx][x])+",";
                }
            }
        }
    gravandoPrimeiraParte.close();
}

void Rede(int neuronioEntrada, int tamHiddenLayer, vector<int>* RespostaTreino, vector<int>* RespostaTeste, vector<int>* RespostaValidacao, double ** peso, double** pesoSaida,string treino,string teste,string avaliacao, double tamanhoArq, double nTreino, int Epoca){

    string nomedoarquivo = treino;
    vector<int>* binario = RespostaTreino;
    bool terminado = false;
    bool testeEpoca = true;
    bool avaliacaoEpoca = true;
    double n = nTreino;
    while(terminado == false){
        for(int i = 0; i < Epoca; i++){
            fstream fileobjectnor (nomedoarquivo);
            string recebeNor, ValorUnitEntrada;
            double bias1 = 0;
            double bias2 = 0;
            int tamSaida = 10;
            double acertou = 0;
            vector<double>HiddenLayer,saida,Terros,THidden,entradas;

            int iterator = 0;
            double erroQuadratico = 0;

            while(fileobjectnor >> recebeNor){
                stringstream iss(recebeNor);
                while(getline(iss,ValorUnitEntrada,',')){
                    entradas.push_back(stod(ValorUnitEntrada));
                }
                //cria saida Hidden Layer
                HiddenLayer = aplicaPeso(neuronioEntrada, tamHiddenLayer, recebeNor, peso, bias1);
                //cria saida output
                saida = aplicaPesoSaida(tamHiddenLayer, tamSaida, HiddenLayer, pesoSaida, bias2, binario[iterator],iterator);
                //devolve termo de Erro para os neuronios de saida
                Terros = TermoErroSaida(saida,binario[iterator]);
                acertou = acertado(acertou,saida,binario[iterator]);
                //devolve Termo de Erro para os neuronios ocultos
                THidden = TErroHidden(Terros,HiddenLayer, pesoSaida, tamHiddenLayer,tamSaida);
                //delta influencia o peso da entrada para camada oculta
                peso =   mudaPesoEntrada1(entradas, THidden, neuronioEntrada, tamHiddenLayer, peso, n);
                //delta influencia o peso da camada oculta para saida
                pesoSaida =  mudaPesoEntrada2(THidden, Terros, HiddenLayer, neuronioEntrada, tamHiddenLayer, pesoSaida, n);
                iterator++;
                //ErroQuad(saida,binario[iterator],erroQuadratico)
                entradas.clear();
                THidden.clear();
                Terros.clear();
                saida.clear();
                HiddenLayer.clear();
            }
            //SortIndividual(tamanhoArq, 64, nomedoarquivo, binario);

            fileobjectnor.close();
            cout<<"acertos : "<<acertou<<"  i:"<<i<<endl;
            cout<<endl;
            //cout<<erroQ<<endl;
        }
        if(testeEpoca == true){
            cout<<"teste"<<endl;
            Epoca = 1;
            nomedoarquivo = teste;
            binario = RespostaTeste;

            //imprimeVetor(binario);
            testeEpoca = false;
            n = 0;
            tamanhoArq = 1873;
        }
        else if(avaliacaoEpoca == true){
            cout<<"avaliacao"<<endl;
            Epoca = 100;
            nomedoarquivo = avaliacao;
            binario = RespostaValidacao;
            avaliacaoEpoca = false;
            n = 0.2;
            tamanhoArq = 1873;
        }
        else{
            terminado = true;
        }
    }
 }

 //divide o normalizado sem randomizar
void Divide(int tam1, int tam2, string NomeTxtNor, vector<int>* binario,bool randomiza){

    int iteradorNorColuna = 0;
    int iteradorNorLinha = 0;
    string str;
    string input;
    //stringstream strstream;
    fstream arquivoNor(NomeTxtNor);

    if(!arquivoNor.is_open()){
        cout << "Erro ao abrir arquivo Nor" << endl;
     };
    double** matriz = new double*[tam1];
    while(arquivoNor >> str){
        stringstream strstream(str);

        matriz[iteradorNorLinha] = new double[tam2];
        while(getline(strstream, input, ',')){
            matriz[iteradorNorLinha][iteradorNorColuna] = stod(input);
            //cout << "add at " << iteradorNorLinha << " " << iteradorNorColuna << " the value:  " << input << endl;
            iteradorNorColuna++;
        }
        iteradorNorColuna = 0;
        iteradorNorLinha++;
    }
    arquivoNor.close();

       ofstream gravandoPrimeiraParte("treino.txt");
       for(int indexx = 0; indexx < 1874;indexx++) {
           for(int x=0;x < tam2;x++){
                if(x == tam2-1){
                    gravandoPrimeiraParte<<to_string(matriz[indexx][x]);
                }
                gravandoPrimeiraParte<<to_string(matriz[indexx][x])+",";
           }
            gravandoPrimeiraParte<<"\n";
        }

        ofstream gravandoSegundaParte ("teste.txt");
        for(int indexx = 1874; indexx < 3747;indexx++) {
            for(int x = 0;x < tam2;x++){
                if(x == tam2-1){
                    gravandoSegundaParte<<to_string(matriz[indexx][x]);
                }
                gravandoSegundaParte<<to_string(matriz[indexx][x])+",";
           }
            gravandoSegundaParte<<"\n";
        }

        ofstream gravandoTerceiraParte ("validacao.txt");
        for(int indexx = 3747; indexx < 5620;indexx++) {
            for(int x=0;x < tam2;x++){
                if(x == tam2-1){
                    gravandoTerceiraParte<<to_string(matriz[indexx][x]);
                }
                gravandoTerceiraParte<<to_string(matriz[indexx][x])+",";
            }
            gravandoTerceiraParte<<"\n";
        }
}



int main()
{
    // cria .dat
    string nomeArqPrincipal;
    cout<<"Por favor Digitar o endereco do arquivo a ser utilizado ja normalizado"<<endl;
    cin>>nomeArqPrincipal;
    cout<<nomeArqPrincipal<<endl;
    cout<<"Por favor Digitar o endereco do arquivo ultimo.txt"<<endl;
    string ultimo;
    cin>>ultimo;

    junta();
    mistura();
    int neuronios = 15;
    int neuroniosSaida = 10;
    int entrada = 64;
    double** random = peso(neuronios, entrada, "pesos.txt",112312415);

    vector<int>* binario = bina(5620);
    escreve(binario);

    double** randomS = peso(neuroniosSaida, neuronios, "pesos2.txt",2131);

    bool randomiza = true;


    SortNor(5620, 64, nomeArqPrincipal,binario,randomiza);
    //Divide(5620, 64, "optdigits.nor",binario,randomiza);

    vector<int>* RespostaTreino = Resposta(binario,1874,0);
    vector<int>* RespostaTeste = Resposta(binario,1873,1874);
    vector<int>* RespostaValidacao = Resposta(binario,1873,3747);

    string treino = "treino.txt";
    string teste = "teste.txt";
    string avaliacao = "validacao.txt";

    int Epoca = 100;
    int tamanhoArq = 1874;
    double n = 0.15;

    Rede (entrada, neuronios, RespostaTreino, RespostaTeste, RespostaValidacao, random, randomS, treino, teste, avaliacao, tamanhoArq, n, Epoca);

    return 0;
}

