#include "runProgram.h"

#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <algorithm>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <cmath>
#include <chrono>
#include <map>

// Linked de RFID waarden aan een naam.
std::map<std::string, std::string> rfidNames = {
    {"D935D814", "Beheerder"},
    {"67B37A05", "Bezoeker"},
    {"B155721D", "Verzorger Jan"},
    {"F4889C04", "Arts Marko"},
    {"5CCC2502", "Client Evert"}
};

TCPServer serverIn;   // 8080 = Pi A
TCPServer serverOut;  // 8081 = Qt/nc

std::map<std::string, bool> rfidPresent; // true = aanwezig, false = weg

// CAN waarden (hex) vertaald, een switch-statement werkt niet op een string.
enum CAN_ID
{
    DEUR = 0x230,
    AFSTAND = 0x102,
    BRAND_ALARM = 0x10
};

runProgram::runProgram() : can("can0"){}

/**
 * @brief Verwijderd witregels aan het begin van een regel die kunnen ontstaan doordat er meerdere berichten
 tegelijk over de TCP-verbinding worden verstuurd.
 * 
 * @param value 
 * @return std::string 
 */
static std::string trim(const std::string& value) {
    const char* whitespace = " \t\n\r"; // Definitie van een witregel.
    size_t start = value.find_first_not_of(whitespace); // zoekt eerste letter
    if (start == std::string::npos) return "";
    size_t end = value.find_last_not_of(whitespace); // zoekt laatste letter
    return value.substr(start, end - start + 1); // returned de bewerkte string
}

/**
 * @brief Splitst een inkomend bericht op in een Key en Value waarde zodat deze later verwerkt
 kunnen worden. 
 * 
 * @param input Het bericht dat binnengekregen is via TCP.  
 * @param key Wordt gevuld met de Key uit het bericht.
 * @param value Wordt gevuld met de Value uit het bericht.
 */
static void parseKeyValue(const std::string& input, std::string& key, std::string& value) {
    std::string buffer = trim(input); // Gebruikt de trim functie.

    // Verwijderd eventuele "".
    if (!buffer.empty() && buffer.front() == '{' && buffer.back() == '}') {
        buffer = trim(buffer.substr(1, buffer.size() - 2));
    }

    // zoekt de : in het bericht
    size_t colon = buffer.find(':');
    if (colon == std::string::npos) {
        colon = buffer.find('=');
    }

    // Splitst het bericht op de plek van het : teken op in een key en value.
    if (colon != std::string::npos) {
        key = trim(buffer.substr(0, colon));
        value = trim(buffer.substr(colon + 1));

        if (!key.empty() && key.front() == '"' && key.back() == '"') {
            key = key.substr(1, key.size() - 2);
        }
        if (!value.empty() && value.front() == '"' && value.back() == '"') {
            value = value.substr(1, value.size() - 2);
        }
    }
}

/**
 * @brief De Main loop van het programma. Hierin worden als het ware alle nodige functies aangeroepen
 voor het ontvangen en versturen van een bericht.
 * 
 */
