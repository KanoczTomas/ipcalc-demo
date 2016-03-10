/*--------------------------------------------------------*/
/* program -> (ip"/"kratka_maska|ip " " maska)            */
/* check_ip-> (cislo"."cislo"."cislo"."cislo)             */
/* check_mask-> (cislo"."cislo"."cislo"."cislo)           */
/* este masku testovat na spravnost                       */
/* kratka_maska -> cislo od 1-32                          */
/* cislo   -> [0-9][0-9]*, ale mensie ako 256             */
/*--------------------------------------------------------*/
//28.12.2007 o 17:29 som zacal
//1.1.2008 som dokoncil kontrolu syntaxe a vkladanie do masky a IP
//zacinam robit vypis masky a IP v human forme - 17:45
//vypis hotovy a aj binarny vypis je uz ok - 19:55
//todo: pri vypise nech pise aj kratku masku, tazke dorobit z dlhej masky generovanie kratkej
//dorobeny vypis tried a urcovanej kratkej masky 2.1.2008 13:12
//todo: dorobit rozpoznavanie privatnych adries, loopback a reserved veci ako napr 0.0.0.0
//todo: dorobit moznost si vybrat rozdelenie IP na x podsieti resp X hostov
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ERROR(string) printf("CHYBA: %s",string)

int symbol; //nacitany symbol
unsigned int ip=0,maska=0; //ip a maska v 32bitovom tvare

int program(void), check_ip(void), check_mask(void);
int kratka_maska(void), skontroluj_masku(void);
void vypis(unsigned int udaj), vypis_binarne(unsigned int udaj); //vypis masky resp IP v doted notation

int program(void){
 check_ip();
 switch(symbol){
 	case '/': kratka_maska();
		  break;
	case ' ': while((symbol=getchar())==' ');//preskoc prazdne miesta
		  check_mask();
		  break;
	case '\n': ERROR("maska nie je zadana!\n");
		   exit(1);
		   break;
	default: ERROR("za IP ma ist \'/\' kratka maska alebo \' \' a maska v dlhom tvare\n");
		 exit(1);
 }
}//koniec program();

int check_ip(void){
 int bodka=0,oktet=0;

 while(bodka<3 && bodka>-1){
	if(oktet>255 || oktet<0){ printf("CHYBA: nespravny oktet -> %d! Oktet moze byt od 0 - 255!!\n",oktet);exit(1);}
 
 	switch(symbol){
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			oktet=symbol-'0';
			while((symbol=getchar())>='0' && symbol<='9')oktet=oktet*10+symbol-'0';
			if(ip==0)ip=0xffffffff; //ciste jednotky na 32 bitoch
			else ip+=255; // preklopime posledny oktet na 1tky, aby & fungoval
			ip=ip&(0xffffff00+(unsigned int)oktet);
			ip= ip << 8;
			break;
		case '.': bodka++;
			  symbol=getchar();
			  if(symbol=='.'){ERROR("pozor dve bodky za sebou!\n");exit(1);}
	 		  break;
		default: printf("CHYBA: nedovoleny znak %c\n",symbol);
			 exit(1);
			 break;
        }
 }
 //pripad ak je uz 3. bodka
 if(isdigit(symbol)){
 	oktet=symbol-'0';
	while((symbol=getchar())>='0' && symbol<='9')oktet=oktet*10+symbol-'0';
	ip+=255;
	ip=ip&(0xffffff00+(unsigned int)oktet);
 }
 if(symbol=='.')bodka++;
 if(bodka>3){ ERROR("priliz vela bodiek!\n");exit(1);}
 if(oktet>255 || oktet<0){ printf("CHYBA: nespravny oktet -> %d! Oktet moze byt od 0 - 255!!\n",oktet);exit(1);}
}//koniec check_ip()

int check_mask(void){
 int bodka=0,oktet=0;

 while(bodka<3 && bodka>-1){
	if(oktet>255 || oktet<0){ printf("CHYBA: nespravny oktet -> %d! Oktet moze byt od 0 - 255!!\n",oktet);exit(1);}
 
 	switch(symbol){
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			oktet=symbol-'0';
			while((symbol=getchar())>='0' && symbol<='9')oktet=oktet*10+symbol-'0';
			if(maska==0)maska=0xffffffff; //ciste jednotky na 32 bitoch
			else maska+=255; // preklopime posledny oktet na 1tky, aby & fungoval
			maska=maska&(0xffffff00+(unsigned int)oktet);
			maska= maska << 8;
			break;
		case '.': bodka++;
			  symbol=getchar();
			  if(symbol=='.'){ERROR("pozor dve bodky za sebou!\n");exit(1);}
	 		  break;
		default: printf("CHYBA: nedovoleny znak %c\n",symbol);
			 exit(1);
			 break;
        }
 }
 //pripad ak je uz 3. bodka
 if(isdigit(symbol)){
 	oktet=symbol-'0';
	while((symbol=getchar())>='0' && symbol<='9')oktet=oktet*10+symbol-'0';
	maska+=255;
	maska=maska&(0xffffff00+(unsigned int)oktet);
 }
 if(symbol=='.')bodka++;
 if(bodka>3){ ERROR("priliz vela bodiek!\n");exit(1);}
 if(oktet>255 || oktet<0){ printf("CHYBA: nespravny oktet -> %d! Oktet moze byt od 0 - 255!!\n",oktet);exit(1);}
 if(!skontroluj_masku()){ ERROR("nespravna maska!\n");exit(1); }

}//koniec check_mack()

