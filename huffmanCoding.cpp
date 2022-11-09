//AUTORE: Filippo Manini
//MATRICOLA: 4798004

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <queue>
#include <map>

using namespace std;
  

//╔══════════════════════════════════════════════════════════════════════╗
//║                           STRUCT                                     ║
//╚══════════════════════════════════════════════════════════════════════╝
// struttura dei nodi dell'albero di Huffman 
struct minHeapNode
{
    char carattere;             // Uno dei caratteri di input
    int freq;             // Frequenza del carattere
    minHeapNode *sinistro, *destro; // figlio destro e sinistro

    minHeapNode(char carattere, int freq) //costruttore 
    {
        sinistro = destro = NULL;
        this->carattere = carattere;
        this->freq = freq;
    }
};

// Oggetto di confronto da utilizzare per ordinare dell'heap
struct compare
{
    bool operator()(minHeapNode* l, minHeapNode* r)
    {
        return (l->freq > r->freq);
    }
};

//╔══════════════════════════════════════════════════════════════════════╗
//║                          FUNZIONI DI STAMPA                          ║
//╚══════════════════════════════════════════════════════════════════════╝
void stampoCodifica(map<char, string> cod)
{
	cout << "codifica dei caratteri:\n";
	cout << endl<<"CARATTERE" <<'\t'<< "CODIFICA"<<endl;

    for (auto v= cod.begin(); v!=cod.end(); v++){
        cout << v->first <<'\t'<<'\t'<< v->second << endl;
	}
}

void stampoFrequenza(map<char, int> freq)
{
	cout << "frequenza dei caratteri:\n";
	cout << endl<<"CARATTERE" <<'\t'<< "FREQUENZA"<<endl;

    for (auto v= freq.begin(); v!=freq.end(); v++){
        cout << v->first <<'\t'<<'\t'<< v->second << endl;
	}
}

void stampaVector(vector<double> v)
{
    for(int i=0; i<v.size(); ++i){
        cout<<v.at(i)<<'\t';
    }
    cout<<endl;
}

// funzione che stampa carattere e il suo valore di huffman
void stampoAlbero(struct minHeapNode* root)
{
    if (!root){
        return;
    }
    if (root->carattere != '~'){
        cout << root->carattere <<"\t"<< root->freq<<endl;
    }
    stampoAlbero(root->sinistro);
    stampoAlbero(root->destro);
}

//╔══════════════════════════════════════════════════════════════════════╗
//║                          I/O                                         ║
//╚══════════════════════════════════════════════════════════════════════╝

//lettura da file
string letturaFile(string s, string nomeFile)
{
	char c;
	ifstream fin(nomeFile); //si rea la variabile e si apre il file

	while(fin.get(c)) //si legge un carattere finche non si arriva a fine file 
	{
		s += c; //salvo il carattere nella stringa s cocatenando 
		//cout<<c; //stampo sulla console il contenuto di c 
	}
	//cout<<s;
	fin.close(); //chiusura file
	return s;
}

void scritturaFile(string s)
{
	ofstream fout("outHuffman.txt"); //crea se non esiste e se esiste sovrascrive il contenuto

	fout<<s; //scrivo tutta la stringa

	fout.close();
}

//╔══════════════════════════════════════════════════════════════════════╗
//║                          FUNZIONI SPECIFICHE                         ║
//╚══════════════════════════════════════════════════════════════════════╝

// funzione di utilità per memorizzare i caratteri insieme a il valore di huffman in una tabella hash
void salvoCod(struct minHeapNode* root, map<char, string>& cod, string s)
{
    if (root==NULL){
        return;
	}
    if (root->carattere != '~'){
        cod[root->carattere] = s; //indice: chiave, inserisco valore
	}
    salvoCod(root->sinistro, cod, s + "0");
    salvoCod(root->destro, cod, s + "1");
}

// funzione per costruire l'albero di Huffman e memorizzarlo in min Heap (pila)
void huffmanCod(int size, map<char, int>& freq, map<char, string>& cod, priority_queue<minHeapNode*, vector<minHeapNode*>, compare>& minHeap)
{
    struct minHeapNode *sinistro, *destro, *top;
    
    for (map<char, int>::iterator v = freq.begin(); v != freq.end(); v++)
    {
        minHeap.push(new minHeapNode(v->first, v->second));
	}
    
    while (minHeap.size() != 1)
    {
        sinistro = minHeap.top(); //top: restituisce primo elemento della coda (senza togliere)
        minHeap.pop(); //pop: tolgie primo elemento della coda 
        destro = minHeap.top();
        minHeap.pop();
        top = new minHeapNode('~', sinistro->freq + destro->freq);
        top->sinistro = sinistro;
        top->destro = destro;
        minHeap.push(top); //push: aggiunge alla fine della coda.
    }
    salvoCod(minHeap.top(), cod, "");
}

// funzione di utilità per memorizzare la mappa di ogni carattere con la sua 
// frequenza nella stringa di input
void calcFreq(string s,  map<char, int>& freq)
{
    for (int i=0; i<s.size(); i++)
    {
        freq[ s[i] ]++; //utilizzo la posizione della mappa come il carattesre designato e incremento il valore della medesima chiave 
    } 
}

