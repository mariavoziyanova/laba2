#include <chrono>
#include <complex>
#include <cstdlib>

#include "arraysequence.h"
#include "gtest/gtest.h"
#include "linkedlist.h"
#include "linkedlistsequence.h"
#include "sequence.h"
#include "stack.h"

// Базовые операции с динамическим массивом
TEST(DynamicArray, basic_operations) {
  // Создаём массив на 3 элемента
  DynamicArray<int> da(3);
  ASSERT_EQ(3, da.getSize());  // Проверяем что размер 3
  try {
    da.get(0);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Element with index 0 not defined", ex.what());
  }
  try {
    da.get(2);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Element with index 2 not defined", ex.what());
  }
  // Задаём значение всех элементов
  da.set(0, 11);
  da.set(1, 22);
  da.set(2, 33);
  // Проверяем значение всех элементов массива
  ASSERT_EQ(11, da.get(0));
  ASSERT_EQ(22, da.get(1));
  ASSERT_EQ(33, da.get(2));
  // Обращение по индексу через оператор
  ASSERT_EQ(11, da[0]);
  ASSERT_EQ(22, da[1]);
  ASSERT_EQ(33, da[2]);
  // Проверяем что на отрицательный индекс генерируется исключение для операции get
  try {
    da.get(-1);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index -1 out of range 0..2", ex.what());
  }
  // Проверяем что на отрицательный индекс генерируется исключение для операции set
  try {
    da.set(-1, 122);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index -1 out of range 0..2", ex.what());
  }
  // Слишком большой индекс - за границами массива => тоже исключение
  try {
    da.get(10);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index 10 out of range 0..2", ex.what());
  }
  try {
    int x = da[11];
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index 11 out of range 0..2", ex.what());
  }
  // Меняем значения по индексу
  da[0] = 101;
  ASSERT_EQ(3, da.getSize());
  ASSERT_EQ(101, da[0]);
  ASSERT_EQ(22, da[1]);
  ASSERT_EQ(33, da[2]);
  // Вставка элемента
  da.insertAt(1, 0);
  ASSERT_EQ(4, da.getSize());
  ASSERT_EQ(1, da[0]);
  ASSERT_EQ(101, da[1]);
  ASSERT_EQ(22, da[2]);
  ASSERT_EQ(33, da[3]);
  // Перегрузка операторов
  da[0] = 2 * da[1] + da[2];             // Более наглядна
  da.set(0, 2 * da.get(1) + da.get(2));  // чем такая
}

TEST(DynamicArray, undefined_elements) {
  DynamicArray<int> da(4);
  try {
    int x = da.get(0);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Element with index 0 not defined", ex.what());
  }
  try {
    int x = da.get(2);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Element with index 2 not defined", ex.what());
  }
  try {
    int x = da.get(-1);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index -1 out of range 0..3", ex.what());
  }
  try {
    int x = da.get(11);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index 11 out of range 0..3", ex.what());
  }
  da[1] = 20;
  da.print();
}

TEST(DynamicArray, swap) {
  int data[] = {11, 22, 33};
  DynamicArray<int> da(data, _countof(data));
  // Меняем местами 2 элемента
  da.swap(0, 1);
  ASSERT_EQ(22, da[0]);
  ASSERT_EQ(11, da[1]);
  ASSERT_EQ(33, da[2]);
}

TEST(DynamicArray, removeAt) {
  int data[] = {11, 22, 33};
  DynamicArray<int> da(data, _countof(data));
  da.removeAt(1);
  ASSERT_EQ(2, da.getSize());
  ASSERT_EQ(11, da[0]);
  ASSERT_EQ(33, da[1]);
  da.removeAt(1);
  ASSERT_EQ(1, da.getSize());
  ASSERT_EQ(11, da[0]);
  da.removeAt(0);
  ASSERT_EQ(0, da.getSize());
}

TEST(DynamicArray, resize) {
  DynamicArray<int> da(0);
  ASSERT_EQ(0, da.getSize());  // Проверяем что размер 0
  for (int size = 1; size <= 10; size++) {
    da.resize(size);
    ASSERT_EQ(size, da.getSize());  // Проверяем размер
  }
  da[4] = 4;
  da[5] = 5;
  da[9] = 9;
  da.print();
}

