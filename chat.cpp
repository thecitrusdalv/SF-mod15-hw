#include "chat.h"
#include "iostream"
#include "string.h"

Chat::Chat() {
    data_count = 0;
}

void Chat::show() const
{
	for (size_t i = 0; i < hash_data_size; i++) {
		if (hd[i].status == engaged)
			std::cout << "Index: " << i << ", Login: " << hd[i].login << std::endl;
	}
	std::cout << "Data count " << data_count << std::endl;
}

void Chat::resize()
{
	size_t old_size = hash_data_size;
	hash_data_size *= 1.2;

	HashData *hd_new = new HashData[hash_data_size];

	for (size_t i = 0; i < old_size; i++) {
		if (hd[i].status == engaged) {
			size_t index;
			for (size_t j = 0; j < hash_data_size; j++) {
				index = hash_func(hd[i].login, j);
				
				if (index >= hash_data_size) {
					resize();
					delete[] hd_new;
					return;
				}

				if (hd_new[index].status != engaged)
					break;
			}

			hd_new[index] = hd[i];
		}
	}

	delete[] hd;
	hd = hd_new;
}

size_t Chat::hash_func(char _login[LOGINLENGTH], int offset) const
{
	const float A = 0.7;
	size_t sum = 0;
	size_t i = 0;

	while (_login[i])
		sum += _login[i++];

	return int ((hash_data_size * (A * sum - int (A * sum))) + offset * offset);
}

void Chat::reg(char _login[LOGINLENGTH], char _pass[], int pass_length) {
	size_t index;
	size_t i;
	for (i = 0; i < Chat::hash_data_size; i++) {
		index = hash_func(_login, i);
		if (hd[index].status != engaged)
			break;
	}

	if (index >= hash_data_size) {
		resize();
		reg(_login, _pass, pass_length);
		return;
	}

	strcpy(hd[index].login, _login);
	hd[index].pass_sha1_hash = sha1(_pass, pass_length);
	hd[index].status = engaged;
	data_count++;
}

bool Chat::login(char _login[LOGINLENGTH], char _pass[], int pass_length) {
	size_t i;
	size_t index;
	for (i = 0; i < hash_data_size; i++) {
		index = hash_func(_login, i);
		if (!strcmp(hd[index].login, _login))
			break;
	}

	if (i >= hash_data_size) return false;

    uint* digest = sha1(_pass, pass_length);

    bool cmpHashes = !memcmp(
                         hd[i].pass_sha1_hash,
                         digest,
                         SHA1HASHLENGTHBYTES);
    delete[] digest;

    return cmpHashes;
}
