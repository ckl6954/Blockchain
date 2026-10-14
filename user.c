#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "user.h"

struct User* add(struct User * head, char* Username) {
	Sleep((rand() % 10 + 1) * 1000);
	struct User* newHead = (struct User*)malloc(sizeof(struct User));
	strcpy(newHead->Username, Username);
	time(&(newHead->loginTime));
	newHead->localLoginTime = *localtime(&(newHead->loginTime));
    newHead->next = NULL;  
    if (head == NULL) {
        newHead->hash.hash0 = 0;
        newHead->hash.hash1 = 0;
        newHead->hash.hash2 = 0;
        newHead->hash.hash3 = 0;
        newHead->hash.hash4 = 0;
        return newHead;
    }
    //this whole logic was wrong i had to redo it to what it is now. 
    struct User* tail = head;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    tail->next = newHead;
    generateDigest(&(newHead->hash), tail);  
    
    return head;
}

void printLog(struct User* head) {
	struct User* iterator = head;
	printf("********** Access Log **********\n");
	while (iterator != NULL) {
		printf("Username: %-20s\t", iterator->Username);

		printf("Last Login: %02d/%02d/%04d %02d:%02d:%02d\t",
			iterator->localLoginTime.tm_mon + 1,
			iterator->localLoginTime.tm_mday,
			iterator->localLoginTime.tm_year + 1900,
			iterator->localLoginTime.tm_hour,
			iterator->localLoginTime.tm_min,
			iterator->localLoginTime.tm_sec);

		printf("\tHash: ");
        printDigest(iterator->hash);
        iterator = iterator->next;

	}
}

void printUser(struct User* user) {
    printf("Username: %-20s\t", user->Username);
    printf("Last Login: %02d/%02d/%04d %02d:%02d:%02d\n",
        user->localLoginTime.tm_mon + 1,
        user->localLoginTime.tm_mday,
        user->localLoginTime.tm_year + 1900,
        user->localLoginTime.tm_hour,
        user->localLoginTime.tm_min,
        user->localLoginTime.tm_sec);
}




void generateDigest(struct Digest* digest, struct User* User) {
    unsigned char* result = SSHA2((unsigned char*)User, STRUCT_SIZE); // Calculating or storing a hash for the wrong node (idk if this is an issue i dont think so)
    digest->hash0 = result[0]; //shifted these by one so its correct
    digest->hash1 = result[1];
    digest->hash2 = result[2];
    digest->hash3 = result[3];
    digest->hash4 = result[4];
    free(result);
}
void verify(struct User* curr) {
    int height = 2;

    printf("******** Verifying Log *********\n\n");

    //fixed starting node
    struct User* next = curr->next;

    printf("User 1, impossible to verify\n");
    printf("\t%-20s", "User Data:");
    printUser(curr);
    printf("\t%-20s", "Saved Hash:");
    printDigest(curr->hash);
    printf("\n");

    while (next != NULL) { // fixed loop
        // unsigned char* computedHash = NULL; like what is it thinking i just gotta know

       
        struct Digest prev_digest_computed;
        generateDigest(&prev_digest_computed, curr); // Comparing a block against the right block 

        if (digest_equal(prev_digest_computed, next->hash)) {
            printf("User %d passed\n", height);
            printf("\t%-20s", "User Data:");
            printUser(next);
            printf("\t%-20s", "Saved Hash:");
            printDigest(next->hash);
            printf("\t%-20s", "Calculated Hash:");
            printDigest(prev_digest_computed);
            printf("\n\n");
        }
        else {
            printf("User %d failed\n", height);
            printf("\t%-20s", "User Data:");
            printUser(next);
            printf("\t%-20s", "Saved Hash:");
            printDigest(next->hash);
            printf("\t%-20s", "Calculated Hash:");
            printDigest(prev_digest_computed);
            printf("\n\n");
            return;
        }
        
        curr = next; // correct traversal logic that does not skip nodes or processes the wrong node
        next = curr->next;
        height++;
    }
    //had to get rid of the nothing to verify, does not make any sense

    printf("**********************************\n");
    printf("* All blocks have been verified. *\n");
    printf("**********************************\n");
}