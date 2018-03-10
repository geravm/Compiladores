#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define PALABRAS_RESERVADAS       7
#define LARGO_PALABRAS_RESERVADAS 8
#define OPERADORES_LOGICOS        3
#define LARGO_OPERADORES_LOGICOS  4

FILE *lexemeStart, *avance, *finalOfFile;
int numeroLinea = 1;
char ultimoLexema[100];
char arrOperadoresLogicos[OPERADORES_LOGICOS][LARGO_OPERADORES_LOGICOS] = {"and", "or", "not"};
char palabrasReservadas[PALABRAS_RESERVADAS][LARGO_PALABRAS_RESERVADAS] = {"program",
                                                                "var","begin",
                                                                "end","if",
                                                                "then","else"};
char mensajedeError[] = "Error en la linea ";
int error_token = 0;

void aceptarPalabra()
{
    char buffer [100];


    long inicioPalabra = ftell(lexemeStart);
    long finPalabra = ftell(avance);
    if(ftell(avance)== ftell(finalOfFile))
    {
        ++finPalabra;
    }
    // fgets prints whatever is between the starting point and the end point
    fgets(buffer, finPalabra - inicioPalabra, lexemeStart);
    printf ("[%s]\t\t", buffer);
    fseek(lexemeStart, finPalabra, SEEK_SET);
}
void aceptarComentario()
{

    long inicioPalabra = ftell(lexemeStart);
    long finPalabra = ftell(avance);
    // fgets prints whatever is between the starting point and the end point
    fseek(lexemeStart, finPalabra, SEEK_SET);
}

void aceptarEspacio()
{
    long finPalabra = ftell(avance);
    fseek(lexemeStart, finPalabra, SEEK_SET);
}

void rejectWord()
{
    long inicioPalabra = ftell(lexemeStart);
    fseek(avance, inicioPalabra, SEEK_SET);
}

char getNextChar()
{
    return fgetc(avance);
}

int isNormalChar(int c)
{
    return (c != ' ' && c != '\n' && c != '\t' && c != EOF);
}

int isPuntuacion(int c)
{
    return (c != '.' && c != ',' && c != ';', c != '(', c != ')');
}

int equalizer()
{
    char c = getNextChar();

    if (c == ' ' || c =='\n' || c == '\t')
    {
        if(c == '\n')
        {
            ++numeroLinea;
        }
        return 1;
    }
    rejectWord();
    return -1;
}

void operacionError()
{
    //printf("%s %d\n", mensajedeError, numeroLinea);
    exit(0);
}

int operadoresLogicos()
{
    char c = getNextChar();
    char construirPalabra [LARGO_OPERADORES_LOGICOS];
    int contador, palabra, actualState;
    palabra = contador = actualState = 0;

    while(isNormalChar(c) && actualState != -1 && contador<LARGO_OPERADORES_LOGICOS)
    {
        if(isalpha(c))
        {
            construirPalabra[contador++] = c;
        }
        else
        {
            actualState = -1;
        }
        c = getNextChar();
    }
    construirPalabra[contador]= '\0';
    for(palabra = 0; palabra < OPERADORES_LOGICOS; ++palabra)
    {
        if(strcmp(arrOperadoresLogicos[palabra], construirPalabra)== 0)
        {
            return 1;
        }

    }


    rejectWord();
    return -1;
}

int palabraReservada()
{
    char c = getNextChar();
    char construirPalabra [LARGO_PALABRAS_RESERVADAS];
    int contador, palabra, actualState;
    palabra = contador = actualState = 0;

    while(isNormalChar(c) && c != '(' && isPuntuacion(c) && actualState != -1 && contador<LARGO_PALABRAS_RESERVADAS)
    {
        if(isalpha(c))
        {
            construirPalabra[contador++] = c;
        }
        else
        {
            actualState = -1;
        }
        c = getNextChar();
    }
    construirPalabra[contador]= '\0';

    for(palabra = 0; palabra < PALABRAS_RESERVADAS; ++palabra)
    {
        if(strcmp(palabrasReservadas[palabra], construirPalabra)== 0)
        {
            return 1;
        }

    }


    rejectWord();
    return -1;
}

int caracterDelimitacion()
{
    char c = getNextChar();
    int actualState = 0;

    switch(c)
    {
        case '(':
            actualState = 1;
            break;
        case ')':
            actualState = 2;
            break;
        case '[':
            actualState = 3;
            break;
        case ']':
            actualState = 4;
            break;
        default:
            actualState = -1;
            break;
    //ok so this seems to be the key to avoid using the hardcoded 1 at the top of acceptword
    //c = getNextChar();
    }
    if(actualState == 1 || actualState == 2 || actualState == 3 || actualState == 4)
    {
        return actualState;
    }
    rejectWord();
    return -1;
}

