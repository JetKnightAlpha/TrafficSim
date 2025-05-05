#include <iostream>
#include <fstream>
#include <sstream>
#include "Parser.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "VehicleGenerator.h"
#include "BusStop.h"
#include "Intersection.h"
#include "../tinyxml/tinyxml.h"

void Parser::parseFile(const std::string& filename,
                       std::vector<Road*>& roads,
                       std::vector<VehicleGenerator*>& generators,
                       std::vector<BusStop*>& busStops,
                       std::vector<Intersection*>& intersections) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open XML file: " << filename << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << "<ROOT>";
    buffer << file.rdbuf();
    buffer << "</ROOT>";

    TiXmlDocument doc;
    doc.Parse(buffer.str().c_str());

    if (doc.Error()) {
        std::cerr << "Failed to parse XML: " << doc.ErrorDesc() << std::endl;
        return;
    }

    TiXmlElement* elem = doc.FirstChildElement("ROOT")->FirstChildElement();
    if (!elem) {
        std::cerr << "No root elements found!" << std::endl;
        return;
    }

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
            TiXmlElement* typeElement = elem->FirstChildElement("type");
            if (baanElement && posElement && typeElement) {
                std::string baan = baanElement->GetText();
                int pos = std::stoi(posElement->GetText());
                std::string type = typeElement->GetText();

                // Check if the road exists, create it if not
                Road* road = nullptr;
                for (auto* r : roads) {
                    if (r->getName() == baan) {
                        road = r;
                        break;
                    }
                }
                if (!road) {
                    road = new Road(baan, 1000); // Default length if road doesn't exist
                    roads.push_back(road);
                }

                road->addVehicle(new Vehicle(road, pos, type));
            }
        }
        else if (tag == "BUSHALTE") {
            TiXmlElement* baanElement = elem->FirstChildElement("baan");
            TiXmlElement* posElement = elem->FirstChildElement("positie");
            TiXmlElement* wachttijdElement = elem->FirstChildElement("wachttijd");
            if (baanElement && posElement && wachttijdElement) {
                std::string baan = baanElement->GetText();
                double pos = std::stod(posElement->GetText());
                double wachttijd = std::stod(wachttijdElement->GetText());

                for (auto* r : roads) {
                    if (r->getName() == baan) {
                        busStops.push_back(new BusStop(baan, pos, wachttijd));
                        break;
                    }
                }
            }
        }
        else if (tag == "KRUISPUNT") {
            TiXmlElement* firstBaan = elem->FirstChildElement("baan");
            TiXmlElement* secondBaan = firstBaan ? firstBaan->NextSiblingElement("baan") : nullptr;

            if (firstBaan && secondBaan) {
                const char* pos1Attr = firstBaan->Attribute("positie");
                const char* pos2Attr = secondBaan->Attribute("positie");
                const char* roadName1 = firstBaan->GetText();
                const char* roadName2 = secondBaan->GetText();

                if (pos1Attr && pos2Attr && roadName1 && roadName2) {
                    double pos1 = std::stod(pos1Attr);
                    double pos2 = std::stod(pos2Attr);

                    Road* road1 = nullptr;
                    Road* road2 = nullptr;

                    for (auto* r : roads) {
                        if (r->getName() == roadName1) road1 = r;
                        if (r->getName() == roadName2) road2 = r;
                    }
                    intersections.push_back(new Intersection(road1, pos1, road2, pos2));
                }
            }
        }



        else if (tag == "VERKEERSLICHT") {
            TiXmlElement* baanElement = elem->FirstChildElement("baan");
            TiXmlElement* posElement = elem->FirstChildElement("positie");
            TiXmlElement* cyclusElement = elem->FirstChildElement("cyclus");
            if (baanElement && posElement && cyclusElement) {
                std::string baan = baanElement->GetText();
                double pos = std::stod(posElement->GetText());
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
            TiXmlElement* typeElement = elem->FirstChildElement("type");
            if (baanElement && freqElement && typeElement) {
                std::string baan = baanElement->GetText();
                int freq = std::stoi(freqElement->GetText());
                std::string type = typeElement->GetText();

                for (auto* r : roads) {
                    if (r->getName() == baan) {
                        generators.push_back(new VehicleGenerator(r, freq, type));
                        break;
                    }
                }
            }
        }

        elem = elem->NextSiblingElement();
    }
}
