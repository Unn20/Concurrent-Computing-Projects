1.1.1. Czytelnicy i pisarze

Zadanie polega na realizacji problemu czytelników i pisarzy (opis wersji klasycznej w Wikipedii), przy czym:

    jest ustalona liczba procesów — N;
    każdy proces działa naprzemiennie w dwóch fazach: fazie relaksu i fazie korzystania z czytelni;
    w dowolnym momencie fazy relaksu proces może (choć nie musi) zmienić swoją rolę: z pisarza na czytelnika lub z czytelnika na pisarza;
    przechodząc do fazy korzystania z czytelni proces musi uzyskać dostęp we właściwym dla swojej aktualnej roli trybie;
    pisarz umieszcza efekt swojej pracy — swoje dzieło — w formie komunikatu w kolejce komunikatów, gdzie komunikat ten pozostaje do momentu, aż wszystkie procesy, które w momencie wydania dzieła były w roli czytelnika, nie odczytają go (po odczytaniu przez wszystkie wymagane procesy komunikat jest usuwany);
    pojemność kolejki komunikatów — reprezentującej półkę z książkami — jest ograniczona, tzn. nie może ona przechowywać więcej niż K dzieł;
    podczas pobytu w czytelni proces (również pisarz) czyta co najwyżej jedno dzieło, po czym czytelnik opuszcza czytelnię, a pisarz czeka na miejsce w kolejce, żeby opublikować kolejne dzieło.




Kompilowanie pliku odbywa się poprzez wpisanie polecenia: make

Uruchomienie programu: ./main.o [N] [K]
Parametry N i K sa opcjonalne, domyślne wartości to N=5 i K=10
Można stworzyć maksymalnie 100 procesów (0>N>100) i ustawić maksymalną liczbę książek 160 (0>K>160)

Plik ipc.c zawiera funkcje P() i V() przeznaczone do podnoszenia i opuszczania semaforu

W pliku entity.c umieszczone zostały funkcje: writer() dla pisarza i reader() dla czytelnika

Plik main.c zawiera incicjalizację elementów IPC oraz sterowanie procesami.