void runProgram::run(){ 
    serverIn.start(8080);
    serverOut.start(8081);
    can.open();

    int clientA = -1;
    int clientB = -1;

    std::cout << "Server started (8080=IN, 8081=OUT)\n";

    char buffer[1024];

    while (true) {

        fd_set readfds;
        FD_ZERO(&readfds);
        
        // Er zijn drie FD's via waar informatie binnen kan komen.
        // 1. Qt-Creator    2. PI A     3. CAN-Bus
        FD_SET(serverIn.getServerFd(), &readfds);
        FD_SET(serverOut.getServerFd(), &readfds);
        FD_SET(can.getFd(), &readfds);  

        if (clientA != -1){
            FD_SET(clientA, &readfds);
        }
            
        if (clientB != -1) {
            FD_SET(clientB, &readfds);
        } 
            
        int maxfd = std::max({serverIn.getServerFd(), serverOut.getServerFd(), clientA, clientB, can.getFd()}); 

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000; // 10 ms                      

        // Wacht tot er data binnenkomt op een van de FD's of tot de timeout voorbij is.
        if (select(maxfd + 1, &readfds, nullptr, nullptr, &timeout) < 0) {
            perror("select");
            continue;
        }

        // Connecten met Pi A op poort 8080.
        if (FD_ISSET(serverIn.getServerFd(), &readfds)) {
            clientA = serverIn.acceptClient();
            std::cout << "Pi A connected\n";
        }

        // Connecten met QT op poort 8081.
        if (FD_ISSET(serverOut.getServerFd(), &readfds)) {
            clientB = serverOut.acceptClient();
            std::cout << "Qt/nc connected\n";
        }

        // Data van Pi A afhandelen en doorsturen naar QT.
        if (clientA != -1 && FD_ISSET(clientA, &readfds)) {

            int len = recv(clientA, buffer, sizeof(buffer), 0);

            if (len <= 0) {
                std::cout << "Pi A disconnected\n";
                close(clientA);
                clientA = -1;
                continue;
            }

            std::string incoming(buffer, len);
            std::cout << incoming << std::endl;

            handleTcpMessage(incoming);

            // Forwarden naar QT.
            if (clientB != -1) {
                int sent = send(clientB, buffer, len, 0);
                if (sent <= 0) {
                    std::cout << "Qt/nc disconnected\n";
                    close(clientB);
                    clientB = -1;
                }
            }
        }

        // Uitlezen van de QT Creator berichten 
        if (clientB != -1 && FD_ISSET(clientB, &readfds)) {
        int len = recv(clientB, buffer, sizeof(buffer), 0);

        if (len <= 0) {
            std::cout << "Qt disconnected\n";
            close(clientB);
            clientB = -1;
        } else {
            std::string incoming(buffer, len);
            std::cout << "Van Qt: " << incoming << std::endl;
            handleTcpMessage(incoming);
        }
}

        // CAN-berichten uitlezen, verwerken en doorsturen naar QT.
        if (FD_ISSET(can.getFd(), &readfds)) {
            struct can_frame frame;

            if (can.readFrame(frame)) {
                canMessageHandler(frame);

                if (clientB != -1) {
                    send(clientB, "Distance received\n", 18, 0);
                }
            }
        }
    }
}

/**
 * @brief Deze functie verwerkt de binnenkomende CAN-berichten. Op basis van het CAN-ID worden de juiste acties ondernomen.
     - AFSTAND: De afstandswaarde wordt uit het CAN-bericht gehaald en opgeslagen in de variabele readDistance. Ook wordt deze waarde geprint.
     - BRAND_ALARM: De brandstatus wordt uit het CAN-bericht gehaald. 
       Afhankelijk van de status (actief of inactief) worden verschillende TCP-berichten verstuurd om de LED, ventilatie en matrix aan te passen.    
 * 
 * @param frame Het binnenkomende CAN-bericht dat verwerkt moet worden.
 */
void runProgram::canMessageHandler(const struct can_frame& frame){
    switch (frame.can_id & CAN_SFF_MASK) {

        case AFSTAND: {
            uint16_t distance =
                (frame.data[0] << 8) | frame.data[1];

            std::cout << "Distance: " << distance << " cm\n";
            readDistance = distance;
            break;
        }
		case BRAND_ALARM:
		{
			uint8_t brandStatus = (frame.data[0]); // De data uit RxData[0] is de brandstatus.
			
			if(brandStatus == 0x01) //Bij brandstatus 1 moet de lamp op wit aan gaan en de ventilatie afgesloten worden.
			{
				handleTcpMessage("LED:BRANDON");
				handleTcpMessage("VENT:OFF");
                handleTcpMessage("MATRIX:BRANDON");
                //std::cout << "LED turned ON2\n";
			}
			else if(brandStatus == 0x00) //Bij brandstatus 0 moet de lamp weer uit gaan en de ventilatie weer opengaan.
			{
				handleTcpMessage("LED:BRANDOFF");
				handleTcpMessage("VENT:ON");
                handleTcpMessage("MATRIX:BRANDOFF");
			}
			break;
		}

        default:
            std::cout << "Unknown CAN ID\n";
            break;
    }
}

/**
 * @brief Deze functie bepaalt het type van een binnenkomend TCP-bericht op basis van de key.
 * @param key De key van het binnenkomende TCP-bericht.
 * @return Het type van het TCP-bericht.
 */
MessageType runProgram::getMessageType(const std::string& key)
{
    if (key == "BED"){
        return MessageType::BED;
    }
    if (key == "ID"){
        return MessageType::ID;
    }
	if (key == "LED")
		return MessageType::LED;
	if (key == "VENT")
		return MessageType::VENT;
    if (key == "MATRIX")
        return MessageType::MATRIX;
    return MessageType::UNKNOWN;
}

