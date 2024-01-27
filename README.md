
Sprawozdanie projektu –  Sieci Komputerowe II

Adrian Witoński

nr indeksu: 151 848

semsetr V informatyka

# Krótki opis projektu

Projekt realizuje grę w szachy online przy pomocy języka c/c++ I gniazd BSD. W projekcie zawarto serwer TCP działający w trybie konsolowym oraz okienkowy program klienta. Serwer potrafi obsługiwać wiele równoległych rozgrywek,  a także  umożliwia graczą tworzenie własnych rozgrywek,  w celu grania z konkretnym graczem. 

# Działanie serwera

Serwer tworzy gniazdo I je wiąże, a następnie nasłuchuje czekając za graczami. Przy przyłączaniu gracza serwer dwukrotnie wykonuje recv() w celu pobrania informacji o nazwie gracza I opcje gry.. Możliwe są trzy opcje: nowa gra losowa (graczowi zostanie dobrany kolejny gracz wybierający tę opcję), stworzenie gry (gracz zostanie umieszczony w puli graczy czekających na dołączenie do nich innego gracza) I dołączenie do gry (serwer wykonuje jeszcze jeden recv() w celu pobrania nazwy gracza do którego chce dołączyć dołączający gracz I tworzy grę z takiej dwójki graczy) . Po poprawnych dobraniu dwóch graczy tworzona jest nowa gra I cała kontrola nad grą oraz połączeniem z graczami przekazywana jest dwóm nowym wątkom, po jednym dla każdego gracza. Każdy z dwóch nowych wątków odbiera poprzez recv() wiadomości od danego gracza. Następnie weryfikuje przesłaną przez gracza informacje z aktualnym stanem gry I ewentualnie wykonuje dalsze działanie. Jako że gracz może ciągle wysyłać wiadomości o chęci wykonania dangeo posunięcia, to po stronie wątku serwera następuje weryfikacja posunięcia I ruch. Jeżeli gracz prześle odpowiednie posunięcie (zaznaczy własną figurę I wybierze poprawne pole do ruchu) oraz wykona to w swojej turze, wtedy następuje ruch w grze I wątek serwera odsyła do obu graczy nowe ułożenie figur. Po każdym ruchu sprawdzane jest również, czy gra nie dobiegła końca. Jeżeli tak się stanie, zostaje wysłana do każdego z dwóch grających graczy specjalna informacja od serwera informująca o zakończeniu rozgrywki. Wtedy też po stronie serwera gracze są usuwani z listy aktywnych graczy, gra jest usuwana z listy gier, a wątki kończą swoje działanie. W przypadku, gdy jeden z graczy w trakcie rozgrywki opuści grę, wątek wysyła do drugiego gracza specjalną wiadomość, dając znać graczowi, że wygrał I rozgrywka dobiegła końca.


# Działanie klienta

Po uruchomieniu programu klienta, graczowi wyświetlane jest okno, w którym wpisuje swoją nazwę I wybiera opcję gry. Gracz może dołączyć do losowej gry (wtedy dobierany jest mu gracz, który również wybierze tę opcję), stworzyć nową grę albo dołączyć do już istniejącej gry (wtedy gracz uzupełnia jeszcze jedno pole podając nazwę gracza, do którego chce dołączyć. Po wciśnięciu odpowiedniego przycisku klient łączy się z serwerem I po udanym połączeniu wykonuje dwukrotnie send() wysyając do serwera nazwę gracza oraz opcję gry, którą ów gracz wybrał. Jeżeli gracz wybrał opcję dołączenia do gry to klient wysyła dodatkową informację z nazwą gracza. Następnie gracz czeka na rozpoczęcie rozgrywki I otrzymanie od serwera informacji o ułożeniu figur na szachownicy. Ruch figury odbywa się poprzez naciśnięcie na pole z figurą I wciśnięcie dowolnego innego pola. Klient nie weryfikuje poprawności ruchu gracza, przesyła jedynie, przy pomocy osobnego wątku, informacje o dwóch wciśniętych polach. Dopiero serwer weryfikuje poprawność ruchu I sprawdza, czyja jest tura na ruch. W błędnego ruchu nic się nie dzieje, a w przypadku poprawnego ruchu, gracz otrzymuje od serwera informacje o nowym ułożeniu figur. Taką informację klient odpowiednio przetwarza I wyświetla figury na szachownicy w nowym ułożeniu. W przypadku otrzymania specjalnej informacji od serwera (gdy następuje szach mat albo przeciwnik rozłączy się z serwerem)  klient blokuje możliwość wciskania pól, informuje gracza o zakończeniu rozgrywki, rozłącza się z serwerem I wyświetla graczowi początkowe okno. 

# Chess API

W programie użyto własnej implementacji gry w szachy. Serwer tworzy obiekt game klasy chess_game w dołączonej bibliotece ‘chess.h’ I korzysta z wbudowanych w klasie metod, aby  m.in. badać poprawność ruchu, wykrywać koniec rozgrywki, czy odczytywac stan gry. 

# Widok okna klienta


![image](https://github.com/Adrix0n/chess_QT_GUI/assets/99897531/df4cac9e-1df5-414d-be2a-6fb3ac33a534)

![image](https://github.com/Adrix0n/chess_QT_GUI/assets/99897531/376d21ad-9c34-4c5d-b5ee-aad2e16d820b)


