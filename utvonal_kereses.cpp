#include <iostream>
#include <vector>
#include <list>

using namespace std;

struct Vonat{
    string vonat_nev;
    vector<string> vonat_ut;            //azaz az allomasok
    vector<int> vonat_menetrend;        //ket allomas kozott eltelt ido
};

///kezdeti probalkozas, nem fontos
struct Csucs{
    string csucs_nev;
    vector<string> szomszedok;          //az adott csucs szomszedjai
    vector<int> tavolsag;               //az adott csucstol a szomszed idobeli tavolsaga
    vector<int> vonat_index;            //vonat indexe, amelyik ezen a vonalon megy, vagyis amelyik ket csucs kozott megy
};

///kereses soran hasznalt csucsok
struct kereses_csucs{                   //kereseshez legyartott graf csucsai, ez egy IANYITOTT graf lesz
    string nev;                         //allomas neve
    vector<kereses_csucs> szomszedok;   //iranyitott graf, tehat vagy az adott vonat kovetkezo allomasa vagy egy masik vonat ugyanazon allomasa, de kesobbi idovel
    int ido;                            //ennel a csucsnal, azaz ezen az allomason mikor van (pontosan)
    int vonat_index;                   //melyik vonat van ezen az allomason ebben az idopontban, vonat indexe
};

///vonat menetrendjenek letrehozasahoz
Vonat letrehoz(string nev, vector<string> ut, vector<int> menetrend){
    Vonat vonat;

    vonat.vonat_nev=nev;
    vonat.vonat_ut=ut;                  //ket allomas kozti idobeli tavolsag
    vonat.vonat_menetrend=menetrend;    //ido alapjan

    return vonat;
}

///osszefesult_ut_graf-hoz segitseg, ha az adott csucs szerepel a csucsok kozott (Utvonal_graf-ban) akkor annak az indexet visszaadja, ha nem akkor -1-at ad vissza
int csucs_index(vector<Csucs> Utvonal_graf, string csucs_nev)
{
    for (size_t i=0;i<Utvonal_graf.size();i++){
        if (csucs_nev==Utvonal_graf[i].csucs_nev){
            return i;
        }
    }
    return -1;
}

///vonatok utjaibol osszefesult graf
void osszefesult_ut_graf(vector<Csucs> &Utvonal_graf, vector<Vonat> vonat_vec){
    Csucs csucs;

    for (size_t i=0;i<vonat_vec.size();i++){    //most megnezem, hogy az elso vonatra legeneralja
        for (size_t j=0;j<vonat_vec[i].vonat_ut.size();j++){
            csucs.szomszedok.clear();
            csucs.tavolsag.clear();
            csucs.vonat_index.clear();
            csucs.csucs_nev=vonat_vec[i].vonat_ut[j];                                   //szomszedok megadasanal a vonatot is jelzi, hogy melyiknek az utvonala

            if (csucs_index(Utvonal_graf, vonat_vec[i].vonat_ut[j])==-1){               //ha ez uj csucs akkor hozzadja az osszesitett grafhoz (Utvonal_graf) a szomszedaival egyutt; szomszedai az eloote illetve utana levo egy-egy elem
                if (j>0){
                    csucs.szomszedok.push_back(vonat_vec[i].vonat_ut[j-1]);
                    csucs.tavolsag.push_back(vonat_vec[i].vonat_menetrend[j-1]);
                    csucs.vonat_index.push_back(i);
                }
                if (j+1<vonat_vec[i].vonat_ut.size()){
                    csucs.szomszedok.push_back(vonat_vec[i].vonat_ut[j+1]);
                    csucs.tavolsag.push_back(vonat_vec[i].vonat_menetrend[j]);
                    csucs.vonat_index.push_back(i);
                }
                Utvonal_graf.push_back(csucs);
            }
            else{                                                                       //ha mar szerepelt csucs akkor kiegesziti uj szomszedokkal
                csucs=Utvonal_graf[csucs_index(Utvonal_graf,vonat_vec[i].vonat_ut[j])];
                if (j>0){
                    csucs.szomszedok.push_back(vonat_vec[i].vonat_ut[j-1]);
                    csucs.tavolsag.push_back(vonat_vec[i].vonat_menetrend[j-1]);
                    csucs.vonat_index.push_back(i);
                }
                if (j+1<vonat_vec[i].vonat_ut.size()){
                    csucs.szomszedok.push_back(vonat_vec[i].vonat_ut[j+1]);
                    csucs.tavolsag.push_back(vonat_vec[i].vonat_menetrend[j]);
                    csucs.vonat_index.push_back(i);
                }
                Utvonal_graf[csucs_index(Utvonal_graf,vonat_vec[i].vonat_ut[j])]=csucs;
            }
        }
    }

}

