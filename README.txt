ADD_LAST

- daca vectorul este NULL ii aloc spatiu => 5 octeti pentru headerul datei care se va introduce si data->header->len octeti pentru data efectiva
- daca vectorul nu este NULL realoc
- copiez tipul datei in vector
- copiez lungimea datei in vector
- copiez data efectiva in vector
- incrementez nr total de octeti ai vectorului cu nr de octeti ai datei tocmai inserate


ADD_AT

- daca indexul este mai mic decat 0 se iese
- parcurg vectorul pana la indexul indicat sau cat timp nr de octeti parcursi este mai mic decat nr total de octeti ai vectorului
- daca nr de octeti parcursi este mai mare decat nr total de octeti ai vectorului inseamna ca indexul nu se afla in vector si se adauga la final
- daca indexul este unul corect:
	- aloc spatiu datei ce va fi introdusa
	- mut datele ce se afla dupa index mai "la dreapta" pentru a putea introduce data in locul indicat
	- adaug data
	- incrementez nr total de octeti


FIND

- cauta doar daca indexul este unul valid
- parcurge vectorul pana la indexul indicat
- printeaza tipul si sare peste octetii headerului
- parcurge primul string pana intalneste '\0'
- in p memoreaza adresa numarului care se afla dupa primul string
- in functie de tip, atunci cand afiseaza va sari peste nr de octeti corespunzatori numerelor
- astfel, la tipul 2 al doilea string incepe de la adresa (p + 6) deoarece a sarit peste cei 2 octeti ai primului nr si cei 4 octeti ai celui de-al doilea nr

 -------------------------------------------
| tip | len | string1 | nr1 | nr2 | string2 |      
 -------------------------------------------
                      ^  ^     ^  ^
                      |  |     |  (p + 6)
                      p  |     |
                         |    4 bytes
                       2 bytes


DELETE

- parcurg vectorul pana la index
- retin nr de octeti ce vor fi stersi in l
- copiez elementele care se afla dupa index peste elementul care se afla la indexul indicat
- realoc memoria fara octetii stersi
- sterg octetii din nr total de octeti ai vectorului


PRINT

- acelasi rationament ca la find doar ca intr-un while
- la fiecare pas din while scad din len nr de octeti ai datei care a fost afisata
- la final len va ajunge la 0, semn ca toate datele din vector au fost afisate


CONVERT_TIP

- in functie de tip, datele care au fost citite de la tastatura vor fi adaugate la data_structure
- astfel, calculeaza cati octeti trebuie adaugati, aloca spatiu pentru data_tot (header + data), aloca spatiu pentru data, dupa aceea pentru header
- transforma suma in int8_t, int16_t sau int32_t in functie de tip
- adauga la data_tot primul string, prima suma, a doua suma, al doilea string
- adauga tipul si lungimea datei
- returneaza data creata


MAIN

- citesc linie cu linie, luand fiecare cuvant din linie
- verific ce task indica primul cuvant de pe linie si citesc specificatiile taskului
- la insert salvez dedicatorul, suma1, suma2 si dedicatul in stringuri, acestea urmand sa fie convertite in tipul data_structure