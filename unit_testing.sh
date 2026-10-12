echo "unit testing"

DIR_NAME="./tests/"
DIR="./tests"

# input $1: file_name
run_test_file()
{
    
}

run_test() 
{
    echo "hello world"
    for file in $DIR/*.c; do
        printf '%s\n' "$file"
    done
}

fail_report()
{
    echo "failed to find $DIR_NAME"
}


if [ -d $DIR_NAME ]; then
    run_test 
else
    fail_report
fi