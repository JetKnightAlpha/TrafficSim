#include <iostream>
#include "Parser.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "VehicleGenerator.h"
#include "../tinyxml/tinyxml.h" // Assuming you are using TinyXML for XML parsing

void Parser::parseFile(const std::string& filename,
                       std::vector<Road*>& roads,
                       std::vector<VehicleGenerator*>& generators) {
    TiXmlDocument doc(filename.c_str());
    if (!doc.LoadFile()) {
        std::cerr << "Failed to load XML file: " << doc.ErrorDesc() << std::endl;
        return;
    }

    TiXmlElement* elem = doc.FirstChildElement();

    while (elem) {
        std::string tag = elem->Value();

        if (tag == "BAAN") {
            TiXmlElement* nameElement = elem->FirstChildElement("naam");
            TiXmlElement* lengthElement = elem->FirstChildElement("lengte");
            if (nameElement && lengthElement) {
                std::string name = nameElement->GetText();
                int length = std::stoi(lengthElement->GetText());
                roads.push_back(new Road(name, length));
            }
        }
        else if (tag == "VOERTUIG") {
            TiXmlElement* baanElement = elem->FirstChildElement("baan");
            TiXmlElement* posElement = elem->FirstChildElement("positie");
            if (baanElement && posElement) {
                std::string baan = baanElement->GetText();
                int pos = std::stoi(posElement->GetText());

                for (auto* r : roads) {
                    if (r->getName() == baan) {
                        r->addVehicle(new Vehicle(r, pos));
                        break;
                    }
                }
            }
        }
        else if (tag == "VERKEERSLICHT") {
            TiXmlElement* baanElement = elem->FirstChildElement("baan");
            TiXmlElement* posElement = elem->FirstChildElement("positie");
            TiXmlElement* cyclusElement = elem->FirstChildElement("cyclus");
            if (baanElement && posElement && cyclusElement) {
                std::string baan = baanElement->GetText();
                int pos = std::stoi(posElement->GetText());
                int cyclus = std::stoi(cyclusElement->GetText());

                for (auto* r : roads) {
                    if (r->getName() == baan) {
                        r->addTrafficLight(new TrafficLight(r, pos, cyclus));
                        break;
                    }
                }
            }
        }
        else if (tag == "VOERTUIGGENERATOR") {
            TiXmlElement* baanElement = elem->FirstChildElement("baan");
            TiXmlElement* freqElement = elem->FirstChildElement("frequentie");
            if (baanElement && freqElement) {
                std::string baan = baanElement->GetText();
                int freq = std::stoi(freqElement->GetText());

                for (auto* r : roads) {
                    if (r->getName() == baan) {
                        generators.push_back(new VehicleGenerator(r, freq));
                        break;
                    }
                }
            }
        }

        elem = elem->NextSiblingElement();
    }
}
