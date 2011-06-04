/*
 *  Plik: crc.h
 *  Plik zawiera deklaracjê funkcji crc16() obliczaj±c± CRC 16-bitowe.
 */

#include "types.h"

/*
 *  Funkcja oblicza CRC 16-bitowe. Wykorzystuje tablicê crc_table (wyznaczonej
 *  dla danego polinominal). Je¶li chcemy obliczaæ crc w kilku krokach,
 *  mo¿emy wywo³aæ funkcjê wielokrotnie przekazuj±c jej kolejne ci±gi bajtów
 *  z których chcemy policzyæ CRC oraz CRC z poprzedniego kroku. Pocz±tkowe
 *  CRC powinno byæ 0.
 *
 *  Parametry:
 *    data - wskazanie na bufor zawieraj±cy dane z których liczymy CRC,
 *    size - wielko¶æ bufora data,
 *    crc  - CRC z poprzedniego wywo³ania lub 0.
 *  Wynik:
 *    Obliczone CRC.
 */
mss_crc16 crc16 (const unsigned char * data, unsigned long size,
                 unsigned short crc = 0);