int skontroluj_masku(void){
 unsigned int pomocna=0xffffffff;//pomocna premenna
 while(1){
 	if(pomocna==maska){
		return 1;
	}
	if(pomocna==0) return 0; 
 	pomocna= pomocna << 1;
 }
}//koniec skontroluj_masku()

int kratka_maska(void){
  int i=0; //pomocna premenna
  symbol=getchar();
  switch(symbol){
  	case '0':case '1':case '2':case '3':case '4':
	case '5':case '6':case '7':case '8':case '9':
		i=symbol-'0';
		while((symbol=getchar())>='0' && symbol<='9') i=i*10+symbol-'0';
		if(!isdigit(symbol) && symbol!='\n'){ printf("CHYBA: nedovoleny znak -> %c\n",symbol); exit(1);}
		break;
	default: printf("CHYBA: nedovoleny znak -> %c\n",symbol);
		 exit(1);
		 break;
  }
  if(i>32){ printf("CHYBA: maska /%d je nespravna. Moze byt od 1 - 32!\n",i);exit(1);}
  maska=0xffffffff << 32-i;
}//koniec kratka_maska()

void vypis(unsigned int udaj){
 int oktet[4],count;
 	for(count=0;count<4;count++){
		oktet[3-count]=udaj&0x000000ff;
		udaj=udaj >> 8;
	}
	for(count=0;count<3;count++){
		printf("%d.",oktet[count]);
	}
	printf("%d\n",oktet[3]);
}//koniec vypis()

void vypis_binarne(unsigned int udaj){
 char binarne_cislo[36]; //36 je 32 bitov + 3 bodky + '\0'
 unsigned int pomocna2=0;
 char pomocna=0;
 int count=0;
 while(count!=35){
 	
 	if(count==8 || count==17 ||count==26){binarne_cislo[count]='.';count++;continue;}
	pomocna2=udaj&0x80000000;
	switch(pomocna2){
		case 0: pomocna='0';break;
		case 0x80000000: pomocna='1';break;
		default: ERROR("kriticka chyba!\n");
			 exit(1);
			 break;
	}
	binarne_cislo[count]=pomocna;
	udaj=udaj<<1;
	count++;
 }
 binarne_cislo[count]='\0';
 printf("\33[34m%s\n\33[0m",binarne_cislo);
}//koniec vypis_binarne()

void urc_adresy(void){
 unsigned int ip_siete=0;

 ip_siete=ip&maska;
 printf("Siet:\t\t\t\t\t");
 vypis(ip_siete);
 printf("Prva IP:\t\t\t\t");
 vypis(ip_siete+1);
 printf("Posledna IP:\t\t\t\t");
 vypis(ip_siete+(~maska)-1);
 printf("Broadcast IP:\t\t\t\t");
 vypis(ip_siete+(~maska));
 
}//koniec urc_adresy

void urc_triedu(void){
// A -> 0xxx
// B -> 10xx
// C -> 110x
// D -> 1110
// E -> 1111
 if(ip&0x80000000){
 	if(ip&0x40000000){
		if(ip&0x20000000){
			if(ip&0x10000000) printf("Trieda:\t\t\t\t\tE=experimental\n");
			else printf("Trieda:\t\t\t\t\tD=multicast\n");
		}
		else printf("Trieda:\t\t\t\t\tC\n");
	}
	else printf("Trieda:\t\t\t\t\tB\n");
 }
 else printf("Trieda:\t\t\t\t\tA\n");

}//koniec urc_triedu()

int urc_kratku_masku(void){
 //spocitam kolko ma nul
 unsigned int pomocna=maska;
 int count=0;
 while((pomocna&0x00000001)==0){
	count++;
	pomocna=pomocna>>1;
 }
 return (32-count);

}//koniec urc_kratku_masku

int urc_pocet(void){
 unsigned int pomocna;

 pomocna=32-(unsigned int)urc_kratku_masku();
 return (pow(2,pomocna));
}//koniec urc_pocet



int main(void){
 symbol=getchar();
 program();
 printf("\n");
 printf("IP:\t\t\t\t\t");
 vypis(ip);
 printf("Maska: =/%d\t\t\t\t",urc_kratku_masku());
 vypis(maska);
 printf("\n");
 printf("IP binarne:\t\t\t\t");
 vypis_binarne(ip);
 printf("Maska binarne:\t\t\t\t");
 vypis_binarne(maska);
 printf("\t\t\t\t\t-----------------------------------\n");
 printf("Siet binarne:\t\t\t\t");
 vypis_binarne(ip&maska);
 printf("\n");
 urc_adresy();
 printf("\n");
 urc_triedu();
 printf("Pocet hostov/sieti:\t\t\t%d/%d\n",urc_pocet()-2,urc_pocet());
 printf("\nkoniec\n");
return 0;
}
