
char* pilotageLED(char nbLED, char lumi, char bleu, char vert, char rouge, char tab[]);
// Permet de changer les couleurs et luminosité des led 
// Les valeurs sont a donner en hexa et le tableau est a fournir aussi
// Pensez a fair un tableau de 248 valeurs avant

void Lum_init(void);
//Permet d'initialiser les ports et la Bibliotheque SPI

short LectureADC();
// fait une lecture sur l'ADC pour le capteur de lumière
//L'attente de conversion est incluse

void tabSend(char tab[]);
// Permet d'envoyer par SPI le tableau pour actualiser les LEDs