///adott vonat hany ora mulva hol lesz
string hol_a_vonat(vector<Vonat> vonat_vec, string melyik_vonat, int mikor){
    int mennyi_plusz_ido_a_pontos_erkezeshez;       //pl 3-ra er oda de meg csak ketto van es uton van (nem egy allomason), tehat a pontos erkezes 1 ora mulva
    vector<string> adott_ut;
    vector<int> adott_menetrend;
    int vonat_index = -1;

    for (size_t i=0;i<vonat_vec.size();i++){
        if (vonat_vec[i].vonat_nev==melyik_vonat){
            vonat_index=i;
        }
    }

    if (vonat_index == -1){cerr<<"Hibasan megadott vonatnev"<<endl; return 0;};

    adott_ut=vonat_vec[vonat_index].vonat_ut;
    adott_menetrend.push_back(vonat_vec[vonat_index].vonat_menetrend[0]);

    for (size_t i=1;i<vonat_vec[vonat_index].vonat_menetrend.size();i++){
        adott_menetrend.push_back(adott_menetrend[i-1]+vonat_vec[vonat_index].vonat_menetrend[i]);
    }

    for (size_t i=vonat_vec[vonat_index].vonat_ut.size()-2;i>0;i--){
        adott_ut.push_back(vonat_vec[vonat_index].vonat_ut[i]);
        adott_menetrend.push_back(adott_menetrend.back() + vonat_vec[vonat_index].vonat_menetrend[i+1]);
    }
    adott_ut.push_back(vonat_vec[vonat_index].vonat_ut.front());
    adott_menetrend.push_back(adott_menetrend.back()+vonat_vec[vonat_index].vonat_menetrend[1]);

    vonat_index=-1;     //most a keresett idopontban levo csucs indexe lesz, nem a vonate

    ///fontos: ha uton van akkor is a celallomast jeleniti meg, pl 3 orara erne A-ba, de meg csak 2 ora van, akkor is A-ban van
    for (size_t i=0;i<adott_menetrend.size()-1;i++){
        if (mikor%adott_menetrend.back()<=adott_menetrend[i] && vonat_index == -1){     //megadja, hogy azon az allomason van, vagy oda fog erni, tehat ha uton van akkor a celallomast jeleniti meg
            mennyi_plusz_ido_a_pontos_erkezeshez=adott_menetrend[i]-mikor%adott_menetrend.back();
            vonat_index=i;
        }
    }

    //kiiratas
    cout<< "Vonat: " << melyik_vonat << " mikor: " << mikor << endl;
    for (size_t i=0;i<adott_menetrend.size();i++){
        cout << "Allomas: " << adott_ut[i] << " ido: " << adott_menetrend[i] << endl;
    }
    cout << "Melyik allomason van: " << adott_ut[vonat_index]<< "; Pontosan: " << mennyi_plusz_ido_a_pontos_erkezeshez << " ora mulva er oda (ha nem nulla akkor ket allomas kozott van)" << endl << endl;

    return adott_ut[vonat_index];
}

