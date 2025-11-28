/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion. 
	(c) Ponga su nombre y numero de cuenta aqui.
	323135577 Trejo Nicolás Santiago
	
	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.
	
******************************************************************************************************************/

#include "stdafx.h"
#include <string.h>
#include "corrector.h"
//Funciones publicas del proyecto
/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario	
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/
void	Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos)


{
	//iNumElementos = 1;
	//strcpy(szPalabras[0], "AquiVaElDiccionario");
	//iEstadisticas[0] = 1; // la primer palabra aparece solo una vez.
	iNumElementos = 0;
	if (!szNombre) return;

	FILE* fp;
	fopen_s(&fp, szNombre, "r");
	if (!fp) return;

	char palabra[TAMTOKEN];
	int k = 0;
	int ch;

	while ((ch = fgetc(fp)) != EOF)
	{
		char c = (char)ch;

		if (c == ' ' || c == '\t' || c == '\n' || c == '\r' ||
			c == '.' || c == ',' || c == ';' || c == '(' || c == ')')
		{
			if (k > 0)
			{
				palabra[k] = 0;

				int pos = -1;
				for (int i = 0; i < iNumElementos; i++)
				{
					if (strcmp(szPalabras[i], palabra) == 0)
					{
						pos = i;
						break;
					}
				}

				if (pos != -1)
				{
					iEstadisticas[pos]++;
				}
				else
				{
					int i = iNumElementos - 1;
					while (i >= 0 && strcmp(palabra, szPalabras[i]) < 0)
					{
						strcpy_s(szPalabras[i + 1], TAMTOKEN, szPalabras[i]);
						iEstadisticas[i + 1] = iEstadisticas[i];
						i--;
					}

					strcpy_s(szPalabras[i + 1], TAMTOKEN, palabra);
					iEstadisticas[i + 1] = 1;
					iNumElementos++;
				}

				k = 0;
			}
		}
		else
		{
			if (c >= 'A' && c <= 'Z')
				c = c - 'A' + 'a';

			if (k < TAMTOKEN - 1)
				palabra[k++] = c;
		}
	}

	if (k > 0)
	{
		palabra[k] = 0;

		int pos = -1;
		for (int i = 0; i < iNumElementos; i++)
		{
			if (strcmp(szPalabras[i], palabra) == 0)
			{
				pos = i;
				break;
			}
		}

		if (pos != -1)
		{
			iEstadisticas[pos]++;
		}
		else
		{
			int i = iNumElementos - 1;
			while (i >= 0 && strcmp(palabra, szPalabras[i]) < 0)
			{
				strcpy_s(szPalabras[i + 1], TAMTOKEN, szPalabras[i]);
				iEstadisticas[i + 1] = iEstadisticas[i];
				i--;
			}

			strcpy_s(szPalabras[i + 1], TAMTOKEN, palabra);
			iEstadisticas[i + 1] = 1;
			iNumElementos++;
		}
	}

	fclose(fp);

}

	/*****************************************************************************************************************
		ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
		Regresa las palabras ordenadas por su peso
		char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
		int		iNumSugeridas,						//Lista de palabras clonadas
		char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
		int		iEstadisticas[],					//Lista de las frecuencias de las palabras
		int		iNumElementos,						//Numero de elementos en el diccionario
		char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
		int		iPeso[],							//Peso de las palabras en la lista final
		int &	iNumLista)							//Numero de elementos en la szListaFinal
	******************************************************************************************************************/
void ListaCandidatas(
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Numero de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista del diccionario
	int		iEstadisticas[],					//Frecuencias
	int		iNumElementos,						//Elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Salida
	int		iPeso[],							//Frecuencias finales
	int& iNumLista)							//Cantidad final

{
	int count = 0;

	for (int i = 0; i < iNumSugeridas; i++)
	{
		char* pal = szPalabrasSugeridas[i];

		for (int j = 0; j < iNumElementos; j++)
		{
			if (strcmp(pal, szPalabras[j]) == 0)
			{
				strcpy_s(szListaFinal[count], TAMTOKEN, pal);
				iPeso[count] = iEstadisticas[j];
				count++;
				break;
			}
		}
	}

	for (int i = 0; i < count - 1; i++)
		for (int j = i + 1; j < count; j++)
			if (iPeso[j] > iPeso[i])
			{
				int aux = iPeso[i];
				iPeso[i] = iPeso[j];
				iPeso[j] = aux;

				char tmp[TAMTOKEN];
				strcpy_s(tmp, szListaFinal[i]);
				strcpy_s(szListaFinal[i], szListaFinal[j]);
				strcpy_s(szListaFinal[j], tmp);
			}

	iNumLista = count;


	//Sustituya estas lineas por su código
	//strcpy(szListaFinal[0], szPalabrasSugeridas[ 0] ); //la palabra candidata
	//iPeso[0] = iEstadisticas[0];			// el peso de la palabra candidata

	//iNumLista = 1;							//Una sola palabra candidata
}

/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void	ClonaPalabras(
	char* szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int& iNumSugeridas)						//Numero de elementos en la lista
{
	iNumSugeridas = 0;
	int n = strlen(szPalabraLeida);

	char letras[] = "abcdefghijklmnopqrstuvwxyzñáéíóú";

	strcpy_s(szPalabrasSugeridas[iNumSugeridas++], TAMTOKEN, szPalabraLeida);

	for (int i = 0; i < n; i++)
	{
		char temp[TAMTOKEN];
		int k = 0;
		for (int j = 0; j < n; j++)
			if (j != i)
				temp[k++] = szPalabraLeida[j];
		temp[k] = 0;

		strcpy_s(szPalabrasSugeridas[iNumSugeridas++], TAMTOKEN, temp);
	}

	for (int i = 0; i < n - 1; i++)
	{
		char temp[TAMTOKEN];
		strcpy_s(temp, szPalabraLeida);

		char c = temp[i];
		temp[i] = temp[i + 1];
		temp[i + 1] = c;

		strcpy_s(szPalabrasSugeridas[iNumSugeridas++], TAMTOKEN, temp);
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; letras[j] != 0; j++)
		{
			char temp[TAMTOKEN];
			strcpy_s(temp, szPalabraLeida);
			temp[i] = letras[j];

			strcpy_s(szPalabrasSugeridas[iNumSugeridas++], TAMTOKEN, temp);
		}

	for (int pos = 0; pos <= n; pos++)
		for (int j = 0; letras[j] != 0; j++)
		{
			char temp[TAMTOKEN];
			int k = 0;

			for (int i = 0; i < pos; i++)
				temp[k++] = szPalabraLeida[i];

			temp[k++] = letras[j];

			for (int i = pos; i < n; i++)
				temp[k++] = szPalabraLeida[i];

			temp[k] = 0;

			strcpy_s(szPalabrasSugeridas[iNumSugeridas++], TAMTOKEN, temp);
		}

	for (int i = 0; i < iNumSugeridas - 1; i++)
		for (int j = i + 1; j < iNumSugeridas; j++)
			if (strcmp(szPalabrasSugeridas[i], szPalabrasSugeridas[j]) > 0)
			{
				char tmp[TAMTOKEN];
				strcpy_s(tmp, szPalabrasSugeridas[i]);
				strcpy_s(szPalabrasSugeridas[i], szPalabrasSugeridas[j]);
				strcpy_s(szPalabrasSugeridas[j], tmp);
			}
	//Sustituya estas lineas por su código
	//strcpy(szPalabrasSugeridas[0], szPalabraLeida); //lo que sea que se capture, es sugerencia
	//iNumSugeridas = 1;							//Una sola palabra sugerida

}