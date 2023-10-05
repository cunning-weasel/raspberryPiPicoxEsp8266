ESP8266 12E D1 Mini Responsibilities:

Configure as an Access Point (AP): The ESP8266 should create a local Wi-Fi network.
Implement a WebSockets Server: Run a WebSockets server to handle game communication between clients.
Manage LAN Connections: Handle LAN connections and coordinate multiplayer games at a high level.
UART Communication: Establish UART communication with the Raspberry Pi Pico for game data exchange.
Serve Control Interface: Serve a control interface (HTML, JS) to connected clients (players).

Raspberry Pi Pico (RP2040) Responsibilities:

Run Quake-WASM Game Code: Execute the Quake-WASM game code, which includes game logic and graphics.
Serve Quake-WASM Files: Serve the Quake-WASM files (HTML, JS, WASM) to connected clients (players) via a web server.
Handle Multiplayer Aspects: Manage multiplayer aspects such as game sessions and multiplayer communication.
UART Communication: Establish UART communication with the ESP8266 for game data exchange.

To-Do's:

Configure the ESP8266 as an Access Point (AP).
Implement a WebSockets server on the ESP8266 to handle game communication.
Serve a control interface (HTML, JS) to connected clients via the ESP8266.
On the Raspberry Pi Pico, serve the Quake-WASM files (HTML, JS, WASM) using your C web server code.
The Pico will run the Quake-WASM game code, handle game logic, and manage multiplayer aspects.
Establish UART communication between the ESP8266 and Raspberry Pi Pico for game data exchange.
Players on other devices connect to the ESP8266's local Wi-Fi network and access the control interface.
Players interact with the game served by the Pico via the control interface, and the ESP8266 handles game communication and coordination.