/**
 * @brief Deze functie verwerkt de binnenkomende TCP-berichten, daarnaast worden
   er commando's uitgevoerd op basis van het type van het bericht en de waarde.
 * @param msg Het binnenkomende TCP-bericht dat verwerkt moet worden.
 */
void runProgram::handleTcpMessage(const std::string& msg) {
    std::string key;
    std::string value;
    parseKeyValue(msg, key, value);

    if (key.empty()) {
        std::cout << "TCP handler: no valid key/value found\n";
        return;
    }

    switch (getMessageType(key))
	{  
        // BED-status verwerken, afhankelijk van de waarde (ON/OFF) wordt er een bericht geprint.
        case MessageType::BED:
            if (value == "ON")
			{
                std::cout << "BED turned ON\n";
            }else 
			{
                std::cout << "BED turned OFF\n";
            }
			break;
        
        // ID-berichten verwerken, afhankelijk van de waarde (RFID) worden er verschillende acties ondernomen.
        case MessageType::ID:
        { 
            bool isNowPresent = !rfidPresent[value]; // toggle of er iemand wel of niet aanwezig is
            rfidPresent[value] = isNowPresent;

            std::string name   = rfidNames.count(value) ? rfidNames[value] : "Onbekend (" + value + ")";
            std::string status = isNowPresent ? "aanwezig" : "vertrokken";

            if (value == "D935D814") // Beheerder
            { 
                if (isNowPresent)
                {
                    can.sendCAN(DEUR, {0x01, 0x01});
                }
            }else if (value == "72E07B05") // Client B
            {
				if (isNowPresent){
					can.sendCAN(DEUR, {0x01, 0x02});
				}
			}
            else if (value == "67B37A05") // Bezoeker
            { 
                if (isNowPresent)
                {
                }
            }
            else if (value == "B155721D") // Arts/verzorger
            {
                serverIn.sendClient("B:" + name + " " + status);
            }
            else if (value == "F4889C04") // Arts/verzorger
            {
                serverIn.sendClient("B:" + name + " " + status);
            }
            else if (value == "5CCC2502") // Client A
            {
                if (isNowPresent)
                {
                    can.sendCAN(DEUR, {0x01, 0x01});
                    handleTcpMessage("MATRIX:ON");
                    handleTcpMessage("LED:ON");
                }
                else
                {
                    handleTcpMessage("MATRIX:OFF");
                    handleTcpMessage("LED:OFF");
                }
            }

            break;
        } // sluit case MessageType::ID scope

        // LED-berichten verwerken, afhankelijk van de waarde worden er verschillende commando's naar Pi A gestuurd.
        case MessageType::LED:
            if (value == "BRANDON")
            {
                serverIn.sendClient("C:LEDBRANDON\n");
            }
            else if (value == "BRANDOFF")
            {
                serverIn.sendClient("C:LEDBRANDOFF\n");
            }
            if (value == "ON")
            {
                serverIn.sendClient("C:LEDon\n");
            }
            else if (value == "OFF")
            {
                 serverIn.sendClient("C:LEDoff\n");
            } 
            break;

        // VENT-berichten verwerken, afhankelijk van de waarde worden er verschillende commando's naar Pi A gestuurd.
        case MessageType::VENT:
            if (value == "ON")
            {
                serverIn.sendClient("B:VENTon\n");
            }
            else if (value == "OFF")
            {
                serverIn.sendClient("B:VENToff\n");
            }
            break;

        // MATRIX-berichten verwerken, afhankelijk van de waarde worden er verschillende commando's naar Pi A gestuurd.
        case MessageType::MATRIX:
            if (value == "BRANDON")
            {
                serverIn.sendClient(std::string("B:MATRIXBRANDON") + "\n");
                serverIn.sendClient(std::string("B:Brand, gebouw verlaten") + "\n");
                
            }
            else if (value == "BRANDOFF")
            {
                serverIn.sendClient(std::string("MATRIXClear") + "\n");
            }
            else if (value == "ON")
            {
                serverIn.sendClient(std::string("B:MATRIXON") + "\n");

            }
            else if (value == "OFF")
            {
                serverIn.sendClient(std::string("B:MATRIXOFF") + "\n");
            }
            else 
            {
            serverIn.sendClient(std::string("B:" + value) + "\n");
            }
            break;

    } 
} 

/**
 * @brief Verwijderd het runProgram::runProgram object
 * 
 */
runProgram::~runProgram() {}
