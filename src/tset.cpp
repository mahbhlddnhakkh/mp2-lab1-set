// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField()
{
  return TBitField(BitField);
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
  return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
  if (Elem >= MaxPower)
    return false;
  return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
  if (Elem >= MaxPower)
    throw out_of_range("Elem is out of universe");
  BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
  if (Elem >= MaxPower)
    throw out_of_range("Elem is out of universe");
  BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
  BitField = s.BitField;
  MaxPower = s.MaxPower;
  return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
  return BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
  return BitField != s.BitField;
}

TSet TSet::operator+(const TSet &s) // объединение
{
  return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
  TSet tmp(BitField);
  tmp.InsElem(Elem);
  return tmp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
  TSet tmp(BitField);
  tmp.DelElem(Elem);
  return tmp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
  return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
  return TSet(~BitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
  const int IN_BUFFER = 100;
  int temp_num[IN_BUFFER];
  char c = '0';
  int k = 1, num = 0, size = 0;
  s = TSet(1);

  while (c && c != 10)
  {
    istr.get(c);
    if (c == ',' || !c || c == 10)
    {
      if (size == 0)
        throw logic_error("Bad input, can't have null character in set");
      for (int i = size - 1; i >= 0; i--)
      {
        num += temp_num[i] * k;
        k *= 10;
      }
      if (s.MaxPower <= num)
        s = s + TSet(num + 1);
      s.InsElem(num);
      k = 1;
      num = 0;
      size = 0;
    }
    else
    {
      if (c < '0' || c > '9')
        throw logic_error("Bad input, invalid character, must be a number");
      if (size >= IN_BUFFER)
        throw overflow_error("Number is too long");
      temp_num[size++] = c - '0';
    }
  }
  return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
  ostr << '{';
  bool found_first = false;
  for (int i = 0; i < s.MaxPower; i++)
    if (s.IsMember(i))
    {
      if (found_first)
        ostr << ',';
      found_first = true;
      ostr << ' ' << i;
    }
  ostr << ' ' << '}';
  return ostr;
}
