#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Simple demo of smart XML testing
class SmartXMLTester {
public:
    static bool analyzeXML(const std::string& xmlContent) {
        // Check for error patterns
        if (xmlContent.find("cyclus=\"-") != std::string::npos) {
            std::cout << "❌ ERROR DETECTED: Negative cycle time" << std::endl;
            return false; // Should fail
        }
        if (xmlContent.find("cyclus=\"0\"") != std::string::npos) {
            std::cout << "❌ ERROR DETECTED: Zero cycle time" << std::endl;
            return false; // Should fail
        }
        if (xmlContent.find("lengte=\"-") != std::string::npos) {
            std::cout << "❌ ERROR DETECTED: Negative length" << std::endl;
            return false; // Should fail
        }
        if (xmlContent.find("positie=\"-") != std::string::npos) {
            std::cout << "❌ ERROR DETECTED: Negative position" << std::endl;
            return false; // Should fail
        }
        
        std::cout << "✅ XML looks valid" << std::endl;
        return true; // Should pass
    }
    
    static void testXMLFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "❌ Could not open file: " << filename << std::endl;
            return;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        
        std::cout << "Testing: " << filename << std::endl;
        bool shouldPass = analyzeXML(content);
        
        if (shouldPass) {
            std::cout << "🎯 EXPECTED: Test should PASS" << std::endl;
        } else {
            std::cout << "🎯 EXPECTED: Test should FAIL" << std::endl;
        }
        std::cout << "---" << std::endl;
    }
    
    static void modifyXMLFile(const std::string& filename, const std::string& search, const std::string& replace) {
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            std::cout << "❌ Could not open file for modification: " << filename << std::endl;
            return;
        }
        
        std::stringstream buffer;
        buffer << inFile.rdbuf();
        std::string content = buffer.str();
        inFile.close();
        
        size_t pos = content.find(search);
        if (pos != std::string::npos) {
            content.replace(pos, search.length(), replace);
            std::cout << "🔧 Modified: " << search << " → " << replace << std::endl;
        }
        
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            std::cout << "❌ Could not write to file: " << filename << std::endl;
            return;
        }
        outFile << content;
        outFile.close();
    }
};

int main() {
    std::cout << "🚀 SMART XML TESTING DEMO" << std::endl;
    std::cout << "=========================" << std::endl;
    
    // Test 1: Bad XML with negative cycle
    std::cout << "\n📋 TEST 1: Bad XML (negative cycle)" << std::endl;
    SmartXMLTester::testXMLFile("tests/XML_Testfiles/04_tlightcycle_bad.xml");
    
    // Test 2: Good XML
    std::cout << "\n📋 TEST 2: Good XML" << std::endl;
    SmartXMLTester::testXMLFile("tests/XML_Testfiles/04_tlightcycle_ok.xml");
    
    // Test 3: Dynamic modification
    std::cout << "\n📋 TEST 3: Dynamic XML Modification" << std::endl;
    std::cout << "Before modification:" << std::endl;
    SmartXMLTester::testXMLFile("tests/XML_Testfiles/04_tlightcycle_bad.xml");
    
    // Modify the XML to make it valid
    SmartXMLTester::modifyXMLFile("tests/XML_Testfiles/04_tlightcycle_bad.xml", 
                                 "cyclus=\"-10\"", "cyclus=\"10\"");
    
    std::cout << "After modification:" << std::endl;
    SmartXMLTester::testXMLFile("tests/XML_Testfiles/04_tlightcycle_bad.xml");
    
    // Restore the original
    SmartXMLTester::modifyXMLFile("tests/XML_Testfiles/04_tlightcycle_bad.xml", 
                                 "cyclus=\"10\"", "cyclus=\"-10\"");
    
    std::cout << "After restoration:" << std::endl;
    SmartXMLTester::testXMLFile("tests/XML_Testfiles/04_tlightcycle_bad.xml");
    
    std::cout << "\n✅ DEMO COMPLETE!" << std::endl;
    std::cout << "The smart tester automatically adapts to XML changes!" << std::endl;
    
    return 0;
} 