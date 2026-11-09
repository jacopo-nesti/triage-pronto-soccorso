#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

class Paziente
{
private:
    string nome;
    string cognome;
    string genere;
    int eta;
    int oraIN;
    int minutiIN;
    string codiceTriage;

    map<string, string> sintomi;

public:
    Paziente(string n, string c, string g, int e, int h, int m, string cT)
        {nome=n; cognome=c; genere=g; eta=e; oraIN=h; minutiIN=m; codiceTriage=cT; }

    string getNome () const {return nome;}
    string getCognome () const {return cognome;}
    string getGenere () const {return genere;}
    int getEta () const {return eta;}
    
    void setSintomo(const string& chiave, const string& valore) 
        {
            sintomi[chiave] = valore;
        }
    
    string getSintomo(const string& chiave) const
        {
            if (sintomi.count(chiave))
                return sintomi.at(chiave);
            return "NON PRESENTE";
        }
    
    void setCodiceTriage(const string& c)
        {
            codiceTriage=c;
        }
        
    string getCodiceTriage() const
        {
            return codiceTriage;
        }

    virtual void mostraInfo() const
        {
            cout<<"Nome: "<<nome<<endl;
            cout<<"cognome: "<<cognome<<endl;
            cout<<"Genere: "<<genere<<endl;
            cout<<"Eta: "<<eta<<endl;
            cout<<"Orario di accesso: "<<oraIN<<":"<<(minutiIN<10?"0":"")<<minutiIN<<endl;
            cout<<"--------------------------------"<<endl;

            if(!sintomi.empty())
                {
                    cout<<"Sintomi: "<<endl;
                    for(const auto& s : sintomi)
                    cout<<"  "<<s.first <<": "<<s.second<<endl;
                }
            cout<<"--------------------------------"<<endl;
        }

    virtual void parametriVitali(Paziente &p) const
        {
            string r;

            cout << "--- PARAMETRI VITALI ---" << endl;

            cout << "Pressione sistolica: ";
            cin >> r;
            p.setSintomo("Pressione sistolica", r);

            cout << "Pressione diastolica: ";
            cin >> r;
            p.setSintomo("Pressione diastolica", r);

            cout << "Saturazione O2: ";
            cin >> r;
            p.setSintomo("Saturazione O2", r);

            cout<<"Frequenza carediaca: ";
            cin>>r;
            p.setSintomo("Frequenza cardiaca", r);
        }
    virtual ~Paziente () {}

    /*virtual void diagnosi()
        {
            cout<<"Descrivi i sintomi specifici del paziente"<<endl;
            cin>>
        }*/
};

class GestionePazienti
{
    private:
        vector<Paziente>listaPazienti;
    public:
        void aggiungiPaziente(const Paziente& p) 
            {
                listaPazienti.push_back(p); 
            }
        void stampaLista() const
            {
                if(listaPazienti.empty())
                    {
                        cout<<"Nessun paziente registrato"<<endl;
                        return;
                    }
                for (const auto& p : listaPazienti)
                    {
                        p.mostraInfo();
                    }
            }
};

string normalizzaRisposta(string r)
{
    // trasforma tutto in maiuscolo
    for(char &c : r)
        c = toupper(c);

    // accetta anche "S"
    if(r == "S")
        r = "SI";

    return r;
}