TEST(DynamicArray, array_initialization) {
  // Инициализация динамического массива из массива значений
  int data[] = {111, 222};
  DynamicArray<int> da(data, _countof(data));
  ASSERT_EQ(_countof(data), da.getSize());
  da.set(0, 111);
  da.set(1, 222);
  ASSERT_EQ(111, da.get(0));
  ASSERT_EQ(222, da.get(1));

  // Копирующий конструктор
  DynamicArray<int> d2(da);
  ASSERT_EQ(da.getSize(), d2.getSize());
  ASSERT_EQ(111, d2.get(0));
  ASSERT_EQ(222, d2.get(1));

  // Уменьшаем размер
  d2.resize(1);
  ASSERT_EQ(1, d2.getSize());
  ASSERT_EQ(111, d2.get(0));

  // Увеличиваем размер
  d2.resize(3);
  ASSERT_EQ(3, d2.getSize());
  ASSERT_EQ(111, d2.get(0));
  d2.set(1, 221);
  d2.set(2, 331);
  ASSERT_EQ(221, d2.get(1));
  ASSERT_EQ(331, d2.get(2));
}

TEST(LinkedList, list_basic_operations) {  // Базовые операции для списка
  // Создаём массив на 3 элемента
  int data[] = {11, 22, 33};
  LinkedList<int> list(data, _countof(data));
  ASSERT_EQ(3, list.getLength());
  // Проверяем значение всех элементов массива
  ASSERT_EQ(11, list.get(0));
  ASSERT_EQ(22, list.get(1));
  ASSERT_EQ(33, list.get(2));
  ASSERT_EQ(11, list[0]);
  ASSERT_EQ(22, list[1]);
  ASSERT_EQ(33, list[2]);
  // Отрицательный индекс
  try {
    list.get(-2);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index -2 out of range 0..2", ex.what());
  }
  // Отрицательный индекс
  try {
    list[-2];
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index -2 out of range 0..2", ex.what());
  }
  // Слишком большой индекс - за границами массива
  try {
    list.get(10);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index 10 out of range 0..2", ex.what());
  }
  list.insertAt(500, 1);
}

TEST(LinkedList, add_elements) {
  LinkedList<int> list;
  // Пока что список пуст
  ASSERT_EQ(0, list.getLength());
  try {
    list.getFirst();
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("List is empty", ex.what());
  }
  try {
    list.getLast();
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("List is empty", ex.what());
  }
  // Добавляем первый элемент
  list.prepend(11);
  ASSERT_EQ(1, list.getLength());
  ASSERT_EQ(11, list.getFirst());
  ASSERT_EQ(11, list.getLast());
  // Добавляем второй элемент в начало списка
  list.prepend(22);
  ASSERT_EQ(2, list.getLength());
  ASSERT_EQ(22, list.getFirst());
  ASSERT_EQ(11, list.getLast());
  // Конструктор копирования
  LinkedList<int> list2(list);
  ASSERT_EQ(2, list2.getLength());
  ASSERT_EQ(22, list2.getFirst());
  ASSERT_EQ(11, list2.getLast());
  ASSERT_EQ(22, list2.get(0));
  ASSERT_EQ(11, list2.get(1));
  // Подсписок
  LinkedList<int> *subList = list2.getSubList(0, 0);
  ASSERT_EQ(1, subList->getLength());
  ASSERT_EQ(22, subList->getFirst());
  ASSERT_EQ(22, subList->getLast());
  delete subList;
  // Исключения при взятии подсписка
  try {
    list2.getSubList(-1, 0);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("startIndex -1 out of range 0..1", ex.what());
  }
  try {
    list2.getSubList(0, -1);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("endIndex -1 out of range 0..1", ex.what());
  }
  try {
    list2.getSubList(1, 0);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("startIndex=1 > endIndex=0", ex.what());
  }
}

