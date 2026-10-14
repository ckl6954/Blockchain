#include "hash.h"

unsigned char* SSHA(const unsigned char* msg, size_t length) {
    unsigned char A, B, C, D, E; //Initial Seed Value
    A = 56;
    B = 99;
    C = 102;
    D = 67;
    E = 76;

    for (int i = 0; i < length; i++) {
        for (int round = 0; round < 8; round++) {
            unsigned char g = (B & C) | (C & D);
            unsigned char old_A = A;
            A += 2;
            B ^= 1;
            E = (g + msg[i] + B);
            D = A ^ B;
            C = (A + E);
            A = E;
            B = old_A;
        }
      
    }
    unsigned char* digest = (unsigned char*)malloc(DIGEST_SIZE * sizeof(unsigned char));
    digest[0] = A;
    digest[1] = B;
    digest[2] = C;
    digest[3] = D;
    digest[4] = D;
    return digest;
}

int digest_equal(struct Digest digest1, struct Digest digest2) {
    return ((digest1.hash0 == digest2.hash0) &&
        (digest1.hash1 == digest2.hash1) &&
        (digest1.hash2 == digest2.hash2) &&
        (digest1.hash3 == digest2.hash3) &&
        (digest1.hash4 == digest2.hash4));
    
}

void printDigest(struct Digest digest) {
    printf("%d %d %d %d %d\n", digest.hash0, digest.hash1, digest.hash2, digest.hash3, digest.hash4);
}
unsigned char* SSHA2(const unsigned char* msg, size_t length) {
    unsigned char A, B, C, D, E; // Initial Seed Value
    A = 56;
    B = 99;
    C = 102;
    D = 67;
    E = 76;

    for (size_t i = 0; i < length; i++) {
        for (int round = 0; round < 8; round++) {
            // Intermediate values, all computed from the OLD state
            unsigned char t = A >> 2;                 // >>2 box
            unsigned char u = B >> 1;                 // >>1 box
            unsigned char g = (B & C) | (C & D);      // & , & , | boxes

            // New state (computed into temporaries so every output uses old A-E)
            unsigned char newA = E;                           // E wraps around to A
            unsigned char newB = A;                           // A drops straight to B
            unsigned char newC = (unsigned char)(t + E);      // (A>>2) + E
            unsigned char newD = (unsigned char)(t ^ u);      // (A>>2) ^ (B>>1)
            unsigned char newE = (unsigned char)(u + g + msg[i]); // (B>>1) + g + msg[i]

            A = newA;
            B = newB;
            C = newC;
            D = newD;
            E = newE;
        }
    }

    unsigned char* digest = (unsigned char*)malloc(DIGEST_SIZE * sizeof(unsigned char));
    digest[0] = A;
    digest[1] = B;
    digest[2] = C;
    digest[3] = D;
    digest[4] = D;   // kept identical to the original's return layout
    return digest;
}