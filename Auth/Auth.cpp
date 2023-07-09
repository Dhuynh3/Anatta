#include "../Auth/Auth.h"


Connection Connect("ws://127.0.0.1", "/chat", 8848);


Connection::Connection(std::string input_server_string, std::string input_path, uint16_t input_port) {
	
	// Set data members to input values.
	this->server = input_server_string;
	this->port = input_port;
	this->path = input_path;
	
	// Create the server string.
	if (this->port == 0) {
		this->serverString = this->server; // ws://127.0.0.1/chat
	}
	else {
		this->serverString = this->server + ":" + std::to_string(this->port); // ws://127.0.0.1/chat:8848
	}
	
	this->wsPtr = WebSocketClient::newWebSocketClient(this->serverString);
	this->req = HttpRequest::newHttpRequest();
	this->req->setPath(this->path);
	
}

void Connection::Run() {
	
	// For debugging outputs.
    app().setLogLevel(trantor::Logger::kDebug);

	// Run the websocket client and begin connection. This should be called in a seperate thread.
    app().run();
    
}

void Connection::PrintConnectionDetails() {
	printf("Full Server String: %s\n", this->serverString.c_str());
}


/**
* This function will send a message to the server. 
* @params msg, the json string to be sent
* @params type, the type of message usually it's text.
*/
void Connection::SendMsg(std::string msg, drogon::WebSocketMessageType type) {
	
	// Nullptr connection pointer means we haven't fully connected yet.
	if (this->wsPtr.get()->getConnection().get() == nullptr) {
		printf("Connection not ready yet.\n");
		return;
	}
	
	// Encrypt the message.
	std::string cipher = this->AES_Encrypt(msg, this->AES_KEY, this->AES_IV);
	
	// Send the message.
	this->wsPtr.get()->getConnection().get()->send(cipher, type);

}


void Connection::CloseConnection() {
	
	// Nullptr connection pointer means we haven't fully connected yet.
	if (this->wsPtr.get()->getConnection().get() == nullptr) {
		printf("Connection not ready yet. Nothing to shutdown.\n");
		return;
	}

	// Close the connection.
	this->wsPtr.get()->getConnection().get()->shutdown();
	this->wsPtr.get()->getConnection().get()->forceClose();
	
}

/**
* This function prepares a json string to send to the server for authentication of the user.
* We will send the command as "Login", the license key, the HWID, and the loader hash.
*/
bool Connection::Login(std::string LicenseKey) {
	
	Json::Value jsonobj;
	jsonobj["C"] = "Login";
	jsonobj["L"] = LicenseKey;
	jsonobj["H"] = this->GetFullHWID();
	jsonobj["LH"] = this->CalcHash256(this->Myexepath());
	
	this->SendMsg(jsonobj.toStyledString(), drogon::WebSocketMessageType::Text);
	
	return true;
}

void Connection::HandleLoginResults(std::string result) {
		
	ExploitParams params;
	params.result = result;
	// Create a new thread to handle the injection.
	Secure.RunThread(ExploitThread, "ExploitThread", &params);

}

/**
* This thread sets up important handlers for a websocket connection with our server.
* 
* MessageHandler will handle all messages sent from the server.
* 
* ConnectionClosedHandler will handle when the connection is closed.
* 
*/
PVOID WebSocketThread(void*) {

	// Set the message handler, this is called when a message is received.
	Connect.wsPtr->setMessageHandler([](const std::string& message, const WebSocketClientPtr&, const WebSocketMessageType& type) {

		// Find the correct message type.
		std::string messageType = "Unknown";
	
		switch (type) {
			case WebSocketMessageType::Text: {
				messageType = "text";

				// Decrypt the message.
				size_t length;
				std::string decryptedMessage = Connect.AES_Decrypt(message, Connect.AES_KEY, Connect.AES_IV, &length);
				printf("Decrypted msg %s\n", decryptedMessage.c_str());
				
				Json::Value root;
				Json::Reader reader;
				bool parse = reader.parse(decryptedMessage, root);

				if (!parse) {
					printf("Failed to parse JSON\n");
					Connect.wsPtr->stop();
				}
				
				std::string command = root["C"].asString();

				// The server sent us the results of login.
				if (command == "Login") {
					
					Connect.frame->DisplayLoginResults(root);
			
				}
				
				/*
				// If it is an object its proper commands, otherwise, we send over the public key
				if (root.isObject()) {

					std::string command = root["C"].asString();

					if (command == "LoadPubKey") {

						std::string pubKey = root["P"].asCString();

						
					}
				}
				else {
					
					ArraySource as((byte*)decryptedMessage.data(), decryptedMessage.size(), true, nullptr);
					Connect.ServerPublicKey.Load(as);
					
					std::string cip = Connect.RSA_Encrypt("Validated msgd", Connect.ServerPublicKey);
					Connect.wsPtr.get()->getConnection().get()->send(cip, WebSocketMessageType::Binary);

				}
				*/

			
				break;
			}
			case WebSocketMessageType::Pong: {
				
				
				break;
			}
			case WebSocketMessageType::Ping: {
				
			
				break;
			}
			case WebSocketMessageType::Binary: {
	
				// Decrypt the message to get AES KEY and IV
				std::string decrypt = Connect.RSA_Decrypt(message, Connect.LoaderPrivateKey);

				Json::Value root;
				Json::Reader reader;
				bool parse = reader.parse(decrypt, root);

				// Close program immediately if we can't parse the JSON.
				if (!parse || !root.isObject()) {
					
				}

				Connect.AES_KEY = root["K"].asString();
				Connect.AES_IV = root["I"].asString();
				
				printf("Aes key %s\n", Connect.AES_KEY.c_str());

				break;
			}
			case WebSocketMessageType::Close: {
				messageType = "Close";
				break;
			}
			case WebSocketMessageType::Unknown: {
				
				

				break;
			}
			
			
											
		}

		//LOG_INFO << "new message (" << messageType << "): " << message;

	});

	// Set the connection closed handler.
	Connect.wsPtr->setConnectionClosedHandler([](const WebSocketClientPtr&) {

		// Once the connection is closed, log it. TODO - Uninstall hooks, etc.
		Connect.Death();
	
		LOG_INFO << "WebSocket connection closed!";
	});

	

	std::string spki;
	StringSink ss(spki);
	Connect.LoaderPublicKey.Save(ss);
	Connect.req->setBody(spki);

	
	// Connect to the server. On connection we will send our public RSA key to the server, and the server will send us it's public key.
	Connect.wsPtr->connectToServer(Connect.req, [](ReqResult r, const HttpResponsePtr&, const WebSocketClientPtr& wsPtr) {

		// Check if the connection was successful.
		if (r != ReqResult::Ok) {
			LOG_ERROR << "Failed to establish WebSocket connection!";
			wsPtr->stop();
			return;
		}

		LOG_INFO << "WebSocket connected!";

	
	});

	
	// Run the websocket client after all handlers have been setup.
	Connect.Run();

	return 0;
}