///adott vonat a kovetkezo allomason mikor lesz
void kovetkezo_allomas_mikor(vector<Vonat> vonat_vec, int vonat_index, string &most_melyik_allomason, int &mikor){   //vonat_index a keresett vonat indexe, amin nezem a kovetkezo allomast, most_melyik allomason a fuggveny lefutasa utan a kovekezo allomas nevet adja (referencia), mikor pedig az idot (referencia)
    vector<string> adott_ut;
    vector<int> adott_menetrend;

    adott_ut=vonat_vec[vonat_index].vonat_ut;
    adott_menetrend.push_back(vonat_vec[vonat_index].vonat_menetrend[0]);

    //az adott vonat menetrendjet hozza letre, iranyitott grafkent, oda-vissza uton pl.: A->B->C utvonalbol lesz A->B->C->B->A
    for (size_t i=1;i<vonat_vec[vonat_index].vonat_menetrend.size();i++){
        adott_menetrend.push_back(adott_menetrend[i-1]+vonat_vec[vonat_index].vonat_menetrend[i]);
    }

    //visszaut legyartasa
    for (size_t i=vonat_vec[vonat_index].vonat_ut.size()-2;i>0;i--){
        adott_ut.push_back(vonat_vec[vonat_index].vonat_ut[i]);
        adott_menetrend.push_back(adott_menetrend.back() + vonat_vec[vonat_index].vonat_menetrend[i+1]);
    }
    adott_ut.push_back(vonat_vec[vonat_index].vonat_ut.front());
    adott_menetrend.push_back(adott_menetrend.back()+vonat_vec[vonat_index].vonat_menetrend[1]);

    int allomas_index=-1;

    //megmondja a kovetkezo allomast, az atcsordulas miatt van maradekos osztas (%)
    for (size_t i=0;i<adott_ut.size();i++){
        if (most_melyik_allomason == adott_ut[i] && mikor % adott_menetrend.back() == adott_menetrend[i]){
            allomas_index=i;
        }
    }
    if (allomas_index==-1){allomas_index=adott_ut.size()-1;}        //az elso elemnel ne dobjon hibat, ha ujrakezdodik a ciklus, "foltoztam" a programot

    most_melyik_allomason = adott_ut[(allomas_index+1)%adott_ut.size()];
    mikor = adott_menetrend.back() * (mikor / adott_menetrend.back()) + adott_menetrend[(allomas_index+1)%adott_ut.size()];
}

///kereseshez szukseges graf letrehozasa
void graf_general(vector<kereses_csucs> &kereso_graf, vector<Vonat> vonat_vec, int meddig){     //hany oraig gyrtsa le a grafot
    int vonat_index;
    kereses_csucs seged_csucs;
    string most_melyik_allomason;
    int mikor;

    for (vonat_index=0;vonat_index<int(vonat_vec.size());vonat_index++){                             //az egyes vonatok menetrendje alapjan hozza letre az iranyitott grafot
        most_melyik_allomason=vonat_vec[vonat_index].vonat_ut[0];
        mikor=vonat_vec[vonat_index].vonat_menetrend[0];

        seged_csucs.ido=mikor;
        seged_csucs.nev=most_melyik_allomason;
        seged_csucs.vonat_index=vonat_index;

        kereso_graf.push_back(seged_csucs);

        while(mikor<meddig){
            kovetkezo_allomas_mikor(vonat_vec,vonat_index,most_melyik_allomason,mikor);
            seged_csucs.ido=mikor;
            seged_csucs.nev=most_melyik_allomason;
            seged_csucs.vonat_index=vonat_index;
            kereso_graf.push_back(seged_csucs);
            kereso_graf[kereso_graf.size()-2].szomszedok.push_back(kereso_graf.back());
        }
        //kereso_graf.back().szomszedok.push_back(kereso_graf.back());            //utolsonak is legyen szomszedja, onmaga
    }

    for (size_t i=0;i<kereso_graf.size();i++){              //eltero vonatok kozott is kapcsolatot letesit, ugyanazon allomason a korabban erkezo vonatnak szomszedja lesz a kesobb erkezo vonat
        for (size_t j=0;j<kereso_graf.size();j++){
            if (kereso_graf[i].nev==kereso_graf[j].nev && kereso_graf[i].ido<=kereso_graf[j].ido && !(kereso_graf[i].ido==kereso_graf[j].ido && kereso_graf[i].vonat_index==kereso_graf[j].vonat_index)){   //kiveve ha azonos allomason ugyanakkor van az azonos vonat
                kereso_graf[i].szomszedok.push_back(kereso_graf[j]);
            }
        }
    }



    //kiiratas
    /*for (size_t i=0;i<kereso_graf.size();i++){
        cout << endl << "Csucs neve: " << kereso_graf[i].nev << " ideje: " << kereso_graf[i].ido << " vonat indexe: " << kereso_graf[i].vonat_index << endl;
        cout << "\t szomszedok: " << kereso_graf[i].szomszedok.size() << " ";
        for (size_t j=0;j<kereso_graf[i].szomszedok.size();j++){
            cout << kereso_graf[i].szomszedok[j].nev << " ideje: " << kereso_graf[i].szomszedok[j].ido  << " vonat indexe: " << kereso_graf[i].szomszedok[j].vonat_index << "   ";
        }
    }*/
}

