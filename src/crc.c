/*
    Plik: crc.cpp

    Plik zawiera funkcjê crc16 obliczaj¹c¹ crc 16 bitowe.
    Dodatkowo jeœli jest zdefiniowana sta³a:
        GENERATE - kompilowana jest funkcja main generuj¹ca tablicê dla szybkiego obliczania CRC
                   (polinominal)
        TEST - kompilowana jest funkcja main testuj¹ca oblicznie CRC dla polinominal x^15 + 1
*/
#include "crc.h"
//#define TEST
//#define GENERATE

#ifdef TEST
    #include <stdlib.h>
    #include <stdio.h>
#endif

#ifdef GENERATE
    #include <stdlib.h>
    #include <stdio.h>
#endif

/* CRC16 polinominal 0100001 = x^15 + 1*/
static mss_crc16 crc_table[256] = {
        0x0000U, 0x8001U, 0x8003U, 0x0002U, 0x8007U, 0x0006U, 0x0004U, 0x8005U,
        0x800fU, 0x000eU, 0x000cU, 0x800dU, 0x0008U, 0x8009U, 0x800bU, 0x000aU,
        0x801fU, 0x001eU, 0x001cU, 0x801dU, 0x0018U, 0x8019U, 0x801bU, 0x001aU,
        0x0010U, 0x8011U, 0x8013U, 0x0012U, 0x8017U, 0x0016U, 0x0014U, 0x8015U,
        0x803fU, 0x003eU, 0x003cU, 0x803dU, 0x0038U, 0x8039U, 0x803bU, 0x003aU,
        0x0030U, 0x8031U, 0x8033U, 0x0032U, 0x8037U, 0x0036U, 0x0034U, 0x8035U,
        0x0020U, 0x8021U, 0x8023U, 0x0022U, 0x8027U, 0x0026U, 0x0024U, 0x8025U,
        0x802fU, 0x002eU, 0x002cU, 0x802dU, 0x0028U, 0x8029U, 0x802bU, 0x002aU,
        0x807fU, 0x007eU, 0x007cU, 0x807dU, 0x0078U, 0x8079U, 0x807bU, 0x007aU,
        0x0070U, 0x8071U, 0x8073U, 0x0072U, 0x8077U, 0x0076U, 0x0074U, 0x8075U,
        0x0060U, 0x8061U, 0x8063U, 0x0062U, 0x8067U, 0x0066U, 0x0064U, 0x8065U,
        0x806fU, 0x006eU, 0x006cU, 0x806dU, 0x0068U, 0x8069U, 0x806bU, 0x006aU,
        0x0040U, 0x8041U, 0x8043U, 0x0042U, 0x8047U, 0x0046U, 0x0044U, 0x8045U,
        0x804fU, 0x004eU, 0x004cU, 0x804dU, 0x0048U, 0x8049U, 0x804bU, 0x004aU,
        0x805fU, 0x005eU, 0x005cU, 0x805dU, 0x0058U, 0x8059U, 0x805bU, 0x005aU,
        0x0050U, 0x8051U, 0x8053U, 0x0052U, 0x8057U, 0x0056U, 0x0054U, 0x8055U,
        0x80ffU, 0x00feU, 0x00fcU, 0x80fdU, 0x00f8U, 0x80f9U, 0x80fbU, 0x00faU,
        0x00f0U, 0x80f1U, 0x80f3U, 0x00f2U, 0x80f7U, 0x00f6U, 0x00f4U, 0x80f5U,
        0x00e0U, 0x80e1U, 0x80e3U, 0x00e2U, 0x80e7U, 0x00e6U, 0x00e4U, 0x80e5U,
        0x80efU, 0x00eeU, 0x00ecU, 0x80edU, 0x00e8U, 0x80e9U, 0x80ebU, 0x00eaU,
        0x00c0U, 0x80c1U, 0x80c3U, 0x00c2U, 0x80c7U, 0x00c6U, 0x00c4U, 0x80c5U,
        0x80cfU, 0x00ceU, 0x00ccU, 0x80cdU, 0x00c8U, 0x80c9U, 0x80cbU, 0x00caU,
        0x80dfU, 0x00deU, 0x00dcU, 0x80ddU, 0x00d8U, 0x80d9U, 0x80dbU, 0x00daU,
        0x00d0U, 0x80d1U, 0x80d3U, 0x00d2U, 0x80d7U, 0x00d6U, 0x00d4U, 0x80d5U,
        0x0080U, 0x8081U, 0x8083U, 0x0082U, 0x8087U, 0x0086U, 0x0084U, 0x8085U,
        0x808fU, 0x008eU, 0x008cU, 0x808dU, 0x0088U, 0x8089U, 0x808bU, 0x008aU,
        0x809fU, 0x009eU, 0x009cU, 0x809dU, 0x0098U, 0x8099U, 0x809bU, 0x009aU,
        0x0090U, 0x8091U, 0x8093U, 0x0092U, 0x8097U, 0x0096U, 0x0094U, 0x8095U,
        0x80bfU, 0x00beU, 0x00bcU, 0x80bdU, 0x00b8U, 0x80b9U, 0x80bbU, 0x00baU,
        0x00b0U, 0x80b1U, 0x80b3U, 0x00b2U, 0x80b7U, 0x00b6U, 0x00b4U, 0x80b5U,
        0x00a0U, 0x80a1U, 0x80a3U, 0x00a2U, 0x80a7U, 0x00a6U, 0x00a4U, 0x80a5U,
        0x80afU, 0x00aeU, 0x00acU, 0x80adU, 0x00a8U, 0x80a9U, 0x80abU, 0x00aaU
};


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
unsigned short crc16( const unsigned char * data, unsigned long size, unsigned short crc )
{
	while( size -- )
		crc = crc_table[ (crc >> 8 ^ * (data++)) & 0xffU ] ^ (crc << 8);
	return crc;
}

