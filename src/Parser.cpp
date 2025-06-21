#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Parser.h"
#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "VehicleGenerator.h"
#include "BusStop.h"
#include "Intersection.h"
#include "../tinyxml/tinyxml.h"

/**
 * @brief Parses the XML input file and constructs simulation elements.
 * 
 * The XML file should contain tags describing roads, vehicles, bus stops,
 * intersections, traffic lights, and vehicle generators. The method parses
 * the file and fills the given vectors with pointers to created objects.
 * 
 * Supported tags:
 * - BAAN (road)
 * - VOERTUIG (vehicle)
 * - BUSHALTE (bus stop)
 * - KRUISPUNT (intersection)
 * - VERKEERSLICHT (traffic light)
 * - VOERTUIGGENERATOR (vehicle generator)
 * 
 * @param filename Path to the XML file to parse.
 * @param roads Vector to append pointers to Road objects.
 * @param generators Vector to append pointers to VehicleGenerator objects.
 * @param busStops Vector to append pointers to BusStop objects.
 * @param intersections Vector to append pointers to Intersection objects.
 * 
 * @throws std::runtime_error On file I/O or XML parsing failure,
 *         or when encountering inconsistent or invalid data.
 */
void Parser::parseFile(const std::string& filename,
                       std::vector<Road*>& roads,
                       std::vector<VehicleGenerator*>& generators,
                       std::vector<BusStop*>& busStops,
                       std::vector<Intersection*>& intersections) 
{
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open XML file: " + filename);
    }

    // Wrap file contents with a root tag to ensure well-formed XML
    std::stringstream buffer;
    buffer << "<ROOT>";
    buffer << file.rdbuf();
    buffer << "</ROOT>";

    TiXmlDocument doc;
    doc.Parse(buffer.str().c_str());

    if (doc.Error()) {
        throw std::runtime_error("Failed to parse XML: " + std::string(doc.ErrorDesc()));
    }

    TiXmlElement* elem = doc.FirstChildElement("ROOT")->FirstChildElement();
    if (!elem) {
        throw std::runtime_error("No root elements found in XML file.");
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

                // Find the road by name or create a new default-length road
                Road* road = nullptr;
                for (auto* r : roads) {
                    if (r->getName() == baan) {
                        road = r;
                        break;
                    }
                }
                if (!road) {
                    road = new Road(baan, 1000);
                    roads.push_back(road);
                }

                if (pos > road->getLength()) {
                    throw std::runtime_error("Vehicle position exceeds road length: " + std::to_string(pos));
                }

                // Instantiate the correct vehicle subclass
                if (type == "bus") {
                    road->addVehicle(new Bus(road, pos));
                } else if (type == "auto") {
                    road->addVehicle(new Auto(road, pos));
                } else if (type == "politiecombi") {
                    road->addVehicle(new Combi(road, pos));
                } else if (type == "ziekenwagen") {
                    road->addVehicle(new Ziek(road, pos));
                } else if (type == "brandweerwagen") {
                    road->addVehicle(new Brand(road, pos));
                } else {
                    throw std::runtime_error("Invalid vehicle type: " + type);
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

                bool found = false;
                for (auto* r : roads) {
                    if (r->getName() == baan) {
                        BusStop* busStop = new BusStop(baan, pos, wachttijd);
                        busStops.push_back(busStop);
                        r->addBusStop(busStop);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    throw std::runtime_error("Bus stop refers to unknown road: " + baan);
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

                    if (road1 && road2) {
                        Intersection* isec = new Intersection(road1, pos1, road2, pos2);
                        intersections.push_back(isec);
                        road1->addIntersection(isec);
                        road2->addIntersection(isec);
                    } else {
                        throw std::runtime_error("Intersection must connect existing roads.");
                    }
                }
            } else {
                throw std::runtime_error("Intersection must have 2 roads.");
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

                bool found = false;
                for (auto* r : roads) {
                    if (r->getName() == baan) {
                        r->addTrafficLight(new TrafficLight(r, pos, cyclus));
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    throw std::runtime_error("Traffic light refers to unknown road: " + baan);
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

                bool found = false;
                for (auto* r : roads) {
                    if (r->getName() == baan) {
                        generators.push_back(new VehicleGenerator(r, freq, type));
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    throw std::runtime_error("Vehicle generator refers to unknown road: " + baan);
                }
            }
        }

        elem = elem->NextSiblingElement();
    }
}
