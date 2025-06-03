
 Projekt Manewrysta – Etap 1

 \section wstep Wstęp

 Niniejszy dokument opisuje pierwszy etap realizacji projektu **Manewrysta**.
 Zgodnie z harmonogramem wykonano podstawowe funkcjonalności oraz interfejs użytkownika
 w środowisku Qt z wykorzystaniem języka C++.

 \section funkcjonalnosci Zrealizowane funkcjonalności
 
  Stworzono **okno główne** aplikacji z graficznym interfejsem użytkownika.
 
 Dodano **wyświetlacze danych (QLCDNumber)** prezentujące symulowane wartości XYZ akcelerometru.

  Przycisk **Charts** otwiera nowe okno zawierające trzy wykresy danych w czasie rzeczywistym.

  Zaimplementowano uproszczoną **symulację działania akcelerometru** (klasa `AccSimulator`), generującą dane co 50 ms.

  Przycisk odpowiedzialny za **zmianę języka** zmienia swój wygląd po kliknięciu.

  Przycisk **Start** uruchamia lub zatrzymuje symulację, zmieniając przy tym kolor i napis przycisku.
 

