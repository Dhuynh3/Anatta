#include "Auth/Auth.h"
#include "GUI/GUI.h"
#include "Security/Security.h"









int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	
	Secure.AllocateConsole();

	
	Integer n("0xbeaadb3d839f3b5f"), e("0x11"), d("0x21a5ae37b9959db9");

	RSA::PublicKey pubKey;
	pubKey.Initialize(n, e);

	Integer m, c;
	string message = "secret";

	cout << "message: " << message << endl;

	// Treat the message as a big endian byte array
	m = Integer((const CryptoPP::byte*)message.data(), message.size());
	cout << "m: " << hex << m << endl;

	// Encrypt
	c = pubKey.ApplyFunction(m);
	cout << "c: " << hex << c << endl;
	
	//// Decryption

	RSA::PrivateKey privKey;
	privKey.Initialize(n, e, d);

	///////////////////////////////////////////////////////////////

	AutoSeededRandomPool prng;
	Integer x("0x3f47c32e8e17e291"), r;
	string recovered;

	// Decrypt
	r = privKey.CalculateInverse(prng, x);
	cout << "r: " << hex << r << endl;

	// Round trip the message
	size_t req = r.MinEncodedSize();
	recovered.resize(req);
	r.Encode((CryptoPP::byte*)recovered.data(), recovered.size());

	cout << "recovered: " << recovered << endl;

	


	


	
	
	Secure.RunThread(WebSocketThread, "WebSocket", 0);

	
	while (true) {
		printf("Heartbeat\n");
		
		printf("Connection %p\n", Connect.wsPtr.get()->getConnection().get());

		if (Connect.wsPtr.get()->getConnection().get() != nullptr) {
			Connect.wsPtr.get()->getConnection().get()->send("yeeeeee!");
		}
	
		
		Sleep(1000);
	}

	return 0;
}