///a leterehozott iranyitott grafban ket allomas kozott megad utvonalat
vector<vector<kereses_csucs> > kereses(vector<kereses_csucs> kereso_graf, string honnan, string hova, int mikortol){
    list<vector<kereses_csucs> > utvonal;               //ez a lista adja a bejaras soran az utvonalat
    vector<kereses_csucs> seged;
    vector<vector<kereses_csucs> > eredmeny;                     //ezen vektorba gyujti a lehetseges utvonalakat
    bool volt_e_mar;                                    //ugyanazt a csucsot ne hasznalja megegyszer

    for (size_t i=0;i<kereso_graf.size();i++){
        if (kereso_graf[i].nev==honnan){
            seged.clear();
            seged.push_back(kereso_graf[i]);
            utvonal.push_back(seged);
            //break;      //most, hogy az elso talalattal keressen
        }
    }

    //utvonal.front().back().nev!=kereso_graf.back().nev && utvonal.front().back().ido!=kereso_graf.back().ido && utvonal.front().back().vonat_index!=kereso_graf.back().vonat_index
    while (utvonal.size()>0){
        while (utvonal.front().back().nev!=hova && utvonal.size()>1){

            //a lista elso elemenek kiiratasa
            /*cout << "Uj kor" << endl;
            for (size_t i=0;i<utvonal.front().size();i++){
                cout << utvonal.front()[i].nev << " " << utvonal.front()[i].ido << " " << utvonal.front()[i].vonat_index << endl;
            }
            //cout << utvonal.size() << endl;
            cout << endl;*/

            for (size_t i=0;i<kereso_graf.size();i++){
                if (utvonal.front().back().ido >= mikortol && utvonal.front().back().nev==kereso_graf[i].nev && utvonal.front().back().ido==kereso_graf[i].ido && utvonal.front().back().vonat_index==kereso_graf[i].vonat_index){
                    for (size_t j=0;j<kereso_graf[i].szomszedok.size();j++){
                        seged.clear();
                        seged=utvonal.front();

                        volt_e_mar=false;

                        for (size_t k=0;k<utvonal.front().size();k++){
                            if (utvonal.front()[k].nev==kereso_graf[i].szomszedok[j].nev && utvonal.front()[k].ido==kereso_graf[i].szomszedok[j].ido && utvonal.front()[k].vonat_index==kereso_graf[i].szomszedok[j].vonat_index){
                                volt_e_mar=true;
                            }
                        }
                        if (!volt_e_mar){
                            seged.push_back(kereso_graf[i].szomszedok[j]);
                            utvonal.push_back(seged);
                        }
                    }
                    break;
                }
            }

            utvonal.pop_front();
        }

        if (utvonal.front().back().nev==hova){
            eredmeny.push_back(utvonal.front());
            /*cout << "Eredmeny: honnan " << honnan << " hova " << hova << endl;
            for (size_t i=0;i<utvonal.front().size();i++){
                cout << "Allomas neve: " << utvonal.front()[i].nev << "\t ideje: " << utvonal.front()[i].ido << "\t vonat indexe: " << utvonal.front()[i].vonat_index << endl;
            }*/
        }

        utvonal.pop_front();
    }

    return eredmeny;
}

