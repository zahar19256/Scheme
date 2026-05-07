# Scheme Basic Interpreter

Небольшой C++-проект: интерпретатор базового подмножества Scheme. В проекте реализованы tokenizer, parser, построение AST и блок вычислений.

## Поддержаны:

- числа и булевы значения
- символы и quoted expressions
- пары и списки
- арифметические операции: `+`, `-`, `*`, `/`, `min`, `max`, `abs`
- числовые предикаты: `=`, `<`, `<=`, `>`, `>=`, `number?`
- булевы операторы: `boolean?`, `not`, `and`, `or`
- операции со списками: `pair?`, `null?`, `list?`, `cons`, `car`, `cdr`, `list`, `list-ref`, `list-tail`
- Scheme-style ошибки: `SyntaxError`, `NameError`, `RuntimeError`

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

## Запуск тестов

```bash
./build/test_scheme_basic
```

Тесты покрывают tokenizer/parser, целочисленные и булевы вычисления, операции со списками. Также есть fuzzing-тесты.

## Запуск REPL

```bash
./build/scheme_basic_repl
```

Пример:

```scheme
> (+ 1 2 3)
6
> '(1 2 . 3)
(1 2 . 3)
```
