#pragma once
#include <cstring>

#include "sha1.h"

#define LOGINLENGTH 8

class Chat {
public:
	enum HashDataStatus {
		free,
		deleted,
		engaged
	};

    Chat();
    void reg(char _login[LOGINLENGTH], char _pass[], int pass_length);
    bool login(char _login[LOGINLENGTH], char _pass[], int pass_length);
	void resize();
	void show() const;

private:
	struct HashData
	{
        HashData():
            login(""),
            pass_sha1_hash(0),
			status (free)
		{}

        ~HashData() {
            if(pass_sha1_hash != 0 )
                delete [] pass_sha1_hash;
        }
        HashData(char _login[LOGINLENGTH], uint* sh1) {
            strcpy(login, _login);
            pass_sha1_hash = sh1;
        }
        HashData& operator = (const HashData& other) {
            strcpy(login, other.login);

            if(pass_sha1_hash != 0)
                delete [] pass_sha1_hash;
            pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];

            memcpy(pass_sha1_hash, other.pass_sha1_hash, SHA1HASHLENGTHBYTES);

			status = engaged;

            return *this;
        }

		char login[LOGINLENGTH];
		uint* pass_sha1_hash = nullptr;
		HashDataStatus status = free;
	};

	size_t hash_func(char[LOGINLENGTH], int offset) const;

	HashData *hd = new HashData[hash_data_size];
	size_t hash_data_size = 10;
    int data_count;
};
