#include <iostream>
#include <string>

using namespace std;

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

string trans(int num) {
    switch (num)
    {
    case 0:
        return "Leer";
    case 1:
        return "WiKra";
    case 2:
        return "WaKra";
    case 3:
        return "Solar";
    default:
        return "Leer";
    }
}

int main()
{
    int breiteBaubereich = abfrage_breitebaubereich();
    int laengeBaubereich = abfrage_laengebaubereich();
    int gewaehlteOption;

    int topLeftX = -1;
    int topLeftY = -1;
    int bottomRightX = -1;
    int bottomRightY = -1;

    enum gebaeudetypen
    {
        Leer,
        WaKra,
        WiKra,
        Solar
    };

    gebaeudetypen** baubereich = new gebaeudetypen * [breiteBaubereich];

    for (int i = 0; i < breiteBaubereich; i++) {
        baubereich[i] = new gebaeudetypen[laengeBaubereich];
    }

    for (int i = 0; i < breiteBaubereich; i++) {
        for (int j = 0; j < laengeBaubereich; j++) {
            baubereich[i][j] = Leer;
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
            bool setzbar = false;
            int laenge = -1;
            int breite = -1;
            string artInput;

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

                cout << "Bitte Art des Gebaeudes eingeben (WaKra, WiKra, Solar):" << endl;
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
                            if (baubereich[i][j] != 0)
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
                    for (int i = topLeftX; i <= (topLeftX + breite - 1); i++)
                    {
                        for (int j = topLeftY; j <= (topLeftY + laenge - 1); j++)
                        {
                            if (artInput == "WaKra")
                            {
                                baubereich[i][j] = WaKra;
                            }
                            else
                            {
                                if (artInput == "WiKra")
                                {
                                    baubereich[i][j] = WiKra;
                                }
                                else
                                {
                                    if (artInput == "Solar") {
                                        baubereich[i][j] = Solar;
                                    }
                                    else {
                                        cout << "Bitte gueltige Art des Gebaeudes waehlen!" << endl;
                                        setzbar = false;
                                    }
                                }
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

            break;
        }
        case 2: // Bereich loeschen
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
                    baubereich[i][j] = Leer;
                }
            }

            cout << "Bereich erfolgreich geloescht!" << endl;

            topLeftX = -1;
            topLeftY = -1;
            bottomRightX = -1;
            bottomRightY = -1;
            break;
        }
        case 3: // Bauplan ausgeben
            for (int i = 0; i < laengeBaubereich; i++)
            {
                for (int j = 0; j < breiteBaubereich; j++)
                {
                    cout << trans(baubereich[j][i]) << "\t";
                }
                cout << endl;
            }
            break;
        case 4: // Exit
            return 0;
        default:
            cout << "Bitte eine gueltige Option eingeben!" << endl;
        }
    }

    return 0;
}