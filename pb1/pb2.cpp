// Olivier Duguay et Vincent Boogaart
// Programme implementant la machine a etats necessaire pour repondre a l'enonce du probleme 2 du travail pratique 2.
// A0, A1 : branchement DEL bicolore
// D2 : branchement bouton poussoir 1
// D3 : branchement bouton poussoir 2

/*
Etat present        D2  D3   Etat Suivant    Sortie A0   A1
    Eteint          0   X       Eteint          0        0
    Eteint          1   X       semiPretRouge   0        0
    semiPretRouge   1   X       semiPretRouge   0        0
    semiPretRouge   0   X       Rouge           0        0
    Rouge           0   0       Rouge           0        1
    Rouge           1   0       RougeAVert      0        1
    RougeAVert      1   X       RougeAVert      0        1
    RougeAVert      0   X       Vert            0        1
    Rouge           0   1       RougeAEteint2   0        1
    RougeAEteint2   X   1       RougeAEteint2   0        1
    RougeAEteint2   X   0       Eteint2         0        1
    Eteint2         0   0       Eteint2         0        0
    Eteint2         0   1       Eteint2AVert    0        0
    Eteint2AVert    X   1       Eteint2AVert    0        0
    Eteint2AVert    X   0       Vert            0        0
    Vert            0   0       Vert            1        0
    Vert            1   0       VertAEteint     1        0
    Vert            0   1       VertAEteint     1        0
    VertAEteint     1   0       VertAEteint     1        0
    VertAEteint     0   1       VertAEteint     1        0
    VertAEteint     0   0       Eteint          1        0
*/


#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>


const double SECONDE_DELAI = 1000;
const double DELAI_ANTIREBOND = 10; 
enum etats { Eteint, semiPretRouge, Rouge, RougeAVert, RougeAEteint2, Eteint2, Eteint2AVert, Vert, VertAEteint };

bool antiRebond()
{
    bool rebond = false;
    if(PIND & 0x04)
    {
        _delay_ms(DELAI_ANTIREBOND);
        if(!(PIND & 0x04))
            rebond = true;
    }
    else if(PIND & 0x08) 
    {
        _delay_ms(DELAI_ANTIREBOND);
        if(!(PIND & 0x08))
            rebond = true;
    }
    return rebond;
}

int main()
{
    DDRA = 0xFF;
    DDRD = 0x00;
    etats etat = Eteint;
    uint8_t D2;
    uint8_t D3;
    const uint8_t vert = 1;
    const uint8_t rouge = 2; 
    for(;;)
    {
        D2 = PIND & 0x04;
        D3 = PIND & 0x08;
        if (antiRebond())
            continue;
        switch(etat)
        {
            case Eteint :
                PORTA = 0;
                if(D2)
                    etat = semiPretRouge;
                break;
                
            case semiPretRouge :
                PORTA = 0;
                if(!(D2))
                    etat = Rouge;
                break;

            case Rouge :
                PORTA = rouge;
                if(D2)
                    etat = RougeAVert;
                if(D3)
                    etat= RougeAEteint2;
                break;

            case RougeAVert : 
                PORTA = rouge;
                if(!D2)
                    etat = Vert;
                break;

            case RougeAEteint2 :
                PORTA = rouge;
                if(!D3)
                    etat = Eteint2;
                break;

            case Eteint2 :
                PORTA = 0;
                if(D3)
                    etat = Eteint2AVert;
                break;
            
            case Eteint2AVert :
                PORTA = 0;
                if(!D3)
                    etat = Vert;
                break;

            case Vert :
                PORTA = vert;
                if(D2 || D3)
                    etat = VertAEteint;
                break;

            case VertAEteint :
                PORTA = vert;
                if(!(D2 || D3))
                    etat = Eteint;
                break;

        }
        _delay_ms(DELAI_ANTIREBOND);
    }
}
