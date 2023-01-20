#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Material {
public:
    double preis;
    string name;

    double getPreis() {
        return preis;
    }

    string getName() {
        return name;
    }

    bool operator<(const Material& other) const {
        return this->preis < other.preis;
    }
};

class Holz : public Material {
public:
    Holz() {
        this->preis = 5.0;
        this->name = "Holz";
    }
};

class Metall : public Material {
public:
    Metall() {
        this->preis = 10.0;
        this->name = "Metall";
    }
};

class Kunststoff : public Material {
public:
    Kunststoff() {
        this->preis = 7.50;
        this->name = "Kunststoff";
    }
};

//Annahme: Einzeilpreis ergibt sich aus Grundpreis des Gebaeudes + den Kosten pro benoetigtes Material, welche pro Feld aufgewendet werden müssen

class Building {
public:
    double grundpreis;
    double einzelpreis = 0;
    int groesse = 0;
    string label;
    map<Material, int> benoetigteMaterialien;   //Carlas Idee, da Map in diesem Fall sehr naheliegend ist (Zuordnung von bestimmten Material(Key) zu variierender Anzahl(Value))
    map<Material, int>::iterator it;            //Für diesen Zweck wäre eine extra Verwaltungsklasse meiner Meinung nach unnötig

    string getLabel() {
        return label;
    }

    double getGrundpreis() {
        return grundpreis;
    }

    void setEinzelpreis(double einzelpreis) {
        this->einzelpreis = einzelpreis;
    }

    double getEinzelpreis() {
        return einzelpreis;
    }

    int getGroesse() {
        return groesse;
    }

    void setGroesse(int size) {
        this->groesse = size;
        calcEinzelpreis();
    }

    void printMaterialien() {
        for (it = benoetigteMaterialien.begin(); it != benoetigteMaterialien.end(); it++) {
            cout << (this->getGroesse() * it->second) << "x " << it->first.name << "\t";
        }
    }

    void calcEinzelpreis() {    //Berechnung des Einzelpreises
        this->einzelpreis = 0;
        for (it = benoetigteMaterialien.begin(); it != benoetigteMaterialien.end(); it++) {
            this->einzelpreis += this->getGroesse() * it->second * it->first.preis;
        }
        this->einzelpreis += this->grundpreis;
    }
};

class Leer : public Building {
public:
    Leer() {
        this->grundpreis = 0.0;
        this->label = "leer";
        this->benoetigteMaterialien = {};
    }
};

class Wasserkraftwerk : public Building {
public:
    Wasserkraftwerk(string label, int size) {
        this->grundpreis = 1000.0;
        this->label = label;
        this->groesse = size;
        this->benoetigteMaterialien = { {Holz(), 2}, {Metall(), 1} };
        calcEinzelpreis();
    }
};

class Windkraftwerk : public Building {
public:
    Windkraftwerk(string label, int size) {
        this->grundpreis = 500.0;
        this->label = label;
        this->groesse = size;
        this->benoetigteMaterialien = { {Kunststoff(), 1}, {Metall(), 2} };
        calcEinzelpreis();
    }
};

class Solaranlage : public Building {
public:
    Solaranlage(string label, int size) {
        this->grundpreis = 100.0;
        this->label = label;
        this->groesse = size;
        this->benoetigteMaterialien = { {Kunststoff(), 2}, {Metall(), 1} };
        calcEinzelpreis();
    }
};

int abfrage_laengebaubereich() //Abfrage der Länge des Baubereichs
{
    int laenge;

    cout << "Bitte die Laenge des Baubereichs eingeben." << endl;
    cin >> laenge;

    return laenge;
}

int abfrage_breitebaubereich() //Abfrage der Breite des baubereichs
{
    int breite;
    cout << "Bitte die Breite des Baubereichs eingeben." << endl;
    cin >> breite;

    return breite;
}

void display_menu() //Menü-Anzeige
{
    cout << "Bitte entsprechende Nummer eingeben:" << endl;
    cout << "(1) Gebaeude setzen" << endl;
    cout << "(2) Bereich loeschen" << endl;
    cout << "(3) Bauplan anzeigen" << endl;
    cout << "(4) Exit" << endl;
}


class CapycitySim {
public:
    int breiteBaubereich;
    int laengeBaubereich;
    int topLeftX;
    int topLeftY;
    int bottomRightX;
    int bottomRightY;
    double gesamtPreis = 0;
    bool buildingFound = false;
    Building** baubereich;
    vector<Building> vorhandeneGebaeude = {};

