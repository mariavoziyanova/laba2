// Консольная программа для демонстрации

#include <fcntl.h>

#include <chrono>
#include <complex>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cwchar>

#include "arraysequence.h"
#include "common.hpp"
#include "linkedlist.h"
#include "linkedlistsequence.h"
#include "menu.h"
#include "stack.h"

// Функиия, которую можно применить к каждому элементу последовательности
template <class T>
T map_function(T x) {
  return x * x;
}

wstring map_function(wstring x) {
  return x + L"!";
}

// Фильтры для разных типов данных. Для каждого типа - своя
bool where_function(int x) {
  bool result = (x % 2) == 0;
  wcout << L"  where: число " << x << L" чётное: " << result << endl;
  return result;
}

bool where_function(double x) {
  bool result = abs(x) < 5;
  wcout << L"  where: число |" << x << L"| < 5: " << result << endl;
  return result;
}

bool where_function(complex<double> x) {
  bool result = abs(x) < 5;
  wcout << L"  where: число |" << x << L"| < 5: " << result << endl;
  return result;
}

bool where_function(const wstring x) {
  bool result = x.size() < 4;
  wcout << L"  where: длина строки " << x << L" < 4: " << result << endl;
  return result;
}

// Сложение для всех чисел (конкатенация для строк)
template <class T>
T reduce_function(T a, T b) {
  T result = a + b;
  wcout << L"  reduce: " << a << L" + " << b << " = " << result << endl;
  return result;
}

template <class T>
void apply_map_where_reduce() {
  wprintf(L"Применение функций map, where, reduce - ручной ввод данных\n");

  Stack<T> stack(new LinkedListSequence<T>, L"Ввод данных стека");  // Стек из элементом типа T

  wprintf(L"Применяем операцию map\n");
  Stack<T> *mapRes = stack.map(map_function);
  mapRes->print();  // Печатаем содержимое стека
  delete mapRes;    // Очищаем память

  wprintf(L"Применяем операцию where\n");
  Stack<T> *whereRes = stack.where(where_function);
  whereRes->print();
  delete whereRes;  // Очищаем память

  wprintf(L"Применяем операцию reduce - сложение для всех чисел (конкатенация для строк)\n");
  T reduceRes = stack.reduce(reduce_function);
  wcout << L"Результат reduce: " << reduceRes << endl << endl;

  wprintf(L"\n");
}

template <class T>
void stack_concat() {
  wprintf(L"Конкатенация двух стеков\n");

  try {
    Stack<T> stack1(new LinkedListSequence<T>, L"Ввод элементов первого стека");
    Stack<T> stack2(new LinkedListSequence<T>, L"Ввод элементов второго стека");

    Stack<T> *result = stack1.concat(stack2);
    result->print();
    delete result;

    wprintf(L"\n");
  } catch (IndexOutOfRange &ex) {
    wcout << L"Exception: " << ex.what() << endl << endl;
  }
}

template <class T>
void stack_getSubSequence() {
  wprintf(L"Извлечение подпоследовательности (по заданным индексам)\n");
  try {
    Stack<T> stack(new LinkedListSequence<T>, L"Ввод элементов стека");
    wcout << L"Введите начальный индекс: ";
    int startIndex;
    wcin >> startIndex;
    wcout << L"Введите конечный индекс: ";
    int endIndex;
    wcin >> endIndex;

    Sequence<T> *result = stack.getSubsequence(startIndex, endIndex);
    result->print();
    delete result;

    wcout << endl;
  } catch (IndexOutOfRange &ex) {
    wcout << L"Exception: " << ex.what() << endl << endl;
  }
}

template <class T>
void stack_findSubSequence() {
  wprintf(L"Поиск на вхождение подпоследовательности\n");

  try {
    LinkedListSequence<T> linkedListSequence;
    Stack<T> stack(new LinkedListSequence<T>, L"Ввод элементов стека");
    Stack<T> subSequence(new LinkedListSequence<T>, L"Ввод подпоследовательности");

    int index = stack.findSubsequence(subSequence);
    wcout << L"Позиция подполедовательности = " << index << endl << endl;
  } catch (IndexOutOfRange &ex) {
    wcout << L"Exception: " << ex.what() << endl << endl;
  }
}

// Замеряем время работы стека
void stackImplementationSpeed(Sequence<int> *sequence) {
  auto begin = chrono::steady_clock::now();  // Засекаем начало работы

  Stack<int> stack(sequence);  // Создаём стек
  const int numbers = 20000;   // Добавим числа
  wcout << L"Количество элементов для тестирования: " << numbers << endl;
  for (int i = 1; i <= numbers; i++) {
    stack.push(i);
  }

  auto end = chrono::steady_clock::now();  // Конец работы
  auto elapsed_mcs = chrono::duration_cast<chrono::microseconds>(end - begin);
  // Вычисляем разницу в секундах времени начала и окончания работы
  const double t = elapsed_mcs.count() / 1e6;
  // Выводим результат в секундах на экран (в консоль)
  wcout << typeid(stack).name() << " time = " << t << endl;
}

template <class T>
void stack_addElementSpeed() {
  wprintf(L"Сравнение времени добавления элементов в стек на основе LinkedList и DynamicArray\n");
  stackImplementationSpeed(new LinkedListSequence<int>());
  stackImplementationSpeed(new ArraySequence<int>());
}

#define PRINT(x) wcout << #x << L" = " << x << endl

// Основные операции с выбранным типом данных
template <class T>
void main_menu() {
  MenuItem menu[] = {
    {L"Применение функции map, where, reduce - ручной ввод данных", apply_map_where_reduce<T>},
    {L"Конкатенация двух стеков", stack_concat<T>},
    {L"Извлечение подпоследовательности (по заданным индексам)", stack_getSubSequence<T>},
    {L"Поиск на вхождение подпоследовательности", stack_findSubSequence<T>},
    {L"Сравнение времени добавления элементов в стек на основе LinkedList и DynamicArray", stack_addElementSpeed<T>}
  };
  menuLoop(L"Возможные операции", _countof(menu), menu);
}

class A {
 public:
  virtual int f(int a, int b);
  virtual int f(int a) {
    return f(a, 2);
  }
};

class B : public A {
 public:
  int f(int a, int b) override {
    return a + b;
  };
};

// Основная программа
int main() {
  // Задаём кодировку UTF-16 для всего вывода в программе
  // Все символы и строки будут wchar_t
#if WIN32 || WIN64
  _setmode(_fileno(stdout), _O_U16TEXT);
  _setmode(_fileno(stdin), _O_U16TEXT);
  _setmode(_fileno(stderr), _O_U16TEXT);
#endif
  wprintf(L"== Тестирование операций ==\n");

  MenuItem menu[] = {{L"Целые числа (int)", main_menu<int>},
    {L"Вещественные числа (double)", main_menu<double>},
    {L"Комплексные числа (complex<double>)", main_menu<complex<double>>},
    {L"Строки/символы (string)", main_menu<wstring>}
  };
  try {
    menuLoop(L"Выберите тип элементов", _countof(menu), menu);
  } catch (IndexOutOfRange &ex) {
    wcout << L"Exception: " << ex.what() << endl << endl;
  }
}
