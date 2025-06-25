# Traffic Simulation Test Suite

## Nieuwe Test Methode

De testmethode is aangepast om gebruik te maken van bestandsvergelijking in plaats van try-catch blokken. Dit maakt de tests preciezer en betrouwbaarder.

### Bestandsstructuur

```
tests/
├── Expected_Testfiles/          # Verwachte error output bestanden
│   ├── expected_01_basic_bad.txt
│   ├── expected_02_roadlength_bad.txt
│   └── ...
├── XML_Testfiles/              # XML test bestanden
│   ├── 01_basic_bad.xml
│   ├── 02_roadlength_bad.xml
│   └── ...
├── test_files/                 # Originele test bestanden
├── run_tests.bat               # Windows batch script
├── run_tests.ps1               # PowerShell script
└── tests.cpp                   # Test implementatie
```

### Hoe het werkt

1. **compare2 functie**: Vergelijkt de output van een XML bestand met een verwacht error bestand
2. **Normalisatie**: Verwijdert whitespace verschillen en normaliseert line endings
3. **Output capture**: Vangt zowel stdout als stderr op, inclusief REQUIRE violations
4. **Precieze vergelijking**: Vergelijkt regel voor regel de genormaliseerde output

### Test Classes

- **TrafficSimulationTest**: Originele tests (behouden voor backward compatibility)
- **XMLParseInValid**: Nieuwe tests die gebruik maken van de compare2 methode

### Voorbeeld Test

```cpp
TEST_F(XMLParseInValid, BasicBadXml) {
    compare2(
        "../tests/Expected_Testfiles/expected_01_basic_bad.txt",
        "../tests/XML_Testfiles/01_basic_bad.xml"
    );
}
```

### REQUIRE Violations

De DesignByContract.h is aangepast om REQUIRE violations naar stderr te sturen in plaats van assert te gebruiken. Dit maakt het mogelijk om de violations op te vangen in de tests.

### Voordelen van de nieuwe methode

1. **Preciezer**: Vergelijkt exacte error berichten
2. **Betrouwbaarder**: Geen afhankelijkheid van try-catch timing
3. **Uitbreidbaar**: Makkelijk nieuwe test cases toevoegen
4. **Debugbaar**: Duidelijke output bij verschillen
5. **Consistent**: Zelfde aanpak voor alle error types

### Tests Uitvoeren

#### Windows (Batch)
```cmd
cd tests
run_tests.bat
```

#### Windows (PowerShell)
```powershell
cd tests
.\run_tests.ps1
```

#### Linux/Mac
```bash
cd tests
g++ -std=c++17 -I../src -I../tinyxml -I../gtest/include -L../gtest/lib -lgtest -lgtest_main -lpthread ../src/*.cpp ../tinyxml/*.cpp tests.cpp -o test_runner
./test_runner
```

### Voorbeeld: Extra Fout Toevoegen

Om te demonstreren hoe de tests falen wanneer je een extra fout toevoegt:

1. **Voeg een extra REQUIRE toe** in `src/Road.cpp`:
```cpp
void Road::addVehicle(Vehicle* vehicle) {
    REQUIRE(vehicle != nullptr, "Vehicle cannot be null");
    REQUIRE(vehicle->getPosition() >= 0, "Vehicle position must be non-negative"); // Nieuwe check
    // ...
}
```

2. **Maak een test bestand** dat de nieuwe fout veroorzaakt:
```xml
<BAAN>
    <naam>Main</naam>
    <lengte>500</lengte>
</BAAN>
<VOERTUIG>
    <baan>Main</baan>
    <positie>-10</positie>  <!-- Negatieve positie -->
    <type>auto</type>
</VOERTUIG>
```

3. **Maak een verwacht error bestand**:
```
REQUIRE Violation: Vehicle position must be non-negative in Road.cpp:58
```

4. **Voeg een test toe**:
```cpp
TEST_F(XMLParseInValid, NegativeVehiclePosition) {
    compare2(
        "../tests/Expected_Testfiles/expected_test_negative_vehicle_position.txt",
        "../tests/XML_Testfiles/test_negative_vehicle_position.xml"
    );
}
```

Nu zal de test falen als de extra REQUIRE violation optreedt, wat aantoont dat de testmethode gevoelig is voor nieuwe fouten.

### Nieuwe Test Toevoegen

1. Maak een XML test bestand in `XML_Testfiles/`
2. Maak een verwacht error bestand in `Expected_Testfiles/`
3. Voeg een test toe aan de `XMLParseInValid` class
4. Gebruik de `compare2` functie om de bestanden te vergelijken 