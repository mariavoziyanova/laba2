#ifndef LINKEDLISTSEQUENCE_INCLUDED
#define LINKEDLISTSEQUENCE_INCLUDED

#include "linkedlist.h"
#include "sequence.h"

// АТД связанный список
template <class T>
class LinkedListSequence : public Sequence<T> {
  LinkedList<T> data;  // Массив данных
  void checkIndex(int index) const {
    if (index < 0)
      throw IndexOutOfRange(string("Index ") + to_string(index) + " out of range 0.." + to_string(getLength() - 1));
    if (index >= getLength())
      throw IndexOutOfRange(string("Index ") + to_string(index) + " out of range 0.." + to_string(getLength() - 1));
  }

 public:
  // Копировать элементы из переданного массива
  LinkedListSequence(T *items, int count) : data(items, count) {}
  // Создать пустой список
  LinkedListSequence() : data() {};
  // Копирующий конструктор
  explicit LinkedListSequence(const LinkedList<T> &list) : data(new LinkedList<T>()) {
    for (int i = 0; i < list.getLength(); i++) {
      data->append(list.get(i));
    }
  };
  // == Декомпозиция ==
  // Получить первый элемент в списке
  // Может выбрасывать исключения: IndexOutOfRange (если список пуст)
  T getFirst() const override {
    return data.getFirst();
  };
  // Получить последний элемент в списке
  // Может выбрасывать исключения: IndexOutOfRange (если список пуст)
  T getLast() const override {
    return data.getLast();
  };
  // Получить элемент по индексу
  // Может выбрасывать исключения: IndexOutOfRange (если индекс отрицательный или больше/равен числу элементов)
  T get(int index) const override {
    return data.get(index);
  };
  // Перегруженные операторы
  T operator[](int i) const {
    return data[i];
  }
  T &operator[](int i) {
    return data[i];
  };
  // Получить список из всех элементов, начиная с startIndex и заканчивая endIndex
  // Может выбрасывать исключения:
  // - IndexOutOfRange (если хотя бы один из индексов отрицательный или больше/равен числу элементов)
  Sequence<T> *getSubsequence(int startIndex, int endIndex) const override {
    checkIndex(startIndex);
    checkIndex(endIndex);
    auto *res = new LinkedListSequence<T>();
    for (int index = startIndex; index <= endIndex; index++) {
      res->append(data.get(index));
    }
    return res;
  }
  // Получить длину списка
  // Может выбрасывать исключения: IndexOutOfRange (если индекс отрицательный или больше/равен числу элементов)
  int getLength() const override {
    return data.getLength();
  }
  // == Операции ==
  // Добавляет элемент в конец списка
  void append(T item) override {
    data.append(item);
  };
  // Добавляет элемент в начало списка
  void prepend(T item) override {
    data.prepend(item);
  };
  // Вставляет элемент в заданную позицию
  // Может выбрасывать исключения:
  // − IndexOutOfRange (если индекс отрицательный или больше/равен числу элементов)
  void insertAt(T item, int index) override {
    data.insertAt(item, index);
  };
  // Сцепляет два списка
  Sequence<T> *concat(Sequence<T> *list) override {
    // Создаём новый список как копию старого
    Sequence<T> *res = new LinkedListSequence<T>(*this);
    // Неэффективный метод
    // Для реализации эффективного нужен итератор по Sequence
    for (int i = 0; i < list->getLength(); i++) {
      res->append(list->get(i));
    }
    return res;
  };
  // Удаление элемента по индексу
  void removeAt(int index) override {
    data.removeAt(index);
  }
  // Печать на экран всех элементов
  void print() override {
    data.print();
  }
  // == Деструктор - очистка памяти ==
  virtual ~LinkedListSequence() = default;
  Sequence<T> *map(T (*f)(T)) const override {
    auto *res = new LinkedListSequence<T>();
    for (int i = 0; i < getLength(); i++) {
      res->append(f(data.get(i)));
    }
    return res;
  }
  Sequence<T> *where(bool (*h)(T)) const override {
    auto *res = new LinkedListSequence<T>;
    for (int i = 0; i < getLength(); i++) {
      T item = data.get(i);
      if (h(item)) {  // Если h возвращает true - добавляем элемент в результат
        res->append(item);
      }
    }
    return res;
  }
  T reduce(T (*f)(T, T)) const override {
    T result = data.getFirst();
    for (int i = 1; i < data.getLength(); i++) {
      result = f(result, data.get(i));
    }
    return result;
  }
};

#endif