    void setze_gebaeude() { //Gebäude setzen
        bool setzbar = false;
        int laenge = -1;
        int breite = -1;
        string artInput;
        string labelInput;

        while (!setzbar) //Abfrage gültiger Koordinaten
        {
            while (topLeftX == -1)
            {
                cout << "Bitte die x-Koordinate der linken oberen Ecke eingeben:";
                cin >> topLeftX;

                if ((topLeftX < 0) || (topLeftX > breiteBaubereich))
                {
                    cout << "Bitte gueltigen Wert eingeben!" << endl;
                    topLeftX = -1;
                }
            }

            while (topLeftY == -1)
            {
                cout << "Bitte die y-Koordinate der linken oberen Ecke eingeben:";
                cin >> topLeftY;

                if ((topLeftY < 0) || (topLeftY > laengeBaubereich))
                {
                    cout << "Bitte gueltigen Wert eingeben!" << endl;
                    topLeftX = -1;
                }
            }

            cout << "Bitte Art des Gebaeudes eingeben (Wasserkraftwerk, Windkraftwerk, Solaranlage):" << endl; //Abfrage von Art, Breite und Länge
            cin >> artInput;
            cout << "Bitte Breite des gewuenschten Gebaeudes eingeben:" << endl;
            cin >> breite;
            cout << "Bitte Laenge des gewuenschten Gebaeudes eingeben:" << endl;
            cin >> laenge;

            if (((topLeftX + breite) <= breiteBaubereich) && ((topLeftY + laenge) <= laengeBaubereich)) //Prüfen, ob Gebäude gebaut werden kann und nicht kollidiert oder außerhalb liegt
            {
                setzbar = true;

                for (int i = topLeftX; i < (topLeftX + breite); i++)
                {
                    for (int j = topLeftY; j < (topLeftY + laenge); j++)
                    {
                        if (baubereich[i][j].getLabel() != "leer")
                        {
                            setzbar = false;
                            cout << "Zu bauendes Gebaeude kollidiert mit einem, dass bereits besteht!" << endl;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            topLeftX = -1;
                            topLeftY = -1;
                        }
                    }
                }
            }
            else
            {
                setzbar = false;
                cout << "Gebaeude liegt nicht innerhalb des Baubereichs!" << endl;
            }



            if (setzbar)
            {
                //Label-Abfrage, Dopplungen ausgeschlossen
                while (labelInput == "") {
                    cout << "Bitte gewuenschtes Label fuer das zu erstellende Gebaeude eingeben:" << endl;
                    cin >> labelInput;

                    for (int i = 0; i < breiteBaubereich; i++) {
                        for (int j = 0; j < laengeBaubereich; j++) {
                            if ((baubereich[i][j].getLabel() != "leer") && (baubereich[i][j].getLabel() == labelInput)) {
                                cout << "Label wird bereits verwendet, denke dir ein anderes aus!" << endl;
                                labelInput = "";
                            }
                        }
                    }
                }

                //Erstellung eines entsprechenden Gebaeude-Objekts und einfügen in den Bauplan
                if (artInput == "Wasserkraftwerk")
                {
                    Wasserkraftwerk addedBuilding = Wasserkraftwerk(labelInput, (breite * laenge));

                    for (int i = topLeftX; i <= (topLeftX + breite - 1); i++)
                    {
                        for (int j = topLeftY; j <= (topLeftY + laenge - 1); j++)
                        {
                            baubereich[i][j] = addedBuilding;
                        }
                    }
                }
                else
                {
                    if (artInput == "Windkraftwerk")
                    {
                        Windkraftwerk addedBuilding = Windkraftwerk(labelInput, (breite * laenge));

                        for (int i = topLeftX; i <= (topLeftX + breite - 1); i++)
                        {
                            for (int j = topLeftY; j <= (topLeftY + laenge - 1); j++)
                            {
                                baubereich[i][j] = addedBuilding;
                            }
                        }
                    }
                    else
                    {
                        if (artInput == "Solaranlage") {
                            Solaranlage addedBuilding = Solaranlage(labelInput, (breite * laenge));

                            for (int i = topLeftX; i <= (topLeftX + breite - 1); i++)
                            {
                                for (int j = topLeftY; j <= (topLeftY + laenge - 1); j++)
                                {
                                    baubereich[i][j] = addedBuilding;
                                }
                            }
                        }
                        else {
                            cout << "Bitte gueltige Art des Gebaeudes waehlen!" << endl;
                            setzbar = false;
                        }
                    }
                }
            }
        }

        topLeftX = -1;
        topLeftY = -1;
        setzbar = false;
        laenge = -1;
        breite = -1;
        labelInput = "";
    }

