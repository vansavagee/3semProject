#ifndef _MATHEXPR_H
#define _MATHEXPR_H

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<float.h>

#define atanl atan
#define asinl asin
#define acosl acos
#define expl exp
#define logl log
#define powl pow
#define pow10l(x) pow(10,x)
#define fabsl fabs
#define cosl cos
#define sinl sin
#define tanl tan
#define fmodl fmod
#define sqrtl sqrt


const double ErrVal=DBL_MAX;

/*! \defgroup math_module Модуль для разбора и вычисления математических выражений заданных строкой */
/**@{*/

/**
 * \brief клас хранит переменные для формул
 * */
class RVar{
 public:
  /// название переменной
  char*name;
  /// указател на переменную
  double*pval;
  /*!
     \brief конструктор
     \param name - имя переменной
     \param pval - переменная
  */
  RVar(){name=NULL;pval=NULL;};
  /*!
     \brief копирующий конструктор
     \param RVar - объект класса RVar
  */
  RVar(const RVar&);
  /*!
     \brief конструктор
     \param char* - имя переменной
     \param double* - переменная
  */
  RVar(const char*,double*);
  /// деструктор
  ~RVar();
  /// оператор сравнения
  friend int operator==(const RVar&,const RVar&);
};
typedef RVar* PRVar;
/// перечисление - хранит возможные математические операции
enum ROperator{ErrOp,Juxt,Num,Var,Add,Sub,Opp,Mult,Div,Pow,Sqrt,
	       NthRoot,Abs,Sin,Cos,Tg,Ln,Exp,Acos,Asin,Atan,E10,Fun};

typedef void ((*pfoncld)(double*&));

class ROperation;
typedef ROperation* PROperation;
class RFunction;
typedef RFunction* PRFunction;

/**
 * \brief класс обрабатывает строки с математическими выражениями
 * 
 * разбирает строки с математическими выражениями, подставляет значения в переменные и вычисляет значения
 * */
class ROperation{
  pfoncld*pinstr;double**pvals;double*ppile;RFunction**pfuncpile;
  mutable signed char containfuncflag;
  void BuildCode();
	void Destroy();
 public:
  ROperator op;
  PROperation mmb1,mmb2;
  double ValC;
  const RVar* pvar;
  double*pvarval;
  RFunction* pfunc;
  /*!
     \brief конструктор по умолчанию
  */
  ROperation();
  /*!
     \brief копирующий конструктор
  */
  ROperation(const ROperation&);
  /*!
     \brief конструктор 
     \param double - значение
  */
  ROperation(double);
  /*!
     \brief конструктор 
     \param RVar - объект класса RVar
  */
  ROperation(const RVar&);
  /*!
     \brief конструктор 
     \param sp - строка с выражением
     \param nvarp - количество переменных
     \param sp - массив переменных
     \param nfuncp - количество функций
     \param ppfuncp - массив функций
  */
  ROperation(char*sp,int nvarp=0,PRVar*ppvarp=NULL,int nfuncp=0,PRFunction*ppfuncp=NULL);
  /// деструктор
  ~ROperation();
  /// созвращает вычислнное значение функции
  double Val() const;
  /*!
     \brief проверяет содержит ли выражение переменную
     \param RVar - переменная
  */
  signed char ContainVar(const RVar&) const;
  /*!
     \brief проверяет содержит ли выражение функцию
     \param RFunction - функция
  */
  signed char ContainFunc(const RFunction&) const;
  /*!
     \brief проверяет содержит ли выражение рекурсивную функцию
     \param RFunction - функция
  */
  signed char ContainFuncNoRec(const RFunction&) const; // No recursive test on subfunctions
  /*!
     \brief возвращает указанную по счету операцию в выражении
     \param int - номер
     \return ROperation
  */
  ROperation NthMember(int) const;int NMembers() const;
  /*!
     \brief возвращает ошибку, если она есть
     \param ROperation - объект выражения
     \return signed char
  */
  signed char HasError(const ROperation* =NULL) const;
  ROperation& operator=(const ROperation&);
  friend int operator==(const ROperation& ,const double);
  friend int operator==(const ROperation& ,const ROperation&);
  friend int operator!=(const ROperation& ,const ROperation&);
  ROperation operator+() const;ROperation operator-() const;
  friend ROperation operator,(const ROperation&,const ROperation&);
  friend ROperation operator+(const ROperation&,const ROperation&);
  friend ROperation operator-(const ROperation&,const ROperation&);
  friend ROperation operator*(const ROperation&,const ROperation&);
  friend ROperation operator/(const ROperation&,const ROperation&);
  friend ROperation operator^(const ROperation&,const ROperation&);  // Caution: wrong associativity and precedence
  friend ROperation sqrt(const ROperation&);
  friend ROperation abs(const ROperation&);
  friend ROperation sin(const ROperation&);
  friend ROperation cos(const ROperation&);
  friend ROperation tan(const ROperation&);
  friend ROperation log(const ROperation&);
  friend ROperation exp(const ROperation&);
  friend ROperation acos(const ROperation&);
  friend ROperation asin(const ROperation&);
  friend ROperation atan(const ROperation&);
  friend ROperation ApplyOperator(int,ROperation**,ROperation (*)(const ROperation&,const ROperation&));
  /*!
     \brief дифференцирует по переменной
     \param RVar - переменная
     \return ROperation
  */
  ROperation Diff(const RVar&) const;
   /*!
     \brief возвращает выражение после обработки
     \return char*
  */
  char* Expr() const;
   /*!
     \brief подставляет переменную строку с выражением
     \param RVar - переменная
     \param ROperation - выражение
     \return ROperation
  */
  ROperation Substitute(const RVar&,const ROperation&) const;
};