// Добавление элементов в конец списка
TEST(LinkedList, add_element_append) {
  LinkedList<int> list;  // Список целых чисел
  // Пока что список пуст
  ASSERT_EQ(0, list.getLength());
  // Пытаемся получить первый элемент на пустом списке
  try {
    list.getFirst();
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("List is empty", ex.what());
  }
  // Пытаемся получить последний элемент на пустом списке
  try {
    list.getLast();
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("List is empty", ex.what());
  }
  // Добавляем первый элемент
  list.append(11);
  ASSERT_EQ(1, list.getLength());
  ASSERT_EQ(11, list.getFirst());
  ASSERT_EQ(11, list.getLast());
  // Добавляем второй элемент в начало списка
  list.append(22);
  ASSERT_EQ(2, list.getLength());
  ASSERT_EQ(11, list.getFirst());
  ASSERT_EQ(22, list.getLast());
}

TEST(LinkedList, Concat) {  // Соединение списков
  int items1[] = {11, 22, 33};
  int items2[] = {44, 55};
  LinkedList<int> list1(items1, _countof(items1));  // Список целых чисел
  LinkedList<int> list2(items2, _countof(items2));  // Список целых чисел
  // Проверяем длину списков
  ASSERT_EQ(3, list1.getLength());
  ASSERT_EQ(2, list2.getLength());
  // Конкатенация
  LinkedList<int> *l = list1.concat(&list2);
  ASSERT_EQ(5, l->getLength());
  ASSERT_EQ(11, l->get(0));
  ASSERT_EQ(22, l->get(1));
  ASSERT_EQ(33, l->get(2));
  ASSERT_EQ(44, l->get(3));
  ASSERT_EQ(55, l->get(4));
  delete l;
}

TEST(LinkedList, Iterator) {  // Итератор
  int items[] = {11, 22, 33};
  LinkedList<int> list(items, _countof(items));  // Список целых чисел
  // Проверяем длину списков
  ASSERT_EQ(3, list.getLength());

  auto it = list.begin();
  ASSERT_EQ(11, *it);
  it++;
  ASSERT_EQ(22, *it);
  ++it;
  ASSERT_EQ(33, *it);
  it++;
  ASSERT_EQ(list.end(), it);

  for (auto it = list.begin(), end = list.end(); it != end; ++it) {
    const auto i = *it;
    std::cout << i << "\n";
  }
  for (int x : list) {
    cout << x << endl;
  }
}

TEST(LinkedList, insertAt) {
  LinkedList<int> list;  // Список целых чисел
  // Проверяем длину списка
  ASSERT_EQ(0, list.getLength());
  // Добавляем первый элемент
  list.insertAt(11, 0);
  ASSERT_EQ(1, list.getLength());
  ASSERT_EQ(11, list.get(0));
  ASSERT_EQ(11, list.getFirst());
  ASSERT_EQ(11, list.getLast());
  // Вставляем элемент в конец непустого списка
  list.insertAt(22, 1);
  ASSERT_EQ(2, list.getLength());
  ASSERT_EQ(11, list.get(0));
  ASSERT_EQ(22, list.get(1));
  ASSERT_EQ(11, list.getFirst());
  ASSERT_EQ(22, list.getLast());
  // Некорректный индекс
  try {
    list.insertAt(1, -1);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("insertAt index=-1 < 0", ex.what());
  }
  // Вставка в середину списка
  list.insertAt(15, 1);
  ASSERT_EQ(3, list.getLength());
  ASSERT_EQ(11, list.get(0));
  ASSERT_EQ(15, list.get(1));
  ASSERT_EQ(22, list.get(2));
  ASSERT_EQ(11, list.getFirst());
  ASSERT_EQ(22, list.getLast());
}

