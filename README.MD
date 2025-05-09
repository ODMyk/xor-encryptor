# XOR Encryptor — Parallel Implementations

Цей проєкт демонструє три варіанти реалізації XOR-шифрування:

- Звичайна послідовна версія (`basic.cpp`)
- OpenMP версія для багатопотоковості (`openmp.cpp`)
- MPI версія для розподілених обчислень (`mpi.cpp`)

## 📁 Структура проєкту

```
├── .gitignore
├── Makefile
├── src
│   ├── basic.cpp
│   ├── openmp.cpp
│   └── mpi.cpp
├── dist/                  # Створюється після компіляції
├── io
│   ├── key.txt
│   ├── data.txt
│   └── output.txt         # Створюється після запуску
└── ReadMe.md
```

## 🚀 Клонування проєкту

```bash
git clone https://github.com/ODMyk/xor-encryptor.git
cd xor-encryptor
```

## 🛠️ Встановлення Make на macOS (якщо потрібно)

Make зазвичай встановлений разом із Xcode Command Line Tools. Якщо його немає:

```bash
xcode-select --install
```

## ⚙️ Компіляція

У репозиторії використовується `Makefile`, що дозволяє зручно компілювати будь-яку з версій:

```bash
make basic    # Компілює src/basic.cpp -> dist/basic
make openmp   # Компілює src/openmp.cpp -> dist/openmp
make mpi      # Компілює src/mpi.cpp -> dist/mpi
```

### Залежності для OpenMP

Щоб зібрати OpenMP версію, необхідно встановити бібліотеку OpenMP. Для macOS це можна зробити так:

```bash
brew install libomp
```

Якщо шлях до бібліотеки відрізняється, можна вказати змінні в Makefile. Для macOS це може бути:

```make
LIBOMP_PREFIX = /opt/homebrew/opt/libomp
OPENMP_FLAG = -Xpreprocessor -fopenmp -I$(LIBOMP_PREFIX)/include -L$(LIBOMP_PREFIX)/lib -lomp
```

Замість $(LIBOMP_PREFIX) використовуйте актуальний шлях до бібліотеки, якщо він інший. Знайти його можна наступною командою:

```bash
brew --prefix libomp
```

### Залежності для MPI

Для використання MPI на macOS, зазвичай достатньо встановити бібліотеку через Homebrew:

```bash
brew install open-mpi
```

## ▶️ Запуск

### basic:

```bash
./dist/basic io/key.txt io/data.txt io/output.txt
```

### openmp (з можливістю вказати кількість потоків):

```bash
./dist/openmp io/key.txt io/data.txt io/output.txt 8   # 8 потоків
```

### mpi:

```bash
mpirun -np 4 ./dist/mpi io/key.txt io/data.txt io/output.txt
```

## 📎 Вхідні файли

Усі необхідні приклади `key.txt` та `data.txt` вже знаходяться у папці `io`. Тому ви можете одразу запустити програму.

## 📤 Результат

Файл `output.txt` з'явиться у папці `io` після запуску однієї з програм. За замовчуванням саме цей шлях використовується для збереження результату.

## 🪪 License

Цей проєкт ліцензовано за умовами MIT License.
