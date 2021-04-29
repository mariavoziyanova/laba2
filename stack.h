#ifndef STACK_INCLUDED
#define STACK_INCLUDED

// == АТД (абстрактные типы данных) ==

#include "sequence.h"

// Стек - целевой АТД, указанный в варианте задания
// T - тип данных которые мы храним в стеке
template <class T>
class Stack {
  Sequence<T> *data;

 public:
  explicit Stack(Sequence<T> *sequence) : data(sequence) {};
  // == Деструктор - очистка памяти ==
  ~Stack() {
    delete data;
    data = nullptr;
  }
  // Пустой ли стек?
  bool empty() const {
    return data->getLength() == 0;
  }
  // Количество элементов в стеке
  int getLength() const {
    return data->getLength();
  }
  // Добавление значения на вершину стека
  void push(T item) {    // Вершина стека - конец списка
    data->append(item);  // Добавляем в начало
  }
  // Извлечение значения с вершины стека
  T pop() {  // Вершина стека - конец списка
    T value = data->getLast();
    data->removeAt(data->getLength() - 1);
    return value;
  }
  T operator[](int i) const {  // Получение значения
    return data->get(i);
  }
  // map - применение функции f(T x) к каждому элементу стека
  // f - функция с параметром типа T, возвращающая значение типа T
  // Создаётся новый стек, в котором к каждому элементу нашего стека применена функция f
  Stack<T> *map(T (*f)(T)) {
    return new Stack<T>(data->map(f));
  }
  // where фильтрует значения из списка l с помощью функции-фильтра h
  // h - функция с параметром типа T, возвращающая true если нужен элемент и false если нет
  Stack<T> *where(bool (*h)(T)) {
    return new Stack<T>(data->where(h));
  }
  // reduce - применяем к каждой паре значений функцию f пока не получим одно значение
  T reduce(T (*f)(T, T)) {
    return data->reduce(f);
  }
  // Конкатенация двух стеков
  Stack<T> *concat(Stack<T> &stack) {
    return new Stack<T>(data->concat(stack.data));
  }
  // Вывод на экран содержимого стека
  void print() {
    wcout << L"Stack size = " << data->getLength() << endl;
    for (int i = 0; i < data->getLength(); i++) {
      wcout << data->get(i) << L" ";
    }
    wcout << endl;
  }
  // Ввод элементов стека
  // Конструктор для ввода элементов стека
  explicit Stack(Sequence<T> *sequence, const wchar_t *string) : data(sequence) {
    wcout << string << endl;
    int N;
    wprintf(L"Введите количество элементов: ");
    wcin >> N;
    // Вводим элементы по одному
    for (int i = 0; i < N; i++) {
      wprintf(L"Введите элемент с индексом %d: ", i);
      T element;
      wcin >> element;
      push(element);  // Добавляем его на вершину стека
      // print(); // Текущее состояние стека
    }
  }
  Sequence<T> *getSubsequence(int startIndex, int endIndex) const {
    return data->getSubsequence(startIndex, endIndex);
  }
  // Поиск подпоследовательности
  int findSubsequence(Stack<T> &stack) {
    return data->findSubsequence(*stack.data);
  }
};

// Персона характеризуется набором атрибутов, таких ФИО, дата рождения, некоторый идентификатор (в роли которого может
// выступать: номер в некотором списке, номер зачетки/табельный номер, номер паспорта, и др.). Пример структуры,
// описывающей персону:
struct PersonID {
  int id;
};

class Person {
 private:
  PersonID id;
  char *firstName;
  char *middleName;
  char *lastName;
  time_t birthDate;

 public:
  PersonID GetID() {
    return id;
  };
  char *GetFirstName() {
    return firstName;
  };
  char *GetMiddleName() {
    return middleName;
  };
  char *GetLastName() {
    return lastName;
  };
  // ...
};

// Функции для работы со стеком
namespace stack {
// map - применение функции f к каждому элементу стека
template <class T>
Stack<T> *map(T (*f)(T), Stack<T> &l) {
  return l.map(f);
}

// where фильтрует значения из списка l с помощью функции-фильтра h
template <class T>
Stack<T> *where(bool (*h)(T), Stack<T> &l) {
  return l.where(h);
}

// Применение операции к элементам пока
template <class T>
T reduce(T (*f)(T, T), Stack<T> &l) {
  return l.reduce(f);
}
}  // namespace stack

#endif  // STACK_INCLUDED
