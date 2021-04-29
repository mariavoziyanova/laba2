#ifndef LINKEDLIST_INCLUDED
#define LINKEDLIST_INCLUDED

#include <iostream>

#include "common.hpp"

// АТД линейный связанный список: класс LinkedList
template <class T>
class LinkedList {
  // Элемент двусвязного списка: значение + указатели на предыдущий и следующий
  struct Node {  // struct - всё будет доступно извне, по-умолчанию public
    T value;     // Значение элемента
    Node *prev;  // Указатель на предыдущий элемент
    Node *next;  // Указатель на следующий элемент
    Node(T value, Node *prev = nullptr, Node *next = nullptr) : value(value), prev(prev), next(next) {}
  };

  Node *first = nullptr;  // Первый элемент списка
  Node *last = nullptr;   // Последний элемент списка
  int size = 0;           // Количество элементов

 public:
  // == Конструкторы - cоздание объекта ==
  LinkedList(T *items, int count) {  // Копировать элементы из переданного массива
    for (int i = 0; i < count; i++) {
      append(items[i]);
    }
  }
  // Создать пустой список
  LinkedList() {};
  // Копирующий конструктор
  LinkedList(const LinkedList<T> &list) : size(list.size) {
    // Пробегаем по элементам исходного списка
    for (Node *n = list.first; n != nullptr; n = n->next) {
      // Создаём новый элемент и помещаем его в конец нашего списка
      // Передаём значение value
      // Предыдущий элемент prev для нового элемента - старый конец списка
      // Следующего элемента нет (nullptr), т.к. node - последний элемент в списке
      auto *node = new Node(n->value, last, nullptr);
      // Если у нас вообще не было элементов, то node ещё и первый элемент в списке
      if (first == nullptr) first = node;
      // Если у нас был какой-то последний элемент, то новый элемент идёт после него
      if (last != nullptr) last->next = node;
      // Созданный элемент - последний элемент
      last = node;
    }
  }
  // == Деструктор - очистка памяти ==
  ~LinkedList() {
    while (first != nullptr) {  // Пока в списке ещё есть элементы
      Node *toDelete = first;  // Запоминаем указатель на элемент который собираемся удалить
      first = first->next;  // Делаем первым следующий за ним элемент
      delete toDelete;      // Удаляем элемент
    }
    last = nullptr;
  }
  // == Декомпозиция ==
  // Может выбрасывать исключения: − IndexOutOfRange (если список пуст)
  T getFirst() const {
    if (first == nullptr) throw IndexOutOfRange("List is empty");
    return first->value;
  }
  // Получить последний элемент в списке
  // Может выбрасывать исключения: − IndexOutOfRange (если список пуст)
  T getLast() const {
    if (last == nullptr) throw IndexOutOfRange("List is empty");
    return last->value;
  }
  // Получить элемент по индексу
  // Может выбрасывать исключения:
  // − IndexOutOfRange (если индекс отрицательный или больше/равен числу элементов)
  T &get(int index) const {
    // Если нам передали отрицательный индекс - генерируем исключение
    if (index < 0)
      throw IndexOutOfRange(string("Index ") + to_string(index) + " out of range 0.." + to_string(getLength() - 1));
    // Пробегаем по элементам от первого до последнего и увеличиваем счётчик размера
    int idx = 0;  // Текущий индекс
    for (Node *n = first; n != nullptr; n = n->next) {
      if (idx == index) return n->value;  // Если уже дошли до нужного индекса
      idx++;                              // Увеличиваем текущий индекс
    }
    throw IndexOutOfRange(string("Index ") + to_string(index) + " out of range 0.." + to_string(getLength() - 1));
  }
  // Перегруженные операторы
  T operator[](int i) const {
    return get(i);
  }
  T &operator[](int i) {
    return get(i);
  }
  // Получить список из всех элементов, начиная с startIndex и заканчивая endIndex.
  // Может выбрасывать исключения:
  // − IndexOutOfRange (если хотя бы один из индексов отрицательный или больше/равен числу элементов)
  LinkedList<T> *getSubList(int startIndex, int endIndex) {
    // Проверяем что индексы неотрицательные
    if (startIndex < 0)
      throw IndexOutOfRange(string("startIndex ") + to_string(startIndex) + " out of range 0.." +
                            to_string(getLength() - 1));
    if (endIndex < 0)
      throw IndexOutOfRange(string("endIndex ") + to_string(endIndex) + " out of range 0.." +
                            to_string(getLength() - 1));
    // И что начало не позже конца
    if (startIndex > endIndex)
      throw IndexOutOfRange(string("startIndex=") + to_string(startIndex) + " > endIndex=" + to_string(endIndex));
    // Пропускаем все элементы до startIndex
    Node *cur = first;
    for (int i = 0; i < startIndex && cur != nullptr; i++) cur = cur->next;
    // Создаём новый список для результата
    auto *subList = new LinkedList<T>();
    for (int i = startIndex; i <= endIndex; i++) {
      subList->append(cur->value);
      cur = cur->next;
    }
    return subList;
  }
  // Получить длину списка
  int getLength() const {
    return size;
  }
  // == Операции ==
  // Добавляет элемент в конец списка
  void append(T item) {
    // Создаём новый элемент списка
    auto *n = new Node(item, last, nullptr);
    // Начало и конец списка
    if (first == nullptr) {  // Если нет первого элемента
      first = n;             // То это первый элемент
    }
    if (last != nullptr) {  // Если был какой-то последний элемент
      last->next = n;  // То у него ссылка вперёд должна быть на наш элемент
    }
    last = n;  // Наш элемент - последний
    size++;    // Добавляем один элемент - размер увеличивается на 1
  }
  // Добавляет элемент в начало списка
  void prepend(T item) {
    // Создаём новый элемент списка
    auto *node = new Node(item, nullptr, first);
    // Начало и конец списка
    if (first != nullptr) {
      first->prev = node;
    }
    first = node;
    if (last == nullptr) {
      last = node;
    }
    size++;  // Добавляем один элемент - размер увеличивается на 1
  }
  // Вставляет элемент в заданную позицию
  // Может выбрасывать исключения:
  // − IndexOutOfRange (если индекс отрицательный или больше/равен числу элементов)
  void insertAt(T item, int index) {
    // Проверяем корректность индекса
    if (index < 0) {
      throw IndexOutOfRange(string("insertAt index=") + to_string(index) + " < 0");
    }
    if (index > getLength()) {
      throw IndexOutOfRange(string("insertAt index=") + to_string(index) + " >= getLength()");
    }
    if (index == 0) {  // Добавляем элемент в начало списка
      prepend(item);
      return;
    }
    // Находим элемент после которого надо вставить наш элемент
    Node *cur = first;
    for (int i = 0; i < index - 1 && cur != nullptr; i++) {  // Сдвигаемся на index элементов вправо
      cur = cur->next;
    }
    // Создаём новый элемент между найденным и следующим
    auto *n = new Node(item, cur, cur->next);
    cur->next = n;
    if (n->next == nullptr) {  // Если следующего элемента нет => то это новый последний элемент
      last = n;
    } else {
      n->next->prev = n;  // Чтобы следующий элемент ссылался на текущий
    }
    size++;  // Добавляем один элемент - размер увеличивается на 1
  }
  // Сцепляет два списка
  LinkedList<T> *concat(LinkedList<T> *list) {
    // Сначала создаём копию текущего списка
    auto *res = new LinkedList<T>(*this);
    // Добавляем в коней по одному все элементы из второго списка
    for (Node *n = list->first; n != nullptr; n = n->next) {
      res->append(n->value);
    }
    return res;
  }
  // Удаление элемента по индексу
  void removeAt(int index) {
    // Проверяем корректность индекса
    if (index < 0) {
      throw IndexOutOfRange(string("removeAt index=") + to_string(index) + " < 0");
    }
    if (index > getLength()) {
      throw IndexOutOfRange(string("removeAt index=") + to_string(index) + " >= getLength()");
    }
    // Ищем элемент который надо удалить по индексу
    Node *toDelete = first;
    for (int i = 0; i < index; i++) {
      toDelete = toDelete->next;
    }
    // Если это первый элемент, то первым должен стать следуюший
    if (toDelete == first) {
      first = toDelete->next;
    }
    // Если это последний
    if (toDelete == last) {
      last = toDelete->prev;
    }
    // Если есть предыдущий элемент, то он должен ссылаться на наш следующий
    if (toDelete->prev != nullptr) {
      toDelete->prev->next = toDelete->next;
    }
    // Если есть следующий элемент, то он должен ссылаться на наш предыдущий
    if (toDelete->next != nullptr) {
      toDelete->next->prev = toDelete->prev;
    }
    // Наконец-то удаляем наш элемент
    delete toDelete;
    // И уменьшаем количество элементов в списке
    size--;
  }
  // Вывод последовательности на печать (в консоль)
  void print() {
    wcout << L"LinkedList (size = " << getLength() << L")";
    for (Node *n = first; n != nullptr; n = n->next) {
      wcout << L" " << n->value;
    }
    wcout << endl;
  };
  // TODO: сделать итератор для LinkedList
  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    Iterator(Node *ptr) : m_ptr(ptr) {}

    reference operator*() const {
      return m_ptr->value;
    }
    pointer operator->() {
      return m_ptr->value;
    }
    Iterator &operator++() {
      m_ptr = m_ptr->next;
      return *this;
    }
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    friend bool operator==(const Iterator &a, const Iterator &b) {
      return a.m_ptr == b.m_ptr;
    };
    friend bool operator!=(const Iterator &a, const Iterator &b) {
      return a.m_ptr != b.m_ptr;
    };

   private:
    Node *m_ptr;
  };

  Iterator begin() {
    return Iterator(first);
  }
  Iterator end() {
    return Iterator(nullptr);
  }
};

#endif