// Удаление элемента из списка
TEST(LinkedList, removeAt) {
  LinkedList<int> list;  // Список целых чисел
  // Проверяем длину списка
  ASSERT_EQ(0, list.getLength());
  // Добавляем первый элемент
  list.insertAt(11, 0);
  ASSERT_EQ(1, list.getLength());
  ASSERT_EQ(11, list.get(0));
  ASSERT_EQ(11, list.getFirst());
  ASSERT_EQ(11, list.getLast());
  // Удаление одного элемента
  list.removeAt(0);
  ASSERT_EQ(0, list.getLength());
  // Добавляем элементы
  list.append(11);
  list.append(22);
  list.append(33);
  list.append(44);
  ASSERT_EQ(4, list.getLength());
  ASSERT_EQ(11, list.get(0));
  ASSERT_EQ(22, list.get(1));
  ASSERT_EQ(33, list.get(2));
  ASSERT_EQ(44, list.get(3));
  list.removeAt(0);
  ASSERT_EQ(3, list.getLength());
  ASSERT_EQ(22, list.get(0));
  ASSERT_EQ(33, list.get(1));
  ASSERT_EQ(44, list.get(2));
  list.removeAt(2);
  ASSERT_EQ(2, list.getLength());
  ASSERT_EQ(22, list.get(0));
  ASSERT_EQ(33, list.get(1));
  list.append(44);
  ASSERT_EQ(3, list.getLength());
  ASSERT_EQ(22, list[0]);
  ASSERT_EQ(33, list[1]);
  ASSERT_EQ(44, list[2]);
  list.removeAt(1);
  ASSERT_EQ(2, list.getLength());
  ASSERT_EQ(22, list[0]);
  ASSERT_EQ(44, list[1]);
  list.print();
}

TEST(ArraySequence, constructor) {
  Sequence<int> *s = new ArraySequence<int>();
  ASSERT_EQ(0, s->getLength());
  for (int x = 1; x <= 10; x++) {
    s->append(x);
    ASSERT_EQ(x, s->getLength());
    ASSERT_EQ(x, s->getLast());
  }
  delete s;
}

TEST(ArraySequence, ArraySequence_Append_Prepend_element) {
  int data[] = {111, 222};
  Sequence<int> *s = new ArraySequence<int>(data, _countof(data));  // Список целых чисел
  ASSERT_EQ(2, s->getLength());
  ASSERT_EQ(111, s->get(0));
  ASSERT_EQ(222, s->get(1));
  ASSERT_EQ(111, s->getFirst());
  ASSERT_EQ(222, s->getLast());
  s->append(333);
  ASSERT_EQ(3, s->getLength());
  ASSERT_EQ(333, s->get(2));
  s->prepend(10);
  ASSERT_EQ(4, s->getLength());
  ASSERT_EQ(10, s->get(0));
  ASSERT_EQ(111, s->get(1));
  ASSERT_EQ(222, s->get(2));
  ASSERT_EQ(333, s->get(3));
  delete s;
}

// Создание второго динамического массива как копии первого
TEST(ArraySequence, create_from_DynamicArray) {
  int data[] = {123, 234, 345};
  DynamicArray<int> da(data, _countof(data));     // Список целых чисел
  Sequence<int> *s = new ArraySequence<int>(da);  // Список целых чисел
  ASSERT_EQ(3, s->getLength());
  ASSERT_EQ(data[0], s->get(0));
  ASSERT_EQ(data[1], s->get(1));
  ASSERT_EQ(data[2], s->get(2));
  ASSERT_EQ(data[0], s->getFirst());
  ASSERT_EQ(data[2], s->getLast());
  s->append(333);
  ASSERT_EQ(4, s->getLength());
  ASSERT_EQ(333, s->getLast());
  s->prepend(10);
  ASSERT_EQ(5, s->getLength());
  // 123, 234, 345
  ASSERT_EQ(10, s->get(0));
  ASSERT_EQ(data[0], s->get(1));
  ASSERT_EQ(data[1], s->get(2));
  ASSERT_EQ(data[2], s->get(3));
  ASSERT_EQ(333, s->get(4));
  Sequence<int> *sub = s->getSubsequence(1, 3);
  ASSERT_EQ(3, sub->getLength());
  ASSERT_EQ(123, sub->get(0));
  ASSERT_EQ(234, sub->get(1));
  ASSERT_EQ(345, sub->get(2));
  // Вставим ещё один элемент посередине
  sub->insertAt(4444, 1);
  ASSERT_EQ(4, sub->getLength());
  ASSERT_EQ(123, sub->get(0));
  ASSERT_EQ(4444, sub->get(1));
  ASSERT_EQ(234, sub->get(2));
  ASSERT_EQ(345, sub->get(3));
  delete s;
  delete sub;
}

