#include "httplib.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
	httplib::SSLClient cli("example.com", 80);
#else
	httplib::Client cli("example.com", 80);
#endif

	auto res = cli.Get("/");
	if (res) {
		cout << res->status << endl;
		cout << res->get_header_value("Content-Type") << endl;
		cout << res->body << endl;
	}
	
	return 0;
}