/**
 * \brief класс для работы с функциями
 * 
 * использует введенные выражения как отдельные функции
 * */
class RFunction{
  double*buf;
public:
  /// тип функциии
  signed char type;
  /// указатель на функцию
  double ((*pfuncval)(double));
  ROperation op;
  int nvars;
  RVar** ppvar;
  char*name;
  /*!
     \brief конструктор по умолчанию
  */
  RFunction();
  /*!
     \brief конструктор 
     \param - указатель на функцию
  */
  RFunction(double ((*)(double)));
  /*!
     \brief конструктор 
     \param ROperation - выражение
     \param RVar - переменная
  */
  RFunction(const ROperation& opp,RVar* pvarp);
  /*!
     \brief конструктор 
     \param ROperation - выражение
     \param nvarsp - количество переменных
     \param RVar - массив  переменных
  */
  RFunction(const ROperation& opp,int nvarsp,RVar**ppvarp);
  /*!
     \brief копирующий конструктор
  */
  RFunction(const RFunction&);
  /// деструктор
  ~RFunction();
  RFunction& operator=(const RFunction&);
  /*!
     \brief задает имя 
     \param s - имя
  */
  void SetName(const char*s);
  /*!
     \brief задает значение 
     \param double - значение
  */
  double Val(double) const;
  /*!
     \brief задает массив значений 
     \param double - массив значений
  */
  double Val(double*) const;
  friend int operator==(const RFunction&,const RFunction&);
  ROperation operator()(const ROperation&);
};

/*!
     \brief выделяет подстроку из строки в новую строку
     \param s - строка
     \param i1 - начало подстроки
     \param i2 - конец подстроки
     \return char*
*/
char* MidStr(const char*s,int i1,int i2);
/*!
     \brief копирование строка
     \param s - строка
     \return char*
*/
char* CopyStr(const char*s);
/*!
     \brief проверка находится ли символ в строке
     \param s - строка
     \param n - построка которая проверяется
     \param c - символ
     \return char*
*/
char* InsStr(const char*s,int n,char c);
/*!
     \brief проверка равенства строк
     \param s - строка
     \param s2 - строка
     \return signed char
*/
signed char EqStr(const char*s,const char*s2);
/*!
     \brief сравнение части строки
     \param s - строка
     \param n - подстрока
     \param s2 - строка
     \return signed char
*/
signed char CompStr(const char*s,int n,const char*s2);
/*!
     \brief удалегие части строки
     \param s - строка
     \param n - подстрока
     \return char*
*/
char* DelStr(const char*s,int n);

/**@}*/

#endif