// Сцепление двух списков
TEST(ArraySequence, concat) {
  // Первая последовательность
  int data1[] = {11, 22};
  DynamicArray<int> da1(data1, _countof(data1));    // Список целых чисел
  Sequence<int> *s1 = new ArraySequence<int>(da1);  // Список целых чисел
  // Вторая последовательность
  int data2[] = {33};
  DynamicArray<int> da2(data2, _countof(data2));    // Список целых чисел
  Sequence<int> *s2 = new ArraySequence<int>(da2);  // Список целых чисел
  // Сцепляем два списка
  Sequence<int> *res = s1->concat(s2);
  ASSERT_EQ(3, res->getLength());
  ASSERT_EQ(11, res->get(0));
  ASSERT_EQ(22, res->get(1));
  ASSERT_EQ(33, res->get(2));
  delete s1;
  delete s2;
  delete res;
}

TEST(ArraySequence, findSubsequence) {
  int data[] = {11, 22, 33, 44, 55};
  DynamicArray<int> da(data, _countof(data));
  Sequence<int> *main = new ArraySequence<int>(da);

  {
    int d[] = {11, 22};
    Sequence<int> *sub1 = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(0, main->findSubsequence(*sub1));
    delete sub1;
  }
  {
    int d[] = {22, 33};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(1, main->findSubsequence(*sub));
    delete sub;
  }
  {
    int d[] = {33, 44, 55};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(2, main->findSubsequence(*sub));
    delete sub;
  }
  {
    int d[] = {44, 55};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(3, main->findSubsequence(*sub));
    delete sub;
  }
  {
    int d[] = {55};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(4, main->findSubsequence(*sub));
    delete sub;
  }
  {
    int d[] = {33, 55};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(-1, main->findSubsequence(*sub));
    delete sub;
  }

  delete main;
}

TEST(LinkedListSequence, findSubsequence) {
  int data[] = {11, 22, 33, 44, 55};
  Sequence<int> *main = new LinkedListSequence<int>(data, _countof(data));
  ASSERT_EQ(5, main->getLength());
  ASSERT_EQ(11, main->get(0));
  ASSERT_EQ(22, main->get(1));
  ASSERT_EQ(33, main->get(2));
  ASSERT_EQ(44, main->get(3));
  ASSERT_EQ(55, main->get(4));

  {
    int d[] = {11, 22};
    Sequence<int> *sub1 = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(0, main->findSubsequence(*sub1));
    delete sub1;
  }
  {
    int d[] = {22, 33};
    Sequence<int> *sub = new LinkedListSequence<int>(d, _countof(d));
    ASSERT_EQ(2, sub->getLength());
    ASSERT_EQ(22, sub->get(0));
    ASSERT_EQ(33, sub->get(1));
    ASSERT_EQ(1, main->findSubsequence(*sub));
    delete sub;
  }
  {
    int d[] = {33, 44, 55};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(2, main->findSubsequence(*sub));
    delete sub;
  }
  {
    int d[] = {44, 55};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(3, main->findSubsequence(*sub));
    delete sub;
  }
  {
    int d[] = {55};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(4, main->findSubsequence(*sub));
    delete sub;
  }
  {
    int d[] = {33, 55};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(-1, main->findSubsequence(*sub));
    delete sub;
  }

  delete main;
}

TEST(LinkedListSequence, operations) {
  auto *list = new LinkedListSequence<int>();  // Список целых чисел
  // Пока что список пуст
  ASSERT_EQ(0, list->getLength());
  // Пытаемся получить первый элемент на пустом списке
  try {
    list->getFirst();
    FAIL();
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("List is empty", ex.what());
  }
  // Пытаемся получить последний элемент на пустом списке
  try {
    list->getLast();
    FAIL();
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("List is empty", ex.what());
  }
  // Добавляем первый элемент
  list->append(11);
  ASSERT_EQ(1, list->getLength());
  ASSERT_EQ(11, list->getFirst());
  ASSERT_EQ(11, list->getLast());
  // Добавляем второй элемент в начало списка
  list->append(22);
  ASSERT_EQ(2, list->getLength());
  ASSERT_EQ(11, list->getFirst());
  ASSERT_EQ(22, list->getLast());
  list->append(33);
  list->append(44);
  // Подсписок извлекаем
  Sequence<int> *sub = list->getSubsequence(1, 2);
  ASSERT_EQ(2, sub->getLength());
  ASSERT_EQ(22, sub->get(0));
  ASSERT_EQ(33, sub->get(1));
  // Добавляем ещё в начало элемент
  sub->prepend(-100);
  ASSERT_EQ(3, sub->getLength());
  ASSERT_EQ(-100, sub->getFirst());
  delete sub;
  delete list;
}

