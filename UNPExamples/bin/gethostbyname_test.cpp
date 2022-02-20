#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
extern int h_errno;

int main(int argc,char**argv){
	char* ptr;
	char** pptr;
	char str[1024];
	struct hostent *hptr;
	while(argc>1){
		ptr = *++argv;
		if((hptr = gethostbyname(ptr)) == NULL){
			printf("error occured when gethostbyname:{%s->%s}\n",ptr,hstrerror(h_errno));
			continue;
		}
		printf("official hostname:%s\n",hptr->h_name);
		for(pptr = hptr->h_aliases;*pptr != NULL;pptr++){
			printf("\talias:%s\n",*pptr);
		}
		switch (hptr->h_addrtype){
		case AF_INET:
			pptr = hptr->h_addr_list;
			for(;*pptr!=NULL;pptr++){
				printf("\taddress:%s\n",inet_ntop(hptr->h_addrtype,*pptr,str,1024));
			}
			break;
		default:
			printf("Unknown Address Type!\n");
			break;
		}
		argc--;
	}
	return 0;
}

