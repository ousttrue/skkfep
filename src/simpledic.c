#include "config.h"
#include <stdio.h>
#include <sys/types.h>
#include "skklib.h"

main() 
{
	Dictionary dic;
	char dicname[64];
	DicList dl,prevdl;
	CandList dcand,scand;

	sprintf(dicname,"%s/%s",getenv("HOME"),USER_DIC_NAME);
	dic = openSKK(dicname);
	if (dic == NULL) {
		fprintf(stderr,"Can't open user dictionary %s\n",dicname);
		exit(1);
	}

	if (openSKKserv() < 0) {
		fprintf(stderr, "Can't open SKK server\n");
		exit(1);
	}

	prevdl = NULL;
	for (dl = dic->dlist; dl != NULL; dl = dl->nextitem) {
		dcand = dl->cand;
		scand = getCandFromServer(dl->kanaword);
		dl->cand = deleteCand(dcand,scand);
		if (dl->cand == NULL) {
			if (dl == dic->dlist)
				dic->dlist = dl->nextitem;
			else
				prevdl->nextitem = dl->nextitem;
		}
		else
			prevdl = dl;
	}

	closeSKK(dic,dicname);

	return 0;
}