TEST(LinkedListSequence, create_list_from_array) {
  wstring items[3] = {L"first", L"second", L"third"};
  LinkedListSequence<wstring> list(items, 3);
  ASSERT_EQ(3, list.getLength());
  ASSERT_EQ(wstring(L"first"), list.get(0));
  ASSERT_EQ(wstring(L"second"), list.get(1));
  ASSERT_EQ(wstring(L"third"), list.get(2));
  ASSERT_EQ(wstring(L"first"), list[0]);
  ASSERT_EQ(wstring(L"second"), list[1]);
  ASSERT_EQ(wstring(L"third"), list[2]);

  LinkedListSequence<wstring> copy(list);
  ASSERT_EQ(3, copy.getLength());
  ASSERT_EQ(wstring(L"first"), copy.getFirst());
  ASSERT_EQ(wstring(L"first"), copy.get(0));
  ASSERT_EQ(wstring(L"second"), copy.get(1));
  ASSERT_EQ(wstring(L"third"), copy.get(2));
  ASSERT_EQ(wstring(L"third"), copy.getLast());
}

TEST(LinkedListSequence, insertAt) {
  wstring items[3] = {L"first", L"second", L"third"};
  Sequence<wstring> *list = new LinkedListSequence<wstring>(items, 3);
  ASSERT_EQ(3, list->getLength());
  ASSERT_EQ(wstring(L"first"), list->get(0));
  ASSERT_EQ(wstring(L"second"), list->get(1));
  ASSERT_EQ(wstring(L"third"), list->get(2));
  // Вставляем элемент после "first"
  list->insertAt(L"test", 1);
  ASSERT_EQ(4, list->getLength());
  ASSERT_EQ(wstring(L"first"), list->get(0));
  ASSERT_EQ(wstring(L"test"), list->get(1));
  ASSERT_EQ(wstring(L"second"), list->get(2));
  ASSERT_EQ(wstring(L"third"), list->get(3));
  delete list;
}

// Для примера - функция которая возводит числа в квадрат
constexpr int square(int x) {
  return x * x;
};
// Инкремент
constexpr int inc_int(int x) {
  return x + 1;
};
// Декремент
constexpr int dec_int(int x) {
  return x - 1;
};

// == Стек ==
TEST(Stack, stack_basic_operations) {  // Элементы: Целые числа
  static_assert(square(2) == 2 * 2, "2^2 == 4");
  static_assert(inc_int(5) == 6, "5 + 1 == 6");
  static_assert(dec_int(5) == 4, "5 - 1 == 4");

  Stack<int> stack(new LinkedListSequence<int>);
  ASSERT_EQ(0, stack.getLength());

  stack.push(11);
  ASSERT_EQ(1, stack.getLength());
  ASSERT_EQ(11, stack[0]);

  stack.push(10);
  ASSERT_EQ(2, stack.getLength());
  ASSERT_EQ(11, stack[0]);
  ASSERT_EQ(10, stack[1]);

  ASSERT_EQ(10, stack.pop());
  ASSERT_EQ(1, stack.getLength());
  ASSERT_EQ(11, stack[0]);

  stack.push(1);
  stack.push(2);
  ASSERT_EQ(3, stack.getLength());

  Stack<int> *stackSquare = stack.map(square);
  ASSERT_EQ(3, stackSquare->getLength());
  ASSERT_EQ(11 * 11, (*stackSquare)[0]);
  ASSERT_EQ(1 * 1, (*stackSquare)[1]);
  ASSERT_EQ(2 * 2, (*stackSquare)[2]);
  delete stackSquare;

  Stack<int> *stackSquare2 = stack::map(square, stack);
  ASSERT_EQ(3, stackSquare2->getLength());
  ASSERT_EQ(11 * 11, (*stackSquare2)[0]);
  ASSERT_EQ(1 * 1, (*stackSquare2)[1]);
  ASSERT_EQ(2 * 2, (*stackSquare2)[2]);
  delete stackSquare2;
}