int main()
{
    GestionePazienti gestione;
    int scelta;
    
    do
    {
        cout<<endl<<"------GESTIONALE TRIAGE PRONTO SOCCORSO------"<<endl;
        cout<<endl<<"------------------MENU------------------"<<endl;
        cout<<"---------------------------------------------------"<<endl;
        cout<<"1. Inserisci nuovo paziente"<<endl;
        cout<<"2. Lista pazienti"<<endl;
        cout<<"0. Esci"<<endl;
        cout<<"---------------------------------------------------"<<endl;
        cout<<"Scegli un opzione: ";
        cin>>scelta;

        switch (scelta)
            {
            case 1:
                {
                    //FASE 1 - DATI ANAGRAFICI
                    // EVENTUALMENTE PUOI METTERE UN ALGORITMO CHE GENERE IL C.F. DAI DATI ANAGRAFICI
                    string nome, cognome, genere, codiceTriage;
                    int eta, priorita, oraIN, minutiIN;
                    cout<<"Nome?"<<endl; cin>>nome;
                    cout<<endl<<"Cognome?"<<endl; cin>>cognome;
                    cout<<endl<<"Genere (M/F)?"<<endl; cin>>genere;
                    cout<<endl<<"Eta (0-120)?"<<endl; cin>>eta;
                    cout<<endl<<"Ora di arrivo (0-23)?"<<endl; cin>>oraIN;
                    cout<<endl<<"Minuti di arrivo (0-59)?"<<endl; cin>>minutiIN;
                    cout<<endl<<"PAZIENTE REGISTRATO! Continuiamo con i sintomi per definire il profilo clinico"<<endl;
                    cout<<"---------------------------------------------------"<<endl;

                    Paziente p(nome, cognome, genere, eta, oraIN, minutiIN, codiceTriage);

                    //FASE 2 - DOMANDE CHIUSE (SI/NO) - PROFILO CLINICO VELOCE
                    string domande []=
                        {
                            "Perdita di coscienza attuale",
                            "Respiro assente o gravemente compromesso",
                            "Dolore toracico severo",
                            "Difficolta respiratoria severa",
                            "Grave deficit neurologico",
                            "Stato confusionale",
                            "Pregressa perdita di coscienza",
                            "Trauma recente significativo",
                            "Dolore intenso",
                            "Sanguinamento importante non controllato",
                            "Convulsioni in corso prolungate",
                        };
                    string risposta;

                    int N=11;

                    for (int i=0; i<N; i++)
                    {
                        cout<<domande[i]<<"? (SI/NO): ";
                        cin>>risposta;

                        risposta = normalizzaRisposta(risposta);

                        p.setSintomo(domande[i], risposta);
                    }
                    cout<<endl<<"SINTOMATOLOGIA GENERICA REGISTRATA!"<<endl;
                    cout<<"---------------------------------------------------"<<endl;

                    //FASE 2-BIS - PRIORITA CODICI GRAVI
                    int punteggio=0;

                    if(p.getSintomo("Perdita di coscienza attuale")=="SI") punteggio +=5;
                    if(p.getSintomo("Respiro assente o gravemente compromesso")=="SI") punteggio +=5;
                    if(p.getSintomo("Dolore toracico severo")=="SI") punteggio +=4;
                    if(p.getSintomo("Difficolta respiratoria severa")=="SI") punteggio +=4;
                    if(p.getSintomo("Grave deficit neurologico")=="SI") punteggio +=4;
                    if(p.getSintomo("Stato confusionale")=="SI") punteggio +=2;
                    if(p.getSintomo("Pregressa perdita di coscienza")=="SI") punteggio +=2;
                    if(p.getSintomo("Trauma recente significativo")=="SI") punteggio +=2;
                    if(p.getSintomo("Dolore intenso")=="SI") punteggio +=2;
                    if(p.getSintomo("Sanguinamento importante non controllato")=="SI") punteggio +=5;
                    if(p.getSintomo("Convulsioni in corso prolungate")=="SI") punteggio +=5;

                    string codice;
                    if(punteggio>=7)
                        codice=" ROSSO - OSPEDALIZZAZIONE IMMEDIATA IN SALA ROSSA";
                    else if(punteggio>=3)
                        codice="GIALLO - OSPEDALIZZAZIONE PRIORITARIA";
                    else
                        codice="VERDE - NON URGENTE";
                    p.setCodiceTriage(codice);
                    cout<<endl<<"PUNTEGGIO: "<<punteggio<<endl;
                    cout<<"CODICE TRIAGE: "<<codice<<endl;
                    gestione.aggiungiPaziente(p);
                    cout<<"---------------------------------------------------"<<endl;
                    cout<<"INDICARE I SINTOMICI SPECIFICI DEL PAZIENTE"<<endl;

                    //FASE 3 - SINTOMI SPECIFICI
                    int sceltaCategoria;
                    do
                    {
                        cout<<"SINTOMI SPECIFICI"<<endl;
                        cout << "1. Cardiaco" << endl;
                        cout << "2. Respiratorio" << endl;
                        cout << "3. Neurologico" << endl;
                        cout << "4. Gastrointestinale" << endl;
                        cout << "5. Traumatologico" << endl;
                        cout << "6. Infettivo" << endl;
                        cout << "7. Metabolico" << endl;
                        cout << "8. Generico" << endl;
                        cout << "0. Uscita - Continuiamo per migliorare il profilo clinico" << endl;
                        cout<<"---------------------------------------------------"<<endl;
                        cout<<"Scegli un opzione: ";
                        cin>>sceltaCategoria;

                        switch(sceltaCategoria)
                        {
                            case 1:
                            {
                                cout << "--- Cardiaco ---" << endl;
                                string risposta;

                                cout << "Dolore toracico presente? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Dolore toracico", risposta);

                                cout << "Tipo dolore (oppressivo/puntorio): ";
                                cin >> risposta;
                                p.setSintomo("Tipo dolore toracico", risposta);

                                cout << "Irradiazione del dolore? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Irradiazione dolore", risposta);

                                cout << "Palpitazioni? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Palpitazioni", risposta);

                                cout << "Dispnea da sforzo? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Dispnea da sforzo", risposta);
                                break;
                            }
                            case 2:
                            {
                                cout << "--- Respiratorio ---" << endl;
                                string risposta;

                                cout << "Tosse? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Tosse", risposta);

                                cout << "Dispnea? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Dispnea", risposta);

                                cout << "Respiro sibilante? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Respiro sibilante", risposta);

                                cout << "Espettorato? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Espettorato", risposta);

                                cout << "Durata sintomi respiratori (giorni): ";
                                cin >> risposta;
                                p.setSintomo("Durata sintomi respiratori", risposta);
                                break;
                            }
                            case 3:
                            {
                                cout << "--- Neurologico ---" << endl;
                                string risposta;

                                cout << "Cefalea? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Cefalea", risposta);

                                cout << "Vertigini? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Vertigini", risposta);

                                cout << "Perdita di sensibilita? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Perdita sensibilita", risposta);

                                cout << "Difficolta nel linguaggio? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Difficolta linguaggio", risposta);

                                cout << "Difficolta motoria? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Difficolta motoria", risposta);

                                cout << "Disturbi visivi? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Disturbi visivi", risposta);
                                break;
                            }
                            case 4:
                            {
                                cout << "--- Gastrointestinale ---" << endl;
                                string risposta;

                                cout << "Nausea? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Nausea", risposta);

                                cout << "Vomito? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Vomito", risposta);

                                cout << "Diarrea? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Diarrea", risposta);

                                cout << "Rettorragia? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Rettorragia", risposta);

                                cout << "Dolore addominale? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Dolore addominale", risposta);

                                cout << "Sede dolore (alto/basso): ";
                                cin >> risposta;
                                p.setSintomo("Sede dolore addominale", risposta);
                                break;
                            }
                            case 5:
                            {
                                cout << "--- Traumatologico ---" << endl;
                                string risposta;

                                cout << "Trauma recente? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Trauma recente", risposta);

                                if(risposta == "SI")
                                {
                                    cout << "Tipologia trauma (incidente/caduta/sport/altro): ";
                                    cin >> risposta;
                                    p.setSintomo("Tipologia trauma", risposta);

                                    cout << "Alta energia (SI/NO)? ";
                                    cin >> risposta;
                                    p.setSintomo("Trauma alta energia", risposta);

                                    cout << "Dolore localizzato? (SI/NO): ";
                                    cin >> risposta;
                                    p.setSintomo("Dolore localizzato", risposta);

                                    if(risposta == "SI")
                                    {
                                        cout << "Sede del dolore: ";
                                        cin >> risposta;
                                        p.setSintomo("Sede dolore", risposta);

                                        cout << "Intensita dolore (1-10): ";
                                        cin >> risposta;
                                        p.setSintomo("Intensita dolore", risposta);
                                    }

                                    cout << "Sospetta frattura? (SI/NO): ";
                                    cin >> risposta;
                                    p.setSintomo("Sospetta frattura", risposta);

                                    if(risposta == "SI")
                                    {
                                        cout << "Arto coinvolto: ";
                                        cin >> risposta;
                                        p.setSintomo("Arto coinvolto", risposta);

                                        cout << "Deformita visibile? (SI/NO): ";
                                        cin >> risposta;
                                        p.setSintomo("Deformita", risposta);
                                    }

                                    cout << "Ferite aperte? (SI/NO): ";
                                    cin >> risposta;
                                    p.setSintomo("Ferite", risposta);

                                    if(risposta == "SI")
                                    {
                                        cout << "Sanguinamento abbondante? (SI/NO): ";
                                        cin >> risposta;
                                        p.setSintomo("Sanguinamento trauma", risposta);
                                    }

                                    cout << "Limitazione movimento? (SI/NO): ";
                                    cin >> risposta;
                                    p.setSintomo("Limitazione movimento", risposta);
                                }
                                else
                                {
                                    cout << "Nessun trauma recente registrato." << endl;
                                }
                                break;
                            }
                            case 6:
                            {
                                cout << "--- Infettivo ---" << endl;
                                string risposta;

                                cout << "Febbre? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Febbre", risposta);

                                cout << "Temperatura corporea: ";
                                cin >> risposta;
                                p.setSintomo("Temperatura", risposta);

                                cout << "Brividi? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Brividi", risposta);

                                cout << "Infezione recente? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Infezione recente", risposta);

                                cout << "Malessere generale? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Malessere generale", risposta);
                                break;
                            }
                            case 7:
                            {
                                cout << "--- Metabolico ---" << endl;
                                string risposta;

                                cout << "Sete eccessiva? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Sete eccessiva", risposta);

                                cout << "Glicemia nota (valore): ";
                                cin >> risposta;
                                p.setSintomo("Glicemia", risposta);

                                cout << "Debolezza? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Debolezza", risposta);

                                cout << "Perdita di peso? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Perdita peso", risposta);

                                cout << "Sudorazione eccessiva? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Sudorazione", risposta);
                                break;
                            }
                            case 8:
                            {
                                cout << "--- Generico ---" << endl;
                                string risposta;

                                cout << "Stanchezza? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Stanchezza", risposta);

                                cout << "Dolore diffuso? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Dolore diffuso", risposta);

                                cout << "Perdita appetito? (SI/NO): ";
                                cin >> risposta;
                                p.setSintomo("Perdita appetito", risposta);

                                cout << "Durata sintomi (giorni): ";
                                cin >> risposta;
                                p.setSintomo("Durata sintomi", risposta);

                                cout << "Altro sintomo: ";
                                cin >> risposta;
                                p.setSintomo("Altro", risposta);
                                break;
                            }
                            case 0:
                            {
                                cout<< "SINTOMI SPECIFICI REGISTRATI! Continuiamo per migliorare il profilo clinico" << endl;
                                cout<<"---------------------------------------------------"<<endl;
                                break;
                            }
                            default:
                            {
                                cout<<"Scelta non valida"<<endl;
                                break;
                            }
                        }
                    } while (sceltaCategoria !=0);

                    //FASE 4 - PARAMETRI VITALI
                    // p.parametriVitali(r)

                    //FASE 5 - DIAGNOSI AUTOMATICA

        
                    break;
                }
            case 2:
                gestione.stampaLista();
                break;

            case 0:
                cout<<"Uscita..."<<endl;
                break;
        
            default:
                cout<<"Scelta non valida"<<endl;
                break;
            }

    } while (scelta !=0);
    
    return 0;
}