#include "pch.h"
#include "CppUnitTest.h"
#include "/ap_labs/lab_10.2.A/lab_10.2.A/lab_10.2.A.cpp" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestStudentSortSearch
{
	TEST_CLASS(UnitTestStudentSortSearch)
	{
	public:

		// 1. Тест допоміжних функцій Sum та Avg
		TEST_METHOD(TestSumAndAvg)
		{
			Student s = { "Іваненко", 2, KOMP_NAUKY, 5, 4, 3 };

			Assert::AreEqual(12, Sum(s));
			Assert::AreEqual(4.0, Avg(s), 0.001);
		}

		// 2. Тест фізичного сортування (середній бал -> математика -> прізвище за зростанням)
		TEST_METHOD(TestSort)
		{
			const int N = 4;
			Student students[N] = {
				{ "Петренко", 2, INFORMATYKA, 4, 4, 4 },   // сума 12, мат 4
				{ "Іваненко", 1, KOMP_NAUKY,  4, 4, 4 },   // сума 12, мат 4 (має бути перед Петренком за алфавітом)
				{ "Сидоренко", 3, MATEM_EKON, 3, 5, 4 },   // сума 12, мат 5 (має бути після Петренка, бо мат 5 > 4)
				{ "Коваленко", 2, FIZ_INF,    3, 3, 3 }    // сума 9 (має бути найпершим, бо сума 9 < 12)
			};

			Sort(students, N);

			Assert::AreEqual(string("Коваленко"), students[0].prizv);
			Assert::AreEqual(string("Іваненко"), students[1].prizv);
			Assert::AreEqual(string("Петренко"), students[2].prizv);
			Assert::AreEqual(string("Сидоренко"), students[3].prizv);
		}

		// 3. Тест індексного сортування (перевіряємо масив індексів та незмінність вихідного масиву)
		TEST_METHOD(TestIndexSort)
		{
			const int N = 3;
			Student students[N] = {
				{ "Шевченко", 3, TRUD_NAVCH,  5, 5, 5 },   // індекс 0, сума 15
				{ "Бондаренко", 1, KOMP_NAUKY, 3, 4, 3 },  // індекс 1, сума 10
				{ "Мельник", 2, INFORMATYKA,  4, 4, 4 }    // індекс 2, сума 12
			};

			int* I = IndexSort(students, N);

			// Перевіряємо правильний порядок індексів: 1 (сума 10) -> 2 (сума 12) -> 0 (сума 15)
			Assert::AreEqual(1, I[0]);
			Assert::AreEqual(2, I[1]);
			Assert::AreEqual(0, I[2]);

			// Перевіряємо, що сам масив students не змінився
			Assert::AreEqual(string("Шевченко"), students[0].prizv);

			delete[] I;
		}

		// 4. Тест бінарного пошуку (успішний пошук та випадок, коли студента не знайдено)
		TEST_METHOD(TestBinSearch)
		{
			const int N = 4;
			// Масив уже впорядкований за критеріями функції Sort
			Student students[N] = {
				{ "Коваленко", 2, FIZ_INF,    3, 3, 3 },   // позиція 0: сума 9, мат 3
				{ "Іваненко", 1, KOMP_NAUKY,  4, 4, 4 },   // позиція 1: сума 12, мат 4
				{ "Петренко", 2, INFORMATYKA, 4, 4, 4 },   // позиція 2: сума 12, мат 4
				{ "Сидоренко", 3, MATEM_EKON, 3, 5, 4 }    // позиція 3: сума 12, мат 5
			};

			// Пошук існуючого студента (Петренко: сума 12, мат 4) -> очікуємо індекс 2
			int foundIdx = BinSearch(students, N, "Петренко", 12, 4);
			Assert::AreEqual(2, foundIdx);

			// Пошук неіснуючого студента -> очікуємо -1
			int notFoundIdx = BinSearch(students, N, "Ткаченко", 12, 4);
			Assert::AreEqual(-1, notFoundIdx);

			// Пошук існуючого прізвища, але з іншою оцінкою з математики -> очікуємо -1
			int wrongGradeIdx = BinSearch(students, N, "Петренко", 12, 5);
			Assert::AreEqual(-1, wrongGradeIdx);
		}
	};
}