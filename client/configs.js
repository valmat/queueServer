module.exports = {
    server: {
        host: '192.168.1.43',
        port: 5591,
    },
    // Префиксы ключей сервера очередей
    prefixes: {
        created  : 'created:',  // Запись создана, но ещё не обработана
        started  : 'start:',    // Запись поставлена в обработку, но ещё не обработана
        finished : 'finish:',   // Запись обработана
        stderr   : 'stderr:',   // stderr сгенерированный командой
        stdout   : 'stdout:',   // stdout сгенерированный командой
        famaly   : 'test:'      // Семейство ключей. Этот префикс используется при очистке. Если он пустой, то очищаются все ключи
    },
    // Опции заполнения
    fill: {
        // Читает этот файл
        file     : '__cmds.txt',
        // применяет к каждой строке файла эту функцию и результат записывает в сервер очередей
        // Ключёь будет `${created}${line_nom}`, значением результат функции
        process: function(line_nom, line) {
            return {
                index: line_nom,
                value: JSON.stringify(line.split(' '), null, '')
            };
        }
    },
    // Опции запуска процессов
    run: {
        // звращает команду с этим префиксом
        // Баш выполняет команды до тех пор, пока есть префикс
        next_prefix: 'next:',
        // Функция выполняется когда кончились ключи или что-то пошло не так
        done: function() {
            console.log('done');
        },
        // Преобразует запись в БД в выполняемую команду
        next: function(pair) {
            return `./__test_cmd.js ${pair.key} ${JSON.parse(pair.value).join(' ')}`;
        },
        // Функция вызывается на этап завершения
        // Например в ней можно забирать посчитанную модель
        finish: function(index, value) {
            console.log({index, value});
        }
    },
    misc: {
        // Префикс для временных файлов
        tmp_prefix: '/tmp/RUNCMDS'
    }
};
