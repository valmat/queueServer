#!/usr/bin/env bash

cd "$(dirname "$0")"

CONFIG="configs.js"

if [[ "$1" != "" ]]; then
    CONFIG="$1"
fi


if ! [[ -f "$CONFIG" ]]; then
    echo "File \"$CONFIG\" not found" 1>&2
    exit 1;
fi

# The prefix for temporary files
tmp_prefix=`./get_conf_field.js misc.tmp_prefix $CONFIG`
next_prefix=`./get_conf_field.js run.next_prefix $CONFIG`
next_prefix_len=${#next_prefix}

# Получает первую комманду
cmd=`./gen_cmd.js $CONFIG`
while [ "${cmd:0:next_prefix_len}" == "$next_prefix" ]; do
    # Непосредственно выполняемая команда
    to_run=${cmd:next_prefix_len}
    # индекс команды в БД
    index="${to_run%%:*}"
    index_end_pos="${#index}"
    let "index_end_pos=index_end_pos+1"
    to_run=${to_run:index_end_pos}

    # Temporary files for output streams
    err_file="${tmp_prefix}_err_${index}"
    out_file="${tmp_prefix}_out_${index}"

    # run command
    echo "[$index] -> [$to_run]"
    $($to_run 1> "$out_file" 2> "$err_file")

    # Завершаем выполнение
    ./finish.js "$index" "$out_file" "$err_file" "$CONFIG"
    rm "$out_file"
    rm "$err_file"

    # get next command
    cmd=`./gen_cmd.js $CONFIG`
done

echo "DONE"
