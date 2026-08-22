# the number as file descriptor
# 0: keyboard input (standard input)
# 1: output (standard output)
# 2: error ouptut (standard error)

if [ -z "$1" ]; then
  echo "give me the fucking name."
  return 1 2>/dev/null || exit 1
  # return 1 (standard output)
  # return 2>/dev/null (return error into the abyss, silencing it)
  # || exit 1 (in case the prior 2 cmd fail, exit 1, prevent the cmd window from shutdown)
fi # like if but reversed?

if [ ! -f "$1" ]; then
  echo "file not found."
  return 1 2>/dev/null || exit 1
fi

gcc *.c -I . -o prog
./prog

if [ "$2" = "d" ]; then
  echo "Delete the prog file..."
  rm -rf prog
fi
