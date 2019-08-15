#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

const int PASS_LEN = 5;

void createTable();
bool checkPass();
void nextValue(int threadNum, string& assumtion, int pos, string& stop, long& counter);
void bruteForce(int threadNum);
void showTime();

vector<char> asciiSymbols; // symbols 0-9 A-z a-z

string password = "";
int nThreads;
bool found = false;

int main()
{
	// создаем вектор всех доступных символов для пароля
	createTable();

	// Ввод пароля не более чем PASS_LEN символов
	cout << "Please enter password from, max pasword length = " << PASS_LEN << ",\n"
		<< "password consists of numbers 0-9 and letters A-Z a-z\n";
	do 
	{
		cout << "password: ";
		cin >> password;
		if (!checkPass())
			cout << "Incorrect password, try again\n";
	} while (!checkPass());

	// просим  указать число потоков для решения задачи
	cout << "\nPlease enter the number of threads (1 - " << asciiSymbols.size() - 1 << ")\n";
	for(;;)
	{
		cout << "Threads: ";
		cin >> nThreads;

		if (!(nThreads > 0 && nThreads < asciiSymbols.size()))
			cout << "Incorrect value, try again\n";
		else
			break;

	} 

	// генерация потоков
	vector<thread> threads;
	for (int i = 1; i <= nThreads; ++i)
		threads.push_back(thread(bruteForce, i));

	showTime();

	// ожидание завершения всех потоков
	for (auto& th : threads) th.join();

	return 0;
}

// функция проверки введенного пароля на соответсвие требованиям длины
// и допустимых символов
bool checkPass()
{
	if (password.length() > PASS_LEN) return false;


	bool f = false;
	for (size_t i = 0; i < password.length(); i++)
	{
		f = false;
		for (auto iter = asciiSymbols.begin(); iter < asciiSymbols.end(); iter++)
		{
			if (password[i] == *iter)
			{
				f = true;
				break;
			}
		}
		if (!f)
			return false;
	}
	return true;
}

// рекурсивная функция перебора всех сочетаний с учетом повторений
void nextValue(int threadNum, string& assumtion, int pos, string& stop, long& counter)
{
	if (pos == assumtion.length())
	{
		if (assumtion == password)
		{
			found = true;
			cout << "Thread number " << threadNum << " found password ("
				<< assumtion << ") for " << counter << " steps\n";
			return;
		}
		if (assumtion == stop)
			return;
		return;
	}
	// для того чтобы каждый поток мог начинать перебор начиная с некотрой последовальности
	// котрыую другие потоки не будут рассматривать
	if (pos == 0)
	{
		int x = (threadNum - 1) * (asciiSymbols.size() / nThreads);
		for (size_t i = x; i < asciiSymbols.size(); i++)
		{
			assumtion[pos] = asciiSymbols[i];
			counter++;
			nextValue(threadNum, assumtion, pos + 1, stop, counter);

			if (found == true)
				return;
		}
	}
	else
	{
		for (size_t i = 0; i < asciiSymbols.size(); i++)
		{
			assumtion[pos] = asciiSymbols[i];
			counter++;
			nextValue(threadNum, assumtion, pos + 1, stop, counter);

			if (found == true)
				return;
		}
	}
	return;
}

void bruteForce(int threadNum)
{
	long counter = 0;

	// запуск полного перебора для всех возможных ждин пароля от 1 до PASS_LEN
	for (size_t i = 1; i <= PASS_LEN; i++)
	{
		string assumption(i, ' ');
		// предельная последовательность символов пароля до которой поток осуществялет перебор
		string stop = assumption; 

		if (threadNum < nThreads)
		{
			stop[0] = asciiSymbols[threadNum * asciiSymbols.size() / nThreads - 1];
			for (size_t j = 1; j < i; j++)
				stop[j] = asciiSymbols.back();
		}
		else
		{
			for (size_t j = 0; j < i; j++)
				stop[j] = asciiSymbols.back();
		}

		nextValue(threadNum, assumption, 0, stop, counter);
	}
}

// отображение длительности выполненения перебора пароля в секундах
// \r - возвращает картку в начало строки для дальнейшей перезаписи
void showTime()
{
	int seconds = 0;
	for (;;)
	{
		if (found == true)
			return;

		this_thread::sleep_for(1s);
		seconds++;
		cout << "time: " << seconds << "s\r";
	}
}

// Предполагая, что пароль может состоять только из цифр
// и букв строчных или заглавных, формируем таблицу(вектор) допустимых символов
void createTable()
{
	asciiSymbols.clear();

	for (size_t i = '0'; i <= '9'; i++)
		asciiSymbols.push_back(char(i));

	for (size_t i = 'A'; i <= 'Z'; i++)
		asciiSymbols.push_back(char(i));

	for (size_t i = 'a'; i <= 'z'; i++)
		asciiSymbols.push_back(char(i));
}