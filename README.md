Steps to run:

Clone the repository:

Bash
git clone https://github.com/Bhargavi49/mini-redis-cpp.git
Compile the code:
Use g++ and link the Windows socket library:

Bash
g++ src/main.cpp src/utils.cpp -o redis_server -lws2_32
Launch the Server:

Bash
./redis_server
Test in Browser:
Open http://localhost:6379/SET to save data, 
Then http://localhost:6379/GET to retrieve it.
