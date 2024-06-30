# Systemy Operacyjne 2 - projekt

### _Automaty - przekąski i napoje dla pracowników_
W siedzibie firmy _GigaCode_, zdecydowano się zainstalować `automaty` z kawą. W tym celu firma zrwóciła się do pośrednika _DispenseMe_, która dostarcza tego typu maszyny i oferuje ich stałą obsługę. _GigaCode_ umieściła automaty w różnych miejscach siedziby.

W ramach przerwy od pracy z `automatu` skorzystać może dowolny `pracownik` _GigaCode_. Jeśli `automat` jest wolny, to `pracownik` może natychmiast zaspokoić swoje pragnienie i wrócić do pracy. Jeżeli jednak `automat` jest zajęty przez innego `pracownika` lub jest uzupełniany przez `dostawcę`, to `pracownik` musi poczekać na swoją kolej. Korzystając z `automatu`, `pracownik` może zauwazyć, że brakuje w nim kawy. Wówczas powinien on odczekać aż `dostawca` sprawdzi stan `automatu`, a następnie uzupełni braki.

W magazynie _DispenseMe_ znajduje się nieskończona ilosć kawy, z która przechowywana jest na wydzieloneuj do tego `półce`. Ze względu na ograniczoną przestrzeń w danym momencie tylko jeden `dostawca` zatrudniony w _DispenseMe_ może ściągnąć kawę z tej `półki`. Dodatkowo każdy `dostawca` w jednym momencie przenieść może wyłącznie ograniczoną ilość kawy.

#### Wątki
Wątkami w opisanym scenariuszu są:
* `pracownicy` firmy _Gigacode_
* `dostawcy` firmy _DispenseMe_

#### Zasoby
Współdzielonymi zasobami w opisanym scenaiuszuszu są:
* **automaty** rozmieszczone w siedzibie _GigaCode_, do których dostęp mają wszystkie wątki **pracowników** oraz **dostawców**
* **półka** z kawą w magazynie _DispenseMe_, do której dostęp mają wszystkie wątki **dostawców**

#### Sekcje krytyczne
W opisanym scenariuszu występować będą następujące sekcje krytyczne:
* **sekcje dostępu do automatów** współdzielone przez wszystkie wątki
* **sekcja dostępu do półki** współdzielona przez wszystkie wątki `dostawców`

#### Zmienne warunkowe
W opisywanym scenariuszu zmienne warunkowe będą występować przy:
* **sekcji krytycznej automatów**, pomagając zrealizować odczekanie `pracownika` na uzupełnienie **automatu** o brakującą kawę (wybudzająca wątek `pracownika`, gdy liczba kawy w `automacie` jest większa niż 0)

#### Potencjalne problemy
W przypadku błędnej implementacji opisywanego scenariusza wystąpić mogą następujące problemy:
1. **Wyścig** w przypadku, gdy dwa dowolne wątki będą mieć jednoczesny dostęp do tego samego `automatu`.
2. **Zagłodzenie** wątków `pracowników` w **sekcji krytycznej automatu**. Może ono wystąpić po: wyczerpaniu się kawy w `automacie` i zbyt powolnym uzupełnianiu braku przez `dostawców`, braku poinformowania `pracowników` o uzupełnieniu braku bądź braku zwolnienai `automatu` przez `pracownika` po zauważeniu braku lub pobraniu kawy. Przy czym powolne uzupełnienie braku może wynikać z:
    * Nieoptymalnego schematu uzupełniania `automatów`. Przykładowo: wysłania zbyt wielu `dostaców` do uzupełnienia tego samego `automatu` w momencie, gdy inny `automat` również jest pusty. Wówczas większość `dostawców` prawdopodobnie oczekuje bezsensowanie na dostęp do pierwszego `automatu`, bo ten zostanie uzupełniony przed nadejściem ich kolejki. 
    * Nieoptymalnego schematu sprawdzania `automatów`. Przykładowo: sprawdzania stanu tego samego `automatu` przez wielu `dostawców` w krótkich odstępach czasu, gdy inny `automat` jest pusty.
3. **Zakleszczenie** wątku `pracownika` oraz `dostawcy` w **sekcji krytycznej automatu**. Wystąpi ono, gdy `pracownik` nie zwolni `automatu` po zauważeniu braku produktu, przez co `dostawca` nie będzie mieć możliwości sprawdzenia stanu `automatu` i uzupełnienia braku.
4. **Zagłodzenie** wątku `dostawcy` w **sekcji krytycznej automatu**. Wystąpi ono, gdy wiele wątków będzie korzystać z `automatu`, którego stan `dostawca` chce sprawdzić.