int main()
{
    vector<Vonat> vonat_vec;
    vector<Csucs> Utvonal_graf;
    vector<kereses_csucs> kereso_graf;

    ///az altalam rajzolt vonatmenetrend letrohozasa
    Vonat vonat_general;
    vonat_general=letrehoz("zold", {"A", "B", "C", "G", "J", "I", "H"}, {0, 1, 3, 2, 2, 1, 3});
    vonat_vec.push_back(vonat_general);
    vonat_general=letrehoz("kek", {"E", "F", "C", "D", "K"}, {1, 2, 1, 1, 3});
    vonat_vec.push_back(vonat_general);
    vonat_general=letrehoz("piros", {"F", "I", "L", "M", "K", "G"}, {2, 2, 3, 4, 1, 1});
    vonat_vec.push_back(vonat_general);

    ///az altalam rajzolt vonatmenetrend grafjanak letrehozasa, idofuggetlen (ez a rész nem kell)
    //osszefesult_ut_graf(Utvonal_graf, vonat_vec);

    //ezen graf kiiratasa
    /*for (size_t i=0;i<Utvonal_graf.size();i++){
        cout << "Csucs: " << Utvonal_graf[i].csucs_nev << endl;
        cout << "\t Csucs szomszedai:" << endl << "\t \t";
        for (size_t j=0;j<Utvonal_graf[i].szomszedok.size();j++){
            cout << Utvonal_graf[i].szomszedok[j] << "(" << Utvonal_graf[i].vonat_index[j] << ")" << " ";
        }
        cout << endl;
        cout << "\t Csucs tavolsagok:" << endl << "\t \t";
        for (size_t j=0;j<Utvonal_graf[i].tavolsag.size();j++){
            cout << Utvonal_graf[i].tavolsag[j] << " ";
        }
        cout << endl;
    }*/

    ///megmondja, hogy adott oraban hol a vonat
    //hol_a_vonat(vonat_vec, "kek", 30);

    /*int vonat_index=0;
    string melyik_allomas=vonat_vec[vonat_index].vonat_ut[0];
    int mikor=vonat_vec[vonat_index].vonat_menetrend[0];
    cout << "jelenlegi allomas: " << melyik_allomas << " mikor: " << mikor <<endl;

    for (int i=0;i<20;i++){
        kovetkezo_allomas_mikor(vonat_vec,vonat_index,melyik_allomas,mikor);
        cout << "Kovetkezo allomas: " << melyik_allomas << " mikor: " << mikor << endl;
    }*/


    ///kereseses resz
    //a kereseshez legeneralok egy grafot
    graf_general(kereso_graf,vonat_vec,25);

    vector<vector<kereses_csucs> > lehetseges_utvonalak;
    string honnan, hova;
    int mikortol;

    //kiindulasi, celallomas es a kezdeti ora
    honnan="F";
    hova="B";
    mikortol=5;

    //lehetseges utvonalak megkeresese, ha van
    lehetseges_utvonalak = kereses(kereso_graf,honnan, hova, mikortol);
    //lehetseges utvonalak kiirasa
    cout << "Eredmeny: honnan " << honnan << " hova " << hova << " mikortol: " << mikortol << endl;
    cout << endl << "Lehetseges utvonalak (ha van):" << endl;
    for (size_t j=0;j<lehetseges_utvonalak.size();j++){
        cout << endl << "Lehetseges utvonal:" << endl;
        for (size_t i=0;i<lehetseges_utvonalak[j].size();i++){
            cout << "Allomas neve: " << lehetseges_utvonalak[j][i].nev << "\t ideje: " << lehetseges_utvonalak[j][i].ido << "\t vonat indexe: " << lehetseges_utvonalak[j][i].vonat_index << " (";
            cout << vonat_vec[lehetseges_utvonalak[j][i].vonat_index].vonat_nev << ")" << endl;
        }
    }



    return 0;
}