// Возведение вещественного (с плавающей точкой) числа в квадрат
double square_double(double x) {
  return x * x;
};

TEST(Stack, float_point) {  // Элементы: Вещественные числа
  Stack<double> stack(new LinkedListSequence<double>());
  ASSERT_EQ(0, stack.getLength());

  stack.push(1.5);
  ASSERT_EQ(1, stack.getLength());
  ASSERT_EQ(1.5, stack[0]);

  stack.push(45.22);
  ASSERT_EQ(2, stack.getLength());
  ASSERT_EQ(1.5, stack[0]);
  ASSERT_EQ(45.22, stack[1]);

  stack.push(6.7);
  ASSERT_EQ(3, stack.getLength());

  Stack<double> *stackSquare = stack.map(square_double);
  ASSERT_EQ(3, stackSquare->getLength());
  ASSERT_EQ(1.5 * 1.5, (*stackSquare)[0]);
  ASSERT_EQ(45.22 * 45.22, (*stackSquare)[1]);
  ASSERT_EQ(6.7 * 6.7, (*stackSquare)[2]);
  delete stackSquare;

  Stack<double> *stackSquare2 = stack::map(square_double, stack);
  ASSERT_EQ(3, stackSquare2->getLength());
  ASSERT_EQ(1.5 * 1.5, (*stackSquare2)[0]);
  ASSERT_EQ(45.22 * 45.22, (*stackSquare2)[1]);
  ASSERT_EQ(6.7 * 6.7, (*stackSquare2)[2]);
  delete stackSquare2;
}

complex<double> square_complex(complex<double> x) {
  return x * x;
};

TEST(Stack, complex_numbers) {  // Элементы: Комплексные числа
  static_assert(1.0i == 1.0i, "Equals");
  static_assert(2.0i != 1.0i, "Not equals");

  constexpr complex<double> z(1.0, 0.0);
  static_assert(z == 1.0, "Real part equals");
  static_assert(1.0 == z, "Real part equals");
  static_assert(2.0 != z, "Not equals");
  static_assert(z != 2.0, "Not equals");
  ASSERT_EQ(1.0, abs(z));  // Абсолютное значение (модуль) комплексного числа
  ASSERT_EQ(0.0, arg(z));

  // чисто мнимое число: 0 + 7-i
  constexpr complex<double> purei(0, 7);
  static_assert(purei.real() == 0.0, "Real");
  static_assert(purei.imag() == 7.0, "Imag");
  // мнимая часть равна 0: 3 + Oi
  constexpr complex<float> real_num(3);
  static_assert(real_num.real() == 3.0, "Real");
  static_assert(real_num.imag() == 0.0, "Imag");

  constexpr complex<double> zz(1.1, 2.2);
  static_assert(zz.real() == 1.1, "Real");
  static_assert(zz.imag() == 2.2, "Imag");
  ASSERT_EQ(1.1, zz.real());
  ASSERT_EQ(2.2, zz.imag());

  Stack<complex<double>> stack(new LinkedListSequence<complex<double>>());
  ASSERT_EQ(0, stack.getLength());

  const complex<double> a = 1.5 + 3i;
  stack.push(a);
  ASSERT_EQ(1, stack.getLength());
  ASSERT_EQ(a, stack[0]);

  complex<double> b = 4.5 + 1.2i;
  stack.push(b);
  ASSERT_EQ(2, stack.getLength());
  ASSERT_EQ(a, stack[0]);
  ASSERT_EQ(b, stack[1]);

  complex<double> c = 6.7 + 5.5i;
  stack.push(c);
  ASSERT_EQ(3, stack.getLength());

  Stack<complex<double>> *stackSquare = stack.map(square_complex);
  ASSERT_EQ(3, stackSquare->getLength());
  ASSERT_EQ(pow(a, 2), (*stackSquare)[0]);
  ASSERT_EQ(pow(b, 2), (*stackSquare)[1]);
  ASSERT_EQ(pow(c, 2), (*stackSquare)[2]);
  delete stackSquare;
}

