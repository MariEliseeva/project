
## Комментарии ко второй попытке
### Про тесты
make test - собирает юнит-тесты<br>
./bin/test - запуск юнит-тестов. <br>
Если тесты проходят, то будет выведено "All tests passed.", <br>
иначе - сколько тестов не прошли, сколько всего тестов было, где именно произошла ошибка.<br>
<br>
Также есть два теста, проверяющих программу целиком: первый проверяет выполняются ли основные команды микроконтроллеров, второй считает число Фибоначчи.<br>
Их запуск (после сборки - make):<br>
./avr -test f_test/input<br>
./avr -test f_test/input_fib<br>
После каждого запуска в файле f_test/output появляется результат выполнения программы.<br>
Ожидаемый результат для первого теста - f_test/output_res, для второго - f_test/output_fib_res.<br>
### Про pull request
Работающий код находится на ветке branch_with_project<br>
Сделан pull request от это ветки к пустой - empty_branch<br>
