# Grafika Komputerowa
### Konrad Brzózka

## Opis projektu
Projekt z grafiki 3D pozwala prowadzić samochód w mieście. Pozwala na oglądanie sceny z perpsektyw 2 kamer: swobodnej
oraz podążającej za obiektem (autem). Istnieje możliwośc wyboru metod cieniowania: model Phonga i model Gourauda.
Zaimplementowane są 3 rodzaje świateł: kierunkowe, punktowe oraz reflektor. Scena zawiera także płynnie zmieniającą się
mgłę w kolorze nieba.

## Rozwiązania
Projekt jest wykonany przy użyciu technologii OpenGL w wersji 4.5. Dodatkowo wykorzystuje biblioteki GLAD, GLFW oraz GLM.
Tekstury sa ładowane za pomocą std_image, natomiast modele za pomocą biblioteki Assimp.

## Sterowanie
Sterowanie autem odbywa się za pomocą klawiszy WASD. Kamerę można obracać przeciągając trzymająć prawy przycisk myszy.
Klawisze strzałek pozwalają na przemieszczanie się kamerą.