// la funzione itera attraverso la stringa codificata s
string decodifica(struct minHeapNode* root, string s)
{
    string auxS = "";
    struct minHeapNode* cur = root;

    for (int i=0; i<s.size(); ++i)
    {
        if (s[i] == '0'){
           cur = cur->sinistro;
		}
        else{
           cur = cur->destro;
		}
        // raggiunto foglia node
        if (cur->sinistro==NULL && cur->destro==NULL)
        {
            auxS += cur->carattere;
            cur = root;
        }
    }
    return auxS+'\0'; //ritorno la stringa piu fine stringa 
}

//funzione per entropia di Shannon
double entropiaShannon(int testoSize, map<char, int>& freq)
{
	double infoContent = 0 ;
	
	for(map<char, int>::iterator it = freq.begin(); it != freq.end(); it++)
	{
		double frequenze = static_cast<double>( it->second ) / testoSize ; //numero di volte che compare il carattere / tot_caratteri anche ripetuti
		infoContent += frequenze * log2( 1 / frequenze ) ;
	}

	return infoContent;
} 

//questa funzione mi permette di andare a vedere i caratteri del testo e sostituiri con la codifica in binario
string stringHuffmanCodificati(string testo, map<char, string> cod)
{
    string encodedString;
    
    for (auto i: testo){
        encodedString += cod[i]; //restuisce il valore data la chiave
    }

    return encodedString;
}

//somma della probabilità del cattere per la lunghezza della sua codifica 
double lunghezzaAttesa(map<char, string> cod, map<char, int> freq, int testoSize)
{
    map<char, int>::iterator it_freq = freq.begin();
    map<char, string>::iterator it_cod = cod.begin();

    double L_attesa = 0;

    for(; it_freq != freq.end() || it_cod != cod.end(); ++it_freq, ++it_cod) //uso || perche basta che finisca uno dei 2 ma noi sappiamo che per entrambe le mappe la key e' la stessa e nello stesso ordine 
    {
        double frequenze = static_cast<double>( it_freq->second ) / testoSize ; //numero di volte che compare il carattere / tot_caratteri anche ripetuti
        L_attesa += frequenze * it_cod->second.size();
    }

    return L_attesa;
}

int dimCodifica(map<char, string> cod, map<char, int> freq)
{
    map<char, int>::iterator it_freq = freq.begin();
    map<char, string>::iterator it_cod = cod.begin();
    int nBit = 0;

    for(; it_freq != freq.end() || it_cod != cod.end(); ++it_freq, ++it_cod)
    {
        nBit += it_freq->second * it_cod->second.size();
    }
    return nBit;
}

//╔══════════════════════════════════════════════════════════════════════╗
//║                          MAIN                                        ║
//╚══════════════════════════════════════════════════════════════════════╝
int main()
{
	string nomeFile;
    cout<<"----------------------------------------------------"<<endl;
	cout<<"Digita il nome del file: ";
	cin >> nomeFile;

	string testo;
	testo = letturaFile(testo,nomeFile);
    int testoLunghezza = testo.size();
    string encodedString, decodedString;

    /* Le mappe sono contenitori associativi che memorizzano gli elementi in modo mappato. 
    Ogni elemento ha un valore chiave e un valore mappato. 
    Non esistono due valori mappati che possono avere gli stessi valori chiave. 
    inserisce in maniera ordinata in base alla chiave  */
    map<char, int> freq; // per memorizzare la frequenza del carattere dei dati di input
    map<char, string> cod; // per mappare ogni carattere con il suo valore di huffman
    priority_queue<minHeapNode*, vector<minHeapNode*>, compare> minHeap; // Coda di priorità STL per memorizzare l'albero di heap, rispetto al valore del nodo radice dell'heap, inserendo in maniera ordinata in base a come compare
	
    calcFreq(testo, freq);
	cout<<endl<<"entropia di Shannon H(X) = "<< entropiaShannon(testoLunghezza, freq) <<endl;
    huffmanCod(testoLunghezza, freq, cod, minHeap);
    cout<<"lunghezza attesa = "<<lunghezzaAttesa(cod, freq, testoLunghezza)<<endl;
    cout<<"----------------------------------------------------"<<endl;
    cout<<"Stampa dell'albero: "<<endl;
    stampoAlbero(minHeap.top());
    cout<<"----------------------------------------------------"<<endl;
	stampoFrequenza(freq);
	cout<<"----------------------------------------------------"<<endl;
    stampoCodifica(cod);
    encodedString = stringHuffmanCodificati(testo, cod);
    cout<<"----------------------------------------------------"<<endl;
	/*cout <<endl<<"scrittura su file in corso..."<<endl;
	scritturaFile(encodedString);
	cout <<"scrittura su file OK"<<endl;*/ //DEBUG
    decodedString = decodifica(minHeap.top(), encodedString);
    //cout << "\nDati Huffman decodificati:\n" << decodedString <<endl<<endl; //DEBUG
    cout<<"dimensione in bit del testo in input: "<<testoLunghezza*8<<" bit"<<endl; //8 ogni carattere e' 8 bit
    cout<<"dimensione in bit dell'output (codifica): "<<dimCodifica(cod,freq)<<" bit"<<endl;
	cout<<"----------------------------------------------------"<<endl;

    return EXIT_SUCCESS;
}