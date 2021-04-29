#ifndef SEQUENCE_INCLUDED
#define SEQUENCE_INCLUDED

// == АТД (абстрактные типы данных) ==
// АТД последовательность - абстрактный класс
// Абстрактный класс - нельзя создать экземпляр (есть абстрактные методы)
template <class T>
class Sequence {
 public:
  // Создание объекта (конструкторы) - в каждом подклассе свое создание
  // == Декомпозиция ==
  // Получить первый элемент в списке
  // Может выбрасывать исключения: IndexOutOfRange (если список пуст)
  virtual T getFirst() const = 0;  // абстрактная (чистая виртуальная функция)
  // Получить последний элемент в списке
  // Может выбрасывать исключения: IndexOutOfRange (если список пуст)
  virtual T getLast() const = 0;  // абстрактная (чистая виртуальная функция)
  // Получить элемент по индексу
  // Может выбрасывать исключения: IndexOutOfRange (если индекс отрицательный или больше/равен числу элементов)
  virtual T get(int index) const = 0;  // абстрактная = чистая виртуальная функция
  virtual T operator[](int i) const = 0;
  virtual T &operator[](int i) = 0;
  // Получить список из всех элементов, начиная с startIndex и заканчивая endIndex
  // Может выбрасывать исключения:
  // - IndexOutOfRange (если хотя бы один из индексов отрицательный или больше/равен числу элементов)
  virtual Sequence<T> *getSubsequence(int startIndex, int endIndex) const = 0;  // (чистая виртуальная функция)
  // Получить длину списка
  // Может выбрасывать исключения:
  // − IndexOutOfRange (если индекс отрицательный или больше/равен числу элементов)
  virtual int getLength() const = 0;  // абстрактная (чистая виртуальная функция)
  // == Операции ==
  // Добавляет элемент в конец списка
  virtual void append(T item) = 0;  // абстрактная (чистая виртуальная функция)
  // Добавляет элемент в начало списка
  virtual void prepend(T item) = 0;  // абстрактная (чистая виртуальная функция)
  // Вставляет элемент в заданную позицию index
  // Может выбрасывать исключения:
  // − IndexOutOfRange (если индекс отрицательный или больше/равен числу элементов)
  virtual void insertAt(T item, int index) = 0;
  // Удаление элемента по индексу
  virtual void removeAt(int index) = 0;  // абстрактная (чистая виртуальная функция)
  // Сцепляет два списка: к текущему списку присоединяем list
  //  Возвращаемый_список = текущий_список + список_list
  virtual Sequence<T> *concat(Sequence<T> *list) = 0;  // абстрактная (чистая виртуальная функция)
  // Вывод последовательности на печать (в консоль)
  virtual void print() = 0;
  // Поиск подпоследовательности
  // Возвращает стартовый индекс или -1 - если не найдена
  virtual int findSubsequence(Sequence<T> &subSequence) {
    int subSeqLen = subSequence.getLength();  // Длина подпоследовательности
    // Перебираем начало подпоследовательности
    for (int start = 0; start <= getLength() - subSeqLen; start++) {
      bool match = true;  // Найдена ли подпоследовательность начиная с индекса
      // Цикл проверки подпоследовательности
      for (int i = 0; i < subSeqLen; i++) {  // Проверяем подпоследовательность поэлементно
        if (get(start + i) != subSequence.get(i)) {  // Если нашли несовпадающий элемент
          match = false;  // Подпоследовательность не совпадает
          break;          // Выходим из цикла проверки
        }
      }
      if (match) {  // Нашли подпоследовательность
        return start;
      }
    }
    return -1;
  };
  // map - применение функции f(T x) к каждому элементу стека
  // f - функция с параметром типа T, возвращающая значение типа T
  // Создаётся новый стек, в котором к каждому элементу нашего стека применена функция f
  virtual Sequence<T> *map(T (*f)(T)) const = 0;
  // where фильтрует значения из списка l с помощью функции-фильтра h
  // h - функция с параметром типа T, возвращающая true если нужен элемент и false если нет
  virtual Sequence<T> *where(bool (*h)(T)) const = 0;
  // reduce - применяем к каждой паре значений функцию f пока не получим одно значение
  virtual T reduce(T (*f)(T, T)) const = 0;
  // == Виртуальный деструктор ==
  virtual ~Sequence<T>() = default;
};

#endif  // SEQUENCE_INCLUDED
