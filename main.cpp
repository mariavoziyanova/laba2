// Консольная программа для демонстрации

#include <fcntl.h>

#include <chrono>
#include <complex>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cwchar>
#include <fstream>

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

wstring map_function(wstring x) { return x + L"!"; }

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
      {L"Сравнение времени добавления элементов в стек на основе LinkedList и DynamicArray", stack_addElementSpeed<T>}};
  menuLoop(L"Возможные операции", _countof(menu), menu);
}

class A {
 public:
  virtual int f(int a, int b);
  virtual int f(int a) { return f(a, 2); }
};

class B : public A {
 public:
  int f(int a, int b) override { return a + b; };
};

void graph() {
  //    {L"Применение функции map, where, reduce - ручной ввод данных", apply_map_where_reduce<T>},
  //    {L"Конкатенация двух стеков", stack_concat<T>},
  //    {L"Извлечение подпоследовательности (по заданным индексам)", stack_getSubSequence<T>},
  //    {L"Поиск на вхождение подпоследовательности", stack_findSubSequence<T>},
  //    {L"Сравнение времени добавления элементов в стек на основе LinkedList и DynamicArray", stack_addElementSpeed<T>}
  ofstream concat_list(R"(C:\Users\vvago\CLionProjects\gtest-lab2\graphics\concat_list.txt)", ios_base::trunc);
  int n = 10000;

  for (int i = n / 100; i < n; i += n / 100) {
    int values1[i / 2];
    int values2[i / 2];
    for (int j = 0; j < i / 2; j++) {
      values1[j] = rand() % n;
      values2[j] = rand() % n;
    }
    Stack<int> stack1(new LinkedListSequence<int>(values1, i / 2));
    Stack<int> stack2(new LinkedListSequence<int>(values2, i / 2));
    auto startTime = chrono::steady_clock::now();
    stack1.concat(stack2);
    auto endTime = chrono::steady_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    concat_list << i << " " << time << "\n";
  }
  concat_list.close();

  ofstream concat_array(R"(C:\Users\vvago\CLionProjects\gtest-lab2\graphics\concat_array.txt)", ios_base::trunc);

  for (int i = n / 100; i < n; i += n / 100) {
    int values1[i / 2];
    int values2[i / 2];
    for (int j = 0; j < i / 2; j++) {
      values1[j] = rand() % n;
      values2[j] = rand() % n;
    }
    Stack<int> stack1(new ArraySequence<int>(values1, i / 2));
    Stack<int> stack2(new ArraySequence<int>(values2, i / 2));
    auto startTime = chrono::steady_clock::now();
    stack1.concat(stack2);
    auto endTime = chrono::steady_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    concat_array << i << " " << time << "\n";
  }
  concat_array.close();

  // getSubSequence list
  ofstream getsub_list(R"(C:\Users\vvago\CLionProjects\gtest-lab2\graphics\getSubSequence_list.txt)", ios_base::trunc);
  int values_list[n];
  for (int j = 0; j < n; j++) {
    values_list[j] = rand() % n;
  }
  Stack<int> stack_list(new LinkedListSequence<int>(values_list, n));
  for (int i = n / 100; i < n / 2; i += n / 100) {
    auto startTime = chrono::steady_clock::now();
    stack_list.getSubsequence(n / 2 - i, n / 2 + i);
    auto endTime = chrono::steady_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    getsub_list << 2 * i << " " << time << "\n";
  }
  getsub_list.close();
  // getSubSequence array
  ofstream getsub_array(R"(C:\Users\vvago\CLionProjects\gtest-lab2\graphics\getSubSequence_array.txt)", ios_base::trunc);
  int values_array[n];
  for (int j = 0; j < n; j++) {
    values_array[j] = rand() % n;
  }
  Stack<int> stack_array(new ArraySequence<int>(values_array, n));
  for (int i = n / 100; i < n / 2; i += n / 100) {
    auto startTime = chrono::steady_clock::now();
    stack_array.getSubsequence(n / 2 - i, n / 2 + i);
    auto endTime = chrono::steady_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    getsub_array << 2 * i << " " << time << "\n";
  }
  getsub_array.close();

  // map,where,reduce list
  ofstream map_list(R"(C:\Users\vvago\CLionProjects\gtest-lab2\graphics\map_list.txt)", ios_base::trunc);
  ofstream where_list(R"(C:\Users\vvago\CLionProjects\gtest-lab2\graphics\where_list.txt)", ios_base::trunc);
  ofstream reduce_list(R"(C:\Users\vvago\CLionProjects\gtest-lab2\graphics\reduce_list.txt)", ios_base::trunc);
  for (int i = n / 100; i < n; i += n / 100) {
    Stack<int> sub_list = Stack<int>(stack_list.getSubsequence(0, i));
    // map
    auto startTime = chrono::steady_clock::now();
    sub_list.map(map_function);
    auto endTime = chrono::steady_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    map_list << i << " " << time << "\n";
    // where
    startTime = chrono::steady_clock::now();
    sub_list.where(where_function);
    endTime = chrono::steady_clock::now();
    time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    where_list << i << " " << time << "\n";
    // reduce
    startTime = chrono::steady_clock::now();
    sub_list.reduce(reduce_function);
    endTime = chrono::steady_clock::now();
    time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    reduce_list << i << " " << time << "\n";
  }
  map_list.close();
  where_list.close();
  reduce_list.close();
  // map,where,reduce array
  ofstream map_array(R"(C:\Users\vvago\CLionProjects\gtest-lab2\graphics\map_array.txt)", ios_base::trunc);
  ofstream where_array(R"(C:\Users\vvago\CLionProjects\gtest-lab2\graphics\where_array.txt)", ios_base::trunc);
  ofstream reduce_array(R"(C:\Users\vvago\CLionProjects\gtest-lab2\graphics\reduce_array.txt)", ios_base::trunc);
  for (int i = n / 100; i < n; i += n / 100) {
    Stack<int> sub_array = Stack<int>(stack_array.getSubsequence(0, i));
    // map
    auto startTime = chrono::steady_clock::now();
    sub_array.map(map_function);
    auto endTime = chrono::steady_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    map_array << i << " " << time << "\n";
    // where
    startTime = chrono::steady_clock::now();
    sub_array.where(where_function);
    endTime = chrono::steady_clock::now();
    time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    where_array << i << " " << time << "\n";
    // reduce
    startTime = chrono::steady_clock::now();
    sub_array.reduce(reduce_function);
    endTime = chrono::steady_clock::now();
    time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    reduce_array << i << " " << time << "\n";
  }
  map_array.close();
  where_array.close();
  reduce_array.close();
  // find
  ofstream find_list(R"(C:\Users\vvago\CLionProjects\gtest-lab2\graphics\find_list.txt)", ios_base::trunc);
  for (int i = n / 100; i < n / 2; i += n / 100) {
    Stack<int> sub_stack = Stack<int>(stack_list.getSubsequence(n / 2 - i, n / 2 + i));
    auto startTime = chrono::steady_clock::now();
    stack_list.findSubsequence(sub_stack);
    auto endTime = chrono::steady_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    find_list << 2 * i << " " << time << "\n";
  }
  find_list.close();

  ofstream find_array(R"(C:\Users\vvago\CLionProjects\gtest-lab2\graphics\find_array.txt)", ios_base::trunc);
  for (int i = n / 100; i < n / 2; i += n / 100) {
    Stack<int> sub_stack = Stack<int>(stack_array.getSubsequence(n / 2 - i, n / 2 + i));
    auto startTime = chrono::steady_clock::now();
    stack_array.findSubsequence(sub_stack);
    auto endTime = chrono::steady_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    find_array << 2 * i << " " << time << "\n";
  }
  find_array.close();
  /*
  // push
  ofstream push_list("push_list.txt", ios_base::trunc);
  Stack<int> push_stack = Stack<int>(new LinkedListSequence<int>());
  int elem = 10;
  int k = 1;
  for (int i = n / 100; i < n; i++) {
    auto start_push = chrono::steady_clock::now();
    push_stack.push(elem);
    auto endTime = chrono::steady_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(endTime - start_push).count();
    if (i == k * n / 100) {
      k++;
      push_list << i << " " << time << "\n";
    }

  }
  push_list.close();

  ofstream push_array("push_array.txt", ios_base::trunc);
  Stack<int> push_stack_array = Stack<int>(new ArraySequence<int>());
  int elem_array = 10;
  int k1 = 1;
  for (int i = n / 100; i < n; i++) {
    auto start_push = chrono::steady_clock::now();
    push_stack.push(elem_array);
    auto endTime = chrono::steady_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(endTime - start_push).count();
    if (i == k1 * n / 100) {
      k1++;
      push_array << i << " " << time << "\n";
    }
  }
  push_array.close();
  */
}

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
                       {L"Строки/символы (string)", main_menu<wstring>}};
    try {
      menuLoop(L"Выберите тип элементов", _countof(menu), menu);
    } catch (IndexOutOfRange &ex) {
      wcout << L"Exception: " << ex.what() << endl << endl;
    }
  graph();
}
