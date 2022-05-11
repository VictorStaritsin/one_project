#include <iostream>
#include <stack>
using namespace std;

struct num_oper //Структура, описывающая любое число или операцию
{
	char symbol; // 0 для чисел, "+" для операции сложения и т.д. (Stack_n)
	double number; //Значение (только для чисел). У операций значение всегда "0" (Stack_o)
};

bool Maths(stack <num_oper >& number, stack <num_oper >& operation, num_oper& item) { //Математическая функция, которая производит расчеты
	//Функция имеет тип bool, чтобы при возникновении какой-либо ошибки возвращать "false"
	double a, b, c;
	a = number.top().number; //Берется верхнее число из стека с числами 'a'
	number.pop(); //Удаляется верхнее число из стека с числами 'a'(чтобы взять второе число)
	switch (operation.top().symbol) {  //Проверяется тип верхней операции из стека с операциями
	case '+': //Если тип верхней операции из стека с операциями сложение
		b = number.top().number;//Берем верхнее число из стека с числами, которое уже является 'b'
		number.pop(); //Удаляется верхнее число из стека с числами 'b'(для того чтобы результат '+' положить обратно в стек)
		c = a + b;//сохраняем результат
		//чтобы занести значение 'c' в стэк item нужно прописать заново тип и значение
		item.symbol = '0';//тип
		item.number = c;//значение
		number.push(item); //Результат операции кладется обратно в стек с числами 'c'
		operation.pop();//Удаляется отработанная операция
		break;//останавливаем цикл 

	case '-':
		b = number.top().number;
		number.pop();
		c = b - a;//Меняем местами 'a' и 'b' для правильной работы стека
		item.symbol = '0';
		item.number = c;
		number.push(item);
		operation.pop();
		break;

	case '*':
		b = number.top().number;
		number.pop();
		c = a * b;
		item.symbol = '0';
		item.number = c;
		number.push(item);
		operation.pop();
		break;

	case '/':
		b = number.top().number;
		if (a == 0) {
			cout << "На 0 делить нельзя!";
			return false;
		}
		else {
			number.pop();
			c = (b / a); // Меняем местами 'a' и 'b' для правильной работы стека
			item.symbol = '0';
			item.number = c;
			number.push(item);
			operation.pop();
			break;
		}
	default://Проверка . есле ввели не '+,-,/,*' а какой-то другой симво, то выводим ошибку
		cout << "Ошибка!";
		return false;
		break;
	}
	return true;
}
int getRang(char Ch) { //Функция возвращает приоритет операции: "1" для сложения и вычитания, "2" для умножения и деления и т.д.
	if (Ch == '+' || Ch == '-')
		return 1;
	if (Ch == '*' || Ch == '/')
		return 2;
	else
		return 0;
}
int main()
{
	setlocale(LC_ALL, "rus");
	cout << "    Пограмма - калькулятор!" << endl;
	cout << "    Калькулятор решает такие задачи как * , / , - , + ." << endl;
	cout << "    И возможность воспользоваться ( ) ." << endl;
	cout << "    Прошу вводить выражение корректно" << endl;
	cout << "    Введи выражение и нажми Enter  \n";
	char Ch; //Переменная, в которую будет записываться текущий обрабатываемый символ
	double value;//переменная для числа
	bool flag = 1; //Нужен для того, чтобы программа поняла, что в выражении с переди стоит '-'
	stack <num_oper > number; //Стек с числами
	stack <num_oper > operation; //Стек с операциями
	num_oper  item; //Объект типа Leksema (инициализация элемента) 
	while (true)
	{
		Ch = cin.peek(); //Смотрим на первый символ//вроде коробочки со всеми возможными значениями; 
		if (Ch == '\n')
			break; //Если достигнут конец строки, выходим из цикла
		if (Ch == ' ') { //Игнорирование пробелов
			cin.ignore();
			continue;
		}
		if (Ch >= '0' && Ch <= '9' || Ch == '-' && flag == 1)//Если прочитано число
		{
			cin >> value;
			item.symbol = '0';    //инициализация числа
			item.number = value; //возьмет все числа в себя (24.3455 или 2345675)
			number.push(item); //Число помещается в стек с числами
			flag = 0;
			continue;
		}
		if (Ch == '+' || Ch == '-' && flag == 0 || Ch == '*' || Ch == '/') { //Если прочитана операция
			if (operation.size() == 0) { //Если стек с операциями пуст
				item.symbol = Ch;
				item.number = 0; //операция никакого значения не предпологает
				operation.push(item); //Результат операции кладется в стек с операциями (возвращается на вершину)
				cin.ignore(); //для игнорирования одного символа, чтобы заглянуть снова в cin.peek()
				continue; //переходим к следущей итерации (расфасовка)
			}
			if (operation.size() != 0 && getRang(Ch) > getRang(operation.top().symbol)) { //Если стек с операциями НЕ пуст, но приоритет текущей операции выше
				item.symbol = Ch;                                                     //верхней операции в стеке с операциями
				item.number = 0;
				operation.push(item); //Операция кладется в стек с операциями 
				cin.ignore();
				continue;
			}
			if (operation.size() != 0 && getRang(Ch) <= getRang(operation.top().symbol)) {//Если стек с операциями НЕ пуст, но приоритет текущей операции ниже 
																					//либо равен верхней операции в стеке с операциями
				if (Maths(number, operation, item) == false) { //Если функция вернет "false", то прекращаем работу
					return 0;
				}
				continue;
			}
		}
		if (Ch == '(') { //Если прочитана открывающаяся скобка
			item.symbol = Ch;
			item.number = 0;
			operation.push(item); //Операция кладется в стек с операциями
			cin.ignore();
			continue;
		}
		if (Ch == ')') { //Если прочитана закрывающаяся скобка не кладем в стек
			while (operation.top().symbol != '(')
			{
				if (Maths(number, operation, item) == false) { //Если функция вернет "false", то прекращаем работу
					return 0;
				}
				else continue; //Если все хорошо	
			}
			operation.pop();
			cin.ignore();//для того, чтобы перейти к следущей операции. когда посчитали выражение в скобках
			continue;
		}
		else { //Если прочитан какой-то странный символ
			cout << "\nНеверно введено выражение!\n";
			return 0;
		}
	}
	while (operation.size() != 0) //Вызываем матем. функцию до тех пор, пока в стеке с операциями не будет 0 элементов
	{
		if (Maths(number, operation, item) == false) { //Если функция вернет "false", то прекращаем работу
			return 0;
		}
		else continue; //Если все хорошо	
	}
	cout << "   Ответ: " << number.top().number << endl; //Выводим ответ
	return 0;
}