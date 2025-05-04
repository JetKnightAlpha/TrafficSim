#include <iostream>
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
            TiXmlElement* typeElement = elem->FirstChildElement("type");
            if (baanElement && posElement && typeElement) {
                std::string baan = baanElement->GetText();
                int pos = std::stoi(posElement->GetText());
                std::string type = typeElement->GetText();

                for (auto* r : roads) {
                    if (r->getName() == baan) {
                        r->addVehicle(new Vehicle(r, pos, type));
                        break;
                    }
                }
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
            std::vector<Intersection::Entry> entries;
            TiXmlElement* baanElement = elem->FirstChildElement("baan");
            while (baanElement) {
                TiXmlElement* posElement = baanElement->FirstChildElement("positie");
                if (posElement) {
                    std::string baan = baanElement->GetText();
                    double pos = std::stod(posElement->GetText());
                    entries.push_back(Intersection::Entry(baan, pos));
                }
                baanElement = baanElement->NextSiblingElement("baan");
            }
            intersections.push_back(new Intersection(entries));
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
