#include "utils.h"
#include <iostream>
#include <winsock2.h> // Windows networking tool
#include <unordered_map>
#include<string>

#pragma comment(lib, "ws2_32.lib") // Needed for Windows networking

namespace redis {

   std::unordered_map<std::string, std::string> memory;
    void printWelcome() {
        std::cout << "--- MINI-REDIS ---" << std::endl;
    }

    void startServer() {
        WSADATA wsa;
        WSAStartup(MAKEWORD(2,2), &wsa);

        SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(6379);

        bind(s, (struct sockaddr *)&server, sizeof(server));
        listen(s, 3);
        
        std::cout << "Redis waiting on port 6379..." << std::endl;

        while (true) {
            SOCKET new_socket = accept(s, NULL, NULL);
            if (new_socket == INVALID_SOCKET) { continue; }

            char buffer[1024] = {0};
            recv(new_socket, buffer, 1024, 0);
            
            // Convert what the user sent into a string so we can read it
            std::string request(buffer);
            std::string body;

            
            // If the user types "SET", we save something.
            // If they just visit the page, we show them a welcome message.
            // THE COMMAND BRAIN:
            if (request.find("GET /SET") != std::string::npos) {
                // 1. SAVE the data
                memory["user"] = "Bhargavi"; 
                body = "SUCCESS: I have saved 'Bhargavi' into the database!";
            } 
            else if (request.find("GET /GET") != std::string::npos) {
                // 2. RETRIEVE the data
                // We check if the name is actually in our cabinet first
                if (memory.find("user") != memory.end()) {
                    body = "THE DATABASE SAYS: The user is " + memory["user"];
                } else {
                    body = "ERROR: No user found! Try visiting /SET first.";
                }
            } 
            else {
                // 3. DEFAULT Welcome message
                body = "Welcome to Mini-Redis! Try /SET to save or /GET to see the data.";
            }

            // Standard HTTP response
            std::string response = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: " + std::to_string(body.length()) + "\r\n"
                "Connection: close\r\n"
                "\r\n" + body;

                // Add << std::flush to the end of your print lines
            std::cout << "SERVER ALERT: Someone just called!" << std::endl << std::flush;
            std::cout << "DATA RECEIVED: " << buffer << std::endl << std::flush;

            send(new_socket, response.c_str(), (int)response.length(), 0);
            closesocket(new_socket);
        }
        closesocket(s);
        WSACleanup();
    }
    }
   