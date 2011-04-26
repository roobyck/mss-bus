/*
    Plik: crc.h

    Plik zawiera deklaracjê funkcji crc16 obliczaj¹c¹ crc 16 bitowe.
*/

#include "types.h"

/*
    Funkcja oblicza CRC 16 bitowe. Wykorzystuje tablicê crc_table (wyznaczonej dla danego polinominal).
    Jeœli chcemy obliczaæ crc w kilku krokach, mo¿emy wywo³aæ funkcjê wielokrotnie przekazyj¹c jej
    kolejne ci¹gi bajtów z których chcemy policzyæ crc oraz crc z poprzedniego kroku. Pocz¹tkowe crc 
    powinno byæ 0.

    Parametry: data - wskazanie na bufor zawieraj¹cy dane z których liczymy CRC,
               size - wielkoœæ bufora data,
               crc - crc z poprzedniego wywo³ania lub 0.
    Wynik: obliczone crc.
 */
mss_crc16 crc16( const unsigned char * data, unsigned long size, unsigned short crc = 0 );
