# Aplikacja do rejestracji i przetwarzania strumieni wideo

To repozytorium zawiera pliki projektowe do mojej pracy inżynierskiej - aplikacja została wykonana w języku C++, przy użyciu framework'a Qt. Ponadto użyto biblioteki OpenCV <br /> <br />
Program umożliwia:

  - podgląd obrazu z kamery wideo
  - zapis pojedynczej klatki wideo oraz pliku wideo
  - odtworzenie pliku wideo
  - zmianę ścieżki domyślnej dla zapisu/odczytu
  - filtrację obrazu w czasie rzeczywistym, wraz z możliwością zapisu plików wynikowych z ustawionym filtrem
  - ustawienie własnej maski do filtracji

Aplikacja dokonuje filtracji poprzez użycie maski konwolucji - umożliwiło to filtrację w 30 klatkach na sekundę wraz z zapisem nagrania. <br />

![10_test_customowego_filtra](https://github.com/user-attachments/assets/6eb1c9a0-ad26-4478-ad76-cc2ea34d0d38)


# Kompilacja

Żeby uruchomić program, potrzebne jest Qt w wersji 6.6.3, oraz biblioteka OpenCV skompilowana przy użyciu MingW z obsługą POSIX.