// Является ли число чётным?
constexpr bool isEven(int x) {
  return (x % 2) == 0;
}

// where фильтрует значения из списка l с помощью функции-фильтра h
TEST(Stack, where) {  // Элементы: Целые числа
  static_assert(isEven(0), "isEven(0)");
  static_assert(!isEven(1), "isEven(1)");
  static_assert(isEven(2), "isEven(2)");
  static_assert(!isEven(3), "isEven(3)");
  static_assert(isEven(4), "isEven(4)");
  static_assert(!isEven(5), "isEven(5)");
  static_assert(isEven(6), "isEven(6)");

  ASSERT_TRUE(isEven(0));
  ASSERT_FALSE(isEven(1));
  ASSERT_TRUE(isEven(2));
  ASSERT_FALSE(isEven(3));
  ASSERT_TRUE(isEven(4));
  ASSERT_FALSE(isEven(5));

  Stack<int> stack(new LinkedListSequence<int>());
  stack.push(1);
  stack.push(2);
  stack.push(3);
  stack.push(10);
  ASSERT_EQ(4, stack.getLength());

  // Все чётные числа
  Stack<int> *even = stack.where(isEven);
  ASSERT_EQ(2, even->getLength());
  ASSERT_EQ(2, (*even)[0]);
  ASSERT_EQ(10, (*even)[1]);
  delete even;

  Stack<int> *even2 = stack::where(isEven, stack);
  ASSERT_EQ(2, even2->getLength());
  ASSERT_EQ(2, (*even2)[0]);
  ASSERT_EQ(10, (*even2)[1]);
  delete even2;
}

// Сумма двух целых чисел
constexpr int sum(int a, int b) {
  return a + b;
}

TEST(Stack, reduce) {  // Элементы: Целые числа
  static_assert(sum(1, 2) == 3, "1 + 2 = 3");
  static_assert(sum(11, 22) == 33, "11 + 22 = 33");
  static_assert(sum(1000, -203) == 797, "1000 - 203 = 797");

  int data[] = {3, 4, 6, 9, 34, 12};
  Stack<int> stack(new LinkedListSequence<int>(data, _countof(data)));

  int res = stack.reduce(sum);
  ASSERT_EQ(3 + 4 + 6 + 9 + 34 + 12, res);

  // Вызываем reduce как функцию
  ASSERT_EQ(3 + 4 + 6 + 9 + 34 + 12, stack::reduce(sum, stack));
}

// Конкатенация
TEST(Stack, concat) {  // Элементы: Целые числа
  int data1[] = {11, 22};
  Stack<int> stack1(new LinkedListSequence<int>(data1, _countof(data1)));
  int data2[] = {33, 44, 55};
  Stack<int> stack2(new LinkedListSequence<int>(data2, _countof(data2)));

  Stack<int> *stack = stack1.concat(stack2);
  ASSERT_EQ(_countof(data1) + _countof(data2), stack->getLength());

  delete stack;
}

// Замеряем время работы стека
double stackImplementationSpeed(Sequence<int> *sequence) {
  auto begin = chrono::steady_clock::now();  // Засекаем начало работы

  Stack<int> stack(sequence);  // Создаём стек
  const int numbers = 10000;   // Добавим числа
  for (int i = 1; i <= numbers; i++) {
    stack.push(i);
  }
  EXPECT_EQ(numbers, stack.getLength());  // Проверяем размер стека (что добавились)

  auto end = chrono::steady_clock::now();  // Конец работы
  auto elapsed_mcs = chrono::duration_cast<chrono::microseconds>(end - begin);
  // Вычисляем разницу в секундах времени начала и окончания работы
  const double t = elapsed_mcs.count() / 1e6;
  // Выводим результат в секундах на экран (в консоль)
  cout << typeid(stack).name() << " time = " << t << endl;
  return t;
}

// Замер скорости работы стека на базе массива
TEST(Stack, push_speed) {
  double listTime = stackImplementationSpeed(new LinkedListSequence<int>);
  double arrayTime = stackImplementationSpeed(new ArraySequence<int>);
  EXPECT_GT(arrayTime, 10 * listTime);  // На основе массива медленнее больше чем в 100 раз
}
