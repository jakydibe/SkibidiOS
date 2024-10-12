#include "strings.h"

#define TXT_BLANCO 0x0F //blanco sobre negro
#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define CURSOR_STYLE 0x70  // Stile speciale per il cursore, es: bianco su sfondo nero
#define NORMAL_STYLE 0x07  // Stile normale, es: bianco su sfondo nero


typedef struct{
    char chr;
    VGA_STYLE style;
} VGA_CHAR;

int cursorX;
int cursorY;

VGA_CHAR (*memoria_video)[VGA_WIDTH] = (VGA_CHAR (*)[VGA_WIDTH]) VGA_MEMORY;


void show_cursor() {
    memoria_video[cursorY][cursorX].style = CURSOR_STYLE;
}


void hide_cursor() {
    memoria_video[cursorY][cursorX].style = TXT_BLANCO;
}

void move_cursor(int newX, int newY) {
    hide_cursor();
    
    cursorX = newX;
    cursorY = newY;
    
    show_cursor();
}

int strlen(char* string){
    int i = 0;
    while(string[i] != '\0'){
        i++;
    }
    if (i == 0){
        return -1;
    } 
    else{
        return i;
    }
}

char digit_to_char(int digit){
    return digit + '0';
}

void putc(char chr){
    int newX = cursorX, newY = cursorY;
    switch (chr)
    {
    case '\n':
        newY++;
        newX=0;
        break;
    
    case '\b':
        //quando premo \b il cursore va indietro
        if (newX == 0 && newY == 0) {
            break;
        }

        // Gestione del backspace quando il cursore è all'inizio della riga
        if (newX == 0) {
            newY--;  
            newX = VGA_WIDTH - 1;  
            // Trova l'ultimo carattere non nullo sulla riga precedente
            while (newX > 0 && memoria_video[newY][newX].chr == 0x0) {
                newX--;
            }

            // Se abbiamo trovato un carattere non nullo, posizioniamoci subito dopo
            if (memoria_video[newY][newX].chr != 0x0) {
                newX++;  // Posiziona il cursore dopo l'ultimo carattere non nullo
            }
        } else {
            newX--;  // Altrimenti, vai indietro di un carattere
        }

        // Cancella il carattere alla posizione attuale
        memoria_video[newY][newX].style = 0x0;
        memoria_video[newY][newX].chr = 0x0;

        break;
    default:
        memoria_video[newY][newX].chr = chr;
        memoria_video[newY][newX].style = TXT_BLANCO;
        newX++;
    }

    if (newX > 80){
        newX = newX % 80;
        newY += 1;  
    }
    move_cursor(newX, newY);
}


void puts(char* string){
    int newX = cursorX, newY = cursorY;
    for(int i = 0; string[i] != 0x0; newX++, i++){
        if (string[i] == '\n'){
            newY++;
            newX=-1;
            continue;
        }

        memoria_video[newY][newX].chr = string[i];
        memoria_video[newY][newX].style = TXT_BLANCO;
        
        if (newX > 80){
            newX = newX % 80;
            newY += 1;  
        }
    }
    move_cursor(newX, newY);

}

void hexprint(unsigned int hexn){
    puts("0x");
    //Make hexn a string
    int mask = 0xF0000000;
    for(int i = 0; i < 8; i++){
        char byte = (hexn & mask) >> 28; //low byte
        char chr;
        if(byte > 9){
            chr = byte + 'A' - 9;
        } else {
            chr = byte + '0';
        }
        
        putc(chr);
        hexn = hexn << 4;
    }
}




void swag_print(char* string, VGA_STYLE style){
    for(int i = 0; string[i] != 0x0; cursorX++, i++){
        if (string[i] == '\n'){
            cursorY++;
            cursorX=-1;
            continue;
        }

        memoria_video[cursorY][cursorX].chr = string[i];
        memoria_video[cursorY][cursorX].style = style;
        
        if (cursorX > 80){
            cursorX = cursorX % 80;
            cursorY += 1;  
        }
    } 
}

void clear_screen(){
    for (int i = 0; i < VGA_HEIGHT; i++){
        for (int j = 0; j < VGA_WIDTH; j++){
            memoria_video[i][j].chr = ' ';
            memoria_video[i][j].style = 0x0;
        }
    }  
}

void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    /* TODO: implement "reverse" */
}

