#pragma once
#include <Windows.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <malloc.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libsslMT.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "libcryptoMT.lib")

using namespace std;

string public_key;
string private_key;

std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

int b64invs[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
    59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
    6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
    43, 44, 45, 46, 47, 48, 49, 50, 51 };

void CreateRSAKey()
{
    //private_key = OBFUSCATE("-----BEGIN RSA PRIVATE KEY-----\nMIICXAIBAAKBgQCjmsZ9yXY8AmgJoZQfLZFkiefr81FwRzHMa38EO9eIOS7vXXIo\ngYzK5mBlXRP8YRAjReoQ1JyGf8qKeQdZV52ln2ALNU5MXpNnC/Aow3Z5gnEqw/i9\n8TuVVehhNKB7bxsJyaRPdwsaAZwmPQOXeyEIEYkgEZ03xl8ttaE5/k1SywIDAQAB\nAoGAE4bPlgfLktH+tNeSJD5+Ee/bvG+oEqyMpwdVe53g1aNtlKgkje0YbsccTjzT\nu9Wh5XPcwfF+VRc/h1N2qxUn/YGtx+MnY59v46Npb+p44zlJA/VCSeN+6LI2CMzg\n6Km4vrP6Wwr5pYcygrT4a2YUyz6ereZEG4o45SHcvbERnaECQQDTLkD0McvZgLRn\n7i3PyJhNjLnRTGVIW66HubBfAAb5JeP0gMpC9U44vwSHms/qKCzkF0XXyEmINjYp\n43pK0pPDAkEAxlOlLOizWjcObvPt3RIJt+2hPuv/cxvadoMpcNVb1BpxrEJh4P+W\n/8fLyAcKgwYI82MsV76/TL4ZCOXjYYz8WQJAZE+CNvw4UjZae0udoycrgRqpYQui\nyH4UJxBDAL0oksw2vXs3mBqhbz78zdTxkc5HlIihel/+3Z6yl68f9c6UtQJBALdk\nfabrf336z1P2KIx2s5nrGWIP5OXSac+RNe7qvMGDun2p3MysTDs1y9CqAiRqnjhC\nCq0Q6+DRIGFEA2Efd/ECQFjX5cycyYkOPS6WrTVSP9W8SC6Q7BxdnGUw6OPx0QXb\n4mSqF5qzL/1+aX/4VuvxTgqXyaqKJQQ1t5sL6kMHDXE=\n-----END RSA PRIVATE KEY-----");
    //public_key = OBFUSCATE("-----BEGIN PUBLIC KEY-----\nMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCZafUTg6W4380/rB5TBqudXdnP\ns4H5YZ/NLXgXb9Y6B1nR05t+D6VfI6OpIwPrjTz/ssecg+Ljed6bVGOlM3XhPLTr\nkxCZwgg94tNqKxmFLENDSNl/hpl+H+KbRL7B0IHhUnIpnScOzXvlYIs6OgrbexBQ\nv6apCNC196a6SZ6k8QIDAQAB\n-----END PUBLIC KEY-----");

    //private_key = "-----BEGIN RSA PRIVATE KEY-----\nMIICXAIBAAKBgQCjmsZ9yXY8AmgJoZQfLZFkiefr81FwRzHMa38EO9eIOS7vXXIo\ngYzK5mBlXRP8YRAjReoQ1JyGf8qKeQdZV52ln2ALNU5MXpNnC/Aow3Z5gnEqw/i9\n8TuVVehhNKB7bxsJyaRPdwsaAZwmPQOXeyEIEYkgEZ03xl8ttaE5/k1SywIDAQAB\nAoGAE4bPlgfLktH+tNeSJD5+Ee/bvG+oEqyMpwdVe53g1aNtlKgkje0YbsccTjzT\nu9Wh5XPcwfF+VRc/h1N2qxUn/YGtx+MnY59v46Npb+p44zlJA/VCSeN+6LI2CMzg\n6Km4vrP6Wwr5pYcygrT4a2YUyz6ereZEG4o45SHcvbERnaECQQDTLkD0McvZgLRn\n7i3PyJhNjLnRTGVIW66HubBfAAb5JeP0gMpC9U44vwSHms/qKCzkF0XXyEmINjYp\n43pK0pPDAkEAxlOlLOizWjcObvPt3RIJt+2hPuv/cxvadoMpcNVb1BpxrEJh4P+W\n/8fLyAcKgwYI82MsV76/TL4ZCOXjYYz8WQJAZE+CNvw4UjZae0udoycrgRqpYQui\nyH4UJxBDAL0oksw2vXs3mBqhbz78zdTxkc5HlIihel/+3Z6yl68f9c6UtQJBALdk\nfabrf336z1P2KIx2s5nrGWIP5OXSac+RNe7qvMGDun2p3MysTDs1y9CqAiRqnjhC\nCq0Q6+DRIGFEA2Efd/ECQFjX5cycyYkOPS6WrTVSP9W8SC6Q7BxdnGUw6OPx0QXb\n4mSqF5qzL/1+aX/4VuvxTgqXyaqKJQQ1t5sL6kMHDXE=\n-----END RSA PRIVATE KEY-----";
    //public_key = "-----BEGIN PUBLIC KEY-----\nMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCZafUTg6W4380/rB5TBqudXdnP\ns4H5YZ/NLXgXb9Y6B1nR05t+D6VfI6OpIwPrjTz/ssecg+Ljed6bVGOlM3XhPLTr\nkxCZwgg94tNqKxmFLENDSNl/hpl+H+KbRL7B0IHhUnIpnScOzXvlYIs6OgrbexBQ\nv6apCNC196a6SZ6k8QIDAQAB\n-----END PUBLIC KEY-----";

    unsigned char ss[] =
    {

        0xc, 0x8c, 0x8c, 0x93, 0x79, 0xf2, 0x56, 0xd2,
        0x1, 0x2f, 0xa7, 0x2e, 0x9a, 0x41, 0xef, 0x41,
        0x16, 0xe, 0x56, 0x9e, 0x4, 0x4d, 0xc9, 0x94,
        0x2d, 0x59, 0xf, 0x5a, 0x97, 0x28, 0xa9, 0x4b,
        0xf1, 0x5d, 0x78, 0x87, 0xdb, 0x69, 0xbc, 0x53,
        0xb5, 0xf5, 0x3e, 0x45, 0x3f, 0xda, 0xc7, 0x15,
        0x5a, 0x55, 0xda, 0xa2, 0x3d, 0xd0, 0xca, 0x5,
        0xb5, 0x30, 0x20, 0x66, 0xfc, 0xd8, 0xbe, 0xb2,
        0xed, 0x58, 0xeb, 0x80, 0x9e, 0x29, 0x84, 0x9b,
        0xd5, 0xe, 0x7d, 0xc9, 0x5c, 0x81, 0xb2, 0x27,
        0xd4, 0xb3, 0x3f, 0x7e, 0xed, 0x46, 0xb2, 0xeb,
        0xc0, 0x2a, 0x81, 0xa4, 0x94, 0xfe, 0x6a, 0xee,
        0x4b, 0x18, 0xe7, 0xfd, 0x33, 0xaa, 0x83, 0xb4,
        0xc9, 0xd7, 0x5, 0xd5, 0xef, 0x53, 0x40, 0xe3,
        0x73, 0x78, 0x72, 0x18, 0xf0, 0x1a, 0x77, 0x19,
        0xc6, 0x5c, 0xa, 0x88, 0xf0, 0x55, 0x9c, 0x3b,
        0x28, 0x81, 0xa4, 0x76, 0xc1, 0xf4, 0xd2, 0x60,
        0x8c, 0x9c, 0x7a, 0x46, 0xf4, 0xda, 0xdb, 0x34,
        0x39, 0xc8, 0x76, 0xd6, 0x35, 0x71, 0x74, 0xc0,
        0xd1, 0x72, 0x11, 0x8b, 0xb5, 0x35, 0x89, 0x65,
        0xb1, 0xfc, 0xa, 0x54, 0x91, 0xa0, 0x41, 0x21,
        0x13, 0x20, 0x6, 0x22, 0x95, 0x39, 0xd0, 0x2a,
        0x10, 0x38, 0xdd, 0xa, 0x89, 0xcf, 0xf, 0x20,
        0xe9, 0x90, 0xe7, 0x73, 0xf6, 0x7b, 0x89, 0x93,
        0xca, 0x8d, 0x52, 0x6e, 0x87, 0x4d, 0x5d, 0x27,
        0xe7, 0x8, 0xfa, 0x8a, 0x47, 0x2b, 0x78, 0xb5,
        0xc7, 0xb1, 0x76, 0x11, 0xc1, 0x17, 0x11, 0xbc,
        0x99, 0xc5, 0xdc, 0x60, 0xc6, 0x3, 0xf, 0x24,
        0xf3, 0xbd, 0x89, 0x3a, 0x39, 0xcd, 0x38, 0xbb,
        0x8e, 0x57, 0xcc, 0xb7, 0x2f, 0x5a, 0x32, 0xca,
        0x12, 0x8d, 0x23, 0xba, 0x51, 0x2d, 0x40, 0x94,
        0xf9, 0xb2, 0x55, 0xc1, 0xd5, 0xe7, 0xfb, 0x50,
        0x48, 0x53, 0x73, 0xec, 0xca, 0x7f, 0x4e, 0x36,
        0x7f, 0x9a, 0xc4, 0xfd, 0xf, 0xcb, 0xd1, 0xf,
        0xdd, 0x48, 0x7b, 0x75, 0xa5, 0x5e, 0x0, 0xb0,
        0x7b, 0xc8, 0x48, 0x24, 0xa7, 0x8b, 0x50, 0x50,
        0x31, 0xd1, 0x60, 0xb, 0x5d, 0xd4, 0xd0, 0x75,
        0xd9, 0x9c, 0x14, 0xab, 0xb9, 0x30, 0xa6, 0x5b,
        0xdd, 0x6f, 0xdc, 0xd0, 0x3b, 0x1a, 0x6e, 0xe8,
        0xe0, 0xe1, 0x6, 0xa1, 0x10, 0x5c, 0xf9, 0x41,
        0x1c, 0x9d, 0xf4, 0x4d, 0x94, 0x8, 0x37, 0x54,
        0x64, 0x1c, 0x4a, 0xa0, 0x89, 0x3f, 0x57, 0x6d,
        0x23, 0xff, 0x85, 0x5c, 0xf0, 0x17, 0xfa, 0x56,
        0xe7, 0x22, 0xd5, 0xc9, 0xd5, 0x53, 0x6a, 0x40,
        0x17, 0x8, 0x58, 0xc9, 0x19, 0x65, 0xd0, 0x48,
        0x8c, 0x1d, 0x78, 0xd5, 0x41, 0xfb, 0x68, 0x2c,
        0x41, 0x4c, 0x5f, 0xa2, 0x5f, 0xba, 0xee, 0x58,
        0xb6, 0x75, 0x64, 0x0, 0x65, 0xea, 0xa6, 0xcd,
        0xa1, 0xfe, 0x3d, 0xd0, 0x49, 0x9e, 0xe5, 0xe6,
        0xa5, 0x21, 0xc4, 0x9f, 0xd8, 0x6f, 0x2e, 0x6c,
        0x8e, 0xe9, 0x8a, 0xec, 0x1d, 0x90, 0xc9, 0x4f,
        0x7e, 0xe0, 0x82, 0x27, 0xec, 0xfc, 0xeb, 0xec,
        0xcb, 0x84, 0x61, 0x83, 0x73, 0xf1, 0x4e, 0x5f,
        0x1e, 0x63, 0xd6, 0x15, 0x76, 0xcb, 0xe1, 0xc5,
        0x70, 0xdf, 0xd5, 0x91, 0xec, 0x52, 0x89, 0x24,
        0xe0, 0xa4, 0x1c, 0xfe, 0xb3, 0xd9, 0xd, 0x88,
        0xb7, 0x10, 0x5e, 0xfe, 0x4e, 0x65, 0x17, 0x3b,
        0x11, 0xa9, 0x91, 0x9, 0x38, 0x50, 0x2f, 0xfd,
        0xce, 0xfe, 0xbe, 0xe2, 0x30, 0x9e, 0x53, 0xe0,
        0x44, 0x6a, 0xe, 0x82, 0x56, 0xf0, 0xc5, 0xb3,
        0xeb, 0x3e, 0x47, 0x5c, 0xfd, 0xa3, 0x73, 0x8c,
        0x65, 0x17, 0xe3, 0xc0, 0x7b, 0x27, 0x4f, 0x48,
        0x66, 0xb2, 0xa2, 0x47, 0xa2, 0x30, 0xd3, 0x7f,
        0x44, 0x1d, 0x17, 0xe0, 0xe2, 0x86, 0xee, 0x4b,
        0xe3, 0xfe, 0x9f, 0xea, 0xb4, 0x42, 0x48, 0x50,
        0x88, 0x1a, 0xc3, 0x9b, 0x6d, 0xd, 0x7e, 0x13,
        0x9e, 0x93, 0xd6, 0x1f, 0x9c, 0xc9, 0x3b, 0xda,
        0x31, 0xbb, 0xa8, 0xe6, 0xbf, 0x40, 0x75, 0x67,
        0x89, 0x5a, 0x83, 0xe6, 0xcc, 0x93, 0xd0, 0xb5,
        0xe2, 0x54, 0x74, 0x86, 0x8c, 0x8a, 0xef, 0x3a,
        0xb4, 0xd3, 0x18, 0x1b, 0x1e, 0x16, 0x1d, 0xe3,
        0xd9, 0x1b, 0x8f, 0x1, 0x70, 0x1e, 0x9f, 0xe9,
        0xf, 0x7d, 0xb6, 0x66, 0x94, 0x2d, 0x7a, 0x7c,
        0x37, 0xcb, 0xb2, 0xf1, 0xab, 0xeb, 0x1b, 0xcc,
        0xc6, 0xaa, 0x8d, 0xb0, 0x10, 0x23, 0x61, 0xf1,
        0x8b, 0x59, 0xc6, 0x40, 0xa2, 0x80, 0x71, 0xb4,
        0xb0, 0xe, 0x5b, 0x1d, 0x67, 0x8d, 0xaf, 0xd1,
        0x5b, 0x1a, 0x7f, 0xe2, 0xb3, 0xdd, 0xe6, 0xf2,
        0x9c, 0xc2, 0x9a, 0xae, 0x6d, 0xa3, 0x14, 0x16,
        0xe0, 0xe6, 0xe4, 0x67, 0xa3, 0x18, 0x6d, 0x99,
        0xe6, 0x11, 0x24, 0x3d, 0x2e, 0xd3, 0x2d, 0x57,
        0x3e, 0xdf, 0xee, 0x46, 0x1c, 0x35, 0x22, 0x7d,
        0x5, 0x89, 0xc0, 0x36, 0x54, 0xff, 0x17, 0x48,
        0x36, 0x27, 0xdc, 0x87, 0xfe, 0xe7, 0xeb, 0xf3,
        0x1b, 0x8b, 0x5e, 0x85, 0x82, 0xd3, 0x85, 0x55,
        0x1, 0xb9, 0xac, 0x5e, 0xc4, 0xe3, 0x43, 0xfd,
        0x16, 0x15, 0x4, 0x2c, 0xd8, 0x6a, 0xf0, 0x3c,
        0xc1, 0xf2, 0xdf, 0xb8, 0x22, 0x65, 0x8b, 0xdb,
        0xef, 0x99, 0xf1, 0xa3, 0xd6, 0x7d, 0x3, 0x94,
        0x24, 0x7a, 0x3c, 0x71, 0x1d, 0x8f, 0x2f, 0x70,
        0x70, 0xe7, 0xb5, 0x40, 0xe4, 0xb7, 0x64, 0x6e,
        0x9f, 0xd8, 0x9b, 0x32, 0xb5, 0xfd, 0xc5, 0x3,
        0xf6, 0xfe, 0x7c, 0xd3, 0x57, 0x82, 0x6f, 0x1f,
        0x53, 0x28, 0x4b, 0xc, 0x42, 0x83, 0xf0, 0x63,
        0x48, 0x2e, 0xbc, 0xf9, 0x82, 0x1f, 0xde, 0xba,
        0xd9, 0x61, 0xb0, 0xbd, 0xbe, 0x16, 0x45, 0xd6,
        0xc6, 0x39, 0xc4, 0xc, 0x24, 0x7f, 0xd0, 0xc4,
        0x1a, 0xda, 0x62, 0x9b, 0x9a, 0x47, 0x98, 0x39,
        0x8d, 0xc, 0x8c, 0xe1, 0x10, 0x6c, 0xe0, 0xf,
        0x73, 0xb6, 0x3c, 0x59, 0xbf, 0xe5, 0x3d, 0x13,
        0x94, 0x21, 0x97, 0xc8, 0xc3, 0xda, 0x6d, 0x5b,
        0x41, 0x9c, 0x65, 0x78, 0xe1, 0x93, 0x94, 0x6b,
        0x28, 0x63, 0x39, 0xb3, 0xf7, 0x8, 0x29, 0x36,
        0x91, 0x98, 0xc1, 0x63, 0xc3, 0xc9, 0xd1, 0xc8,
        0xf5, 0xa6, 0x8, 0xff, 0x9c, 0x24, 0xc5, 0x51,
        0x1b, 0xcb, 0xec, 0x60, 0xf8, 0x65, 0xb7, 0xdd,
        0x74, 0xb0, 0x7c, 0x3e, 0x4b, 0x69, 0x46, 0x9,
        0xed, 0x7, 0xfd, 0x7a, 0xcc, 0x85, 0x26, 0xd2,
        0xb3, 0xbb, 0x6b, 0x40, 0x41, 0x6d, 0xab, 0x12,
        0x3e, 0x12, 0x9, 0x58, 0xbc, 0x2b, 0x4d, 0x36,
        0x68, 0xb5, 0x64, 0xeb, 0x37, 0x7e, 0x53
    };

    for (unsigned int m = 0; m < sizeof(ss); ++m)
    {
        unsigned char c = ss[m];
        c ^= 0xc9;
        c = -c;
        c ^= 0x38;
        c = -c;
        c ^= 0x45;
        c += 0x41;
        c = ~c;
        c -= m;
        c = (c >> 0x5) | (c << 0x3);
        c += m;
        c ^= m;
        c += 0x69;
        c = (c >> 0x7) | (c << 0x1);
        c += m;
        c = (c >> 0x2) | (c << 0x6);
        c = ~c;
        c ^= 0x28;
        c += 0x48;
        c ^= m;
        c = (c >> 0x2) | (c << 0x6);
        c = ~c;
        c -= 0x5c;
        c = (c >> 0x7) | (c << 0x1);
        c += m;
        c = ~c;
        c -= m;
        c ^= 0x2d;
        c += 0x5a;
        c = -c;
        c += 0x89;
        c = (c >> 0x5) | (c << 0x3);
        c ^= 0xc9;
        c = (c >> 0x3) | (c << 0x5);
        c += 0xa6;
        c = ~c;
        c -= m;
        c = ~c;
        c -= 0x33;
        c = -c;
        c ^= 0x85;
        c = (c >> 0x5) | (c << 0x3);
        c ^= 0xde;
        c = ~c;
        c ^= 0xff;
        c = -c;
        c = (c >> 0x6) | (c << 0x2);
        c += 0xd3;
        c ^= m;
        c = (c >> 0x5) | (c << 0x3);
        c -= 0xec;
        c ^= m;
        c = (c >> 0x2) | (c << 0x6);
        c -= 0x3d;
        c = ~c;
        c -= m;
        c ^= m;
        c = -c;
        c -= m;
        c ^= m;
        c += m;
        c = ~c;
        c -= m;
        c = (c >> 0x3) | (c << 0x5);
        c -= m;
        c ^= m;
        c -= 0x30;
        c = (c >> 0x5) | (c << 0x3);
        c ^= m;
        c += 0xb3;
        c = ~c;
        c += 0xd6;
        c = -c;
        c = (c >> 0x2) | (c << 0x6);
        c = ~c;
        c -= 0x1;
        c ^= m;
        c += m;
        c = (c >> 0x2) | (c << 0x6);
        c -= m;
        c = -c;
        c = ~c;
        c = (c >> 0x6) | (c << 0x2);
        c ^= 0x8d;
        c -= m;
        c = -c;
        c = (c >> 0x7) | (c << 0x1);
        c = -c;
        c -= m;
        c ^= 0x14;
        c += m;
        c = (c >> 0x3) | (c << 0x5);
        c -= m;
        c ^= 0x6;
        c += m;
        c = (c >> 0x1) | (c << 0x7);
        c = ~c;
        c = -c;
        c ^= m;
        c = (c >> 0x7) | (c << 0x1);
        c += 0x41;
        c = -c;
        c -= 0x28;
        c = ~c;
        c -= m;
        c = -c;
        c -= 0x56;
        c = -c;
        c = ~c;
        c += m;
        c = (c >> 0x3) | (c << 0x5);
        c ^= 0x9c;
        c = -c;
        c -= m;
        c ^= 0x3b;
        c -= 0xfa;
        c ^= 0xd9;
        c = -c;
        c += m;
        c ^= 0x3b;
        c = -c;
        c -= 0x6a;
        c ^= 0xe1;
        c = -c;
        c ^= m;
        c += 0x92;
        c = (c >> 0x5) | (c << 0x3);
        c ^= m;
        c = (c >> 0x6) | (c << 0x2);
        c += m;
        c ^= 0x59;
        c = ~c;
        c += 0xed;
        c ^= m;
        c = -c;
        c = ~c;
        c ^= m;
        c = ~c;
        c -= 0x27;
        c = (c >> 0x2) | (c << 0x6);
        c ^= 0x5b;
        c = -c;
        c += 0xce;
        c ^= 0xee;
        c = ~c;
        c ^= m;
        c = ~c;
        c = -c;
        c ^= 0xa9;
        c -= 0x47;
        c = (c >> 0x7) | (c << 0x1);
        c ^= m;
        c -= m;
        c = ~c;
        c -= m;
        c = -c;
        c += 0x4;
        c = (c >> 0x3) | (c << 0x5);
        c -= 0x37;
        c = (c >> 0x6) | (c << 0x2);
        c += m;
        c = -c;
        c += 0xff;
        c ^= m;
        c = (c >> 0x5) | (c << 0x3);
        c -= 0xc8;
        c = (c >> 0x5) | (c << 0x3);
        c ^= 0x21;
        c = ~c;
        c -= m;
        c = -c;
        c = ~c;
        c ^= m;
        c = ~c;
        c -= 0x9e;
        c = ~c;
        c += m;
        c ^= m;
        c += m;
        c ^= m;
        c -= m;
        c = ~c;
        c = -c;
        c ^= 0xdd;
        c -= m;
        c = -c;
        c -= 0x6a;
        c ^= m;
        c -= m;
        c ^= m;
        c = (c >> 0x7) | (c << 0x1);
        c += 0xdb;
        c = (c >> 0x5) | (c << 0x3);
        c ^= 0xc4;
        c = -c;
        c += 0xa5;
        c ^= 0x5c;
        c -= 0x51;
        c ^= 0x31;
        c = ~c;
        c += 0xe6;
        c = (c >> 0x6) | (c << 0x2);
        c += m;
        c ^= 0x7f;
        c -= 0xf2;
        c = -c;
        c ^= m;
        c += m;
        c ^= m;
        c = ~c;
        c = -c;
        c += 0xa9;
        c = ~c;
        c += 0x29;
        c ^= m;
        c += m;
        c = -c;
        c ^= 0x6d;
        c -= m;
        c = -c;
        c -= m;
        c ^= 0xa4;
        c = (c >> 0x2) | (c << 0x6);
        c -= 0x95;
        c = ~c;
        c = (c >> 0x7) | (c << 0x1);
        c ^= m;
        c += m;
        c = -c;
        c ^= 0xb5;
        c += 0xf1;
        c = (c >> 0x1) | (c << 0x7);
        c = ~c;
        c += 0x20;
        c ^= 0xb8;
        c -= 0x33;
        c ^= m;
        c = ~c;
        c ^= 0x36;
        c += m;
        c ^= m;
        c = -c;
        c ^= m;
        c += m;
        c ^= m;
        c += 0x70;
        c ^= 0xc9;
        c = (c >> 0x2) | (c << 0x6);
        c ^= 0x6c;
        c = -c;
        c = ~c;
        c ^= m;
        c = ~c;
        c ^= m;
        c = -c;
        c ^= m;
        c += 0xad;
        c = (c >> 0x5) | (c << 0x3);
        c += m;
        c ^= 0x11;
        c = ~c;
        c += m;
        c = ~c;
        c = (c >> 0x2) | (c << 0x6);
        c += 0xa9;
        c ^= m;
        c = (c >> 0x3) | (c << 0x5);
        c = ~c;
        c += m;
        c ^= 0x89;
        c -= 0xf6;
        c = -c;
        c -= 0x40;
        c ^= m;
        c = (c >> 0x3) | (c << 0x5);
        c -= 0x79;
        c = -c;
        c = ~c;
        c = (c >> 0x7) | (c << 0x1);
        c ^= 0x4f;
        c -= 0x3;
        c = ~c;
        c ^= 0xf1;
        c -= m;
        c = ~c;
        c ^= m;
        c = ~c;
        c = (c >> 0x3) | (c << 0x5);
        c = ~c;
        c ^= m;
        c = (c >> 0x6) | (c << 0x2);
        c ^= m;
        c = -c;
        c += m;
        c = -c;
        c ^= m;
        c += m;
        c ^= m;
        c = (c >> 0x3) | (c << 0x5);
        c = ~c;
        c += m;
        c ^= m;
        c -= m;
        c = (c >> 0x1) | (c << 0x7);
        c -= 0x87;
        c ^= m;
        c += 0x44;
        c = ~c;
        c ^= m;
        c = -c;
        c = ~c;
        c -= 0x49;
        c = ~c;
        c ^= 0x84;
        c += 0x9b;
        c ^= m;
        c += m;
        c = ~c;
        c ^= m;
        c += m;
        c = ~c;
        c ^= m;
        c += 0x4b;
        c ^= 0x8e;
        c = -c;
        c ^= 0xcb;
        c += m;
        c = ~c;
        c ^= 0x85;
        c = ~c;
        c ^= m;
        c = ~c;
        c -= 0xaa;
        c ^= m;
        c += 0x99;
        c ^= 0xbc;
        c += m;
        c ^= m;
        c = -c;
        c -= 0xa6;
        c ^= 0x21;
        c -= m;
        c = ~c;
        c -= m;
        c = (c >> 0x6) | (c << 0x2);
        c = -c;
        c -= 0x52;
        c = -c;
        c += 0xf5;
        c = (c >> 0x2) | (c << 0x6);
        c ^= m;
        c += 0xe4;
        c = -c;
        c ^= 0x2;
        c = ~c;
        c ^= 0xc6;
        c -= 0xe6;
        c = (c >> 0x2) | (c << 0x6);
        c = -c;
        c ^= m;
        c += 0x8c;
        c = (c >> 0x2) | (c << 0x6);
        c += m;
        c ^= 0xac;
        c -= m;
        c = -c;
        c -= m;
        c ^= m;
        c = ~c;
        c ^= m;
        c += 0xc9;
        c = ~c;
        c = (c >> 0x1) | (c << 0x7);
        c = -c;
        c = ~c;
        c = (c >> 0x3) | (c << 0x5);
        c += 0x65;
        c ^= 0x45;
        c = -c;
        c -= m;
        c ^= 0x70;
        c += 0xc;
        c ^= m;
        c += m;
        c ^= m;
        c += 0xd9;
        c ^= 0x8f;
        c += m;
        c = (c >> 0x6) | (c << 0x2);
        c += 0xc4;
        c ^= 0xd8;
        c += m;
        c ^= 0x37;
        c -= 0x11;
        c = -c;
        c -= m;
        c = (c >> 0x7) | (c << 0x1);
        c += 0xde;
        c ^= m;
        c = (c >> 0x1) | (c << 0x7);
        c -= m;
        c ^= m;
        c -= m;
        c ^= m;
        c += 0x7b;
        c ^= m;
        c = (c >> 0x5) | (c << 0x3);
        c ^= 0xe4;
        c += m;
        c ^= m;
        c += 0xd9;
        c = (c >> 0x1) | (c << 0x7);
        c ^= m;
        c = -c;
        c -= 0x58;
        c ^= 0xe4;
        c = ~c;
        c += 0x76;
        c = (c >> 0x2) | (c << 0x6);
        c += 0x6e;
        c = -c;
        c -= 0x78;
        c = (c >> 0x1) | (c << 0x7);
        c += m;
        c = (c >> 0x1) | (c << 0x7);
        c = ~c;
        c += m;
        c = ~c;
        c -= 0xb1;
        c ^= 0x67;
        c -= 0x25;
        c = (c >> 0x3) | (c << 0x5);
        c += 0xb1;
        c ^= m;
        c -= m;
        c = ~c;
        c += m;
        c = (c >> 0x1) | (c << 0x7);
        c -= 0x57;
        c ^= 0xc9;
        c = ~c;
        c -= m;
        c ^= 0xe;
        c = (c >> 0x5) | (c << 0x3);
        c = ~c;
        c = (c >> 0x6) | (c << 0x2);
        c = ~c;
        c ^= m;
        c = ~c;
        c = (c >> 0x7) | (c << 0x1);
        c = -c;
        c -= m;
        c = ~c;
        c -= m;
        c ^= 0x82;
        c = (c >> 0x5) | (c << 0x3);
        c ^= m;
        c += m;
        c ^= 0x6a;
        c -= 0x8f;
        c = -c;
        c ^= 0xc3;
        c -= 0xd4;
        c = (c >> 0x6) | (c << 0x2);
        c += 0x7c;
        c = -c;
        c += m;
        c ^= 0xa0;
        c -= 0xf4;
        c ^= 0xf9;
        c -= m;
        c = ~c;
        c -= m;
        c = ~c;
        c = -c;
        c -= 0x1e;
        c = (c >> 0x3) | (c << 0x5);
        c = ~c;
        c -= m;
        c ^= 0xcc;
        c += 0xa5;
        c ^= 0x83;
        c -= 0xbe;
        c = ~c;
        c -= m;
        c = -c;
        c = ~c;
        c = (c >> 0x7) | (c << 0x1);
        c -= m;
        c = (c >> 0x1) | (c << 0x7);
        c ^= 0x75;
        c = ~c;
        c = -c;
        c += m;
        c ^= 0xd3;
        c = ~c;
        c = -c;
        c += m;
        c ^= m;
        c += m;
        c ^= m;
        c = (c >> 0x7) | (c << 0x1);
        c ^= 0xd7;
        c -= m;
        c = ~c;
        c ^= m;
        c += 0x53;
        c = -c;
        c -= 0xb1;
        c ^= 0x59;
        c += 0x9;
        c = (c >> 0x1) | (c << 0x7);
        c = -c;
        c ^= 0xf7;
        c -= 0xd;
        c ^= 0xdd;
        c = ~c;
        c = -c;
        c += 0x9c;
        c ^= 0x38;
        c += m;
        c = -c;
        c = (c >> 0x6) | (c << 0x2);
        c ^= 0x9d;
        c = (c >> 0x5) | (c << 0x3);
        c += m;
        c = -c;
        c = (c >> 0x3) | (c << 0x5);
        c ^= m;
        c = ~c;
        c -= 0xf;
        c ^= 0xbf;
        c = -c;
        c ^= 0xb4;
        c = ~c;
        c += 0xd6;
        c ^= 0x17;
        c -= 0xe1;
        c ^= m;
        c += m;
        c ^= m;
        c = -c;
        c += 0x11;
        c = ~c;
        c += 0xff;
        c = -c;
        c -= 0x8f;
        c = ~c;
        c += m;
        c ^= 0x9e;
        c = (c >> 0x2) | (c << 0x6);
        c ^= m;
        c -= m;
        c = ~c;
        c -= m;
        c = -c;
        c = (c >> 0x2) | (c << 0x6);
        c -= m;
        c ^= m;
        c = ~c;
        c ^= 0xdd;
        c = (c >> 0x1) | (c << 0x7);
        c += m;
        c = (c >> 0x3) | (c << 0x5);
        c += m;
        c ^= m;
        c = -c;
        c -= m;
        c ^= m;
        c -= m;
        c ^= m;
        c -= 0x1a;
        c = -c;
        c = ~c;
        c = -c;
        c ^= 0x44;
        c -= 0xdc;
        c = ~c;
        c ^= 0xfd;
        c -= 0x68;
        c ^= 0xe5;
        c = (c >> 0x6) | (c << 0x2);
        c ^= 0x26;
        c = (c >> 0x7) | (c << 0x1);
        c = -c;
        c += 0xf6;
        c ^= 0xa3;
        c -= m;
        c = (c >> 0x6) | (c << 0x2);
        c = ~c;
        c += m;
        c = ~c;
        c += m;
        c = ~c;
        c = -c;
        c ^= 0xa6;
        c = ~c;
        c -= m;
        c = ~c;
        c -= 0x9e;
        c = -c;
        c ^= m;
        c = -c;
        c -= 0xb9;
        c = (c >> 0x1) | (c << 0x7);
        c += m;
        c = (c >> 0x6) | (c << 0x2);
        c ^= 0xc9;
        c = -c;
        c += 0x81;
        c = ~c;
        c = -c;
        c ^= 0x8c;
        c -= 0x2f;
        c = -c;
        c = (c >> 0x7) | (c << 0x1);
        c -= 0xe0;
        c ^= m;
        c -= 0xc0;
        c = -c;
        c += m;
        c ^= 0xba;
        c -= m;
        c ^= m;
        c += 0xc7;
        c = -c;
        c = (c >> 0x7) | (c << 0x1);
        c ^= m;
        c = (c >> 0x3) | (c << 0x5);
        c -= 0x2f;
        c = (c >> 0x1) | (c << 0x7);
        c += 0x88;
        c = -c;
        c += m;
        c ^= 0x16;
        c = (c >> 0x1) | (c << 0x7);
        c += m;
        c = -c;
        c ^= 0xde;
        c -= m;
        c ^= m;
        c -= 0x72;
        c = (c >> 0x7) | (c << 0x1);
        c = ~c;
        c = -c;
        c += 0x6f;
        c ^= m;
        c = (c >> 0x1) | (c << 0x7);
        c += 0x81;
        c ^= 0xf4;
        c -= m;
        c = (c >> 0x2) | (c << 0x6);
        c ^= m;
        c = -c;
        c = (c >> 0x6) | (c << 0x2);
        c ^= 0xf0;
        c += 0x5b;
        c = -c;
        c ^= 0xe7;
        c -= 0xc2;
        c = (c >> 0x6) | (c << 0x2);
        c ^= 0xc9;
        c -= 0xe7;
        c ^= 0xb2;
        c += m;
        c ^= m;
        c -= 0x25;
        c = ~c;
        c += m;
        c = (c >> 0x6) | (c << 0x2);
        c ^= m;
        c = (c >> 0x3) | (c << 0x5);
        c ^= m;
        c -= m;
        c ^= m;
        c -= 0xaa;
        c = (c >> 0x7) | (c << 0x1);
        c -= m;
        c = -c;
        c ^= 0x93;
        c = (c >> 0x6) | (c << 0x2);
        c -= m;
        c ^= m;
        c -= 0x92;
        c = (c >> 0x5) | (c << 0x3);
        c += 0xa1;
        c ^= m;
        c -= 0x1e;
        c ^= m;
        c += m;
        c = ~c;
        c += m;
        c ^= 0x2;
        c -= 0x7d;
        c ^= m;
        c -= 0xf2;
        c ^= m;
        c = (c >> 0x6) | (c << 0x2);
        c ^= m;
        c += m;
        c ^= 0x39;
        c -= m;
        c = (c >> 0x5) | (c << 0x3);
        c = -c;
        c -= 0x8a;
        c = (c >> 0x5) | (c << 0x3);
        c -= 0xdd;
        c = -c;
        c = ~c;
        c += 0x70;
        c = -c;
        c -= 0x93;
        c ^= m;
        c -= 0xc4;
        c ^= 0xb;
        c -= 0xf3;
        c = -c;
        c ^= 0xa4;
        c = ~c;
        c = -c;
        c += m;
        c = (c >> 0x1) | (c << 0x7);
        c += 0x74;
        c = -c;
        c += 0xfb;
        c = ~c;
        c = -c;
        c += 0x27;
        c ^= 0xb;
        c = ~c;
        c -= 0x99;
        c ^= m;
        c += m;
        c ^= m;
        c = (c >> 0x1) | (c << 0x7);
        c ^= 0xf2;
        c = (c >> 0x6) | (c << 0x2);
        c ^= 0xea;
        c = -c;
        c ^= 0x7e;
        c -= m;
        c = (c >> 0x7) | (c << 0x1);
        c -= m;
        c ^= m;
        c -= m;
        c ^= 0x38;
        c -= 0x3b;
        c ^= m;
        c += m;
        c ^= 0x6c;
        c = (c >> 0x7) | (c << 0x1);
        c ^= 0xbf;
        c += 0x47;
        c ^= m;
        c += m;
        c = -c;
        c = (c >> 0x7) | (c << 0x1);
        c -= m;
        c = -c;
        c -= m;
        c = -c;
        c += m;
        c = -c;
        c -= m;
        c = ~c;
        c -= 0x96;
        c ^= m;
        c = (c >> 0x3) | (c << 0x5);
        c -= m;
        c ^= m;
        c = (c >> 0x7) | (c << 0x1);
        c ^= m;
        c = -c;
        c ^= m;
        c -= 0xbc;
        c = ~c;
        c = (c >> 0x6) | (c << 0x2);
        c ^= 0xe3;
        c -= 0x10;
        c ^= 0xbe;
        c = (c >> 0x7) | (c << 0x1);
        c -= 0xfc;
        c ^= m;
        c -= 0xbb;
        c = -c;
        c += 0x8d;
        c = -c;
        c = ~c;
        c ^= m;
        c -= 0x32;
        c = ~c;
        c = (c >> 0x2) | (c << 0x6);
        c += m;
        c = ~c;
        c ^= 0xb8;
        c = (c >> 0x5) | (c << 0x3);
        c -= 0x2e;
        c ^= 0xa2;
        c -= m;
        c = -c;
        c ^= m;
        c += 0x48;
        c = (c >> 0x2) | (c << 0x6);
        c = ~c;
        c ^= 0x43;
        c -= 0xb1;
        c = -c;
        c ^= m;
        c += m;
        c = -c;
        c += 0xea;
        c = (c >> 0x1) | (c << 0x7);
        c += m;
        c = -c;
        c ^= m;
        c += m;
        c ^= 0x57;
        c -= 0xfd;
        c = -c;
        c -= 0x8e;
        c = (c >> 0x6) | (c << 0x2);
        c ^= m;
        c -= 0x7b;
        c = -c;
        c = (c >> 0x3) | (c << 0x5);
        c ^= 0x51;
        c -= m;
        c ^= 0x27;
        c = (c >> 0x7) | (c << 0x1);
        c ^= 0x3;
        c = (c >> 0x1) | (c << 0x7);
        c -= m;
        c = (c >> 0x5) | (c << 0x3);
        c += 0x73;
        c = -c;
        c = ~c;
        c = (c >> 0x1) | (c << 0x7);
        c = -c;
        c ^= 0xa7;
        c -= 0x58;
        c = (c >> 0x3) | (c << 0x5);
        c -= 0xf7;
        c ^= 0x9e;
        c += 0x1b;
        c = ~c;
        c += 0x7a;
        c = (c >> 0x6) | (c << 0x2);
        c += m;
        c ^= 0xc8;
        c = -c;
        c = ~c;
        c ^= 0x1c;
        c = -c;
        c ^= m;
        c -= m;
        c ^= 0xda;
        c -= m;
        c = (c >> 0x5) | (c << 0x3);
        c -= 0x51;
        c = (c >> 0x3) | (c << 0x5);
        c ^= m;
        c -= 0xb0;
        c = -c;
        c -= 0x1d;
        c = ~c;
        c += 0x8e;
        c = -c;
        c = (c >> 0x6) | (c << 0x2);
        c ^= m;
        c = (c >> 0x6) | (c << 0x2);
        c -= m;
        c ^= m;
        c = -c;
        c = (c >> 0x2) | (c << 0x6);
        c = -c;
        c = ~c;
        c ^= 0x2f;
        c = (c >> 0x1) | (c << 0x7);
        c += m;
        c = (c >> 0x7) | (c << 0x1);
        c += m;
        c = ~c;
        c += 0xed;
        c = (c >> 0x6) | (c << 0x2);
        c += m;
        c = ~c;
        c += 0xaa;
        c = -c;
        c ^= 0xb8;
        c = -c;
        c = ~c;
        c += 0x5b;
        c ^= m;
        c = (c >> 0x6) | (c << 0x2);
        c -= m;
        c ^= m;
        c = (c >> 0x2) | (c << 0x6);
        c ^= m;
        c += 0x43;
        c = ~c;
        c ^= m;
        c += 0xd2;
        c = (c >> 0x2) | (c << 0x6);
        c -= m;
        c ^= m;
        c -= m;
        c ^= m;
        c += 0x6a;
        c ^= m;
        c += 0x8b;
        c = -c;
        c -= m;
        c ^= 0xe;
        c = -c;
        c ^= 0x5;
        c += 0x50;
        c = ~c;
        c = -c;
        c += 0x94;
        c ^= 0xa4;
        c += m;
        c ^= 0xfd;
        c -= m;
        c = -c;
        c += m;
        c ^= 0x9;
        c = ~c;
        c -= m;
        c = ~c;
        c -= 0xd;
        c = -c;
        c -= 0x48;
        c = -c;
        c = ~c;
        c -= m;
        c = ~c;
        c ^= 0x49;
        c -= m;
        c ^= m;
        c = -c;
        c ^= m;
        c -= m;
        c = (c >> 0x2) | (c << 0x6);
        c += 0x69;
        c ^= 0xc3;
        c -= m;
        c = -c;
        c -= m;
        c = -c;
        c = (c >> 0x3) | (c << 0x5);
        c += 0xf3;
        c = ~c;
        c += 0xbb;
        c ^= 0x9c;
        c -= 0xb9;
        c ^= 0xf8;
        c -= m;
        c ^= m;
        c = -c;
        c += m;
        c = (c >> 0x7) | (c << 0x1);
        c += 0x46;
        c ^= m;
        c = -c;
        c ^= 0x7b;
        c = ~c;
        c ^= m;
        c -= 0x20;
        c ^= m;
        c -= m;
        c = -c;
        c ^= 0xa9;
        c -= 0x4c;
        c ^= 0x2a;
        c += m;
        c ^= m;
        c -= m;
        c = (c >> 0x7) | (c << 0x1);
        c -= m;
        c = (c >> 0x1) | (c << 0x7);
        c -= m;
        c ^= m;
        c += m;
        c = -c;
        c -= 0x31;
        c ^= 0xd2;
        c = ~c;
        c += m;
        c ^= m;
        c -= m;
        c = (c >> 0x3) | (c << 0x5);
        c ^= m;
        c += m;
        c = (c >> 0x6) | (c << 0x2);
        c += 0x84;
        c ^= 0x32;
        c = -c;
        c += 0x47;
        c = (c >> 0x3) | (c << 0x5);
        c = ~c;
        c -= m;
        ss[m] = c;
    }

    private_key = string((char*)ss);

    unsigned char s[] =
    {

        0xf6, 0x3a, 0x8a, 0x99, 0xcb, 0x2e, 0xe6, 0xc9,
        0x21, 0x29, 0x2e, 0x33, 0xa8, 0x1, 0xac, 0x4b,
        0xe6, 0x27, 0x3b, 0xce, 0x8e, 0x71, 0x7f, 0xd1,
        0xcf, 0xbd, 0xc7, 0xbe, 0x4b, 0x41, 0x56, 0x3f,
        0x2c, 0xae, 0xe2, 0xf0, 0xe8, 0xe0, 0x3c, 0xcc,
        0x28, 0xa, 0xf4, 0x98, 0xb8, 0xf5, 0x7f, 0x11,
        0xd2, 0xd4, 0x48, 0xff, 0xd4, 0xbc, 0x3a, 0xaf,
        0x0, 0xca, 0x5c, 0x89, 0x9, 0x32, 0xb5, 0x2c,
        0x67, 0x4, 0xd4, 0xf, 0x24, 0x3f, 0x71, 0x6f,
        0xd8, 0x18, 0x90, 0xf1, 0xfd, 0x21, 0xf7, 0x8b,
        0x5a, 0x47, 0x1b, 0xbf, 0x9a, 0xf, 0x52, 0xa8,
        0x7b, 0x90, 0x6c, 0xfa, 0xc9, 0xd9, 0x8, 0xe0,
        0x6f, 0x57, 0x4a, 0xd1, 0xe4, 0x27, 0x4c, 0x94,
        0x87, 0xb, 0x7a, 0x88, 0x9b, 0x93, 0x38, 0xa0,
        0x8f, 0x7c, 0x15, 0x9f, 0xf1, 0xf6, 0x14, 0xef,
        0x3f, 0xb6, 0xfd, 0xd6, 0x23, 0x20, 0x7e, 0x43,
        0xd5, 0x9f, 0x3, 0xf6, 0x76, 0x3a, 0x2c, 0xe1,
        0xd3, 0x4, 0x42, 0x69, 0x16, 0xae, 0xa, 0x3e,
        0x9c, 0xab, 0xd, 0x4d, 0xa1, 0xa1, 0x81, 0x9f,
        0xe4, 0xe3, 0xc, 0xb3, 0xc9, 0x8f, 0x51, 0x79,
        0xe5, 0xff, 0x1f, 0xb, 0xaf, 0x87, 0xb3, 0x5d,
        0xcc, 0x1f, 0xae, 0xa5, 0x7, 0x9d, 0x95, 0xa0,
        0x6c, 0xd8, 0x6c, 0x15, 0x17, 0x2e, 0x40, 0xca,
        0x5b, 0xbb, 0x3f, 0x8, 0xae, 0xbb, 0x5b, 0xbf,
        0x5, 0x22, 0xb2, 0x33, 0xff, 0xe7, 0xff, 0x2,
        0x72, 0xe7, 0x9d, 0x56, 0x35, 0xc, 0x2e, 0x73,
        0x51, 0x22, 0x41, 0x2c, 0xca, 0x8a, 0xa2, 0x5e,
        0x86, 0x61, 0x9b, 0x93, 0xa7, 0x7d, 0xbd, 0xbb,
        0x70, 0x9, 0x57, 0xb, 0xc6, 0xc, 0xc6, 0xab,
        0x3d, 0x8c, 0x71, 0x9f, 0x55, 0x31, 0x58, 0x92,
        0x66, 0x52, 0xc3, 0xb2, 0x73, 0x76, 0x61, 0x2f,
        0x7b, 0xf6, 0x1c, 0xd0, 0x99, 0x71, 0xd8, 0xd,
        0x6b, 0x48, 0x4e, 0x5, 0x1e, 0xe2, 0x6, 0xda,
        0x87, 0x33, 0x8, 0x2a, 0x20, 0xdf, 0x41, 0x3e
    };

    for (unsigned int m = 0; m < sizeof(s); ++m)
    {
        unsigned char c = s[m];
        c ^= m;
        c += 0x2b;
        c ^= m;
        c += 0x14;
        c ^= 0x2f;
        c = ~c;
        c ^= m;
        c = (c >> 0x6) | (c << 0x2);
        c ^= 0x4c;
        c = (c >> 0x1) | (c << 0x7);
        c = -c;
        c ^= m;
        c -= 0xeb;
        c = -c;
        c ^= m;
        c += 0xa4;
        c = -c;
        c ^= m;
        c += m;
        c ^= m;
        c += m;
        c ^= 0xee;
        c -= m;
        c = (c >> 0x3) | (c << 0x5);
        c -= 0x70;
        c = ~c;
        c ^= 0x1d;
        c -= m;
        c = (c >> 0x6) | (c << 0x2);
        c += 0x10;
        c = (c >> 0x2) | (c << 0x6);
        c -= 0xfa;
        c ^= m;
        c += 0x27;
        c = (c >> 0x6) | (c << 0x2);
        c ^= 0xe3;
        c = -c;
        c ^= m;
        c -= 0xe6;
        c = (c >> 0x6) | (c << 0x2);
        c += m;
        c ^= m;
        c -= m;
        c = ~c;
        c ^= m;
        c -= 0x65;
        c = ~c;
        c = -c;
        c ^= 0x3d;
        c -= 0xe4;
        c = ~c;
        c += m;
        c ^= m;
        c += m;
        c ^= 0xef;
        c = ~c;
        c -= 0xa1;
        c = -c;
        c = (c >> 0x3) | (c << 0x5);
        c = ~c;
        c += 0x1d;
        c = ~c;
        c -= m;
        c ^= m;
        c = -c;
        c += m;
        c ^= 0x4f;
        c += m;
        c = (c >> 0x1) | (c << 0x7);
        c = ~c;
        c ^= 0xb3;
        c += 0x68;
        c ^= m;
        c += 0xc3;
        c ^= 0x40;
        c += m;
        c = (c >> 0x5) | (c << 0x3);
        c = -c;
        c ^= 0xf6;
        c -= m;
        c = -c;
        c += 0x6d;
        c = (c >> 0x7) | (c << 0x1);
        c = ~c;
        c ^= 0xcc;
        c -= 0x4c;
        c = (c >> 0x2) | (c << 0x6);
        c ^= m;
        c = -c;
        c -= 0xa5;
        c = -c;
        c = (c >> 0x1) | (c << 0x7);
        c -= 0x71;
        c = ~c;
        c ^= 0x40;
        c -= 0x31;
        c ^= m;
        c = (c >> 0x7) | (c << 0x1);
        c += 0x30;
        c = ~c;
        c -= m;
        c = (c >> 0x2) | (c << 0x6);
        c ^= m;
        c = ~c;
        c += 0x97;
        c = ~c;
        c = (c >> 0x7) | (c << 0x1);
        c += m;
        c = (c >> 0x2) | (c << 0x6);
        c = -c;
        c = ~c;
        c ^= m;
        c -= 0x3e;
        c = -c;
        c ^= m;
        c += m;
        c = (c >> 0x1) | (c << 0x7);
        c += m;
        c ^= 0x35;
        c = ~c;
        c += m;
        c = (c >> 0x3) | (c << 0x5);
        c ^= m;
        c += m;
        c = -c;
        c -= m;
        c = ~c;
        c -= 0x79;
        c ^= m;
        c += 0xab;
        c ^= 0x8e;
        c = (c >> 0x7) | (c << 0x1);
        c ^= 0x13;
        c += 0xe4;
        c = (c >> 0x1) | (c << 0x7);
        c ^= m;
        c += 0x94;
        c = (c >> 0x6) | (c << 0x2);
        c = -c;
        c ^= m;
        c -= 0x7b;
        c ^= 0x42;
        c += m;
        c = -c;
        c = (c >> 0x7) | (c << 0x1);
        c = ~c;
        c = (c >> 0x7) | (c << 0x1);
        c -= 0x87;
        c ^= m;
        c = (c >> 0x5) | (c << 0x3);
        c ^= 0x20;
        c += m;
        c ^= 0x11;
        c -= m;
        c = (c >> 0x2) | (c << 0x6);
        c ^= 0x9b;
        c = -c;
        c -= 0x12;
        c = -c;
        c -= m;
        c ^= m;
        c += 0x13;
        c = ~c;
        c = (c >> 0x3) | (c << 0x5);
        c ^= 0xa;
        c -= 0x89;
        c = ~c;
        c -= 0x7f;
        c ^= m;
        c = (c >> 0x3) | (c << 0x5);
        c = -c;
        c += 0x5f;
        c ^= 0x7c;
        c -= m;
        c = ~c;
        c -= m;
        c ^= m;
        c = -c;
        c = (c >> 0x3) | (c << 0x5);
        c -= m;
        c = ~c;
        c += m;
        c ^= 0xe8;
        c -= m;
        c = ~c;
        c = -c;
        c = (c >> 0x3) | (c << 0x5);
        c ^= 0xdd;
        c += m;
        c = -c;
        c -= 0xf;
        c ^= m;
        c = -c;
        c += m;
        c = ~c;
        c = -c;
        c -= 0x37;
        c = -c;
        c += m;
        c = -c;
        c ^= 0xec;
        c = (c >> 0x5) | (c << 0x3);
        c ^= m;
        c += m;
        c = -c;
        c += 0xa1;
        c = ~c;
        c = -c;
        c -= m;
        c ^= 0xf5;
        c = (c >> 0x2) | (c << 0x6);
        c -= m;
        c = -c;
        c = ~c;
        c = -c;
        c -= m;
        c = ~c;
        c ^= 0xee;
        c = ~c;
        c ^= 0xa6;
        c += m;
        c ^= 0x3a;
        c = ~c;
        c += m;
        c = -c;
        c -= 0xe5;
        c = -c;
        c = ~c;
        c += 0x55;
        c = ~c;
        c += 0x67;
        c = -c;
        c = ~c;
        c ^= 0x3f;
        c = ~c;
        c += 0x14;
        c = (c >> 0x5) | (c << 0x3);
        c = -c;
        c -= m;
        c = -c;
        c = ~c;
        c ^= m;
        c -= m;
        c = ~c;
        c = (c >> 0x1) | (c << 0x7);
        c -= 0x32;
        c ^= m;
        c = (c >> 0x7) | (c << 0x1);
        c = -c;
        c += m;
        c = -c;
        c ^= m;
        c = ~c;
        c ^= m;
        c -= 0x2b;
        c ^= m;
        c = (c >> 0x3) | (c << 0x5);
        c -= m;
        c = (c >> 0x6) | (c << 0x2);
        c = ~c;
        c += 0x86;
        c = -c;
        c += 0xec;
        c = -c;
        c -= m;
        c ^= m;
        c -= m;
        c = -c;
        c += m;
        c = (c >> 0x3) | (c << 0x5);
        c -= m;
        c = -c;
        c -= m;
        c = -c;
        c ^= 0xea;
        c = ~c;
        c = -c;
        c = ~c;
        c += 0x18;
        c ^= m;
        c -= 0x66;
        c = -c;
        c = (c >> 0x6) | (c << 0x2);
        c ^= m;
        c -= 0x5a;
        c = (c >> 0x1) | (c << 0x7);
        c ^= m;
        c = -c;
        c = ~c;
        c -= 0x47;
        c = ~c;
        c ^= 0xf0;
        c -= m;
        c ^= m;
        c = (c >> 0x1) | (c << 0x7);
        c -= 0x6e;
        c = (c >> 0x7) | (c << 0x1);
        c = ~c;
        c = (c >> 0x3) | (c << 0x5);
        c = ~c;
        c = -c;
        c += m;
        c = (c >> 0x5) | (c << 0x3);
        c += m;
        c ^= m;
        c += m;
        c ^= 0x20;
        c += m;
        c = ~c;
        c += m;
        c ^= 0xfb;
        c -= 0x22;
        c = ~c;
        c += 0x73;
        c = ~c;
        c -= m;
        c = -c;
        c ^= m;
        c = -c;
        c = ~c;
        c -= 0x57;
        c = ~c;
        c += 0x7c;
        c ^= 0x4d;
        c -= 0xdf;
        c ^= 0xdb;
        c += 0x6;
        c = -c;
        c ^= m;
        c = ~c;
        c -= 0xc4;
        c ^= m;
        c -= m;
        c ^= 0xa;
        c += m;
        c = ~c;
        c += m;
        c = ~c;
        c += m;
        c ^= 0x53;
        c = -c;
        c -= m;
        c = -c;
        c ^= 0x8c;
        c += 0xf;
        c = ~c;
        c ^= 0x81;
        c -= m;
        c = -c;
        c += 0x77;
        c = (c >> 0x6) | (c << 0x2);
        c = -c;
        c ^= m;
        c = (c >> 0x2) | (c << 0x6);
        c += 0x8b;
        c = ~c;
        c += 0xa6;
        c = ~c;
        c -= 0xb9;
        c ^= m;
        c -= 0xe4;
        c ^= 0xd3;
        c -= m;
        c = (c >> 0x7) | (c << 0x1);
        c = ~c;
        c += m;
        c ^= m;
        c = ~c;
        c ^= 0x1d;
        c += m;
        c ^= 0x70;
        c -= m;
        c = (c >> 0x3) | (c << 0x5);
        c = ~c;
        c += 0x3;
        c = (c >> 0x3) | (c << 0x5);
        c ^= m;
        c = (c >> 0x1) | (c << 0x7);
        c ^= m;
        c = (c >> 0x3) | (c << 0x5);
        c -= m;
        c ^= m;
        c += 0x56;
        c ^= m;
        c = ~c;
        c -= 0xfb;
        c ^= m;
        c = -c;
        c ^= m;
        c = (c >> 0x3) | (c << 0x5);
        c = -c;
        c -= m;
        c ^= m;
        c += 0xda;
        c = (c >> 0x5) | (c << 0x3);
        c -= m;
        c = ~c;
        c -= 0x2;
        c = ~c;
        c -= m;
        c ^= 0xc8;
        c += m;
        c = ~c;
        c ^= 0xe9;
        c -= 0x21;
        c = ~c;
        c += 0x51;
        c = ~c;
        c = (c >> 0x2) | (c << 0x6);
        c ^= m;
        c -= 0xb9;
        c ^= 0xf;
        c -= 0xd3;
        c = -c;
        c -= 0x45;
        c ^= 0xfd;
        c += 0xde;
        c = (c >> 0x1) | (c << 0x7);
        c = -c;
        c = ~c;
        c += m;
        c ^= 0x6b;
        c -= m;
        c ^= m;
        c -= m;
        c = -c;
        c += m;
        c ^= m;
        c += m;
        c ^= m;
        c = (c >> 0x1) | (c << 0x7);
        c += m;
        c ^= 0xe6;
        c = ~c;
        c += m;
        c = -c;
        c -= m;
        c = ~c;
        c ^= 0xce;
        c += 0x56;
        c ^= m;
        c += 0x97;
        c = -c;
        c -= m;
        c = ~c;
        c += 0x6;
        c = -c;
        c = ~c;
        c -= m;
        c = -c;
        c -= 0x42;
        c = -c;
        c ^= 0xad;
        c = ~c;
        c ^= m;
        c = (c >> 0x1) | (c << 0x7);
        c ^= 0x68;
        c += m;
        c = -c;
        c = (c >> 0x2) | (c << 0x6);
        c = -c;
        c = ~c;
        c = -c;
        c ^= m;
        c = -c;
        c ^= m;
        c = -c;
        c -= m;
        c ^= 0xd5;
        c -= m;
        c ^= 0xcf;
        c = (c >> 0x5) | (c << 0x3);
        c -= 0x12;
        c ^= m;
        c = -c;
        c += m;
        c = ~c;
        c = -c;
        c += 0x12;
        c = (c >> 0x6) | (c << 0x2);
        c -= 0x7a;
        c ^= m;
        c = ~c;
        c = -c;
        c -= m;
        c ^= 0xa2;
        c -= m;
        c = ~c;
        c ^= m;
        c = ~c;
        c ^= 0x14;
        c += 0xa5;
        c = ~c;
        c ^= 0xb2;
        c = (c >> 0x1) | (c << 0x7);
        c -= m;
        c = ~c;
        c = -c;
        c += m;
        c = -c;
        c += m;
        c = (c >> 0x1) | (c << 0x7);
        c = ~c;
        c += 0x3d;
        c = -c;
        c -= m;
        c = ~c;
        c = -c;
        c = (c >> 0x3) | (c << 0x5);
        c -= m;
        c = -c;
        c ^= 0xcd;
        c -= m;
        c = -c;
        c ^= 0x45;
        c += m;
        c = (c >> 0x7) | (c << 0x1);
        c = -c;
        c += m;
        c = ~c;
        c ^= 0xcf;
        c -= 0xfc;
        c = (c >> 0x1) | (c << 0x7);
        c -= 0xac;
        c ^= 0x43;
        c = -c;
        c += m;
        c = ~c;
        c -= m;
        c = (c >> 0x6) | (c << 0x2);
        c += m;
        c = ~c;
        c += 0xdb;
        c = (c >> 0x1) | (c << 0x7);
        c ^= 0x71;
        c = (c >> 0x7) | (c << 0x1);
        c -= 0x26;
        c = -c;
        c = (c >> 0x3) | (c << 0x5);
        c += 0x7e;
        c = ~c;
        c -= 0xc9;
        c = -c;
        c = (c >> 0x1) | (c << 0x7);
        c -= m;
        c ^= m;
        c -= 0x48;
        c = -c;
        c = (c >> 0x7) | (c << 0x1);
        c += 0xc7;
        c = ~c;
        c = (c >> 0x2) | (c << 0x6);
        c ^= 0x19;
        c = (c >> 0x2) | (c << 0x6);
        c = -c;
        c += 0x73;
        c ^= m;
        c -= m;
        c ^= 0x8a;
        c = ~c;
        c = (c >> 0x5) | (c << 0x3);
        c += 0xfc;
        c ^= m;
        c += 0x55;
        c ^= m;
        c = ~c;
        c += 0xbd;
        c = (c >> 0x3) | (c << 0x5);
        c -= 0x36;
        c ^= m;
        c = ~c;
        c ^= m;
        c -= m;
        c = (c >> 0x7) | (c << 0x1);
        c -= m;
        c ^= m;
        c = (c >> 0x5) | (c << 0x3);
        c += m;
        c = (c >> 0x6) | (c << 0x2);
        c -= 0x74;
        c = -c;
        c = ~c;
        c += 0x1;
        c ^= 0xbf;
        c = -c;
        c -= m;
        c ^= 0xbb;
        c = (c >> 0x5) | (c << 0x3);
        c ^= 0x47;
        c += m;
        c = ~c;
        c ^= 0xab;
        c += m;
        c = (c >> 0x1) | (c << 0x7);
        c = -c;
        c += 0x32;
        c ^= m;
        c = ~c;
        c -= m;
        c ^= m;
        c = -c;
        c ^= m;
        c -= 0xac;
        c = (c >> 0x3) | (c << 0x5);
        c += 0x6d;
        c = (c >> 0x6) | (c << 0x2);
        c = -c;
        c = (c >> 0x2) | (c << 0x6);
        c -= 0x1e;
        c = (c >> 0x1) | (c << 0x7);
        c = ~c;
        c += 0x8b;
        c ^= m;
        c += 0xb5;
        c = -c;
        c += 0xb0;
        c = (c >> 0x6) | (c << 0x2);
        c = -c;
        c ^= m;
        c += m;
        c = -c;
        c += 0x2f;
        c ^= 0x32;
        c += m;
        c = -c;
        c = (c >> 0x2) | (c << 0x6);
        c += 0xca;
        c ^= m;
        c += m;
        c = ~c;
        c ^= 0x52;
        c = -c;
        c ^= 0x26;
        c += m;
        c = (c >> 0x5) | (c << 0x3);
        c ^= 0x22;
        c -= m;
        c ^= m;
        c += m;
        c = ~c;
        c = (c >> 0x5) | (c << 0x3);
        c -= m;
        c = (c >> 0x7) | (c << 0x1);
        c += m;
        c ^= m;
        c -= m;
        c = -c;
        c -= m;
        c ^= m;
        c += 0x64;
        c = ~c;
        c += m;
        c ^= m;
        c += m;
        c = ~c;
        c += m;
        c = -c;
        c -= m;
        c ^= m;
        c += 0x43;
        c = ~c;
        c = -c;
        c ^= 0xdd;
        c -= m;
        c = (c >> 0x6) | (c << 0x2);
        c -= 0x33;
        c = (c >> 0x6) | (c << 0x2);
        c = ~c;
        c = -c;
        c -= 0x57;
        c = ~c;
        c -= 0x3;
        c = (c >> 0x2) | (c << 0x6);
        c += 0x38;
        c = (c >> 0x2) | (c << 0x6);
        c ^= m;
        c += 0x8d;
        c ^= 0x56;
        c += m;
        c = (c >> 0x1) | (c << 0x7);
        c ^= 0x4a;
        c = (c >> 0x1) | (c << 0x7);
        c ^= 0xb5;
        c -= m;
        c = -c;
        c += 0x8c;
        c = ~c;
        c = -c;
        c ^= m;
        c -= 0x2a;
        c = ~c;
        c -= m;
        c = -c;
        c -= 0xc;
        c = ~c;
        c += m;
        c = (c >> 0x1) | (c << 0x7);
        c -= m;
        c = -c;
        c += 0xdc;
        c = ~c;
        c ^= m;
        c = (c >> 0x1) | (c << 0x7);
        c ^= m;
        c = -c;
        c = ~c;
        c += m;
        c = (c >> 0x2) | (c << 0x6);
        c = ~c;
        c -= 0x26;
        c ^= m;
        c = ~c;
        c -= 0xbf;
        c = -c;
        c -= 0x89;
        c ^= 0xf0;
        c = (c >> 0x1) | (c << 0x7);
        c -= m;
        c = (c >> 0x1) | (c << 0x7);
        c -= 0xe0;
        c = -c;
        c -= m;
        c = (c >> 0x5) | (c << 0x3);
        c = ~c;
        c ^= 0x47;
        c = ~c;
        c ^= 0x43;
        c = ~c;
        c += 0xe9;
        c = ~c;
        c = -c;
        c -= m;
        c = ~c;
        c -= 0x5e;
        c = ~c;
        c -= 0xd3;
        c = ~c;
        c += m;
        c ^= m;
        c -= m;
        c = -c;
        c ^= m;
        c -= m;
        c = -c;
        c += m;
        c = (c >> 0x5) | (c << 0x3);
        c += m;
        c = ~c;
        c += m;
        c ^= m;
        c += m;
        c = (c >> 0x5) | (c << 0x3);
        c = ~c;
        c = -c;
        c += m;
        c = ~c;
        c -= 0x6e;
        c = (c >> 0x2) | (c << 0x6);
        c -= m;
        c = (c >> 0x1) | (c << 0x7);
        c = -c;
        c = (c >> 0x3) | (c << 0x5);
        c ^= 0x5b;
        c -= m;
        c = ~c;
        c -= m;
        c = -c;
        c += m;
        c ^= m;
        c = ~c;
        c ^= m;
        c = (c >> 0x2) | (c << 0x6);
        c += 0x89;
        c = -c;
        c = (c >> 0x1) | (c << 0x7);
        c += m;
        c = -c;
        c -= m;
        c = (c >> 0x6) | (c << 0x2);
        c ^= m;
        c -= m;
        c = -c;
        c -= m;
        c = -c;
        c = ~c;
        c += 0xb8;
        c = (c >> 0x2) | (c << 0x6);
        c = ~c;
        c -= m;
        c ^= 0x5;
        c += m;
        c ^= m;
        c -= 0x90;
        c ^= 0x96;
        c -= 0xbb;
        c = ~c;
        c -= m;
        c = ~c;
        c = -c;
        c -= 0x94;
        c = ~c;
        c -= 0xe4;
        c = ~c;
        c += 0x31;
        c = (c >> 0x7) | (c << 0x1);
        c -= m;
        c = -c;
        c ^= m;
        c -= 0xbf;
        c = (c >> 0x5) | (c << 0x3);
        c ^= m;
        c += 0x3f;
        c = ~c;
        c = (c >> 0x6) | (c << 0x2);
        c -= 0xd5;
        c ^= 0xca;
        c -= m;
        c = -c;
        c -= m;
        c ^= m;
        c = (c >> 0x5) | (c << 0x3);
        c = ~c;
        c -= m;
        c ^= m;
        c += 0xfe;
        c ^= 0x1;
        c = ~c;
        c += 0x9e;
        c ^= m;
        c = -c;
        c += 0x16;
        c = -c;
        c = ~c;
        c ^= 0x87;
        c -= 0xdf;
        c ^= m;
        c += 0xba;
        c = (c >> 0x1) | (c << 0x7);
        c += m;
        c = ~c;
        c -= 0xe2;
        c ^= m;
        c = (c >> 0x6) | (c << 0x2);
        c = ~c;
        c ^= m;
        c = -c;
        c += m;
        c ^= 0xeb;
        c = (c >> 0x3) | (c << 0x5);
        c += 0x33;
        c ^= 0x91;
        c = -c;
        c += m;
        c ^= m;
        c += m;
        c ^= 0xe;
        c += m;
        c ^= 0x88;
        c -= 0xc8;
        c ^= 0x56;
        c -= m;
        c = -c;
        c = (c >> 0x6) | (c << 0x2);
        c -= m;
        c = -c;
        c = (c >> 0x1) | (c << 0x7);
        c = -c;
        c += 0x5d;
        c = ~c;
        c ^= 0xe1;
        c -= 0x6a;
        c ^= 0x3e;
        c += m;
        c ^= 0x64;
        c = -c;
        c ^= m;
        c = (c >> 0x7) | (c << 0x1);
        c ^= 0x10;
        c = ~c;
        c += m;
        c = ~c;
        c ^= m;
        c = (c >> 0x5) | (c << 0x3);
        c -= 0xf6;
        c = (c >> 0x1) | (c << 0x7);
        c += 0xed;
        c ^= m;
        c += m;
        c = -c;
        c = (c >> 0x2) | (c << 0x6);
        c ^= 0x41;
        c = -c;
        c -= m;
        c = -c;
        c += 0xac;
        c = (c >> 0x7) | (c << 0x1);
        c = -c;
        c += m;
        c = (c >> 0x3) | (c << 0x5);
        c -= 0xfd;
        c = -c;
        c ^= 0xf;
        c = ~c;
        c -= 0xec;
        c = (c >> 0x1) | (c << 0x7);
        c += 0xa2;
        c ^= m;
        c -= m;
        c = -c;
        c -= m;
        c ^= m;
        c = (c >> 0x1) | (c << 0x7);
        c = ~c;
        c -= 0x4b;
        c ^= 0xbe;
        c -= 0x60;
        c ^= m;
        c = -c;
        c = (c >> 0x2) | (c << 0x6);
        c -= m;
        c ^= 0xba;
        c -= 0x99;
        c = (c >> 0x1) | (c << 0x7);
        c = ~c;
        c += 0xae;
        c ^= 0x8a;
        c -= 0x5f;
        c ^= 0x1b;
        c = ~c;
        c = -c;
        c ^= 0x17;
        c = -c;
        c -= 0xba;
        c ^= 0x75;
        c += 0xc7;
        c = (c >> 0x7) | (c << 0x1);
        c += 0xd0;
        c = ~c;
        c += m;
        c = (c >> 0x6) | (c << 0x2);
        c = ~c;
        c ^= m;
        c += m;
        c ^= 0xba;
        c -= m;
        c = ~c;
        c += m;
        c ^= m;
        c = (c >> 0x3) | (c << 0x5);
        c -= m;
        c ^= 0x3;
        c = ~c;
        c = (c >> 0x3) | (c << 0x5);
        c -= m;
        c ^= m;
        c += m;
        c = (c >> 0x6) | (c << 0x2);
        c += m;
        c ^= 0x43;
        c = ~c;
        c ^= 0x5b;
        c += m;
        c ^= 0x3c;
        c = (c >> 0x5) | (c << 0x3);
        c -= 0x49;
        c ^= m;
        c -= m;
        c = (c >> 0x1) | (c << 0x7);
        c = -c;
        c = (c >> 0x1) | (c << 0x7);
        c += 0xbd;
        c = ~c;
        c -= m;
        c ^= 0xfa;
        c -= 0x6f;
        c ^= 0x66;
        c -= m;
        c ^= 0x6b;
        c += m;
        c ^= 0x64;
        c = (c >> 0x5) | (c << 0x3);
        c -= 0xba;
        c = ~c;
        c ^= 0xa0;
        c -= m;
        c ^= 0x13;
        c -= 0x65;
        c ^= m;
        c = (c >> 0x2) | (c << 0x6);
        c ^= 0x7e;
        c += m;
        c ^= 0x9d;
        c -= m;
        c = -c;
        c ^= m;
        c = ~c;
        c -= 0x39;
        c ^= m;
        c -= m;
        c ^= 0x2a;
        s[m] = c;
    }

    public_key = string((char*)s);

    //private_key = "-----BEGIN RSA PRIVATE KEY-----\nMIICXAIBAAKBgQCjmsZ9yXY8AmgJoZQfLZFkiefr81FwRzHMa38EO9eIOS7vXXIo\ngYzK5mBlXRP8YRAjReoQ1JyGf8qKeQdZV52ln2ALNU5MXpNnC/Aow3Z5gnEqw/i9\n8TuVVehhNKB7bxsJyaRPdwsaAZwmPQOXeyEIEYkgEZ03xl8ttaE5/k1SywIDAQAB\nAoGAE4bPlgfLktH+tNeSJD5+Ee/bvG+oEqyMpwdVe53g1aNtlKgkje0YbsccTjzT\nu9Wh5XPcwfF+VRc/h1N2qxUn/YGtx+MnY59v46Npb+p44zlJA/VCSeN+6LI2CMzg\n6Km4vrP6Wwr5pYcygrT4a2YUyz6ereZEG4o45SHcvbERnaECQQDTLkD0McvZgLRn\n7i3PyJhNjLnRTGVIW66HubBfAAb5JeP0gMpC9U44vwSHms/qKCzkF0XXyEmINjYp\n43pK0pPDAkEAxlOlLOizWjcObvPt3RIJt+2hPuv/cxvadoMpcNVb1BpxrEJh4P+W\n/8fLyAcKgwYI82MsV76/TL4ZCOXjYYz8WQJAZE+CNvw4UjZae0udoycrgRqpYQui\nyH4UJxBDAL0oksw2vXs3mBqhbz78zdTxkc5HlIihel/+3Z6yl68f9c6UtQJBALdk\nfabrf336z1P2KIx2s5nrGWIP5OXSac+RNe7qvMGDun2p3MysTDs1y9CqAiRqnjhC\nCq0Q6+DRIGFEA2Efd/ECQFjX5cycyYkOPS6WrTVSP9W8SC6Q7BxdnGUw6OPx0QXb\n4mSqF5qzL/1+aX/4VuvxTgqXyaqKJQQ1t5sL6kMHDXE=\n-----END RSA PRIVATE KEY-----";
    //public_key = "-----BEGIN PUBLIC KEY-----\nMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCZafUTg6W4380/rB5TBqudXdnP\ns4H5YZ/NLXgXb9Y6B1nR05t+D6VfI6OpIwPrjTz/ssecg+Ljed6bVGOlM3XhPLTr\nkxCZwgg94tNqKxmFLENDSNl/hpl+H+KbRL7B0IHhUnIpnScOzXvlYIs6OgrbexBQ\nv6apCNC196a6SZ6k8QIDAQAB\n-----END PUBLIC KEY-----";

    return;
}