int assignOperation()
{
    char c = getNextChar();
    int actualState = 0;
    while(isNormalChar(c) && actualState != -1)
    {
        switch(actualState)
        {
        case 0:
            switch(c)
            {
            case '=':
                actualState = 1;
                break;
            default:
                actualState = -1;
                break;
            }
            break;
        default:
            actualState = -1;
        }
        c = getNextChar();

    }

    if(actualState == 1)
    {
        return actualState;
    }
    rejectWord();
    return -1;
}

int arithmeticOperations()
{
    char c = getNextChar();
    int actualState = 0;
    while(isNormalChar(c) && actualState != -1)
    {
        switch(actualState)
        {
        case 0:
            switch(c)
            {
            case '+':
                actualState = 1;
                break;
            case '-':
                actualState = 2;
                break;
            case '*':
                actualState = 3;
                break;
            case '/':
                actualState = 4;
                break;
            default:
                actualState = -1;
                break;
            }
            break;
        default:
            actualState = -1;
        }
        c = getNextChar();

    }

    if(actualState == 1 || actualState == 2 || actualState == 3 || actualState == 4)
    {
        return actualState;
    }
    rejectWord();
    return -1;
}

int comment()
{
    char c = getNextChar();
    int actualState = 0;
    while(isNormalChar(c) && actualState != -1)
    {
        if(actualState == 0 && c == '/')
        {
            actualState = 1;
        }
        else if (actualState == 1 && c == '/')
        {
            actualState = 2;

        }
        else
        {
            actualState = -1;
        }
        c = getNextChar();

    }

    if(actualState == 2)
    {
        while( c != '\n' && c != EOF)
        {
            c = getNextChar();
        }
    }

    if(actualState == 2)
    {
        return actualState;
    }
    rejectWord();
    return -1;
}

int punctuationSign()
{
    char c = getNextChar();
    int actualState = 0;
    while(isNormalChar(c) && actualState != -1)
    {
        switch(actualState)
        {
        case 0:
            switch(c)
            {
            case '.':
                actualState = 1;
                break;
            case ',':
                actualState = 2;
                break;
            case ';':
                actualState = 3;
                break;
            default:
                actualState = -1;
                break;
            }
            break;
        default:
            actualState = -1;
        }
        c = getNextChar();

    }

    if(actualState == 1 || actualState == 2 || actualState == 3)
    {
        return actualState;
    }
    rejectWord();
    return -1;
}

int identifier()
{
    int actualState = 0;
    char c = getNextChar();
    while(isNormalChar(c) && c != ','  && c != ')' && actualState != -1)
    {
        if(isalpha(c))
        {
            actualState = 1;
        }
        else if(c == '_')
        {
            actualState = 2;
        }
        else
        {
            actualState = -1;
        }
        if(actualState == 1)
        {
            actualState = (isalpha(c) || isdigit(c) || c == '_') ? 1 : -1;
        }
        else if(actualState == 2)
        {
            if(isalpha(c))
            {
                actualState = 1;
            }
            else if(c == '_' || isdigit(c))
            {
                actualState = 2;
            }
            else
            {
                actualState = -1;
            }
        }
        c = getNextChar();
    }

    if(actualState == 1)
    {
        return actualState;
    }
    rejectWord();
    return -1;
}

int numeros()
{
    int actualState = 0;
    char c = getNextChar();

    while (isNormalChar(c) && actualState != -1 && c != ';')
    {
        switch (actualState){
        case 0:
            if(c == '0')
            {
                actualState = 1;
            }
            else if(isdigit(c))
            {
                actualState = 2;
            }
            else
            {
                actualState= -1;
            }
            break;
        case 1:
            if(c >= '1' && c <= '7')
            {
                actualState = 3;
            }
            else if(c == 'x' || c == 'X')
            {
                actualState = 4;
            }
            else if(c == '.')
            {
                actualState = 5;
            }
            else
            {
                actualState= -1;
            }
            break;
        case 2:
            if(isdigit(c))
            {
                actualState = 2;
            }
            else if(c == '.')
            {
                actualState = 5;
            }
            else
            {
                actualState = -1;
            }
            break;
        case 3:
            actualState = (c >= '0' && c <= '7') ? 3 : -1;
            break;
        case 4:
            actualState = (c >= '0' && c <= '9' || c >= 'a' && c <= 'f'|| c >= 'A' && c <= 'F') ? 4 : -1;
            break;
        case 5:
            actualState = (isdigit(c)) ? 6:-1;
            break;
        case 6:
            if(isdigit(c))
            {
                actualState = 6;
            }
            else if(c == 'e' || c == 'E')
            {
                actualState = 7;
            }
            else
            {
                actualState = -1;
            }
            break;
        case 7:
            if(isdigit(c))
            {
                actualState = 8;
            }
            else if(c == '+' || c == '-')
            {
                actualState = 9;
            }
            break;
        case 8:
            actualState = (isdigit(c)) ? 8 : -1;
            break;
        case 9:
            actualState = (isdigit(c)) ? 8 : -1;
            break;
        default:
            actualState = -1;
            break;
        }
        c = getNextChar();
    }

    if (actualState == 2 || actualState == 3 || actualState == 4 || actualState == 6 || actualState == 8)
    {
        return actualState;
    }
    rejectWord();

    return -1;
}

