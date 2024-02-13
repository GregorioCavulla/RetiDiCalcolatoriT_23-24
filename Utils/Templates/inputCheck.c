
int main(int argc, char const *argv[])
{
    char buffer[64];
    ////lettura di una stringa non nulla
    do
    {
        printf("inserire str\n");
        if (gets(buffer) != 0)
            break;
    } while (1);
    evento.str = (char *)malloc(strlen(buffer) + 1);
	strcpy(evento.dest, buffer);

    //lettura di una stringa valida tra scelte
    do
    {
        printf("inserire tipo\n");
        gets(buffer);
        if (strcmp(buffer, "Concerto") == 0 || strcmp(buffer, "Formula1") == 0 ||
            strcmp(buffer, "Calcio") == 0)
            break;
    } while (1);
    evento.tipo = (char *)malloc(strlen(buffer) + 1);
    strcpy(evento.tipo, buffer);

    //lettura di un numero int
    do
    {
        printf("inserire il prezz\n");
        gets(buffer);
        if (isNumber(buffer) == 1)
            break;
    } while (1);
    evento.prez = (char *)malloc(strlen(buffer) + 1);
    strcpy(evento.prez, buffer);

    //lettura di un numero int
    do
    {
        printf("inserire il numero di biglietti\n");
        gets(buffer);
        if (isNumber(buffer) == 1)
            break;
    } while (1);
    biglietto.nBiglietti = atoi(buffer);


    return 0;
}

int isNumber(char *text)
{
	int j;
	j = strlen(text);
	while (j--)
	{
		if (text[j] >= '0' && text[j] <= '9')
			continue;

		return 0;
	}
	return 1;
}