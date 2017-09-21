#!/bin/bash

# Префикс для временных файлов
tmp_prefix=`./get_conf_field.js misc.tmp_prefix`
next_prefix=`./get_conf_field.js run.next_prefix`
next_prefix_len=${#next_prefix}

# Получает первую комманду
cmd=`./gen_cmd.js`
while [ "${cmd:0:next_prefix_len}" == "$next_prefix" ]; do
    # Непосредственно выполняемая команда
    to_run=${cmd:next_prefix_len}
    # индекс команды в БД
    index="${to_run%%:*}"
    index_end_pos="${#index}"
    let "index_end_pos=index_end_pos+1"
    to_run=${to_run:index_end_pos}

    # Временные файлы для выходных потоков
    err_file="${tmp_prefix}_err_${index}"
    out_file="${tmp_prefix}_out_${index}"

    # run command
    echo "[$index] -> [$to_run]"
    $($to_run 1> "$out_file" 2> "$err_file")

    # Завершаем выполнение
    ./finish.js "$index" "$out_file" "$err_file"
    rm "$out_file"
    rm "$err_file"

    # get next command
    cmd=`./gen_cmd.js`
done

echo "DONE"
