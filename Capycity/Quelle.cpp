#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Material {
    public: 
        double preis;

        double getPreis() {
            return preis;
        }
};

class Holz : public Material {
    public:
        Holz() {
            this->preis = 5.0;
        }
};

class Metall: public Material {
    public:
        Metall() {
            this->preis = 10.0;
        }
};

class Kunststoff : public Material {
    public:
        Kunststoff() {
            this->preis = 7.50;
        }
};

//Annahme: Einzeilpreis ergibt sich aus Grundpreis des Gebaeudes + den Kosten pro benoetigtes Material
//Annahme: Grundpreis und benoetigtes Material muss pro belegtes Feld aufgewendet werden

class Building {
    public:
        double grundpreis;
        double einzelpreis;
        string label;  
        vector<Material> benoetigteMaterialien;

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
        Wasserkraftwerk(string label) {
            this->grundpreis = 1000.0;
            this->label = label;
            this->benoetigteMaterialien = {Holz(), Holz(), Metall()};
        }
};

class Windkraftwerk : public Building {
    public:
        Windkraftwerk(string label) {
            this->grundpreis = 500.0;
            this->label = label;
            this->benoetigteMaterialien = {Metall(), Metall(), Kunststoff()};
        }
};

class Solaranlage : public Building {
    public:
        Solaranlage(string label) {
            this->grundpreis = 100.0;
            this->label = label;
            this->benoetigteMaterialien = {Kunststoff(), Kunststoff(), Metall()};
        }
};

int abfrage_laengebaubereich()
{
    int laenge;

    cout << "Bitte die Laenge des Baubereichs eingeben." << endl;
    cin >> laenge;

    return laenge;
}

int abfrage_breitebaubereich()
{
    int breite;
    cout << "Bitte die Breite des Baubereichs eingeben." << endl;
    cin >> breite;

    return breite;
}

void display_menu()
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
        Building** baubereich;
        vector<Building> vorhandeneGebaeude = {};

        void setze_gebaeude() {
            bool setzbar = false;
            int laenge = -1;
            int breite = -1;
            string artInput;
            string labelInput;

            while (!setzbar)
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

                cout << "Bitte Art des Gebaeudes eingeben (Wasserkraftwerk, Windkraftwerk, Solaranlage):" << endl;
                cin >> artInput;
                cout << "Bitte Breite des gewuenschten Gebaeudes eingeben:" << endl;
                cin >> breite;
                cout << "Bitte Laenge des gewuenschten Gebaeudes eingeben:" << endl;
                cin >> laenge;

                if (((topLeftX + breite) <= breiteBaubereich) && ((topLeftY + laenge) <= laengeBaubereich))
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

                    //Erstellung eines entsprechenden Gebaeude-Objekts und einfügen in das Bauplan-Array
                    if (artInput == "Wasserkraftwerk")
                    {
                        Wasserkraftwerk addedBuilding = Wasserkraftwerk(labelInput);

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
                            Windkraftwerk addedBuilding = Windkraftwerk(labelInput);

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
                                Solaranlage addedBuilding = Solaranlage(labelInput);

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

        void loesche_gebaeude() {
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

            for (int i = topLeftX; i <= bottomRightX; i++)
            {
                for (int j = topLeftY; j <= bottomRightY; j++)
                {
                    baubereich[i][j] = Leer();
                }
            }

            cout << "Bereich erfolgreich geloescht!" << endl;

            topLeftX = -1;
            topLeftY = -1;
            bottomRightX = -1;
            bottomRightY = -1;
        }

        void print_baubereich() {
            for (int i = 0; i < laengeBaubereich; i++)
            {
                for (int j = 0; j < breiteBaubereich; j++)
                {
                    cout << baubereich[j][i].getLabel() << "\t";
                    
                    for (Building b : vorhandeneGebaeude) {
                        if (b.getLabel() != baubereich[j][i].getLabel()) {
                            vorhandeneGebaeude.push_back(baubereich[j][i]);
                        }
                    }
                }
                cout << endl;
            }
        }

        void init() {
            breiteBaubereich = abfrage_breitebaubereich();
            laengeBaubereich = abfrage_laengebaubereich();
            int gewaehlteOption;

            topLeftX = -1;
            topLeftY = -1;
            bottomRightX = -1;
            bottomRightY = -1;

            baubereich = new Building * [breiteBaubereich];

            for (int i = 0; i < breiteBaubereich; i++) {
                baubereich[i] = new Building[laengeBaubereich];
            }

            for (int i = 0; i < breiteBaubereich; i++) {
                for (int j = 0; j < laengeBaubereich; j++) {
                    baubereich[i][j] = Leer();
                }
            }

            while (true)
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