/****************************************************************************/
#ifdef TEST

/*
 * PURPOSE
 *	Test udf_crc()
 *
 * HISTORY
 *	July 21, 1997 - Andrew E. Mileski
 *	Adapted from OSTA-UDF(tm) 1.50 standard.
 */

unsigned char t1[] = { 0x30U, 0x30U, 0x30U, 0x30U, 0x30U, 0x30U, 0x3BU, 0x0DU };

int main(void)
{
	unsigned short x;
	x = crc16(t1, sizeof( t1 ), 0 );
	printf("udf_crc16 t1: calculated = %4.4x, correct = %4.4x\n", x, 0x0bb6U );
    return 0;
}

#endif /* defined(TEST) */

/****************************************************************************/
#ifdef GENERATE

/*
 * PURPOSE
 *	Generate a table for fast 16-bit CRC calculations (any polynomial).
 *
 * DESCRIPTION
 *	The ITU-T V.41 polynomial is 010041.
 *
 * HISTORY
 *	July 21, 1997 - Andrew E. Mileski
 *	Adapted from OSTA-UDF(tm) 1.50 standard.
 */

#include <stdio.h>

int main(int argc, char **argv)
{
	unsigned long crc, poly;
	int n, i;

	/* Get the polynomial */
//	sscanf(argv[1], "%lo", &poly);
    poly = 0x8001;
	if (poly & 0xffff0000U){
		fprintf(stderr, "polynomial is too large\n");
		exit(1);
	}

	printf("/* CRC 0%o */\n", poly);

	/* Create a table */
	printf("static unsigned short crc_table[256] = {\n");
	for (n = 0; n < 256; n++){
		if (n % 8 == 0)
			printf("\t");
		crc = n << 8;
		for (i = 0; i < 8; i++){
			if(crc & 0x8000U)
				crc = (crc << 1) ^ poly;
			else
				crc <<= 1;
		crc &= 0xFFFFU;
		}
		if (n == 255)
			printf("0x%04xU ", crc);
		else
			printf("0x%04xU, ", crc);
		if(n % 8 == 7)
			printf("\n");
	}
	printf("};\n");

	return 0;
}

#endif /* defined(GENERATE) */
