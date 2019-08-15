# multithread_bruteforce

Многопоточная программа поиска пароля путем полного перебора всех вариантов для всех доступных длин пароля. Пароль состоит из строчных и заглавных букв, а также цифр от 0 до 9 в кодировке ASCII. Программа позваляет выбрать число потоков для паралельного перебора значений, каждый поток осуществляет перебор в своем диапазоне. Потоки от 1 до n-1 выполняют одинаковый объем операций перебора, последний поток n выполянет тот же объем или больший.

A multi-threaded password search program by exhaustive search of all options for all available password lengths. The password consists of lowercase and uppercase letters, as well as numbers from 0 to 9 in ASCII encoding. The program allows you to select the number of threads for parallel enumeration of values, each thread enumerates in its own range. Threads from 1 to n-1 perform the same amount of brute force operations; the last thread n will execute the same amount or more.