    void loesche_gebaeude() {   //Löschen eines bestimmten Bereichs
        while (topLeftX == -1)  //Abfrage gültiger Koordinaten des zu löschenden Bereichs
        {
            cout << "Bitte die x-Koordinate der linken oberen Ecke eingeben:";
            cin >> topLeftX;

            if ((topLeftX < 0) || (topLeftX > breiteBaubereich))
            {
                cout << "Bitte gueltigen Wert eingeben!" << endl;
                topLeftX = -1;
            }
        }

        while (topLeftY == -1)
        {
            cout << "Bitte die y-Koordinate der linken oberen Ecke eingeben:";
            cin >> topLeftY;

            if ((topLeftY < 0) || (topLeftY > laengeBaubereich))
            {
                cout << "Bitte gueltigen Wert eingeben!" << endl;
                topLeftX = -1;
            }
        }

        while (bottomRightX == -1)
        {
            cout << "Bitte die x-Koordinate der rechten unteren Ecke eingeben:";
            cin >> bottomRightX;

            if ((bottomRightX < 0) || (bottomRightX > breiteBaubereich) || (bottomRightX < topLeftX))
            {
                cout << "Bitte gueltigen Wert eingeben!" << endl;
                bottomRightX = -1;
            }
        }

        while (bottomRightY == -1)
        {
            cout << "Bitte die y-Koordinate der rechten unteren Ecke eingeben:";
            cin >> bottomRightY;

            if ((bottomRightY < 0) || (bottomRightY > laengeBaubereich) || (bottomRightY < topLeftY))
            {
                cout << "Bitte gueltigen Wert eingeben!" << endl;
                bottomRightY = -1;
            }
        }

        for (int i = topLeftX; i <= bottomRightX; i++)      //Größe der Gebäude, die auf zu löschenden Flächen stehen verkleinern und durch freie Fläche ersetzen
        {
            for (int j = topLeftY; j <= bottomRightY; j++)
            {
                baubereich[i][j].setGroesse((baubereich[i][j].getGroesse() - 1));
                baubereich[i][j] = Leer();
            }
        }

        cout << "Bereich erfolgreich geloescht!" << endl;

        topLeftX = -1;
        topLeftY = -1;
        bottomRightX = -1;
        bottomRightY = -1;
    }

    void print_baubereich() {                                           //Ausgabe des Bauplans
        for (int i = 0; i < laengeBaubereich; i++)
        {
            for (int j = 0; j < breiteBaubereich; j++)
            {
                cout << baubereich[j][i].getLabel() << "\t";
            }
            cout << endl;
        }

        updateVorhandeneGebäude();

        for (Building b : vorhandeneGebaeude) {                             //Ausgabe der Gebäude und ihren Materialien und Preisen + Gesamtpreis
            cout << endl << b.getLabel() << "\t";
            b.printMaterialien();
            cout << "\t" << b.getEinzelpreis();
            gesamtPreis += b.getEinzelpreis();
        }

        cout << endl << "Gesamtpreis: " << getGesamtPreis() << " Euro" << endl << endl;
        gesamtPreis = 0;
    }

    void updateVorhandeneGebäude() { //Erstellt Liste mit allen vorhandenen Gebäuden, keine Dopplungen oder leere Felder
        vorhandeneGebaeude.clear();

        for (int i = 0; i < breiteBaubereich; i++) {
            for (int j = 0; j < laengeBaubereich; j++) {
                for (Building b : vorhandeneGebaeude) {
                    if (baubereich[i][j].getLabel() == b.getLabel() || baubereich[i][j].getLabel() == "leer")
                        buildingFound = true;
                }

                if (!buildingFound)
                    vorhandeneGebaeude.push_back(baubereich[i][j]);
                buildingFound = false;
            }
        }
    }

    void calcGesamtPreis() {
        updateVorhandeneGebäude();
        gesamtPreis = 0.0;

        for (Building b : vorhandeneGebaeude)
            gesamtPreis += b.getEinzelpreis();
    }

    double getGesamtPreis() {
        calcGesamtPreis();
        return gesamtPreis;
    }

    void init() {
        breiteBaubereich = abfrage_breitebaubereich();      //Abfrage Länge/Breite des Baubereichs
        laengeBaubereich = abfrage_laengebaubereich();
        int gewaehlteOption;

        topLeftX = -1;
        topLeftY = -1;
        bottomRightX = -1;
        bottomRightY = -1;

        baubereich = new Building * [breiteBaubereich];     //Anlegen des Baubereichs und mit "freier Fläche" belegen

        for (int i = 0; i < breiteBaubereich; i++) {
            baubereich[i] = new Building[laengeBaubereich];
        }

        for (int i = 0; i < breiteBaubereich; i++) {
            for (int j = 0; j < laengeBaubereich; j++) {
                baubereich[i][j] = Leer();
            }
        }

        while (true)                            //Menü-Anzeige und Auswahl des Nutzers
        {
            display_menu();

            cin >> gewaehlteOption;

            switch (gewaehlteOption)
            {                        // Optionenwahl
            case 1:                  // Gebaeude setzen
            {
                setze_gebaeude();
                break;
            }
            case 2: // Bereich loeschen
            {
                loesche_gebaeude();
                break;
            }
            case 3: // Bauplan ausgeben
                print_baubereich();
                break;
            case 4: // Exit
                exit(0);
            default:
                cout << "Bitte eine gueltige Option eingeben!" << endl;
            }
        }

    }

    CapycitySim() {
        init();
    }
};

int main()
{
    CapycitySim c = CapycitySim();

    return 0;
}