int oprel()
{
    int actualState = 0;
    int c = getNextChar();


    while (isNormalChar(c) && actualState != -1)
    {
        switch (actualState){
        case 0:
            switch (c){
            case '=':
                actualState = 1;
                break;
            case '!':
                actualState = 3;
                break;
            case '>':
                actualState = 5;
                break;
            case '<':
                actualState = 7;
                break;
            default:
                actualState = -1;
                break;
            }
            break;
        case 1:
            actualState = (c == '=') ? 2 : -1;
            break;
        case 3:
            actualState = (c == '=') ? 4 : -1;
            break;
        case 5:
            actualState = (c == '=') ? 6 : -1;
            break;
        case 7:
            actualState = (c == '=') ? 8 : -1;
            break;
        default:
            actualState = -1;
            break;
        }
        c = getNextChar();
    }

    if (actualState == 2 || actualState == 4 || actualState == 5 || actualState == 6 || actualState == 7 || actualState == 8)
    {
        return actualState;
    }
    // if the word is rejected then we need to move the pointer to the beginning if the word.
    rejectWord();

    return -1;
}

int isfeof()
{
    char c = getNextChar();
    rejectWord();
    return c == EOF;
}

int main()
{
    lexemeStart = fopen("origen.txt", "r");
    avance = fopen("origen.txt", "r");
    finalOfFile = fopen("origen.txt", "r");
    fseek(finalOfFile, 0, SEEK_END);

    int c;

    while (!isfeof())
    {
        if ((c = oprel()) != -1)
        {
            aceptarPalabra();
            switch(c)
            {
                case 2:
                    printf("Operador igualdad \n");
                    break;
                case 4:
                    printf("Operador desigualdad \n");
                    break;
                case 5:
                    printf("Operador mayor que \n");
                    break;
                case 6:
                    printf("Operador mayor ó igual que \n");
                    break;
                case 7:
                    printf("Operador menor que \n");
                    break;
                case 8:
                    printf("Operador menor ó igual que \n");
                    break;
            }
        }
        else if(equalizer() != -1)
        {
            aceptarEspacio();
        }
        else if(comment()!=-1)
        {
            aceptarComentario();
        }
        else if((c = numeros()) != -1)
        {
            aceptarPalabra();
            switch(c)
            {
                case 2:
                    puts("Numero natural");
                    break;
                case 3:
                    puts("Numero octal");
                    break;
                case 4:
                    puts("Numero hexadecimal");
                    break;
                case 6:
                    puts("Numero de punto flotante");
                    break;
                case 8:
                    puts("Numero de punto flotante");
                    break;
            }
        }
        else if(palabraReservada()!=-1)
        {
            aceptarPalabra();
            puts("Palabra reservada");
        }
        else if(operadoresLogicos()!=-1)
        {
            aceptarPalabra();
            puts("Operador logico");
        }
        else if(identifier() != -1)
        {
            aceptarPalabra();
            puts("Identificador");
        }
        else if((c=caracterDelimitacion())!=-1)
        {
            aceptarPalabra();
            switch(c)
            {
                case 1:
                    puts("Parentesis izquierdo");
                    break;
                case 2:
                    puts("Parentesis derecho");
                    break;
                case 3:
                    puts("Corchete izquierdo");
                    break;
                case 4:
                    puts("Corchete derecho");
                    break;
            }
        }
        else if((c=arithmeticOperations())!=-1)
        {
            aceptarPalabra();
            switch(c)
            {
                case 1:
                    puts("suma");
                    break;
                case 2:
                    puts("resta");
                    break;
                case 3:
                    puts("multiplicacion");
                    break;
                case 4:
                    puts("division");
                    break;
            }
        }
        else if(assignOperation()!=-1)
        {
            aceptarPalabra();
            puts("Asignacion");
        }
        else if((c=punctuationSign()) !=-1)
        {
            aceptarPalabra();
            switch(c)
            {
                case 1:
                    puts("Punto");
                    break;
                case 2:
                    puts("Coma");
                    break;
                case 3:
                    puts("Punto y coma");
                    break;
            }
        }
        else
        {
            operacionError();
        }
    }

    fclose(lexemeStart);
    fclose(avance);
    //system("PAUSE");
    system("read -p 'Press Enter to continue...' var");
    //getc(stdin);
    return 0;
}