RSA* createRSA(unsigned char* key, int public_)
{
    RSA* rsa = NULL;
    BIO* keybio;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio == NULL)
    {
        //printf("Failed to create key BIO");
        return 0;
    }
    if (public_)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
    }
    if (rsa == NULL)
    {
        //printf("Failed to create RSA");
    }

    return rsa;
}

size_t b64_decoded_size(const char* in)
{
    size_t len;
    size_t ret;
    size_t i;

    if (in == NULL)
        return 0;

    len = strlen(in);
    ret = len / 4 * 3;

    for (i = len; i-- > 0; ) {
        if (in[i] == '=') {
            ret--;
        }
        else {
            break;
        }
    }

    return ret;
}

int b64_isvalidchar(char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 1;
    if (c >= 'a' && c <= 'z')
        return 1;
    if (c == '+' || c == '/' || c == '=')
        return 1;
    return 0;
}

int b64_decode(const char* in, unsigned char* out, size_t outlen)
{
    size_t len;
    size_t i;
    size_t j;
    int    v;

    if (in == NULL || out == NULL)
        return 0;

    len = strlen(in);
    if (outlen < b64_decoded_size(in) || len % 4 != 0)
        return 0;

    for (i = 0; i < len; i++) {
        if (!b64_isvalidchar(in[i])) {
            return 0;
        }
    }

    for (i = 0, j = 0; i < len; i += 4, j += 3) {
        v = b64invs[in[i] - 43];
        v = (v << 6) | b64invs[in[i + 1] - 43];
        v = in[i + 2] == '=' ? v << 6 : (v << 6) | b64invs[in[i + 2] - 43];
        v = in[i + 3] == '=' ? v << 6 : (v << 6) | b64invs[in[i + 3] - 43];

        out[j] = (v >> 16) & 0xFF;
        if (in[i + 2] != '=')
            out[j + 1] = (v >> 8) & 0xFF;
        if (in[i + 3] != '=')
            out[j + 2] = v & 0xFF;
    }

    return 1;
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
{
    std::string ret{};
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';

    }

    return ret;
}

string openssl_public_encrypt(string str, string key)
{
    unsigned char raw_estr[4096];
    RSA* rsaKey = createRSA((unsigned char*)(key.c_str()), 1);
    int len = RSA_public_encrypt(str.length() * sizeof(char), (unsigned char*)(str.c_str()), (unsigned char*)(raw_estr), rsaKey, RSA_PKCS1_OAEP_PADDING);
    return base64_encode(raw_estr, len);
}

string openssl_private_decrypt(string estr, string key)
{
    unsigned char str[4096];
    unsigned char raw_estr[4096];
    RSA* rsaKey = createRSA((unsigned char*)(key.c_str()), 0);
    int len = b64_decoded_size(estr.c_str());
    b64_decode(estr.c_str(), raw_estr, len);
    RSA_private_decrypt(len, (unsigned char*)raw_estr, (unsigned char*)(str), rsaKey, RSA_PKCS1_OAEP_PADDING);
    return string((char*)str);
}